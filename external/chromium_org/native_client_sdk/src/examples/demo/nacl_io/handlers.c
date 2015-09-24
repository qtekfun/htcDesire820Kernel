/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file. */

#include "handlers.h"

#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>

#include "nacl_io/osdirent.h"

#include "nacl_io_demo.h"

#define MAX_OPEN_FILES 10
#define MAX_OPEN_DIRS 10

#if defined(WIN32)
#define stat _stat
#endif

static FILE* g_OpenFiles[MAX_OPEN_FILES];

static void* g_OpenDirs[MAX_OPEN_DIRS];

static int AddToMap(void** map, int max_map_size, void* object) {
  int i;
  assert(object != NULL);
  for (i = 0; i < max_map_size; ++i) {
    if (map[i] == NULL) {
      map[i] = object;
      return i;
    }
  }

  return -1;
}

static void RemoveFromMap(void** map, int max_map_size, int i) {
  assert(i >= 0 && i < max_map_size);
  map[i] = NULL;
}

static void* GetFromMap(void** map, int max_map_size, int i) {
  assert(i >= 0 && i < max_map_size);
  return map[i];
}

static void* GetFromIndexString(void** map,
                                int max_map_size,
                                const char* s,
                                int* index) {
  char* endptr;
  int result = strtol(s, &endptr, 10);
  if (endptr != s + strlen(s)) {
    
    return NULL;
  }

  if (index)
    *index = result;

  return GetFromMap(map, max_map_size, result);
}

static int AddFileToMap(FILE* file) {
  return AddToMap((void**)g_OpenFiles, MAX_OPEN_FILES, file);
}

static void RemoveFileFromMap(int i) {
  RemoveFromMap((void**)g_OpenFiles, MAX_OPEN_FILES, i);
}

static FILE* GetFileFromIndexString(const char* s, int* file_index) {
  return (FILE*)GetFromIndexString(
      (void**)g_OpenFiles, MAX_OPEN_FILES, s, file_index);
}

#if !defined(WIN32)
static int AddDirToMap(DIR* dir) {
  return AddToMap((void**)g_OpenDirs, MAX_OPEN_DIRS, dir);
}

static void RemoveDirFromMap(int i) {
  RemoveFromMap((void**)g_OpenDirs, MAX_OPEN_DIRS, i);
}

static DIR* GetDirFromIndexString(const char* s, int* dir_index) {
  return (DIR*)GetFromIndexString(
      (void**)g_OpenDirs, MAX_OPEN_DIRS, s, dir_index);
}
#endif

int HandleFopen(int num_params, char** params, char** output) {
  FILE* file;
  int file_index;
  const char* filename;
  const char* mode;

  if (num_params != 2) {
    *output = PrintfToNewString("fopen takes 2 parameters.");
    return 1;
  }

  filename = params[0];
  mode = params[1];

  file = fopen(filename, mode);
  if (!file) {
    *output = PrintfToNewString("fopen returned a NULL FILE*.");
    return 2;
  }

  file_index = AddFileToMap(file);
  if (file_index == -1) {
    *output = PrintfToNewString(
        "Example only allows %d open file handles.", MAX_OPEN_FILES);
    return 3;
  }

  *output = PrintfToNewString("fopen\1%s\1%d", filename, file_index);
  return 0;
}

/**
 * Handle a call to fwrite() made by JavaScript.
 *
 * fwrite expects 2 parameters:
 *   0: The index of the file (which is mapped to a FILE*)
 *   1: A string to write to the file
 * on success, fwrite returns a result in |output| separated by \1:
 *   0: "fwrite"
 *   1: the file index
 *   2: the number of bytes written
 * on failure, fwrite returns an error string in |output|.
 *
 * @param[in] num_params The number of params in |params|.
 * @param[in] params An array of strings, parameters to this function.
 * @param[out] output A string to write informational function output to.
 * @return An errorcode; 0 means success, anything else is a failure.
 */
int HandleFwrite(int num_params, char** params, char** output) {
  FILE* file;
  const char* file_index_string;
  const char* data;
  size_t data_len;
  size_t bytes_written;

  if (num_params != 2) {
    *output = PrintfToNewString("fwrite takes 2 parameters.");
    return 1;
  }

  file_index_string = params[0];
  file = GetFileFromIndexString(file_index_string, NULL);
  data = params[1];
  data_len = strlen(data);

  if (!file) {
    *output =
        PrintfToNewString("Unknown file handle %s.", file_index_string);
    return 2;
  }

  bytes_written = fwrite(data, 1, data_len, file);

  if (ferror(file)) {
    *output = PrintfToNewString(
        "Wrote %d bytes, but ferror() returns true.", bytes_written);
    return 3;
  }

  *output =
      PrintfToNewString("fwrite\1%s\1%d", file_index_string, bytes_written);
  return 0;
}

int HandleFread(int num_params, char** params, char** output) {
  FILE* file;
  const char* file_index_string;
  char* buffer;
  size_t data_len;
  size_t bytes_read;

  if (num_params != 2) {
    *output = PrintfToNewString("fread takes 2 parameters.");
    return 1;
  }

  file_index_string = params[0];
  file = GetFileFromIndexString(file_index_string, NULL);
  data_len = strtol(params[1], NULL, 10);

  if (!file) {
    *output =
        PrintfToNewString("Unknown file handle %s.", file_index_string);
    return 2;
  }

  buffer = (char*)malloc(data_len + 1);
  bytes_read = fread(buffer, 1, data_len, file);
  buffer[bytes_read] = 0;

  if (ferror(file)) {
    *output = PrintfToNewString(
        "Read %d bytes, but ferror() returns true.", bytes_read);
    return 3;
  }

  *output = PrintfToNewString("fread\1%s\1%s", file_index_string, buffer);
  free(buffer);
  return 0;
}

int HandleFseek(int num_params, char** params, char** output) {
  FILE* file;
  const char* file_index_string;
  long offset;
  int whence;
  int result;

  if (num_params != 3) {
    *output = PrintfToNewString("fseek takes 3 parameters.");
    return 1;
  }

  file_index_string = params[0];
  file = GetFileFromIndexString(file_index_string, NULL);
  offset = strtol(params[1], NULL, 10);
  whence = strtol(params[2], NULL, 10);

  if (!file) {
    *output =
        PrintfToNewString("Unknown file handle %s.", file_index_string);
    return 2;
  }

  result = fseek(file, offset, whence);
  if (result) {
    *output = PrintfToNewString("fseek returned error %d.", result);
    return 3;
  }

  offset = ftell(file);
  if (offset < 0) {
    *output = PrintfToNewString(
        "fseek succeeded, but ftell returned error %d.", offset);
    return 4;
  }

  *output = PrintfToNewString("fseek\1%s\1%d", file_index_string, offset);
  return 0;
}

int HandleFclose(int num_params, char** params, char** output) {
  FILE* file;
  int file_index;
  const char* file_index_string;
  int result;

  if (num_params != 1) {
    *output = PrintfToNewString("fclose takes 1 parameters.");
    return 1;
  }

  file_index_string = params[0];
  file = GetFileFromIndexString(file_index_string, &file_index);
  if (!file) {
    *output =
        PrintfToNewString("Unknown file handle %s.", file_index_string);
    return 2;
  }

  result = fclose(file);
  if (result) {
    *output = PrintfToNewString("fclose returned error %d.", result);
    return 3;
  }

  RemoveFileFromMap(file_index);

  *output = PrintfToNewString("fclose\1%s", file_index_string);
  return 0;
}

int HandleStat(int num_params, char** params, char** output) {
  const char* filename;
  int result;
  struct stat buf;

  if (num_params != 1) {
    *output = PrintfToNewString("stat takes 1 parameter.");
    return 1;
  }

  filename = params[0];

  memset(&buf, 0, sizeof(buf));
  result = stat(filename, &buf);
  if (result == -1) {
    *output = PrintfToNewString("stat returned error %d.", errno);
    return 2;
  }

  *output = PrintfToNewString("stat\1%s\1%d", filename, buf.st_size);
  return 0;
}

int HandleOpendir(int num_params, char** params, char** output) {
#if defined(WIN32)
  *output = PrintfToNewString("Win32 does not support opendir.");
  return 1;
#else
  DIR* dir;
  int dir_index;
  const char* dirname;

  if (num_params != 1) {
    *output = PrintfToNewString("opendir takes 1 parameter.");
    return 1;
  }

  dirname = params[0];

  dir = opendir(dirname);
  if (!dir) {
    *output = PrintfToNewString("opendir returned a NULL DIR*.");
    return 2;
  }

  dir_index = AddDirToMap(dir);
  if (dir_index == -1) {
    *output = PrintfToNewString(
        "Example only allows %d open dir handles.", MAX_OPEN_DIRS);
    return 3;
  }

  *output = PrintfToNewString("opendir\1%s\1%d", dirname, dir_index);
  return 0;
#endif
}

int HandleReaddir(int num_params, char** params, char** output) {
#if defined(WIN32)
  *output = PrintfToNewString("Win32 does not support readdir.");
  return 1;
#else
  DIR* dir;
  const char* dir_index_string;
  struct dirent* entry;

  if (num_params != 1) {
    *output = PrintfToNewString("readdir takes 1 parameter.");
    return 1;
  }

  dir_index_string = params[0];
  dir = GetDirFromIndexString(dir_index_string, NULL);

  if (!dir) {
    *output = PrintfToNewString("Unknown dir handle %s.", dir_index_string);
    return 2;
  }

  entry = readdir(dir);
  if (entry != NULL) {
    *output = PrintfToNewString("readdir\1%s\1%d\1%s", dir_index_string,
                                entry->d_ino, entry->d_name);
  } else {
    *output = PrintfToNewString("readdir\1%s\1\1", dir_index_string);
  }

  return 0;
#endif
}

int HandleClosedir(int num_params, char** params, char** output) {
#if defined(WIN32)
  *output = PrintfToNewString("Win32 does not support closedir.");
  return 1;
#else
  DIR* dir;
  int dir_index;
  const char* dir_index_string;
  int result;

  if (num_params != 1) {
    *output = PrintfToNewString("closedir takes 1 parameters.");
    return 1;
  }

  dir_index_string = params[0];
  dir = GetDirFromIndexString(dir_index_string, &dir_index);
  if (!dir) {
    *output = PrintfToNewString("Unknown dir handle %s.",
                                dir_index_string);
    return 2;
  }

  result = closedir(dir);
  if (result) {
    *output = PrintfToNewString("closedir returned error %d.", result);
    return 3;
  }

  RemoveDirFromMap(dir_index);

  *output = PrintfToNewString("closedir\1%s", dir_index_string);
  return 0;
#endif
}

int HandleMkdir(int num_params, char** params, char** output) {
  const char* dirname;
  int result;
  int mode;

  if (num_params != 2) {
    *output = PrintfToNewString("mkdir takes 2 parameters.");
    return 1;
  }

  dirname = params[0];
  mode = strtol(params[1], NULL, 8);

  result = mkdir(dirname, mode);
  if (result != 0) {
    *output = PrintfToNewString("mkdir returned error: %d", errno);
    return 2;
  }

  *output = PrintfToNewString("mkdir\1%s", dirname);
  return 0;
}

int HandleRmdir(int num_params, char** params, char** output) {
  if (num_params != 1) {
    *output = PrintfToNewString("rmdir takes 1 parameter.");
    return 1;
  }

  const char* dirname = params[0];
  int result = rmdir(dirname);
  if (result != 0) {
    *output = PrintfToNewString("rmdir returned error: %d", errno);
    return 2;
  }

  *output = PrintfToNewString("rmdir\1%s", dirname);
  return 0;
}

int HandleChdir(int num_params, char** params, char** output) {
  if (num_params != 1) {
    *output = PrintfToNewString("chdir takes 1 parameter.");
    return 1;
  }

  const char* dirname = params[0];
  int result = chdir(dirname);
  if (result != 0) {
    *output = PrintfToNewString("chdir returned error: %d", errno);
    return 2;
  }

  *output = PrintfToNewString("chdir\1%s", dirname);
  return 0;
}

int HandleGetcwd(int num_params, char** params, char** output) {
  if (num_params != 0) {
    *output = PrintfToNewString("getcwd takes 0 parameters.");
    return 1;
  }

  char cwd[PATH_MAX];
  char* result = getcwd(cwd, PATH_MAX);
  if (result == NULL) {
    *output = PrintfToNewString("getcwd returned error: %d", errno);
    return 1;
  }

  *output = PrintfToNewString("getcwd\1%s", cwd);
  return 0;
}

int HandleGethostbyname(int num_params, char** params, char** output) {
  struct hostent* info;
  struct in_addr **addr_list;
  const char* addr_type;
  const char* name;
  char inet6_addr_str[INET6_ADDRSTRLEN];
  int non_variable_len, output_len;
  int current_pos;
  int i;

  if (num_params != 1) {
    *output = PrintfToNewString("gethostbyname takes 1 parameter.");
    return 1;
  }

  name = params[0];

  info = gethostbyname(name);
  if (!info) {
    *output = PrintfToNewString("gethostbyname failed, error is \"%s\"",
                                hstrerror(h_errno));
    return 2;
  }

  addr_type = info->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6";

  non_variable_len = strlen("gethostbyname") + 1
    + strlen(info->h_name) + 1 + strlen(addr_type);
  output_len = non_variable_len;

  addr_list = (struct in_addr **)info->h_addr_list;
  for (i = 0; addr_list[i] != NULL; i++) {
    output_len += 1; 
    if (info->h_addrtype == AF_INET) {
      output_len += strlen(inet_ntoa(*addr_list[i]));
    } else { 
      inet_ntop(AF_INET6, addr_list[i], inet6_addr_str, INET6_ADDRSTRLEN);
      output_len += strlen(inet6_addr_str);
    }
  }

  *output = (char*) calloc(output_len + 1, 1);
  if (!*output) {
    *output = PrintfToNewString("out of memory.");
    return 3;
  }
  snprintf(*output, non_variable_len + 1, "gethostbyname\1%s\1%s",
           info->h_name, addr_type);

  current_pos = non_variable_len;
  for (i = 0; addr_list[i] != NULL; i++) {
    if (info->h_addrtype == AF_INET) {
      current_pos += sprintf(*output + current_pos,
                             "\1%s", inet_ntoa(*addr_list[i]));
    } else { 
      inet_ntop(AF_INET6, addr_list[i], inet6_addr_str, INET6_ADDRSTRLEN);
      sprintf(*output + current_pos, "\1%s", inet6_addr_str);
    }
  }
  return 0;
}

int HandleConnect(int num_params, char** params, char** output) {
  if (num_params != 2) {
    *output = PrintfToNewString("connect takes 2 parameters.");
    return 1;
  }

  struct sockaddr_in addr;
  socklen_t addrlen = sizeof(addr);
  const char* hostname = params[0];
  int port = strtol(params[1], NULL, 10);

  
  struct hostent* hostent = gethostbyname(hostname);
  if (hostent == NULL) {
    *output = PrintfToNewString("gethostbyname() returned error: %d", errno);
    return 1;
  }

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  memcpy(&addr.sin_addr.s_addr, hostent->h_addr_list[0], hostent->h_length);

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    *output = PrintfToNewString("socket() failed: %s", strerror(errno));
    return 1;
  }

  int result = connect(sock, (struct sockaddr*)&addr, addrlen);
  if (result != 0) {
    *output = PrintfToNewString("connect() failed: %s", strerror(errno));
    close(sock);
    return 1;
  }

  *output = PrintfToNewString("connect\1%d", sock);
  return 0;
}

int HandleSend(int num_params, char** params, char** output) {
  if (num_params != 2) {
    *output = PrintfToNewString("send takes 2 parameters.");
    return 1;
  }

  int sock = strtol(params[0], NULL, 10);
  const char* buffer = params[1];
  int result = send(sock, buffer, strlen(buffer), 0);
  if (result <= 0) {
    *output = PrintfToNewString("send failed: %s", strerror(errno));
    return 1;
  }

  *output = PrintfToNewString("send\1%d", result);
  return 0;
}

int HandleRecv(int num_params, char** params, char** output) {
  if (num_params != 2) {
    *output = PrintfToNewString("recv takes 2 parameters.");
    return 1;
  }

  int sock = strtol(params[0], NULL, 10);
  int buffersize = strtol(params[1], NULL, 10);
  if (buffersize < 0 || buffersize > 65 * 1024) {
    *output = PrintfToNewString("recv buffersize must be between 0 and 65k.");
    return 1;
  }

  char* buffer = alloca(buffersize);
  memset(buffer, 0, buffersize);
  int result = recv(sock, buffer, buffersize, 0);
  if (result <= 0) {
    *output = PrintfToNewString("recv failed: %s", strerror(errno));
    return 2;
  }

  *output = PrintfToNewString("recv\1%d\1%s", result, buffer);
  return 0;
}

int HandleClose(int num_params, char** params, char** output) {
  if (num_params != 1) {
    *output = PrintfToNewString("close takes 1 parameters.");
    return 1;
  }

  int sock = strtol(params[0], NULL, 10);
  int result = close(sock);
  if (result != 0) {
    *output = PrintfToNewString("close returned error: %d", errno);
    return 2;
  }

  *output = PrintfToNewString("close\1%d", sock);
  return 0;
}
