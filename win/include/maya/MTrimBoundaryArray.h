#ifndef _MTrimBoundaryArray
#define _MTrimBoundaryArray
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
// CLASS:    MTrimBoundaryArray
//
// ****************************************************************************

#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES


#include <maya/MStatus.h>
#include <vector>

class MObjectArray;

// ****************************************************************************
// CLASS DECLARATION (MTrimBoundaryArray)

//! \ingroup OpenMaya
//! \brief  An array class for trim boundaries. 
/*!

  This class implements an array of MObjectArray objects. Each
  MObjectArray is a collection of curves (edges, in fact) that
  represent a trim boundary.

  This class owns all of the MObjectArrays in the class. It doesn't
  hold a reference to any other MObjectArray.
*/
class OPENMAYA_EXPORT MTrimBoundaryArray
{

public:
	MTrimBoundaryArray();
	MTrimBoundaryArray(const MTrimBoundaryArray& other);
	~MTrimBoundaryArray();


	MStatus			reserve(unsigned int length);
 	unsigned int		length() const;
 	unsigned int		size() const;

 	const MObjectArray&	operator[](unsigned int index) const;
	const MObjectArray&	get( unsigned int index ) const;

 	MStatus	set(const MObjectArray& boundary, unsigned int index);
 	MStatus	insert(const MObjectArray& boundary, unsigned int index);
 	MStatus	append(const MObjectArray& boundary);
 	MStatus	remove(unsigned int index);
 	MStatus	clear();

	MObject	getMergedBoundary(unsigned int index, MStatus *status=NULL) const;

	static const char* className();

protected:
// No protected members

private:
 	MTrimBoundaryArray& operator = (const MTrimBoundaryArray&);

	bool		fIsUVBoundary;
	void*		fBoundaryArray;
};

#endif /* __cplusplus */
#endif /* _MTrimBoundaryArray */
