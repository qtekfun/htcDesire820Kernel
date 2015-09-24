// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef BASE_COMMANDLINEFLAGS_H_
#define BASE_COMMANDLINEFLAGS_H_

#include <assert.h>
#include <string>
#include <vector>
#include "base/basictypes.h"
#include "base/port.h"
#include "base/stl_decl_msvc.h"
#include "base/global_strip_options.h"


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
bool RegisterFlagValidator(const string* flag,
                           bool (*validate_fn)(const char*, const string&));



struct CommandLineFlagInfo {
  string name;            
  string type;            
  string description;     
  string current_value;   
  string default_value;   
  string filename;        
  bool is_default;        
  bool has_validator_fn;  
};

extern void GetAllFlags(vector<CommandLineFlagInfo>* OUTPUT);
extern void ShowUsageWithFlags(const char *argv0);  
extern void ShowUsageWithFlagsRestrict(const char *argv0, const char *restrict);

extern string DescribeOneFlag(const CommandLineFlagInfo& flag);

extern void SetArgv(int argc, const char** argv);
extern const vector<string>& GetArgvs();    
extern const char* GetArgv();               
extern const char* GetArgv0();              
extern uint32 GetArgvSum();                 
extern const char* ProgramInvocationName(); 
extern const char* ProgramInvocationShortName();   
extern const char* ProgramUsage();          



extern bool GetCommandLineOption(const char* name, string* OUTPUT);

extern bool GetCommandLineFlagInfo(const char* name,
                                   CommandLineFlagInfo* OUTPUT);

extern CommandLineFlagInfo GetCommandLineFlagInfoOrDie(const char* name);

enum FlagSettingMode {
  
  SET_FLAGS_VALUE,
  
  
  SET_FLAG_IF_DEFAULT,
  
  
  
  SET_FLAGS_DEFAULT
};


extern string SetCommandLineOption(const char* name, const char* value);
extern string SetCommandLineOptionWithMode(const char* name, const char* value,
                                           FlagSettingMode set_mode);



extern string CommandlineFlagsIntoString();
extern bool ReadFlagsFromString(const string& flagfilecontents,
                                const char* prog_name,
                                bool errors_are_fatal); 

extern bool AppendFlagsIntoFile(const string& filename, const char* prog_name);
extern bool SaveCommandFlags();  
extern bool ReadFromFlagsFile(const string& filename, const char* prog_name,
                              bool errors_are_fatal);   



extern bool BoolFromEnv(const char *varname, bool defval);
extern int32 Int32FromEnv(const char *varname, int32 defval);
extern int64 Int64FromEnv(const char *varname, int64 defval);
extern uint64 Uint64FromEnv(const char *varname, uint64 defval);
extern double DoubleFromEnv(const char *varname, double defval);
extern const char *StringFromEnv(const char *varname, const char *defval);



extern void SetUsageMessage(const string& usage);

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

#ifndef SWIG  


extern const char kStrippedFlagHelp[];

#if STRIP_FLAG_HELP > 0
#define MAYBE_STRIPPED_HELP(txt) (false ? (txt) : kStrippedFlagHelp)
#else
#define MAYBE_STRIPPED_HELP(txt) txt
#endif

#define DEFINE_VARIABLE(type, shorttype, name, value, help) \
  namespace fL##shorttype {                                     \
    static const type FLAGS_nono##name = value;                 \
    type FLAGS_##name = FLAGS_nono##name;                       \
    type FLAGS_no##name = FLAGS_nono##name;                     \
    static FlagRegisterer o_##name(                             \
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
extern bool FlagsTypeWarn(const char *name);

#define DECLARE_bool(name)          DECLARE_VARIABLE(bool,B, name)
#define DEFINE_bool(name,val,txt)   namespace fLB { \
                                      const bool FLAGS_nonono##name = \
                                        (sizeof(::fLB::IsBoolFlag(val)) \
                                        == sizeof(double)) \
                                        ? FlagsTypeWarn(#name) : true; \
                                    } \
                                    DEFINE_VARIABLE(bool,B, name, val, txt)
#define DECLARE_int32(name)         DECLARE_VARIABLE(int32,I, name)
#define DEFINE_int32(name,val,txt)  DEFINE_VARIABLE(int32,I, name, val, txt)

#define DECLARE_int64(name)         DECLARE_VARIABLE(int64,I64, name)
#define DEFINE_int64(name,val,txt)  DEFINE_VARIABLE(int64,I64, name, val, txt)

#define DECLARE_uint64(name)        DECLARE_VARIABLE(uint64,U64, name)
#define DEFINE_uint64(name,val,txt) DEFINE_VARIABLE(uint64,U64, name, val, txt)

#define DECLARE_double(name)        DECLARE_VARIABLE(double,D, name)
#define DEFINE_double(name,val,txt) DEFINE_VARIABLE(double,D, name, val, txt)

#define DECLARE_string(name)  namespace fLS { extern string& FLAGS_##name; } \
                              using fLS::FLAGS_##name

#define DEFINE_string(name, val, txt)                                     \
  namespace fLS {                                                         \
    static union { void* align; char s[sizeof(string)]; } s_##name[2];    \
    const string* const FLAGS_no##name = new (s_##name[0].s) string(val); \
    static FlagRegisterer o_##name(                                       \
      #name, "string", MAYBE_STRIPPED_HELP(txt), __FILE__,                \
      s_##name[0].s, new (s_##name[1].s) string(*FLAGS_no##name));        \
    string& FLAGS_##name = *(reinterpret_cast<string*>(s_##name[0].s));   \
  }                                                                       \
  using fLS::FLAGS_##name

#endif  

#endif  
