// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DISK_CACHE_SIMPLE_SIMPLE_SYNCHRONOUS_ENTRY_H_
#define NET_DISK_CACHE_SIMPLE_SIMPLE_SYNCHRONOUS_ENTRY_H_

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/platform_file.h"
#include "base/time/time.h"
#include "net/base/cache_type.h"
#include "net/base/net_export.h"
#include "net/disk_cache/simple/simple_entry_format.h"

namespace net {
class GrowableIOBuffer;
class IOBuffer;
}

namespace disk_cache {

class SimpleSynchronousEntry;

class NET_EXPORT_PRIVATE SimpleEntryStat {
 public:
  SimpleEntryStat(base::Time last_used,
                  base::Time last_modified,
                  const int32 data_size[],
                  const int32 sparse_data_size);

  int GetOffsetInFile(const std::string& key,
                      int offset,
                      int stream_index) const;
  int GetEOFOffsetInFile(const std::string& key, int stream_index) const;
  int GetLastEOFOffsetInFile(const std::string& key, int file_index) const;
  int GetFileSize(const std::string& key, int file_index) const;

  base::Time last_used() const { return last_used_; }
  base::Time last_modified() const { return last_modified_; }
  void set_last_used(base::Time last_used) { last_used_ = last_used; }
  void set_last_modified(base::Time last_modified) {
    last_modified_ = last_modified;
  }

  int32 data_size(int stream_index) const { return data_size_[stream_index]; }
  void set_data_size(int stream_index, int data_size) {
    data_size_[stream_index] = data_size;
  }

  int32 sparse_data_size() const { return sparse_data_size_; }
  void set_sparse_data_size(int32 sparse_data_size) {
    sparse_data_size_ = sparse_data_size;
  }

 private:
  base::Time last_used_;
  base::Time last_modified_;
  int32 data_size_[kSimpleEntryStreamCount];
  int32 sparse_data_size_;
};

struct SimpleEntryCreationResults {
  explicit SimpleEntryCreationResults(SimpleEntryStat entry_stat);
  ~SimpleEntryCreationResults();

  SimpleSynchronousEntry* sync_entry;
  scoped_refptr<net::GrowableIOBuffer> stream_0_data;
  SimpleEntryStat entry_stat;
  uint32 stream_0_crc32;
  int result;
};

class SimpleSynchronousEntry {
 public:
  struct CRCRecord {
    CRCRecord();
    CRCRecord(int index_p, bool has_crc32_p, uint32 data_crc32_p);

    int index;
    bool has_crc32;
    uint32 data_crc32;
  };

  struct EntryOperationData {
    EntryOperationData(int index_p, int offset_p, int buf_len_p);
    EntryOperationData(int index_p,
                       int offset_p,
                       int buf_len_p,
                       bool truncate_p,
                       bool doomed_p);
    EntryOperationData(int64 sparse_offset_p, int buf_len_p);

    int index;
    int offset;
    int64 sparse_offset;
    int buf_len;
    bool truncate;
    bool doomed;
  };

  static void OpenEntry(net::CacheType cache_type,
                        const base::FilePath& path,
                        uint64 entry_hash,
                        bool had_index,
                        SimpleEntryCreationResults* out_results);

  static void CreateEntry(net::CacheType cache_type,
                          const base::FilePath& path,
                          const std::string& key,
                          uint64 entry_hash,
                          bool had_index,
                          SimpleEntryCreationResults* out_results);

  
  
  
  static int DoomEntry(const base::FilePath& path,
                       uint64 entry_hash);

  
  
  
  static int DoomEntrySet(const std::vector<uint64>* key_hashes,
                          const base::FilePath& path);

  
  void ReadData(const EntryOperationData& in_entry_op,
                net::IOBuffer* out_buf,
                uint32* out_crc32,
                SimpleEntryStat* entry_stat,
                int* out_result) const;
  void WriteData(const EntryOperationData& in_entry_op,
                 net::IOBuffer* in_buf,
                 SimpleEntryStat* out_entry_stat,
                 int* out_result);
  void CheckEOFRecord(int index,
                      const SimpleEntryStat& entry_stat,
                      uint32 expected_crc32,
                      int* out_result) const;

  void ReadSparseData(const EntryOperationData& in_entry_op,
                      net::IOBuffer* out_buf,
                      base::Time* out_last_used,
                      int* out_result);
  void WriteSparseData(const EntryOperationData& in_entry_op,
                       net::IOBuffer* in_buf,
                       int64 max_sparse_data_size,
                       SimpleEntryStat* out_entry_stat,
                       int* out_result);
  void GetAvailableRange(const EntryOperationData& in_entry_op,
                         int64* out_start,
                         int* out_result);

  
  
  void Close(const SimpleEntryStat& entry_stat,
             scoped_ptr<std::vector<CRCRecord> > crc32s_to_write,
             net::GrowableIOBuffer* stream_0_data);

  const base::FilePath& path() const { return path_; }
  std::string key() const { return key_; }

 private:
  enum CreateEntryResult {
    CREATE_ENTRY_SUCCESS = 0,
    CREATE_ENTRY_PLATFORM_FILE_ERROR = 1,
    CREATE_ENTRY_CANT_WRITE_HEADER = 2,
    CREATE_ENTRY_CANT_WRITE_KEY = 3,
    CREATE_ENTRY_MAX = 4,
  };

  enum FileRequired {
    FILE_NOT_REQUIRED,
    FILE_REQUIRED
  };

  struct SparseRange {
    int64 offset;
    int64 length;
    uint32 data_crc32;
    int64 file_offset;

    bool operator<(const SparseRange& other) const {
      return offset < other.offset;
    }
  };

  SimpleSynchronousEntry(
      net::CacheType cache_type,
      const base::FilePath& path,
      const std::string& key,
      uint64 entry_hash);

  
  
  ~SimpleSynchronousEntry();

  
  
  
  bool MaybeOpenFile(int file_index,
                     base::PlatformFileError* out_error);
  
  
  
  bool MaybeCreateFile(int file_index,
                       FileRequired file_required,
                       base::PlatformFileError* out_error);
  bool OpenFiles(bool had_index,
                 SimpleEntryStat* out_entry_stat);
  bool CreateFiles(bool had_index,
                   SimpleEntryStat* out_entry_stat);
  void CloseFile(int index);
  void CloseFiles();

  
  
  
  int InitializeForOpen(bool had_index,
                        SimpleEntryStat* out_entry_stat,
                        scoped_refptr<net::GrowableIOBuffer>* stream_0_data,
                        uint32* out_stream_0_crc32);

  
  
  
  bool InitializeCreatedFile(int index, CreateEntryResult* out_result);

  
  
  
  
  int InitializeForCreate(bool had_index, SimpleEntryStat* out_entry_stat);

  
  
  int ReadAndValidateStream0(
      int total_data_size,
      SimpleEntryStat* out_entry_stat,
      scoped_refptr<net::GrowableIOBuffer>* stream_0_data,
      uint32* out_stream_0_crc32) const;

  int GetEOFRecordData(int index,
                       const SimpleEntryStat& entry_stat,
                       bool* out_has_crc32,
                       uint32* out_crc32,
                       int* out_data_size) const;
  void Doom() const;

  
  bool OpenSparseFileIfExists(int32* out_sparse_data_size);

  
  bool CreateSparseFile();

  
  bool CloseSparseFile();

  
  bool InitializeSparseFile();

  
  bool TruncateSparseFile();

  
  
  
  bool ScanSparseFile(int32* out_sparse_data_size);

  
  
  bool ReadSparseRange(const SparseRange* range,
                       int offset, int len, char* buf);

  
  
  bool WriteSparseRange(SparseRange* range,
                        int offset, int len, const char* buf);

  
  bool AppendSparseRange(int64 offset, int len, const char* buf);

  static bool DeleteFileForEntryHash(const base::FilePath& path,
                                     uint64 entry_hash,
                                     int file_index);
  static bool DeleteFilesForEntryHash(const base::FilePath& path,
                                      uint64 entry_hash);

  void RecordSyncCreateResult(CreateEntryResult result, bool had_index);

  base::FilePath GetFilenameFromFileIndex(int file_index);

  bool sparse_file_open() const {
    return sparse_file_ != base::kInvalidPlatformFileValue;
  }

  const net::CacheType cache_type_;
  const base::FilePath path_;
  const uint64 entry_hash_;
  std::string key_;

  bool have_open_files_;
  bool initialized_;

  base::PlatformFile files_[kSimpleEntryFileCount];

  
  
  bool empty_file_omitted_[kSimpleEntryFileCount];

  typedef std::map<int64, SparseRange> SparseRangeOffsetMap;
  typedef SparseRangeOffsetMap::iterator SparseRangeIterator;
  SparseRangeOffsetMap sparse_ranges_;
  base::PlatformFile sparse_file_;
  
  // written).
  int64 sparse_tail_offset_;

  
  
  bool files_created_;
};

}  

#endif  
