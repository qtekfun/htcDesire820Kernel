// Copyright (C) 2008-2010 Gael Guennebaud <gael.guennebaud@inria.fr>
// Copyright (C) 2006-2008 Benoit Jacob <jacob.benoit.1@gmail.com>
// Public License v. 2.0. If a copy of the MPL was not distributed

#ifndef EIGEN_VECTORBLOCK_H
#define EIGEN_VECTORBLOCK_H

namespace Eigen { 


namespace internal {
template<typename VectorType, int Size>
struct traits<VectorBlock<VectorType, Size> >
  : public traits<Block<VectorType,
                     traits<VectorType>::Flags & RowMajorBit ? 1 : Size,
                     traits<VectorType>::Flags & RowMajorBit ? Size : 1> >
{
};
}

template<typename VectorType, int Size> class VectorBlock
  : public Block<VectorType,
                     internal::traits<VectorType>::Flags & RowMajorBit ? 1 : Size,
                     internal::traits<VectorType>::Flags & RowMajorBit ? Size : 1>
{
    typedef Block<VectorType,
                     internal::traits<VectorType>::Flags & RowMajorBit ? 1 : Size,
                     internal::traits<VectorType>::Flags & RowMajorBit ? Size : 1> Base;
    enum {
      IsColVector = !(internal::traits<VectorType>::Flags & RowMajorBit)
    };
  public:
    EIGEN_DENSE_PUBLIC_INTERFACE(VectorBlock)

    using Base::operator=;

    inline VectorBlock(VectorType& vector, Index start, Index size)
      : Base(vector,
             IsColVector ? start : 0, IsColVector ? 0 : start,
             IsColVector ? size  : 1, IsColVector ? 1 : size)
    {
      EIGEN_STATIC_ASSERT_VECTOR_ONLY(VectorBlock);
    }

    inline VectorBlock(VectorType& vector, Index start)
      : Base(vector, IsColVector ? start : 0, IsColVector ? 0 : start)
    {
      EIGEN_STATIC_ASSERT_VECTOR_ONLY(VectorBlock);
    }
};


template<typename Derived>
inline typename DenseBase<Derived>::SegmentReturnType
DenseBase<Derived>::segment(Index start, Index size)
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived)
  return SegmentReturnType(derived(), start, size);
}

template<typename Derived>
inline typename DenseBase<Derived>::ConstSegmentReturnType
DenseBase<Derived>::segment(Index start, Index size) const
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived)
  return ConstSegmentReturnType(derived(), start, size);
}

template<typename Derived>
inline typename DenseBase<Derived>::SegmentReturnType
DenseBase<Derived>::head(Index size)
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived)
  return SegmentReturnType(derived(), 0, size);
}

template<typename Derived>
inline typename DenseBase<Derived>::ConstSegmentReturnType
DenseBase<Derived>::head(Index size) const
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived)
  return ConstSegmentReturnType(derived(), 0, size);
}

template<typename Derived>
inline typename DenseBase<Derived>::SegmentReturnType
DenseBase<Derived>::tail(Index size)
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived)
  return SegmentReturnType(derived(), this->size() - size, size);
}

template<typename Derived>
inline typename DenseBase<Derived>::ConstSegmentReturnType
DenseBase<Derived>::tail(Index size) const
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived)
  return ConstSegmentReturnType(derived(), this->size() - size, size);
}

template<typename Derived>
template<int Size>
inline typename DenseBase<Derived>::template FixedSegmentReturnType<Size>::Type
DenseBase<Derived>::segment(Index start)
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived)
  return typename FixedSegmentReturnType<Size>::Type(derived(), start);
}

template<typename Derived>
template<int Size>
inline typename DenseBase<Derived>::template ConstFixedSegmentReturnType<Size>::Type
DenseBase<Derived>::segment(Index start) const
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived)
  return typename ConstFixedSegmentReturnType<Size>::Type(derived(), start);
}

template<typename Derived>
template<int Size>
inline typename DenseBase<Derived>::template FixedSegmentReturnType<Size>::Type
DenseBase<Derived>::head()
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived)
  return typename FixedSegmentReturnType<Size>::Type(derived(), 0);
}

template<typename Derived>
template<int Size>
inline typename DenseBase<Derived>::template ConstFixedSegmentReturnType<Size>::Type
DenseBase<Derived>::head() const
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived)
  return typename ConstFixedSegmentReturnType<Size>::Type(derived(), 0);
}

template<typename Derived>
template<int Size>
inline typename DenseBase<Derived>::template FixedSegmentReturnType<Size>::Type
DenseBase<Derived>::tail()
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived)
  return typename FixedSegmentReturnType<Size>::Type(derived(), size() - Size);
}

template<typename Derived>
template<int Size>
inline typename DenseBase<Derived>::template ConstFixedSegmentReturnType<Size>::Type
DenseBase<Derived>::tail() const
{
  EIGEN_STATIC_ASSERT_VECTOR_ONLY(Derived)
  return typename ConstFixedSegmentReturnType<Size>::Type(derived(), size() - Size);
}

} 

#endif 
