//-*****************************************************************************
//
// Copyright (c) 2009-2013,
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
// *       Neither the name of Industrial Light & Magic nor the names of
// its contributors may be used to endorse or promote products derived
// from this software without specific prior written permission.
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

#ifndef _Alembic_Util_Foundation_h_
#define _Alembic_Util_Foundation_h_

// tr1/memory is not avaliable in Visual Studio.
#if !defined(_MSC_VER)

#if defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L
#include <unordered_map>
#else
#include <tr1/memory>
#include <tr1/unordered_map>
#endif

#elif _MSC_VER <= 1600

// no tr1 in these older versions of MS VS so fall back to boost
#include <boost/type_traits.hpp>
#include <boost/ref.hpp>
#include <boost/format.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/static_assert.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/utility.hpp>
#include <boost/cstdint.hpp>
#include <boost/array.hpp>
#include <boost/operators.hpp>
#include <boost/foreach.hpp>
#include <boost/unordered_map.hpp>

#else
#include <unordered_map>
#endif

#include <memory>

#include <half.h>

#include <iomanip>
#include <iostream>
#include <sstream>
#include <exception>
#include <limits>

#include <map>
#include <string>
#include <vector>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef _MSC_VER

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// needed for mutex stuff
#include <Windows.h>
#endif

#ifndef ALEMBIC_VERSION_NS
#define ALEMBIC_VERSION_NS v7
#endif

namespace Alembic {
namespace Util {
namespace ALEMBIC_VERSION_NS {

#if defined( _MSC_VER ) && _MSC_VER <= 1600
using boost::dynamic_pointer_cast;
using boost::enable_shared_from_this;
using boost::shared_ptr;
using boost::static_pointer_cast;
using boost::weak_ptr;
using boost::unordered_map;
#define ALEMBIC_LIB_USES_BOOST
#elif defined(__GXX_EXPERIMENTAL_CXX0X) || __cplusplus >= 201103L
using std::dynamic_pointer_cast;
using std::enable_shared_from_this;
using std::shared_ptr;
using std::static_pointer_cast;
using std::weak_ptr;
using std::unordered_map;
#else
using std::tr1::dynamic_pointer_cast;
using std::tr1::enable_shared_from_this;
using std::tr1::shared_ptr;
using std::tr1::static_pointer_cast;
using std::tr1::weak_ptr;
using std::tr1::unordered_map;
#endif

using std::auto_ptr;

// similiar to boost::noncopyable
// explicitly hides copy construction and copy assignment
class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}

private:
    noncopyable( const noncopyable& );
    const noncopyable& operator=( const noncopyable& );
};

// similiar to boost::totally_ordered
// only need < and == operators and this fills in the rest
template < class T >
class totally_ordered
{
    friend bool operator > ( const T& x, const T& y )
    {
        return y < x;
    }

    friend bool operator <= ( const T& x, const T& y )
    {
        return !( y < x );
    }

    friend bool operator >= ( const T& x, const T& y )
    {
        return !( x < y );
    }

    friend bool operator != ( const T& x, const T& y )
    {
        return !( x == y );
    }
};

// inspired by boost::mutex
#ifdef _MSC_VER

class mutex : noncopyable
{
public:
    mutex()
    {
        m = CreateMutex( NULL, FALSE, NULL );
    }

    ~mutex()
    {
        CloseHandle( m );
    }

    void lock()
    {
        WaitForSingleObject( m, INFINITE );
    }

    void unlock()
    {
        ReleaseMutex( m );
    }

private:
    HANDLE m;
};

#else


class mutex : noncopyable
{
public:
    mutex()
    {
        pthread_mutex_init( &m, NULL );
    }

    ~mutex()
    {
        pthread_mutex_destroy( &m );
    }

    void lock()
    {
        pthread_mutex_lock( &m );
    }

    void unlock()
    {
        pthread_mutex_unlock( &m );
    }

private:
    pthread_mutex_t m;
};

#endif

class scoped_lock : noncopyable
{
public:
    scoped_lock( mutex & l ) : m( l )
    {
        m.lock();
    }

    ~scoped_lock()
    {
        m.unlock();
    }

private:
    mutex & m;
};

} // End namespace ALEMBIC_VERSION_NS

using namespace ALEMBIC_VERSION_NS;

} // End namespace Util
} // End namespace Alembic

#endif
