// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIBRARIES_NACL_IO_KERNEL_PROXY_H_
#define LIBRARIES_NACL_IO_KERNEL_PROXY_H_

#include <map>
#include <string>

#include "nacl_io/event_emitter.h"
#include "nacl_io/host_resolver.h"
#include "nacl_io/kernel_object.h"
#include "nacl_io/mount_factory.h"
#include "nacl_io/mount_stream.h"
#include "nacl_io/ossignal.h"
#include "nacl_io/ossocket.h"
#include "nacl_io/ostypes.h"
#include "nacl_io/osutime.h"

struct fuse_operations;
struct timeval;

namespace nacl_io {

class PepperInterface;


class KernelProxy : protected KernelObject {
 public:
  typedef std::map<std::string, MountFactory*> MountFactoryMap_t;

  KernelProxy();
  virtual ~KernelProxy();

  
  
  virtual Error Init(PepperInterface* ppapi);

  
  
  bool RegisterMountType(const char* mount_type, fuse_operations* fuse_ops);
  bool UnregisterMountType(const char* mount_type);

  virtual int pipe(int pipefds[2]);

  
  virtual int open_resource(const char* file);

  
  virtual int open(const char* path, int open_flags);
  virtual int close(int fd);
  virtual int dup(int fd);
  virtual int dup2(int fd, int newfd);

  
  virtual int chdir(const char* path);
  virtual char* getcwd(char* buf, size_t size);
  virtual char* getwd(char* buf);
  virtual int mount(const char *source,
                    const char *target,
                    const char *filesystemtype,
                    unsigned long mountflags,
                    const void *data);
  virtual int umount(const char *path);

  
  virtual int chown(const char* path, uid_t owner, gid_t group);
  virtual int fchown(int fd, uid_t owner, gid_t group);
  virtual int lchown(const char* path, uid_t owner, gid_t group);
  virtual int utime(const char* filename, const struct utimbuf* times);

  
  
  
  
  
  virtual int chmod(const char *path, mode_t mode);
  virtual int mkdir(const char *path, mode_t mode);
  virtual int rmdir(const char *path);
  virtual int stat(const char *path, struct stat *buf);

  
  
  
  
  virtual ssize_t read(int fd, void *buf, size_t nbyte);
  virtual ssize_t write(int fd, const void *buf, size_t nbyte);

  virtual int fchmod(int fd, int prot);
  virtual int fcntl(int fd, int request, va_list args);
  virtual int fstat(int fd, struct stat *buf);
  virtual int getdents(int fd, void *buf, unsigned int count);
  virtual int fchdir(int fd);
  virtual int ftruncate(int fd, off_t length);
  virtual int fsync(int fd);
  virtual int fdatasync(int fd);
  virtual int isatty(int fd);
  virtual int ioctl(int fd, int request, va_list args);

  
  
  virtual off_t lseek(int fd, off_t offset, int whence);

  
  
  
  virtual int remove(const char* path);
  
  virtual int unlink(const char* path);
  virtual int truncate(const char* path, off_t len);
  virtual int lstat(const char* path, struct stat* buf);
  virtual int rename(const char* path, const char* newpath);
  
  virtual int access(const char* path, int amode);
  virtual int readlink(const char *path, char *buf, size_t count);
  virtual int utimes(const char *filename, const struct timeval times[2]);

  virtual int link(const char* oldpath, const char* newpath);
  virtual int symlink(const char* oldpath, const char* newpath);

  virtual void* mmap(void* addr,
                     size_t length,
                     int prot,
                     int flags,
                     int fd,
                     size_t offset);
  virtual int munmap(void* addr, size_t length);
  virtual int tcflush(int fd, int queue_selector);
  virtual int tcgetattr(int fd, struct termios* termios_p);
  virtual int tcsetattr(int fd, int optional_actions,
                           const struct termios *termios_p);

  virtual int kill(pid_t pid, int sig);
  virtual int sigaction(int signum, const struct sigaction* action,
                        struct sigaction* oaction);

#ifdef PROVIDES_SOCKET_API
  virtual int select(int nfds, fd_set* readfds, fd_set* writefds,
                    fd_set* exceptfds, struct timeval* timeout);

  virtual int poll(struct pollfd *fds, nfds_t nfds, int timeout);

  
  virtual int accept(int fd, struct sockaddr* addr, socklen_t* len);
  virtual int bind(int fd, const struct sockaddr* addr, socklen_t len);
  virtual int connect(int fd, const struct sockaddr* addr, socklen_t len);
  virtual struct hostent* gethostbyname(const char* name);
  virtual int getpeername(int fd, struct sockaddr* addr, socklen_t* len);
  virtual int getsockname(int fd, struct sockaddr* addr, socklen_t* len);
  virtual int getsockopt(int fd,
                         int lvl,
                         int optname,
                         void* optval,
                         socklen_t* len);
  virtual int listen(int fd, int backlog);
  virtual ssize_t recv(int fd,
                       void* buf,
                       size_t len,
                       int flags);
  virtual ssize_t recvfrom(int fd,
                           void* buf,
                           size_t len,
                           int flags,
                           struct sockaddr* addr,
                           socklen_t* addrlen);
  virtual ssize_t recvmsg(int fd, struct msghdr* msg, int flags);
  virtual ssize_t send(int fd, const void* buf, size_t len, int flags);
  virtual ssize_t sendto(int fd,
                         const void* buf,
                         size_t len,
                         int flags,
                         const struct sockaddr* addr,
                         socklen_t addrlen);
  virtual ssize_t sendmsg(int fd, const struct msghdr* msg, int flags);
  virtual int setsockopt(int fd,
                         int lvl,
                         int optname,
                         const void* optval,
                         socklen_t len);
  virtual int shutdown(int fd, int how);
  virtual int socket(int domain, int type, int protocol);
  virtual int socketpair(int domain, int type, int protocol, int* sv);
#endif  

 protected:
  MountFactoryMap_t factories_;
  sdk_util::ScopedRef<MountStream> stream_mount_;
  int dev_;
  PepperInterface* ppapi_;
  static KernelProxy *s_instance_;
  struct sigaction sigwinch_handler_;
#ifdef PROVIDES_SOCKET_API
  HostResolver host_resolver_;
#endif

#ifdef PROVIDES_SOCKET_API
  virtual int AcquireSocketHandle(int fd, ScopedKernelHandle* handle);
#endif

  ScopedEventEmitter signal_emitter_;
  DISALLOW_COPY_AND_ASSIGN(KernelProxy);
};

}  

#endif  
