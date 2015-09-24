// Copyright (c) 2009 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_FTP_FTP_SERVER_TYPE_HISTOGRAMS_H_
#define NET_FTP_FTP_SERVER_TYPE_HISTOGRAMS_H_


namespace net {

enum FtpServerType {
  
  
  
  SERVER_UNKNOWN = 0,

  SERVER_LS = 1,       
  SERVER_WINDOWS = 2,  
  SERVER_VMS = 3,      
  SERVER_NETWARE = 4,  
  SERVER_OS2 = 5,      

  NUM_OF_SERVER_TYPES
};

void UpdateFtpServerTypeHistograms(FtpServerType type);

}  

#endif  
