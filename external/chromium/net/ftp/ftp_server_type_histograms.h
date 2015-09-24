// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_FTP_FTP_SERVER_TYPE_HISTOGRAMS_H_
#define NET_FTP_FTP_SERVER_TYPE_HISTOGRAMS_H_
#pragma once


namespace net {

enum FtpServerType {
  
  
  
  SERVER_UNKNOWN = 0,

  
  

  SERVER_LS = 9,        
  SERVER_WINDOWS = 10,  
  SERVER_VMS = 11,      
  SERVER_NETWARE = 12,  

  

  NUM_OF_SERVER_TYPES
};

void UpdateFtpServerTypeHistograms(FtpServerType type);

}  

#endif  
