// Copyright (C) 2009 Gael Guennebaud <gael.guennebaud@inria.fr>
// Public License v. 2.0. If a copy of the MPL was not distributed

#ifndef EIGEN_STABLENORM_H
#define EIGEN_STABLENORM_H

namespace Eigen { 

namespace internal {
template<typename ExpressionType, typename Scalar>
inline void stable_norm_kernel(const ExpressionType& bl, Scalar& ssq, Scalar& scale, Scalar& invScale)
{
  Scalar max = bl.cwiseAbs().maxCoeff();
  if (max>scale)
  {
    ssq = ssq * abs2(scale/max);
    scale = max;
    invScale = Scalar(1)/scale;
  }
  
  
  ssq += (bl*invScale).squaredNorm();
}
}

template<typename Derived>
inline typename NumTraits<typename internal::traits<Derived>::Scalar>::Real
MatrixBase<Derived>::stableNorm() const
{
  using std::min;
  const Index blockSize = 4096;
  RealScalar scale(0);
  RealScalar invScale(1);
  RealScalar ssq(0); 
  enum {
    Alignment = (int(Flags)&DirectAccessBit) || (int(Flags)&AlignedBit) ? 1 : 0
  };
  Index n = size();
  Index bi = internal::first_aligned(derived());
  if (bi>0)
    internal::stable_norm_kernel(this->head(bi), ssq, scale, invScale);
  for (; bi<n; bi+=blockSize)
    internal::stable_norm_kernel(this->segment(bi,(min)(blockSize, n - bi)).template forceAlignedAccessIf<Alignment>(), ssq, scale, invScale);
  return scale * internal::sqrt(ssq);
}

template<typename Derived>
inline typename NumTraits<typename internal::traits<Derived>::Scalar>::Real
MatrixBase<Derived>::blueNorm() const
{
  using std::pow;
  using std::min;
  using std::max;
  static Index nmax = -1;
  static RealScalar b1, b2, s1m, s2m, overfl, rbig, relerr;
  if(nmax <= 0)
  {
    int nbig, ibeta, it, iemin, iemax, iexp;
    RealScalar abig, eps;
    
    
    
    
    
    
    
    
    nbig  = (std::numeric_limits<Index>::max)();            
    ibeta = std::numeric_limits<RealScalar>::radix;         
    it    = std::numeric_limits<RealScalar>::digits;        
    iemin = std::numeric_limits<RealScalar>::min_exponent;  
    iemax = std::numeric_limits<RealScalar>::max_exponent;  
    rbig  = (std::numeric_limits<RealScalar>::max)();         

    iexp  = -((1-iemin)/2);
    b1    = RealScalar(pow(RealScalar(ibeta),RealScalar(iexp)));  
    iexp  = (iemax + 1 - it)/2;
    b2    = RealScalar(pow(RealScalar(ibeta),RealScalar(iexp)));   

    iexp  = (2-iemin)/2;
    s1m   = RealScalar(pow(RealScalar(ibeta),RealScalar(iexp)));   
    iexp  = - ((iemax+it)/2);
    s2m   = RealScalar(pow(RealScalar(ibeta),RealScalar(iexp)));   

    overfl  = rbig*s2m;             
    eps     = RealScalar(pow(double(ibeta), 1-it));
    relerr  = internal::sqrt(eps);         
    abig    = RealScalar(1.0/eps - 1.0);
    if (RealScalar(nbig)>abig)  nmax = int(abig);  
    else                        nmax = nbig;
  }
  Index n = size();
  RealScalar ab2 = b2 / RealScalar(n);
  RealScalar asml = RealScalar(0);
  RealScalar amed = RealScalar(0);
  RealScalar abig = RealScalar(0);
  for(Index j=0; j<n; ++j)
  {
    RealScalar ax = internal::abs(coeff(j));
    if(ax > ab2)     abig += internal::abs2(ax*s2m);
    else if(ax < b1) asml += internal::abs2(ax*s1m);
    else             amed += internal::abs2(ax);
  }
  if(abig > RealScalar(0))
  {
    abig = internal::sqrt(abig);
    if(abig > overfl)
    {
      eigen_assert(false && "overflow");
      return rbig;
    }
    if(amed > RealScalar(0))
    {
      abig = abig/s2m;
      amed = internal::sqrt(amed);
    }
    else
      return abig/s2m;
  }
  else if(asml > RealScalar(0))
  {
    if (amed > RealScalar(0))
    {
      abig = internal::sqrt(amed);
      amed = internal::sqrt(asml) / s1m;
    }
    else
      return internal::sqrt(asml)/s1m;
  }
  else
    return internal::sqrt(amed);
  asml = (min)(abig, amed);
  abig = (max)(abig, amed);
  if(asml <= abig*relerr)
    return abig;
  else
    return abig * internal::sqrt(RealScalar(1) + internal::abs2(asml/abig));
}

template<typename Derived>
inline typename NumTraits<typename internal::traits<Derived>::Scalar>::Real
MatrixBase<Derived>::hypotNorm() const
{
  return this->cwiseAbs().redux(internal::scalar_hypot_op<RealScalar>());
}

} 

#endif 
