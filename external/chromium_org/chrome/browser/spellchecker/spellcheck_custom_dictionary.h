// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROME_BROWSER_SPELLCHECKER_SPELLCHECK_CUSTOM_DICTIONARY_H_
#define CHROME_BROWSER_SPELLCHECKER_SPELLCHECK_CUSTOM_DICTIONARY_H_

#include <string>

#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "base/memory/weak_ptr.h"
#include "base/observer_list.h"
#include "chrome/browser/spellchecker/spellcheck_dictionary.h"
#include "chrome/common/spellcheck_common.h"
#include "sync/api/syncable_service.h"

class SpellcheckCustomDictionary : public SpellcheckDictionary,
                                   public syncer::SyncableService {
 public:
  
  class Change {
   public:
    Change();
    Change(const Change& other);
    explicit Change(const chrome::spellcheck_common::WordList& to_add);
    ~Change();

    
    void AddWord(const std::string& word);

    
    void RemoveWord(const std::string& word);

    
    
    
    
    int Sanitize(const chrome::spellcheck_common::WordSet& words);

    
    const chrome::spellcheck_common::WordList& to_add() const;

    
    const chrome::spellcheck_common::WordList& to_remove() const;

    
    bool empty() const;

   private:
    
    chrome::spellcheck_common::WordList to_add_;

    
    chrome::spellcheck_common::WordList to_remove_;
  };

  
  class Observer {
   public:
    
    virtual void OnCustomDictionaryLoaded() = 0;

    
    virtual void OnCustomDictionaryChanged(const Change& dictionary_change) = 0;
  };

  explicit SpellcheckCustomDictionary(const base::FilePath& path);
  virtual ~SpellcheckCustomDictionary();

  
  const chrome::spellcheck_common::WordSet& GetWords() const;

  
  
  
  bool AddWord(const std::string& word);

  
  
  
  bool RemoveWord(const std::string& word);

  
  bool HasWord(const std::string& word) const;

  
  void AddObserver(Observer* observer);

  
  void RemoveObserver(Observer* observer);

  
  bool IsLoaded();

  
  bool IsSyncing();

  
  virtual void Load() OVERRIDE;

  
  virtual syncer::SyncMergeResult MergeDataAndStartSyncing(
      syncer::ModelType type,
      const syncer::SyncDataList& initial_sync_data,
      scoped_ptr<syncer::SyncChangeProcessor> sync_processor,
      scoped_ptr<syncer::SyncErrorFactory> sync_error_handler) OVERRIDE;
  virtual void StopSyncing(syncer::ModelType type) OVERRIDE;
  virtual syncer::SyncDataList GetAllSyncData(
      syncer::ModelType type) const OVERRIDE;
  virtual syncer::SyncError ProcessSyncChanges(
      const tracked_objects::Location& from_here,
      const syncer::SyncChangeList& change_list) OVERRIDE;

 private:
  friend class DictionarySyncIntegrationTestHelper;
  friend class SpellcheckCustomDictionaryTest;

  
  
  
  static chrome::spellcheck_common::WordList LoadDictionaryFile(
      const base::FilePath& path);

  
  
  static void UpdateDictionaryFile(
      const Change& dictionary_change,
      const base::FilePath& path);

  
  
  
  
  void OnLoaded(chrome::spellcheck_common::WordList custom_words);

  
  
  void Apply(const Change& dictionary_change);

  
  
  void Save(const Change& dictionary_change);

  
  
  
  syncer::SyncError Sync(const Change& dictionary_change);

  
  
  void Notify(const Change& dictionary_change);

  
  chrome::spellcheck_common::WordSet words_;

  
  base::FilePath custom_dictionary_path_;

  
  ObserverList<Observer> observers_;

  
  scoped_ptr<syncer::SyncChangeProcessor> sync_processor_;

  
  scoped_ptr<syncer::SyncErrorFactory> sync_error_handler_;

  
  bool is_loaded_;

  
  base::WeakPtrFactory<SpellcheckCustomDictionary> weak_ptr_factory_;

  DISALLOW_COPY_AND_ASSIGN(SpellcheckCustomDictionary);
};

#endif  
