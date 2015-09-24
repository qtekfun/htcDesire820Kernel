// Copyright 2001-2010 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.


#ifndef STRINGS_STRINGPIECE_H__
#define STRINGS_STRINGPIECE_H__

#include <string.h>
#include <cstddef>
#include <iosfwd>
#include <string>
#ifdef WIN32
#include <algorithm>
#endif

namespace re2 {

class StringPiece {
 private:
  const char*   ptr_;
  int           length_;

 public:
  
  
  
  StringPiece() : ptr_(NULL), length_(0) { }
  StringPiece(const char* str)
    : ptr_(str), length_((str == NULL) ? 0 : static_cast<int>(strlen(str))) { }
  StringPiece(const std::string& str)
    : ptr_(str.data()), length_(static_cast<int>(str.size())) { }
  StringPiece(const char* offset, int len) : ptr_(offset), length_(len) { }

  
  
  
  
  const char* data() const { return ptr_; }
  int size() const { return length_; }
  int length() const { return length_; }
  bool empty() const { return length_ == 0; }

  void clear() { ptr_ = NULL; length_ = 0; }
  void set(const char* data, int len) { ptr_ = data; length_ = len; }
  void set(const char* str) {
    ptr_ = str;
    if (str != NULL)
      length_ = static_cast<int>(strlen(str));
    else
      length_ = 0;
  }
  void set(const void* data, int len) {
    ptr_ = reinterpret_cast<const char*>(data);
    length_ = len;
  }

  char operator[](int i) const { return ptr_[i]; }

  void remove_prefix(int n) {
    ptr_ += n;
    length_ -= n;
  }

  void remove_suffix(int n) {
    length_ -= n;
  }

  int compare(const StringPiece& x) const {
    int r = memcmp(ptr_, x.ptr_, std::min(length_, x.length_));
    if (r == 0) {
      if (length_ < x.length_) r = -1;
      else if (length_ > x.length_) r = +1;
    }
    return r;
  }

  std::string as_string() const {
    return std::string(data(), size());
  }
  
  
  
  
  
  std::string ToString() const {
    return std::string(data(), size());
  }

  void CopyToString(std::string* target) const;
  void AppendToString(std::string* target) const;

  
  bool starts_with(const StringPiece& x) const {
    return ((length_ >= x.length_) &&
            (memcmp(ptr_, x.ptr_, x.length_) == 0));
  }

  
  bool ends_with(const StringPiece& x) const {
    return ((length_ >= x.length_) &&
            (memcmp(ptr_ + (length_-x.length_), x.ptr_, x.length_) == 0));
  }

  
  typedef char value_type;
  typedef const char* pointer;
  typedef const char& reference;
  typedef const char& const_reference;
  typedef size_t size_type;
  typedef ptrdiff_t difference_type;
  static const size_type npos;
  typedef const char* const_iterator;
  typedef const char* iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  iterator begin() const { return ptr_; }
  iterator end() const { return ptr_ + length_; }
  const_reverse_iterator rbegin() const {
    return const_reverse_iterator(ptr_ + length_);
  }
  const_reverse_iterator rend() const {
    return const_reverse_iterator(ptr_);
  }
  
  int max_size() const { return length_; }
  int capacity() const { return length_; }

  int copy(char* buf, size_type n, size_type pos = 0) const;

  int find(const StringPiece& s, size_type pos = 0) const;
  int find(char c, size_type pos = 0) const;
  int rfind(const StringPiece& s, size_type pos = npos) const;
  int rfind(char c, size_type pos = npos) const;

  StringPiece substr(size_type pos, size_type n = npos) const;
  
  static bool _equal(const StringPiece&, const StringPiece&);
};

inline bool operator==(const StringPiece& x, const StringPiece& y) {
  return StringPiece::_equal(x, y);
}

inline bool operator!=(const StringPiece& x, const StringPiece& y) {
  return !(x == y);
}

inline bool operator<(const StringPiece& x, const StringPiece& y) {
  const int r = memcmp(x.data(), y.data(),
                       std::min(x.size(), y.size()));
  return ((r < 0) || ((r == 0) && (x.size() < y.size())));
}

inline bool operator>(const StringPiece& x, const StringPiece& y) {
  return y < x;
}

inline bool operator<=(const StringPiece& x, const StringPiece& y) {
  return !(x > y);
}

inline bool operator>=(const StringPiece& x, const StringPiece& y) {
  return !(x < y);
}

}  

extern std::ostream& operator<<(std::ostream& o, const re2::StringPiece& piece);

#endif  
