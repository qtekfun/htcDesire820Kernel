// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COURGETTE_COURGETTE_H_
#define COURGETTE_COURGETTE_H_

#include <stddef.h>   

#include "base/files/file_path.h"

namespace courgette {

enum Status {
  C_OK = 1,                       

  C_GENERAL_ERROR = 2,            

  C_READ_OPEN_ERROR = 3,          
  C_READ_ERROR = 4,               

  C_WRITE_OPEN_ERROR = 3,         
  C_WRITE_ERROR = 4,              

  C_BAD_ENSEMBLE_MAGIC = 5,       
  C_BAD_ENSEMBLE_VERSION = 6,     
  C_BAD_ENSEMBLE_HEADER = 7,      
  C_BAD_ENSEMBLE_CRC = 8,         

  C_BAD_TRANSFORM = 12,           
  C_BAD_BASE = 13,                

  C_BINARY_DIFF_CRC_ERROR = 14,   
                                  

  
  C_STREAM_ERROR = 20,            
  C_STREAM_NOT_CONSUMED = 21,     
                                  
  C_SERIALIZATION_FAILED = 22,    
  C_DESERIALIZATION_FAILED = 23,  
  C_INPUT_NOT_RECOGNIZED = 24,    
  C_DISASSEMBLY_FAILED = 25,      
  C_ASSEMBLY_FAILED = 26,         
  C_ADJUSTMENT_FAILED = 27,       
  C_TRIM_FAILED = 28,             
};

enum ExecutableType {
  EXE_UNKNOWN = 0,
  EXE_WIN_32_X86 = 1,
  EXE_ELF_32_X86 = 2,
  EXE_ELF_32_ARM = 3,
  EXE_WIN_32_X64 = 4,
};

class SinkStream;
class SinkStreamSet;
class SourceStream;
class SourceStreamSet;

class AssemblyProgram;
class EncodedProgram;

Status ApplyEnsemblePatch(SourceStream* old, SourceStream* patch,
                          SinkStream* output);

Status ApplyEnsemblePatch(const base::FilePath::CharType* old_file_name,
                          const base::FilePath::CharType* patch_file_name,
                          const base::FilePath::CharType* new_file_name);

Status GenerateEnsemblePatch(SourceStream* old, SourceStream* target,
                             SinkStream* patch);

Status DetectExecutableType(const void* buffer, size_t length,
                            ExecutableType* type,
                            size_t* detected_length);

Status ParseDetectedExecutable(const void* buffer, size_t length,
                               AssemblyProgram** output);

Status TrimLabels(AssemblyProgram* program);

Status Encode(AssemblyProgram* program, EncodedProgram** output);

Status WriteEncodedProgram(EncodedProgram* encoded, SinkStreamSet* sink);

Status Assemble(EncodedProgram* encoded, SinkStream* buffer);

Status ReadEncodedProgram(SourceStreamSet* source, EncodedProgram** output);

void DeleteAssemblyProgram(AssemblyProgram* program);

void DeleteEncodedProgram(EncodedProgram* encoded);

Status Adjust(const AssemblyProgram& model, AssemblyProgram *program);

}  
#endif  
