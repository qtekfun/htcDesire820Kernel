// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#ifndef CHROME_TEST_LOGGING_WIN_LOG_FILE_PRINTER_H_
#define CHROME_TEST_LOGGING_WIN_LOG_FILE_PRINTER_H_

#include <iosfwd>

namespace base {
class FilePath;
}

namespace logging_win {

void PrintLogFile(const base::FilePath& log_file, std::ostream* out);

}  

#endif  
