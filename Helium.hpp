#ifndef JMN_HELIUM_INCLUDED
#  define JMN_HELIUM_INCLUDED

#  include <jmn/Hydrogen.hpp>

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

}

#endif // JMN_HELIUM_INCLUDED

#ifdef __INTELLISENSE__
#  define JMN_HELIUM_IMPLEMENTATION
#endif // __INTELLISENSE__

#ifdef JMN_HELIUM_IMPLEMENTATION
#  ifndef JMN_HELIUM_IMPLEMENTATED
#    define JMN_HELIUM_IMPLEMENTATED

namespace jmn
{

}

#  endif // JMN_HELIUM_IMPLEMENTATED
#endif // JMN_HELIUM_IMPLEMENTATION
