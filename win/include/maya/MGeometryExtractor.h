#ifndef _MGeometryExtractor__
#define _MGeometryExtractor__
//-
// ==========================================================================
// Copyright (C) 2011 Autodesk, Inc., and/or its licensors.  All
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
// CLASS: MGeometryExtractor
//
// ****************************************************************************


#if defined __cplusplus

// ****************************************************************************
// INCLUDED HEADER FILES

#include <maya/MStatus.h>
#include <maya/MHWGeometry.h>
#include <maya/MDagPath.h>
#include <maya/MViewport2Renderer.h>

// ****************************************************************************
// DECLARATIONS

class MString;
class MUserData;

// ****************************************************************************
// NAMESPACE

namespace MHWRender
{
	typedef int MPolyGeomOptions;
	//! Indicates the geometry options for polygon.
	enum{
		kPolyGeom_Normal = 0,				//!< Normal Indicates the polygon performs the default geometry.
		kPolyGeom_NotSharing = 1 << 0,  /*!< NotSharing Indicates if you don't want vertex sharing to be computed by the extractor.
												Vertex buffer size will not be reduced if sharing can be performed.
												*/
		kPolyGeom_BaseMesh = 1 << 1,	/*!< BaseMesh Indicates if you want the base geometry in smoothCage mode.
												 The geometry in extractor is always the base geometry in normal mode.
												*/
	};

    // ****************************************************************************
    // CLASS DECLARATION (MGeometryExtractor)

    //! \ingroup OpenMayaRender
    //! \brief class for extracting renderable geometry
    /*!
        Construct an instance of this class to populate buffers with vertex and indexing 
        data.  Buffers are filled based on the supplied requirements.  Use the 
        MIndexBufferDescriptor and MVertexBufferDescriptor classes to request a broad range
        of data types, formats, and content.

        Buffers are filled in-place so you should request the primitive count and vertex count 
        before filling the buffers.  Use the count multiplied by the element stride to construct
        data buffers large enough to fill properly prior to calling one of the populate methods.
    */
    class OPENMAYARENDER_EXPORT MGeometryExtractor
    {
     public:

       // initialization
        MGeometryExtractor(const MHWRender::MGeometryRequirements& requirements, 
           const MDagPath& path, MPolyGeomOptions options, MStatus* status = NULL);
        ~MGeometryExtractor();

        // Indices
        unsigned int primitiveCount(const MHWRender::MIndexBufferDescriptor& indexDesc) const;
        MStatus populateIndexBuffer( void* data, const unsigned int primitiveCount,
            const MHWRender::MIndexBufferDescriptor& indexDesc) const;

        // Streams
        unsigned int vertexCount( ) const;
        MStatus populateVertexBuffer( void* data, const unsigned int vertexCount,
            const MHWRender::MVertexBufferDescriptor& bufferDesc ) const;

        static unsigned int minimumBufferSize( unsigned int primitiveCount, 
            MGeometry::Primitive primitive, unsigned int primitiveStride=0 );

		 // Deprecated methods
		MGeometryExtractor(const MHWRender::MGeometryRequirements& requirements, 
			const MDagPath& path, const bool sharing = true, MStatus* status = NULL);
    private:
		void MGeometryExtractorConstructHelper(const MHWRender::MGeometryRequirements& requirements, const MDagPath& path,
			MPolyGeomOptions options, MStatus* status);

        bool isValid() const;

        void* shape;
        void* extractor;
        void* dataPtr; // for future use.
    };

} // namespace MHWRender

#endif /* __cplusplus */
#endif /* _MGeometryExtractor__ */
