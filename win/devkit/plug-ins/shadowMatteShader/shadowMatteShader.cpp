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

#include <math.h>

#include <maya/MPxNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnLightDataAttribute.h>
#include <maya/MFloatVector.h>
#include <maya/MFnPlugin.h>

//
// DESCRIPTION:
///////////////////////////////////////////////////////
class ShadowMatte : public MPxNode
{
	public:
                      ShadowMatte();
    virtual           ~ShadowMatte();

    virtual MStatus   compute( const MPlug&, MDataBlock& );
	virtual void      postConstructor();

    static void *     creator();
    static MStatus    initialize();
    static MTypeId    id;

	private:

	static MObject  aOutColor;
	static MObject  aOutTransparency;
    static MObject  aViewColor;

	static MObject  aLightDirection;
	static MObject  aLightIntensity;
    static MObject  aLightAmbient;
    static MObject  aLightDiffuse;
    static MObject  aLightSpecular;
    static MObject  aLightShadowFraction;
    static MObject  aPreShadowIntensity;
    static MObject  aLightBlindData;
    static MObject  aLightData;
};

MTypeId ShadowMatte::id( 0x81013 );

MObject  ShadowMatte::aViewColor;
MObject  ShadowMatte::aOutColor;
MObject  ShadowMatte::aOutTransparency;
MObject  ShadowMatte::aLightData;
MObject  ShadowMatte::aLightDirection;
MObject  ShadowMatte::aLightIntensity;
MObject  ShadowMatte::aLightAmbient;
MObject  ShadowMatte::aLightDiffuse;
MObject  ShadowMatte::aLightSpecular;
MObject  ShadowMatte::aLightShadowFraction;
MObject  ShadowMatte::aPreShadowIntensity;
MObject  ShadowMatte::aLightBlindData;

//
// DESCRIPTION:
///////////////////////////////////////////////////////
void ShadowMatte::postConstructor( )
{
	setMPSafe(true);
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
ShadowMatte::ShadowMatte()
{
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
ShadowMatte::~ShadowMatte()
{
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
void * ShadowMatte::creator()
{
    return new ShadowMatte();
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus ShadowMatte::initialize()
{
    MFnNumericAttribute nAttr; 
    MFnLightDataAttribute lAttr;

// Outputs

    aViewColor = nAttr.create( "viewColor", "vc", MFnNumericData::kBoolean);
    nAttr.setKeyable(true);
    nAttr.setStorable(true);
    nAttr.setHidden(false);
    nAttr.setDefault(true);

    aOutColor = nAttr.createColor( "outColor", "oc");
    nAttr.setStorable(false);
    nAttr.setHidden(false);
    nAttr.setReadable(true);
    nAttr.setWritable(false);

    aOutTransparency  = nAttr.createColor( "outTransparency", "ot" );
    nAttr.setStorable(false);
    nAttr.setHidden(false);
    nAttr.setReadable(true);
    nAttr.setWritable(false);


// Inputs

    aLightDirection = nAttr.createPoint( "lightDirection", "ld" );
    nAttr.setStorable(false);
    nAttr.setHidden(true);
    nAttr.setReadable(true);
    nAttr.setWritable(false);
    nAttr.setDefault(1.0f, 1.0f, 1.0f);

    aLightIntensity = nAttr.createColor( "lightIntensity", "li" );
    nAttr.setStorable(false);
    nAttr.setHidden(true);
    nAttr.setReadable(true);
    nAttr.setWritable(false);
    nAttr.setDefault(1.0f, 1.0f, 1.0f);

    aLightAmbient = nAttr.create( "lightAmbient", "la",
								  MFnNumericData::kBoolean);
    nAttr.setStorable(false);
    nAttr.setHidden(true);
    nAttr.setReadable(true);
    nAttr.setWritable(false);
    nAttr.setDefault(true);

    aLightDiffuse = nAttr.create( "lightDiffuse", "ldf",
								  MFnNumericData::kBoolean);
    nAttr.setStorable(false);
    nAttr.setHidden(true);
    nAttr.setReadable(true);
    nAttr.setWritable(false);
    nAttr.setDefault(true);

    aLightSpecular = nAttr.create( "lightSpecular", "ls", 
								   MFnNumericData::kBoolean);
    nAttr.setStorable(false);
    nAttr.setHidden(true);
    nAttr.setReadable(true);
    nAttr.setWritable(false);
    nAttr.setDefault(false);

    aLightShadowFraction = nAttr.create("lightShadowFraction", "lsf",
										MFnNumericData::kFloat);
    nAttr.setStorable(false);
    nAttr.setHidden(true);
    nAttr.setReadable(true);
    nAttr.setWritable(false);
    nAttr.setDefault(0.0f);

    aPreShadowIntensity = nAttr.create("preShadowIntensity", "psi",
									   MFnNumericData::kFloat);
    nAttr.setStorable(false);
    nAttr.setHidden(true);
    nAttr.setReadable(true);
    nAttr.setWritable(false);
    nAttr.setDefault(0.0f);

    aLightBlindData = nAttr.createAddr("lightBlindData", "lbld");
    nAttr.setStorable(false);
    nAttr.setHidden(true);
    nAttr.setReadable(true);
    nAttr.setWritable(false);

    aLightData = lAttr.create( "lightDataArray", "ltd", 
                               aLightDirection, aLightIntensity, aLightAmbient,
                               aLightDiffuse, aLightSpecular,
							   aLightShadowFraction,
                               aPreShadowIntensity,
                               aLightBlindData);
    lAttr.setArray(true);
    lAttr.setStorable(false);
    lAttr.setHidden(true);
    lAttr.setDefault(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, true, true,
					 false, 0.0f, 1.0f, NULL);

    addAttribute(aLightData);
    addAttribute(aOutColor);
    addAttribute(aOutTransparency);
    addAttribute(aViewColor);


    attributeAffects (aViewColor, aOutColor);
    attributeAffects (aLightIntensity, aOutColor);
    attributeAffects (aLightData, aOutColor);
    attributeAffects (aLightAmbient, aOutColor);
    attributeAffects (aLightSpecular, aOutColor);
    attributeAffects (aLightDiffuse, aOutColor);
    attributeAffects (aLightDirection, aOutColor);
    attributeAffects (aLightShadowFraction, aOutColor);
    attributeAffects (aPreShadowIntensity, aOutColor);
    attributeAffects (aLightBlindData, aOutColor);

    attributeAffects (aViewColor, aOutTransparency);
    attributeAffects (aLightIntensity, aOutTransparency);
    attributeAffects (aLightData, aOutTransparency);
    attributeAffects (aLightAmbient, aOutTransparency);
    attributeAffects (aLightSpecular, aOutTransparency);
    attributeAffects (aLightDiffuse, aOutTransparency);
    attributeAffects (aLightDirection, aOutTransparency);
    attributeAffects (aLightShadowFraction, aOutTransparency);
    attributeAffects (aPreShadowIntensity, aOutTransparency);
    attributeAffects (aLightBlindData, aOutTransparency);

    return MS::kSuccess;
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus ShadowMatte::compute(
const MPlug&      plug,
      MDataBlock& block ) 
{
    if ((plug != aOutColor) && (plug.parent() != aOutColor) &&
		(plug != aOutTransparency) && (plug.parent() != aOutTransparency))
		return MS::kUnknownParameter;

    MFloatVector shadowColor(0.0,0.0,0.0);

    bool  ViewFlag  = block.inputValue( aViewColor ).asBool();

    // get light list
    MArrayDataHandle lightData = block.inputArrayValue( aLightData );
    int numLights = lightData.elementCount();

    // iterate through light list and get ambient/diffuse values
    for( int count=1; count <= numLights; count++ )
    {
        MDataHandle currentLight = lightData.inputValue();
        float lightShadow = currentLight.child(aLightShadowFraction).asFloat();

		// shadow fraction tells how much an object is in shadow:
		// (1)   totally in shadow
		// (0-1) partially in shadow
		// (0)   not in shadow

        shadowColor[0] += lightShadow;
        shadowColor[1] += lightShadow;
        shadowColor[2] += lightShadow;
     
        if( !lightData.next() ) break;
    }

    // set ouput color attribute
    MFloatVector ghostColor(0.0,0.0,0.0);
    MDataHandle outColorHandle = block.outputValue( aOutColor );
    MFloatVector& outColor = outColorHandle.asFloatVector();

    if (ViewFlag)
        outColor = shadowColor;
    else
        outColor = ghostColor;

    outColorHandle.setClean();

    // set ouput transparency
    MDataHandle outTransHandle = block.outputValue( aOutTransparency );
    MFloatVector& outTrans = outTransHandle.asFloatVector();
    outTrans = shadowColor;
    outTransHandle.setClean();

    return MS::kSuccess;
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus initializePlugin( MObject obj )
{ 
   const MString UserClassify( "shader/surface" );

   MFnPlugin plugin( obj, PLUGIN_COMPANY, "3.0", "Any");
   plugin.registerNode( "shadowMatte", ShadowMatte::id, 
                         ShadowMatte::creator, ShadowMatte::initialize,
                         MPxNode::kDependNode, &UserClassify );

   return MS::kSuccess;
}

//
// DESCRIPTION:
///////////////////////////////////////////////////////
MStatus uninitializePlugin( MObject obj )
{
   MFnPlugin plugin( obj );
   plugin.deregisterNode( ShadowMatte::id );

   return MS::kSuccess;
}
