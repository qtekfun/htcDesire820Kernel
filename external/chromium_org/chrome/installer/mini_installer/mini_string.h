// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_INSTALLER_MINI_INSTALLER_MINI_STRING_H_
#define CHROME_INSTALLER_MINI_INSTALLER_MINI_STRING_H_

#ifndef COMPILE_ASSERT
template <bool>
struct CompileAssert {};
#define COMPILE_ASSERT(expr, msg) \
    typedef CompileAssert<(bool(expr))> msg[bool(expr) ? 1 : -1]
#endif

namespace mini_installer {


bool HexEncode(const void* bytes, size_t size, wchar_t* str, size_t str_size);

size_t SafeStrLen(const wchar_t* str, size_t alloc_size);

bool SafeStrCopy(wchar_t* dest, size_t dest_size, const wchar_t* src);

bool SafeStrCat(wchar_t* dest, size_t dest_size, const wchar_t* src);

bool StrEndsWith(const wchar_t* str, const wchar_t* end_str);

bool StrStartsWith(const wchar_t* str, const wchar_t* start_str);

const wchar_t* SearchStringI(const wchar_t* source, const wchar_t* find);

bool FindTagInStr(const wchar_t* str, const wchar_t* tag,
                  const wchar_t** position);

wchar_t* GetNameFromPathExt(wchar_t* path, size_t size);

template <size_t kCapacity>
class StackString {
 public:
  StackString() {
    COMPILE_ASSERT(kCapacity != 0, invalid_buffer_size);
    buffer_[kCapacity] = L'\0';  
    clear();
  }

  
  

  
  
  size_t capacity() const {
    return kCapacity;
  }

  wchar_t* get() {
    return buffer_;
  }

  bool assign(const wchar_t* str) {
    return SafeStrCopy(buffer_, kCapacity, str);
  }

  bool append(const wchar_t* str) {
    return SafeStrCat(buffer_, kCapacity, str);
  }

  void clear() {
    buffer_[0] = L'\0';
  }

  size_t length() const {
    return SafeStrLen(buffer_, kCapacity);
  }

  
  const wchar_t* findi(const wchar_t* find) const {
    return SearchStringI(buffer_, find);
  }

  
  int comparei(const wchar_t* str) const {
    return lstrcmpiW(buffer_, str);
  }

  
  int compare(const wchar_t* str) const {
    return lstrcmpW(buffer_, str);
  }

  
  
  
  bool truncate_at(size_t location) {
    if (location >= kCapacity)
      return false;
    buffer_[location] = L'\0';
    return true;
  }

 protected:
  
  
  wchar_t buffer_[kCapacity + 1];

 private:
  StackString(const StackString&);
  StackString& operator=(const StackString&);
};

}  

#endif  
