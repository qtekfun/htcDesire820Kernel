// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_ITUNES_DATA_PROVIDER_H_
#define CHROME_BROWSER_MEDIA_GALLERIES_FILEAPI_ITUNES_DATA_PROVIDER_H_

#include <map>
#include <set>
#include <string>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/files/file_path.h"
#include "base/files/file_path_watcher.h"
#include "chrome/browser/media_galleries/fileapi/iapps_data_provider.h"
#include "chrome/browser/media_galleries/fileapi/safe_iapps_library_parser.h"

namespace itunes {

class TestITunesDataProvider;

class ITunesDataProvider : public iapps::IAppsDataProvider {
 public:
  typedef std::string ArtistName;
  typedef std::string AlbumName;
  typedef std::string TrackName;
  typedef std::map<TrackName, base::FilePath> Album;

  explicit ITunesDataProvider(const base::FilePath& library_path);
  virtual ~ITunesDataProvider();

  
  virtual const base::FilePath& auto_add_path() const;

  
  bool KnownArtist(const ArtistName& artist) const;

  
  bool KnownAlbum(const ArtistName& artist, const AlbumName& album) const;

  
  
  base::FilePath GetTrackLocation(const ArtistName& artist,
                                  const AlbumName& album,
                                  const TrackName& track) const;

  
  std::set<ArtistName> GetArtistNames() const;

  
  std::set<AlbumName> GetAlbumNames(const ArtistName& artist) const;

  
  Album GetAlbum(const ArtistName& artist, const AlbumName& album) const;

 private:
  friend class TestITunesDataProvider;

  typedef std::map<AlbumName, Album> Artist;
  typedef std::map<ArtistName, Artist> Library;

  
  virtual void DoParseLibrary(const base::FilePath& library_path,
                              const ReadyCallback& ready_callback) OVERRIDE;

  
  void OnLibraryParsed(const ReadyCallback& ready_callback,
                       bool result,
                       const parser::Library& library);

  
  const base::FilePath auto_add_path_;

  
  Library library_;

  scoped_refptr<iapps::SafeIAppsLibraryParser> xml_parser_;

  
  
  base::WeakPtrFactory<ITunesDataProvider> weak_factory_;

  DISALLOW_COPY_AND_ASSIGN(ITunesDataProvider);
};

}  

#endif  
