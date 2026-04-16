#ifndef JMN_HELIUM_INCLUDED
#  define JMN_HELIUM_INCLUDED

#  define _USE_MATH_DEFINES

#  include <math.h>
#  include <cmath>

#  include <Hydrogen.hpp>

namespace jmn
{

    template<typename T, Size N> union Vector
    {
        T dat[N];
    };

    template<typename T, Size M, Size N> union Matrix
    {
        T dat[M * N];
        T mtx[N][M];
    };

    template<typename T> union Vector<T, 2>
    {
        struct { T dat[2]; };
        struct { T x, y; };

        Vector() : x(T(0)), y(T(0)) {}
        Vector(T x, T y) : x(x), y(y) {}
    };

    template<typename T> union Vector<T, 3>
    {
        struct { T dat[3]; };
        struct { T x, y, z; };
    };

    template<typename T> union Vector<T, 4>
    {
        struct { T dat[4]; };
        struct { T x, y, z, w; };
    };

    template<typename T> using Vector2 = Vector<T, 2>;
    template<typename T> using Vector3 = Vector<T, 3>;
    template<typename T> using Vector4 = Vector<T, 4>;

    using V2U8  = Vector2<U8>;
    using V2U16 = Vector2<U16>;
    using V2U32 = Vector2<U32>;
    using V2U64 = Vector2<U64>;
    using V2S8  = Vector2<S8>;
    using V2S16 = Vector2<S16>;
    using V2S32 = Vector2<S32>;
    using V2S64 = Vector2<S64>;
    using V2F32 = Vector2<F32>;
    using V2F64 = Vector2<F64>;

    using V3U8  = Vector3<U8>;
    using V3U16 = Vector3<U16>;
    using V3U32 = Vector3<U32>;
    using V3U64 = Vector3<U64>;
    using V3S8  = Vector3<S8>;
    using V3S16 = Vector3<S16>;
    using V3S32 = Vector3<S32>;
    using V3S64 = Vector3<S64>;
    using V3F32 = Vector3<F32>;
    using V3F64 = Vector3<F64>;

    using V4U8  = Vector4<U8>;
    using V4U16 = Vector4<U16>;
    using V4U32 = Vector4<U32>;
    using V4U64 = Vector4<U64>;
    using V4S8  = Vector4<S8>;
    using V4S16 = Vector4<S16>;
    using V4S32 = Vector4<S32>;
    using V4S64 = Vector4<S64>;
    using V4F32 = Vector4<F32>;
    using V4F64 = Vector4<F64>;

    S8  MapNormalizedS8 (F32);
    S16 MapNormalizedS16(F32);

    U8  MapNormalizedU8 (F32);
    U16 MapNormalizedU16(F32);

}

#endif // JMN_HELIUM_INCLUDED

#ifdef __INTELLISENSE__
#  define JMN_HELIUM_IMPLEMENTATION
#endif // __INTELLISENSE__

#ifdef JMN_HELIUM_IMPLEMENTATION
#  ifndef JMN_HELIUM_IMPLEMENTATED
#    define JMN_HELIUM_IMPLEMENTATED

#    ifdef JMN_HELIUM_SINGLE_FILE
#      define JMN_HELIUM_IMPL_INLINE JMN_INLINE
#    else
#      define JMN_HELIUM_IMPL_INLINE
#    endif // JMN_HELIUM_SINGLE_FILE

namespace jmn
{

    JMN_HELIUM_IMPL_INLINE S8 MapNormalizedS8(F32 x)
    {
        // IDK if this works, more testing needed
        if (x < -1.0f) return (S8)INT8_MIN;
        if (x > +1.0f) return (S8)INT8_MAX;
        if (x <  0.0f) return (S8)(x * -(F32)INT8_MIN);
        if (x >  0.0f) return (S8)(x *  (F32)INT8_MAX);
        return (S8)0;
    }

    JMN_HELIUM_IMPL_INLINE S16 MapNormalizedS16(F32 x)
    {
        // IDK if this works, more testing needed
        if (x < -1.0f) return (S16)INT16_MIN;
        if (x > +1.0f) return (S16)INT16_MAX;
        if (x <  0.0f) return (S16)(x * -(F32)INT16_MIN);
        if (x >  0.0f) return (S16)(x *  (F32)INT16_MAX);
        return (S16)0;
    }

    JMN_HELIUM_IMPL_INLINE U8 MapNormalizedU8(F32 x)
    {
        // IDK if this works, more testing needed
        if (x <  0.0f) return (U8)0;
        if (x >  1.0f) return (U8)UINT8_MAX;
        return (U8)(x * (F32)UINT8_MAX);
    }

    JMN_HELIUM_IMPL_INLINE U16 MapNormalizedU16(F32 x)
    {
        // IDK if this works, more testing needed
        if (x <  0.0f) return (U16)0;
        if (x >  1.0f) return (U16)UINT16_MAX;
        return (U16)(x * (F32)UINT16_MAX);
    }

}

#  endif // JMN_HELIUM_IMPLEMENTATED
#endif // JMN_HELIUM_IMPLEMENTATION
