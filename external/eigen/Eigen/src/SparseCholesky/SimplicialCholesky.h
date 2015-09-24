// Copyright (C) 2008-2010 Gael Guennebaud <gael.guennebaud@inria.fr>
// Public License v. 2.0. If a copy of the MPL was not distributed

/*

NOTE: the _symbolic, and _numeric functions has been adapted from
      the LDL library:

LDL Copyright (c) 2005 by Timothy A. Davis.  All Rights Reserved.

LDL License:

    Your use or distribution of LDL or any modified version of
    LDL implies that you agree to this License.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
    USA

    Permission is hereby granted to use or copy this program under the
    terms of the GNU LGPL, provided that the Copyright, this License,
    and the Availability of the original version is retained on all copies.
    User documentation of any code that uses this code or any modified
    version of this code must cite the Copyright, this License, the
    Availability note, and "Used by permission." Permission to modify
    the code and to distribute modified code is granted, provided the
    Copyright, this License, and the Availability note are retained,
    and a notice that the code was modified is included.
 */

#include "../Core/util/NonMPL2.h"

#ifndef EIGEN_SIMPLICIAL_CHOLESKY_H
#define EIGEN_SIMPLICIAL_CHOLESKY_H

namespace Eigen { 

enum SimplicialCholeskyMode {
  SimplicialCholeskyLLT,
  SimplicialCholeskyLDLT
};

template<typename Derived>
class SimplicialCholeskyBase : internal::noncopyable
{
  public:
    typedef typename internal::traits<Derived>::MatrixType MatrixType;
    enum { UpLo = internal::traits<Derived>::UpLo };
    typedef typename MatrixType::Scalar Scalar;
    typedef typename MatrixType::RealScalar RealScalar;
    typedef typename MatrixType::Index Index;
    typedef SparseMatrix<Scalar,ColMajor,Index> CholMatrixType;
    typedef Matrix<Scalar,Dynamic,1> VectorType;

  public:

    
    SimplicialCholeskyBase()
      : m_info(Success), m_isInitialized(false), m_shiftOffset(0), m_shiftScale(1)
    {}

    SimplicialCholeskyBase(const MatrixType& matrix)
      : m_info(Success), m_isInitialized(false), m_shiftOffset(0), m_shiftScale(1)
    {
      derived().compute(matrix);
    }

    ~SimplicialCholeskyBase()
    {
    }

    Derived& derived() { return *static_cast<Derived*>(this); }
    const Derived& derived() const { return *static_cast<const Derived*>(this); }
    
    inline Index cols() const { return m_matrix.cols(); }
    inline Index rows() const { return m_matrix.rows(); }
    
    ComputationInfo info() const
    {
      eigen_assert(m_isInitialized && "Decomposition is not initialized.");
      return m_info;
    }
    
    template<typename Rhs>
    inline const internal::solve_retval<SimplicialCholeskyBase, Rhs>
    solve(const MatrixBase<Rhs>& b) const
    {
      eigen_assert(m_isInitialized && "Simplicial LLT or LDLT is not initialized.");
      eigen_assert(rows()==b.rows()
                && "SimplicialCholeskyBase::solve(): invalid number of rows of the right hand side matrix b");
      return internal::solve_retval<SimplicialCholeskyBase, Rhs>(*this, b.derived());
    }
    
    template<typename Rhs>
    inline const internal::sparse_solve_retval<SimplicialCholeskyBase, Rhs>
    solve(const SparseMatrixBase<Rhs>& b) const
    {
      eigen_assert(m_isInitialized && "Simplicial LLT or LDLT is not initialized.");
      eigen_assert(rows()==b.rows()
                && "SimplicialCholesky::solve(): invalid number of rows of the right hand side matrix b");
      return internal::sparse_solve_retval<SimplicialCholeskyBase, Rhs>(*this, b.derived());
    }
    
    const PermutationMatrix<Dynamic,Dynamic,Index>& permutationP() const
    { return m_P; }
    
    const PermutationMatrix<Dynamic,Dynamic,Index>& permutationPinv() const
    { return m_Pinv; }

    Derived& setShift(const RealScalar& offset, const RealScalar& scale = 1)
    {
      m_shiftOffset = offset;
      m_shiftScale = scale;
      return derived();
    }

#ifndef EIGEN_PARSED_BY_DOXYGEN
    
    template<typename Stream>
    void dumpMemory(Stream& s)
    {
      int total = 0;
      s << "  L:        " << ((total+=(m_matrix.cols()+1) * sizeof(int) + m_matrix.nonZeros()*(sizeof(int)+sizeof(Scalar))) >> 20) << "Mb" << "\n";
      s << "  diag:     " << ((total+=m_diag.size() * sizeof(Scalar)) >> 20) << "Mb" << "\n";
      s << "  tree:     " << ((total+=m_parent.size() * sizeof(int)) >> 20) << "Mb" << "\n";
      s << "  nonzeros: " << ((total+=m_nonZerosPerCol.size() * sizeof(int)) >> 20) << "Mb" << "\n";
      s << "  perm:     " << ((total+=m_P.size() * sizeof(int)) >> 20) << "Mb" << "\n";
      s << "  perm^-1:  " << ((total+=m_Pinv.size() * sizeof(int)) >> 20) << "Mb" << "\n";
      s << "  TOTAL:    " << (total>> 20) << "Mb" << "\n";
    }

    
    template<typename Rhs,typename Dest>
    void _solve(const MatrixBase<Rhs> &b, MatrixBase<Dest> &dest) const
    {
      eigen_assert(m_factorizationIsOk && "The decomposition is not in a valid state for solving, you must first call either compute() or symbolic()/numeric()");
      eigen_assert(m_matrix.rows()==b.rows());

      if(m_info!=Success)
        return;

      if(m_P.size()>0)
        dest = m_P * b;
      else
        dest = b;

      if(m_matrix.nonZeros()>0) 
        derived().matrixL().solveInPlace(dest);

      if(m_diag.size()>0)
        dest = m_diag.asDiagonal().inverse() * dest;

      if (m_matrix.nonZeros()>0) 
        derived().matrixU().solveInPlace(dest);

      if(m_P.size()>0)
        dest = m_Pinv * dest;
    }

    
    template<typename Rhs, typename DestScalar, int DestOptions, typename DestIndex>
    void _solve_sparse(const Rhs& b, SparseMatrix<DestScalar,DestOptions,DestIndex> &dest) const
    {
      eigen_assert(m_factorizationIsOk && "The decomposition is not in a valid state for solving, you must first call either compute() or symbolic()/numeric()");
      eigen_assert(m_matrix.rows()==b.rows());
      
      
      static const int NbColsAtOnce = 4;
      int rhsCols = b.cols();
      int size = b.rows();
      Eigen::Matrix<DestScalar,Dynamic,Dynamic> tmp(size,rhsCols);
      for(int k=0; k<rhsCols; k+=NbColsAtOnce)
      {
        int actualCols = std::min<int>(rhsCols-k, NbColsAtOnce);
        tmp.leftCols(actualCols) = b.middleCols(k,actualCols);
        tmp.leftCols(actualCols) = derived().solve(tmp.leftCols(actualCols));
        dest.middleCols(k,actualCols) = tmp.leftCols(actualCols).sparseView();
      }
    }

#endif 

  protected:
    
    
    template<bool DoLDLT>
    void compute(const MatrixType& matrix)
    {
      eigen_assert(matrix.rows()==matrix.cols());
      Index size = matrix.cols();
      CholMatrixType ap(size,size);
      ordering(matrix, ap);
      analyzePattern_preordered(ap, DoLDLT);
      factorize_preordered<DoLDLT>(ap);
    }
    
    template<bool DoLDLT>
    void factorize(const MatrixType& a)
    {
      eigen_assert(a.rows()==a.cols());
      int size = a.cols();
      CholMatrixType ap(size,size);
      ap.template selfadjointView<Upper>() = a.template selfadjointView<UpLo>().twistedBy(m_P);
      factorize_preordered<DoLDLT>(ap);
    }

    template<bool DoLDLT>
    void factorize_preordered(const CholMatrixType& a);

    void analyzePattern(const MatrixType& a, bool doLDLT)
    {
      eigen_assert(a.rows()==a.cols());
      int size = a.cols();
      CholMatrixType ap(size,size);
      ordering(a, ap);
      analyzePattern_preordered(ap,doLDLT);
    }
    void analyzePattern_preordered(const CholMatrixType& a, bool doLDLT);
    
    void ordering(const MatrixType& a, CholMatrixType& ap);

    
    struct keep_diag {
      inline bool operator() (const Index& row, const Index& col, const Scalar&) const
      {
        return row!=col;
      }
    };

    mutable ComputationInfo m_info;
    bool m_isInitialized;
    bool m_factorizationIsOk;
    bool m_analysisIsOk;
    
    CholMatrixType m_matrix;
    VectorType m_diag;                                
    VectorXi m_parent;                                
    VectorXi m_nonZerosPerCol;
    PermutationMatrix<Dynamic,Dynamic,Index> m_P;     
    PermutationMatrix<Dynamic,Dynamic,Index> m_Pinv;  

    RealScalar m_shiftOffset;
    RealScalar m_shiftScale;
};

template<typename _MatrixType, int _UpLo = Lower> class SimplicialLLT;
template<typename _MatrixType, int _UpLo = Lower> class SimplicialLDLT;
template<typename _MatrixType, int _UpLo = Lower> class SimplicialCholesky;

namespace internal {

template<typename _MatrixType, int _UpLo> struct traits<SimplicialLLT<_MatrixType,_UpLo> >
{
  typedef _MatrixType MatrixType;
  enum { UpLo = _UpLo };
  typedef typename MatrixType::Scalar                         Scalar;
  typedef typename MatrixType::Index                          Index;
  typedef SparseMatrix<Scalar, ColMajor, Index>               CholMatrixType;
  typedef SparseTriangularView<CholMatrixType, Eigen::Lower>  MatrixL;
  typedef SparseTriangularView<typename CholMatrixType::AdjointReturnType, Eigen::Upper>   MatrixU;
  static inline MatrixL getL(const MatrixType& m) { return m; }
  static inline MatrixU getU(const MatrixType& m) { return m.adjoint(); }
};

template<typename _MatrixType,int _UpLo> struct traits<SimplicialLDLT<_MatrixType,_UpLo> >
{
  typedef _MatrixType MatrixType;
  enum { UpLo = _UpLo };
  typedef typename MatrixType::Scalar                             Scalar;
  typedef typename MatrixType::Index                              Index;
  typedef SparseMatrix<Scalar, ColMajor, Index>                   CholMatrixType;
  typedef SparseTriangularView<CholMatrixType, Eigen::UnitLower>  MatrixL;
  typedef SparseTriangularView<typename CholMatrixType::AdjointReturnType, Eigen::UnitUpper> MatrixU;
  static inline MatrixL getL(const MatrixType& m) { return m; }
  static inline MatrixU getU(const MatrixType& m) { return m.adjoint(); }
};

template<typename _MatrixType, int _UpLo> struct traits<SimplicialCholesky<_MatrixType,_UpLo> >
{
  typedef _MatrixType MatrixType;
  enum { UpLo = _UpLo };
};

}

template<typename _MatrixType, int _UpLo>
    class SimplicialLLT : public SimplicialCholeskyBase<SimplicialLLT<_MatrixType,_UpLo> >
{
public:
    typedef _MatrixType MatrixType;
    enum { UpLo = _UpLo };
    typedef SimplicialCholeskyBase<SimplicialLLT> Base;
    typedef typename MatrixType::Scalar Scalar;
    typedef typename MatrixType::RealScalar RealScalar;
    typedef typename MatrixType::Index Index;
    typedef SparseMatrix<Scalar,ColMajor,Index> CholMatrixType;
    typedef Matrix<Scalar,Dynamic,1> VectorType;
    typedef internal::traits<SimplicialLLT> Traits;
    typedef typename Traits::MatrixL  MatrixL;
    typedef typename Traits::MatrixU  MatrixU;
public:
    
    SimplicialLLT() : Base() {}
    
    SimplicialLLT(const MatrixType& matrix)
        : Base(matrix) {}

    
    inline const MatrixL matrixL() const {
        eigen_assert(Base::m_factorizationIsOk && "Simplicial LLT not factorized");
        return Traits::getL(Base::m_matrix);
    }

    
    inline const MatrixU matrixU() const {
        eigen_assert(Base::m_factorizationIsOk && "Simplicial LLT not factorized");
        return Traits::getU(Base::m_matrix);
    }
    
    
    SimplicialLLT& compute(const MatrixType& matrix)
    {
      Base::template compute<false>(matrix);
      return *this;
    }

    void analyzePattern(const MatrixType& a)
    {
      Base::analyzePattern(a, false);
    }

    void factorize(const MatrixType& a)
    {
      Base::template factorize<false>(a);
    }

    
    Scalar determinant() const
    {
      Scalar detL = Base::m_matrix.diagonal().prod();
      return internal::abs2(detL);
    }
};

template<typename _MatrixType, int _UpLo>
    class SimplicialLDLT : public SimplicialCholeskyBase<SimplicialLDLT<_MatrixType,_UpLo> >
{
public:
    typedef _MatrixType MatrixType;
    enum { UpLo = _UpLo };
    typedef SimplicialCholeskyBase<SimplicialLDLT> Base;
    typedef typename MatrixType::Scalar Scalar;
    typedef typename MatrixType::RealScalar RealScalar;
    typedef typename MatrixType::Index Index;
    typedef SparseMatrix<Scalar,ColMajor,Index> CholMatrixType;
    typedef Matrix<Scalar,Dynamic,1> VectorType;
    typedef internal::traits<SimplicialLDLT> Traits;
    typedef typename Traits::MatrixL  MatrixL;
    typedef typename Traits::MatrixU  MatrixU;
public:
    
    SimplicialLDLT() : Base() {}

    
    SimplicialLDLT(const MatrixType& matrix)
        : Base(matrix) {}

    
    inline const VectorType vectorD() const {
        eigen_assert(Base::m_factorizationIsOk && "Simplicial LDLT not factorized");
        return Base::m_diag;
    }
    
    inline const MatrixL matrixL() const {
        eigen_assert(Base::m_factorizationIsOk && "Simplicial LDLT not factorized");
        return Traits::getL(Base::m_matrix);
    }

    
    inline const MatrixU matrixU() const {
        eigen_assert(Base::m_factorizationIsOk && "Simplicial LDLT not factorized");
        return Traits::getU(Base::m_matrix);
    }

    
    SimplicialLDLT& compute(const MatrixType& matrix)
    {
      Base::template compute<true>(matrix);
      return *this;
    }
    
    void analyzePattern(const MatrixType& a)
    {
      Base::analyzePattern(a, true);
    }

    void factorize(const MatrixType& a)
    {
      Base::template factorize<true>(a);
    }

    
    Scalar determinant() const
    {
      return Base::m_diag.prod();
    }
};

template<typename _MatrixType, int _UpLo>
    class SimplicialCholesky : public SimplicialCholeskyBase<SimplicialCholesky<_MatrixType,_UpLo> >
{
public:
    typedef _MatrixType MatrixType;
    enum { UpLo = _UpLo };
    typedef SimplicialCholeskyBase<SimplicialCholesky> Base;
    typedef typename MatrixType::Scalar Scalar;
    typedef typename MatrixType::RealScalar RealScalar;
    typedef typename MatrixType::Index Index;
    typedef SparseMatrix<Scalar,ColMajor,Index> CholMatrixType;
    typedef Matrix<Scalar,Dynamic,1> VectorType;
    typedef internal::traits<SimplicialCholesky> Traits;
    typedef internal::traits<SimplicialLDLT<MatrixType,UpLo> > LDLTTraits;
    typedef internal::traits<SimplicialLLT<MatrixType,UpLo>  > LLTTraits;
  public:
    SimplicialCholesky() : Base(), m_LDLT(true) {}

    SimplicialCholesky(const MatrixType& matrix)
      : Base(), m_LDLT(true)
    {
      compute(matrix);
    }

    SimplicialCholesky& setMode(SimplicialCholeskyMode mode)
    {
      switch(mode)
      {
      case SimplicialCholeskyLLT:
        m_LDLT = false;
        break;
      case SimplicialCholeskyLDLT:
        m_LDLT = true;
        break;
      default:
        break;
      }

      return *this;
    }

    inline const VectorType vectorD() const {
        eigen_assert(Base::m_factorizationIsOk && "Simplicial Cholesky not factorized");
        return Base::m_diag;
    }
    inline const CholMatrixType rawMatrix() const {
        eigen_assert(Base::m_factorizationIsOk && "Simplicial Cholesky not factorized");
        return Base::m_matrix;
    }
    
    
    SimplicialCholesky& compute(const MatrixType& matrix)
    {
      if(m_LDLT)
        Base::template compute<true>(matrix);
      else
        Base::template compute<false>(matrix);
      return *this;
    }

    void analyzePattern(const MatrixType& a)
    {
      Base::analyzePattern(a, m_LDLT);
    }

    void factorize(const MatrixType& a)
    {
      if(m_LDLT)
        Base::template factorize<true>(a);
      else
        Base::template factorize<false>(a);
    }

    
    template<typename Rhs,typename Dest>
    void _solve(const MatrixBase<Rhs> &b, MatrixBase<Dest> &dest) const
    {
      eigen_assert(Base::m_factorizationIsOk && "The decomposition is not in a valid state for solving, you must first call either compute() or symbolic()/numeric()");
      eigen_assert(Base::m_matrix.rows()==b.rows());

      if(Base::m_info!=Success)
        return;

      if(Base::m_P.size()>0)
        dest = Base::m_P * b;
      else
        dest = b;

      if(Base::m_matrix.nonZeros()>0) 
      {
        if(m_LDLT)
          LDLTTraits::getL(Base::m_matrix).solveInPlace(dest);
        else
          LLTTraits::getL(Base::m_matrix).solveInPlace(dest);
      }

      if(Base::m_diag.size()>0)
        dest = Base::m_diag.asDiagonal().inverse() * dest;

      if (Base::m_matrix.nonZeros()>0) 
      {
        if(m_LDLT)
          LDLTTraits::getU(Base::m_matrix).solveInPlace(dest);
        else
          LLTTraits::getU(Base::m_matrix).solveInPlace(dest);
      }

      if(Base::m_P.size()>0)
        dest = Base::m_Pinv * dest;
    }
    
    Scalar determinant() const
    {
      if(m_LDLT)
      {
        return Base::m_diag.prod();
      }
      else
      {
        Scalar detL = Diagonal<const CholMatrixType>(Base::m_matrix).prod();
        return internal::abs2(detL);
      }
    }
    
  protected:
    bool m_LDLT;
};

template<typename Derived>
void SimplicialCholeskyBase<Derived>::ordering(const MatrixType& a, CholMatrixType& ap)
{
  eigen_assert(a.rows()==a.cols());
  const Index size = a.rows();
  
  
  {
    CholMatrixType C;
    C = a.template selfadjointView<UpLo>();
    
    
    
    internal::minimum_degree_ordering(C, m_Pinv);
  }

  if(m_Pinv.size()>0)
    m_P = m_Pinv.inverse();
  else
    m_P.resize(0);

  ap.resize(size,size);
  ap.template selfadjointView<Upper>() = a.template selfadjointView<UpLo>().twistedBy(m_P);
}

template<typename Derived>
void SimplicialCholeskyBase<Derived>::analyzePattern_preordered(const CholMatrixType& ap, bool doLDLT)
{
  const Index size = ap.rows();
  m_matrix.resize(size, size);
  m_parent.resize(size);
  m_nonZerosPerCol.resize(size);
  
  ei_declare_aligned_stack_constructed_variable(Index, tags, size, 0);

  for(Index k = 0; k < size; ++k)
  {
    
    m_parent[k] = -1;             
    tags[k] = k;                  
    m_nonZerosPerCol[k] = 0;      
    for(typename CholMatrixType::InnerIterator it(ap,k); it; ++it)
    {
      Index i = it.index();
      if(i < k)
      {
        
        for(; tags[i] != k; i = m_parent[i])
        {
          
          if (m_parent[i] == -1)
            m_parent[i] = k;
          m_nonZerosPerCol[i]++;        
          tags[i] = k;                  
        }
      }
    }
  }
  
  
  Index* Lp = m_matrix.outerIndexPtr();
  Lp[0] = 0;
  for(Index k = 0; k < size; ++k)
    Lp[k+1] = Lp[k] + m_nonZerosPerCol[k] + (doLDLT ? 0 : 1);

  m_matrix.resizeNonZeros(Lp[size]);
  
  m_isInitialized     = true;
  m_info              = Success;
  m_analysisIsOk      = true;
  m_factorizationIsOk = false;
}


template<typename Derived>
template<bool DoLDLT>
void SimplicialCholeskyBase<Derived>::factorize_preordered(const CholMatrixType& ap)
{
  eigen_assert(m_analysisIsOk && "You must first call analyzePattern()");
  eigen_assert(ap.rows()==ap.cols());
  const Index size = ap.rows();
  eigen_assert(m_parent.size()==size);
  eigen_assert(m_nonZerosPerCol.size()==size);

  const Index* Lp = m_matrix.outerIndexPtr();
  Index* Li = m_matrix.innerIndexPtr();
  Scalar* Lx = m_matrix.valuePtr();

  ei_declare_aligned_stack_constructed_variable(Scalar, y, size, 0);
  ei_declare_aligned_stack_constructed_variable(Index,  pattern, size, 0);
  ei_declare_aligned_stack_constructed_variable(Index,  tags, size, 0);
  
  bool ok = true;
  m_diag.resize(DoLDLT ? size : 0);
  
  for(Index k = 0; k < size; ++k)
  {
    
    y[k] = 0.0;                     
    Index top = size;               
    tags[k] = k;                    
    m_nonZerosPerCol[k] = 0;        
    for(typename MatrixType::InnerIterator it(ap,k); it; ++it)
    {
      Index i = it.index();
      if(i <= k)
      {
        y[i] += internal::conj(it.value());            
        Index len;
        for(len = 0; tags[i] != k; i = m_parent[i])
        {
          pattern[len++] = i;     
          tags[i] = k;            
        }
        while(len > 0)
          pattern[--top] = pattern[--len];
      }
    }

    

    RealScalar d = internal::real(y[k]) * m_shiftScale + m_shiftOffset;    
    y[k] = 0.0;
    for(; top < size; ++top)
    {
      Index i = pattern[top];       
      Scalar yi = y[i];             
      y[i] = 0.0;
      
      
      Scalar l_ki;
      if(DoLDLT)
        l_ki = yi / m_diag[i];       
      else
        yi = l_ki = yi / Lx[Lp[i]];
      
      Index p2 = Lp[i] + m_nonZerosPerCol[i];
      Index p;
      for(p = Lp[i] + (DoLDLT ? 0 : 1); p < p2; ++p)
        y[Li[p]] -= internal::conj(Lx[p]) * yi;
      d -= internal::real(l_ki * internal::conj(yi));
      Li[p] = k;                          
      Lx[p] = l_ki;
      ++m_nonZerosPerCol[i];              
    }
    if(DoLDLT)
    {
      m_diag[k] = d;
      if(d == RealScalar(0))
      {
        ok = false;                         
        break;
      }
    }
    else
    {
      Index p = Lp[k] + m_nonZerosPerCol[k]++;
      Li[p] = k ;                
      if(d <= RealScalar(0)) {
        ok = false;              
        break;
      }
      Lx[p] = internal::sqrt(d) ;
    }
  }

  m_info = ok ? Success : NumericalIssue;
  m_factorizationIsOk = true;
}

namespace internal {
  
template<typename Derived, typename Rhs>
struct solve_retval<SimplicialCholeskyBase<Derived>, Rhs>
  : solve_retval_base<SimplicialCholeskyBase<Derived>, Rhs>
{
  typedef SimplicialCholeskyBase<Derived> Dec;
  EIGEN_MAKE_SOLVE_HELPERS(Dec,Rhs)

  template<typename Dest> void evalTo(Dest& dst) const
  {
    dec().derived()._solve(rhs(),dst);
  }
};

template<typename Derived, typename Rhs>
struct sparse_solve_retval<SimplicialCholeskyBase<Derived>, Rhs>
  : sparse_solve_retval_base<SimplicialCholeskyBase<Derived>, Rhs>
{
  typedef SimplicialCholeskyBase<Derived> Dec;
  EIGEN_MAKE_SPARSE_SOLVE_HELPERS(Dec,Rhs)

  template<typename Dest> void evalTo(Dest& dst) const
  {
    dec().derived()._solve_sparse(rhs(),dst);
  }
};

} 

} 

#endif 
