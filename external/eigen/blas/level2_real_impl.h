// Copyright (C) 2009-2010 Gael Guennebaud <gael.guennebaud@inria.fr>
// Public License v. 2.0. If a copy of the MPL was not distributed

#include "common.h"

int EIGEN_BLAS_FUNC(symv) (char *uplo, int *n, RealScalar *palpha, RealScalar *pa, int *lda, RealScalar *px, int *incx, RealScalar *pbeta, RealScalar *py, int *incy)
{
  Scalar* a = reinterpret_cast<Scalar*>(pa);
  Scalar* x = reinterpret_cast<Scalar*>(px);
  Scalar* y = reinterpret_cast<Scalar*>(py);
  Scalar alpha  = *reinterpret_cast<Scalar*>(palpha);
  Scalar beta   = *reinterpret_cast<Scalar*>(pbeta);

  
  int info = 0;
  if(UPLO(*uplo)==INVALID)        info = 1;
  else if(*n<0)                   info = 2;
  else if(*lda<std::max(1,*n))    info = 5;
  else if(*incx==0)               info = 7;
  else if(*incy==0)               info = 10;
  if(info)
    return xerbla_(SCALAR_SUFFIX_UP"SYMV ",&info,6);

  if(*n==0)
    return 0;

  Scalar* actual_x = get_compact_vector(x,*n,*incx);
  Scalar* actual_y = get_compact_vector(y,*n,*incy);

  if(beta!=Scalar(1))
  {
    if(beta==Scalar(0)) vector(actual_y, *n).setZero();
    else                vector(actual_y, *n) *= beta;
  }

  
       if(UPLO(*uplo)==UP) vector(actual_y,*n).noalias() += matrix(a,*n,*n,*lda).selfadjointView<Upper>() * (alpha * vector(actual_x,*n));
  else if(UPLO(*uplo)==LO) vector(actual_y,*n).noalias() += matrix(a,*n,*n,*lda).selfadjointView<Lower>() * (alpha * vector(actual_x,*n));

  if(actual_x!=x) delete[] actual_x;
  if(actual_y!=y) delete[] copy_back(actual_y,y,*n,*incy);

  return 1;
}

int EIGEN_BLAS_FUNC(syr)(char *uplo, int *n, RealScalar *palpha, RealScalar *px, int *incx, RealScalar *pc, int *ldc)
{




  Scalar* x = reinterpret_cast<Scalar*>(px);
  Scalar* c = reinterpret_cast<Scalar*>(pc);
  Scalar alpha = *reinterpret_cast<Scalar*>(palpha);

  int info = 0;
  if(UPLO(*uplo)==INVALID)                                            info = 1;
  else if(*n<0)                                                       info = 2;
  else if(*incx==0)                                                   info = 5;
  else if(*ldc<std::max(1,*n))                                        info = 7;
  if(info)
    return xerbla_(SCALAR_SUFFIX_UP"SYR  ",&info,6);

  if(*n==0 || alpha==Scalar(0)) return 1;

  
  Scalar* x_cpy = get_compact_vector(x,*n,*incx);

  Matrix<Scalar,Dynamic,Dynamic> m2(matrix(c,*n,*n,*ldc));
  
  

  if(UPLO(*uplo)==LO)
    for(int j=0;j<*n;++j)
      matrix(c,*n,*n,*ldc).col(j).tail(*n-j) += alpha * x_cpy[j] * vector(x_cpy+j,*n-j);
  else
    for(int j=0;j<*n;++j)
      matrix(c,*n,*n,*ldc).col(j).head(j+1) += alpha * x_cpy[j] * vector(x_cpy,j+1);

  if(x_cpy!=x)  delete[] x_cpy;

  return 1;
}

int EIGEN_BLAS_FUNC(syr2)(char *uplo, int *n, RealScalar *palpha, RealScalar *px, int *incx, RealScalar *py, int *incy, RealScalar *pc, int *ldc)
{

  Scalar* x = reinterpret_cast<Scalar*>(px);
  Scalar* y = reinterpret_cast<Scalar*>(py);
  Scalar* c = reinterpret_cast<Scalar*>(pc);
  Scalar alpha = *reinterpret_cast<Scalar*>(palpha);

  int info = 0;
  if(UPLO(*uplo)==INVALID)                                            info = 1;
  else if(*n<0)                                                       info = 2;
  else if(*incx==0)                                                   info = 5;
  else if(*incy==0)                                                   info = 7;
  else if(*ldc<std::max(1,*n))                                        info = 9;
  if(info)
    return xerbla_(SCALAR_SUFFIX_UP"SYR2 ",&info,6);

  if(alpha==Scalar(0))
    return 1;

  Scalar* x_cpy = get_compact_vector(x,*n,*incx);
  Scalar* y_cpy = get_compact_vector(y,*n,*incy);

  
  if(UPLO(*uplo)==LO)       matrix(c,*n,*n,*ldc).selfadjointView<Lower>().rankUpdate(vector(x_cpy,*n), vector(y_cpy,*n), alpha);
  else if(UPLO(*uplo)==UP)  matrix(c,*n,*n,*ldc).selfadjointView<Upper>().rankUpdate(vector(x_cpy,*n), vector(y_cpy,*n), alpha);

  if(x_cpy!=x)  delete[] x_cpy;
  if(y_cpy!=y)  delete[] y_cpy;


  return 1;
}






