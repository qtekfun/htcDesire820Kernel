// Copyright (C) 2009-2010 Gael Guennebaud <gael.guennebaud@inria.fr>
// Public License v. 2.0. If a copy of the MPL was not distributed

#include "common.h"

struct scalar_norm1_op {
  typedef RealScalar result_type;
  EIGEN_EMPTY_STRUCT_CTOR(scalar_norm1_op)
  inline RealScalar operator() (const Scalar& a) const { return internal::norm1(a); }
};
namespace Eigen {
  namespace internal {
    template<> struct functor_traits<scalar_norm1_op >
    {
      enum { Cost = 3 * NumTraits<Scalar>::AddCost, PacketAccess = 0 };
    };
  }
}

RealScalar EIGEN_CAT(EIGEN_CAT(REAL_SCALAR_SUFFIX,SCALAR_SUFFIX),asum_)(int *n, RealScalar *px, int *incx)
{
  Complex* x = reinterpret_cast<Complex*>(px);

  if(*n<=0) return 0;

  if(*incx==1)  return vector(x,*n).unaryExpr<scalar_norm1_op>().sum();
  else          return vector(x,*n,std::abs(*incx)).unaryExpr<scalar_norm1_op>().sum();
}

int EIGEN_BLAS_FUNC(dotcw)(int *n, RealScalar *px, int *incx, RealScalar *py, int *incy, RealScalar* pres)
{

  if(*n<=0) return 0;

  Scalar* x = reinterpret_cast<Scalar*>(px);
  Scalar* y = reinterpret_cast<Scalar*>(py);
  Scalar* res = reinterpret_cast<Scalar*>(pres);

  if(*incx==1 && *incy==1)    *res = (vector(x,*n).dot(vector(y,*n)));
  else if(*incx>0 && *incy>0) *res = (vector(x,*n,*incx).dot(vector(y,*n,*incy)));
  else if(*incx<0 && *incy>0) *res = (vector(x,*n,-*incx).reverse().dot(vector(y,*n,*incy)));
  else if(*incx>0 && *incy<0) *res = (vector(x,*n,*incx).dot(vector(y,*n,-*incy).reverse()));
  else if(*incx<0 && *incy<0) *res = (vector(x,*n,-*incx).reverse().dot(vector(y,*n,-*incy).reverse()));
  return 0;
}

int EIGEN_BLAS_FUNC(dotuw)(int *n, RealScalar *px, int *incx, RealScalar *py, int *incy, RealScalar* pres)
{

  if(*n<=0) return 0;

  Scalar* x = reinterpret_cast<Scalar*>(px);
  Scalar* y = reinterpret_cast<Scalar*>(py);
  Scalar* res = reinterpret_cast<Scalar*>(pres);

  if(*incx==1 && *incy==1)    *res = (vector(x,*n).cwiseProduct(vector(y,*n))).sum();
  else if(*incx>0 && *incy>0) *res = (vector(x,*n,*incx).cwiseProduct(vector(y,*n,*incy))).sum();
  else if(*incx<0 && *incy>0) *res = (vector(x,*n,-*incx).reverse().cwiseProduct(vector(y,*n,*incy))).sum();
  else if(*incx>0 && *incy<0) *res = (vector(x,*n,*incx).cwiseProduct(vector(y,*n,-*incy).reverse())).sum();
  else if(*incx<0 && *incy<0) *res = (vector(x,*n,-*incx).reverse().cwiseProduct(vector(y,*n,-*incy).reverse())).sum();
  return 0;
}

RealScalar EIGEN_CAT(EIGEN_CAT(REAL_SCALAR_SUFFIX,SCALAR_SUFFIX),nrm2_)(int *n, RealScalar *px, int *incx)
{
  if(*n<=0) return 0;

  Scalar* x = reinterpret_cast<Scalar*>(px);

  if(*incx==1)
    return vector(x,*n).stableNorm();

  return vector(x,*n,*incx).stableNorm();
}

int EIGEN_CAT(EIGEN_CAT(SCALAR_SUFFIX,REAL_SCALAR_SUFFIX),rot_)(int *n, RealScalar *px, int *incx, RealScalar *py, int *incy, RealScalar *pc, RealScalar *ps)
{
  if(*n<=0) return 0;

  Scalar* x = reinterpret_cast<Scalar*>(px);
  Scalar* y = reinterpret_cast<Scalar*>(py);
  RealScalar c = *pc;
  RealScalar s = *ps;

  StridedVectorType vx(vector(x,*n,std::abs(*incx)));
  StridedVectorType vy(vector(y,*n,std::abs(*incy)));

  Reverse<StridedVectorType> rvx(vx);
  Reverse<StridedVectorType> rvy(vy);

  
       if(*incx<0 && *incy>0) internal::apply_rotation_in_the_plane(rvx, vy, JacobiRotation<Scalar>(c,s));
  else if(*incx>0 && *incy<0) internal::apply_rotation_in_the_plane(vx, rvy, JacobiRotation<Scalar>(c,s));
  else                        internal::apply_rotation_in_the_plane(vx, vy,  JacobiRotation<Scalar>(c,s));

  return 0;
}

int EIGEN_CAT(EIGEN_CAT(SCALAR_SUFFIX,REAL_SCALAR_SUFFIX),scal_)(int *n, RealScalar *palpha, RealScalar *px, int *incx)
{
  if(*n<=0) return 0;

  Scalar* x = reinterpret_cast<Scalar*>(px);
  RealScalar alpha = *palpha;


  if(*incx==1)  vector(x,*n) *= alpha;
  else          vector(x,*n,std::abs(*incx)) *= alpha;

  return 0;
}
