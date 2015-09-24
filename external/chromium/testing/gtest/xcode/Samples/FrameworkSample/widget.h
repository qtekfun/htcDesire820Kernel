// Copyright 2008, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#import <string>

class Widget {
 public:
  Widget(int number, const std::string& name);
  ~Widget();

  
  float GetFloatValue() const;
  int GetIntValue() const;

  
  std::string GetStringValue() const;
  void GetCharPtrValue(char* buffer, size_t max_size) const;

 private:
  
  float number_;
  std::string name_;
};
