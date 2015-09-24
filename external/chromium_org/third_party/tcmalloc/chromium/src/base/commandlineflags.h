// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT

#ifndef BASE_COMMANDLINEFLAGS_H_
#define BASE_COMMANDLINEFLAGS_H_

#include <config.h>
#include <string>
#include <string.h>               
#include <stdlib.h>               
#include "base/basictypes.h"

#if defined(__ANDROID__) || defined(ANDROID)
#include <sys/system_properties.h>
#endif

#define DECLARE_VARIABLE(type, name)                                          \
  namespace FLAG__namespace_do_not_use_directly_use_DECLARE_##type##_instead {  \
  extern PERFTOOLS_DLL_DECL type FLAGS_##name;                                \
  }                                                                           \
  using FLAG__namespace_do_not_use_directly_use_DECLARE_##type##_instead::FLAGS_##name

#define DEFINE_VARIABLE(type, name, value, meaning) \
  namespace FLAG__namespace_do_not_use_directly_use_DECLARE_##type##_instead {  \
  PERFTOOLS_DLL_DECL type FLAGS_##name(value);                                \
  char FLAGS_no##name;                                                        \
  }                                                                           \
  using FLAG__namespace_do_not_use_directly_use_DECLARE_##type##_instead::FLAGS_##name

#define DECLARE_bool(name) \
  DECLARE_VARIABLE(bool, name)
#define DEFINE_bool(name, value, meaning) \
  DEFINE_VARIABLE(bool, name, value, meaning)

#define DECLARE_int32(name) \
  DECLARE_VARIABLE(int32, name)
#define DEFINE_int32(name, value, meaning) \
  DEFINE_VARIABLE(int32, name, value, meaning)

#define DECLARE_int64(name) \
  DECLARE_VARIABLE(int64, name)
#define DEFINE_int64(name, value, meaning) \
  DEFINE_VARIABLE(int64, name, value, meaning)

#define DECLARE_uint64(name) \
  DECLARE_VARIABLE(uint64, name)
#define DEFINE_uint64(name, value, meaning) \
  DEFINE_VARIABLE(uint64, name, value, meaning)

#define DECLARE_double(name) \
  DECLARE_VARIABLE(double, name)
#define DEFINE_double(name, value, meaning) \
  DEFINE_VARIABLE(double, name, value, meaning)

#define DECLARE_string(name)                                          \
  namespace FLAG__namespace_do_not_use_directly_use_DECLARE_string_instead {  \
  extern std::string FLAGS_##name;                                                   \
  }                                                                           \
  using FLAG__namespace_do_not_use_directly_use_DECLARE_string_instead::FLAGS_##name
#define DEFINE_string(name, value, meaning) \
  namespace FLAG__namespace_do_not_use_directly_use_DECLARE_string_instead {  \
  std::string FLAGS_##name(value);                                                   \
  char FLAGS_no##name;                                                        \
  }                                                                           \
  using FLAG__namespace_do_not_use_directly_use_DECLARE_string_instead::FLAGS_##name


#if defined(__ANDROID__) || defined(ANDROID)

inline const char* const EnvToString(const char* envname, const char* dflt) {
  static char system_property_value[PROP_VALUE_MAX];
  if (__system_property_get(envname, system_property_value) > 0)
    return system_property_value;
  return dflt;
}

inline bool EnvToBool(const char* envname, bool dflt) {
  static const char kTrueValues[] = "tTyY1";
  char system_property_value[PROP_VALUE_MAX];
  if (__system_property_get(envname, system_property_value) > 0)
    return memchr(kTrueValues, system_property_value[0], sizeof(kTrueValues));
  return dflt;
}

inline int EnvToInt(const char* envname, int dflt) {
  char system_property_value[PROP_VALUE_MAX];
  if (__system_property_get(envname, system_property_value) > 0)
    return strtol(system_property_value, NULL, 10);
  return dflt;
}

inline int64 EnvToInt64(const char* envname, int64 dflt) {
  char system_property_value[PROP_VALUE_MAX];
  if (__system_property_get(envname, system_property_value) > 0)
    return strtoll(system_property_value, NULL, 10);
  return dflt;
}

inline double EnvToDouble(const char* envname, double dflt) {
  char system_property_value[PROP_VALUE_MAX];
  if (__system_property_get(envname, system_property_value) > 0)
    return strtod(system_property_value, NULL);
  return dflt;
}

#else  

#define EnvToString(envname, dflt)   \
  (!getenv(envname) ? (dflt) : getenv(envname))

#define EnvToBool(envname, dflt)   \
  (!getenv(envname) ? (dflt) : memchr("tTyY1\0", getenv(envname)[0], 6) != NULL)

#define EnvToInt(envname, dflt)  \
  (!getenv(envname) ? (dflt) : strtol(getenv(envname), NULL, 10))

#define EnvToInt64(envname, dflt)  \
  (!getenv(envname) ? (dflt) : strtoll(getenv(envname), NULL, 10))

#define EnvToDouble(envname, dflt)  \
  (!getenv(envname) ? (dflt) : strtod(getenv(envname), NULL))

#endif  

#endif  
