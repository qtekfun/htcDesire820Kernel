// Copyright (C) 2008 Gael Guennebaud <gael.guennebaud@inria.fr>
// Copyright (C) 2006-2010 Benoit Jacob <jacob.benoit.1@gmail.com>
// Public License v. 2.0. If a copy of the MPL was not distributed

#ifndef EIGEN_BLOCK_H
#define EIGEN_BLOCK_H

namespace Eigen { 


namespace internal {
template<typename XprType, int BlockRows, int BlockCols, bool InnerPanel, bool HasDirectAccess>
struct traits<Block<XprType, BlockRows, BlockCols, InnerPanel, HasDirectAccess> > : traits<XprType>
{
  typedef typename traits<XprType>::Scalar Scalar;
  typedef typename traits<XprType>::StorageKind StorageKind;
  typedef typename traits<XprType>::XprKind XprKind;
  typedef typename nested<XprType>::type XprTypeNested;
  typedef typename remove_reference<XprTypeNested>::type _XprTypeNested;
  enum{
    MatrixRows = traits<XprType>::RowsAtCompileTime,
    MatrixCols = traits<XprType>::ColsAtCompileTime,
    RowsAtCompileTime = MatrixRows == 0 ? 0 : BlockRows,
    ColsAtCompileTime = MatrixCols == 0 ? 0 : BlockCols,
    MaxRowsAtCompileTime = BlockRows==0 ? 0
                         : RowsAtCompileTime != Dynamic ? int(RowsAtCompileTime)
                         : int(traits<XprType>::MaxRowsAtCompileTime),
    MaxColsAtCompileTime = BlockCols==0 ? 0
                         : ColsAtCompileTime != Dynamic ? int(ColsAtCompileTime)
                         : int(traits<XprType>::MaxColsAtCompileTime),
    XprTypeIsRowMajor = (int(traits<XprType>::Flags)&RowMajorBit) != 0,
    IsRowMajor = (MaxRowsAtCompileTime==1&&MaxColsAtCompileTime!=1) ? 1
               : (MaxColsAtCompileTime==1&&MaxRowsAtCompileTime!=1) ? 0
               : XprTypeIsRowMajor,
    HasSameStorageOrderAsXprType = (IsRowMajor == XprTypeIsRowMajor),
    InnerSize = IsRowMajor ? int(ColsAtCompileTime) : int(RowsAtCompileTime),
    InnerStrideAtCompileTime = HasSameStorageOrderAsXprType
                             ? int(inner_stride_at_compile_time<XprType>::ret)
                             : int(outer_stride_at_compile_time<XprType>::ret),
    OuterStrideAtCompileTime = HasSameStorageOrderAsXprType
                             ? int(outer_stride_at_compile_time<XprType>::ret)
                             : int(inner_stride_at_compile_time<XprType>::ret),
    MaskPacketAccessBit = (InnerSize == Dynamic || (InnerSize % packet_traits<Scalar>::size) == 0)
                       && (InnerStrideAtCompileTime == 1)
                        ? PacketAccessBit : 0,
    MaskAlignedBit = (InnerPanel && (OuterStrideAtCompileTime!=Dynamic) && (((OuterStrideAtCompileTime * int(sizeof(Scalar))) % 16) == 0)) ? AlignedBit : 0,
    FlagsLinearAccessBit = (RowsAtCompileTime == 1 || ColsAtCompileTime == 1) ? LinearAccessBit : 0,
    FlagsLvalueBit = is_lvalue<XprType>::value ? LvalueBit : 0,
    FlagsRowMajorBit = IsRowMajor ? RowMajorBit : 0,
    Flags0 = traits<XprType>::Flags & ( (HereditaryBits & ~RowMajorBit) |
                                        DirectAccessBit |
                                        MaskPacketAccessBit |
                                        MaskAlignedBit),
    Flags = Flags0 | FlagsLinearAccessBit | FlagsLvalueBit | FlagsRowMajorBit
  };
};
}

template<typename XprType, int BlockRows, int BlockCols, bool InnerPanel, bool HasDirectAccess> class Block
  : public internal::dense_xpr_base<Block<XprType, BlockRows, BlockCols, InnerPanel, HasDirectAccess> >::type
{
  public:

    typedef typename internal::dense_xpr_base<Block>::type Base;
    EIGEN_DENSE_PUBLIC_INTERFACE(Block)

    class InnerIterator;

    inline Block(XprType& xpr, Index i)
      : m_xpr(xpr),
        
        
        
        
        m_startRow( (BlockRows==1) && (BlockCols==XprType::ColsAtCompileTime) ? i : 0),
        m_startCol( (BlockRows==XprType::RowsAtCompileTime) && (BlockCols==1) ? i : 0),
        m_blockRows(BlockRows==1 ? 1 : xpr.rows()),
        m_blockCols(BlockCols==1 ? 1 : xpr.cols())
    {
      eigen_assert( (i>=0) && (
          ((BlockRows==1) && (BlockCols==XprType::ColsAtCompileTime) && i<xpr.rows())
        ||((BlockRows==XprType::RowsAtCompileTime) && (BlockCols==1) && i<xpr.cols())));
    }

    inline Block(XprType& xpr, Index startRow, Index startCol)
      : m_xpr(xpr), m_startRow(startRow), m_startCol(startCol),
        m_blockRows(BlockRows), m_blockCols(BlockCols)
    {
      EIGEN_STATIC_ASSERT(RowsAtCompileTime!=Dynamic && ColsAtCompileTime!=Dynamic,THIS_METHOD_IS_ONLY_FOR_FIXED_SIZE)
      eigen_assert(startRow >= 0 && BlockRows >= 1 && startRow + BlockRows <= xpr.rows()
             && startCol >= 0 && BlockCols >= 1 && startCol + BlockCols <= xpr.cols());
    }

    inline Block(XprType& xpr,
          Index startRow, Index startCol,
          Index blockRows, Index blockCols)
      : m_xpr(xpr), m_startRow(startRow), m_startCol(startCol),
                          m_blockRows(blockRows), m_blockCols(blockCols)
    {
      eigen_assert((RowsAtCompileTime==Dynamic || RowsAtCompileTime==blockRows)
          && (ColsAtCompileTime==Dynamic || ColsAtCompileTime==blockCols));
      eigen_assert(startRow >= 0 && blockRows >= 0 && startRow + blockRows <= xpr.rows()
          && startCol >= 0 && blockCols >= 0 && startCol + blockCols <= xpr.cols());
    }

    EIGEN_INHERIT_ASSIGNMENT_OPERATORS(Block)

    inline Index rows() const { return m_blockRows.value(); }
    inline Index cols() const { return m_blockCols.value(); }

    inline Scalar& coeffRef(Index row, Index col)
    {
      EIGEN_STATIC_ASSERT_LVALUE(XprType)
      return m_xpr.const_cast_derived()
               .coeffRef(row + m_startRow.value(), col + m_startCol.value());
    }

    inline const Scalar& coeffRef(Index row, Index col) const
    {
      return m_xpr.derived()
               .coeffRef(row + m_startRow.value(), col + m_startCol.value());
    }

    EIGEN_STRONG_INLINE const CoeffReturnType coeff(Index row, Index col) const
    {
      return m_xpr.coeff(row + m_startRow.value(), col + m_startCol.value());
    }

    inline Scalar& coeffRef(Index index)
    {
      EIGEN_STATIC_ASSERT_LVALUE(XprType)
      return m_xpr.const_cast_derived()
             .coeffRef(m_startRow.value() + (RowsAtCompileTime == 1 ? 0 : index),
                       m_startCol.value() + (RowsAtCompileTime == 1 ? index : 0));
    }

    inline const Scalar& coeffRef(Index index) const
    {
      return m_xpr.const_cast_derived()
             .coeffRef(m_startRow.value() + (RowsAtCompileTime == 1 ? 0 : index),
                       m_startCol.value() + (RowsAtCompileTime == 1 ? index : 0));
    }

    inline const CoeffReturnType coeff(Index index) const
    {
      return m_xpr
             .coeff(m_startRow.value() + (RowsAtCompileTime == 1 ? 0 : index),
                    m_startCol.value() + (RowsAtCompileTime == 1 ? index : 0));
    }

    template<int LoadMode>
    inline PacketScalar packet(Index row, Index col) const
    {
      return m_xpr.template packet<Unaligned>
              (row + m_startRow.value(), col + m_startCol.value());
    }

    template<int LoadMode>
    inline void writePacket(Index row, Index col, const PacketScalar& x)
    {
      m_xpr.const_cast_derived().template writePacket<Unaligned>
              (row + m_startRow.value(), col + m_startCol.value(), x);
    }

    template<int LoadMode>
    inline PacketScalar packet(Index index) const
    {
      return m_xpr.template packet<Unaligned>
              (m_startRow.value() + (RowsAtCompileTime == 1 ? 0 : index),
               m_startCol.value() + (RowsAtCompileTime == 1 ? index : 0));
    }

    template<int LoadMode>
    inline void writePacket(Index index, const PacketScalar& x)
    {
      m_xpr.const_cast_derived().template writePacket<Unaligned>
         (m_startRow.value() + (RowsAtCompileTime == 1 ? 0 : index),
          m_startCol.value() + (RowsAtCompileTime == 1 ? index : 0), x);
    }

    #ifdef EIGEN_PARSED_BY_DOXYGEN
    
    inline const Scalar* data() const;
    inline Index innerStride() const;
    inline Index outerStride() const;
    #endif

    const typename internal::remove_all<typename XprType::Nested>::type& nestedExpression() const 
    { 
      return m_xpr; 
    }
      
    Index startRow() const 
    { 
      return m_startRow.value(); 
    }
      
    Index startCol() const 
    { 
      return m_startCol.value(); 
    }

  protected:

    const typename XprType::Nested m_xpr;
    const internal::variable_if_dynamic<Index, XprType::RowsAtCompileTime == 1 ? 0 : Dynamic> m_startRow;
    const internal::variable_if_dynamic<Index, XprType::ColsAtCompileTime == 1 ? 0 : Dynamic> m_startCol;
    const internal::variable_if_dynamic<Index, RowsAtCompileTime> m_blockRows;
    const internal::variable_if_dynamic<Index, ColsAtCompileTime> m_blockCols;
};

template<typename XprType, int BlockRows, int BlockCols, bool InnerPanel>
class Block<XprType,BlockRows,BlockCols, InnerPanel,true>
  : public MapBase<Block<XprType, BlockRows, BlockCols, InnerPanel, true> >
{
  public:

    typedef MapBase<Block> Base;
    EIGEN_DENSE_PUBLIC_INTERFACE(Block)

    EIGEN_INHERIT_ASSIGNMENT_OPERATORS(Block)

    inline Block(XprType& xpr, Index i)
      : Base(internal::const_cast_ptr(&xpr.coeffRef(
              (BlockRows==1) && (BlockCols==XprType::ColsAtCompileTime) ? i : 0,
              (BlockRows==XprType::RowsAtCompileTime) && (BlockCols==1) ? i : 0)),
             BlockRows==1 ? 1 : xpr.rows(),
             BlockCols==1 ? 1 : xpr.cols()),
        m_xpr(xpr)
    {
      eigen_assert( (i>=0) && (
          ((BlockRows==1) && (BlockCols==XprType::ColsAtCompileTime) && i<xpr.rows())
        ||((BlockRows==XprType::RowsAtCompileTime) && (BlockCols==1) && i<xpr.cols())));
      init();
    }

    inline Block(XprType& xpr, Index startRow, Index startCol)
      : Base(internal::const_cast_ptr(&xpr.coeffRef(startRow,startCol))), m_xpr(xpr)
    {
      eigen_assert(startRow >= 0 && BlockRows >= 1 && startRow + BlockRows <= xpr.rows()
             && startCol >= 0 && BlockCols >= 1 && startCol + BlockCols <= xpr.cols());
      init();
    }

    inline Block(XprType& xpr,
          Index startRow, Index startCol,
          Index blockRows, Index blockCols)
      : Base(internal::const_cast_ptr(&xpr.coeffRef(startRow,startCol)), blockRows, blockCols),
        m_xpr(xpr)
    {
      eigen_assert((RowsAtCompileTime==Dynamic || RowsAtCompileTime==blockRows)
             && (ColsAtCompileTime==Dynamic || ColsAtCompileTime==blockCols));
      eigen_assert(startRow >= 0 && blockRows >= 0 && startRow + blockRows <= xpr.rows()
             && startCol >= 0 && blockCols >= 0 && startCol + blockCols <= xpr.cols());
      init();
    }

    const typename internal::remove_all<typename XprType::Nested>::type& nestedExpression() const 
    { 
      return m_xpr; 
    }
      
    
    inline Index innerStride() const
    {
      return internal::traits<Block>::HasSameStorageOrderAsXprType
             ? m_xpr.innerStride()
             : m_xpr.outerStride();
    }

    
    inline Index outerStride() const
    {
      return m_outerStride;
    }

  #ifndef __SUNPRO_CC
  
  
  protected:
  #endif

    #ifndef EIGEN_PARSED_BY_DOXYGEN
    
    inline Block(XprType& xpr, const Scalar* data, Index blockRows, Index blockCols)
      : Base(data, blockRows, blockCols), m_xpr(xpr)
    {
      init();
    }
    #endif

  protected:
    void init()
    {
      m_outerStride = internal::traits<Block>::HasSameStorageOrderAsXprType
                    ? m_xpr.outerStride()
                    : m_xpr.innerStride();
    }

    typename XprType::Nested m_xpr;
    Index m_outerStride;
};

} 

#endif 
