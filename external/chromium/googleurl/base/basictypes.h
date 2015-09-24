// Copyright 2001 - 2003 Google Inc. All Rights Reserved

#ifndef BASE_BASICTYPES_H__
#define BASE_BASICTYPES_H__

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;

const uint8  kuint8max  = (( uint8) 0xFF);
const uint32 kuint32max = ((uint32) 0xFFFFFFFF);


template <typename T, size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

#ifndef _MSC_VER
template <typename T, size_t N>
char (&ArraySizeHelper(const T (&array)[N]))[N];
#endif

#define arraysize(array) (sizeof(ArraySizeHelper(array)))

#define ARRAYSIZE_UNSAFE(a) \
  ((sizeof(a) / sizeof(*(a))) / \
   static_cast<size_t>(!(sizeof(a) % sizeof(*(a)))))

#define DISALLOW_EVIL_CONSTRUCTORS(TypeName)    \
  TypeName(const TypeName&);                    \
  void operator=(const TypeName&)

#endif  
