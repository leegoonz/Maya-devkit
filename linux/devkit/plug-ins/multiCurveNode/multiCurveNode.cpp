//-
// ==========================================================================
// Copyright 1995,2006,2008 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk
// license agreement provided at the time of installation or download,
// or which otherwise accompanies this software in either electronic
// or hard copy form.
// ==========================================================================
//+

#include <string.h>
#include <maya/MIOStream.h>
#include <math.h>

#include <maya/MPxNode.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnPlugin.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnNurbsCurveData.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MPointArray.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MArrayDataBuilder.h>

class multiCurve : public MPxNode
{
public:
						multiCurve() {};
	virtual				~multiCurve();

	virtual MStatus		compute( const MPlug& plug, MDataBlock& data );
	static	void*		creator();
	static	MStatus		initialize();

public:
	static	MObject     inputCurve;
	static	MObject		outputCurves;
	static	MObject		numCurves;
	static	MObject		curveOffset;
	static	MTypeId		id;
};


MTypeId	    multiCurve::id( 0x8000a );
MObject     multiCurve::inputCurve;
MObject     multiCurve::outputCurves;
MObject     multiCurve::numCurves;
MObject     multiCurve::curveOffset;

// Error macro: if not successful, print error message and return
// the MStatus instance containing the error code.
// Assumes that "stat" contains the error value
#define PERRORfail(stat,msg) \
								if (!(stat)) { \
									stat.perror((msg)); \
									return (stat); \
								}

multiCurve::~multiCurve () {}

void* multiCurve::creator()
{
	return new multiCurve();
}

MStatus multiCurve::initialize()
{
	MStatus				stat;
	MFnNumericAttribute	nAttr;
	MFnTypedAttribute	typedAttr;
	
	numCurves = nAttr.create ("numCurves", "nc",
							  MFnNumericData::kLong, 5, &stat);
	PERRORfail(stat, "initialize create numCurves attribute");
	CHECK_MSTATUS ( nAttr.setKeyable( true ) );
	stat = addAttribute( numCurves );
	PERRORfail(stat, "addAttribute(numCurves)");
	
	curveOffset = nAttr.create ("curveOffset", "co",
							  MFnNumericData::kDouble, 1.0, &stat);
	PERRORfail(stat, "initialize create curveOffset attribute");
	CHECK_MSTATUS ( nAttr.setKeyable( true ) );
	stat = addAttribute( curveOffset );
	PERRORfail(stat, "addAttribute(curveOffset)");

	inputCurve = typedAttr.create( "inputCurve", "ic",
								   MFnNurbsCurveData::kNurbsCurve, &stat );
	PERRORfail(stat, "initialize create inputCurve attribute");
	CHECK_MSTATUS ( typedAttr.setReadable( false ) );
	CHECK_MSTATUS ( typedAttr.setWritable( true ) );
	stat = addAttribute( inputCurve );
	PERRORfail(stat, "addAttribute(inputCurve)");
	
	outputCurves = typedAttr.create( "outputCurves", "oc",
									 MFnNurbsCurveData::kNurbsCurve, &stat );
	PERRORfail(stat, "initialize create outputCurves attribute");
	CHECK_MSTATUS ( typedAttr.setArray( true ) );
	CHECK_MSTATUS ( typedAttr.setReadable( true ) );
	CHECK_MSTATUS ( typedAttr.setWritable( false ) );
	CHECK_MSTATUS ( typedAttr.setUsesArrayDataBuilder( true ) );
	stat = addAttribute( outputCurves );
	PERRORfail(stat, "addAttribute(outputCurves)");

	stat = attributeAffects( numCurves, outputCurves );
	PERRORfail(stat, "attributeAffects(inputCurve, outputCurves)");

	stat = attributeAffects( curveOffset, outputCurves );
	PERRORfail(stat, "attributeAffects(inputCurve, outputCurves)");

	stat = attributeAffects( inputCurve, outputCurves );
	PERRORfail(stat, "attributeAffects(inputCurve, outputCurves)");

	return stat;
}

MStatus multiCurve::compute( const MPlug& plug, MDataBlock& data )
{
	MStatus stat;

	if ( plug == outputCurves )
	{
		MDataHandle numCurvesHandle =  data.inputValue(numCurves, &stat);
		PERRORfail(stat, "multiCurve::compute getting numCurves");
		int num = numCurvesHandle.asLong();

		MDataHandle curveOffsetHandle =  data.inputValue(curveOffset, &stat);
		PERRORfail(stat, "multiCurve::compute getting curveOffset");
		double baseOffset = curveOffsetHandle.asDouble();

		MDataHandle inputCurveHandle = data.inputValue(inputCurve, &stat);
		PERRORfail(stat, "multiCurve::compute getting inputCurve");

		MObject inputCurveObject ( inputCurveHandle.asNurbsCurveTransformed() );
		MFnNurbsCurve inCurveFS ( inputCurveObject );

		MArrayDataHandle outputArray = data.outputArrayValue(outputCurves,
															 &stat);
		PERRORfail(stat, "multiCurve::compute getting output data handle");

		// Create an array data build that is preallocated to hold just
		// the number of curves we plan on creating.  When this builder
		// is set in to the MArrayDataHandle at the end of the compute
		// method, the new array will replace the existing array in the
		// scene.
		// 
		// If the number of elements of the multi does not change between
		// compute cycles, then one can reuse the space allocated on a
		// previous cycle by extracting the existing builder from the
		// MArrayDataHandle:
		//		MArrayDataBuilder builder( outputArray.builder(&stat) );
		// this later form of the builder will allow you to rewrite elements
		// of the array, and to grow it, but the array can only be shrunk by
		// explicitly removing elements with the method
		//		MArrayDataBuilder::removeElement(unsigned index);
		//
		MArrayDataBuilder builder(outputCurves, num, &stat);
		PERRORfail(stat, "multiCurve::compute creating builder");
		
		for (int curveNum = 0; curveNum < num; curveNum++) {
			MDataHandle outHandle = builder.addElement(curveNum);
			MFnNurbsCurveData dataCreator;
			MObject outCurveData = dataCreator.create();
			MObject outputCurve  = inCurveFS.copy(inputCurveObject,
												  outCurveData, &stat);
			PERRORfail(stat, "multiCurve::compute copying curve");

			MFnNurbsCurve outCurveFS ( outputCurve );
			MPointArray cvs;

			double offset = baseOffset * (curveNum+1);

			outCurveFS.getCVs ( cvs, MSpace::kWorld );
			int numCVs = cvs.length();
			for (int i = 0; i < numCVs; i++) {
				cvs[i].x += offset;
			}
			outCurveFS.setCVs ( cvs );

			outHandle.set(outCurveData);
		}
		
		// Set the builder back into the output array.  This statement
		// is always required, no matter what constructor was used to
		// create the builder.
		stat = outputArray.set(builder);
		PERRORfail(stat, "multiCurve::compute setting the builder");

		// Since we compute all the elements of the array, instead of
		// just marking the plug we were asked to compute as clean, mark
		// every element of the array as clean to prevent further calls
		// to this compute method during this DG evaluation cycle.
		stat = outputArray.setAllClean();
		PERRORfail(stat, "multiCurve::compute cleaning outputCurves");

	} else {
		return MS::kUnknownParameter;
	}
	
	return stat;
}

MStatus initializePlugin( MObject obj )
{
	MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");

	CHECK_MSTATUS (plugin.registerNode( "multiCurve", multiCurve::id,
								multiCurve::creator, multiCurve::initialize ) );
	return MS::kSuccess;;
}

MStatus uninitializePlugin( MObject obj )
{
	MFnPlugin plugin( obj );

	CHECK_MSTATUS (plugin.deregisterNode( multiCurve::id ) );
	
	return MS::kSuccess;;
}
