#ifndef JMN_HELIUM_INCLUDED
#  define JMN_HELIUM_INCLUDED

#  define _USE_MATH_DEFINES

#  include <math.h>
#  include <intrin.h>
#  include <cmath>

#  include <Hydrogen.hpp>

namespace jmn
{

    enum class ComplexForm
    {
        Rectangular = 1,
        Polar       = 2,
    };

    template<typename T, Size N> union Vector
    {
        struct { T data[N]; };

        JMN_INLINE Vector() : data{} {}
        JMN_INLINE Vector(T const &s) : data() { for (Size i = 0; i < N; ++i) data[i] = s; }
    };

    template<typename T, Size M, Size N> union Matrix
    {
        struct { T data[M * N]; };
        struct { T mtx[N][M]; };

        JMN_INLINE Matrix() : data{} {}
    };

    template<typename T, ComplexForm F> union Complex
    {
        struct { T data[2]; };

        JMN_INLINE Complex() : data{} {}
    };

    template<typename T> union Vector<T, 2>
    {
        struct { T data[2]; };
        struct { T x, y; };

        JMN_INLINE Vector() : data{} {}
        JMN_INLINE Vector(T const &s) : x(s), y(s) {}
        JMN_INLINE Vector(T const &x, T const &y) : x(x), y(y) {}
    };

    template<typename T> union Vector<T, 3>
    {
        struct { T data[3]; };
        struct { T x, y, z; };
        struct { T r, g, b; };

        JMN_INLINE Vector() : data{}  {}
        JMN_INLINE Vector(T const &s) : x(s), y(s), z(s) {}
        JMN_INLINE Vector(T const &x, T const &y, T const &z) : x(x), y(y), z(z) {}
    };

    template<typename T> union Vector<T, 4>
    {
        struct { T data[4]; };
        struct { T x, y, z, w; };
        struct { T r, g, b, a; };

        JMN_INLINE Vector() : data{} {}
        JMN_INLINE Vector(T const &s) : x(s), y(s), z(s), w(s) {}
        JMN_INLINE Vector(T const &x, T const &y, T const &z, T const &w) : x(x), y(y), z(z), w(w) {}
    };

    template<> union Vector<F32, 4>
    {
        struct { F32    data[4]; };
        struct { F32    x, y, z, w; };
        struct { F32    r, g, b, a; };
        struct { __m128 m128; };

        JMN_INLINE Vector() : m128(_mm_setzero_ps()) {}
        JMN_INLINE Vector(F32 s) : m128(_mm_set1_ps(s)) {}
        JMN_INLINE Vector(F32 x, F32 y, F32 z, F32 w) : m128(_mm_set_ps(w, z, y, x)) {}
    };

    template<typename T> union Complex<T, ComplexForm::Rectangular>
    {
        struct { T data[2]; };
        struct { T a, b; };
    };

    template<typename T> union Complex<T, ComplexForm::Polar>
    {
        struct { T data[2]; };
        struct { T r, p; };
    };

    template<typename T> using Vector2 = Vector<T, 2>;
    template<typename T> using Vector3 = Vector<T, 3>;
    template<typename T> using Vector4 = Vector<T, 4>;

    template<typename T> using ComplexR = Complex<T, ComplexForm::Rectangular>;
    template<typename T> using ComplexP = Complex<T, ComplexForm::Polar>;

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

    using CRF32 = ComplexR<F32>;
    using CRF64 = ComplexR<F64>;

    using CPF32 = ComplexP<F32>;
    using CPF64 = ComplexP<F64>;

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
