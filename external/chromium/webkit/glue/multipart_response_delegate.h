// Copyright (c) 2006-2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
// This seems like a derivative work, so here's the original license:

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
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
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

#ifndef WEBKIT_GLUE_MULTIPART_RESPONSE_DELEGATE_H_
#define WEBKIT_GLUE_MULTIPART_RESPONSE_DELEGATE_H_

#include <string>

#include "base/basictypes.h"
#include "third_party/WebKit/Source/WebKit/chromium/public/WebURLResponse.h"

namespace WebKit {
class WebURLLoader;
class WebURLLoaderClient;
}

namespace webkit_glue {

class MultipartResponseDelegateTester;

class MultipartResponseDelegate {
 public:
  MultipartResponseDelegate(WebKit::WebURLLoaderClient* client,
                            WebKit::WebURLLoader* loader,
                            const WebKit::WebURLResponse& response,
                            const std::string& boundary);

  
  void OnReceivedData(const char* data, int data_len, int encoded_data_length);
  void OnCompletedRequest();

  
  void Cancel() {
    client_ = NULL;
    loader_ = NULL;
  }

  
  
  
  static bool ReadMultipartBoundary(const WebKit::WebURLResponse& response,
                                    std::string* multipart_boundary);

  
  
  
  static bool ReadContentRanges(const WebKit::WebURLResponse& response,
                                int* content_range_lower_bound,
                                int* content_range_upper_bound,
                                int* content_range_instance_size);

 private:
  friend class MultipartResponseDelegateTester;  

  
  
  WebKit::WebURLLoaderClient* client_;
  WebKit::WebURLLoader* loader_;

  
  
  WebKit::WebURLResponse original_response_;

  
  
  int PushOverLine(const std::string& data, size_t pos);

  
  
  
  bool ParseHeaders();

  
  
  size_t FindBoundary();

  
  int encoded_data_length_;

  
  
  std::string data_;

  
  std::string boundary_;

  
  bool first_received_data_;

  
  bool processing_headers_;

  
  
  bool stop_sending_;

  
  bool has_sent_first_response_;

  DISALLOW_COPY_AND_ASSIGN(MultipartResponseDelegate);
};

}  

#endif
