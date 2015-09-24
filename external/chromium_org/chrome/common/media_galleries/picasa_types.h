// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_COMMON_MEDIA_GALLERIES_PICASA_TYPES_H_
#define CHROME_COMMON_MEDIA_GALLERIES_PICASA_TYPES_H_

#include <map>
#include <set>
#include <string>

#include "base/files/file_path.h"
#include "base/platform_file.h"
#include "ipc/ipc_platform_file.h"

namespace picasa {

struct AlbumInfo;

typedef std::map<std::string, base::FilePath> AlbumImages;
typedef std::set<std::string> AlbumUIDSet;
typedef std::map<std::string, AlbumImages> AlbumImagesMap;
typedef std::map<std::string, AlbumInfo> AlbumMap;

extern const char kPicasaDatabaseDirName[];
extern const char kPicasaTempDirName[];
extern const char kPicasaINIFilename[];

extern const char kPicasaAlbumTableName[];
extern const char kAlbumTokenPrefix[];

extern const uint32 kAlbumCategoryAlbum;
extern const uint32 kAlbumCategoryFolder;
extern const uint32 kAlbumCategoryInvalid;

struct AlbumInfo {
  AlbumInfo();
  AlbumInfo(const std::string& name, const base::Time& timestamp,
            const std::string& uid, const base::FilePath& path);

  ~AlbumInfo();

  std::string name;
  base::Time timestamp;
  std::string uid;
  base::FilePath path;
};

struct AlbumTableFiles {
  AlbumTableFiles();
  explicit AlbumTableFiles(const base::FilePath& directory_path);

  
  base::PlatformFile indicator_file;

  base::PlatformFile category_file;
  base::PlatformFile date_file;
  base::PlatformFile filename_file;
  base::PlatformFile name_file;
  base::PlatformFile token_file;
  base::PlatformFile uid_file;
};

struct AlbumTableFilesForTransit {
  AlbumTableFilesForTransit();
  IPC::PlatformFileForTransit indicator_file;

  IPC::PlatformFileForTransit category_file;
  IPC::PlatformFileForTransit date_file;
  IPC::PlatformFileForTransit filename_file;
  IPC::PlatformFileForTransit name_file;
  IPC::PlatformFileForTransit token_file;
  IPC::PlatformFileForTransit uid_file;
};

struct FolderINIContents {
  base::FilePath folder_path;
  std::string ini_contents;

  bool operator<(const FolderINIContents& that) const {
    return folder_path < that.folder_path;
  }
};

void CloseAlbumTableFiles(AlbumTableFiles* table_files);

}  

#endif  
