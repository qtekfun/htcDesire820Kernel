/**
 * Copyright (c) 2012, Code Aurora Forum. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other *materials provided
 *     with the distribution.
 *   * Neither the name of Code Aurora Forum, Inc. nor the names of its
 *     contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **/

#ifndef HTTP_GETZIP_FACTORY_H_
#define HTTP_GETZIP_FACTORY_H_

#include "net/socket/client_socket.h"
#include <sys/types.h>
#include "base/basictypes.h"

namespace net
{

class HttpRequestHeaders;
class HttpResponseHeaders;

typedef enum
{
  GETZIP_OK                  = 1,
  REQUEST_RETRY_NEEDED       = 2, 
  NULL_ARGUMENT              = 3, 
  NO_GETZIP_CONNECTION       = 4  
} GETZipDecompressionStatus;

class IGetZipManager
{
public:
  IGetZipManager();
  virtual void CompressRequestHeaders(HttpRequestHeaders&, ClientSocket*) = 0;
  virtual GETZipDecompressionStatus
      DecompressResponseHeaders(HttpResponseHeaders*, ClientSocket*) = 0;
  virtual void StopGetZipConnection(ClientSocket*) = 0;
  virtual void OpenGetZipConnection(ClientSocket*) = 0;
  virtual ~IGetZipManager() = 0;

private:
  DISALLOW_COPY_AND_ASSIGN(IGetZipManager);
};

class GetZipManager: public IGetZipManager
{
public:

  GetZipManager();
  virtual ~GetZipManager()
  {
  }
  ;

  virtual void CompressRequestHeaders(HttpRequestHeaders&, ClientSocket*)
  {
  }
  ;
  virtual GETZipDecompressionStatus DecompressResponseHeaders(HttpResponseHeaders*, ClientSocket*)
  {
    return NO_GETZIP_CONNECTION;
  }
  ;
  virtual void StopGetZipConnection(ClientSocket*)
  {
  }
  ;
  virtual void OpenGetZipConnection(ClientSocket*)
  {
  }
  ;

private:
  DISALLOW_COPY_AND_ASSIGN(GetZipManager);
};

class HttpGetZipFactory
{

public:

  
  
  
  
  static IGetZipManager* GetGETZipManager();

  
  
  static void InitGETZipManager();

  void StopGETZipManager();

private:
  IGetZipManager* m_pMngr;
  static HttpGetZipFactory* s_pFactory;

  void* libHandle;

  HttpGetZipFactory();
  ~HttpGetZipFactory();

  DISALLOW_COPY_AND_ASSIGN(HttpGetZipFactory);
};

}; 
#endif 
