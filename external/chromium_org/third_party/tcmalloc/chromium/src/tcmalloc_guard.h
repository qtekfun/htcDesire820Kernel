// Copyright (c) 2005, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_TCMALLOC_GUARD_H_
#define TCMALLOC_TCMALLOC_GUARD_H_

class TCMallocGuard {
 public:
  TCMallocGuard();
  ~TCMallocGuard();
};

#endif  
