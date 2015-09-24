// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef EXTENSIONS_COMMON_USER_SCRIPT_H_
#define EXTENSIONS_COMMON_USER_SCRIPT_H_

#include <string>
#include <vector>

#include "base/files/file_path.h"
#include "base/strings/string_piece.h"
#include "extensions/common/url_pattern.h"
#include "extensions/common/url_pattern_set.h"
#include "url/gurl.h"

class Pickle;
class PickleIterator;

namespace extensions {

class UserScript {
 public:
  
  static const char kFileExtension[];

  
  
  static bool IsURLUserScript(const GURL& url, const std::string& mime_type);

  
  
  static int ValidUserScriptSchemes(bool canExecuteScriptEverywhere = false);

  
  enum RunLocation {
    UNDEFINED,
    DOCUMENT_START,  
                     
    DOCUMENT_END,  
                   
    DOCUMENT_IDLE,  
                    
                    
                    
    RUN_LOCATION_LAST  
  };

  
  class File {
   public:
    File(const base::FilePath& extension_root,
         const base::FilePath& relative_path,
         const GURL& url);
    File();
    ~File();

    const base::FilePath& extension_root() const { return extension_root_; }
    const base::FilePath& relative_path() const { return relative_path_; }

    const GURL& url() const { return url_; }
    void set_url(const GURL& url) { url_ = url; }

    
    
    const base::StringPiece GetContent() const {
      if (external_content_.data())
        return external_content_;
      else
        return content_;
    }
    void set_external_content(const base::StringPiece& content) {
      external_content_ = content;
    }
    void set_content(const base::StringPiece& content) {
      content_.assign(content.begin(), content.end());
    }

    
    
    void Pickle(::Pickle* pickle) const;
    void Unpickle(const ::Pickle& pickle, PickleIterator* iter);

   private:
    
    
    base::FilePath extension_root_;
    base::FilePath relative_path_;

    
    GURL url_;

    
    
    base::StringPiece external_content_;

    
    std::string content_;
  };

  typedef std::vector<File> FileList;

  
  
  UserScript();
  ~UserScript();

  const std::string& name_space() const { return name_space_; }
  void set_name_space(const std::string& name_space) {
    name_space_ = name_space;
  }

  const std::string& name() const { return name_; }
  void set_name(const std::string& name) { name_ = name; }

  const std::string& version() const { return version_; }
  void set_version(const std::string& version) {
    version_ = version;
  }

  const std::string& description() const { return description_; }
  void set_description(const std::string& description) {
    description_ = description;
  }

  
  RunLocation run_location() const { return run_location_; }
  void set_run_location(RunLocation location) { run_location_ = location; }

  
  bool emulate_greasemonkey() const { return emulate_greasemonkey_; }
  void set_emulate_greasemonkey(bool val) { emulate_greasemonkey_ = val; }

  
  bool match_all_frames() const { return match_all_frames_; }
  void set_match_all_frames(bool val) { match_all_frames_ = val; }

  
  
  const std::vector<std::string>& globs() const { return globs_; }
  void add_glob(const std::string& glob) { globs_.push_back(glob); }
  void clear_globs() { globs_.clear(); }
  const std::vector<std::string>& exclude_globs() const {
    return exclude_globs_;
  }
  void add_exclude_glob(const std::string& glob) {
    exclude_globs_.push_back(glob);
  }
  void clear_exclude_globs() { exclude_globs_.clear(); }

  
  
  const URLPatternSet& url_patterns() const { return url_set_; }
  void add_url_pattern(const URLPattern& pattern);
  const URLPatternSet& exclude_url_patterns() const {
    return exclude_url_set_;
  }
  void add_exclude_url_pattern(const URLPattern& pattern);

  
  FileList& js_scripts() { return js_scripts_; }
  const FileList& js_scripts() const { return js_scripts_; }

  
  FileList& css_scripts() { return css_scripts_; }
  const FileList& css_scripts() const { return css_scripts_; }

  const std::string& extension_id() const { return extension_id_; }
  void set_extension_id(const std::string& id) { extension_id_ = id; }

  bool is_incognito_enabled() const { return incognito_enabled_; }
  void set_incognito_enabled(bool enabled) { incognito_enabled_ = enabled; }

  bool is_standalone() const { return extension_id_.empty(); }

  
  
  bool MatchesURL(const GURL& url) const;

  
  
  void Pickle(::Pickle* pickle) const;

  
  
  
  void Unpickle(const ::Pickle& pickle, PickleIterator* iter);

 private:
  
  void PickleGlobs(::Pickle* pickle,
                   const std::vector<std::string>& globs) const;
  void PickleURLPatternSet(::Pickle* pickle,
                           const URLPatternSet& pattern_list) const;
  void PickleScripts(::Pickle* pickle, const FileList& scripts) const;

  
  void UnpickleGlobs(const ::Pickle& pickle, PickleIterator* iter,
                     std::vector<std::string>* globs);
  void UnpickleURLPatternSet(const ::Pickle& pickle, PickleIterator* iter,
                             URLPatternSet* pattern_list);
  void UnpickleScripts(const ::Pickle& pickle, PickleIterator* iter,
                       FileList* scripts);

  
  RunLocation run_location_;

  
  
  std::string name_space_;

  
  std::string name_;

  
  std::string description_;

  
  
  std::string version_;

  
  
  std::vector<std::string> globs_;
  std::vector<std::string> exclude_globs_;

  
  
  URLPatternSet url_set_;
  URLPatternSet exclude_url_set_;

  
  FileList js_scripts_;

  
  FileList css_scripts_;

  
  
  std::string extension_id_;

  
  
  bool emulate_greasemonkey_;

  
  
  bool match_all_frames_;

  
  bool incognito_enabled_;
};

typedef std::vector<UserScript> UserScriptList;

}  

#endif  
