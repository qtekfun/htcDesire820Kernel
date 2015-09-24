// Copyright 2008 Google Inc.  All rights reserved.
//     * Redistributions of source code must retain the above copyright
// copyright notice, this list of conditions and the following disclaimer
// this software without specific prior written permission.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT


#ifndef GOOGLE_PROTOBUF_COMPILER_IMPORTER_H__
#define GOOGLE_PROTOBUF_COMPILER_IMPORTER_H__

#include <string>
#include <vector>
#include <set>
#include <utility>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/descriptor_database.h>
#include <google/protobuf/compiler/parser.h>

namespace google {
namespace protobuf {

namespace io { class ZeroCopyInputStream; }

namespace compiler {

class Importer;
class MultiFileErrorCollector;
class SourceTree;
class DiskSourceTree;


class LIBPROTOBUF_EXPORT SourceTreeDescriptorDatabase : public DescriptorDatabase {
 public:
  SourceTreeDescriptorDatabase(SourceTree* source_tree);
  ~SourceTreeDescriptorDatabase();

  
  
  
  
  void RecordErrorsTo(MultiFileErrorCollector* error_collector) {
    error_collector_ = error_collector;
  }

  
  
  
  
  DescriptorPool::ErrorCollector* GetValidationErrorCollector() {
    using_validation_error_collector_ = true;
    return &validation_error_collector_;
  }

  
  bool FindFileByName(const string& filename, FileDescriptorProto* output);
  bool FindFileContainingSymbol(const string& symbol_name,
                                FileDescriptorProto* output);
  bool FindFileContainingExtension(const string& containing_type,
                                   int field_number,
                                   FileDescriptorProto* output);

 private:
  class SingleFileErrorCollector;

  SourceTree* source_tree_;
  MultiFileErrorCollector* error_collector_;

  class LIBPROTOBUF_EXPORT ValidationErrorCollector : public DescriptorPool::ErrorCollector {
   public:
    ValidationErrorCollector(SourceTreeDescriptorDatabase* owner);
    ~ValidationErrorCollector();

    
    void AddError(const string& filename,
                  const string& element_name,
                  const Message* descriptor,
                  ErrorLocation location,
                  const string& message);

   private:
    SourceTreeDescriptorDatabase* owner_;
  };
  friend class ValidationErrorCollector;

  bool using_validation_error_collector_;
  SourceLocationTable source_locations_;
  ValidationErrorCollector validation_error_collector_;
};

class LIBPROTOBUF_EXPORT Importer {
 public:
  Importer(SourceTree* source_tree,
           MultiFileErrorCollector* error_collector);
  ~Importer();

  
  
  
  
  
  
  
  
  
  
  
  
  
  const FileDescriptor* Import(const string& filename);

  
  
  inline const DescriptorPool* pool() const {
    return &pool_;
  }

 private:
  SourceTreeDescriptorDatabase database_;
  DescriptorPool pool_;

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(Importer);
};

class LIBPROTOBUF_EXPORT MultiFileErrorCollector {
 public:
  inline MultiFileErrorCollector() {}
  virtual ~MultiFileErrorCollector();

  
  
  virtual void AddError(const string& filename, int line, int column,
                        const string& message) = 0;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(MultiFileErrorCollector);
};

class LIBPROTOBUF_EXPORT SourceTree {
 public:
  inline SourceTree() {}
  virtual ~SourceTree();

  
  
  
  
  virtual io::ZeroCopyInputStream* Open(const string& filename) = 0;

 private:
  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(SourceTree);
};

class LIBPROTOBUF_EXPORT DiskSourceTree : public SourceTree {
 public:
  DiskSourceTree();
  ~DiskSourceTree();

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void MapPath(const string& virtual_path, const string& disk_path);

  
  enum DiskFileToVirtualFileResult {
    SUCCESS,
    SHADOWED,
    CANNOT_OPEN,
    NO_MAPPING
  };

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  DiskFileToVirtualFileResult
    DiskFileToVirtualFile(const string& disk_file,
                          string* virtual_file,
                          string* shadowing_disk_file);

  
  
  
  bool VirtualFileToDiskFile(const string& virtual_file, string* disk_file);

  
  io::ZeroCopyInputStream* Open(const string& filename);

 private:
  struct Mapping {
    string virtual_path;
    string disk_path;

    inline Mapping(const string& virtual_path_param,
                   const string& disk_path_param)
      : virtual_path(virtual_path_param), disk_path(disk_path_param) {}
  };
  vector<Mapping> mappings_;

  
  
  io::ZeroCopyInputStream* OpenVirtualFile(const string& virtual_file,
                                           string* disk_file);

  
  io::ZeroCopyInputStream* OpenDiskFile(const string& filename);

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(DiskSourceTree);
};

}  
}  

}  
#endif  
