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

#ifdef WIN32
#pragma warning( disable : 4786 )
#endif

#include <maya/MIOStream.h>
#include <maya/MDagPath.h>
#include <maya/MDoubleArray.h>
#include <maya/MFloatVector.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MString.h>

#include "blindDataShader.h"

void mergeSort( int startIndex, int sortingOffset,
			    MIntArray& sortingArray, int* tempSortingArray,
				MDoubleArray& otherArray, double* tempOtherArray)
//
// Description:
// This is a very dirty recursive implementation of the merge sort
// that is very application-specific
//
{
	if (sortingOffset == 2)
	{
		if (sortingArray[startIndex] > sortingArray[startIndex+1])
		{
			int temp = sortingArray[startIndex+1];
			sortingArray[startIndex+1] = sortingArray[startIndex];
			sortingArray[startIndex] = temp;
			double ftemp = otherArray[startIndex+1];
			otherArray[startIndex+1] = otherArray[startIndex];
			otherArray[startIndex] = ftemp;
		}
	}
	else if (sortingOffset > 2)
	{
		// First, sort each half
		//
		int leftCount = sortingOffset / 2;
		int rightCount = sortingOffset - leftCount;
		int leftIndex = startIndex;
		int rightIndex = startIndex + leftCount;
		mergeSort(leftIndex, leftCount, sortingArray, tempSortingArray,
			otherArray, tempOtherArray);
		mergeSort(rightIndex, rightCount, sortingArray, tempSortingArray,
			otherArray, tempOtherArray);

		// Second, merge the halves into the temporary arrays
		//
		int index = startIndex;
		while (leftIndex < (startIndex + leftCount)
			&& rightIndex < (startIndex + sortingOffset) )
		{
			if (sortingArray[leftIndex] < sortingArray[rightIndex])
			{
				tempSortingArray[index] = sortingArray[leftIndex];
				tempOtherArray[index++] = otherArray[leftIndex++];
			}
			else
			{
				tempSortingArray[index] = sortingArray[rightIndex];
				tempOtherArray[index++] = otherArray[rightIndex++];
			}
		}

		// One of the sides is done, so add the other side
		//
		while (leftIndex < (startIndex + leftCount) )
		{
			tempSortingArray[index] = sortingArray[leftIndex];
			tempOtherArray[index++] = otherArray[leftIndex++];
		}
		while (rightIndex < (startIndex + sortingOffset) )
		{
			tempSortingArray[index] = sortingArray[rightIndex];
			tempOtherArray[index++] = otherArray[rightIndex++];
		}

		// Finally, copy the temporary sorting array over to the other side
		//
		for (index = startIndex; index < (startIndex + sortingOffset); ++index)
		{
			sortingArray[index] = tempSortingArray[index];
			otherArray[index] = tempOtherArray[index];
		}
	}
}

int binarySearch(int searchValue, MIntArray& searchArray)
//
// Description:
// Application-specific implementation of the binary search algorithm.
// It returns the index in the "searchArray" where the "searchValue"
// is found or -1 if it is not found.
//
{
	int max = searchArray.length();
	int min = 0;
	while (max - min > 1)
	{
		int currentIndex = ((max - min) / 2) + min;
		int currentValue = searchArray[currentIndex];
		if (currentValue == searchValue) return currentIndex;
		else if (currentValue < searchValue) min = currentIndex;
		else max = currentIndex;
	}
	if (searchArray[min] == searchValue) return min;
	else return -1;
}

/****************************************************************************
 * Blind Data Shader class
 ***************************************************************************/

// Static class member variables
//
// Unique Node TypeId
MTypeId blindDataShader::id( 0x00086000 );

void* blindDataShader::creator()
//
// Description: Static member function that returns a new
// dynamically-allocated instance of the class.
//
{
    return new blindDataShader();
}

MStatus blindDataShader::initialize()
//
// Description: Static member function that initializes the static
// member variables of the class. In this case, it does nothing, since
// "id" is already set to the default value.
//
{
	return MS::kSuccess;
}

MStatus blindDataShader::compute(
const MPlug&      plug,
      MDataBlock& block ) 
{ 
    bool k = false;
    k |= (plug==outColor);
    k |= (plug==outColorR);
    k |= (plug==outColorG);
    k |= (plug==outColorB);
    if( !k ) return MS::kUnknownParameter;

	// Always return black for now.
    MFloatVector resultColor(0.0,0.0,0.0);

    // set ouput color attribute
    MDataHandle outColorHandle = block.outputValue( outColor );
    MFloatVector& outColor = outColorHandle.asFloatVector();
    outColor = resultColor;
    outColorHandle.setClean();

    return MS::kSuccess;
}

MStatus	blindDataShader::bind(const MDrawRequest& request, M3dView& view)
{
	view.beginGL();

	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

	view.endGL();

	return MS::kSuccess;
}


MStatus	blindDataShader::unbind(const MDrawRequest& request,
			   M3dView& view)
{
	view.beginGL();	

	glPopClientAttrib();
	glPopAttrib();

	view.endGL();

	return MS::kSuccess;
}

MStatus	blindDataShader::geometry( const MDrawRequest& request,
								M3dView& view,
							    int prim,
								unsigned int writable,
								int indexCount,
								const unsigned int * indexArray,
								int vertexCount,
							    const int * vertexIDs,
								const float * vertexArray,
								int normalCount,
								const float ** normalArrays,
								int colorCount,
								const float ** colorArrays,
								int texCoordCount,
								const float ** texCoordArrays)
//
// Description:
// Once everything is plugged, this function is called by the Maya rendering
// engine to render the given objects. It will be called for each frame, so 
// the efficiency of this algorithm is important.
//
{
	const int blindDataUniqueID = 60;
	MStatus stat;
	int i;

	// Create our own empty colour array and
	// populate it with the default colour value for the attribute.
	//
	double* colours = new double[vertexCount * 3];
	float defaultColour[3] = { 0.0f, 0.0f, 0.0f };
	MPlug plug( thisMObject(), outColor );
	MObject colorObject;
	plug.getValue(colorObject);
	MFnNumericData data(colorObject);
	data.getData(defaultColour[0], defaultColour[1], defaultColour[2]);

	// Use the blind data to set independant colour values
	//
	MFnMesh mesh(request.multiPath().node(), &stat);
	if (stat && mesh.hasBlindData(MFn::kMeshVertComponent, &stat)
		&& mesh.hasBlindData(vertexIDs[0], MFn::kMeshVertComponent,
		blindDataUniqueID, &stat))
	{
		MIntArray redComponentIDs, greenComponentIDs, blueComponentIDs;
		MDoubleArray redColour, greenColour, blueColour;

		stat = mesh.getDoubleBlindData( MFn::kMeshVertComponent,
			blindDataUniqueID, "red", redComponentIDs, redColour);
		if (stat) stat = mesh.getDoubleBlindData( MFn::kMeshVertComponent,
			blindDataUniqueID, "green", greenComponentIDs, greenColour);
		if (stat) stat = mesh.getDoubleBlindData( MFn::kMeshVertComponent,
			blindDataUniqueID, "blue", blueComponentIDs, blueColour);

		if (stat && redComponentIDs.length() == redColour.length()
			&& greenComponentIDs.length() == greenColour.length()
			&& blueComponentIDs.length() == blueColour.length())
		{
			// Now that I have all the blind data,
			// use the vertex IDs and the component IDs arrays
			// to map the blind data values inside the colour array.
			//

			// Sort the components IDs and the colour values in increasing
			// order to speed up the association
			//
			int maxArrayLength =
				(redComponentIDs.length() > greenComponentIDs.length()) ?
				((redComponentIDs.length() > blueComponentIDs.length()) ?
					redComponentIDs.length() : blueComponentIDs.length()) :
				((greenComponentIDs.length() > blueComponentIDs.length()) ?
					greenComponentIDs.length() : blueComponentIDs.length());
			int* tempIntArray = new int[maxArrayLength];
			double* tempFloatArray = new double[maxArrayLength];
			mergeSort(0, redComponentIDs.length(), redComponentIDs,
				tempIntArray, redColour, tempFloatArray);
			mergeSort(0, greenComponentIDs.length(), greenComponentIDs,
				tempIntArray, greenColour, tempFloatArray);
			mergeSort(0, blueComponentIDs.length(), blueComponentIDs,
				tempIntArray, blueColour, tempFloatArray);

			// Associate the vertex IDs with the colour values
			//
			for (i = 0; i < vertexCount; ++i)
			{
				int index = binarySearch(vertexIDs[i], redComponentIDs);
				if (index != -1) colours[3*i] = redColour[index];
				else colours[3*i] = defaultColour[0];

				index = binarySearch(vertexIDs[i], greenComponentIDs);
				if (index != -1) colours[3*i+1] = greenColour[index];
				else colours[3*i+1] = defaultColour[1];

				index = binarySearch(vertexIDs[i], blueComponentIDs);
				if (index != -1) colours[3*i+2] = blueColour[index];
				else colours[3*i+2] = defaultColour[2];
			}
		}
	}

	// Render the triangles using the new vertex colour information
	// taken from the blind data values of the mesh
	//
	view.beginGL();

	glPushAttrib( GL_ALL_ATTRIB_BITS );
	glPushClientAttrib(GL_CLIENT_VERTEX_ARRAY_BIT);

	glDisable(GL_LIGHTING);

	glVertexPointer(3, GL_FLOAT, 0, vertexArray);
	glEnableClientState(GL_VERTEX_ARRAY);

	// Is the next line necessary?
	glColor4f( 1.0f, 1.0f, 1.0f, 1.0f );

	glColorPointer(3, GL_DOUBLE, 0, colours);
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawElements(prim, indexCount, GL_UNSIGNED_INT, indexArray);

	glDisable(GL_COLOR_MATERIAL);

	glPopClientAttrib();
	glPopAttrib();

	view.endGL();

	return MS::kSuccess;
}
