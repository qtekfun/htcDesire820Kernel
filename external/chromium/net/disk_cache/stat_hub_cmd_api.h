/** ---------------------------------------------------------------------------
Copyright (c) 2011, 2012 The Linux Foundation. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-----------------------------------------------------------------------------**/

#ifndef STAT_HUB_CMD_API_H_
#define STAT_HUB_CMD_API_H_

#include <string>

class StatHubCmd;

typedef enum {
    SH_CMD_GP_EVENT,                
    SH_CMD_WK_MAIN_URL,             
    SH_CMD_WK_MEMORY_CACHE,         
    SH_CMD_WK_RESOURCE,             
    SH_CMD_WK_PAGE,                 
    SH_CMD_WK_INSPECTOR_RECORD,     
    SH_CMD_WK_JS_SEQ,               
    SH_CMD_JAVA_GP_EVENT,           
    SH_CMD_CH_URL_REQUEST,          
    SH_CMD_CH_TRANS_NET,            
    SH_CMD_CH_TRANS_CACHE,          
    SH_CMD_TCPIP_SOCKET,            
    SH_CMD_TBD_12,                  
    SH_CMD_TBD_13,                  
    SH_CMD_TBD_14,                  
    SH_CMD_TBD_15,                  
    SH_CMD_TBD_16,                  
    SH_CMD_TBD_17,                  
    SH_CMD_TBD_18,                  
    SH_CMD_TBD_19,                  
    SH_CMD_TBD_20,                  
    SH_CMD_TBD_21,                  
    SH_CMD_TBD_22,                  
    SH_CMD_TBD_23,                  
    SH_CMD_TBD_24,                  
    SH_CMD_TBD_25,                  
    SH_CMD_TBD_26,                  
    SH_CMD_TBD_27,                  
    SH_CMD_TBD_28,                  
    SH_CMD_TBD_29,                  
    SH_CMD_TBD_30,                  
    SH_CMD_TBD_31,                  

    SH_CMD_USER_DEFINED = 32        
} StatHubCmdType;

typedef enum {
    SH_ACTION_NONE,
    SH_ACTION_WILL_START,
    SH_ACTION_DID_START,
    SH_ACTION_WILL_FINISH,
    SH_ACTION_DID_FINISH,
    SH_ACTION_WILL_START_LOAD,
    SH_ACTION_DID_START_LOAD,
    SH_ACTION_WILL_FINISH_LOAD,
    SH_ACTION_DID_FINISH_LOAD,
    SH_ACTION_WILL_SEND_REQUEST,
    SH_ACTION_DID_SEND_REQUEST,
    SH_ACTION_WILL_RECEIVE_RESPONSE,
    SH_ACTION_DID_RECEIVE_RESPONSE,
    SH_ACTION_STATUS,
    SH_ACTION_CLEAR,
    SH_ACTION_CREATE,
    SH_ACTION_CONNECT,
    SH_ACTION_CLOSE,
    SH_ACTION_READ,
    SH_ACTION_WRITE,
    SH_ACTION_FETCH_DELAYED,
    SH_ACTION_PREFETCH,
    SH_ACTION_JS_SEQ,

    SH_ACTION_LAST,
} StatHubActionType;


#define SH_PREFETCH_PRIO_LOW    1024
#define SH_PREFETCH_PRIO_NORMAL 512
#define SH_PREFETCH_PRIO_HIGH   0

extern unsigned int StatHubHash(const char* str)
    __attribute__ ((visibility ("default"), used));
extern void StatHubDebugLog(const char* str)
    __attribute__ ((visibility ("default"), used));
extern bool StatHubIsReady()
    __attribute__ ((visibility ("default"), used));
extern bool StatHubIsProcReady(const char* name)
    __attribute__ ((visibility ("default"), used));
extern bool StatHubIsPerfEnabled()
    __attribute__ ((visibility ("default"), used));

extern StatHubCmd* StatHubCmdCreate(StatHubCmdType cmd_id, StatHubActionType action = SH_ACTION_NONE, unsigned int cookie = 0)
    __attribute__ ((visibility ("default"), used));
extern void StatHubCmdAddParamAsUint32(StatHubCmd* cmd, unsigned int param)
    __attribute__ ((visibility ("default"), used));
extern void StatHubCmdAddParamAsString(StatHubCmd* cmd, const char* param)
    __attribute__ ((visibility ("default"), used));
extern void StatHubCmdAddParamAsBuf(StatHubCmd* cmd, const void* param, unsigned int size)
    __attribute__ ((visibility ("default"), used));
extern void StatHubCmdAddParamAsBool(StatHubCmd* cmd, bool param)
    __attribute__ ((visibility ("default"), used));
extern void StatHubCmdTimeStamp(StatHubCmd* cmd)
    __attribute__ ((visibility ("default"), used));
extern void StatHubCmdCommit(StatHubCmd* cmd)
    __attribute__ ((visibility ("default"), used));
extern void StatHubCmdCommitDelayed(StatHubCmd* cmd, unsigned int delay_ms)
    __attribute__ ((visibility ("default"), used));
extern void StatHubCmdPush(StatHubCmd* cmd)
    __attribute__ ((visibility ("default"), used));
extern StatHubCmd* StatHubCmdPop(unsigned int cookie, StatHubCmdType cmd_id, StatHubActionType action = SH_ACTION_NONE)
    __attribute__ ((visibility ("default"), used));

extern void StatHubURLRequestContextCreated(unsigned int context)
    __attribute__ ((visibility ("default"), used));
extern void StatHubURLRequestContextDestroyed(unsigned int context)
    __attribute__ ((visibility ("default"), used));
extern bool StatHubIsInDC(const char* url)
    __attribute__ ((visibility ("default"), used));
extern bool StatHubIsPreloaded(const char* url, std::string& headers, char*& data, int& size)
    __attribute__ ((visibility ("default"), used));
extern bool StatHubReleasePreloaded(const char* url)
    __attribute__ ((visibility ("default"), used));
extern bool StatHubIsPreloaderEnabled()
    __attribute__ ((visibility ("default"), used));
#endif 
