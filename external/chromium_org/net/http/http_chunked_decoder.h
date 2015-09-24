// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// The license block is:
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications.
 * Portions created by the Initial Developer are Copyright (C) 2001
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Darin Fisher <darin@netscape.com> (original author)
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#ifndef NET_HTTP_HTTP_CHUNKED_DECODER_H_
#define NET_HTTP_HTTP_CHUNKED_DECODER_H_

#include <string>

#include "net/base/net_export.h"

namespace net {

class NET_EXPORT_PRIVATE HttpChunkedDecoder {
 public:
  
  
  static const size_t kMaxLineBufLen;

  HttpChunkedDecoder();

  
  bool reached_eof() const { return reached_eof_; }

  
  int bytes_after_eof() const { return bytes_after_eof_; }

  
  
  
  
  
  int FilterBuf(char* buf, int buf_len);

 private:
  
  
  
  int ScanForChunkRemaining(const char* buf, int buf_len);

  
  
  
  static bool ParseChunkSize(const char* start, int len, int* out);

  
  int chunk_remaining_;

  
  std::string line_buf_;

  
  bool chunk_terminator_remaining_;

  
  bool reached_last_chunk_;

  
  bool reached_eof_;

  
  
  int bytes_after_eof_;
};

}  

#endif  