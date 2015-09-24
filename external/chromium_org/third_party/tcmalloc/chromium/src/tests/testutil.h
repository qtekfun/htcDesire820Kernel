// Copyright (c) 2007, Google Inc.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef TCMALLOC_TOOLS_TESTUTIL_H_
#define TCMALLOC_TOOLS_TESTUTIL_H_

extern "C" void RunThread(void (*fn)());

extern "C" void RunManyThreads(void (*fn)(), int count);

extern "C" void RunManyThreadsWithId(void (*fn)(int), int count, int stacksize);

void SetTestResourceLimit();

#endif  
