// Copyright (C) 2007-2009 Benoit Jacob <jacob.benoit.1@gmail.com>
// Copyright (C) 2009-2010 Gael Guennebaud <gael.guennebaud@inria.fr>
// Public License v. 2.0. If a copy of the MPL was not distributed

#ifndef EIGEN_DIAGONAL_H
#define EIGEN_DIAGONAL_H

namespace Eigen { 


namespace internal {
template<typename MatrixType, int DiagIndex>
struct traits<Diagonal<MatrixType,DiagIndex> >
 : traits<MatrixType>
{
  typedef typename nested<MatrixType>::type MatrixTypeNested;
  typedef typename remove_reference<MatrixTypeNested>::type _MatrixTypeNested;
  typedef typename MatrixType::StorageKind StorageKind;
  enum {
    RowsAtCompileTime = (int(DiagIndex) == Dynamic || int(MatrixType::SizeAtCompileTime) == Dynamic) ? Dynamic
    : (EIGEN_PLAIN_ENUM_MIN(MatrixType::RowsAtCompileTime - EIGEN_PLAIN_ENUM_MAX(-DiagIndex, 0),
                            MatrixType::ColsAtCompileTime - EIGEN_PLAIN_ENUM_MAX( DiagIndex, 0))),
    ColsAtCompileTime = 1,
    MaxRowsAtCompileTime = int(MatrixType::MaxSizeAtCompileTime) == Dynamic ? Dynamic
                         : DiagIndex == Dynamic ? EIGEN_SIZE_MIN_PREFER_FIXED(MatrixType::MaxRowsAtCompileTime,
                                                                              MatrixType::MaxColsAtCompileTime)
                         : (EIGEN_PLAIN_ENUM_MIN(MatrixType::MaxRowsAtCompileTime - EIGEN_PLAIN_ENUM_MAX(-DiagIndex, 0),
                                                 MatrixType::MaxColsAtCompileTime - EIGEN_PLAIN_ENUM_MAX( DiagIndex, 0))),
    MaxColsAtCompileTime = 1,
    MaskLvalueBit = is_lvalue<MatrixType>::value ? LvalueBit : 0,
    Flags = (unsigned int)_MatrixTypeNested::Flags & (HereditaryBits | LinearAccessBit | MaskLvalueBit | DirectAccessBit) & ~RowMajorBit,
    CoeffReadCost = _MatrixTypeNested::CoeffReadCost,
    MatrixTypeOuterStride = outer_stride_at_compile_time<MatrixType>::ret,
    InnerStrideAtCompileTime = MatrixTypeOuterStride == Dynamic ? Dynamic : MatrixTypeOuterStride+1,
    OuterStrideAtCompileTime = 0
  };
};
}

template<typename MatrixType, int DiagIndex> class Diagonal
   : public internal::dense_xpr_base< Diagonal<MatrixType,DiagIndex> >::type
{
  public:

    typedef typename internal::dense_xpr_base<Diagonal>::type Base;
    EIGEN_DENSE_PUBLIC_INTERFACE(Diagonal)

    inline Diagonal(MatrixType& matrix, Index index = DiagIndex) : m_matrix(matrix), m_index(index) {}

    EIGEN_INHERIT_ASSIGNMENT_OPERATORS(Diagonal)

    inline Index rows() const
    { return m_index.value()<0 ? (std::min)(m_matrix.cols(),m_matrix.rows()+m_index.value()) : (std::min)(m_matrix.rows(),m_matrix.cols()-m_index.value()); }

    inline Index cols() const { return 1; }

    inline Index innerStride() const
    {
      return m_matrix.outerStride() + 1;
    }

    inline Index outerStride() const
    {
      return 0;
    }

    typedef typename internal::conditional<
                       internal::is_lvalue<MatrixType>::value,
                       Scalar,
                       const Scalar
                     >::type ScalarWithConstIfNotLvalue;

    inline ScalarWithConstIfNotLvalue* data() { return &(m_matrix.const_cast_derived().coeffRef(rowOffset(), colOffset())); }
    inline const Scalar* data() const { return &(m_matrix.const_cast_derived().coeffRef(rowOffset(), colOffset())); }

    inline Scalar& coeffRef(Index row, Index)
    {
      EIGEN_STATIC_ASSERT_LVALUE(MatrixType)
      return m_matrix.const_cast_derived().coeffRef(row+rowOffset(), row+colOffset());
    }

    inline const Scalar& coeffRef(Index row, Index) const
    {
      return m_matrix.const_cast_derived().coeffRef(row+rowOffset(), row+colOffset());
    }

    inline CoeffReturnType coeff(Index row, Index) const
    {
      return m_matrix.coeff(row+rowOffset(), row+colOffset());
    }

    inline Scalar& coeffRef(Index index)
    {
      EIGEN_STATIC_ASSERT_LVALUE(MatrixType)
      return m_matrix.const_cast_derived().coeffRef(index+rowOffset(), index+colOffset());
    }

    inline const Scalar& coeffRef(Index index) const
    {
      return m_matrix.const_cast_derived().coeffRef(index+rowOffset(), index+colOffset());
    }

    inline CoeffReturnType coeff(Index index) const
    {
      return m_matrix.coeff(index+rowOffset(), index+colOffset());
    }

    const typename internal::remove_all<typename MatrixType::Nested>::type& 
    nestedExpression() const 
    {
      return m_matrix;
    }

    int index() const
    {
      return m_index.value();
    }

  protected:
    typename MatrixType::Nested m_matrix;
    const internal::variable_if_dynamic<Index, DiagIndex> m_index;

  private:
    
    EIGEN_STRONG_INLINE Index absDiagIndex() const { return m_index.value()>0 ? m_index.value() : -m_index.value(); }
    EIGEN_STRONG_INLINE Index rowOffset() const { return m_index.value()>0 ? 0 : -m_index.value(); }
    EIGEN_STRONG_INLINE Index colOffset() const { return m_index.value()>0 ? m_index.value() : 0; }
    
    template<int LoadMode> typename MatrixType::PacketReturnType packet(Index) const;
    template<int LoadMode> typename MatrixType::PacketReturnType packet(Index,Index) const;
};

template<typename Derived>
inline typename MatrixBase<Derived>::DiagonalReturnType
MatrixBase<Derived>::diagonal()
{
  return derived();
}

template<typename Derived>
inline const typename MatrixBase<Derived>::ConstDiagonalReturnType
MatrixBase<Derived>::diagonal() const
{
  return ConstDiagonalReturnType(derived());
}

template<typename Derived>
inline typename MatrixBase<Derived>::template DiagonalIndexReturnType<Dynamic>::Type
MatrixBase<Derived>::diagonal(Index index)
{
  return typename DiagonalIndexReturnType<Dynamic>::Type(derived(), index);
}

template<typename Derived>
inline typename MatrixBase<Derived>::template ConstDiagonalIndexReturnType<Dynamic>::Type
MatrixBase<Derived>::diagonal(Index index) const
{
  return typename ConstDiagonalIndexReturnType<Dynamic>::Type(derived(), index);
}

template<typename Derived>
template<int Index>
inline typename MatrixBase<Derived>::template DiagonalIndexReturnType<Index>::Type
MatrixBase<Derived>::diagonal()
{
  return derived();
}

template<typename Derived>
template<int Index>
inline typename MatrixBase<Derived>::template ConstDiagonalIndexReturnType<Index>::Type
MatrixBase<Derived>::diagonal() const
{
  return derived();
}

} 

#endif 
