// Copyright 2006 Google Inc.
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//      http://www.apache.org/licenses/LICENSE-2.0
// distributed under the License is distributed on an "AS IS" BASIS,
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef OPEN_VCDIFF_BLOCKHASH_H_
#define OPEN_VCDIFF_BLOCKHASH_H_

#include <config.h>
#include <stddef.h>  
#include <stdint.h>  
#include <vector>

namespace open_vcdiff {

class BlockHash {
 public:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static const int kBlockSize = 16;

  
  
  class Match {
   public:
    Match() : size_(0), source_offset_(-1), target_offset_(-1) { }

    void ReplaceIfBetterMatch(size_t candidate_size,
                              int candidate_source_offset,
                              int candidate_target_offset) {
      if (candidate_size > size_) {
        size_ = candidate_size;
        source_offset_ = candidate_source_offset;
        target_offset_ = candidate_target_offset;
      }
    }

    size_t size() const { return size_; }
    int source_offset() const { return source_offset_; }
    int target_offset() const { return target_offset_; }

   private:
     
    size_t size_;

    
    
    int source_offset_;

    
    
    int target_offset_;

    
    
    Match(const Match&);  
    void operator=(const Match&);
  };

  
  
  
  
  
  
  
  
  
  BlockHash(const char* source_data, size_t source_size, int starting_offset);

  ~BlockHash();

  
  
  
  
  
  
  
  
  
  
  
  
  
  bool Init(bool populate_hash_table);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static const BlockHash* CreateDictionaryHash(const char* dictionary_data,
                                               size_t dictionary_size);
  static BlockHash* CreateTargetHash(const char* target_data,
                                     size_t target_size,
                                     size_t dictionary_size);

  
  
  
  
  
  
  void AddOneIndexHash(int index, uint32_t hash_value) {
    if (index == NextIndexToAdd()) {
      AddBlock(hash_value);
    }
  }

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void AddAllBlocksThroughIndex(int end_index);

  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  void FindBestMatch(uint32_t hash_value,
                     const char* target_candidate_start,
                     const char* target_start,
                     size_t target_size,
                     Match* best_match) const;

 protected:
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  static const int kMaxMatchesToCheck = (kBlockSize >= 32) ? 32 :
                                            (32 * (32 / kBlockSize));

  
  
  static const int kMaxProbes = 16;

  
  
  
  
  static size_t CalcTableSize(const size_t dictionary_size);

  size_t GetNumberOfBlocks() const {
    return source_size_ / kBlockSize;
  }

  
  
  uint32_t GetHashTableIndex(uint32_t hash_value) const {
    return hash_value & hash_table_mask_;
  }

  
  
  int NextIndexToAdd() const {
    return (last_block_added_ + 1) * kBlockSize;
  }

  static inline bool TooManyMatches(int* match_counter);

  const char* source_data() { return source_data_; }
  size_t source_size() { return source_size_; }

  
  
  
  
  
  void AddBlock(uint32_t hash_value);

  
  
  
  
  void AddAllBlocks();

  
  
  
  static bool BlockContentsMatch(const char* block1, const char* block2);

  
  
  
  
  
  
  
  
  
  
  
  
  
  static bool BlockCompareWords(const char* block1, const char* block2);

  
  
  
  
  
  
  
  
  
  
  
  
  int FirstMatchingBlock(uint32_t hash_value, const char* block_ptr) const;

  
  
  
  
  int NextMatchingBlock(int block_number, const char* block_ptr) const;

  
  
  
  inline int FirstMatchingBlockInline(uint32_t hash_value,
                                      const char* block_ptr) const;

  
  
  
  
  
  
  int SkipNonMatchingBlocks(int block_number, const char* block_ptr) const;

  
  
  
  
  static int MatchingBytesToLeft(const char* source_match_start,
                                 const char* target_match_start,
                                 int max_bytes);

  
  
  
  
  static int MatchingBytesToRight(const char* source_match_end,
                                  const char* target_match_end,
                                  int max_bytes);

  
  
  
  friend class BlockHashTest;

 private:
  const char* const  source_data_;
  const size_t       source_size_;

  
  
  
  
  
  
  
  
  std::vector<int> hash_table_;

  
  
  
  std::vector<int> next_block_table_;

  
  
  
  
  
  
  
  
  
  
  
  std::vector<int> last_block_table_;

  
  
  uint32_t hash_table_mask_;

  
  
  
  
  
  
  
  
  
  
  const int starting_offset_;

  
  
  
  int last_block_added_;

  
  BlockHash(const BlockHash&);  
  void operator=(const BlockHash&);
};

}  

#endif  
