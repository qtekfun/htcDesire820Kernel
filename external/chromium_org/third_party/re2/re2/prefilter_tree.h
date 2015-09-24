// Copyright 2009 The RE2 Authors.  All Rights Reserved.
// license that can be found in the LICENSE file.

#ifndef RE2_PREFILTER_TREE_H_
#define RE2_PREFILTER_TREE_H_

#include <map>

#include "util/util.h"
#include "util/sparse_array.h"

namespace re2 {

typedef SparseArray<int> IntMap;
typedef std::map<int, int> StdIntMap;

class Prefilter;

class PrefilterTree {
 public:
  PrefilterTree();
  ~PrefilterTree();

  
  
  
  void Add(Prefilter* prefilter);

  
  
  
  
  
  
  void Compile(vector<string>* atom_vec);

  
  
  
  
  
  void RegexpsGivenStrings(const vector<int>& matched_atoms,
                           vector<int>* regexps) const;

  
  
  void PrintPrefilter(int regexpid);


  
  
  struct Entry {
   public:
    
    
    
    int propagate_up_at_count;

    
    
    
    
    
    
    StdIntMap* parents;

    
    
    vector<int> regexps;
  };

 private:
  
  
  
  void AssignUniqueIds(vector<string>* atom_vec);

  
  void PropagateMatch(const vector<int>& atom_ids,
                      IntMap* regexps) const;

  
  
  Prefilter* CanonicalNode(Prefilter* node);

  
  
  string NodeString(Prefilter* node) const;

  
  string DebugNodeString(Prefilter* node) const;

  
  void PrintDebugInfo();

  
  
  vector<Entry> entries_;

  
  map<string, Prefilter*> node_map_;

  
  
  vector<int> unfiltered_;

  
  vector<Prefilter*> prefilter_vec_;

  
  vector<int> atom_index_to_id_;

  
  bool compiled_;

  DISALLOW_EVIL_CONSTRUCTORS(PrefilterTree);
};

}  

#endif  
