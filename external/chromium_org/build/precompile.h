// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#if defined(BUILD_PRECOMPILE_H_)
#error You shouldn't include the precompiled header file more than once.
#endif

#define BUILD_PRECOMPILE_H_

// The Windows header needs to come before almost all the other
// Windows-specific headers.
#include <Windows.h>
#include <dwmapi.h>
#include <shellapi.h>
#include <wincrypt.h>  // 4
#include <wtypes.h>  // 2

// Defines in atlbase.h cause conflicts; if we could figure out how
// this family of headers can be included in the PCH, it might speed
// up the build as several of them are used frequently.
/*
#include <atlbase.h>
#include <atlapp.h>
#include <atlcom.h>
#include <atlcrack.h>  // 2
#include <atlctrls.h>  // 2
#include <atlmisc.h>  // 2
#include <atlsafe.h>  // 1
#include <atltheme.h>  // 1
#include <atlwin.h>  // 2
*/

// Objbase.h and other files that rely on it bring in [ #define
// interface struct ] which can cause problems in a multi-platform
// build like Chrome's. #undef-ing it does not work as there are


#include <errno.h>
#include <fcntl.h>
#include <limits.h>  
#include <math.h>
#include <memory.h>  
#include <signal.h>
#include <stdarg.h>  
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>  

#include <algorithm>
#include <bitset>  
#include <cmath>
#include <cstddef>
#include <cstdio>  
#include <cstdlib>  
#include <cstring>
#include <deque>
#include <fstream>  
#include <functional>
#include <iomanip>  
#include <iosfwd>  
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <numeric>  
#include <ostream>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "intsafe_workaround.h"
