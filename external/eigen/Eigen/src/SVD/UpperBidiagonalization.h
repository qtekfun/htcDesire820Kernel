// Copyright (C) 2010 Benoit Jacob <jacob.benoit.1@gmail.com>
// Public License v. 2.0. If a copy of the MPL was not distributed

#ifndef EIGEN_BIDIAGONALIZATION_H
#define EIGEN_BIDIAGONALIZATION_H

namespace Eigen { 

namespace internal {

template<typename _MatrixType> class UpperBidiagonalization
{
  public:

    typedef _MatrixType MatrixType;
    enum {
      RowsAtCompileTime = MatrixType::RowsAtCompileTime,
      ColsAtCompileTime = MatrixType::ColsAtCompileTime,
      ColsAtCompileTimeMinusOne = internal::decrement_size<ColsAtCompileTime>::ret
    };
    typedef typename MatrixType::Scalar Scalar;
    typedef typename MatrixType::RealScalar RealScalar;
    typedef typename MatrixType::Index Index;
    typedef Matrix<Scalar, 1, ColsAtCompileTime> RowVectorType;
    typedef Matrix<Scalar, RowsAtCompileTime, 1> ColVectorType;
    typedef BandMatrix<RealScalar, ColsAtCompileTime, ColsAtCompileTime, 1, 0> BidiagonalType;
    typedef Matrix<Scalar, ColsAtCompileTime, 1> DiagVectorType;
    typedef Matrix<Scalar, ColsAtCompileTimeMinusOne, 1> SuperDiagVectorType;
    typedef HouseholderSequence<
              const MatrixType,
              CwiseUnaryOp<internal::scalar_conjugate_op<Scalar>, const Diagonal<const MatrixType,0> >
            > HouseholderUSequenceType;
    typedef HouseholderSequence<
              const MatrixType,
              Diagonal<const MatrixType,1>,
              OnTheRight
            > HouseholderVSequenceType;
    
    UpperBidiagonalization() : m_householder(), m_bidiagonal(), m_isInitialized(false) {}

    UpperBidiagonalization(const MatrixType& matrix)
      : m_householder(matrix.rows(), matrix.cols()),
        m_bidiagonal(matrix.cols(), matrix.cols()),
        m_isInitialized(false)
    {
      compute(matrix);
    }
    
    UpperBidiagonalization& compute(const MatrixType& matrix);
    
    const MatrixType& householder() const { return m_householder; }
    const BidiagonalType& bidiagonal() const { return m_bidiagonal; }
    
    const HouseholderUSequenceType householderU() const
    {
      eigen_assert(m_isInitialized && "UpperBidiagonalization is not initialized.");
      return HouseholderUSequenceType(m_householder, m_householder.diagonal().conjugate());
    }

    const HouseholderVSequenceType householderV() 
    {
      eigen_assert(m_isInitialized && "UpperBidiagonalization is not initialized.");
      return HouseholderVSequenceType(m_householder, m_householder.const_derived().template diagonal<1>())
             .setLength(m_householder.cols()-1)
             .setShift(1);
    }
    
  protected:
    MatrixType m_householder;
    BidiagonalType m_bidiagonal;
    bool m_isInitialized;
};

template<typename _MatrixType>
UpperBidiagonalization<_MatrixType>& UpperBidiagonalization<_MatrixType>::compute(const _MatrixType& matrix)
{
  Index rows = matrix.rows();
  Index cols = matrix.cols();
  
  eigen_assert(rows >= cols && "UpperBidiagonalization is only for matrices satisfying rows>=cols.");
  
  m_householder = matrix;

  ColVectorType temp(rows);

  for (Index k = 0;  ; ++k)
  {
    Index remainingRows = rows - k;
    Index remainingCols = cols - k - 1;

    
    m_householder.col(k).tail(remainingRows)
                 .makeHouseholderInPlace(m_householder.coeffRef(k,k),
                                         m_bidiagonal.template diagonal<0>().coeffRef(k));
    
    m_householder.bottomRightCorner(remainingRows, remainingCols)
                 .applyHouseholderOnTheLeft(m_householder.col(k).tail(remainingRows-1),
                                            m_householder.coeff(k,k),
                                            temp.data());

    if(k == cols-1) break;
    
    
    m_householder.row(k).tail(remainingCols)
                 .makeHouseholderInPlace(m_householder.coeffRef(k,k+1),
                                         m_bidiagonal.template diagonal<1>().coeffRef(k));
    
    m_householder.bottomRightCorner(remainingRows-1, remainingCols)
                 .applyHouseholderOnTheRight(m_householder.row(k).tail(remainingCols-1).transpose(),
                                             m_householder.coeff(k,k+1),
                                             temp.data());
  }
  m_isInitialized = true;
  return *this;
}

#if 0
template<typename Derived>
const UpperBidiagonalization<typename MatrixBase<Derived>::PlainObject>
MatrixBase<Derived>::bidiagonalization() const
{
  return UpperBidiagonalization<PlainObject>(eval());
}
#endif

} 

} 

#endif 