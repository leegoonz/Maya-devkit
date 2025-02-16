//-
// ==========================================================================
// Copyright 2015 Autodesk, Inc.  All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

//	File Name: narrowPolyViewer.cpp
//
//	Description:
//		A simple test of the MPx3dModelView code.
//		A view that allows multiple cameras to be added is made.
//

#include "narrowPolyViewer.h"

#include <maya/MItDag.h>
#include <maya/MStringArray.h>
#include <maya/MGlobal.h>

#include <maya/MPoint.h>
#include <maya/MSelectionList.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnMesh.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MPointArray.h>
#include <maya/MIOStream.h>

#include <maya/MGL.h>

#include <math.h>

narrowPolyViewer::narrowPolyViewer()
:	fDrawManips(true)
,	fLightTest(false)
{
	setMultipleDrawEnable(true);
	tol = 10;
}

narrowPolyViewer::~narrowPolyViewer()
{
}

MStatus narrowPolyViewer::setCameraList(const MDagPathArray &cameraList)
//
//	Description:
//		Replace the contents of fCameraList with the passed list of cameras.
//
{
	setMultipleDrawEnable(true);
	fCameraList.clear();

	unsigned ii = 0;
	unsigned nCameras = cameraList.length();
	for (ii = 0; ii < nCameras; ++ii) {
		fCameraList.append(cameraList[ii]);
	}

	refresh();

	return MS::kSuccess;
}

MStatus narrowPolyViewer::getCameraList(MDagPathArray &cameraList) const
//
//	Description:
//		Fills the passed camera list with the current camera list used by
//		this view. The default camera is not added to the list. Also, the list 
//		is cleared of any previous values.
//
{
	cameraList.clear();

	unsigned ii = 0;
	unsigned nCameras = fCameraList.length();
	for (ii = 0; ii < nCameras; ++ii) {
		cameraList.append(fCameraList[ii]);
	}

	return MS::kSuccess;
}

MStatus narrowPolyViewer::getCameraList(MStringArray &cameraList) const
//
//	Description:
//		Fills the passed camera list with the current camera list used by
//		this view. The default camera is not added to the list. Also, the list 
//		is cleared of any previous values.
//
{
	cameraList.clear();

	unsigned ii = 0;
	unsigned nCameras = fCameraList.length();
	for (ii = 0; ii < nCameras; ++ii) {
		cameraList.append(fCameraList[ii].partialPathName());
	}

	return MS::kSuccess;
}

MStatus narrowPolyViewer::appendCamera(const MDagPath &camera)
//
//	Description:
//		Append a camera to the list of cameras used by this view.
//
{
	MStatus ReturnStatus = fCameraList.append(camera);
	refresh(true);

	return ReturnStatus;
}

MStatus narrowPolyViewer::removeCamera(const MDagPath &camera)
//
//	Description:
//		Removes the specified camera from the list of cameras. If the
//		camera is not on the list, it is still considered removed.
//
{
	//	Loop backwards to avoid any array resize problems.
	//
	int ii = 0;
	bool cameraRemoved = false;
	int nCameras = (int)fCameraList.length();
	for (ii = nCameras - 1; ii >= 0; ii--) {
		if (camera == fCameraList[ii]) {
			fCameraList.remove(ii);
			cameraRemoved = true;
		}
	}

	if (cameraRemoved) {
		refresh();
	}
	return MS::kSuccess;
}

MStatus narrowPolyViewer::removeAllCameras()
//
//	Description:
//		Remove all cameras from the list.
//	
{
	MStatus ReturnStatus = fCameraList.clear();
	refresh();
	return ReturnStatus;
}

MString narrowPolyViewer::getCameraHUDName()
{
	MString hudName("narrowPolyViewer: ");

	//	Oops, well this method should be non-const, or there
	//	should be a const version of get camera. For now, we will
	//	live with a cast.
	//
	MDagPath cameraPath;
	getCamera(cameraPath);

	cameraPath.pop();
	hudName += cameraPath.partialPathName();
	return hudName;
}

MStatus narrowPolyViewer::setIsolateSelect(MSelectionList &list) 
{
	setViewSelected(true);
	return setObjectsToView(list);
}

MStatus narrowPolyViewer::setIsolateSelectOff()
{
	return setViewSelected(false);
}

MStatus narrowPolyViewer::setLightTest(MSelectionList &list)
{
	MStatus ReturnStatus = MS::kSuccess;
	MItSelectionList it(list, MFn::kLight, &ReturnStatus);
	if (MS::kSuccess != ReturnStatus) {
		return ReturnStatus;
	}

	for (; !it.isDone(); it.next()) {
		MDagPath lightPath;
		if (MS::kSuccess != it.getDagPath(lightPath)) {
			continue;
		}

		fLightList.append(lightPath);
	}

	if (fLightList.length() > 0) {
		fLightTest = true;
	}

	return ReturnStatus;
}

void narrowPolyViewer::preMultipleDraw()
{
	fCurrentPass = 0;
	fDrawManips = false;

	MDagPath oldCamera;
	MStatus status = getCamera(oldCamera);

	if (MS::kSuccess != status) {
		status.perror("M3dView::getCamera");
	}

	fOldCamera = oldCamera;

	displayHUD();

	//	Clear the test camera list
	//
	fTestCameraList.clear();

//////////////////////////////////////////////////////////////////////////////////////////////////////

	// get the first item in the selection list
	MStatus stat;
	MSelectionList sList;
	MGlobal::getActiveSelectionList(sList);
	MDagPath dagPath;
	stat = sList.getDagPath(0, dagPath);
	if (stat != MS::kSuccess) return;
	MItMeshPolygon itMeshPolygon(dagPath, MObject::kNullObj, &stat);
	if (stat != MS::kSuccess) return;

	beginGL();

	// Iterate over all faces on the mesh
	for (; !itMeshPolygon.isDone(); itMeshPolygon.next())
	{
		MPointArray points;
		itMeshPolygon.getPoints(points, MSpace::kWorld, &stat);
		int length = points.length();

		// Only care about triangles
		if (length == 3)
		{
			// Iterate over all points on the triangle face and if the angle
			// between any 2 sides are less than the tolerance, then draw the 
			// triangle white
			for (int i=0; i<length; i++)
			{
				// get the points of the triangle
				MPoint p = points[i];
				MPoint p1 = points[(i+1)%length];
				MPoint p2 = points[(i+2)%length];

				// create the 2 adjacent vectors to the current point
				MVector v1(p1 - p);
				MVector v2(p2 - p);

				// get the angle between the two vectors in degrees
				double angle = v1.angle(v2) * 180 / 3.14159;
				
				// Dump some data
				#if 0
				MString astr = "The angle is : ";
				astr += angle;
				astr += " tolerance : ";
				astr += tol;
				MGlobal::displayInfo( astr );
				#endif

				// if angle less than tolerance then draw the triangle white
				if ( (fabs( angle - tol ) < .0001) || angle < tol )
				{
					glBegin(GL_POLYGON);
						glVertex3f((float)points[0].x, (float)points[0].y, (float)points[0].z);
						glVertex3f((float)points[1].x, (float)points[1].y, (float)points[1].z);
						glVertex3f((float)points[2].x, (float)points[2].y, (float)points[2].z);
					glEnd();
					break;
				}
			}
		}
	}

	endGL();
//////////////////////////////////////////////////////////////////////////////////////////////////////
}

void narrowPolyViewer::postMultipleDraw()
{
	MStatus status = setCamera(fOldCamera);
	fDrawManips = true;

	updateViewingParameters();
	if (MS::kSuccess != status) {
		status.perror("M3dView::setCamera");
	}
}

void narrowPolyViewer::preMultipleDrawPass(unsigned index)
{
	fCurrentPass = index;
	MStatus status;

	if (MS::kSuccess != (status = setDisplayAxis(false))) {
		status.perror("M3dView::setDisplayAxis");
	}
	if (MS::kSuccess != (status = setDisplayAxisAtOrigin(false))) {
		status.perror("M3dView::setDisplayAxisAtOrigin");
	}
	if (MS::kSuccess != (status = setDisplayCameraAnnotation(false))) {
		status.perror("M3dView::setDisplayCameraAnnotation");
	}

	MDagPath dagPath;

	//	Draw the main camera first.
	//
	if (fCurrentPass == 0) {
		getCamera(dagPath);
	} else {
		unsigned nCameras = fCameraList.length();
		if (fCurrentPass <= nCameras) {
			dagPath = fCameraList[fCurrentPass-1];
		} else {
			cerr << "Error ... too many passes specified: " 
				 << fCurrentPass << endl;
			return;
		}
	}

	if (MS::kSuccess != (status = setCameraInDraw(dagPath))) {
		status.perror("M3dView::setCamera");
		return;
	}

	//	Add the camera to the list of cameras used.
	//
	MString thisCameraPath = dagPath.partialPathName();
	fTestCameraList.append(thisCameraPath);

	//	Turn on the display of everything, then selectively disable
	//	things.
	//
	setObjectDisplay(M3dView::kDisplayEverything, true);

	//	Only draw manips and grids for the main cameras.
	//	Also do not draw lights, cameras, ikHandles, dimensions,
	//	and selection handles for non-main cameras.
	//
	if (dagPath == fOldCamera) {
		fDrawManips = true;
		setObjectDisplay(M3dView::kDisplayGrid, true);

		//	For testing purposes, verify that the display grid is
		//	properly set.
		//
		if (!objectDisplay(M3dView::kDisplayGrid)) {
			MGlobal::displayError("objectDisplay kDisplayGrid should be true!");
		}

		//	Add fog for the main camera
		//
		if (MS::kSuccess != (status = setFogEnabled(true))) {
			status.perror("M3dView::setFogEnabled");
		} 

		if (!isFogEnabled()) {
			MGlobal::displayError("setFogEnabled did not work!");
		}

		//	Turn off background fog. If not set, background fog would be
		//	used.
		//
		setBackgroundFogEnabled(false);

		setObjectDisplay(M3dView::kDisplayLights, true);
		setObjectDisplay(M3dView::kDisplayCameras, true);
		setObjectDisplay(M3dView::kDisplayIkHandles, true);
		setObjectDisplay(M3dView::kDisplayDimensions, true);
		setObjectDisplay(M3dView::kDisplaySelectHandles, true);

		//	Draw some text in the main view.
		//
		MPoint textPos(0, 0, 0);
		MString str("Main View");
		drawText(str, textPos, M3dView::kLeft);
	} else {
		fDrawManips = false;
		setObjectDisplay(M3dView::kDisplayGrid, false);

		//	For testing purposes, verify that the display grid is
		//	properly set.
		//
		if (objectDisplay(M3dView::kDisplayGrid)) {
			MGlobal::displayError(
				"objectDisplay kDisplayGrid should be false!");
		}

		if (MS::kSuccess != (status = setFogEnabled(false))) {
			status.perror("M3dView::setFogEnabled");
		}
		if (isFogEnabled()) {
			MGlobal::displayError("setFogEnabled did not work!");
		}

		setObjectDisplay(M3dView::kDisplayLights, false);
		setObjectDisplay(M3dView::kDisplayCameras, false);
		setObjectDisplay(M3dView::kDisplayIkHandles, false);
		setObjectDisplay(M3dView::kDisplayDimensions, false);
		setObjectDisplay(M3dView::kDisplaySelectHandles, false);
	}
	
	if (fLightTest) {
		unsigned step = 4;
		unsigned min = 0 + (fCurrentPass * step);
		unsigned max = (fCurrentPass * step);

		if (max > 0) {
			max--;
		}

		unsigned nLights = fLightList.length();
		if (nLights < min) {
			setLightingMode(kLightDefault);
		} else {
			if (nLights < max) {
				max = nLights;
			}

			MSelectionList list;
			unsigned ii = 0;
			for (ii = min; ii < max; ii++) {
			list.add(fLightList[ii].node());
			}

			MGlobal::setActiveSelectionList(list);
			setLightingMode(kLightSelected);
		}
	} else {
		setLightingMode(kLightDefault);
	}

	//	Make every other pass alternate between shaded and wireframe.
	//
	if (fCurrentPass % 2 == 0) {
		setObjectDisplay(M3dView::kDisplayNurbsSurfaces, true);
		setObjectDisplay(M3dView::kDisplayNurbsCurves, true);
	}

	updateViewingParameters();
}

void narrowPolyViewer::postMultipleDrawPass(unsigned index)
{
	//	Until better control over selection and picking is exposed,
	//	turn on the display of everything after drawing. The selection
	//	methods will not consider things that are not visible.
	//
	//	If picking masks are to be used for the view, set them
	//	here. 
	//
	setObjectDisplay(M3dView::kDisplayEverything, true);
}

bool narrowPolyViewer::okForMultipleDraw(const MDagPath &dagPath)
{
	// Don't draw manipulators unless the fDrawManips value is true.
	//
	if (!fDrawManips && dagPath.hasFn(MFn::kManipulator3D)) {
		return false;
	}

	return true;
}

unsigned narrowPolyViewer::multipleDrawPassCount()
{
	//	The camera list plus the main camera.
	//
	return fCameraList.length() + 1;
}

void narrowPolyViewer::removingCamera(MDagPath &cameraPath)
//
//	Description:
//		A method that is called when the passed camera path is
//		being deleted. Be sure to reemove any call to this 
//		camera.
//
{
	int ii = 0;
	int nCameras = (int)fCameraList.length();
	for (ii = nCameras - 1; ii >= 0; ii--) {
		if (cameraPath == fCameraList[ii]) {
			fCameraList.remove(ii);
		}
	}
}

void * narrowPolyViewer::creator()
{
	return new narrowPolyViewer();
}

MStatus narrowPolyViewer::copy(MPx3dModelView &src)
//
//	Description:
//		Copy the contents for the src and put them 
//		into this.
//
{
	return MS::kSuccess;
}

MStatus narrowPolyViewer::swap(MPx3dModelView &src)
//
//	Description:
//		Swap out the contents for the src and put them 
//		into this.
//		
{
	return MS::kSuccess;
}

MString narrowPolyViewer::viewType() const
//
//	Description:
//		Returns a string with the type of the view.
//
{
	return MString("narrowPolyViewer");
}
