// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_TOOLS_BALSA_BALSA_HEADERS_H_
#define NET_TOOLS_BALSA_BALSA_HEADERS_H_

#include <algorithm>
#include <iosfwd>
#include <iterator>
#include <string>
#include <utility>
#include <vector>

#include "base/logging.h"
#include "base/port.h"
#include "base/strings/string_piece.h"
#include "net/tools/balsa/balsa_enums.h"
#include "net/tools/balsa/string_piece_utils.h"

namespace net {


class BalsaBuffer {
 public:
  static const size_t kDefaultBlocksize = 4096;
  
  
  
  
  
  friend class BalsaBufferTestSpouse;
  friend class BalsaHeaders;
  friend class BalsaBufferTest;

  
  
  
  
  struct BufferBlock {
   public:
    char* buffer;
    size_t buffer_size;
    size_t bytes_free;

    size_t bytes_used() const {
      return buffer_size - bytes_free;
    }
    char* start_of_unused_bytes() const {
      return buffer + bytes_used();
    }

    BufferBlock() : buffer(NULL), buffer_size(0), bytes_free(0) {}
    ~BufferBlock() {}

    BufferBlock(char* buf, size_t size, size_t free) :
        buffer(buf), buffer_size(size), bytes_free(free) {}
    
    
    
    
  };

  typedef std::vector<BufferBlock> Blocks;

  ~BalsaBuffer();

  
  size_t GetTotalBufferBlockSize() const;

  const char* GetPtr(Blocks::size_type block_idx) const {
    DCHECK_LT(block_idx, blocks_.size())
      << block_idx << ", " << blocks_.size();
    return blocks_[block_idx].buffer;
  }

  char* GetPtr(Blocks::size_type block_idx) {
    DCHECK_LT(block_idx, blocks_.size())
      << block_idx << ", " << blocks_.size();
    return blocks_[block_idx].buffer;
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void WriteToContiguousBuffer(const base::StringPiece& sp);

  void NoMoreWriteToContiguousBuffer() {
    can_write_to_contiguous_buffer_ = false;
  }

  
  
  
  
  
  
  base::StringPiece Write(const base::StringPiece& sp,
                          Blocks::size_type* block_buffer_idx);

  
  
  
  
  
  char* Reserve(size_t size, Blocks::size_type* block_buffer_idx);

  void Clear();

  void Swap(BalsaBuffer* b);

  void CopyFrom(const BalsaBuffer& b);

  const char* StartOfFirstBlock() const {
    return blocks_[0].buffer;
  }

  const char* EndOfFirstBlock() const {
    return blocks_[0].buffer + blocks_[0].bytes_used();
  }

  bool can_write_to_contiguous_buffer() const {
    return can_write_to_contiguous_buffer_;
  }
  size_t blocksize() const { return blocksize_; }
  Blocks::size_type num_blocks() const { return blocks_.size(); }
  size_t buffer_size(size_t idx) const { return blocks_[idx].buffer_size; }
  size_t bytes_used(size_t idx) const { return blocks_[idx].bytes_used(); }

 protected:
  BalsaBuffer();

  explicit BalsaBuffer(size_t blocksize);

  BufferBlock AllocBlock();

  BufferBlock AllocCustomBlock(size_t blocksize);

  BufferBlock CopyBlock(const BufferBlock& b);

  
  
  
  void CleanupBlocksStartingFrom(Blocks::size_type start_idx);

  
  Blocks blocks_;

  
  
  
  size_t blocksize_;

  
  
  
  
  
  bool can_write_to_contiguous_buffer_;
};



class BalsaHeaders {
 public:
  struct HeaderLineDescription {
    HeaderLineDescription(size_t first_character_index,
                          size_t key_end_index,
                          size_t value_begin_index,
                          size_t last_character_index,
                          size_t buffer_base_index) :
        first_char_idx(first_character_index),
        key_end_idx(key_end_index),
        value_begin_idx(value_begin_index),
        last_char_idx(last_character_index),
        buffer_base_idx(buffer_base_index),
        skip(false) {}

    HeaderLineDescription() :
        first_char_idx(0),
        key_end_idx(0),
        value_begin_idx(0),
        last_char_idx(0),
        buffer_base_idx(0),
        skip(false) {}

    size_t first_char_idx;
    size_t key_end_idx;
    size_t value_begin_idx;
    size_t last_char_idx;
    BalsaBuffer::Blocks::size_type buffer_base_idx;
    bool skip;
  };

  typedef std::vector<base::StringPiece> HeaderTokenList;
  friend bool ParseHTTPFirstLine(const char* begin,
                                 const char* end,
                                 bool is_request,
                                 size_t max_request_uri_length,
                                 BalsaHeaders* headers,
                                 BalsaFrameEnums::ErrorCode* error_code);

 protected:
  typedef std::vector<HeaderLineDescription> HeaderLines;

  
  
  
  
  
  
  
  
  

  class iterator_base {
   public:
    friend class BalsaHeaders;
    friend class reverse_iterator_base;
    typedef std::pair<base::StringPiece, base::StringPiece> StringPiecePair;
    typedef StringPiecePair value_type;
    typedef value_type& reference;
    typedef value_type* pointer;

    typedef std::forward_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;

    typedef iterator_base self;

    
    iterator_base();

    
    iterator_base(const iterator_base& it);

    reference operator*() const {
      return Lookup(idx_);
    }

    pointer operator->() const {
      return &(this->operator*());
    }

    bool operator==(const self& it) const {
      return idx_ == it.idx_;
    }

    bool operator<(const self& it) const {
      return idx_ < it.idx_;
    }

    bool operator<=(const self& it) const {
      return idx_ <= it.idx_;
    }

    bool operator!=(const self& it) const {
      return !(*this == it);
    }

    bool operator>(const self& it) const {
      return it < *this;
    }

    bool operator>=(const self& it) const {
      return it <= *this;
    }

    
    
    
    
    
    std::ostream& operator<<(std::ostream& os) const;

   protected:
    iterator_base(const BalsaHeaders* headers, HeaderLines::size_type index);

    void increment() {
      const HeaderLines& header_lines = headers_->header_lines_;
      const HeaderLines::size_type header_lines_size = header_lines.size();
      const HeaderLines::size_type original_idx = idx_;
      do {
        ++idx_;
      } while (idx_ < header_lines_size && header_lines[idx_].skip == true);
      
      
      
      
      
      
      if (idx_ == header_lines_size) {
        idx_ = original_idx + 1;
      }
    }

    void decrement() {
      const HeaderLines& header_lines = headers_->header_lines_;
      const HeaderLines::size_type header_lines_size = header_lines.size();
      const HeaderLines::size_type original_idx = idx_;
      do {
        --idx_;
      } while (idx_ < header_lines_size && header_lines[idx_].skip == true);
      
      
      
      
      
      
      if (idx_ > header_lines_size) {
        idx_ = original_idx - 1;
      }
    }

    reference Lookup(HeaderLines::size_type index) const {
      DCHECK_LT(index, headers_->header_lines_.size());
      const HeaderLineDescription& line = headers_->header_lines_[index];
      const char* stream_begin = headers_->GetPtr(line.buffer_base_idx);
      value_ = value_type(
          base::StringPiece(stream_begin + line.first_char_idx,
                      line.key_end_idx - line.first_char_idx),
          base::StringPiece(stream_begin + line.value_begin_idx,
                      line.last_char_idx - line.value_begin_idx));
      DCHECK_GE(line.key_end_idx, line.first_char_idx);
      DCHECK_GE(line.last_char_idx, line.value_begin_idx);
      return value_;
    }

    const BalsaHeaders* headers_;
    HeaderLines::size_type idx_;
    mutable StringPiecePair value_;
  };

  class reverse_iterator_base : public iterator_base {
   public:
    typedef reverse_iterator_base self;
    typedef iterator_base::reference reference;
    typedef iterator_base::pointer pointer;
    using iterator_base::headers_;
    using iterator_base::idx_;

    reverse_iterator_base() : iterator_base() {}

    
    reverse_iterator_base(const iterator_base& it) :  
        iterator_base(it) {
    }

    self& operator=(const iterator_base& it) {
      idx_ = it.idx_;
      headers_ = it.headers_;
      return *this;
    }

    self& operator=(const reverse_iterator_base& it) {
      idx_ = it.idx_;
      headers_ = it.headers_;
      return *this;
    }

    reference operator*() const {
      return Lookup(idx_ - 1);
    }

    pointer operator->() const {
      return &(this->operator*());
    }

    reverse_iterator_base(const reverse_iterator_base& it) :
        iterator_base(it) { }

   protected:
    void increment() {
      --idx_;
      iterator_base::decrement();
      ++idx_;
    }

    void decrement() {
      ++idx_;
      iterator_base::increment();
      --idx_;
    }

    reverse_iterator_base(const BalsaHeaders* headers,
                          HeaderLines::size_type index) :
        iterator_base(headers, index) {}
  };

 public:
  class const_header_lines_iterator : public iterator_base {
    friend class BalsaHeaders;
   public:
    typedef const_header_lines_iterator self;
    const_header_lines_iterator() : iterator_base() {}

    const_header_lines_iterator(const const_header_lines_iterator& it) :
        iterator_base(it.headers_, it.idx_) {}

    self& operator++() {
      iterator_base::increment();
      return *this;
    }

    self& operator--() {
      iterator_base::decrement();
      return *this;
    }
   protected:
    const_header_lines_iterator(const BalsaHeaders* headers,
                                HeaderLines::size_type index) :
        iterator_base(headers, index) {}
  };

  class const_reverse_header_lines_iterator : public reverse_iterator_base {
   public:
    typedef const_reverse_header_lines_iterator self;
    const_reverse_header_lines_iterator() : reverse_iterator_base() {}

    const_reverse_header_lines_iterator(
      const const_header_lines_iterator& it) :
        reverse_iterator_base(it.headers_, it.idx_) {}

    const_reverse_header_lines_iterator(
      const const_reverse_header_lines_iterator& it) :
        reverse_iterator_base(it.headers_, it.idx_) {}

    const_header_lines_iterator base() {
      return const_header_lines_iterator(headers_, idx_);
    }

    self& operator++() {
      reverse_iterator_base::increment();
      return *this;
    }

    self& operator--() {
      reverse_iterator_base::decrement();
      return *this;
    }
   protected:
    const_reverse_header_lines_iterator(const BalsaHeaders* headers,
                                        HeaderLines::size_type index) :
        reverse_iterator_base(headers, index) {}

    friend class BalsaHeaders;
  };

  
  
  
  
  
  class const_header_lines_key_iterator : public iterator_base {
    friend class BalsaHeaders;
   public:
    typedef const_header_lines_key_iterator self;
    const_header_lines_key_iterator(const const_header_lines_key_iterator&);

    self& operator++() {
      do {
        iterator_base::increment();
      } while (!AtEnd() &&
               !StringPieceUtils::EqualIgnoreCase(key_, (**this).first));
      return *this;
    }

    void operator++(int ignore) {
      ++(*this);
    }

    

   private:
    const_header_lines_key_iterator(const BalsaHeaders* headers,
                                    HeaderLines::size_type index,
                                    const base::StringPiece& key);

    
    const_header_lines_key_iterator(const BalsaHeaders* headers,
                                    HeaderLines::size_type index);

    bool AtEnd() const {
      return *this >= headers_->header_lines_end();
    }

    base::StringPiece key_;
  };

  
  
  
  BalsaHeaders();
  ~BalsaHeaders();

  const_header_lines_iterator header_lines_begin() {
    return HeaderLinesBeginHelper<const_header_lines_iterator>();
  }

  const_header_lines_iterator header_lines_begin() const {
    return HeaderLinesBeginHelper<const_header_lines_iterator>();
  }

  const_header_lines_iterator header_lines_end() {
    return HeaderLinesEndHelper<const_header_lines_iterator>();
  }

  const_header_lines_iterator header_lines_end() const {
    return HeaderLinesEndHelper<const_header_lines_iterator>();
  }

  const_reverse_header_lines_iterator header_lines_rbegin() {
    return const_reverse_header_lines_iterator(header_lines_end());
  }

  const_reverse_header_lines_iterator header_lines_rbegin() const {
    return const_reverse_header_lines_iterator(header_lines_end());
  }

  const_reverse_header_lines_iterator header_lines_rend() {
    return const_reverse_header_lines_iterator(header_lines_begin());
  }

  const_reverse_header_lines_iterator header_lines_rend() const {
    return const_reverse_header_lines_iterator(header_lines_begin());
  }

  const_header_lines_key_iterator header_lines_key_end() const {
    return HeaderLinesEndHelper<const_header_lines_key_iterator>();
  }

  void erase(const const_header_lines_iterator& it) {
    DCHECK_EQ(it.headers_, this);
    DCHECK_LT(it.idx_, header_lines_.size());
    DCHECK_GE(it.idx_, 0u);
    header_lines_[it.idx_].skip = true;
  }

  void Clear();

  void Swap(BalsaHeaders* other);

  void CopyFrom(const BalsaHeaders& other);

  void HackHeader(const base::StringPiece& key, const base::StringPiece& value);

  
  
  
  
  
  
  
  
  
  void HackAppendToHeader(const base::StringPiece& key,
                          const base::StringPiece& value);

  
  
  
  
  
  
  void ReplaceOrAppendHeader(const base::StringPiece& key,
                             const base::StringPiece& value);

  
  
  
  
  
  
  
  
  void AppendHeader(const base::StringPiece& key,
                    const base::StringPiece& value);

  
  
  
  
  void AppendToHeader(const base::StringPiece& key,
                      const base::StringPiece& value);

  
  
  
  
  void PrependToHeader(const base::StringPiece& key,
                       const base::StringPiece& value);

  const base::StringPiece GetHeader(const base::StringPiece& key) const;

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  void GetAllOfHeader(const base::StringPiece& key,
                      std::vector<base::StringPiece>* out) const;

  
  
  
  void GetAllOfHeaderAsString(const base::StringPiece& key,
                              std::string* out) const;

  
  
  static bool IsMultivaluedHeader(const base::StringPiece& header);

  
  inline bool HasHeader(const base::StringPiece& key) const {
    return (GetConstHeaderLinesIterator(key, header_lines_.begin()) !=
            header_lines_.end());
  }

  
  bool HasNonEmptyHeader(const base::StringPiece& key) const;

  const_header_lines_iterator GetHeaderPosition(
      const base::StringPiece& key) const;

  
  
  
  
  
  const_header_lines_key_iterator GetIteratorForKey(
      const base::StringPiece& key) const;

  void RemoveAllOfHeader(const base::StringPiece& key);

  
  void RemoveAllHeadersWithPrefix(const base::StringPiece& key);

  
  
  
  
  size_t GetMemoryUsedLowerBound() const;

  
  
  
  size_t GetSizeForWriteBuffer() const;

  
  
  
  
  
  
  
  

  
  
  // data should be written to the buffer.
  template <typename Buffer>
  void WriteHeaderAndEndingToBuffer(Buffer* buffer) const {
    WriteToBuffer(buffer);
    WriteHeaderEndingToBuffer(buffer);
  }

  
  // After this method returns, no more header data should be written to the
  
  template <typename Buffer>
  static void WriteHeaderEndingToBuffer(Buffer* buffer) {
    buffer->Write("\r\n", 2);
  }

  
  
  
  
  
  template <typename Buffer>
  void WriteToBuffer(Buffer* buffer) const {
    
    const size_t firstline_len = whitespace_4_idx_ - non_whitespace_1_idx_;
    const char* stream_begin = GetPtr(firstline_buffer_base_idx_);
    buffer->Write(stream_begin + non_whitespace_1_idx_, firstline_len);
    buffer->Write("\r\n", 2);
    const HeaderLines::size_type end = header_lines_.size();
    for (HeaderLines::size_type i = 0; i < end; ++i) {
      const HeaderLineDescription& line = header_lines_[i];
      if (line.skip) {
        continue;
      }
      const char* line_ptr = GetPtr(line.buffer_base_idx);
      WriteHeaderLineToBuffer(
          buffer,
          base::StringPiece(line_ptr + line.first_char_idx,
                      line.key_end_idx - line.first_char_idx),
          base::StringPiece(line_ptr + line.value_begin_idx,
                      line.last_char_idx - line.value_begin_idx));
    }
  }

  
  
  
  
  
  
  template <typename Buffer>
  static void WriteHeaderLineToBuffer(Buffer* buffer,
                                      const base::StringPiece& key,
                                      const base::StringPiece& value) {
    
    
    if (!key.empty()) {
      buffer->Write(key.data(), key.size());
      buffer->Write(": ", 2);
      buffer->Write(value.data(), value.size());
      buffer->Write("\r\n", 2);
    }
  }

  
  
  
  
  
  
  
  void DumpToString(std::string* str) const;

  const base::StringPiece first_line() const {
    DCHECK_GE(whitespace_4_idx_, non_whitespace_1_idx_);
    return base::StringPiece(BeginningOfFirstLine() + non_whitespace_1_idx_,
                       whitespace_4_idx_ - non_whitespace_1_idx_);
  }

  
  
  
  
  
  
  size_t parsed_response_code() const { return parsed_response_code_; }

  const base::StringPiece request_method() const {
    DCHECK_GE(whitespace_2_idx_, non_whitespace_1_idx_);
    return base::StringPiece(BeginningOfFirstLine() + non_whitespace_1_idx_,
                       whitespace_2_idx_ - non_whitespace_1_idx_);
  }

  const base::StringPiece response_version() const {
    
    
    
    
    return request_method();
  }

  const base::StringPiece request_uri() const {
    DCHECK_GE(whitespace_3_idx_, non_whitespace_2_idx_);
    return base::StringPiece(BeginningOfFirstLine() + non_whitespace_2_idx_,
                       whitespace_3_idx_ - non_whitespace_2_idx_);
  }

  const base::StringPiece response_code() const {
    
    
    
    return request_uri();
  }

  const base::StringPiece request_version() const {
    DCHECK_GE(whitespace_4_idx_, non_whitespace_3_idx_);
    return base::StringPiece(BeginningOfFirstLine() + non_whitespace_3_idx_,
                       whitespace_4_idx_ - non_whitespace_3_idx_);
  }

  const base::StringPiece response_reason_phrase() const {
    
    
    
    
    return request_version();
  }

  
  
  
  
  
  
  void SetFirstlineFromStringPieces(const base::StringPiece& firstline_a,
                                    const base::StringPiece& firstline_b,
                                    const base::StringPiece& firstline_c);

  void SetRequestFirstlineFromStringPieces(const base::StringPiece& method,
                                           const base::StringPiece& uri,
                                           const base::StringPiece& version) {
    SetFirstlineFromStringPieces(method, uri, version);
  }

  void SetResponseFirstlineFromStringPieces(
      const base::StringPiece& version,
      const base::StringPiece& code,
      const base::StringPiece& reason_phrase) {
    SetFirstlineFromStringPieces(version, code, reason_phrase);
  }

  
  
  
  void SetRequestMethod(const base::StringPiece& method);
  void SetResponseVersion(const base::StringPiece& version);

  void SetRequestUri(const base::StringPiece& uri);
  void SetResponseCode(const base::StringPiece& code);
  void set_parsed_response_code(size_t parsed_response_code) {
    parsed_response_code_ = parsed_response_code;
  }
  void SetParsedResponseCodeAndUpdateFirstline(size_t parsed_response_code);

  
  
  
  void SetRequestVersion(const base::StringPiece& version);
  void SetResponseReasonPhrase(const base::StringPiece& reason_phrase);

  
  
  
  
  
  
  
  
  
  void SetFirstLine(const base::StringPiece& line);

  
  bool transfer_encoding_is_chunked() const {
    return transfer_encoding_is_chunked_;
  }

  static bool ResponseCodeImpliesNoBody(int code) {
    
    
    return ((code >= 100) && (code <= 199)) || (code == 204) || (code == 304);
  }

  
  
  
  bool is_framed_by_connection_close() const {
    
    
    
    
    
    return (content_length_status_ == BalsaHeadersEnums::NO_CONTENT_LENGTH) &&
        !transfer_encoding_is_chunked_ &&
        !ResponseCodeImpliesNoBody(parsed_response_code_);
  }

  size_t content_length() const { return content_length_; }
  BalsaHeadersEnums::ContentLengthStatus content_length_status() const {
    return content_length_status_;
  }

  
  
  
  
  void SetContentLength(size_t length);
  void SetChunkEncoding(bool chunk_encode);

 protected:
  friend class BalsaFrame;
  friend class SpdyFrame;
  friend class HTTPMessage;
  friend class BalsaHeadersTokenUtils;

  const char* BeginningOfFirstLine() const {
    return GetPtr(firstline_buffer_base_idx_);
  }

  char* GetPtr(BalsaBuffer::Blocks::size_type block_idx) {
    return balsa_buffer_.GetPtr(block_idx);
  }

  const char* GetPtr(BalsaBuffer::Blocks::size_type block_idx) const {
    return balsa_buffer_.GetPtr(block_idx);
  }

  void WriteFromFramer(const char* ptr, size_t size) {
    balsa_buffer_.WriteToContiguousBuffer(base::StringPiece(ptr, size));
  }

  void DoneWritingFromFramer() {
    balsa_buffer_.NoMoreWriteToContiguousBuffer();
  }

  const char* OriginalHeaderStreamBegin() const {
    return balsa_buffer_.StartOfFirstBlock();
  }

  const char* OriginalHeaderStreamEnd() const {
    return balsa_buffer_.EndOfFirstBlock();
  }

  size_t GetReadableBytesFromHeaderStream() const {
    return OriginalHeaderStreamEnd() - OriginalHeaderStreamBegin();
  }

  void GetReadablePtrFromHeaderStream(const char** p, size_t* s) {
    *p = OriginalHeaderStreamBegin();
    *s = GetReadableBytesFromHeaderStream();
  }

  base::StringPiece GetValueFromHeaderLineDescription(
      const HeaderLineDescription& line) const;

  void AddAndMakeDescription(const base::StringPiece& key,
                             const base::StringPiece& value,
                             HeaderLineDescription* d);

  void AppendOrPrependAndMakeDescription(const base::StringPiece& key,
                                         const base::StringPiece& value,
                                         bool append,
                                         HeaderLineDescription* d);

  
  void RemoveAllOfHeaderStartingAt(const base::StringPiece& key,
                                   HeaderLines::iterator start);

  
  
  
  
  void AppendOrPrependToHeader(const base::StringPiece& key,
                               const base::StringPiece& value,
                               bool append);

  HeaderLines::const_iterator GetConstHeaderLinesIterator(
      const base::StringPiece& key,
      HeaderLines::const_iterator start) const;

  HeaderLines::iterator GetHeaderLinesIteratorNoSkip(
      const base::StringPiece& key,
      HeaderLines::iterator start);

  HeaderLines::iterator GetHeaderLinesIterator(
      const base::StringPiece& key,
      HeaderLines::iterator start);

  template <typename IteratorType>
  const IteratorType HeaderLinesBeginHelper() const {
    if (header_lines_.empty()) {
      return IteratorType(this, 0);
    }
    const HeaderLines::size_type header_lines_size = header_lines_.size();
    for (HeaderLines::size_type i = 0; i < header_lines_size; ++i) {
      if (header_lines_[i].skip == false) {
        return IteratorType(this, i);
      }
    }
    return IteratorType(this, 0);
  }

  template <typename IteratorType>
  const IteratorType HeaderLinesEndHelper() const {
    if (header_lines_.empty()) {
      return IteratorType(this, 0);
    }
    const HeaderLines::size_type header_lines_size = header_lines_.size();
    HeaderLines::size_type i = header_lines_size;
    do {
      --i;
      if (header_lines_[i].skip == false) {
        return IteratorType(this, i + 1);
      }
    } while (i != 0);
    return IteratorType(this, 0);
  }

  
  
  
  
  
  
  base::StringPiece OriginalHeadersForDebugging() const {
    return base::StringPiece(OriginalHeaderStreamBegin(),
                       OriginalHeaderStreamEnd() - OriginalHeaderStreamBegin());
  }

  BalsaBuffer balsa_buffer_;

  size_t content_length_;
  BalsaHeadersEnums::ContentLengthStatus content_length_status_;
  size_t parsed_response_code_;
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  BalsaBuffer::Blocks::size_type firstline_buffer_base_idx_;
  size_t whitespace_1_idx_;
  size_t non_whitespace_1_idx_;
  size_t whitespace_2_idx_;
  size_t non_whitespace_2_idx_;
  size_t whitespace_3_idx_;
  size_t non_whitespace_3_idx_;
  size_t whitespace_4_idx_;
  size_t end_of_firstline_idx_;

  bool transfer_encoding_is_chunked_;

  HeaderLines header_lines_;
};

}  

#endif  
