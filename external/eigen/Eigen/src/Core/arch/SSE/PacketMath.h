// Copyright (C) 2008-2009 Gael Guennebaud <gael.guennebaud@inria.fr>
// Public License v. 2.0. If a copy of the MPL was not distributed

#ifndef EIGEN_PACKET_MATH_SSE_H
#define EIGEN_PACKET_MATH_SSE_H

namespace Eigen {

namespace internal {

#ifndef EIGEN_CACHEFRIENDLY_PRODUCT_THRESHOLD
#define EIGEN_CACHEFRIENDLY_PRODUCT_THRESHOLD 8
#endif

#ifndef EIGEN_ARCH_DEFAULT_NUMBER_OF_REGISTERS
#define EIGEN_ARCH_DEFAULT_NUMBER_OF_REGISTERS (2*sizeof(void*))
#endif

typedef __m128  Packet4f;
typedef __m128i Packet4i;
typedef __m128d Packet2d;

template<> struct is_arithmetic<__m128>  { enum { value = true }; };
template<> struct is_arithmetic<__m128i> { enum { value = true }; };
template<> struct is_arithmetic<__m128d> { enum { value = true }; };

#define vec4f_swizzle1(v,p,q,r,s) \
  (_mm_castsi128_ps(_mm_shuffle_epi32( _mm_castps_si128(v), ((s)<<6|(r)<<4|(q)<<2|(p)))))

#define vec4i_swizzle1(v,p,q,r,s) \
  (_mm_shuffle_epi32( v, ((s)<<6|(r)<<4|(q)<<2|(p))))

#define vec2d_swizzle1(v,p,q) \
  (_mm_castsi128_pd(_mm_shuffle_epi32( _mm_castpd_si128(v), ((q*2+1)<<6|(q*2)<<4|(p*2+1)<<2|(p*2)))))
  
#define vec4f_swizzle2(a,b,p,q,r,s) \
  (_mm_shuffle_ps( (a), (b), ((s)<<6|(r)<<4|(q)<<2|(p))))

#define vec4i_swizzle2(a,b,p,q,r,s) \
  (_mm_castps_si128( (_mm_shuffle_ps( _mm_castsi128_ps(a), _mm_castsi128_ps(b), ((s)<<6|(r)<<4|(q)<<2|(p))))))

#define _EIGEN_DECLARE_CONST_Packet4f(NAME,X) \
  const Packet4f p4f_##NAME = pset1<Packet4f>(X)

#define _EIGEN_DECLARE_CONST_Packet4f_FROM_INT(NAME,X) \
  const Packet4f p4f_##NAME = _mm_castsi128_ps(pset1<Packet4i>(X))

#define _EIGEN_DECLARE_CONST_Packet4i(NAME,X) \
  const Packet4i p4i_##NAME = pset1<Packet4i>(X)


template<> struct packet_traits<float>  : default_packet_traits
{
  typedef Packet4f type;
  enum {
    Vectorizable = 1,
    AlignedOnScalar = 1,
    size=4,

    HasDiv    = 1,
    HasSin  = EIGEN_FAST_MATH,
    HasCos  = EIGEN_FAST_MATH,
    HasLog  = 1,
    HasExp  = 1,
    HasSqrt = 1
  };
};
template<> struct packet_traits<double> : default_packet_traits
{
  typedef Packet2d type;
  enum {
    Vectorizable = 1,
    AlignedOnScalar = 1,
    size=2,

    HasDiv    = 1
  };
};
template<> struct packet_traits<int>    : default_packet_traits
{
  typedef Packet4i type;
  enum {
    
    Vectorizable = 1,
    AlignedOnScalar = 1,
    size=4
  };
};

template<> struct unpacket_traits<Packet4f> { typedef float  type; enum {size=4}; };
template<> struct unpacket_traits<Packet2d> { typedef double type; enum {size=2}; };
template<> struct unpacket_traits<Packet4i> { typedef int    type; enum {size=4}; };

#if defined(_MSC_VER) && (_MSC_VER==1500)
template<> EIGEN_STRONG_INLINE Packet4f pset1<Packet4f>(const float&  from) { return _mm_set_ps(from,from,from,from); }
template<> EIGEN_STRONG_INLINE Packet2d pset1<Packet2d>(const double& from) { return _mm_set_pd(from,from); }
template<> EIGEN_STRONG_INLINE Packet4i pset1<Packet4i>(const int&    from) { return _mm_set_epi32(from,from,from,from); }
#else
template<> EIGEN_STRONG_INLINE Packet4f pset1<Packet4f>(const float&  from) { return _mm_set1_ps(from); }
template<> EIGEN_STRONG_INLINE Packet2d pset1<Packet2d>(const double& from) { return _mm_set1_pd(from); }
template<> EIGEN_STRONG_INLINE Packet4i pset1<Packet4i>(const int&    from) { return _mm_set1_epi32(from); }
#endif

template<> EIGEN_STRONG_INLINE Packet4f plset<float>(const float& a) { return _mm_add_ps(pset1<Packet4f>(a), _mm_set_ps(3,2,1,0)); }
template<> EIGEN_STRONG_INLINE Packet2d plset<double>(const double& a) { return _mm_add_pd(pset1<Packet2d>(a),_mm_set_pd(1,0)); }
template<> EIGEN_STRONG_INLINE Packet4i plset<int>(const int& a) { return _mm_add_epi32(pset1<Packet4i>(a),_mm_set_epi32(3,2,1,0)); }

template<> EIGEN_STRONG_INLINE Packet4f padd<Packet4f>(const Packet4f& a, const Packet4f& b) { return _mm_add_ps(a,b); }
template<> EIGEN_STRONG_INLINE Packet2d padd<Packet2d>(const Packet2d& a, const Packet2d& b) { return _mm_add_pd(a,b); }
template<> EIGEN_STRONG_INLINE Packet4i padd<Packet4i>(const Packet4i& a, const Packet4i& b) { return _mm_add_epi32(a,b); }

template<> EIGEN_STRONG_INLINE Packet4f psub<Packet4f>(const Packet4f& a, const Packet4f& b) { return _mm_sub_ps(a,b); }
template<> EIGEN_STRONG_INLINE Packet2d psub<Packet2d>(const Packet2d& a, const Packet2d& b) { return _mm_sub_pd(a,b); }
template<> EIGEN_STRONG_INLINE Packet4i psub<Packet4i>(const Packet4i& a, const Packet4i& b) { return _mm_sub_epi32(a,b); }

template<> EIGEN_STRONG_INLINE Packet4f pnegate(const Packet4f& a)
{
  const Packet4f mask = _mm_castsi128_ps(_mm_setr_epi32(0x80000000,0x80000000,0x80000000,0x80000000));
  return _mm_xor_ps(a,mask);
}
template<> EIGEN_STRONG_INLINE Packet2d pnegate(const Packet2d& a)
{
  const Packet2d mask = _mm_castsi128_pd(_mm_setr_epi32(0x0,0x80000000,0x0,0x80000000));
  return _mm_xor_pd(a,mask);
}
template<> EIGEN_STRONG_INLINE Packet4i pnegate(const Packet4i& a)
{
  return psub(_mm_setr_epi32(0,0,0,0), a);
}

template<> EIGEN_STRONG_INLINE Packet4f pmul<Packet4f>(const Packet4f& a, const Packet4f& b) { return _mm_mul_ps(a,b); }
template<> EIGEN_STRONG_INLINE Packet2d pmul<Packet2d>(const Packet2d& a, const Packet2d& b) { return _mm_mul_pd(a,b); }
template<> EIGEN_STRONG_INLINE Packet4i pmul<Packet4i>(const Packet4i& a, const Packet4i& b)
{
#ifdef EIGEN_VECTORIZE_SSE4_1
  return _mm_mullo_epi32(a,b);
#else
  
  return vec4i_swizzle1(
            vec4i_swizzle2(
              _mm_mul_epu32(a,b),
              _mm_mul_epu32(vec4i_swizzle1(a,1,0,3,2),
                            vec4i_swizzle1(b,1,0,3,2)),
              0,2,0,2),
            0,2,1,3);
#endif
}

template<> EIGEN_STRONG_INLINE Packet4f pdiv<Packet4f>(const Packet4f& a, const Packet4f& b) { return _mm_div_ps(a,b); }
template<> EIGEN_STRONG_INLINE Packet2d pdiv<Packet2d>(const Packet2d& a, const Packet2d& b) { return _mm_div_pd(a,b); }
template<> EIGEN_STRONG_INLINE Packet4i pdiv<Packet4i>(const Packet4i& , const Packet4i& )
{ eigen_assert(false && "packet integer division are not supported by SSE");
  return pset1<Packet4i>(0);
}

template<> EIGEN_STRONG_INLINE Packet4i pmadd(const Packet4i& a, const Packet4i& b, const Packet4i& c) { return padd(pmul(a,b), c); }

template<> EIGEN_STRONG_INLINE Packet4f pmin<Packet4f>(const Packet4f& a, const Packet4f& b) { return _mm_min_ps(a,b); }
template<> EIGEN_STRONG_INLINE Packet2d pmin<Packet2d>(const Packet2d& a, const Packet2d& b) { return _mm_min_pd(a,b); }
template<> EIGEN_STRONG_INLINE Packet4i pmin<Packet4i>(const Packet4i& a, const Packet4i& b)
{
  
  Packet4i mask = _mm_cmplt_epi32(a,b);
  return _mm_or_si128(_mm_and_si128(mask,a),_mm_andnot_si128(mask,b));
}

template<> EIGEN_STRONG_INLINE Packet4f pmax<Packet4f>(const Packet4f& a, const Packet4f& b) { return _mm_max_ps(a,b); }
template<> EIGEN_STRONG_INLINE Packet2d pmax<Packet2d>(const Packet2d& a, const Packet2d& b) { return _mm_max_pd(a,b); }
template<> EIGEN_STRONG_INLINE Packet4i pmax<Packet4i>(const Packet4i& a, const Packet4i& b)
{
  
  Packet4i mask = _mm_cmpgt_epi32(a,b);
  return _mm_or_si128(_mm_and_si128(mask,a),_mm_andnot_si128(mask,b));
}

template<> EIGEN_STRONG_INLINE Packet4f pand<Packet4f>(const Packet4f& a, const Packet4f& b) { return _mm_and_ps(a,b); }
template<> EIGEN_STRONG_INLINE Packet2d pand<Packet2d>(const Packet2d& a, const Packet2d& b) { return _mm_and_pd(a,b); }
template<> EIGEN_STRONG_INLINE Packet4i pand<Packet4i>(const Packet4i& a, const Packet4i& b) { return _mm_and_si128(a,b); }

template<> EIGEN_STRONG_INLINE Packet4f por<Packet4f>(const Packet4f& a, const Packet4f& b) { return _mm_or_ps(a,b); }
template<> EIGEN_STRONG_INLINE Packet2d por<Packet2d>(const Packet2d& a, const Packet2d& b) { return _mm_or_pd(a,b); }
template<> EIGEN_STRONG_INLINE Packet4i por<Packet4i>(const Packet4i& a, const Packet4i& b) { return _mm_or_si128(a,b); }

template<> EIGEN_STRONG_INLINE Packet4f pxor<Packet4f>(const Packet4f& a, const Packet4f& b) { return _mm_xor_ps(a,b); }
template<> EIGEN_STRONG_INLINE Packet2d pxor<Packet2d>(const Packet2d& a, const Packet2d& b) { return _mm_xor_pd(a,b); }
template<> EIGEN_STRONG_INLINE Packet4i pxor<Packet4i>(const Packet4i& a, const Packet4i& b) { return _mm_xor_si128(a,b); }

template<> EIGEN_STRONG_INLINE Packet4f pandnot<Packet4f>(const Packet4f& a, const Packet4f& b) { return _mm_andnot_ps(a,b); }
template<> EIGEN_STRONG_INLINE Packet2d pandnot<Packet2d>(const Packet2d& a, const Packet2d& b) { return _mm_andnot_pd(a,b); }
template<> EIGEN_STRONG_INLINE Packet4i pandnot<Packet4i>(const Packet4i& a, const Packet4i& b) { return _mm_andnot_si128(a,b); }

template<> EIGEN_STRONG_INLINE Packet4f pload<Packet4f>(const float*   from) { EIGEN_DEBUG_ALIGNED_LOAD return _mm_load_ps(from); }
template<> EIGEN_STRONG_INLINE Packet2d pload<Packet2d>(const double*  from) { EIGEN_DEBUG_ALIGNED_LOAD return _mm_load_pd(from); }
template<> EIGEN_STRONG_INLINE Packet4i pload<Packet4i>(const int*     from) { EIGEN_DEBUG_ALIGNED_LOAD return _mm_load_si128(reinterpret_cast<const Packet4i*>(from)); }

#if defined(_MSC_VER)
  template<> EIGEN_STRONG_INLINE Packet4f ploadu<Packet4f>(const float*  from) {
    EIGEN_DEBUG_UNALIGNED_LOAD
    #if (_MSC_VER==1600)
    
    
    
    
    __m128 res = _mm_loadl_pi(_mm_set1_ps(0.0f), (const __m64*)(from));
    res = _mm_loadh_pi(res, (const __m64*)(from+2));
    return res;
    #else
    return _mm_loadu_ps(from);
    #endif
  }
  template<> EIGEN_STRONG_INLINE Packet2d ploadu<Packet2d>(const double* from) { EIGEN_DEBUG_UNALIGNED_LOAD return _mm_loadu_pd(from); }
  template<> EIGEN_STRONG_INLINE Packet4i ploadu<Packet4i>(const int*    from) { EIGEN_DEBUG_UNALIGNED_LOAD return _mm_loadu_si128(reinterpret_cast<const Packet4i*>(from)); }
#else

#if defined(__GNUC__) && defined(__i386__)
  
  #define EIGEN_AVOID_CUSTOM_UNALIGNED_LOADS 1
#elif defined(__clang__)
  
  #define EIGEN_AVOID_CUSTOM_UNALIGNED_LOADS 1
#else
  #define EIGEN_AVOID_CUSTOM_UNALIGNED_LOADS 0
#endif

template<> EIGEN_STRONG_INLINE Packet4f ploadu<Packet4f>(const float* from)
{
  EIGEN_DEBUG_UNALIGNED_LOAD
#if EIGEN_AVOID_CUSTOM_UNALIGNED_LOADS
  return _mm_loadu_ps(from);
#else
  __m128d res;
  res =  _mm_load_sd((const double*)(from)) ;
  res =  _mm_loadh_pd(res, (const double*)(from+2)) ;
  return _mm_castpd_ps(res);
#endif
}
template<> EIGEN_STRONG_INLINE Packet2d ploadu<Packet2d>(const double* from)
{
  EIGEN_DEBUG_UNALIGNED_LOAD
#if EIGEN_AVOID_CUSTOM_UNALIGNED_LOADS
  return _mm_loadu_pd(from);
#else
  __m128d res;
  res = _mm_load_sd(from) ;
  res = _mm_loadh_pd(res,from+1);
  return res;
#endif
}
template<> EIGEN_STRONG_INLINE Packet4i ploadu<Packet4i>(const int* from)
{
  EIGEN_DEBUG_UNALIGNED_LOAD
#if EIGEN_AVOID_CUSTOM_UNALIGNED_LOADS
  return _mm_loadu_si128(reinterpret_cast<const Packet4i*>(from));
#else
  __m128d res;
  res =  _mm_load_sd((const double*)(from)) ;
  res =  _mm_loadh_pd(res, (const double*)(from+2)) ;
  return _mm_castpd_si128(res);
#endif
}
#endif

template<> EIGEN_STRONG_INLINE Packet4f ploaddup<Packet4f>(const float*   from)
{
  return vec4f_swizzle1(_mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(from))), 0, 0, 1, 1);
}
template<> EIGEN_STRONG_INLINE Packet2d ploaddup<Packet2d>(const double*  from)
{ return pset1<Packet2d>(from[0]); }
template<> EIGEN_STRONG_INLINE Packet4i ploaddup<Packet4i>(const int*     from)
{
  Packet4i tmp;
  tmp = _mm_loadl_epi64(reinterpret_cast<const Packet4i*>(from));
  return vec4i_swizzle1(tmp, 0, 0, 1, 1);
}

template<> EIGEN_STRONG_INLINE void pstore<float>(float*   to, const Packet4f& from) { EIGEN_DEBUG_ALIGNED_STORE _mm_store_ps(to, from); }
template<> EIGEN_STRONG_INLINE void pstore<double>(double* to, const Packet2d& from) { EIGEN_DEBUG_ALIGNED_STORE _mm_store_pd(to, from); }
template<> EIGEN_STRONG_INLINE void pstore<int>(int*       to, const Packet4i& from) { EIGEN_DEBUG_ALIGNED_STORE _mm_store_si128(reinterpret_cast<Packet4i*>(to), from); }

template<> EIGEN_STRONG_INLINE void pstoreu<double>(double* to, const Packet2d& from) {
  EIGEN_DEBUG_UNALIGNED_STORE
  _mm_storel_pd((to), from);
  _mm_storeh_pd((to+1), from);
}
template<> EIGEN_STRONG_INLINE void pstoreu<float>(float*  to, const Packet4f& from) { EIGEN_DEBUG_UNALIGNED_STORE pstoreu(reinterpret_cast<double*>(to), _mm_castps_pd(from)); }
template<> EIGEN_STRONG_INLINE void pstoreu<int>(int*      to, const Packet4i& from) { EIGEN_DEBUG_UNALIGNED_STORE pstoreu(reinterpret_cast<double*>(to), _mm_castsi128_pd(from)); }

template<> EIGEN_STRONG_INLINE void pstore1<Packet4f>(float* to, const float& a)
{
  Packet4f pa = _mm_set_ss(a);
  pstore(to, vec4f_swizzle1(pa,0,0,0,0));
}
template<> EIGEN_STRONG_INLINE void pstore1<Packet2d>(double* to, const double& a)
{
  Packet2d pa = _mm_set_sd(a);
  pstore(to, vec2d_swizzle1(pa,0,0));
}

template<> EIGEN_STRONG_INLINE void prefetch<float>(const float*   addr) { _mm_prefetch((const char*)(addr), _MM_HINT_T0); }
template<> EIGEN_STRONG_INLINE void prefetch<double>(const double* addr) { _mm_prefetch((const char*)(addr), _MM_HINT_T0); }
template<> EIGEN_STRONG_INLINE void prefetch<int>(const int*       addr) { _mm_prefetch((const char*)(addr), _MM_HINT_T0); }

#if defined(_MSC_VER) && defined(_WIN64) && !defined(__INTEL_COMPILER)
template<> EIGEN_STRONG_INLINE float  pfirst<Packet4f>(const Packet4f& a) { return a.m128_f32[0]; }
template<> EIGEN_STRONG_INLINE double pfirst<Packet2d>(const Packet2d& a) { return a.m128d_f64[0]; }
template<> EIGEN_STRONG_INLINE int    pfirst<Packet4i>(const Packet4i& a) { int x = _mm_cvtsi128_si32(a); return x; }
#elif defined(_MSC_VER) && !defined(__INTEL_COMPILER)
template<> EIGEN_STRONG_INLINE float  pfirst<Packet4f>(const Packet4f& a) { float x = _mm_cvtss_f32(a); return x; }
template<> EIGEN_STRONG_INLINE double pfirst<Packet2d>(const Packet2d& a) { double x = _mm_cvtsd_f64(a); return x; }
template<> EIGEN_STRONG_INLINE int    pfirst<Packet4i>(const Packet4i& a) { int x = _mm_cvtsi128_si32(a); return x; }
#else
template<> EIGEN_STRONG_INLINE float  pfirst<Packet4f>(const Packet4f& a) { return _mm_cvtss_f32(a); }
template<> EIGEN_STRONG_INLINE double pfirst<Packet2d>(const Packet2d& a) { return _mm_cvtsd_f64(a); }
template<> EIGEN_STRONG_INLINE int    pfirst<Packet4i>(const Packet4i& a) { return _mm_cvtsi128_si32(a); }
#endif

template<> EIGEN_STRONG_INLINE Packet4f preverse(const Packet4f& a)
{ return _mm_shuffle_ps(a,a,0x1B); }
template<> EIGEN_STRONG_INLINE Packet2d preverse(const Packet2d& a)
{ return _mm_shuffle_pd(a,a,0x1); }
template<> EIGEN_STRONG_INLINE Packet4i preverse(const Packet4i& a)
{ return _mm_shuffle_epi32(a,0x1B); }


template<> EIGEN_STRONG_INLINE Packet4f pabs(const Packet4f& a)
{
  const Packet4f mask = _mm_castsi128_ps(_mm_setr_epi32(0x7FFFFFFF,0x7FFFFFFF,0x7FFFFFFF,0x7FFFFFFF));
  return _mm_and_ps(a,mask);
}
template<> EIGEN_STRONG_INLINE Packet2d pabs(const Packet2d& a)
{
  const Packet2d mask = _mm_castsi128_pd(_mm_setr_epi32(0xFFFFFFFF,0x7FFFFFFF,0xFFFFFFFF,0x7FFFFFFF));
  return _mm_and_pd(a,mask);
}
template<> EIGEN_STRONG_INLINE Packet4i pabs(const Packet4i& a)
{
  #ifdef EIGEN_VECTORIZE_SSSE3
  return _mm_abs_epi32(a);
  #else
  Packet4i aux = _mm_srai_epi32(a,31);
  return _mm_sub_epi32(_mm_xor_si128(a,aux),aux);
  #endif
}

EIGEN_STRONG_INLINE void punpackp(Packet4f* vecs)
{
  vecs[1] = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(vecs[0]), 0x55));
  vecs[2] = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(vecs[0]), 0xAA));
  vecs[3] = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(vecs[0]), 0xFF));
  vecs[0] = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(vecs[0]), 0x00));
}

#ifdef EIGEN_VECTORIZE_SSE3
template<> EIGEN_STRONG_INLINE Packet4f preduxp<Packet4f>(const Packet4f* vecs)
{
  return _mm_hadd_ps(_mm_hadd_ps(vecs[0], vecs[1]),_mm_hadd_ps(vecs[2], vecs[3]));
}
template<> EIGEN_STRONG_INLINE Packet2d preduxp<Packet2d>(const Packet2d* vecs)
{
  return _mm_hadd_pd(vecs[0], vecs[1]);
}

template<> EIGEN_STRONG_INLINE float predux<Packet4f>(const Packet4f& a)
{
  Packet4f tmp0 = _mm_hadd_ps(a,a);
  return pfirst(_mm_hadd_ps(tmp0, tmp0));
}

template<> EIGEN_STRONG_INLINE double predux<Packet2d>(const Packet2d& a) { return pfirst(_mm_hadd_pd(a, a)); }

#else
template<> EIGEN_STRONG_INLINE float predux<Packet4f>(const Packet4f& a)
{
  Packet4f tmp = _mm_add_ps(a, _mm_movehl_ps(a,a));
  return pfirst(_mm_add_ss(tmp, _mm_shuffle_ps(tmp,tmp, 1)));
}
template<> EIGEN_STRONG_INLINE double predux<Packet2d>(const Packet2d& a)
{
  return pfirst(_mm_add_sd(a, _mm_unpackhi_pd(a,a)));
}

template<> EIGEN_STRONG_INLINE Packet4f preduxp<Packet4f>(const Packet4f* vecs)
{
  Packet4f tmp0, tmp1, tmp2;
  tmp0 = _mm_unpacklo_ps(vecs[0], vecs[1]);
  tmp1 = _mm_unpackhi_ps(vecs[0], vecs[1]);
  tmp2 = _mm_unpackhi_ps(vecs[2], vecs[3]);
  tmp0 = _mm_add_ps(tmp0, tmp1);
  tmp1 = _mm_unpacklo_ps(vecs[2], vecs[3]);
  tmp1 = _mm_add_ps(tmp1, tmp2);
  tmp2 = _mm_movehl_ps(tmp1, tmp0);
  tmp0 = _mm_movelh_ps(tmp0, tmp1);
  return _mm_add_ps(tmp0, tmp2);
}

template<> EIGEN_STRONG_INLINE Packet2d preduxp<Packet2d>(const Packet2d* vecs)
{
  return _mm_add_pd(_mm_unpacklo_pd(vecs[0], vecs[1]), _mm_unpackhi_pd(vecs[0], vecs[1]));
}
#endif  

template<> EIGEN_STRONG_INLINE int predux<Packet4i>(const Packet4i& a)
{
  Packet4i tmp = _mm_add_epi32(a, _mm_unpackhi_epi64(a,a));
  return pfirst(tmp) + pfirst(_mm_shuffle_epi32(tmp, 1));
}

template<> EIGEN_STRONG_INLINE Packet4i preduxp<Packet4i>(const Packet4i* vecs)
{
  Packet4i tmp0, tmp1, tmp2;
  tmp0 = _mm_unpacklo_epi32(vecs[0], vecs[1]);
  tmp1 = _mm_unpackhi_epi32(vecs[0], vecs[1]);
  tmp2 = _mm_unpackhi_epi32(vecs[2], vecs[3]);
  tmp0 = _mm_add_epi32(tmp0, tmp1);
  tmp1 = _mm_unpacklo_epi32(vecs[2], vecs[3]);
  tmp1 = _mm_add_epi32(tmp1, tmp2);
  tmp2 = _mm_unpacklo_epi64(tmp0, tmp1);
  tmp0 = _mm_unpackhi_epi64(tmp0, tmp1);
  return _mm_add_epi32(tmp0, tmp2);
}


template<> EIGEN_STRONG_INLINE float predux_mul<Packet4f>(const Packet4f& a)
{
  Packet4f tmp = _mm_mul_ps(a, _mm_movehl_ps(a,a));
  return pfirst(_mm_mul_ss(tmp, _mm_shuffle_ps(tmp,tmp, 1)));
}
template<> EIGEN_STRONG_INLINE double predux_mul<Packet2d>(const Packet2d& a)
{
  return pfirst(_mm_mul_sd(a, _mm_unpackhi_pd(a,a)));
}
template<> EIGEN_STRONG_INLINE int predux_mul<Packet4i>(const Packet4i& a)
{
  
  
  
  EIGEN_ALIGN16 int aux[4];
  pstore(aux, a);
  return  (aux[0] * aux[1]) * (aux[2] * aux[3]);;
}

template<> EIGEN_STRONG_INLINE float predux_min<Packet4f>(const Packet4f& a)
{
  Packet4f tmp = _mm_min_ps(a, _mm_movehl_ps(a,a));
  return pfirst(_mm_min_ss(tmp, _mm_shuffle_ps(tmp,tmp, 1)));
}
template<> EIGEN_STRONG_INLINE double predux_min<Packet2d>(const Packet2d& a)
{
  return pfirst(_mm_min_sd(a, _mm_unpackhi_pd(a,a)));
}
template<> EIGEN_STRONG_INLINE int predux_min<Packet4i>(const Packet4i& a)
{
  
  
  EIGEN_ALIGN16 int aux[4];
  pstore(aux, a);
  register int aux0 = aux[0]<aux[1] ? aux[0] : aux[1];
  register int aux2 = aux[2]<aux[3] ? aux[2] : aux[3];
  return aux0<aux2 ? aux0 : aux2;
}

template<> EIGEN_STRONG_INLINE float predux_max<Packet4f>(const Packet4f& a)
{
  Packet4f tmp = _mm_max_ps(a, _mm_movehl_ps(a,a));
  return pfirst(_mm_max_ss(tmp, _mm_shuffle_ps(tmp,tmp, 1)));
}
template<> EIGEN_STRONG_INLINE double predux_max<Packet2d>(const Packet2d& a)
{
  return pfirst(_mm_max_sd(a, _mm_unpackhi_pd(a,a)));
}
template<> EIGEN_STRONG_INLINE int predux_max<Packet4i>(const Packet4i& a)
{
  
  
  EIGEN_ALIGN16 int aux[4];
  pstore(aux, a);
  register int aux0 = aux[0]>aux[1] ? aux[0] : aux[1];
  register int aux2 = aux[2]>aux[3] ? aux[2] : aux[3];
  return aux0>aux2 ? aux0 : aux2;
}

#if (defined __GNUC__)
#endif

#ifdef EIGEN_VECTORIZE_SSSE3
template<int Offset>
struct palign_impl<Offset,Packet4f>
{
  static EIGEN_STRONG_INLINE void run(Packet4f& first, const Packet4f& second)
  {
    if (Offset!=0)
      first = _mm_castsi128_ps(_mm_alignr_epi8(_mm_castps_si128(second), _mm_castps_si128(first), Offset*4));
  }
};

template<int Offset>
struct palign_impl<Offset,Packet4i>
{
  static EIGEN_STRONG_INLINE void run(Packet4i& first, const Packet4i& second)
  {
    if (Offset!=0)
      first = _mm_alignr_epi8(second,first, Offset*4);
  }
};

template<int Offset>
struct palign_impl<Offset,Packet2d>
{
  static EIGEN_STRONG_INLINE void run(Packet2d& first, const Packet2d& second)
  {
    if (Offset==1)
      first = _mm_castsi128_pd(_mm_alignr_epi8(_mm_castpd_si128(second), _mm_castpd_si128(first), 8));
  }
};
#else
template<int Offset>
struct palign_impl<Offset,Packet4f>
{
  static EIGEN_STRONG_INLINE void run(Packet4f& first, const Packet4f& second)
  {
    if (Offset==1)
    {
      first = _mm_move_ss(first,second);
      first = _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(first),0x39));
    }
    else if (Offset==2)
    {
      first = _mm_movehl_ps(first,first);
      first = _mm_movelh_ps(first,second);
    }
    else if (Offset==3)
    {
      first = _mm_move_ss(first,second);
      first = _mm_shuffle_ps(first,second,0x93);
    }
  }
};

template<int Offset>
struct palign_impl<Offset,Packet4i>
{
  static EIGEN_STRONG_INLINE void run(Packet4i& first, const Packet4i& second)
  {
    if (Offset==1)
    {
      first = _mm_castps_si128(_mm_move_ss(_mm_castsi128_ps(first),_mm_castsi128_ps(second)));
      first = _mm_shuffle_epi32(first,0x39);
    }
    else if (Offset==2)
    {
      first = _mm_castps_si128(_mm_movehl_ps(_mm_castsi128_ps(first),_mm_castsi128_ps(first)));
      first = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(first),_mm_castsi128_ps(second)));
    }
    else if (Offset==3)
    {
      first = _mm_castps_si128(_mm_move_ss(_mm_castsi128_ps(first),_mm_castsi128_ps(second)));
      first = _mm_castps_si128(_mm_shuffle_ps(_mm_castsi128_ps(first),_mm_castsi128_ps(second),0x93));
    }
  }
};

template<int Offset>
struct palign_impl<Offset,Packet2d>
{
  static EIGEN_STRONG_INLINE void run(Packet2d& first, const Packet2d& second)
  {
    if (Offset==1)
    {
      first = _mm_castps_pd(_mm_movehl_ps(_mm_castpd_ps(first),_mm_castpd_ps(first)));
      first = _mm_castps_pd(_mm_movelh_ps(_mm_castpd_ps(first),_mm_castpd_ps(second)));
    }
  }
};
#endif

} 

} 

#endif 