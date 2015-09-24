// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GPU_COMMAND_BUFFER_SERVICE_QUERY_MANAGER_H_
#define GPU_COMMAND_BUFFER_SERVICE_QUERY_MANAGER_H_

#include <deque>
#include <vector>
#include "base/basictypes.h"
#include "base/containers/hash_tables.h"
#include "base/logging.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "gpu/command_buffer/service/feature_info.h"
#include "gpu/command_buffer/service/gl_utils.h"
#include "gpu/gpu_export.h"

namespace gpu {

class GLES2Decoder;

namespace gles2 {

class FeatureInfo;

class GPU_EXPORT QueryManager {
 public:
  class GPU_EXPORT Query : public base::RefCounted<Query> {
   public:
    Query(
        QueryManager* manager, GLenum target, int32 shm_id, uint32 shm_offset);

    GLenum target() const {
      return target_;
    }

    bool IsDeleted() const {
      return deleted_;
    }

    bool IsValid() const {
      return target() && !IsDeleted();
    }

    bool pending() const {
      return pending_;
    }

    int32 shm_id() const {
      return shm_id_;
    }

    uint32 shm_offset() const {
      return shm_offset_;
    }

    
    virtual bool Begin() = 0;

    
    virtual bool End(uint32 submit_count) = 0;

    
    virtual bool Process() = 0;

    virtual void Destroy(bool have_context) = 0;

    void AddCallback(base::Closure callback);

   protected:
    virtual ~Query();

    QueryManager* manager() const {
      return manager_;
    }

    void MarkAsDeleted() {
      deleted_ = true;
    }

    
    bool MarkAsCompleted(uint64 result);

    void MarkAsPending(uint32 submit_count) {
      DCHECK(!pending_);
      pending_ = true;
      submit_count_ = submit_count;
    }

    void UnmarkAsPending() {
      DCHECK(pending_);
      pending_ = false;
    }

    
    bool AddToPendingQueue(uint32 submit_count) {
      return manager_->AddPendingQuery(this, submit_count);
    }

    
    bool AddToPendingTransferQueue(uint32 submit_count) {
      return manager_->AddPendingTransferQuery(this, submit_count);
    }

    void BeginQueryHelper(GLenum target, GLuint id) {
      manager_->BeginQueryHelper(target, id);
    }

    void EndQueryHelper(GLenum target) {
      manager_->EndQueryHelper(target);
    }

    uint32 submit_count() const {
      return submit_count_;
    }

   private:
    friend class QueryManager;
    friend class QueryManagerTest;
    friend class base::RefCounted<Query>;

    void RunCallbacks();

    
    QueryManager* manager_;

    
    GLenum target_;

    
    int32 shm_id_;
    uint32 shm_offset_;

    
    uint32 submit_count_;

    
    bool pending_;

    
    bool deleted_;

    
    std::vector<base::Closure> callbacks_;
  };

  QueryManager(
      GLES2Decoder* decoder,
      FeatureInfo* feature_info);
  ~QueryManager();

  
  void Destroy(bool have_context);

  
  Query* CreateQuery(
      GLenum target, GLuint client_id, int32 shm_id, uint32 shm_offset);

  
  Query* GetQuery(GLuint client_id);

  
  void RemoveQuery(GLuint client_id);

  
  bool BeginQuery(Query* query);

  
  bool EndQuery(Query* query, uint32 submit_count);

  
  
  bool ProcessPendingQueries();

  
  bool HavePendingQueries();

  
  
  bool ProcessPendingTransferQueries();

  
  bool HavePendingTransferQueries();

  GLES2Decoder* decoder() const {
    return decoder_;
  }

 private:
  void StartTracking(Query* query);
  void StopTracking(Query* query);

  
  
  void BeginQueryHelper(GLenum target, GLuint id);
  void EndQueryHelper(GLenum target);

  
  
  bool AddPendingQuery(Query* query, uint32 submit_count);

  
  
  bool AddPendingTransferQuery(Query* query, uint32 submit_count);

  
  
  bool RemovePendingQuery(Query* query);

  
  
  GLenum AdjustTargetForEmulation(GLenum target);

  
  GLES2Decoder* decoder_;

  bool use_arb_occlusion_query2_for_occlusion_query_boolean_;
  bool use_arb_occlusion_query_for_occlusion_query_boolean_;

  
  
  unsigned query_count_;

  
  typedef base::hash_map<GLuint, scoped_refptr<Query> > QueryMap;
  QueryMap queries_;

  
  typedef std::deque<scoped_refptr<Query> > QueryQueue;
  QueryQueue pending_queries_;

  
  QueryQueue pending_transfer_queries_;

  DISALLOW_COPY_AND_ASSIGN(QueryManager);
};

}  
}  

#endif  
