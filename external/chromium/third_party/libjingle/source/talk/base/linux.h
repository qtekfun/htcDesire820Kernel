/*
 * libjingle
 * Copyright 2011, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef TALK_BASE_LINUX_H_
#define TALK_BASE_LINUX_H_

#ifdef LINUX
#include <string>
#include <map>
#include <vector>
#include "talk/base/stream.h"

namespace talk_base {



class ConfigParser {
 public:
  typedef std::map<std::string, std::string> SimpleMap;
  typedef std::vector<SimpleMap> MapVector;

  ConfigParser();
  virtual ~ConfigParser();

  virtual bool Open(const std::string& filename);
  virtual void Attach(StreamInterface* stream);
  virtual bool Parse(MapVector *key_val_pairs);
  virtual bool ParseSection(SimpleMap *key_val_pair);
  virtual bool ParseLine(std::string *key, std::string *value);

 private:
  scoped_ptr<StreamInterface> instream_;
};



class ProcCpuInfo {
 public:
  ProcCpuInfo();
  virtual ~ProcCpuInfo();

  
  
  virtual bool LoadFromSystem();

  
  virtual bool GetNumCpus(int *num);

  
  virtual bool GetNumPhysicalCpus(int *num);

  
  
  virtual bool GetCpuStringValue(int cpu_id, const std::string& key,
                                 std::string *result);

  
  
  virtual bool GetCpuIntValue(int cpu_id, const std::string& key,
                              int *result);

 private:
  ConfigParser::MapVector cpu_info_;
};

std::string ReadLinuxLsbRelease();

std::string ReadLinuxUname();

int ReadCpuMaxFreq();

}  

#endif  
#endif  
