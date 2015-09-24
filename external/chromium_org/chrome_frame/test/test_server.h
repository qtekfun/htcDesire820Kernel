// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_FRAME_TEST_TEST_SERVER_H_
#define CHROME_FRAME_TEST_TEST_SERVER_H_


#include <list>
#include <string>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/files/memory_mapped_file.h"
#include "base/message_loop/message_loop.h"
#include "net/socket/stream_listen_socket.h"

namespace test_server {

class Request {
 public:
  Request() : content_length_(0) {
  }

  void ParseHeaders(const std::string& headers);

  const std::string& method() const {
    return method_;
  }

  const std::string& path() const {
    return path_;
  }

  
  
  std::string arguments() const {
    std::string ret;
    std::string::size_type pos = path_.find('?');
    if (pos != std::string::npos)
      ret = path_.substr(pos + 1);
    return ret;
  }

  const std::string& headers() const {
    return headers_;
  }

  const std::string& content() const {
    return content_;
  }

  size_t content_length() const {
    return content_length_;
  }

  bool AllContentReceived() const {
    return method_.length() && content_.size() >= content_length_;
  }

  void OnDataReceived(const std::string& data);

 protected:
  std::string method_;
  std::string path_;
  std::string version_;
  std::string headers_;
  std::string content_;
  size_t content_length_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Request);
};

class Connection {
 public:
  explicit Connection(scoped_ptr<net::StreamListenSocket> sock)
      : socket_(sock.Pass()) {
  }

  ~Connection() {
  }

  bool IsSame(const net::StreamListenSocket* socket) const {
    return socket_ == socket;
  }

  const Request& request() const {
    return request_;
  }

  Request& request() {
    return request_;
  }

  void OnSocketClosed() {
    socket_.reset();
  }

 protected:
  scoped_ptr<net::StreamListenSocket> socket_;
  Request request_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Connection);
};

class Response {
 public:
  Response() : accessed_(0) {
  }

  virtual ~Response() {
  }

  
  virtual bool Matches(const Request& r) const = 0;

  
  
  virtual bool GetCustomHeaders(std::string* headers) const {
    return false;
  }

  
  
  virtual bool GetContentType(std::string* content_type) const {
    return false;
  }

  virtual size_t ContentLength() const {
    return 0;
  }

  virtual void WriteContents(net::StreamListenSocket* socket) const {
  }

  virtual void IncrementAccessCounter() {
    accessed_++;
  }

  size_t accessed() const {
    return accessed_;
  }

 protected:
  size_t accessed_;

 private:
  DISALLOW_COPY_AND_ASSIGN(Response);
};

class ResponseForPath : public Response {
 public:
  explicit ResponseForPath(const char* request_path)
      : request_path_(request_path) {
  }

  virtual ~ResponseForPath();

  virtual bool Matches(const Request& r) const {
    std::string path = r.path();
    std::string::size_type pos = path.find('?');
    if (pos != std::string::npos)
      path = path.substr(0, pos);
    return path.compare(request_path_) == 0;
  }

 protected:
  std::string request_path_;

 private:
  DISALLOW_COPY_AND_ASSIGN(ResponseForPath);
};

class SimpleResponse : public ResponseForPath {
 public:
  SimpleResponse(const char* request_path, const std::string& contents)
      : ResponseForPath(request_path), contents_(contents) {
  }

  virtual ~SimpleResponse();

  virtual void WriteContents(net::StreamListenSocket* socket) const {
    socket->Send(contents_.c_str(), contents_.length(), false);
  }

  virtual size_t ContentLength() const {
    return contents_.length();
  }

 protected:
  std::string contents_;

 private:
  DISALLOW_COPY_AND_ASSIGN(SimpleResponse);
};

class FileResponse : public ResponseForPath {
 public:
  FileResponse(const char* request_path, const base::FilePath& file_path)
      : ResponseForPath(request_path), file_path_(file_path) {
  }

  virtual bool GetContentType(std::string* content_type) const;
  virtual void WriteContents(net::StreamListenSocket* socket) const;
  virtual size_t ContentLength() const;

 protected:
  base::FilePath file_path_;
  mutable scoped_ptr<base::MemoryMappedFile> file_;

 private:
  DISALLOW_COPY_AND_ASSIGN(FileResponse);
};

class RedirectResponse : public ResponseForPath {
 public:
  RedirectResponse(const char* request_path, const std::string& redirect_url)
      : ResponseForPath(request_path), redirect_url_(redirect_url) {
  }

  virtual bool GetCustomHeaders(std::string* headers) const;

 protected:
  std::string redirect_url_;

 private:
  DISALLOW_COPY_AND_ASSIGN(RedirectResponse);
};

typedef std::list<Connection*> ConnectionList;

class SimpleWebServer : public net::StreamListenSocket::Delegate {
 public:
  
  
  explicit SimpleWebServer(int port);

  
  SimpleWebServer(const std::string& address, int port);
  virtual ~SimpleWebServer();

  void AddResponse(Response* response);

  
  
  
  
  
  
  void DeleteAllResponses();

  
  virtual void DidAccept(net::StreamListenSocket* server,
                         scoped_ptr<net::StreamListenSocket> connection);
  virtual void DidRead(net::StreamListenSocket* connection,
                       const char* data,
                       int len);
  virtual void DidClose(net::StreamListenSocket* sock);

  
  
  const std::string& host() const {
    return host_;
  }

  const ConnectionList& connections() const {
    return connections_;
  }

 protected:
  class QuitResponse : public SimpleResponse {
   public:
    QuitResponse()
        : SimpleResponse("/quit", "So long and thanks for all the fish.") {
    }

    virtual void WriteContents(net::StreamListenSocket* socket) const {
      SimpleResponse::WriteContents(socket);
      base::MessageLoop::current()->Quit();
    }
  };

  Response* FindResponse(const Request& request) const;
  Connection* FindConnection(const net::StreamListenSocket* socket) const;

  std::string host_;
  scoped_ptr<net::StreamListenSocket> server_;
  ConnectionList connections_;
  std::list<Response*> responses_;
  QuitResponse quit_;

 private:
  void Construct(const std::string& address, int port);
  DISALLOW_COPY_AND_ASSIGN(SimpleWebServer);
};

class ConfigurableConnection : public base::RefCounted<ConfigurableConnection> {
 public:
  struct SendOptions {
    enum Speed { IMMEDIATE, DELAYED, IMMEDIATE_HEADERS_DELAYED_CONTENT };
    SendOptions() : speed_(IMMEDIATE), chunk_size_(0), timeout_(0) { }
    SendOptions(Speed speed, int chunk_size, int64 timeout)
        : speed_(speed), chunk_size_(chunk_size), timeout_(timeout) {
    }

    Speed speed_;
    int chunk_size_;
    int64 timeout_;
  };

  explicit ConfigurableConnection(scoped_ptr<net::StreamListenSocket> sock)
      : socket_(sock.Pass()),
        cur_pos_(0) {}

  
  
  void Send(const std::string& headers, const std::string& content);

  
  
  
  void SendWithOptions(const std::string& headers, const std::string& content,
                       const SendOptions& options);

 private:
  friend class HTTPTestServer;
  
  
  void SendChunk();

  
  void Close();

  scoped_ptr<net::StreamListenSocket> socket_;
  Request r_;
  SendOptions options_;
  std::string data_;
  int cur_pos_;

  DISALLOW_COPY_AND_ASSIGN(ConfigurableConnection);
};

class HTTPTestServer : public net::StreamListenSocket::Delegate {
 public:
  HTTPTestServer(int port, const std::wstring& address,
                 base::FilePath root_dir);
  virtual ~HTTPTestServer();

  
  
  virtual void Get(ConfigurableConnection* connection,
                   const std::wstring& path, const Request& r) = 0;

  
  
  virtual void Post(ConfigurableConnection* connection,
                    const std::wstring& path, const Request& r) = 0;

  
  std::wstring Resolve(const std::wstring& path);

  base::FilePath root_dir() { return root_dir_; }

 protected:
  int port_;
  std::wstring address_;
  base::FilePath root_dir_;

 private:
  typedef std::list<scoped_refptr<ConfigurableConnection> > ConnectionList;
  ConnectionList::iterator FindConnection(
      const net::StreamListenSocket* socket);
  scoped_refptr<ConfigurableConnection> ConnectionFromSocket(
      const net::StreamListenSocket* socket);

  
  virtual void DidAccept(net::StreamListenSocket* server,
                         scoped_ptr<net::StreamListenSocket> socket);
  virtual void DidRead(net::StreamListenSocket* socket,
                       const char* data, int len);
  virtual void DidClose(net::StreamListenSocket* socket);

  scoped_ptr<net::StreamListenSocket> server_;
  ConnectionList connection_list_;

  DISALLOW_COPY_AND_ASSIGN(HTTPTestServer);
};

}  

#endif  
