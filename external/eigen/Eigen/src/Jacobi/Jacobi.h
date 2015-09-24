// Copyright (C) 2009 Benoit Jacob <jacob.benoit.1@gmail.com>
// Copyright (C) 2009 Gael Guennebaud <gael.guennebaud@inria.fr>
// Public License v. 2.0. If a copy of the MPL was not distributed

#ifndef EIGEN_JACOBI_H
#define EIGEN_JACOBI_H

namespace Eigen { 

template<typename Scalar> class JacobiRotation
{
  public:
    typedef typename NumTraits<Scalar>::Real RealScalar;

    
    JacobiRotation() {}

    
    JacobiRotation(const Scalar& c, const Scalar& s) : m_c(c), m_s(s) {}

    Scalar& c() { return m_c; }
    Scalar c() const { return m_c; }
    Scalar& s() { return m_s; }
    Scalar s() const { return m_s; }

    
    JacobiRotation operator*(const JacobiRotation& other)
    {
      return JacobiRotation(m_c * other.m_c - internal::conj(m_s) * other.m_s,
                            internal::conj(m_c * internal::conj(other.m_s) + internal::conj(m_s) * internal::conj(other.m_c)));
    }

    
    JacobiRotation transpose() const { return JacobiRotation(m_c, -internal::conj(m_s)); }

    
    JacobiRotation adjoint() const { return JacobiRotation(internal::conj(m_c), -m_s); }

    template<typename Derived>
    bool makeJacobi(const MatrixBase<Derived>&, typename Derived::Index p, typename Derived::Index q);
    bool makeJacobi(RealScalar x, Scalar y, RealScalar z);

    void makeGivens(const Scalar& p, const Scalar& q, Scalar* z=0);

  protected:
    void makeGivens(const Scalar& p, const Scalar& q, Scalar* z, internal::true_type);
    void makeGivens(const Scalar& p, const Scalar& q, Scalar* z, internal::false_type);

    Scalar m_c, m_s;
};

template<typename Scalar>
bool JacobiRotation<Scalar>::makeJacobi(RealScalar x, Scalar y, RealScalar z)
{
  typedef typename NumTraits<Scalar>::Real RealScalar;
  if(y == Scalar(0))
  {
    m_c = Scalar(1);
    m_s = Scalar(0);
    return false;
  }
  else
  {
    RealScalar tau = (x-z)/(RealScalar(2)*internal::abs(y));
    RealScalar w = internal::sqrt(internal::abs2(tau) + RealScalar(1));
    RealScalar t;
    if(tau>RealScalar(0))
    {
      t = RealScalar(1) / (tau + w);
    }
    else
    {
      t = RealScalar(1) / (tau - w);
    }
    RealScalar sign_t = t > RealScalar(0) ? RealScalar(1) : RealScalar(-1);
    RealScalar n = RealScalar(1) / internal::sqrt(internal::abs2(t)+RealScalar(1));
    m_s = - sign_t * (internal::conj(y) / internal::abs(y)) * internal::abs(t) * n;
    m_c = n;
    return true;
  }
}

template<typename Scalar>
template<typename Derived>
inline bool JacobiRotation<Scalar>::makeJacobi(const MatrixBase<Derived>& m, typename Derived::Index p, typename Derived::Index q)
{
  return makeJacobi(internal::real(m.coeff(p,p)), m.coeff(p,q), internal::real(m.coeff(q,q)));
}

template<typename Scalar>
void JacobiRotation<Scalar>::makeGivens(const Scalar& p, const Scalar& q, Scalar* z)
{
  makeGivens(p, q, z, typename internal::conditional<NumTraits<Scalar>::IsComplex, internal::true_type, internal::false_type>::type());
}


template<typename Scalar>
void JacobiRotation<Scalar>::makeGivens(const Scalar& p, const Scalar& q, Scalar* r, internal::true_type)
{
  if(q==Scalar(0))
  {
    m_c = internal::real(p)<0 ? Scalar(-1) : Scalar(1);
    m_s = 0;
    if(r) *r = m_c * p;
  }
  else if(p==Scalar(0))
  {
    m_c = 0;
    m_s = -q/internal::abs(q);
    if(r) *r = internal::abs(q);
  }
  else
  {
    RealScalar p1 = internal::norm1(p);
    RealScalar q1 = internal::norm1(q);
    if(p1>=q1)
    {
      Scalar ps = p / p1;
      RealScalar p2 = internal::abs2(ps);
      Scalar qs = q / p1;
      RealScalar q2 = internal::abs2(qs);

      RealScalar u = internal::sqrt(RealScalar(1) + q2/p2);
      if(internal::real(p)<RealScalar(0))
        u = -u;

      m_c = Scalar(1)/u;
      m_s = -qs*internal::conj(ps)*(m_c/p2);
      if(r) *r = p * u;
    }
    else
    {
      Scalar ps = p / q1;
      RealScalar p2 = internal::abs2(ps);
      Scalar qs = q / q1;
      RealScalar q2 = internal::abs2(qs);

      RealScalar u = q1 * internal::sqrt(p2 + q2);
      if(internal::real(p)<RealScalar(0))
        u = -u;

      p1 = internal::abs(p);
      ps = p/p1;
      m_c = p1/u;
      m_s = -internal::conj(ps) * (q/u);
      if(r) *r = ps * u;
    }
  }
}

template<typename Scalar>
void JacobiRotation<Scalar>::makeGivens(const Scalar& p, const Scalar& q, Scalar* r, internal::false_type)
{

  if(q==Scalar(0))
  {
    m_c = p<Scalar(0) ? Scalar(-1) : Scalar(1);
    m_s = Scalar(0);
    if(r) *r = internal::abs(p);
  }
  else if(p==Scalar(0))
  {
    m_c = Scalar(0);
    m_s = q<Scalar(0) ? Scalar(1) : Scalar(-1);
    if(r) *r = internal::abs(q);
  }
  else if(internal::abs(p) > internal::abs(q))
  {
    Scalar t = q/p;
    Scalar u = internal::sqrt(Scalar(1) + internal::abs2(t));
    if(p<Scalar(0))
      u = -u;
    m_c = Scalar(1)/u;
    m_s = -t * m_c;
    if(r) *r = p * u;
  }
  else
  {
    Scalar t = p/q;
    Scalar u = internal::sqrt(Scalar(1) + internal::abs2(t));
    if(q<Scalar(0))
      u = -u;
    m_s = -Scalar(1)/u;
    m_c = -t * m_s;
    if(r) *r = q * u;
  }

}


namespace internal {
template<typename VectorX, typename VectorY, typename OtherScalar>
void apply_rotation_in_the_plane(VectorX& _x, VectorY& _y, const JacobiRotation<OtherScalar>& j);
}

template<typename Derived>
template<typename OtherScalar>
inline void MatrixBase<Derived>::applyOnTheLeft(Index p, Index q, const JacobiRotation<OtherScalar>& j)
{
  RowXpr x(this->row(p));
  RowXpr y(this->row(q));
  internal::apply_rotation_in_the_plane(x, y, j);
}

template<typename Derived>
template<typename OtherScalar>
inline void MatrixBase<Derived>::applyOnTheRight(Index p, Index q, const JacobiRotation<OtherScalar>& j)
{
  ColXpr x(this->col(p));
  ColXpr y(this->col(q));
  internal::apply_rotation_in_the_plane(x, y, j.transpose());
}

namespace internal {
template<typename VectorX, typename VectorY, typename OtherScalar>
void  apply_rotation_in_the_plane(VectorX& _x, VectorY& _y, const JacobiRotation<OtherScalar>& j)
{
  typedef typename VectorX::Index Index;
  typedef typename VectorX::Scalar Scalar;
  enum { PacketSize = packet_traits<Scalar>::size };
  typedef typename packet_traits<Scalar>::type Packet;
  eigen_assert(_x.size() == _y.size());
  Index size = _x.size();
  Index incrx = _x.innerStride();
  Index incry = _y.innerStride();

  Scalar* EIGEN_RESTRICT x = &_x.coeffRef(0);
  Scalar* EIGEN_RESTRICT y = &_y.coeffRef(0);

  

  if(VectorX::SizeAtCompileTime == Dynamic &&
    (VectorX::Flags & VectorY::Flags & PacketAccessBit) &&
    ((incrx==1 && incry==1) || PacketSize == 1))
  {
    
    enum { Peeling = 2 };

    Index alignedStart = internal::first_aligned(y, size);
    Index alignedEnd = alignedStart + ((size-alignedStart)/PacketSize)*PacketSize;

    const Packet pc = pset1<Packet>(j.c());
    const Packet ps = pset1<Packet>(j.s());
    conj_helper<Packet,Packet,NumTraits<Scalar>::IsComplex,false> pcj;

    for(Index i=0; i<alignedStart; ++i)
    {
      Scalar xi = x[i];
      Scalar yi = y[i];
      x[i] =  j.c() * xi + conj(j.s()) * yi;
      y[i] = -j.s() * xi + conj(j.c()) * yi;
    }

    Scalar* EIGEN_RESTRICT px = x + alignedStart;
    Scalar* EIGEN_RESTRICT py = y + alignedStart;

    if(internal::first_aligned(x, size)==alignedStart)
    {
      for(Index i=alignedStart; i<alignedEnd; i+=PacketSize)
      {
        Packet xi = pload<Packet>(px);
        Packet yi = pload<Packet>(py);
        pstore(px, padd(pmul(pc,xi),pcj.pmul(ps,yi)));
        pstore(py, psub(pcj.pmul(pc,yi),pmul(ps,xi)));
        px += PacketSize;
        py += PacketSize;
      }
    }
    else
    {
      Index peelingEnd = alignedStart + ((size-alignedStart)/(Peeling*PacketSize))*(Peeling*PacketSize);
      for(Index i=alignedStart; i<peelingEnd; i+=Peeling*PacketSize)
      {
        Packet xi   = ploadu<Packet>(px);
        Packet xi1  = ploadu<Packet>(px+PacketSize);
        Packet yi   = pload <Packet>(py);
        Packet yi1  = pload <Packet>(py+PacketSize);
        pstoreu(px, padd(pmul(pc,xi),pcj.pmul(ps,yi)));
        pstoreu(px+PacketSize, padd(pmul(pc,xi1),pcj.pmul(ps,yi1)));
        pstore (py, psub(pcj.pmul(pc,yi),pmul(ps,xi)));
        pstore (py+PacketSize, psub(pcj.pmul(pc,yi1),pmul(ps,xi1)));
        px += Peeling*PacketSize;
        py += Peeling*PacketSize;
      }
      if(alignedEnd!=peelingEnd)
      {
        Packet xi = ploadu<Packet>(x+peelingEnd);
        Packet yi = pload <Packet>(y+peelingEnd);
        pstoreu(x+peelingEnd, padd(pmul(pc,xi),pcj.pmul(ps,yi)));
        pstore (y+peelingEnd, psub(pcj.pmul(pc,yi),pmul(ps,xi)));
      }
    }

    for(Index i=alignedEnd; i<size; ++i)
    {
      Scalar xi = x[i];
      Scalar yi = y[i];
      x[i] =  j.c() * xi + conj(j.s()) * yi;
      y[i] = -j.s() * xi + conj(j.c()) * yi;
    }
  }

  
  else if(VectorX::SizeAtCompileTime != Dynamic &&
          (VectorX::Flags & VectorY::Flags & PacketAccessBit) &&
          (VectorX::Flags & VectorY::Flags & AlignedBit))
  {
    const Packet pc = pset1<Packet>(j.c());
    const Packet ps = pset1<Packet>(j.s());
    conj_helper<Packet,Packet,NumTraits<Scalar>::IsComplex,false> pcj;
    Scalar* EIGEN_RESTRICT px = x;
    Scalar* EIGEN_RESTRICT py = y;
    for(Index i=0; i<size; i+=PacketSize)
    {
      Packet xi = pload<Packet>(px);
      Packet yi = pload<Packet>(py);
      pstore(px, padd(pmul(pc,xi),pcj.pmul(ps,yi)));
      pstore(py, psub(pcj.pmul(pc,yi),pmul(ps,xi)));
      px += PacketSize;
      py += PacketSize;
    }
  }

  
  else
  {
    for(Index i=0; i<size; ++i)
    {
      Scalar xi = *x;
      Scalar yi = *y;
      *x =  j.c() * xi + conj(j.s()) * yi;
      *y = -j.s() * xi + conj(j.c()) * yi;
      x += incrx;
      y += incry;
    }
  }
}

} 

} 

#endif 
