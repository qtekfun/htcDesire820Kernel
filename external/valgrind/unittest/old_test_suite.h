/*
  This file is part of Valgrind, a dynamic binary instrumentation
  framework.

  Copyright (C) 2008-2008 Google Inc
     opensource@google.com

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License as
  published by the Free Software Foundation; either version 2 of the
  License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
  02111-1307, USA.

  The GNU General Public License is contained in the file COPYING.
*/

#ifndef OLD_TEST_SUITE_H__
#define OLD_TEST_SUITE_H__

#include <map>
#include <set>
#include <cstring>

#include "test_utils.h"

typedef void (*void_func_void_t)(void);

enum TEST_FLAG {
  FEATURE           = 1 << 0,
  STABILITY         = 1 << 1,
  PERFORMANCE       = 1 << 2,
  EXCLUDE_FROM_ALL  = 1 << 3,
  NEEDS_ANNOTATIONS = 1 << 4,
  RACE_DEMO         = 1 << 5,
  MEMORY_USAGE      = 1 << 6,
  PRINT_STATS       = 1 << 7
};

extern Mutex printf_mu;
#ifndef WIN32
#define printf(args...) \
    do{ \
      printf_mu.Lock();\
      fprintf(stderr, args);\
      printf_mu.Unlock(); \
    }while(0)
#endif

struct Test{
  void_func_void_t f_;
  int id_;
  int flags_;
  Test(void_func_void_t f, int id, int flags)
    : f_(f)
    , id_(id)
    , flags_(flags)
  {}
  Test() : f_(0), flags_(0) {}
  void Run() {
     if (flags_ & PERFORMANCE) {
        long start = GetTimeInMs();
        f_();
        long end = GetTimeInMs();
        printf("*RESULT test%d: time= %4ld ms\n", id_, end - start);
     } else
        f_();
  }
};

extern std::map<int, Test> *TheMapOfTests;

struct TestAdder {
  TestAdder(void_func_void_t f, int id, int flags = FEATURE) {
    if (TheMapOfTests == NULL)
      TheMapOfTests = new std::map<int, Test>;
    CHECK(TheMapOfTests->count(id) == 0);
    (*TheMapOfTests)[id] = Test(f, id, flags);
  }
};

#define REGISTER_TEST(f, id)         TestAdder add_test_##id (f, id);
#define REGISTER_TEST2(f, id, flags) TestAdder add_test_##id (f, id, flags);

#endif  
 
