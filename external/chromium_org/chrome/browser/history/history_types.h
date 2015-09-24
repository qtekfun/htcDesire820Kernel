// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_HISTORY_TYPES_H_
#define CHROME_BROWSER_HISTORY_HISTORY_TYPES_H_

#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/containers/stack_container.h"
#include "base/memory/ref_counted_memory.h"
#include "base/memory/scoped_vector.h"
#include "base/strings/string16.h"
#include "base/time/time.h"
#include "chrome/browser/history/snippet.h"
#include "chrome/browser/search_engines/template_url_id.h"
#include "chrome/common/favicon/favicon_types.h"
#include "chrome/common/ref_counted_util.h"
#include "chrome/common/thumbnail_score.h"
#include "content/public/common/page_transition_types.h"
#include "ui/gfx/image/image.h"
#include "ui/gfx/size.h"
#include "url/gurl.h"

class PageUsageData;

namespace history {

class HistoryBackend;
class URLDatabase;

typedef std::map<GURL, scoped_refptr<RefCountedVector<GURL> > > RedirectMap;

typedef std::vector<GURL> RedirectList;

typedef int64 FaviconBitmapID; 
typedef int64 SegmentID;  
typedef int64 SegmentDurationID;  
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

  
  
  
  
  void set_id(URLID id) { id_ = id; }

  const GURL& url() const { return url_; }

  const base::string16& title() const {
    return title_;
  }
  void set_title(const base::string16& title) {
    
    
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

  
  class URLRowHasURL {
   public:
    explicit URLRowHasURL(const GURL& url) : url_(url) {}

    bool operator()(const URLRow& row) {
      return row.url() == url_;
    }

   private:
    const GURL& url_;
  };

 protected:
  
  
  void Swap(URLRow* other);

 private:
  
  
  friend class URLDatabase;
  friend class HistoryBackend;

  
  
  void Initialize();

  
  
  
  URLID id_;

  
  
  
  GURL url_;

  base::string16 title_;

  
  int visit_count_;

  
  int typed_count_;

  
  
  base::Time last_visit_;

  
  
  bool hidden_;

  
};
typedef std::vector<URLRow> URLRows;

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
           content::PageTransition arg_transition,
           SegmentID arg_segment_id);
  ~VisitRow();

  
  VisitID visit_id;

  
  URLID url_id;

  base::Time visit_time;

  
  
  VisitID referring_visit;

  
  content::PageTransition transition;

  
  
  SegmentID segment_id;

  
  
  
  
  base::TimeDelta visit_duration;

  
  bool operator<(const VisitRow& other) {
    return visit_time < other.visit_time;
  }

  
};

typedef std::vector<VisitRow> VisitVector;

typedef std::pair<base::Time, content::PageTransition> VisitInfo;


struct PageVisit {
  URLID page_id;
  base::Time visit_time;
};


class URLResult : public URLRow {
 public:
  URLResult();
  URLResult(const GURL& url, base::Time visit_time);
  
  
  URLResult(const GURL& url, const Snippet::MatchPositions& title_matches);
  explicit URLResult(const URLRow& url_row);
  virtual ~URLResult();

  base::Time visit_time() const { return visit_time_; }
  void set_visit_time(base::Time visit_time) { visit_time_ = visit_time; }

  const Snippet& snippet() const { return snippet_; }

  bool blocked_visit() const { return blocked_visit_; }
  void set_blocked_visit(bool blocked_visit) {
    blocked_visit_ = blocked_visit;
  }

  
  
  
  const Snippet::MatchPositions& title_match_positions() const {
    return title_match_positions_;
  }

  void SwapResult(URLResult* other);

  static bool CompareVisitTime(const URLResult& lhs, const URLResult& rhs);

 private:
  friend class HistoryBackend;

  
  base::Time visit_time_;

  
  Snippet snippet_;
  Snippet::MatchPositions title_match_positions_;

  
  bool blocked_visit_;

  
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

  URLResult& back() { return *results_.back(); }
  const URLResult& back() const { return *results_.back(); }

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

  
  void DeleteURL(const GURL& url);

  
  void DeleteRange(size_t begin, size_t end);

 private:
  
  
  
  
  typedef std::map<GURL, base::StackVector<size_t, 4> > URLToResultIndices;

  
  
  void AddURLUsageAtIndex(const GURL& url, size_t index);

  
  
  void AdjustResultMap(size_t begin, size_t end, ptrdiff_t delta);

  base::Time first_time_searched_;

  
  bool reached_beginning_;

  
  
  ScopedVector<URLResult> results_;

  
  URLToResultIndices url_to_results_;

  DISALLOW_COPY_AND_ASSIGN(QueryResults);
};


struct QueryOptions {
  QueryOptions();

  
  
  
  
  
  
  
  
  base::Time begin_time;
  base::Time end_time;

  
  void SetRecentDayRange(int days_ago);

  
  
  
  int max_count;

  enum DuplicateHandling {
    
    
    REMOVE_ALL_DUPLICATES,

    
    
    
    REMOVE_DUPLICATES_PER_DAY,

    
    KEEP_ALL_DUPLICATES
  };

  
  
  DuplicateHandling duplicate_policy;

  
  
  int EffectiveMaxCount() const;
  int64 EffectiveBeginTime() const;
  int64 EffectiveEndTime() const;
};


struct KeywordSearchTermVisit {
  KeywordSearchTermVisit();
  ~KeywordSearchTermVisit();

  base::string16 term;    
  int visits;       
  base::Time time;  
};


struct KeywordSearchTermRow {
  KeywordSearchTermRow();
  ~KeywordSearchTermRow();

  TemplateURLID keyword_id;  
  URLID url_id;              
  base::string16 term;             
};


struct MostVisitedURL {
  MostVisitedURL();
  MostVisitedURL(const GURL& url, const base::string16& title);
  MostVisitedURL(const GURL& url,
                 const base::string16& title,
                 const base::Time& last_forced_time);
  ~MostVisitedURL();

  GURL url;
  base::string16 title;

  
  
  
  base::Time last_forced_time;

  RedirectList redirects;

  bool operator==(const MostVisitedURL& other) {
    return url == other.url;
  }
};


struct FilteredURL {
  struct ExtendedInfo {
    ExtendedInfo();
    
    unsigned int total_visits;
    
    unsigned int visits;
    
    int64 duration_opened;
    
    base::Time last_visit_time;
  };

  FilteredURL();
  explicit FilteredURL(const PageUsageData& data);
  ~FilteredURL();

  GURL url;
  base::string16 title;
  double score;
  ExtendedInfo extended_info;
};


struct HistoryAddPageArgs {
  
  
  
  
  
  
  HistoryAddPageArgs();
  HistoryAddPageArgs(const GURL& url,
                     base::Time time,
                     const void* id_scope,
                     int32 page_id,
                     const GURL& referrer,
                     const history::RedirectList& redirects,
                     content::PageTransition transition,
                     VisitSource source,
                     bool did_replace_entry);
  ~HistoryAddPageArgs();

  GURL url;
  base::Time time;

  const void* id_scope;
  int32 page_id;

  GURL referrer;
  history::RedirectList redirects;
  content::PageTransition transition;
  VisitSource visit_source;
  bool did_replace_entry;
};


typedef std::vector<MostVisitedURL> MostVisitedURLList;
typedef std::vector<FilteredURL> FilteredURLList;

struct Images {
  Images();
  ~Images();

  scoped_refptr<base::RefCountedMemory> thumbnail;
  ThumbnailScore thumbnail_score;

  
  
};

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

typedef std::map<GURL, scoped_refptr<base::RefCountedBytes> > URLToThumbnailMap;

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

  MostVisitedURLList most_visited;
  URLToImagesMap url_to_images_map;

 private:
  friend class base::RefCountedThreadSafe<MostVisitedThumbnails>;
  virtual ~MostVisitedThumbnails();

  DISALLOW_COPY_AND_ASSIGN(MostVisitedThumbnails);
};


extern const int kLowQualityMatchTypedLimit;
extern const int kLowQualityMatchVisitLimit;
extern const int kLowQualityMatchAgeLimitInDays;

base::Time AutocompleteAgeThreshold();

bool RowQualifiesAsSignificant(const URLRow& row, const base::Time& threshold);


struct IconMapping {
  IconMapping();
  ~IconMapping();

  
  IconMappingID mapping_id;

  
  GURL page_url;

  
  chrome::FaviconID icon_id;

  
  GURL icon_url;

  
  chrome::IconType icon_type;
};

struct FaviconBitmapIDSize {
  FaviconBitmapIDSize();
  ~FaviconBitmapIDSize();

  
  FaviconBitmapID bitmap_id;

  
  gfx::Size pixel_size;
};

struct FaviconBitmap {
  FaviconBitmap();
  ~FaviconBitmap();

  
  FaviconBitmapID bitmap_id;

  
  chrome::FaviconID icon_id;

  
  base::Time last_updated;

  
  scoped_refptr<base::RefCountedMemory> bitmap_data;

  
  gfx::Size pixel_size;
};

struct BriefVisitInfo {
  URLID url_id;
  base::Time time;
  content::PageTransition transition;
};

class VisitDatabaseObserver {
 public:
  virtual ~VisitDatabaseObserver();
  virtual void OnAddVisit(const BriefVisitInfo& info) = 0;
};

struct ExpireHistoryArgs {
  ExpireHistoryArgs();
  ~ExpireHistoryArgs();

  
  
  void SetTimeRangeForOneDay(base::Time time);

  std::set<GURL> urls;
  base::Time begin_time;
  base::Time end_time;
};

}  

#endif  
