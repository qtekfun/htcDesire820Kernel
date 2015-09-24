// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef MOJO_PUBLIC_BINDINGS_LIB_BINDINGS_SERIALIZATION_H_
#define MOJO_PUBLIC_BINDINGS_LIB_BINDINGS_SERIALIZATION_H_

#include <string.h>

#include <vector>

#include "mojo/public/bindings/lib/bindings.h"
#include "mojo/public/bindings/lib/message.h"

namespace mojo {
namespace internal {

size_t Align(size_t size);

void EncodePointer(const void* ptr, uint64_t* offset);
const void* DecodePointerRaw(const uint64_t* offset);

template <typename T>
inline void DecodePointer(const uint64_t* offset, T** ptr) {
  *ptr = reinterpret_cast<T*>(const_cast<void*>(DecodePointerRaw(offset)));
}

bool ValidatePointer(const void* ptr, const Message& message);

void EncodeHandle(Handle* handle, std::vector<Handle>* handles);
bool DecodeHandle(Handle* handle, std::vector<Handle>* handles);


template <typename T>
inline size_t ComputeSizeOf(const T* obj) {
  return obj ? ObjectTraits<T>::ComputeSizeOf(obj) : 0;
}

template <typename T>
inline T* Clone(const T* obj, Buffer* buf) {
  return obj ? ObjectTraits<T>::Clone(obj, buf) : NULL;
}

template <typename T>
inline void CloseHandles(T* obj) {
  if (obj)
    ObjectTraits<T>::CloseHandles(obj);
}

template <typename T>
inline void EncodePointersAndHandles(T* obj,
                                     std::vector<Handle>* handles) {
  ObjectTraits<T>::EncodePointersAndHandles(obj, handles);
}

template <typename T>
inline bool DecodePointersAndHandles(T* obj, Message* message) {
  return ObjectTraits<T>::DecodePointersAndHandles(obj, message);
}


template <typename T>
inline void Encode(T* obj, std::vector<Handle>* handles) {
  if (obj->ptr)
    EncodePointersAndHandles(obj->ptr, handles);
  EncodePointer(obj->ptr, &obj->offset);
}

template <typename T>
inline bool Decode(T* obj, Message* message) {
  DecodePointer(&obj->offset, &obj->ptr);
  if (obj->ptr) {
    if (!ValidatePointer(obj->ptr, *message))
      return false;
    if (!DecodePointersAndHandles(obj->ptr, message))
      return false;
  }
  return true;
}


template <typename T>
struct ArrayHelper {
  typedef T ElementType;

  static size_t ComputeSizeOfElements(const ArrayHeader* header,
                                      const ElementType* elements) {
    return 0;
  }

  static void CloneElements(const ArrayHeader* header,
                            ElementType* elements,
                            Buffer* buf) {
  }

  static void EncodePointersAndHandles(const ArrayHeader* header,
                                       ElementType* elements,
                                       std::vector<Handle>* handles) {
  }
  static bool DecodePointersAndHandles(const ArrayHeader* header,
                                       ElementType* elements,
                                       Message* message) {
    return true;
  }
};

template <>
struct ArrayHelper<Handle> {
  typedef Handle ElementType;

  static size_t ComputeSizeOfElements(const ArrayHeader* header,
                                      const ElementType* elements) {
    return 0;
  }

  static void CloneElements(const ArrayHeader* header,
                            ElementType* elements,
                            Buffer* buf) {
  }

  static void EncodePointersAndHandles(const ArrayHeader* header,
                                       ElementType* elements,
                                       std::vector<Handle>* handles);
  static bool DecodePointersAndHandles(const ArrayHeader* header,
                                       ElementType* elements,
                                       Message* message);
};

template <typename P>
struct ArrayHelper<P*> {
  typedef StructPointer<P> ElementType;

  static size_t ComputeSizeOfElements(const ArrayHeader* header,
                                      const ElementType* elements) {
    size_t result = 0;
    for (uint32_t i = 0; i < header->num_elements; ++i)
      result += ComputeSizeOf(elements[i].ptr);
    return result;
  }

  static void CloneElements(const ArrayHeader* header,
                            ElementType* elements,
                            Buffer* buf) {
    for (uint32_t i = 0; i < header->num_elements; ++i)
      elements[i].ptr = Clone(elements[i].ptr, buf);
  }

  static void EncodePointersAndHandles(const ArrayHeader* header,
                                       ElementType* elements,
                                       std::vector<Handle>* handles) {
    for (uint32_t i = 0; i < header->num_elements; ++i)
      Encode(&elements[i], handles);
  }
  static bool DecodePointersAndHandles(const ArrayHeader* header,
                                       ElementType* elements,
                                       Message* message) {
    for (uint32_t i = 0; i < header->num_elements; ++i) {
      if (!Decode(&elements[i], message))
        return false;
    }
    return true;
  }
};

template <typename T>
class ObjectTraits<Array_Data<T> > {
 public:
  static size_t ComputeSizeOf(const Array_Data<T>* array) {
    return Align(array->header_.num_bytes) +
        ArrayHelper<T>::ComputeSizeOfElements(&array->header_,
                                              array->storage());
  }

  static Array_Data<T>* Clone(const Array_Data<T>* array, Buffer* buf) {
    Array_Data<T>* clone = Array_Data<T>::New(array->header_.num_elements, buf);
    memcpy(clone->storage(),
           array->storage(),
           array->header_.num_bytes - sizeof(Array_Data<T>));

    ArrayHelper<T>::CloneElements(&clone->header_, clone->storage(), buf);
    return clone;
  }

  static void CloseHandles(Array_Data<T>* array) {
    
  }

  static void EncodePointersAndHandles(Array_Data<T>* array,
                                       std::vector<Handle>* handles) {
    ArrayHelper<T>::EncodePointersAndHandles(&array->header_, array->storage(),
                                             handles);
  }

  static bool DecodePointersAndHandles(Array_Data<T>* array,
                                       Message* message) {
    return ArrayHelper<T>::DecodePointersAndHandles(&array->header_,
                                                    array->storage(),
                                                    message);
  }
};

}  
}  

#endif  
