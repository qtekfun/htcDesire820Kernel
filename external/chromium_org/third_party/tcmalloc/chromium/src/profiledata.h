// Copyright (c) 2007, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef BASE_PROFILEDATA_H_
#define BASE_PROFILEDATA_H_

#include <config.h>
#include <time.h>   
#include <stdint.h>
#include "base/basictypes.h"

class ProfileData {
 public:
  struct State {
    bool     enabled;             
    time_t   start_time;          
    char     profile_name[1024];  // Name of file being written, or '\0'
    int      samples_gathered;    
  };

  class Options {
   public:
    Options();

    
    int frequency() const {
      return frequency_;
    }
    void set_frequency(int frequency) {
      frequency_ = frequency;
    }

   private:
    int      frequency_;                  
  };

  static const int kMaxStackDepth = 64;  

  ProfileData();
  ~ProfileData();

  
  
  
  
  
  
  
  bool Start(const char *fname, const Options& options);

  
  
  void Stop();

  
  
  void Reset();

  
  
  
  
  
  
  
  void Add(int depth, const void* const* stack);

  
  
  void FlushTable();

  
  bool enabled() const { return out_ >= 0; }

  
  void GetCurrentState(State* state) const;

 private:
  static const int kAssociativity = 4;          
  static const int kBuckets = 1 << 10;          
  static const int kBufferLength = 1 << 18;     

  
  typedef uintptr_t Slot;

  
  struct Entry {
    Slot count;                  
    Slot depth;                  
    Slot stack[kMaxStackDepth];  
  };

  
  struct Bucket {
    Entry entry[kAssociativity];
  };

  Bucket*       hash_;          
  Slot*         evict_;         
  int           num_evicted_;   
  int           out_;           
  int           count_;         
  int           evictions_;     
  size_t        total_bytes_;   
  char*         fname_;         
  time_t        start_time_;    

  
  void Evict(const Entry& entry);

  
  void FlushEvicted();

  DISALLOW_COPY_AND_ASSIGN(ProfileData);
};

#endif  
