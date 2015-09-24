// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_GENERATED_ENUM_REFLECTION_H__
#define GOOGLE_PROTOBUF_GENERATED_ENUM_REFLECTION_H__

#include <string>

namespace google {
namespace protobuf {
  class EnumDescriptor;
}  

namespace protobuf {

template <typename E>
const EnumDescriptor* GetEnumDescriptor();

namespace internal {

LIBPROTOBUF_EXPORT bool ParseNamedEnum(const EnumDescriptor* descriptor,
                    const string& name,
                    int* value);

template<typename EnumType>
bool ParseNamedEnum(const EnumDescriptor* descriptor,
                    const string& name,
                    EnumType* value) {
  int tmp;
  if (!ParseNamedEnum(descriptor, name, &tmp)) return false;
  *value = static_cast<EnumType>(tmp);
  return true;
}

LIBPROTOBUF_EXPORT const string& NameOfEnum(const EnumDescriptor* descriptor, int value);

}  
}  

}  
#endif  
