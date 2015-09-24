// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.



#ifndef BASE_TRACKED_H_
#define BASE_TRACKED_H_
#pragma once

#include <string>

#include "base/base_api.h"
#include "base/time.h"

#ifndef NDEBUG
#ifndef TRACK_ALL_TASK_OBJECTS
#define TRACK_ALL_TASK_OBJECTS
#endif   
#endif  

namespace tracked_objects {


class BASE_API Location {
 public:
  
  
  
  Location(const char* function_name, const char* file_name, int line_number);

  
  Location();

  
  
  
  
  
  
  bool operator < (const Location& other) const {
    if (line_number_ != other.line_number_)
      return line_number_ < other.line_number_;
    if (file_name_ != other.file_name_)
      return file_name_ < other.file_name_;
    return function_name_ < other.function_name_;
  }

  const char* function_name() const { return function_name_; }
  const char* file_name()     const { return file_name_; }
  int line_number()           const { return line_number_; }

  void Write(bool display_filename, bool display_function_name,
             std::string* output) const;

  
  void WriteFunctionName(std::string* output) const;

 private:
  const char* const function_name_;
  const char* const file_name_;
  const int line_number_;
};



#define FROM_HERE tracked_objects::Location(__FUNCTION__, __FILE__, __LINE__)




class Births;

class BASE_API Tracked {
 public:
  Tracked();
  virtual ~Tracked();

  
  void SetBirthPlace(const Location& from_here);
  const Location GetBirthPlace() const;

  
  
  
  void ResetBirthTime();

  bool MissingBirthplace() const;

#if defined(TRACK_ALL_TASK_OBJECTS)
  base::TimeTicks tracked_birth_time() const { return tracked_birth_time_; }
#else
  base::TimeTicks tracked_birth_time() const { return base::TimeTicks::Now(); }
#endif  

 private:
#if defined(TRACK_ALL_TASK_OBJECTS)

  
  
  Births* tracked_births_;
  
  
  
  base::TimeTicks tracked_birth_time_;

#endif  

  DISALLOW_COPY_AND_ASSIGN(Tracked);
};

}  

#endif  
