/*
    Copyright 2005-2014 Intel Corporation.  All Rights Reserved.

    The source code contained or described herein and all documents related
    to the source code ("Material") are owned by Intel Corporation or its
    suppliers or licensors.  Title to the Material remains with Intel
    Corporation or its suppliers and licensors.  The Material is protected
    by worldwide copyright laws and treaty provisions.  No part of the
    Material may be used, copied, reproduced, modified, published, uploaded,
    posted, transmitted, distributed, or disclosed in any way without
    Intel's prior express written permission.

    No license under any patent, copyright, trade secret or other
    intellectual property right is granted to or conferred upon you by
    disclosure or delivery of the Materials, either expressly, by
    implication, inducement, estoppel or otherwise.  Any license under such
    intellectual property rights must be express and approved by Intel in
    writing.
*/

#ifndef __TBB__flow_graph_types_impl_H
#define __TBB__flow_graph_types_impl_H

#ifndef __TBB_flow_graph_H
#error Do not #include this internal file directly; use public TBB headers instead.
#endif

// included in namespace tbb::flow::interface7

namespace internal {
// wrap each element of a tuple in a template, and make a tuple of the result.

    template<int N, template<class> class PT, typename TypeTuple>
    struct wrap_tuple_elements;

    template<template<class> class PT, typename TypeTuple>
    struct wrap_tuple_elements<1, PT, TypeTuple> {
        typedef typename tbb::flow::tuple<
                PT<typename tbb::flow::tuple_element<0,TypeTuple>::type> >
            type;
    };

    template<template<class> class PT, typename TypeTuple>
    struct wrap_tuple_elements<2, PT, TypeTuple> {
        typedef typename tbb::flow::tuple<
                PT<typename tbb::flow::tuple_element<0,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<1,TypeTuple>::type> >
            type;
    };

    template<template<class> class PT, typename TypeTuple>
    struct wrap_tuple_elements<3, PT, TypeTuple> {
        typedef typename tbb::flow::tuple<
                PT<typename tbb::flow::tuple_element<0,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<1,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<2,TypeTuple>::type> >
            type;
    };

    template<template<class> class PT, typename TypeTuple>
    struct wrap_tuple_elements<4, PT, TypeTuple> {
        typedef typename tbb::flow::tuple<
                PT<typename tbb::flow::tuple_element<0,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<1,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<2,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<3,TypeTuple>::type> >
            type;
    };

    template<template<class> class PT, typename TypeTuple>
    struct wrap_tuple_elements<5, PT, TypeTuple> {
        typedef typename tbb::flow::tuple<
                PT<typename tbb::flow::tuple_element<0,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<1,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<2,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<3,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<4,TypeTuple>::type> >
            type;
    };

#if __TBB_VARIADIC_MAX >= 6
    template<template<class> class PT, typename TypeTuple>
    struct wrap_tuple_elements<6, PT, TypeTuple> {
        typedef typename tbb::flow::tuple<
                PT<typename tbb::flow::tuple_element<0,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<1,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<2,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<3,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<4,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<5,TypeTuple>::type> >
            type;
    };
#endif

#if __TBB_VARIADIC_MAX >= 7
    template<template<class> class PT, typename TypeTuple>
    struct wrap_tuple_elements<7, PT, TypeTuple> {
        typedef typename tbb::flow::tuple<
                PT<typename tbb::flow::tuple_element<0,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<1,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<2,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<3,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<4,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<5,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<6,TypeTuple>::type> >
            type;
    };
#endif

#if __TBB_VARIADIC_MAX >= 8
    template<template<class> class PT, typename TypeTuple>
    struct wrap_tuple_elements<8, PT, TypeTuple> {
        typedef typename tbb::flow::tuple<
                PT<typename tbb::flow::tuple_element<0,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<1,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<2,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<3,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<4,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<5,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<6,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<7,TypeTuple>::type> >
            type;
    };
#endif

#if __TBB_VARIADIC_MAX >= 9
    template<template<class> class PT, typename TypeTuple>
    struct wrap_tuple_elements<9, PT, TypeTuple> {
        typedef typename tbb::flow::tuple<
                PT<typename tbb::flow::tuple_element<0,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<1,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<2,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<3,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<4,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<5,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<6,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<7,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<8,TypeTuple>::type> >
            type;
    };
#endif

#if __TBB_VARIADIC_MAX >= 10
    template<template<class> class PT, typename TypeTuple>
    struct wrap_tuple_elements<10, PT, TypeTuple> {
        typedef typename tbb::flow::tuple<
                PT<typename tbb::flow::tuple_element<0,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<1,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<2,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<3,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<4,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<5,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<6,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<7,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<8,TypeTuple>::type>,
                PT<typename tbb::flow::tuple_element<9,TypeTuple>::type> >
            type;
    };
#endif

//! type mimicking std::pair but with trailing fill to ensure each element of an array
//* will have the correct alignment
    template<typename T1, typename T2, size_t REM>
    struct type_plus_align {
        char first[sizeof(T1)];
        T2 second;
        char fill1[REM];
    };

    template<typename T1, typename T2>
    struct type_plus_align<T1,T2,0> {
        char first[sizeof(T1)];
        T2 second;
    };

    template<class U> struct alignment_of {
        typedef struct { char t; U    padded; } test_alignment;
        static const size_t value = sizeof(test_alignment) - sizeof(U);
    };

    // T1, T2 are actual types stored.  The space defined for T1 in the type returned
    // is a char array of the correct size.  Type T2 should be trivially-constructible,
    // T1 must be explicitly managed.
    template<typename T1, typename T2>
    struct aligned_pair {
        static const size_t t1_align = alignment_of<T1>::value;
        static const size_t t2_align = alignment_of<T2>::value;
        typedef type_plus_align<T1, T2, 0 > just_pair;
        static const size_t max_align = t1_align < t2_align ? t2_align : t1_align;
        static const size_t extra_bytes = sizeof(just_pair) % max_align;
        static const size_t remainder = extra_bytes ? max_align - extra_bytes : 0;
    public:
        typedef type_plus_align<T1,T2,remainder> type;
    };  // aligned_pair

// support for variant type
// type we use when we're not storing a value
struct default_constructed { };

// type which contains another type, tests for what type is contained, and references to it.
// internal::Wrapper<T>
//     void CopyTo( void *newSpace) : builds a Wrapper<T> copy of itself in newSpace

// struct to allow us to copy and test the type of objects
struct WrapperBase {
    virtual ~WrapperBase() {}
    virtual void CopyTo(void* /*newSpace*/) const {  }
};

// Wrapper<T> contains a T, with the ability to test what T is.  The Wrapper<T> can be
// constructed from a T, can be copy-constructed from another Wrapper<T>, and can be
// examined via value(), but not modified.
template<typename T>
struct Wrapper: public WrapperBase {
    typedef T value_type;
    typedef T* pointer_type;
private:
    T value_space;
public:
    const value_type &value() const { return value_space; }

private:
    Wrapper();

    // on exception will ensure the Wrapper will contain only a trivially-constructed object
    struct _unwind_space {
        pointer_type space;
        _unwind_space(pointer_type p) : space(p) {}
        ~_unwind_space() {
            if(space) (void) new (space) Wrapper<default_constructed>(default_constructed());
        }
    };
public:
    explicit Wrapper( const T& other ) : value_space(other) { }
    explicit Wrapper(const Wrapper& other) : value_space(other.value_space) { }

    /*override*/void CopyTo(void* newSpace) const {
        _unwind_space guard((pointer_type)newSpace);
        (void) new(newSpace) Wrapper(value_space);
        guard.space = NULL;
    }
    /*override*/~Wrapper() { }
};

// specialization for array objects
template<typename T, size_t N>
struct Wrapper<T[N]> : public WrapperBase {
    typedef T value_type;
    typedef T* pointer_type;
    // space must be untyped.
    typedef T ArrayType[N];
private:
    // The space is not of type T[N] because when copy-constructing, it would be
    // default-initialized and then copied to in some fashion, resulting in two
    // constructions and one destruction per element.  If the type is char[ ], we
    // placement new into each element, resulting in one construction per element.
    static const size_t space_size = sizeof(ArrayType) / sizeof(char);
    char value_space[space_size];


    // on exception will ensure the already-built objects will be destructed
    // (the value_space is a char array, so it is already trivially-destructible.)
    struct _unwind_class {
        pointer_type space;
        int    already_built;
        _unwind_class(pointer_type p) : space(p), already_built(0) {}
        ~_unwind_class() {
            if(space) {
                for(size_t i = already_built; i > 0 ; --i ) space[i-1].~value_type();
                (void) new(space) Wrapper<default_constructed>(default_constructed());
            }
        }
    };
public:
    const ArrayType &value() const {
        char *vp = const_cast<char *>(value_space);
        return reinterpret_cast<ArrayType &>(*vp);
    }

private:
    Wrapper();
public:
    // have to explicitly construct because other decays to a const value_type*
    explicit Wrapper(const ArrayType& other) {
        _unwind_class guard((pointer_type)value_space);
        pointer_type vp = reinterpret_cast<pointer_type>(&value_space);
        for(size_t i = 0; i < N; ++i ) {
            (void) new(vp++) value_type(other[i]);
            ++(guard.already_built);
        }
        guard.space = NULL;
    }
    explicit Wrapper(const Wrapper& other) : WrapperBase() {
        // we have to do the heavy lifting to copy contents
        _unwind_class guard((pointer_type)value_space);
        pointer_type dp = reinterpret_cast<pointer_type>(value_space);
        pointer_type sp = reinterpret_cast<pointer_type>(const_cast<char *>(other.value_space));
        for(size_t i = 0; i < N; ++i, ++dp, ++sp) {
            (void) new(dp) value_type(*sp);
            ++(guard.already_built);
        }
        guard.space = NULL;
    }

    /*override*/void CopyTo(void* newSpace) const {
        (void) new(newSpace) Wrapper(*this);  // exceptions handled in copy constructor
    }

    /*override*/~Wrapper() {
        // have to destroy explicitly in reverse order
        pointer_type vp = reinterpret_cast<pointer_type>(&value_space);
        for(size_t i = N; i > 0 ; --i ) vp[i-1].~value_type();
    }
};

// given a tuple, return the type of the element that has the maximum alignment requirement.
// Given a tuple and that type, return the number of elements of the object with the max
// alignment requirement that is at least as big as the largest object in the tuple.

template<bool, class T1, class T2> struct pick_one;
template<class T1, class T2> struct pick_one<true , T1, T2> { typedef T1 type; };
template<class T1, class T2> struct pick_one<false, T1, T2> { typedef T2 type; };

template< template<class> class Selector, typename T1, typename T2 >
struct pick_max {
    typedef typename pick_one< (Selector<T1>::value > Selector<T2>::value), T1, T2 >::type type;
};

template<typename T> struct size_of { static const int value = sizeof(T); };

template< size_t N, class Tuple, template<class> class Selector > struct pick_tuple_max {
    typedef typename pick_tuple_max<N-1, Tuple, Selector>::type LeftMaxType;
    typedef typename tbb::flow::tuple_element<N-1, Tuple>::type ThisType;
    typedef typename pick_max<Selector, LeftMaxType, ThisType>::type type;
};

template< class Tuple, template<class> class Selector > struct pick_tuple_max<0, Tuple, Selector> {
    typedef typename tbb::flow::tuple_element<0, Tuple>::type type;
};

// is the specified type included in a tuple?

template<class U, class V> struct is_same_type      { static const bool value = false; };
template<class W>          struct is_same_type<W,W> { static const bool value = true; };

template<class Q, size_t N, class Tuple>
struct is_element_of {
    typedef typename tbb::flow::tuple_element<N-1, Tuple>::type T_i;
    static const bool value = is_same_type<Q,T_i>::value || is_element_of<Q,N-1,Tuple>::value;
};

template<class Q, class Tuple>
struct is_element_of<Q,0,Tuple> {
    typedef typename tbb::flow::tuple_element<0, Tuple>::type T_i;
    static const bool value = is_same_type<Q,T_i>::value;
};

// allow the construction of types that are listed tuple.  If a disallowed type
// construction is written, a method involving this type is created.  The
// type has no definition, so a syntax error is generated.
template<typename T> struct ERROR_Type_Not_allowed_In_Tagged_Msg_Not_Member_Of_Tuple;

template<typename T, bool BUILD_IT> struct do_if;
template<typename T>
struct do_if<T, true> {
    static void construct(void *mySpace, const T& x) {
        (void) new(mySpace) Wrapper<T>(x);
    }
};
template<typename T>
struct do_if<T, false> {
    static void construct(void * /*mySpace*/, const T& x) {
        // This method is instantiated when the type T does not match any of the
        // element types in the Tuple in variant<Tuple>.
        ERROR_Type_Not_allowed_In_Tagged_Msg_Not_Member_Of_Tuple<T>::bad_type(x);
    }
};

// Tuple tells us the allowed types that variant can hold.  It determines the alignment of the space in
// Wrapper, and how big Wrapper is.
//
// the object can only be tested for type, and a read-only reference can be fetched by cast_to<T>().

using tbb::internal::punned_cast;
struct tagged_null_type {};
template<typename TagType, typename T0, typename T1=tagged_null_type, typename T2=tagged_null_type, typename T3=tagged_null_type, 
                           typename T4=tagged_null_type, typename T5=tagged_null_type, typename T6=tagged_null_type,
                           typename T7=tagged_null_type, typename T8=tagged_null_type, typename T9=tagged_null_type>
class tagged_msg {
    typedef tbb::flow::tuple<T0, T1, T2, T3, T4
                  #if __TBB_VARIADIC_MAX >= 6
                  , T5
                  #endif
                  #if __TBB_VARIADIC_MAX >= 7
                  , T6
                  #endif
                  #if __TBB_VARIADIC_MAX >= 8
                  , T7
                  #endif
                  #if __TBB_VARIADIC_MAX >= 9
                  , T8
                  #endif
                  #if __TBB_VARIADIC_MAX >= 10
                  , T9
                  #endif
                  > Tuple;   

private:
    class variant {
        static const size_t N = tbb::flow::tuple_size<Tuple>::value;
        typedef typename pick_tuple_max<N, Tuple, alignment_of>::type AlignType;
        typedef typename pick_tuple_max<N, Tuple, size_of>::type MaxSizeType;
        static const size_t MaxNBytes = (sizeof(Wrapper<MaxSizeType>)+sizeof(AlignType)-1);
        static const size_t MaxNElements = MaxNBytes/sizeof(AlignType);
        typedef typename tbb::aligned_space<AlignType, MaxNElements> SpaceType;
        SpaceType my_space;
        static const size_t MaxSize = sizeof(SpaceType);

    public:
        variant() { (void) new(&my_space) Wrapper<default_constructed>(default_constructed()); }

        template<typename T>
        variant( const T& x ) {
            do_if<T, is_element_of<T, N, Tuple>::value>::construct(&my_space,x);
        }

        variant(const variant& other) {
            const WrapperBase * h = punned_cast<const WrapperBase *>(&(other.my_space));
            h->CopyTo(&my_space);
        }

        // assignment must destroy and re-create the Wrapper type, as there is no way
        // to create a Wrapper-to-Wrapper assign even if we find they agree in type.
        void operator=( const variant& rhs ) {
            if(&rhs != this) {
                WrapperBase *h = punned_cast<WrapperBase *>(&my_space);
                h->~WrapperBase();
                const WrapperBase *ch = punned_cast<const WrapperBase *>(&(rhs.my_space));
                ch->CopyTo(&my_space);
            }
        }

        template<typename U>
        const U& variant_cast_to() const {
            const Wrapper<U> *h = dynamic_cast<const Wrapper<U>*>(punned_cast<const WrapperBase *>(&my_space));
            if(!h) {
                tbb::internal::throw_exception(tbb::internal::eid_bad_tagged_msg_cast);
            }
            return h->value();
        }
        template<typename U>
        bool variant_is_a() const { return dynamic_cast<const Wrapper<U>*>(punned_cast<const WrapperBase *>(&my_space)) != NULL; }

        bool variant_is_default_constructed() const {return variant_is_a<default_constructed>();}

        ~variant() {
            WrapperBase *h = punned_cast<WrapperBase *>(&my_space);
            h->~WrapperBase();
        }
    }; //class variant

    TagType my_tag;
    variant my_msg;

public:
    tagged_msg(): my_tag(TagType(~0)), my_msg(){} 

    template<typename T, typename R>
    tagged_msg(T const &index, R const &value) : my_tag(index), my_msg(value) {}
    
    #if __TBB_CONST_REF_TO_ARRAY_TEMPLATE_PARAM_BROKEN
    template<typename T, typename R, size_t N>
    tagged_msg(T const &index,  R (&value)[N]) : my_tag(index), my_msg(value) {}
    #endif

    void set_tag(TagType const &index) {my_tag = index;}
    TagType tag() const {return my_tag;}

    template<typename V>
    const V& cast_to() const {return my_msg.template variant_cast_to<V>();}

    template<typename V>
    bool is_a() const {return my_msg.template variant_is_a<V>();}

    bool is_default_constructed() const {return my_msg.variant_is_default_constructed();}
}; //class tagged_msg

// template to simplify cast and test for tagged_msg in template contexts
template<typename T, typename V>
const T& cast_to(V const &v) { return v.template cast_to<T>(); }

template<typename T, typename V>
bool is_a(V const &v) { return v.template is_a<T>(); }

}  // namespace internal

#endif  /* __TBB__flow_graph_types_impl_H */
