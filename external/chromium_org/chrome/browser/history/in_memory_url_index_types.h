// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_HISTORY_IN_MEMORY_URL_INDEX_TYPES_H_
#define CHROME_BROWSER_HISTORY_IN_MEMORY_URL_INDEX_TYPES_H_

#include <map>
#include <set>
#include <vector>

#include "base/strings/string16.h"
#include "chrome/browser/autocomplete/history_provider_util.h"
#include "chrome/browser/history/history_types.h"
#include "url/gurl.h"

namespace history {

const size_t kMaxSignificantChars = 50;


struct TermMatch {
  TermMatch() : term_num(0), offset(0), length(0) {}
  TermMatch(int term_num, size_t offset, size_t length)
      : term_num(term_num),
        offset(offset),
        length(length) {}

  int term_num;  
  size_t offset;  
  size_t length;  
};
typedef std::vector<TermMatch> TermMatches;

base::string16 CleanUpUrlForMatching(const GURL& gurl,
                                     const std::string& languages);

base::string16 CleanUpTitleForMatching(const base::string16& title);

TermMatches MatchTermInString(const base::string16& term,
                              const base::string16& cleaned_string,
                              int term_num);

TermMatches SortAndDeoverlapMatches(const TermMatches& matches);

std::vector<size_t> OffsetsFromTermMatches(const TermMatches& matches);

TermMatches ReplaceOffsetsInTermMatches(const TermMatches& matches,
                                        const std::vector<size_t>& offsets);


typedef std::vector<base::string16> String16Vector;
typedef std::set<base::string16> String16Set;
typedef std::set<char16> Char16Set;
typedef std::vector<char16> Char16Vector;

typedef std::vector<size_t> WordStarts;


String16Set String16SetFromString16(const base::string16& cleaned_uni_string,
                                    WordStarts* word_starts);

String16Vector String16VectorFromString16(
    const base::string16& cleaned_uni_string,
    bool break_on_space,
    WordStarts* word_starts);

Char16Set Char16SetFromString16(const base::string16& uni_word);


typedef size_t WordID;

typedef std::map<base::string16, WordID> WordMap;

typedef std::set<WordID> WordIDSet;  
typedef std::map<char16, WordIDSet> CharWordIDMap;

typedef history::URLID HistoryID;
typedef std::set<HistoryID> HistoryIDSet;
typedef std::vector<HistoryID> HistoryIDVector;
typedef std::map<WordID, HistoryIDSet> WordIDHistoryMap;
typedef std::map<HistoryID, WordIDSet> HistoryIDWordMap;


typedef std::vector<VisitInfo> VisitInfoVector;
struct HistoryInfoMapValue {
  HistoryInfoMapValue();
  ~HistoryInfoMapValue();

  
  URLRow url_row;

  
  
  
  
  VisitInfoVector visits;
};

typedef std::map<HistoryID, HistoryInfoMapValue> HistoryInfoMap;

struct RowWordStarts {
  RowWordStarts();
  ~RowWordStarts();

  
  void Clear();

  WordStarts url_word_starts_;
  WordStarts title_word_starts_;
};
typedef std::map<HistoryID, RowWordStarts> WordStartsMap;

}  

#endif  
