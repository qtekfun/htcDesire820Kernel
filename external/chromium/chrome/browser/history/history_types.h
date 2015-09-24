// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_HISTORY_TYPES_H_
#define CHROME_BROWSER_HISTORY_HISTORY_TYPES_H_
#pragma once

#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/memory/ref_counted_memory.h"
#include "base/stack_container.h"
#include "base/string16.h"
#include "base/time.h"
#include "chrome/browser/history/snippet.h"
#include "chrome/browser/search_engines/template_url_id.h"
#include "chrome/common/ref_counted_util.h"
#include "chrome/common/thumbnail_score.h"
#include "content/common/page_transition_types.h"
#include "googleurl/src/gurl.h"

namespace history {

class HistoryBackend;
class URLDatabase;

typedef std::map<GURL, scoped_refptr<RefCountedVector<GURL> > > RedirectMap;

typedef std::vector<GURL> RedirectList;

typedef int64 StarID;  
typedef int64 UIStarID;  
typedef int64 DownloadID;   
typedef int64 FaviconID;  
typedef int64 SegmentID;  
typedef int64 IconMappingID; 


typedef int64 URLID;

class URLRow {
 public:
  URLRow();

  explicit URLRow(const GURL& url);

  
  
  URLRow(const GURL& url, URLID id);

  virtual ~URLRow();
  URLRow& operator=(const URLRow& other);

  URLID id() const { return id_; }
  const GURL& url() const { return url_; }

  const string16& title() const {
    return title_;
  }
  void set_title(const string16& title) {
    
    
    if (title != title_) {
      title_ = title;
    }
  }

  int visit_count() const {
    return visit_count_;
  }
  void set_visit_count(int visit_count) {
    visit_count_ = visit_count;
  }

  
  int typed_count() const {
    return typed_count_;
  }
  void set_typed_count(int typed_count) {
    typed_count_ = typed_count;
  }

  base::Time last_visit() const {
    return last_visit_;
  }
  void set_last_visit(base::Time last_visit) {
    last_visit_ = last_visit;
  }

  
  bool hidden() const {
    return hidden_;
  }
  void set_hidden(bool hidden) {
    hidden_ = hidden;
  }

 protected:
  
  
  void Swap(URLRow* other);

 private:
  
  
  friend class URLDatabase;
  friend class HistoryBackend;

  
  
  void Initialize();

  
  
  URLID id_;

  
  
  
  GURL url_;

  string16 title_;

  
  int visit_count_;

  
  int typed_count_;

  
  
  base::Time last_visit_;

  
  
  bool hidden_;

  
};

enum VisitSource {
  SOURCE_SYNCED = 0,         
  SOURCE_BROWSED = 1,        
  SOURCE_EXTENSION = 2,      
  SOURCE_FIREFOX_IMPORTED = 3,
  SOURCE_IE_IMPORTED = 4,
  SOURCE_SAFARI_IMPORTED = 5,
};

typedef int64 VisitID;
typedef std::map<VisitID, VisitSource> VisitSourceMap;


class VisitRow {
 public:
  VisitRow();
  VisitRow(URLID arg_url_id,
           base::Time arg_visit_time,
           VisitID arg_referring_visit,
           PageTransition::Type arg_transition,
           SegmentID arg_segment_id);
  ~VisitRow();

  
  VisitID visit_id;

  
  URLID url_id;

  base::Time visit_time;

  
  
  VisitID referring_visit;

  
  PageTransition::Type transition;

  
  
  SegmentID segment_id;

  
  
  
  
  
  bool is_indexed;

  
  bool operator<(const VisitRow& other) {
    return visit_time < other.visit_time;
  }

  
};

typedef std::vector<VisitRow> VisitVector;


struct ImportedFaviconUsage {
  ImportedFaviconUsage();
  ~ImportedFaviconUsage();

  
  GURL favicon_url;

  
  std::vector<unsigned char> png_data;

  
  std::set<GURL> urls;
};


struct PageVisit {
  URLID page_id;
  base::Time visit_time;
};


struct StarredEntry {
  enum Type {
    
    URL,

    
    BOOKMARK_BAR,

    
    USER_FOLDER,

    
    OTHER
  };

  StarredEntry();
  ~StarredEntry();

  void Swap(StarredEntry* other);

  
  StarID id;

  
  string16 title;

  
  base::Time date_added;

  
  
  UIStarID parent_folder_id;

  
  
  
  
  UIStarID folder_id;

  
  int visual_order;

  
  Type type;

  
  GURL url;

  
  
  URLID url_id;

  
  
  base::Time date_folder_modified;
};


class URLResult : public URLRow {
 public:
  URLResult();
  URLResult(const GURL& url, base::Time visit_time);
  
  
  URLResult(const GURL& url, const Snippet::MatchPositions& title_matches);
  ~URLResult();

  base::Time visit_time() const { return visit_time_; }
  void set_visit_time(base::Time visit_time) { visit_time_ = visit_time; }

  const Snippet& snippet() const { return snippet_; }

  
  
  
  const Snippet::MatchPositions& title_match_positions() const {
    return title_match_positions_;
  }

  void SwapResult(URLResult* other);

 private:
  friend class HistoryBackend;

  
  base::Time visit_time_;

  
  Snippet snippet_;
  Snippet::MatchPositions title_match_positions_;

  
};


class QueryResults {
 public:
  typedef std::vector<URLResult*> URLResultVector;

  QueryResults();
  ~QueryResults();

  
  
  
  
  
  
  
  
  
  
  
  
  base::Time first_time_searched() const { return first_time_searched_; }
  void set_first_time_searched(base::Time t) { first_time_searched_ = t; }
  

  void set_reached_beginning(bool reached) { reached_beginning_ = reached; }
  bool reached_beginning() { return reached_beginning_; }

  size_t size() const { return results_.size(); }
  bool empty() const { return results_.empty(); }

  URLResult& operator[](size_t i) { return *results_[i]; }
  const URLResult& operator[](size_t i) const { return *results_[i]; }

  URLResultVector::const_iterator begin() const { return results_.begin(); }
  URLResultVector::const_iterator end() const { return results_.end(); }
  URLResultVector::const_reverse_iterator rbegin() const {
    return results_.rbegin();
  }
  URLResultVector::const_reverse_iterator rend() const {
    return results_.rend();
  }

  
  
  
  
  
  
  
  const size_t* MatchesForURL(const GURL& url, size_t* num_matches) const;

  
  
  void Swap(QueryResults* other);

  
  
  
  void AppendURLBySwapping(URLResult* result);

  
  
  
  
  void AppendResultsBySwapping(QueryResults* other, bool remove_dupes);

  
  void DeleteURL(const GURL& url);

  
  void DeleteRange(size_t begin, size_t end);

 private:
  
  
  
  
  typedef std::map<GURL, StackVector<size_t, 4> > URLToResultIndices;

  
  
  void AddURLUsageAtIndex(const GURL& url, size_t index);

  
  
  void AdjustResultMap(size_t begin, size_t end, ptrdiff_t delta);

  base::Time first_time_searched_;

  
  bool reached_beginning_;

  
  
  URLResultVector results_;

  
  URLToResultIndices url_to_results_;

  DISALLOW_COPY_AND_ASSIGN(QueryResults);
};


struct QueryOptions {
  QueryOptions();

  
  
  
  
  
  
  
  
  
  
  
  
  base::Time begin_time;
  base::Time end_time;

  
  void SetRecentDayRange(int days_ago);

  
  
  
  int max_count;
};


struct KeywordSearchTermVisit {
  KeywordSearchTermVisit();
  ~KeywordSearchTermVisit();

  
  base::Time time;

  
  string16 term;
};


struct KeywordSearchTermRow {
  KeywordSearchTermRow();
  ~KeywordSearchTermRow();

  
  TemplateURLID keyword_id;

  
  URLID url_id;

  
  string16 term;
};


struct MostVisitedURL {
  MostVisitedURL();
  MostVisitedURL(const GURL& in_url,
                 const GURL& in_favicon_url,
                 const string16& in_title);
  ~MostVisitedURL();

  GURL url;
  GURL favicon_url;
  string16 title;

  RedirectList redirects;

  bool operator==(const MostVisitedURL& other) {
    return url == other.url;
  }
};


class HistoryAddPageArgs
    : public base::RefCountedThreadSafe<HistoryAddPageArgs> {
 public:
  HistoryAddPageArgs(const GURL& arg_url,
                     base::Time arg_time,
                     const void* arg_id_scope,
                     int32 arg_page_id,
                     const GURL& arg_referrer,
                     const history::RedirectList& arg_redirects,
                     PageTransition::Type arg_transition,
                     VisitSource arg_source,
                     bool arg_did_replace_entry);

  
  
  HistoryAddPageArgs* Clone() const;

  GURL url;
  base::Time time;

  const void* id_scope;
  int32 page_id;

  GURL referrer;
  history::RedirectList redirects;
  PageTransition::Type transition;
  VisitSource visit_source;
  bool did_replace_entry;

 private:
  friend class base::RefCountedThreadSafe<HistoryAddPageArgs>;

  ~HistoryAddPageArgs();

  DISALLOW_COPY_AND_ASSIGN(HistoryAddPageArgs);
};


typedef std::vector<MostVisitedURL> MostVisitedURLList;

struct Images {
  Images();
  ~Images();

  scoped_refptr<RefCountedBytes> thumbnail;
  ThumbnailScore thumbnail_score;

  
  
};

typedef std::vector<MostVisitedURL> MostVisitedURLList;

struct MostVisitedURLWithRank {
  MostVisitedURL url;
  int rank;
};

typedef std::vector<MostVisitedURLWithRank> MostVisitedURLWithRankList;

struct TopSitesDelta {
  TopSitesDelta();
  ~TopSitesDelta();

  MostVisitedURLList deleted;
  MostVisitedURLWithRankList added;
  MostVisitedURLWithRankList moved;
};

typedef std::map<GURL, scoped_refptr<RefCountedBytes> > URLToThumbnailMap;

struct ThumbnailMigration {
  ThumbnailMigration();
  ~ThumbnailMigration();

  MostVisitedURLList most_visited;
  URLToThumbnailMap url_to_thumbnail_map;
};

typedef std::map<GURL, Images> URLToImagesMap;

class MostVisitedThumbnails
    : public base::RefCountedThreadSafe<MostVisitedThumbnails> {
 public:
  MostVisitedThumbnails();
  virtual ~MostVisitedThumbnails();

  MostVisitedURLList most_visited;
  URLToImagesMap url_to_images_map;

 private:
  friend class base::RefCountedThreadSafe<MostVisitedThumbnails>;

  DISALLOW_COPY_AND_ASSIGN(MostVisitedThumbnails);
};


extern const int kLowQualityMatchTypedLimit;
extern const int kLowQualityMatchVisitLimit;
extern const int kLowQualityMatchAgeLimitInDays;

base::Time AutocompleteAgeThreshold();

bool RowQualifiesAsSignificant(const URLRow& row, const base::Time& threshold);

enum IconType {
  INVALID_ICON = 0x0,
  FAVICON = 1 << 0,
  TOUCH_ICON = 1 << 1,
  TOUCH_PRECOMPOSED_ICON = 1 << 2
};

struct IconMapping {
  IconMapping();
  ~IconMapping();

  
  IconMappingID mapping_id;

  
  GURL page_url;

  
  FaviconID icon_id;

  
  IconType icon_type;
};

struct FaviconData {
  FaviconData();
  ~FaviconData();

  
  bool is_valid();

  
  bool known_icon;

  
  scoped_refptr<RefCountedMemory> image_data;

  
  bool expired;

  
  GURL icon_url;

  
  history::IconType icon_type;
};

}  

#endif  
