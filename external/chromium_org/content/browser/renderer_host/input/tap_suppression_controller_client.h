// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_RENDERER_HOST_INPUT_TAP_SUPPRESSION_CONTROLLER_CLIENT_H_
#define CONTENT_BROWSER_RENDERER_HOST_INPUT_TAP_SUPPRESSION_CONTROLLER_CLIENT_H_

namespace content {

class TapSuppressionControllerClient {
 public:
  virtual ~TapSuppressionControllerClient() {}

  
  
  virtual int MaxCancelToDownTimeInMs() = 0;

  
  
  virtual int MaxTapGapTimeInMs() = 0;

  
  virtual void DropStashedTapDown() = 0;

  
  
  
  virtual void ForwardStashedTapDown() = 0;

 protected:
  TapSuppressionControllerClient() {}

 private:
  DISALLOW_COPY_AND_ASSIGN(TapSuppressionControllerClient);
};

}  

#endif  
