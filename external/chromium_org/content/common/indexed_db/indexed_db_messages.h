// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.


#include <vector>

#include "content/common/indexed_db/indexed_db_key.h"
#include "content/common/indexed_db/indexed_db_key_path.h"
#include "content/common/indexed_db/indexed_db_key_range.h"
#include "content/common/indexed_db/indexed_db_param_traits.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_param_traits.h"
#include "third_party/WebKit/public/platform/WebIDBCursor.h"
#include "third_party/WebKit/public/platform/WebIDBDatabase.h"

#define IPC_MESSAGE_START IndexedDBMsgStart


IPC_ENUM_TRAITS(blink::WebIDBCursor::Direction)
IPC_ENUM_TRAITS(blink::WebIDBDatabase::PutMode)
IPC_ENUM_TRAITS(blink::WebIDBDatabase::TaskType)

IPC_ENUM_TRAITS_MAX_VALUE(blink::WebIDBDataLoss, blink::WebIDBDataLossTotal)

IPC_STRUCT_BEGIN(IndexedDBHostMsg_FactoryGetDatabaseNames_Params)
  
  IPC_STRUCT_MEMBER(int32, ipc_thread_id)
  IPC_STRUCT_MEMBER(int32, ipc_callbacks_id)
  
  IPC_STRUCT_MEMBER(std::string, database_identifier)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBHostMsg_FactoryOpen_Params)
  
  IPC_STRUCT_MEMBER(int32, ipc_thread_id)
  
  IPC_STRUCT_MEMBER(int32, ipc_callbacks_id)
  
  IPC_STRUCT_MEMBER(int32, ipc_database_callbacks_id)
  
  IPC_STRUCT_MEMBER(std::string, database_identifier)
  
  IPC_STRUCT_MEMBER(base::string16, name)
  
  IPC_STRUCT_MEMBER(int64, transaction_id)
  
  IPC_STRUCT_MEMBER(int64, version)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBHostMsg_FactoryDeleteDatabase_Params)
  
  IPC_STRUCT_MEMBER(int32, ipc_thread_id)
  IPC_STRUCT_MEMBER(int32, ipc_callbacks_id)
  
  IPC_STRUCT_MEMBER(std::string, database_identifier)
  
  IPC_STRUCT_MEMBER(base::string16, name)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBHostMsg_DatabaseCreateTransaction_Params)
  IPC_STRUCT_MEMBER(int32, ipc_thread_id)
  
  IPC_STRUCT_MEMBER(int32, ipc_database_id)
  
  IPC_STRUCT_MEMBER(int64, transaction_id)
  
  IPC_STRUCT_MEMBER(int32, ipc_database_callbacks_id)
  
  IPC_STRUCT_MEMBER(std::vector<int64>, object_store_ids)
  
  IPC_STRUCT_MEMBER(int32, mode)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBHostMsg_DatabaseCreateObjectStore_Params)
  
  IPC_STRUCT_MEMBER(int32, ipc_database_id)
  
  IPC_STRUCT_MEMBER(int64, transaction_id)
  
  IPC_STRUCT_MEMBER(int64, object_store_id)
  
  IPC_STRUCT_MEMBER(base::string16, name)
  
  IPC_STRUCT_MEMBER(content::IndexedDBKeyPath, key_path)
  
  IPC_STRUCT_MEMBER(bool, auto_increment)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBHostMsg_DatabaseGet_Params)
  IPC_STRUCT_MEMBER(int32, ipc_thread_id)
  
  IPC_STRUCT_MEMBER(int32, ipc_callbacks_id)
  
  IPC_STRUCT_MEMBER(int32, ipc_database_id)
  
  IPC_STRUCT_MEMBER(int64, transaction_id)
  
  IPC_STRUCT_MEMBER(int64, object_store_id)
  
  IPC_STRUCT_MEMBER(int64, index_id)
  
  IPC_STRUCT_MEMBER(content::IndexedDBKeyRange, key_range)
  
  IPC_STRUCT_MEMBER(bool, key_only)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBHostMsg_DatabasePut_Params)
  
  IPC_STRUCT_MEMBER(int32, ipc_thread_id)
  IPC_STRUCT_MEMBER(int32, ipc_callbacks_id)
  
  IPC_STRUCT_MEMBER(int32, ipc_database_id)
  
  IPC_STRUCT_MEMBER(int64, transaction_id)
  
  IPC_STRUCT_MEMBER(int64, object_store_id)
  
  IPC_STRUCT_MEMBER(int64, index_id)
  
  IPC_STRUCT_MEMBER(std::string, value)
  
  IPC_STRUCT_MEMBER(content::IndexedDBKey, key)
  
  IPC_STRUCT_MEMBER(blink::WebIDBDatabase::PutMode, put_mode)
  
  IPC_STRUCT_MEMBER(std::vector<int64>, index_ids)
  
  
  IPC_STRUCT_MEMBER(std::vector<std::vector<content::IndexedDBKey> >,
                    index_keys)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBHostMsg_DatabaseOpenCursor_Params)
  
  IPC_STRUCT_MEMBER(int32, ipc_thread_id)
  IPC_STRUCT_MEMBER(int32, ipc_callbacks_id)
  
  IPC_STRUCT_MEMBER(int32, ipc_database_id)
  
  IPC_STRUCT_MEMBER(int64, transaction_id)
  
  IPC_STRUCT_MEMBER(int64, object_store_id)
  
  IPC_STRUCT_MEMBER(int64, index_id)
  
  IPC_STRUCT_MEMBER(content::IndexedDBKeyRange, key_range)
  
  IPC_STRUCT_MEMBER(int32, direction)
  
  IPC_STRUCT_MEMBER(bool, key_only)
  
  IPC_STRUCT_MEMBER(blink::WebIDBDatabase::TaskType, task_type)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBHostMsg_DatabaseCount_Params)
  
  IPC_STRUCT_MEMBER(int32, ipc_thread_id)
  IPC_STRUCT_MEMBER(int32, ipc_callbacks_id)
  
  IPC_STRUCT_MEMBER(int64, transaction_id)
  
  IPC_STRUCT_MEMBER(int32, ipc_database_id)
  
  IPC_STRUCT_MEMBER(int64, object_store_id)
  
  IPC_STRUCT_MEMBER(int64, index_id)
  
  IPC_STRUCT_MEMBER(content::IndexedDBKeyRange, key_range)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBHostMsg_DatabaseDeleteRange_Params)
  
  IPC_STRUCT_MEMBER(int32, ipc_thread_id)
  IPC_STRUCT_MEMBER(int32, ipc_callbacks_id)
  
  IPC_STRUCT_MEMBER(int32, ipc_database_id)
  
  IPC_STRUCT_MEMBER(int64, transaction_id)
  
  IPC_STRUCT_MEMBER(int64, object_store_id)
  
  IPC_STRUCT_MEMBER(content::IndexedDBKeyRange, key_range)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBHostMsg_DatabaseSetIndexKeys_Params)
  
  IPC_STRUCT_MEMBER(int32, ipc_database_id)
  
  IPC_STRUCT_MEMBER(int64, transaction_id)
  
  IPC_STRUCT_MEMBER(int64, object_store_id)
  
  IPC_STRUCT_MEMBER(content::IndexedDBKey, primary_key)
  
  IPC_STRUCT_MEMBER(std::vector<int64>, index_ids)
  
  IPC_STRUCT_MEMBER(std::vector<std::vector<content::IndexedDBKey> >,
                    index_keys)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBHostMsg_DatabaseCreateIndex_Params)
  
  IPC_STRUCT_MEMBER(int64, transaction_id)
  
  IPC_STRUCT_MEMBER(int32, ipc_database_id)
  
  IPC_STRUCT_MEMBER(int64, object_store_id)
  
  IPC_STRUCT_MEMBER(int64, index_id)
  
  IPC_STRUCT_MEMBER(base::string16, name)
  
  IPC_STRUCT_MEMBER(content::IndexedDBKeyPath, key_path)
  
  IPC_STRUCT_MEMBER(bool, unique)
  
  IPC_STRUCT_MEMBER(bool, multi_entry)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBMsg_CallbacksSuccessIDBCursor_Params)
  IPC_STRUCT_MEMBER(int32, ipc_thread_id)
  IPC_STRUCT_MEMBER(int32, ipc_callbacks_id)
  IPC_STRUCT_MEMBER(int32, ipc_cursor_id)
  IPC_STRUCT_MEMBER(content::IndexedDBKey, key)
  IPC_STRUCT_MEMBER(content::IndexedDBKey, primary_key)
  IPC_STRUCT_MEMBER(std::string, value)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBMsg_CallbacksSuccessCursorContinue_Params)
  IPC_STRUCT_MEMBER(int32, ipc_thread_id)
  IPC_STRUCT_MEMBER(int32, ipc_callbacks_id)
  IPC_STRUCT_MEMBER(int32, ipc_cursor_id)
  IPC_STRUCT_MEMBER(content::IndexedDBKey, key)
  IPC_STRUCT_MEMBER(content::IndexedDBKey, primary_key)
  IPC_STRUCT_MEMBER(std::string, value)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBMsg_CallbacksSuccessCursorPrefetch_Params)
  IPC_STRUCT_MEMBER(int32, ipc_thread_id)
  IPC_STRUCT_MEMBER(int32, ipc_callbacks_id)
  IPC_STRUCT_MEMBER(int32, ipc_cursor_id)
  IPC_STRUCT_MEMBER(std::vector<content::IndexedDBKey>, keys)
  IPC_STRUCT_MEMBER(std::vector<content::IndexedDBKey>, primary_keys)
  IPC_STRUCT_MEMBER(std::vector<std::string>, values)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBIndexMetadata)
  IPC_STRUCT_MEMBER(int64, id)
  IPC_STRUCT_MEMBER(base::string16, name)
  IPC_STRUCT_MEMBER(content::IndexedDBKeyPath, keyPath)
  IPC_STRUCT_MEMBER(bool, unique)
  IPC_STRUCT_MEMBER(bool, multiEntry)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBObjectStoreMetadata)
  IPC_STRUCT_MEMBER(int64, id)
  IPC_STRUCT_MEMBER(base::string16, name)
  IPC_STRUCT_MEMBER(content::IndexedDBKeyPath, keyPath)
  IPC_STRUCT_MEMBER(bool, autoIncrement)
  IPC_STRUCT_MEMBER(int64, max_index_id)
  IPC_STRUCT_MEMBER(std::vector<IndexedDBIndexMetadata>, indexes)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBDatabaseMetadata)
  IPC_STRUCT_MEMBER(int64, id)
  IPC_STRUCT_MEMBER(base::string16, name)
  IPC_STRUCT_MEMBER(base::string16, version)
  IPC_STRUCT_MEMBER(int64, int_version)
  IPC_STRUCT_MEMBER(int64, max_object_store_id)
  IPC_STRUCT_MEMBER(std::vector<IndexedDBObjectStoreMetadata>, object_stores)
IPC_STRUCT_END()

IPC_STRUCT_BEGIN(IndexedDBMsg_CallbacksUpgradeNeeded_Params)
  IPC_STRUCT_MEMBER(int32, ipc_thread_id)
  IPC_STRUCT_MEMBER(int32, ipc_callbacks_id)
  IPC_STRUCT_MEMBER(int32, ipc_database_callbacks_id)
  IPC_STRUCT_MEMBER(int32, ipc_database_id)
  IPC_STRUCT_MEMBER(int64, old_version)
  IPC_STRUCT_MEMBER(blink::WebIDBDataLoss, data_loss)
  IPC_STRUCT_MEMBER(std::string, data_loss_message)
  IPC_STRUCT_MEMBER(IndexedDBDatabaseMetadata, idb_metadata)
IPC_STRUCT_END()



IPC_MESSAGE_CONTROL1(IndexedDBMsg_CallbacksSuccessIDBCursor,
                     IndexedDBMsg_CallbacksSuccessIDBCursor_Params)

IPC_MESSAGE_CONTROL1(IndexedDBMsg_CallbacksSuccessCursorContinue,
                     IndexedDBMsg_CallbacksSuccessCursorContinue_Params)

IPC_MESSAGE_CONTROL1(IndexedDBMsg_CallbacksSuccessCursorAdvance,
                     IndexedDBMsg_CallbacksSuccessCursorContinue_Params)

IPC_MESSAGE_CONTROL1(IndexedDBMsg_CallbacksSuccessCursorPrefetch,
                     IndexedDBMsg_CallbacksSuccessCursorPrefetch_Params)

IPC_MESSAGE_CONTROL5(IndexedDBMsg_CallbacksSuccessIDBDatabase,
                     int32 ,
                     int32 ,
                     int32 ,
                     int32 ,
                     IndexedDBDatabaseMetadata)
IPC_MESSAGE_CONTROL3(IndexedDBMsg_CallbacksSuccessIndexedDBKey,
                     int32 ,
                     int32 ,
                     content::IndexedDBKey )
IPC_MESSAGE_CONTROL3(IndexedDBMsg_CallbacksSuccessValue,
                     int32 ,
                     int32 ,
                     std::string )
IPC_MESSAGE_CONTROL5(IndexedDBMsg_CallbacksSuccessValueWithKey,
                     int32 ,
                     int32 ,
                     std::string ,
                     content::IndexedDBKey ,
                     content::IndexedDBKeyPath )
IPC_MESSAGE_CONTROL3(IndexedDBMsg_CallbacksSuccessInteger,
                     int32 ,
                     int32 ,
                     int64 )
IPC_MESSAGE_CONTROL2(IndexedDBMsg_CallbacksSuccessUndefined,
                     int32 ,
                     int32 )
IPC_MESSAGE_CONTROL3(IndexedDBMsg_CallbacksSuccessStringList,
                     int32 ,
                     int32 ,
                     std::vector<base::string16> )
IPC_MESSAGE_CONTROL4(IndexedDBMsg_CallbacksError,
                     int32 ,
                     int32 ,
                     int ,
                     base::string16 )
IPC_MESSAGE_CONTROL2(IndexedDBMsg_CallbacksBlocked,
                     int32 ,
                     int32 )
IPC_MESSAGE_CONTROL3(IndexedDBMsg_CallbacksIntBlocked,
                     int32 ,
                     int32 ,
                     int64 )
IPC_MESSAGE_CONTROL1(IndexedDBMsg_CallbacksUpgradeNeeded,
                     IndexedDBMsg_CallbacksUpgradeNeeded_Params)

IPC_MESSAGE_CONTROL2(IndexedDBMsg_DatabaseCallbacksForcedClose,
                     int32, 
                     int32) 
IPC_MESSAGE_CONTROL4(IndexedDBMsg_DatabaseCallbacksIntVersionChange,
                     int32, 
                     int32, 
                     int64, 
                     int64) 
IPC_MESSAGE_CONTROL5(IndexedDBMsg_DatabaseCallbacksAbort,
                     int32, 
                     int32, 
                     int64, 
                     int, 
                     base::string16) 
IPC_MESSAGE_CONTROL3(IndexedDBMsg_DatabaseCallbacksComplete,
                     int32, 
                     int32, 
                     int64) 


IPC_MESSAGE_CONTROL4(IndexedDBHostMsg_CursorAdvance,
                     int32, 
                     int32, 
                     int32, 
                     unsigned long) 

IPC_MESSAGE_CONTROL5(IndexedDBHostMsg_CursorContinue,
                     int32, 
                     int32, 
                     int32, 
                     content::IndexedDBKey, 
                     content::IndexedDBKey) 

IPC_MESSAGE_CONTROL4(IndexedDBHostMsg_CursorPrefetch,
                     int32, 
                     int32, 
                     int32, 
                     int32) 

IPC_MESSAGE_CONTROL3(IndexedDBHostMsg_CursorPrefetchReset,
                     int32, 
                     int32, 
                     int32)  

IPC_MESSAGE_CONTROL1(IndexedDBHostMsg_FactoryGetDatabaseNames,
                     IndexedDBHostMsg_FactoryGetDatabaseNames_Params)

IPC_MESSAGE_CONTROL1(IndexedDBHostMsg_FactoryOpen,
                     IndexedDBHostMsg_FactoryOpen_Params)

IPC_MESSAGE_CONTROL1(IndexedDBHostMsg_FactoryDeleteDatabase,
                     IndexedDBHostMsg_FactoryDeleteDatabase_Params)

IPC_MESSAGE_CONTROL1(IndexedDBHostMsg_DatabaseCreateObjectStore,
                     IndexedDBHostMsg_DatabaseCreateObjectStore_Params)

IPC_MESSAGE_CONTROL3(IndexedDBHostMsg_DatabaseDeleteObjectStore,
                     int32, 
                     int64, 
                     int64) 

IPC_MESSAGE_CONTROL1(IndexedDBHostMsg_DatabaseCreateTransaction,
                     IndexedDBHostMsg_DatabaseCreateTransaction_Params)

IPC_MESSAGE_CONTROL1(IndexedDBHostMsg_DatabaseClose,
                     int32 )

IPC_MESSAGE_CONTROL1(IndexedDBHostMsg_DatabaseDestroyed,
                     int32 )

IPC_MESSAGE_CONTROL1(IndexedDBHostMsg_DatabaseGet,
                     IndexedDBHostMsg_DatabaseGet_Params)

IPC_MESSAGE_CONTROL1(IndexedDBHostMsg_DatabasePut,
                     IndexedDBHostMsg_DatabasePut_Params)

IPC_MESSAGE_CONTROL1(IndexedDBHostMsg_DatabaseSetIndexKeys,
                     IndexedDBHostMsg_DatabaseSetIndexKeys_Params)

IPC_MESSAGE_CONTROL4(IndexedDBHostMsg_DatabaseSetIndexesReady,
                     int32, 
                     int64, 
                     int64, 
                     std::vector<int64>) 

IPC_MESSAGE_CONTROL1(IndexedDBHostMsg_DatabaseOpenCursor,
                     IndexedDBHostMsg_DatabaseOpenCursor_Params)

IPC_MESSAGE_CONTROL1(IndexedDBHostMsg_DatabaseCount,
                     IndexedDBHostMsg_DatabaseCount_Params)

IPC_MESSAGE_CONTROL1(IndexedDBHostMsg_DatabaseDeleteRange,
                     IndexedDBHostMsg_DatabaseDeleteRange_Params)

IPC_MESSAGE_CONTROL5(IndexedDBHostMsg_DatabaseClear,
                     int32, 
                     int32, 
                     int32, 
                     int64, 
                     int64) 

IPC_MESSAGE_CONTROL1(IndexedDBHostMsg_DatabaseCreateIndex,
                     IndexedDBHostMsg_DatabaseCreateIndex_Params)

IPC_MESSAGE_CONTROL4(IndexedDBHostMsg_DatabaseDeleteIndex,
                     int32, 
                     int64, 
                     int64, 
                     int64) 

IPC_MESSAGE_CONTROL2(IndexedDBHostMsg_DatabaseAbort,
                     int32, 
                     int64) 

IPC_MESSAGE_CONTROL2(IndexedDBHostMsg_DatabaseCommit,
                     int32, 
                     int64) 

IPC_MESSAGE_CONTROL1(IndexedDBHostMsg_CursorDestroyed,
                     int32 )

