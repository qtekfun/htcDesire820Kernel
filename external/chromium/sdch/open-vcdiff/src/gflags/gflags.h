// Copyright (c) 2006, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_GFLAGS_H_
#define GOOGLE_GFLAGS_H_

#include "config.h"
#include <string>
#include <vector>

#if defined(HAVE_STDINT_H)
#include <stdint.h>             
#elif defined(HAVE_SYSTYPES_H)
#include <sys/types.h>          
#elif defined(HAVE_INTTYPES_H)
#include <inttypes.h>           
#endif

namespace google {

#if defined(HAVE_UINT16_T)  
typedef int32_t int32;
typedef uint32_t uint32;
typedef int64_t int64;
typedef uint64_t uint64;
#elif defined(HAVE_U_INT16_T)  
typedef int32_t int32;
typedef u_int32_t uint32;
typedef int64_t int64;
typedef u_int64_t uint64;
#elif defined(HAVE___INT16)  
typedef __int32 int32;
typedef unsigned __int32 uint32;
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
#error Do not know how to define a 32-bit integer quantity on your system
#endif


bool RegisterFlagValidator(const bool* flag,
                           bool (*validate_fn)(const char*, bool));
bool RegisterFlagValidator(const int32* flag,
                           bool (*validate_fn)(const char*, int32));
bool RegisterFlagValidator(const int64* flag,
                           bool (*validate_fn)(const char*, int64));
bool RegisterFlagValidator(const uint64* flag,
                           bool (*validate_fn)(const char*, uint64));
bool RegisterFlagValidator(const double* flag,
                           bool (*validate_fn)(const char*, double));
bool RegisterFlagValidator(const std::string* flag,
                           bool (*validate_fn)(const char*, const std::string&));



struct CommandLineFlagInfo {
  std::string name;           
  std::string type;           
  std::string description;    
  std::string current_value;  
  std::string default_value;  
  std::string filename;       
  bool has_validator_fn;      
  bool is_default;            
};

extern void GetAllFlags(std::vector<CommandLineFlagInfo>* OUTPUT);
extern void ShowUsageWithFlags(const char *argv0);  
extern void ShowUsageWithFlagsRestrict(const char *argv0, const char *restrict);

extern std::string DescribeOneFlag(const CommandLineFlagInfo& flag);

extern void SetArgv(int argc, const char** argv);
extern const std::vector<std::string>& GetArgvs();  
extern const char* GetArgv();               
extern const char* GetArgv0();              
extern uint32 GetArgvSum();                 
extern const char* ProgramInvocationName(); 
extern const char* ProgramInvocationShortName();   
extern const char* ProgramUsage();          



extern bool GetCommandLineOption(const char* name, std::string* OUTPUT);

extern bool GetCommandLineFlagInfo(const char* name,
                                   CommandLineFlagInfo* OUTPUT);

extern CommandLineFlagInfo GetCommandLineFlagInfoOrDie(const char* name);

enum FlagSettingMode {
  
  SET_FLAGS_VALUE,
  
  
  SET_FLAG_IF_DEFAULT,
  
  
  
  SET_FLAGS_DEFAULT
};


extern std::string SetCommandLineOption(const char* name, const char* value);
extern std::string SetCommandLineOptionWithMode(const char* name, const char* value,
                                                FlagSettingMode set_mode);



class FlagSaver {
 public:
  FlagSaver();
  ~FlagSaver();

 private:
  class FlagSaverImpl* impl_;   

  FlagSaver(const FlagSaver&);  
  void operator=(const FlagSaver&);
#if HAVE___ATTRIBUTE__
} __attribute__ ((unused));
#else  
};
#endif  


extern std::string CommandlineFlagsIntoString();
extern bool ReadFlagsFromString(const std::string& flagfilecontents,
                                const char* prog_name,
                                bool errors_are_fatal); 

extern bool AppendFlagsIntoFile(const std::string& filename, const char* prog_name);
extern bool SaveCommandFlags();  
extern bool ReadFromFlagsFile(const std::string& filename, const char* prog_name,
                              bool errors_are_fatal);   



extern bool BoolFromEnv(const char *varname, bool defval);
extern int32 Int32FromEnv(const char *varname, int32 defval);
extern int64 Int64FromEnv(const char *varname, int64 defval);
extern uint64 Uint64FromEnv(const char *varname, uint64 defval);
extern double DoubleFromEnv(const char *varname, double defval);
extern const char *StringFromEnv(const char *varname, const char *defval);



extern void SetUsageMessage(const std::string& usage);

#ifndef SWIG   
extern uint32 ParseCommandLineFlags(int *argc, char*** argv,
                                    bool remove_flags);
#endif


extern uint32 ParseCommandLineNonHelpFlags(int *argc, char*** argv,
                                           bool remove_flags);
extern void HandleCommandLineHelpFlags();   

extern void AllowCommandLineReparsing();

extern uint32 ReparseCommandLineNonHelpFlags();



class FlagRegisterer {
 public:
  FlagRegisterer(const char* name, const char* type,
                 const char* help, const char* filename,
                 void* current_storage, void* defvalue_storage);
};

extern bool FlagsTypeWarn(const char *name);


extern const char kStrippedFlagHelp[];

}  

#ifndef SWIG  

#if defined(STRIP_FLAG_HELP) && STRIP_FLAG_HELP > 0
#define MAYBE_STRIPPED_HELP(txt) (false ? (txt) : kStrippedFlagHelp)
#else
#define MAYBE_STRIPPED_HELP(txt) txt
#endif

#define DEFINE_VARIABLE(type, shorttype, name, value, help) \
  namespace fL##shorttype {                                     \
    static const type FLAGS_nono##name = value;                 \
    type FLAGS_##name = FLAGS_nono##name;                       \
    type FLAGS_no##name = FLAGS_nono##name;                     \
    static ::google::FlagRegisterer o_##name(      \
      #name, #type, MAYBE_STRIPPED_HELP(help), __FILE__,        \
      &FLAGS_##name, &FLAGS_no##name);                          \
  }                                                             \
  using fL##shorttype::FLAGS_##name

#define DECLARE_VARIABLE(type, shorttype, name) \
  namespace fL##shorttype {                     \
    extern type FLAGS_##name;                   \
  }                                             \
  using fL##shorttype::FLAGS_##name

namespace fLB {
template<typename From> double IsBoolFlag(const From& from);
bool IsBoolFlag(bool from);
}

#define DECLARE_bool(name)          DECLARE_VARIABLE(bool,B, name)
#define DEFINE_bool(name,val,txt)   namespace fLB { \
                                      const bool FLAGS_nonono##name = \
                                        (sizeof(::fLB::IsBoolFlag(val)) \
                                        == sizeof(double)) \
                                        ? ::google::FlagsTypeWarn(#name) : true; \
                                    } \
                                    DEFINE_VARIABLE(bool,B, name, val, txt)
#define DECLARE_int32(name)         DECLARE_VARIABLE(::google::int32,I, name)
#define DEFINE_int32(name,val,txt)  DEFINE_VARIABLE(::google::int32,I, name, val, txt)

#define DECLARE_int64(name)         DECLARE_VARIABLE(::google::int64,I64, name)
#define DEFINE_int64(name,val,txt)  DEFINE_VARIABLE(::google::int64,I64, name, val, txt)

#define DECLARE_uint64(name)        DECLARE_VARIABLE(::google::uint64,U64, name)
#define DEFINE_uint64(name,val,txt) DEFINE_VARIABLE(::google::uint64,U64, name, val, txt)

#define DECLARE_double(name)        DECLARE_VARIABLE(double,D, name)
#define DEFINE_double(name,val,txt) DEFINE_VARIABLE(double,D, name, val, txt)

#define DECLARE_string(name)  namespace fLS { extern std::string& FLAGS_##name; } \
                              using fLS::FLAGS_##name

#define DEFINE_string(name, val, txt)                                     \
  namespace fLS {                                                         \
    static union { void* align; char s[sizeof(std::string)]; } s_##name[2]; \
    const std::string* const FLAGS_no##name = new (s_##name[0].s) std::string(val); \
    static ::google::FlagRegisterer o_##name(                \
      #name, "string", MAYBE_STRIPPED_HELP(txt), __FILE__,                \
      s_##name[0].s, new (s_##name[1].s) std::string(*FLAGS_no##name));   \
    extern std::string& FLAGS_##name;                                     \
    using fLS::FLAGS_##name;                                              \
    std::string& FLAGS_##name = *(reinterpret_cast<std::string*>(s_##name[0].s));   \
  }                                                                       \
  using fLS::FLAGS_##name

#endif  

#endif  
