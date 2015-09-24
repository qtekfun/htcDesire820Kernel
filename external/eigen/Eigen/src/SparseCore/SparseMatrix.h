// Copyright (C) 2008-2010 Gael Guennebaud <gael.guennebaud@inria.fr>
// Public License v. 2.0. If a copy of the MPL was not distributed

#ifndef EIGEN_SPARSEMATRIX_H
#define EIGEN_SPARSEMATRIX_H

namespace Eigen { 


namespace internal {
template<typename _Scalar, int _Options, typename _Index>
struct traits<SparseMatrix<_Scalar, _Options, _Index> >
{
  typedef _Scalar Scalar;
  typedef _Index Index;
  typedef Sparse StorageKind;
  typedef MatrixXpr XprKind;
  enum {
    RowsAtCompileTime = Dynamic,
    ColsAtCompileTime = Dynamic,
    MaxRowsAtCompileTime = Dynamic,
    MaxColsAtCompileTime = Dynamic,
    Flags = _Options | NestByRefBit | LvalueBit,
    CoeffReadCost = NumTraits<Scalar>::ReadCost,
    SupportedAccessPatterns = InnerRandomAccessPattern
  };
};

template<typename _Scalar, int _Options, typename _Index, int DiagIndex>
struct traits<Diagonal<const SparseMatrix<_Scalar, _Options, _Index>, DiagIndex> >
{
  typedef SparseMatrix<_Scalar, _Options, _Index> MatrixType;
  typedef typename nested<MatrixType>::type MatrixTypeNested;
  typedef typename remove_reference<MatrixTypeNested>::type _MatrixTypeNested;

  typedef _Scalar Scalar;
  typedef Dense StorageKind;
  typedef _Index Index;
  typedef MatrixXpr XprKind;

  enum {
    RowsAtCompileTime = Dynamic,
    ColsAtCompileTime = 1,
    MaxRowsAtCompileTime = Dynamic,
    MaxColsAtCompileTime = 1,
    Flags = 0,
    CoeffReadCost = _MatrixTypeNested::CoeffReadCost*10
  };
};

} 

template<typename _Scalar, int _Options, typename _Index>
class SparseMatrix
  : public SparseMatrixBase<SparseMatrix<_Scalar, _Options, _Index> >
{
  public:
    EIGEN_SPARSE_PUBLIC_INTERFACE(SparseMatrix)
    EIGEN_SPARSE_INHERIT_ASSIGNMENT_OPERATOR(SparseMatrix, +=)
    EIGEN_SPARSE_INHERIT_ASSIGNMENT_OPERATOR(SparseMatrix, -=)

    typedef MappedSparseMatrix<Scalar,Flags> Map;
    using Base::IsRowMajor;
    typedef internal::CompressedStorage<Scalar,Index> Storage;
    enum {
      Options = _Options
    };

  protected:

    typedef SparseMatrix<Scalar,(Flags&~RowMajorBit)|(IsRowMajor?RowMajorBit:0)> TransposedSparseMatrix;

    Index m_outerSize;
    Index m_innerSize;
    Index* m_outerIndex;
    Index* m_innerNonZeros;     
    Storage m_data;
    
    Eigen::Map<Matrix<Index,Dynamic,1> > innerNonZeros() { return Eigen::Map<Matrix<Index,Dynamic,1> >(m_innerNonZeros, m_innerNonZeros?m_outerSize:0); }
    const  Eigen::Map<const Matrix<Index,Dynamic,1> > innerNonZeros() const { return Eigen::Map<const Matrix<Index,Dynamic,1> >(m_innerNonZeros, m_innerNonZeros?m_outerSize:0); }

  public:
    
    
    inline bool isCompressed() const { return m_innerNonZeros==0; }

    
    inline Index rows() const { return IsRowMajor ? m_outerSize : m_innerSize; }
    
    inline Index cols() const { return IsRowMajor ? m_innerSize : m_outerSize; }

    
    inline Index innerSize() const { return m_innerSize; }
    
    inline Index outerSize() const { return m_outerSize; }
    
    inline const Scalar* valuePtr() const { return &m_data.value(0); }
    inline Scalar* valuePtr() { return &m_data.value(0); }

    inline const Index* innerIndexPtr() const { return &m_data.index(0); }
    inline Index* innerIndexPtr() { return &m_data.index(0); }

    inline const Index* outerIndexPtr() const { return m_outerIndex; }
    inline Index* outerIndexPtr() { return m_outerIndex; }

    inline const Index* innerNonZeroPtr() const { return m_innerNonZeros; }
    inline Index* innerNonZeroPtr() { return m_innerNonZeros; }

    
    inline Storage& data() { return m_data; }
    
    inline const Storage& data() const { return m_data; }

    inline Scalar coeff(Index row, Index col) const
    {
      const Index outer = IsRowMajor ? row : col;
      const Index inner = IsRowMajor ? col : row;
      Index end = m_innerNonZeros ? m_outerIndex[outer] + m_innerNonZeros[outer] : m_outerIndex[outer+1];
      return m_data.atInRange(m_outerIndex[outer], end, inner);
    }

    inline Scalar& coeffRef(Index row, Index col)
    {
      const Index outer = IsRowMajor ? row : col;
      const Index inner = IsRowMajor ? col : row;

      Index start = m_outerIndex[outer];
      Index end = m_innerNonZeros ? m_outerIndex[outer] + m_innerNonZeros[outer] : m_outerIndex[outer+1];
      eigen_assert(end>=start && "you probably called coeffRef on a non finalized matrix");
      if(end<=start)
        return insert(row,col);
      const Index p = m_data.searchLowerIndex(start,end-1,inner);
      if((p<end) && (m_data.index(p)==inner))
        return m_data.value(p);
      else
        return insert(row,col);
    }

    EIGEN_DONT_INLINE Scalar& insert(Index row, Index col)
    {
      if(isCompressed())
      {
        reserve(VectorXi::Constant(outerSize(), 2));
      }
      return insertUncompressed(row,col);
    }

  public:

    class InnerIterator;
    class ReverseInnerIterator;

    
    inline void setZero()
    {
      m_data.clear();
      memset(m_outerIndex, 0, (m_outerSize+1)*sizeof(Index));
      if(m_innerNonZeros)
        memset(m_innerNonZeros, 0, (m_outerSize)*sizeof(Index));
    }

    
    inline Index nonZeros() const
    {
      if(m_innerNonZeros)
        return innerNonZeros().sum();
      return static_cast<Index>(m_data.size());
    }

    inline void reserve(Index reserveSize)
    {
      eigen_assert(isCompressed() && "This function does not make sense in non compressed mode.");
      m_data.reserve(reserveSize);
    }
    
    #ifdef EIGEN_PARSED_BY_DOXYGEN
    template<class SizesType>
    inline void reserve(const SizesType& reserveSizes);
    #else
    template<class SizesType>
    inline void reserve(const SizesType& reserveSizes, const typename SizesType::value_type& enableif = typename SizesType::value_type())
    {
      EIGEN_UNUSED_VARIABLE(enableif);
      reserveInnerVectors(reserveSizes);
    }
    template<class SizesType>
    inline void reserve(const SizesType& reserveSizes, const typename SizesType::Scalar& enableif =
    #if (!defined(_MSC_VER)) || (_MSC_VER>=1500) 
        typename
    #endif
        SizesType::Scalar())
    {
      EIGEN_UNUSED_VARIABLE(enableif);
      reserveInnerVectors(reserveSizes);
    }
    #endif 
  protected:
    template<class SizesType>
    inline void reserveInnerVectors(const SizesType& reserveSizes)
    {
      
      if(isCompressed())
      {
        std::size_t totalReserveSize = 0;
        
        m_innerNonZeros = new Index[m_outerSize];
        
        
        Index* newOuterIndex = m_innerNonZeros;
        
        Index count = 0;
        for(Index j=0; j<m_outerSize; ++j)
        {
          newOuterIndex[j] = count;
          count += reserveSizes[j] + (m_outerIndex[j+1]-m_outerIndex[j]);
          totalReserveSize += reserveSizes[j];
        }
        m_data.reserve(totalReserveSize);
        std::ptrdiff_t previousOuterIndex = m_outerIndex[m_outerSize];
        for(std::ptrdiff_t j=m_outerSize-1; j>=0; --j)
        {
          ptrdiff_t innerNNZ = previousOuterIndex - m_outerIndex[j];
          for(std::ptrdiff_t i=innerNNZ-1; i>=0; --i)
          {
            m_data.index(newOuterIndex[j]+i) = m_data.index(m_outerIndex[j]+i);
            m_data.value(newOuterIndex[j]+i) = m_data.value(m_outerIndex[j]+i);
          }
          previousOuterIndex = m_outerIndex[j];
          m_outerIndex[j] = newOuterIndex[j];
          m_innerNonZeros[j] = innerNNZ;
        }
        m_outerIndex[m_outerSize] = m_outerIndex[m_outerSize-1] + m_innerNonZeros[m_outerSize-1] + reserveSizes[m_outerSize-1];
        
        m_data.resize(m_outerIndex[m_outerSize]);
      }
      else
      {
        Index* newOuterIndex = new Index[m_outerSize+1];
        Index count = 0;
        for(Index j=0; j<m_outerSize; ++j)
        {
          newOuterIndex[j] = count;
          Index alreadyReserved = (m_outerIndex[j+1]-m_outerIndex[j]) - m_innerNonZeros[j];
          Index toReserve = std::max<std::ptrdiff_t>(reserveSizes[j], alreadyReserved);
          count += toReserve + m_innerNonZeros[j];
        }
        newOuterIndex[m_outerSize] = count;
        
        m_data.resize(count);
        for(ptrdiff_t j=m_outerSize-1; j>=0; --j)
        {
          std::ptrdiff_t offset = newOuterIndex[j] - m_outerIndex[j];
          if(offset>0)
          {
            std::ptrdiff_t innerNNZ = m_innerNonZeros[j];
            for(std::ptrdiff_t i=innerNNZ-1; i>=0; --i)
            {
              m_data.index(newOuterIndex[j]+i) = m_data.index(m_outerIndex[j]+i);
              m_data.value(newOuterIndex[j]+i) = m_data.value(m_outerIndex[j]+i);
            }
          }
        }
        
        std::swap(m_outerIndex, newOuterIndex);
        delete[] newOuterIndex;
      }
      
    }
  public:

    

    inline Scalar& insertBack(Index row, Index col)
    {
      return insertBackByOuterInner(IsRowMajor?row:col, IsRowMajor?col:row);
    }

    inline Scalar& insertBackByOuterInner(Index outer, Index inner)
    {
      eigen_assert(size_t(m_outerIndex[outer+1]) == m_data.size() && "Invalid ordered insertion (invalid outer index)");
      eigen_assert( (m_outerIndex[outer+1]-m_outerIndex[outer]==0 || m_data.index(m_data.size()-1)<inner) && "Invalid ordered insertion (invalid inner index)");
      Index p = m_outerIndex[outer+1];
      ++m_outerIndex[outer+1];
      m_data.append(0, inner);
      return m_data.value(p);
    }

    inline Scalar& insertBackByOuterInnerUnordered(Index outer, Index inner)
    {
      Index p = m_outerIndex[outer+1];
      ++m_outerIndex[outer+1];
      m_data.append(0, inner);
      return m_data.value(p);
    }

    inline void startVec(Index outer)
    {
      eigen_assert(m_outerIndex[outer]==int(m_data.size()) && "You must call startVec for each inner vector sequentially");
      eigen_assert(m_outerIndex[outer+1]==0 && "You must call startVec for each inner vector sequentially");
      m_outerIndex[outer+1] = m_outerIndex[outer];
    }

    inline void finalize()
    {
      if(isCompressed())
      {
        Index size = static_cast<Index>(m_data.size());
        Index i = m_outerSize;
        
        while (i>=0 && m_outerIndex[i]==0)
          --i;
        ++i;
        while (i<=m_outerSize)
        {
          m_outerIndex[i] = size;
          ++i;
        }
      }
    }

    

    template<typename InputIterators>
    void setFromTriplets(const InputIterators& begin, const InputIterators& end);

    void sumupDuplicates();

    
    
    EIGEN_DONT_INLINE Scalar& insertByOuterInner(Index j, Index i)
    {
      return insert(IsRowMajor ? j : i, IsRowMajor ? i : j);
    }

    void makeCompressed()
    {
      if(isCompressed())
        return;
      
      Index oldStart = m_outerIndex[1];
      m_outerIndex[1] = m_innerNonZeros[0];
      for(Index j=1; j<m_outerSize; ++j)
      {
        Index nextOldStart = m_outerIndex[j+1];
        std::ptrdiff_t offset = oldStart - m_outerIndex[j];
        if(offset>0)
        {
          for(Index k=0; k<m_innerNonZeros[j]; ++k)
          {
            m_data.index(m_outerIndex[j]+k) = m_data.index(oldStart+k);
            m_data.value(m_outerIndex[j]+k) = m_data.value(oldStart+k);
          }
        }
        m_outerIndex[j+1] = m_outerIndex[j] + m_innerNonZeros[j];
        oldStart = nextOldStart;
      }
      delete[] m_innerNonZeros;
      m_innerNonZeros = 0;
      m_data.resize(m_outerIndex[m_outerSize]);
      m_data.squeeze();
    }

    
    void prune(Scalar reference, RealScalar epsilon = NumTraits<RealScalar>::dummy_precision())
    {
      prune(default_prunning_func(reference,epsilon));
    }
    
    template<typename KeepFunc>
    void prune(const KeepFunc& keep = KeepFunc())
    {
      
      
      makeCompressed();

      Index k = 0;
      for(Index j=0; j<m_outerSize; ++j)
      {
        Index previousStart = m_outerIndex[j];
        m_outerIndex[j] = k;
        Index end = m_outerIndex[j+1];
        for(Index i=previousStart; i<end; ++i)
        {
          if(keep(IsRowMajor?j:m_data.index(i), IsRowMajor?m_data.index(i):j, m_data.value(i)))
          {
            m_data.value(k) = m_data.value(i);
            m_data.index(k) = m_data.index(i);
            ++k;
          }
        }
      }
      m_outerIndex[m_outerSize] = k;
      m_data.resize(k,0);
    }

    void resize(Index rows, Index cols)
    {
      const Index outerSize = IsRowMajor ? rows : cols;
      m_innerSize = IsRowMajor ? cols : rows;
      m_data.clear();
      if (m_outerSize != outerSize || m_outerSize==0)
      {
        delete[] m_outerIndex;
        m_outerIndex = new Index [outerSize+1];
        m_outerSize = outerSize;
      }
      if(m_innerNonZeros)
      {
        delete[] m_innerNonZeros;
        m_innerNonZeros = 0;
      }
      memset(m_outerIndex, 0, (m_outerSize+1)*sizeof(Index));
    }

    void resizeNonZeros(Index size)
    {
      
      m_data.resize(size);
    }

    
    const Diagonal<const SparseMatrix> diagonal() const { return *this; }

    
    inline SparseMatrix()
      : m_outerSize(-1), m_innerSize(0), m_outerIndex(0), m_innerNonZeros(0)
    {
      check_template_parameters();
      resize(0, 0);
    }

    
    inline SparseMatrix(Index rows, Index cols)
      : m_outerSize(0), m_innerSize(0), m_outerIndex(0), m_innerNonZeros(0)
    {
      check_template_parameters();
      resize(rows, cols);
    }

    
    template<typename OtherDerived>
    inline SparseMatrix(const SparseMatrixBase<OtherDerived>& other)
      : m_outerSize(0), m_innerSize(0), m_outerIndex(0), m_innerNonZeros(0)
    {
      check_template_parameters();
      *this = other.derived();
    }

    
    inline SparseMatrix(const SparseMatrix& other)
      : Base(), m_outerSize(0), m_innerSize(0), m_outerIndex(0), m_innerNonZeros(0)
    {
      check_template_parameters();
      *this = other.derived();
    }

    inline void swap(SparseMatrix& other)
    {
      
      std::swap(m_outerIndex, other.m_outerIndex);
      std::swap(m_innerSize, other.m_innerSize);
      std::swap(m_outerSize, other.m_outerSize);
      std::swap(m_innerNonZeros, other.m_innerNonZeros);
      m_data.swap(other.m_data);
    }

    inline SparseMatrix& operator=(const SparseMatrix& other)
    {
      if (other.isRValue())
      {
        swap(other.const_cast_derived());
      }
      else
      {
        initAssignment(other);
        if(other.isCompressed())
        {
          memcpy(m_outerIndex, other.m_outerIndex, (m_outerSize+1)*sizeof(Index));
          m_data = other.m_data;
        }
        else
        {
          Base::operator=(other);
        }
      }
      return *this;
    }

    #ifndef EIGEN_PARSED_BY_DOXYGEN
    template<typename Lhs, typename Rhs>
    inline SparseMatrix& operator=(const SparseSparseProduct<Lhs,Rhs>& product)
    { return Base::operator=(product); }
    
    template<typename OtherDerived>
    inline SparseMatrix& operator=(const ReturnByValue<OtherDerived>& other)
    { return Base::operator=(other.derived()); }
    
    template<typename OtherDerived>
    inline SparseMatrix& operator=(const EigenBase<OtherDerived>& other)
    { return Base::operator=(other.derived()); }
    #endif

    template<typename OtherDerived>
    EIGEN_DONT_INLINE SparseMatrix& operator=(const SparseMatrixBase<OtherDerived>& other)
    {
      initAssignment(other.derived());
      const bool needToTranspose = (Flags & RowMajorBit) != (OtherDerived::Flags & RowMajorBit);
      if (needToTranspose)
      {
        
        
        
        
        typedef typename internal::nested<OtherDerived,2>::type OtherCopy;
        typedef typename internal::remove_all<OtherCopy>::type _OtherCopy;
        OtherCopy otherCopy(other.derived());

        Eigen::Map<Matrix<Index, Dynamic, 1> > (m_outerIndex,outerSize()).setZero();
        
        
        for (Index j=0; j<otherCopy.outerSize(); ++j)
          for (typename _OtherCopy::InnerIterator it(otherCopy, j); it; ++it)
            ++m_outerIndex[it.index()];

        
        Index count = 0;
        VectorXi positions(outerSize());
        for (Index j=0; j<outerSize(); ++j)
        {
          Index tmp = m_outerIndex[j];
          m_outerIndex[j] = count;
          positions[j] = count;
          count += tmp;
        }
        m_outerIndex[outerSize()] = count;
        
        m_data.resize(count);
        
        for (Index j=0; j<otherCopy.outerSize(); ++j)
        {
          for (typename _OtherCopy::InnerIterator it(otherCopy, j); it; ++it)
          {
            Index pos = positions[it.index()]++;
            m_data.index(pos) = j;
            m_data.value(pos) = it.value();
          }
        }
        return *this;
      }
      else
      {
        
        return Base::operator=(other.derived());
      }
    }

    friend std::ostream & operator << (std::ostream & s, const SparseMatrix& m)
    {
      EIGEN_DBG_SPARSE(
        s << "Nonzero entries:\n";
        if(m.isCompressed())
          for (Index i=0; i<m.nonZeros(); ++i)
            s << "(" << m.m_data.value(i) << "," << m.m_data.index(i) << ") ";
        else
          for (Index i=0; i<m.outerSize(); ++i)
          {
            int p = m.m_outerIndex[i];
            int pe = m.m_outerIndex[i]+m.m_innerNonZeros[i];
            Index k=p;
            for (; k<pe; ++k)
              s << "(" << m.m_data.value(k) << "," << m.m_data.index(k) << ") ";
            for (; k<m.m_outerIndex[i+1]; ++k)
              s << "(_,_) ";
          }
        s << std::endl;
        s << std::endl;
        s << "Outer pointers:\n";
        for (Index i=0; i<m.outerSize(); ++i)
          s << m.m_outerIndex[i] << " ";
        s << " $" << std::endl;
        if(!m.isCompressed())
        {
          s << "Inner non zeros:\n";
          for (Index i=0; i<m.outerSize(); ++i)
            s << m.m_innerNonZeros[i] << " ";
          s << " $" << std::endl;
        }
        s << std::endl;
      );
      s << static_cast<const SparseMatrixBase<SparseMatrix>&>(m);
      return s;
    }

    
    inline ~SparseMatrix()
    {
      delete[] m_outerIndex;
      delete[] m_innerNonZeros;
    }

#ifndef EIGEN_PARSED_BY_DOXYGEN
    
    Scalar sum() const;
#endif
    
#   ifdef EIGEN_SPARSEMATRIX_PLUGIN
#     include EIGEN_SPARSEMATRIX_PLUGIN
#   endif

protected:

    template<typename Other>
    void initAssignment(const Other& other)
    {
      resize(other.rows(), other.cols());
      if(m_innerNonZeros)
      {
        delete[] m_innerNonZeros;
        m_innerNonZeros = 0;
      }
    }

    EIGEN_DONT_INLINE Scalar& insertCompressed(Index row, Index col)
    {
      eigen_assert(isCompressed());

      const Index outer = IsRowMajor ? row : col;
      const Index inner = IsRowMajor ? col : row;

      Index previousOuter = outer;
      if (m_outerIndex[outer+1]==0)
      {
        
        while (previousOuter>=0 && m_outerIndex[previousOuter]==0)
        {
          m_outerIndex[previousOuter] = static_cast<Index>(m_data.size());
          --previousOuter;
        }
        m_outerIndex[outer+1] = m_outerIndex[outer];
      }

      
      
      
      bool isLastVec = (!(previousOuter==-1 && m_data.size()!=0))
                    && (size_t(m_outerIndex[outer+1]) == m_data.size());

      size_t startId = m_outerIndex[outer];
      
      size_t p = m_outerIndex[outer+1];
      ++m_outerIndex[outer+1];

      float reallocRatio = 1;
      if (m_data.allocatedSize()<=m_data.size())
      {
        
        if (m_data.size()==0)
        {
          m_data.reserve(32);
        }
        else
        {
          
          
          
          float nnzEstimate = float(m_outerIndex[outer])*float(m_outerSize)/float(outer+1);
          reallocRatio = (nnzEstimate-float(m_data.size()))/float(m_data.size());
          
          
          
          reallocRatio = (std::min)((std::max)(reallocRatio,1.5f),8.f);
        }
      }
      m_data.resize(m_data.size()+1,reallocRatio);

      if (!isLastVec)
      {
        if (previousOuter==-1)
        {
          
          
          for (Index k=0; k<=(outer+1); ++k)
            m_outerIndex[k] = 0;
          Index k=outer+1;
          while(m_outerIndex[k]==0)
            m_outerIndex[k++] = 1;
          while (k<=m_outerSize && m_outerIndex[k]!=0)
            m_outerIndex[k++]++;
          p = 0;
          --k;
          k = m_outerIndex[k]-1;
          while (k>0)
          {
            m_data.index(k) = m_data.index(k-1);
            m_data.value(k) = m_data.value(k-1);
            k--;
          }
        }
        else
        {
          
          
          Index j = outer+2;
          while (j<=m_outerSize && m_outerIndex[j]!=0)
            m_outerIndex[j++]++;
          --j;
          
          Index k = m_outerIndex[j]-1;
          while (k>=Index(p))
          {
            m_data.index(k) = m_data.index(k-1);
            m_data.value(k) = m_data.value(k-1);
            k--;
          }
        }
      }

      while ( (p > startId) && (m_data.index(p-1) > inner) )
      {
        m_data.index(p) = m_data.index(p-1);
        m_data.value(p) = m_data.value(p-1);
        --p;
      }

      m_data.index(p) = inner;
      return (m_data.value(p) = 0);
    }

    class SingletonVector
    {
        Index m_index;
        Index m_value;
      public:
        typedef Index value_type;
        SingletonVector(Index i, Index v)
          : m_index(i), m_value(v)
        {}

        Index operator[](Index i) const { return i==m_index ? m_value : 0; }
    };

    EIGEN_DONT_INLINE Scalar& insertUncompressed(Index row, Index col)
    {
      eigen_assert(!isCompressed());

      const Index outer = IsRowMajor ? row : col;
      const Index inner = IsRowMajor ? col : row;

      std::ptrdiff_t room = m_outerIndex[outer+1] - m_outerIndex[outer];
      std::ptrdiff_t innerNNZ = m_innerNonZeros[outer];
      if(innerNNZ>=room)
      {
        
        reserve(SingletonVector(outer,std::max<std::ptrdiff_t>(2,innerNNZ)));
      }

      Index startId = m_outerIndex[outer];
      Index p = startId + m_innerNonZeros[outer];
      while ( (p > startId) && (m_data.index(p-1) > inner) )
      {
        m_data.index(p) = m_data.index(p-1);
        m_data.value(p) = m_data.value(p-1);
        --p;
      }

      m_innerNonZeros[outer]++;

      m_data.index(p) = inner;
      return (m_data.value(p) = 0);
    }

public:
    inline Scalar& insertBackUncompressed(Index row, Index col)
    {
      const Index outer = IsRowMajor ? row : col;
      const Index inner = IsRowMajor ? col : row;

      eigen_assert(!isCompressed());
      eigen_assert(m_innerNonZeros[outer]<=(m_outerIndex[outer+1] - m_outerIndex[outer]));

      Index p = m_outerIndex[outer] + m_innerNonZeros[outer];
      m_innerNonZeros[outer]++;
      m_data.index(p) = inner;
      return (m_data.value(p) = 0);
    }

private:
  static void check_template_parameters()
  {
    EIGEN_STATIC_ASSERT(NumTraits<Index>::IsSigned,THE_INDEX_TYPE_MUST_BE_A_SIGNED_TYPE);
  }

  struct default_prunning_func {
    default_prunning_func(Scalar ref, RealScalar eps) : reference(ref), epsilon(eps) {}
    inline bool operator() (const Index&, const Index&, const Scalar& value) const
    {
      return !internal::isMuchSmallerThan(value, reference, epsilon);
    }
    Scalar reference;
    RealScalar epsilon;
  };
};

template<typename Scalar, int _Options, typename _Index>
class SparseMatrix<Scalar,_Options,_Index>::InnerIterator
{
  public:
    InnerIterator(const SparseMatrix& mat, Index outer)
      : m_values(mat.valuePtr()), m_indices(mat.innerIndexPtr()), m_outer(outer), m_id(mat.m_outerIndex[outer])
    {
      if(mat.isCompressed())
        m_end = mat.m_outerIndex[outer+1];
      else
        m_end = m_id + mat.m_innerNonZeros[outer];
    }

    inline InnerIterator& operator++() { m_id++; return *this; }

    inline const Scalar& value() const { return m_values[m_id]; }
    inline Scalar& valueRef() { return const_cast<Scalar&>(m_values[m_id]); }

    inline Index index() const { return m_indices[m_id]; }
    inline Index outer() const { return m_outer; }
    inline Index row() const { return IsRowMajor ? m_outer : index(); }
    inline Index col() const { return IsRowMajor ? index() : m_outer; }

    inline operator bool() const { return (m_id < m_end); }

  protected:
    const Scalar* m_values;
    const Index* m_indices;
    const Index m_outer;
    Index m_id;
    Index m_end;
};

template<typename Scalar, int _Options, typename _Index>
class SparseMatrix<Scalar,_Options,_Index>::ReverseInnerIterator
{
  public:
    ReverseInnerIterator(const SparseMatrix& mat, Index outer)
      : m_values(mat.valuePtr()), m_indices(mat.innerIndexPtr()), m_outer(outer), m_start(mat.m_outerIndex[outer])
    {
      if(mat.isCompressed())
        m_id = mat.m_outerIndex[outer+1];
      else
        m_id = m_start + mat.m_innerNonZeros[outer];
    }

    inline ReverseInnerIterator& operator--() { --m_id; return *this; }

    inline const Scalar& value() const { return m_values[m_id-1]; }
    inline Scalar& valueRef() { return const_cast<Scalar&>(m_values[m_id-1]); }

    inline Index index() const { return m_indices[m_id-1]; }
    inline Index outer() const { return m_outer; }
    inline Index row() const { return IsRowMajor ? m_outer : index(); }
    inline Index col() const { return IsRowMajor ? index() : m_outer; }

    inline operator bool() const { return (m_id > m_start); }

  protected:
    const Scalar* m_values;
    const Index* m_indices;
    const Index m_outer;
    Index m_id;
    const Index m_start;
};

namespace internal {

template<typename InputIterator, typename SparseMatrixType>
void set_from_triplets(const InputIterator& begin, const InputIterator& end, SparseMatrixType& mat, int Options = 0)
{
  EIGEN_UNUSED_VARIABLE(Options);
  enum { IsRowMajor = SparseMatrixType::IsRowMajor };
  typedef typename SparseMatrixType::Scalar Scalar;
  typedef typename SparseMatrixType::Index Index;
  SparseMatrix<Scalar,IsRowMajor?ColMajor:RowMajor> trMat(mat.rows(),mat.cols());

  
  VectorXi wi(trMat.outerSize());
  wi.setZero();
  for(InputIterator it(begin); it!=end; ++it)
    wi(IsRowMajor ? it->col() : it->row())++;

  
  trMat.reserve(wi);
  for(InputIterator it(begin); it!=end; ++it)
    trMat.insertBackUncompressed(it->row(),it->col()) = it->value();

  
  trMat.sumupDuplicates();

  
  mat = trMat;
}

}


template<typename Scalar, int _Options, typename _Index>
template<typename InputIterators>
void SparseMatrix<Scalar,_Options,_Index>::setFromTriplets(const InputIterators& begin, const InputIterators& end)
{
  internal::set_from_triplets(begin, end, *this);
}

template<typename Scalar, int _Options, typename _Index>
void SparseMatrix<Scalar,_Options,_Index>::sumupDuplicates()
{
  eigen_assert(!isCompressed());
  
  VectorXi wi(innerSize());
  wi.fill(-1);
  Index count = 0;
  
  for(int j=0; j<outerSize(); ++j)
  {
    Index start   = count;
    Index oldEnd  = m_outerIndex[j]+m_innerNonZeros[j];
    for(Index k=m_outerIndex[j]; k<oldEnd; ++k)
    {
      Index i = m_data.index(k);
      if(wi(i)>=start)
      {
        
        m_data.value(wi(i)) += m_data.value(k);
      }
      else
      {
        m_data.value(count) = m_data.value(k);
        m_data.index(count) = m_data.index(k);
        wi(i) = count;
        ++count;
      }
    }
    m_outerIndex[j] = start;
  }
  m_outerIndex[m_outerSize] = count;

  
  delete[] m_innerNonZeros;
  m_innerNonZeros = 0;
  m_data.resize(m_outerIndex[m_outerSize]);
}

} 

#endif 
