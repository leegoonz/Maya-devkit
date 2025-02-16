#ifndef _MPxFluidEmitterNode
#define _MPxFluidEmitterNode
//-
// ==========================================================================
// Copyright (C) 1995 - 2006 Autodesk, Inc., and/or its licensors.  All
// rights reserved.
//
// The coded instructions, statements, computer programs, and/or related
// material (collectively the "Data") in these files contain unpublished
// information proprietary to Autodesk, Inc. ("Autodesk") and/or its
// licensors,  which is protected by U.S. and Canadian federal copyright law
// and by international treaties.
//
// The Data may not be disclosed or distributed to third parties or be
// copied or duplicated, in whole or in part, without the prior written
// consent of Autodesk.
//
// The copyright notices in the Software and this entire statement,
// including the above license grant, this restriction and the following
// disclaimer, must be included in all copies of the Software, in whole
// or in part, and all derivative works of the Software, unless such copies
// or derivative works are solely in the form of machine-executable object
// code generated by a source language processor.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND.
// AUTODESK DOES NOT MAKE AND HEREBY DISCLAIMS ANY EXPRESS OR IMPLIED
// WARRANTIES INCLUDING, BUT NOT LIMITED TO, THE WARRANTIES OF
// NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE,
// OR ARISING FROM A COURSE OF DEALING, USAGE, OR TRADE PRACTICE. IN NO
// EVENT WILL AUTODESK AND/OR ITS LICENSORS BE LIABLE FOR ANY LOST
// REVENUES, DATA, OR PROFITS, OR SPECIAL, DIRECT, INDIRECT, OR
// CONSEQUENTIAL DAMAGES, EVEN IF AUTODESK AND/OR ITS LICENSORS HAS
// BEEN ADVISED OF THE POSSIBILITY OR PROBABILITY OF SUCH DAMAGES.
// ==========================================================================
//+
//
// CLASS:    MPxFluidEmitterNode
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <maya/MObject.h>
#include <maya/MPxNode.h>
#include <maya/MPxEmitterNode.h>
#include <maya/MPointArray.h>
#include <maya/MVectorArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnFluid.h>
#include <maya/MColor.h>

// ****************************************************************************
// CLASS DECLARATION (MPxFluidEmitterNode)

//! \ingroup OpenMayaFX
//! \brief Base class for user defined particle emitters
/*!
  MPxFluidEmitterNode allows the creation and manipulation of
  dependency graph nodes representing fluid emitters.
*/
class OPENMAYAFX_EXPORT MPxFluidEmitterNode : public MPxEmitterNode
{
public:
	MPxFluidEmitterNode();
	virtual ~MPxFluidEmitterNode();
	virtual MPxNode::Type	type() const;
	virtual MStatus			compute(const MPlug& plug, MDataBlock& dataBlock);

	virtual MStatus			fluidEmitter( const MObject& fluidObj, const MMatrix& worldMatrix, int plugIndex );

	//! density emission rate attribute
	static MObject mFluidDensityEmission;
	//!	heat emission rate attribute
	static MObject mFluidHeatEmission;
	//! fuel emission rate attribute
	static MObject mFluidFuelEmission;
	//! color emission toggle attribute
	static MObject mEmitFluidColor;
	//! emission color attribute
	static MObject mFluidColor;
	//! emission red color channel attribute
	static MObject mFluidColorR;
	//! emission green color channel attribute
	static MObject mFluidColorG;
	//! emission blue color channel attribute
	static MObject mFluidColorB;
	//! emission dropoff attribute
	static MObject mFluidDropoff;
	//! emission turbulence attribute
	static MObject mTurbulence;
	//! emission jitter attribute
	static MObject mFluidJitter;
	//!	emission function attribute
	static MObject mEmissionFunction;

	double fluidDensityEmission( MDataBlock& block );
	double fluidHeatEmission( MDataBlock& block );
	double fluidFuelEmission( MDataBlock& block );
	bool   fluidEmitColor( MDataBlock& block );
	MColor fluidColor( MDataBlock& block );

	double fluidDropoff( MDataBlock& block );
	double turbulence( MDataBlock& block );
	bool   fluidJitter( MDataBlock& block );

	static const char*	className();

protected:
// No protected members

private:

	static void			initialSetup();

};

#endif /* __cplusplus */
#endif /* _MPxFluidEmitterNode */

