//-*****************************************************************************
//
// Copyright (c) 2009-2012,
//  Sony Pictures Imageworks Inc. and
//  Industrial Light & Magic, a division of Lucasfilm Entertainment Company Ltd.
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// *       Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// *       Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
// *       Neither the name of Sony Pictures Imageworks, nor
// Industrial Light & Magic, nor the names of their contributors may be used
// to endorse or promote products derived from this software without specific
// prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//-*****************************************************************************

#ifndef _AlembicExport_MayaUtility_h_
#define _AlembicExport_MayaUtility_h_

#include "Foundation.h"
#include <Alembic/Abc/OArrayProperty.h>
#include <Alembic/Abc/OScalarProperty.h>

namespace util 
{
struct mObjectCmp
{
    bool operator()(const MObject& o1, const MObject& o2) const
        {
            return strcmp(MFnDependencyNode(o1).name().asChar(), MFnDependencyNode(o2).name().asChar()) < 0;
        }
};

typedef std::map <MObject, MSelectionList, mObjectCmp> GetMembersMap;

struct cmpDag
{
    bool operator()( const MDagPath& lhs, const MDagPath& rhs )
    {
            std::string name1(lhs.fullPathName().asChar());
            std::string name2(rhs.fullPathName().asChar());
            return (name1.compare(name2) < 0);
     }
};
typedef std::set< MDagPath, cmpDag > ShapeSet;

inline MStatus isFloat(MString str, const MString & usage)
{
    MStatus status = MS::kSuccess;

    if (!str.isFloat())
    {
        MGlobal::displayInfo(usage);
        status = MS::kFailure;
    }

    return status;
}

inline MStatus isUnsigned(MString str, const MString & usage)
{
    MStatus status = MS::kSuccess;

    if (!str.isUnsigned())
    {
        MGlobal::displayInfo(usage);
        status = MS::kFailure;
    }

    return status;
}

// safely inverse a scale component
inline double inverseScale(double scale)
{
    const double kScaleEpsilon = 1.0e-12;

    if (scale < kScaleEpsilon && scale >= 0.0)
        return 1.0 / kScaleEpsilon;
    else if (scale > -kScaleEpsilon && scale < 0.0)
        return 1.0 / -kScaleEpsilon;
    else
        return 1.0 / scale;
}

// seconds per frame
double spf();

bool isAncestorDescendentRelationship(const MDagPath & path1,
    const MDagPath & path2);

// returns 0 if static, 1 if sampled, and 2 if a curve
int getSampledType(const MPlug& iPlug);

// 0 dont write, 1 write static 0, 2 write anim 0, 3 write anim 1
int getVisibilityType(const MPlug & iPlug);

// determines what order we do the rotation in, returns false if iOrder is
// kInvalid or kLast
bool getRotOrder(MTransformationMatrix::RotationOrder iOrder,
    unsigned int & oXAxis, unsigned int & oYAxis, unsigned int & oZAxis);

// determine if a Maya Object is animated or not
// copy from mayapit code (MayaPit.h .cpp)
bool isAnimated(MObject & object, bool checkParent = false);

// determine if a Maya Object is intermediate
bool isIntermediate(const MObject & object);

// returns true for visible and lod invisible and not templated objects
bool isRenderable(const MObject & object);

// strip iDepth namespaces from the node name, go from taco:foo:bar to bar
// for iDepth > 1
MString stripNamespaces(const MString & iNodeName, unsigned int iDepth);

// returns the Help string for AbcExport
MString getHelpText();

} // namespace util

struct PlugAndObjScalar
{
    MPlug plug;
    MObject obj;
    Alembic::Abc::OScalarProperty prop;
};

struct PlugAndObjArray
{
    MPlug plug;
    MObject obj;
    Alembic::Abc::OArrayProperty prop;
};

struct JobArgs
{
    JobArgs()
    {
        excludeInvisible = false;
        filterEulerRotations = false;
        noNormals = false;
        stripNamespace = 0;
        useSelectionList = false;
        worldSpace = false;
        writeVisibility = false;
        writeUVs = false;
        writeColorSets = false;
        writeFaceSets = false;
        verbose = false;
    }

    bool excludeInvisible;
    bool filterEulerRotations;
    bool noNormals;
    unsigned int stripNamespace;
    bool useSelectionList;
    bool worldSpace;
    bool writeVisibility;
    bool writeUVs;
    bool writeColorSets;
    bool writeFaceSets;
    bool verbose;

    std::string melPerFrameCallback;
    std::string melPostCallback;
    std::string pythonPerFrameCallback;
    std::string pythonPostCallback;

    // to put into .arbGeomParam
    std::vector< std::string > prefixFilters;
    std::set< std::string > attribs;

    // to put into .userProperties
    std::vector< std::string > userPrefixFilters;
    std::set< std::string > userAttribs;

    util::ShapeSet dagPaths;
};

#endif  // _AlembicExport_MayaUtility_h_
