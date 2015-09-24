/* 
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 * 
 * The Original Code is the Netscape Portable Runtime (NSPR).
 * 
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are 
 * Copyright (C) 1998-2000 Netscape Communications Corporation.  All
 * Rights Reserved.
 * 
 * Contributor(s):
 * 
 * Alternatively, the contents of this file may be used under the
 * terms of the GNU General Public License Version 2 or later (the
 * "GPL"), in which case the provisions of the GPL are applicable 
 * instead of those above.  If you wish to allow use of your 
 * version of this file only under the terms of the GPL and not to
 * allow others to use your version of this file under the MPL,
 * indicate your decision by deleting the provisions above and
 * replace them with the notice and other provisions required by
 * the GPL.  If you do not delete the provisions above, a recipient
 * may use your version of this file under either the MPL or the
 * GPL.
 */


#ifndef prio_h___
#define prio_h___

#include "prlong.h"
#include "prtime.h"
#include "prinrval.h"
#include "prinet.h"

PR_BEGIN_EXTERN_C

typedef struct PRDir            PRDir;
typedef struct PRDirEntry       PRDirEntry;
#ifdef MOZ_UNICODE
typedef struct PRDirUTF16       PRDirUTF16;
typedef struct PRDirEntryUTF16  PRDirEntryUTF16;
#endif 
typedef struct PRFileDesc       PRFileDesc;
typedef struct PRFileInfo       PRFileInfo;
typedef struct PRFileInfo64     PRFileInfo64;
typedef union  PRNetAddr        PRNetAddr;
typedef struct PRIOMethods      PRIOMethods;
typedef struct PRPollDesc       PRPollDesc;
typedef struct PRFilePrivate    PRFilePrivate;
typedef struct PRSendFileData   PRSendFileData;


typedef PRIntn PRDescIdentity;          

struct PRFileDesc {
    const PRIOMethods *methods;         
    PRFilePrivate *secret;              
    PRFileDesc *lower, *higher;         
    void (PR_CALLBACK *dtor)(PRFileDesc *fd);
                                        
    PRDescIdentity identity;            
};

typedef enum PRTransmitFileFlags {
    PR_TRANSMITFILE_KEEP_OPEN = 0,    
    PR_TRANSMITFILE_CLOSE_SOCKET = 1  
} PRTransmitFileFlags;


#ifdef WIN32

#define PR_AF_INET 2
#define PR_AF_LOCAL 1
#define PR_INADDR_ANY (unsigned long)0x00000000
#define PR_INADDR_LOOPBACK 0x7f000001
#define PR_INADDR_BROADCAST (unsigned long)0xffffffff

#else 

#define PR_AF_INET AF_INET
#define PR_AF_LOCAL AF_UNIX
#define PR_INADDR_ANY INADDR_ANY
#define PR_INADDR_LOOPBACK INADDR_LOOPBACK
#define PR_INADDR_BROADCAST INADDR_BROADCAST

#endif 

#ifndef PR_AF_INET6
#define PR_AF_INET6 100
#endif

#ifndef PR_AF_UNSPEC
#define PR_AF_UNSPEC 0
#endif


struct PRIPv6Addr {
	union {
		PRUint8  _S6_u8[16];
		PRUint16 _S6_u16[8];
		PRUint32 _S6_u32[4];
		PRUint64 _S6_u64[2];
	} _S6_un;
};
#define pr_s6_addr		_S6_un._S6_u8
#define pr_s6_addr16	_S6_un._S6_u16
#define pr_s6_addr32	_S6_un._S6_u32
#define pr_s6_addr64 	_S6_un._S6_u64

typedef struct PRIPv6Addr PRIPv6Addr;

union PRNetAddr {
    struct {
        PRUint16 family;                
#ifdef XP_BEOS
        char data[10];                  
#else
        char data[14];                  
#endif
    } raw;
    struct {
        PRUint16 family;                
        PRUint16 port;                  
        PRUint32 ip;                    
#ifdef XP_BEOS
        char pad[4];                    
#else
        char pad[8];
#endif
    } inet;
    struct {
        PRUint16 family;                
        PRUint16 port;                  
        PRUint32 flowinfo;              
        PRIPv6Addr ip;                  
        PRUint32 scope_id;              
    } ipv6;
#if defined(XP_UNIX) || defined(XP_OS2)
    struct {                            
        PRUint16 family;                
#ifdef XP_OS2
        char path[108];                 
                                        
#else
        char path[104];                 
#endif
    } local;
#endif
};

typedef enum PRSockOption
{
    PR_SockOpt_Nonblocking,     
    PR_SockOpt_Linger,          
    PR_SockOpt_Reuseaddr,       
    PR_SockOpt_Keepalive,       
    PR_SockOpt_RecvBufferSize,  
    PR_SockOpt_SendBufferSize,  

    PR_SockOpt_IpTimeToLive,    
    PR_SockOpt_IpTypeOfService, 

    PR_SockOpt_AddMember,       
    PR_SockOpt_DropMember,      
    PR_SockOpt_McastInterface,  
    PR_SockOpt_McastTimeToLive, 
    PR_SockOpt_McastLoopback,   

    PR_SockOpt_NoDelay,         
    PR_SockOpt_MaxSegment,      
    PR_SockOpt_Broadcast,       
    PR_SockOpt_Last
} PRSockOption;

typedef struct PRLinger {
	PRBool polarity;		    
	PRIntervalTime linger;	    
} PRLinger;

typedef struct PRMcastRequest {
	PRNetAddr mcaddr;			
	PRNetAddr ifaddr;			
} PRMcastRequest;

typedef struct PRSocketOptionData
{
    PRSockOption option;
    union
    {
        PRUintn ip_ttl;             
        PRUintn mcast_ttl;          
        PRUintn tos;                
        PRBool non_blocking;        
        PRBool reuse_addr;          
        PRBool keep_alive;          
        PRBool mcast_loopback;      
        PRBool no_delay;            
        PRBool broadcast;           
        PRSize max_segment;         
        PRSize recv_buffer_size;    
        PRSize send_buffer_size;    
        PRLinger linger;            
        PRMcastRequest add_member;  
        PRMcastRequest drop_member; 
        PRNetAddr mcast_if;         
    } value;
} PRSocketOptionData;

typedef struct PRIOVec {
    char *iov_base;
    int iov_len;
} PRIOVec;

typedef enum PRDescType
{
    PR_DESC_FILE = 1,
    PR_DESC_SOCKET_TCP = 2,
    PR_DESC_SOCKET_UDP = 3,
    PR_DESC_LAYERED = 4,
    PR_DESC_PIPE = 5
} PRDescType;

typedef enum PRSeekWhence {
    PR_SEEK_SET = 0,
    PR_SEEK_CUR = 1,
    PR_SEEK_END = 2
} PRSeekWhence;

NSPR_API(PRDescType) PR_GetDescType(PRFileDesc *file);


typedef PRStatus (PR_CALLBACK *PRCloseFN)(PRFileDesc *fd);
typedef PRInt32 (PR_CALLBACK *PRReadFN)(PRFileDesc *fd, void *buf, PRInt32 amount);
typedef PRInt32 (PR_CALLBACK *PRWriteFN)(PRFileDesc *fd, const void *buf, PRInt32 amount);
typedef PRInt32 (PR_CALLBACK *PRAvailableFN)(PRFileDesc *fd);
typedef PRInt64 (PR_CALLBACK *PRAvailable64FN)(PRFileDesc *fd);
typedef PRStatus (PR_CALLBACK *PRFsyncFN)(PRFileDesc *fd);
typedef PROffset32 (PR_CALLBACK *PRSeekFN)(PRFileDesc *fd, PROffset32 offset, PRSeekWhence how);
typedef PROffset64 (PR_CALLBACK *PRSeek64FN)(PRFileDesc *fd, PROffset64 offset, PRSeekWhence how);
typedef PRStatus (PR_CALLBACK *PRFileInfoFN)(PRFileDesc *fd, PRFileInfo *info);
typedef PRStatus (PR_CALLBACK *PRFileInfo64FN)(PRFileDesc *fd, PRFileInfo64 *info);
typedef PRInt32 (PR_CALLBACK *PRWritevFN)(
    PRFileDesc *fd, const PRIOVec *iov, PRInt32 iov_size,
    PRIntervalTime timeout);
typedef PRStatus (PR_CALLBACK *PRConnectFN)(
    PRFileDesc *fd, const PRNetAddr *addr, PRIntervalTime timeout);
typedef PRFileDesc* (PR_CALLBACK *PRAcceptFN) (
    PRFileDesc *fd, PRNetAddr *addr, PRIntervalTime timeout);
typedef PRStatus (PR_CALLBACK *PRBindFN)(PRFileDesc *fd, const PRNetAddr *addr);
typedef PRStatus (PR_CALLBACK *PRListenFN)(PRFileDesc *fd, PRIntn backlog);
typedef PRStatus (PR_CALLBACK *PRShutdownFN)(PRFileDesc *fd, PRIntn how);
typedef PRInt32 (PR_CALLBACK *PRRecvFN)(
    PRFileDesc *fd, void *buf, PRInt32 amount,
    PRIntn flags, PRIntervalTime timeout);
typedef PRInt32 (PR_CALLBACK *PRSendFN) (
    PRFileDesc *fd, const void *buf, PRInt32 amount,
    PRIntn flags, PRIntervalTime timeout);
typedef PRInt32 (PR_CALLBACK *PRRecvfromFN)(
    PRFileDesc *fd, void *buf, PRInt32 amount,
    PRIntn flags, PRNetAddr *addr, PRIntervalTime timeout);
typedef PRInt32 (PR_CALLBACK *PRSendtoFN)(
    PRFileDesc *fd, const void *buf, PRInt32 amount,
    PRIntn flags, const PRNetAddr *addr, PRIntervalTime timeout);
typedef PRInt16 (PR_CALLBACK *PRPollFN)(
    PRFileDesc *fd, PRInt16 in_flags, PRInt16 *out_flags);
typedef PRInt32 (PR_CALLBACK *PRAcceptreadFN)(
    PRFileDesc *sd, PRFileDesc **nd, PRNetAddr **raddr,
    void *buf, PRInt32 amount, PRIntervalTime t);
typedef PRInt32 (PR_CALLBACK *PRTransmitfileFN)(
     PRFileDesc *sd, PRFileDesc *fd, const void *headers,
     PRInt32 hlen, PRTransmitFileFlags flags, PRIntervalTime t);
typedef PRStatus (PR_CALLBACK *PRGetsocknameFN)(PRFileDesc *fd, PRNetAddr *addr);
typedef PRStatus (PR_CALLBACK *PRGetpeernameFN)(PRFileDesc *fd, PRNetAddr *addr);
typedef PRStatus (PR_CALLBACK *PRGetsocketoptionFN)(
    PRFileDesc *fd, PRSocketOptionData *data);
typedef PRStatus (PR_CALLBACK *PRSetsocketoptionFN)(
    PRFileDesc *fd, const PRSocketOptionData *data);
typedef PRInt32 (PR_CALLBACK *PRSendfileFN)(
	PRFileDesc *networkSocket, PRSendFileData *sendData,
	PRTransmitFileFlags flags, PRIntervalTime timeout);
typedef PRStatus (PR_CALLBACK *PRConnectcontinueFN)(
    PRFileDesc *fd, PRInt16 out_flags);
typedef PRIntn (PR_CALLBACK *PRReservedFN)(PRFileDesc *fd);

struct PRIOMethods {
    PRDescType file_type;           
    PRCloseFN close;                
    PRReadFN read;                  
    PRWriteFN write;                
    PRAvailableFN available;        
    PRAvailable64FN available64;    
    PRFsyncFN fsync;                
    PRSeekFN seek;                  
    PRSeek64FN seek64;              
    PRFileInfoFN fileInfo;          
    PRFileInfo64FN fileInfo64;      
    PRWritevFN writev;              
    PRConnectFN connect;            
    PRAcceptFN accept;              
    PRBindFN bind;                  
    PRListenFN listen;              
    PRShutdownFN shutdown;          
    PRRecvFN recv;                  
    PRSendFN send;                  
    PRRecvfromFN recvfrom;          
    PRSendtoFN sendto;              
    PRPollFN poll;                  
    PRAcceptreadFN acceptread;      
    PRTransmitfileFN transmitfile;  
    PRGetsocknameFN getsockname;    
    PRGetpeernameFN getpeername;    
    PRReservedFN reserved_fn_6;     
    PRReservedFN reserved_fn_5;     
    PRGetsocketoptionFN getsocketoption;
                                    
    PRSetsocketoptionFN setsocketoption;
                                    
    PRSendfileFN sendfile;			
    PRConnectcontinueFN connectcontinue;
                                    
    PRReservedFN reserved_fn_3;		
    PRReservedFN reserved_fn_2;		
    PRReservedFN reserved_fn_1;		
    PRReservedFN reserved_fn_0;		
};


typedef enum PRSpecialFD
{
    PR_StandardInput,          
    PR_StandardOutput,         
    PR_StandardError           
} PRSpecialFD;

NSPR_API(PRFileDesc*) PR_GetSpecialFD(PRSpecialFD id);

#define PR_STDIN	PR_GetSpecialFD(PR_StandardInput)
#define PR_STDOUT	PR_GetSpecialFD(PR_StandardOutput)
#define PR_STDERR	PR_GetSpecialFD(PR_StandardError)


#define PR_IO_LAYER_HEAD (PRDescIdentity)-3
#define PR_INVALID_IO_LAYER (PRDescIdentity)-1
#define PR_TOP_IO_LAYER (PRDescIdentity)-2
#define PR_NSPR_IO_LAYER (PRDescIdentity)0

NSPR_API(PRDescIdentity) PR_GetUniqueIdentity(const char *layer_name);
NSPR_API(const char*) PR_GetNameForIdentity(PRDescIdentity ident);
NSPR_API(PRDescIdentity) PR_GetLayersIdentity(PRFileDesc* fd);
NSPR_API(PRFileDesc*) PR_GetIdentitiesLayer(PRFileDesc* fd_stack, PRDescIdentity id);

NSPR_API(const PRIOMethods *) PR_GetDefaultIOMethods(void);

NSPR_API(PRFileDesc*) PR_CreateIOLayerStub(
    PRDescIdentity ident, const PRIOMethods *methods);

NSPR_API(PRFileDesc*) PR_CreateIOLayer(PRFileDesc* fd);

NSPR_API(PRStatus) PR_PushIOLayer(
    PRFileDesc *fd_stack, PRDescIdentity id, PRFileDesc *layer);

NSPR_API(PRFileDesc*) PR_PopIOLayer(PRFileDesc *fd_stack, PRDescIdentity id);


#define PR_RDONLY       0x01
#define PR_WRONLY       0x02
#define PR_RDWR         0x04
#define PR_CREATE_FILE  0x08
#define PR_APPEND       0x10
#define PR_TRUNCATE     0x20
#define PR_SYNC         0x40
#define PR_EXCL         0x80


NSPR_API(PRFileDesc*) PR_Open(const char *name, PRIntn flags, PRIntn mode);


#define PR_IRWXU 00700  
#define PR_IRUSR 00400  
#define PR_IWUSR 00200  
#define PR_IXUSR 00100  
#define PR_IRWXG 00070  
#define PR_IRGRP 00040  
#define PR_IWGRP 00020  
#define PR_IXGRP 00010  
#define PR_IRWXO 00007  
#define PR_IROTH 00004  
#define PR_IWOTH 00002  
#define PR_IXOTH 00001  

NSPR_API(PRFileDesc*) PR_OpenFile(
    const char *name, PRIntn flags, PRIntn mode);

#ifdef MOZ_UNICODE
NSPR_API(PRFileDesc*) PR_OpenFileUTF16(
    const PRUnichar *name, PRIntn flags, PRIntn mode);
#endif 


NSPR_API(PRStatus)    PR_Close(PRFileDesc *fd);

/*
 **************************************************************************
 * FUNCTION: PR_Read
 * DESCRIPTION:
 *     Read bytes from a file or socket.
 *     The operation will block until either an end of stream indication is
 *     encountered, some positive number of bytes are transferred, or there
 *     is an error. No more than 'amount' bytes will be transferred.
 * INPUTS:
 *     PRFileDesc *fd
 *         pointer to the PRFileDesc object for the file or socket
 *     void *buf
 *         pointer to a buffer to hold the data read in.
 *     PRInt32 amount
 *         the size of 'buf' (in bytes)
 * OUTPUTS:
 * RETURN:
 *     PRInt32
 *         a positive number indicates the number of bytes actually read in.
 *         0 means end of file is reached or the network connection is closed.
 *         -1 indicates a failure. The reason for the failure is obtained
 *         by calling PR_GetError().
 * SIDE EFFECTS:
 *     data is written into the buffer pointed to by 'buf'.
 * RESTRICTIONS:
 *     None.
 * MEMORY:
 *     N/A
 * ALGORITHM:
 *     N/A
 **************************************************************************
 */

NSPR_API(PRInt32) PR_Read(PRFileDesc *fd, void *buf, PRInt32 amount);

/*
 ***************************************************************************
 * FUNCTION: PR_Write
 * DESCRIPTION:
 *     Write a specified number of bytes to a file or socket.  The thread
 *     invoking this function blocks until all the data is written.
 * INPUTS:
 *     PRFileDesc *fd
 *         pointer to a PRFileDesc object that refers to a file or socket
 *     const void *buf
 *         pointer to the buffer holding the data
 *     PRInt32 amount
 *         amount of data in bytes to be written from the buffer
 * OUTPUTS:
 *     None.
 * RETURN: PRInt32
 *     A positive number indicates the number of bytes successfully written.
 *     A -1 is an indication that the operation failed. The reason
 *     for the failure is obtained by calling PR_GetError().
 ***************************************************************************
 */

NSPR_API(PRInt32) PR_Write(PRFileDesc *fd,const void *buf,PRInt32 amount);

/*
 ***************************************************************************
 * FUNCTION: PR_Writev
 * DESCRIPTION:
 *     Write data to a socket.  The data is organized in a PRIOVec array. The
 *     operation will block until all the data is written or the operation
 *     fails.
 * INPUTS:
 *     PRFileDesc *fd
 *         Pointer that points to a PRFileDesc object for a socket.
 *     const PRIOVec *iov
 *         An array of PRIOVec.  PRIOVec is a struct with the following
 *         two fields:
 *             char *iov_base;
 *             int iov_len;
 *     PRInt32 iov_size
 *         Number of elements in the iov array. The value of this
 *         argument must not be greater than PR_MAX_IOVECTOR_SIZE.
 *         If it is, the method will fail (PR_BUFFER_OVERFLOW_ERROR).
 *     PRIntervalTime timeout
 *       Time limit for completion of the entire write operation.
 * OUTPUTS:
 *     None
 * RETURN:
 *     A positive number indicates the number of bytes successfully written.
 *     A -1 is an indication that the operation failed. The reason
 *     for the failure is obtained by calling PR_GetError().
 ***************************************************************************
 */

#define PR_MAX_IOVECTOR_SIZE 16   

NSPR_API(PRInt32) PR_Writev(
    PRFileDesc *fd, const PRIOVec *iov, PRInt32 iov_size,
    PRIntervalTime timeout);


NSPR_API(PRStatus) PR_Delete(const char *name);


typedef enum PRFileType
{
    PR_FILE_FILE = 1,
    PR_FILE_DIRECTORY = 2,
    PR_FILE_OTHER = 3
} PRFileType;

struct PRFileInfo {
    PRFileType type;        
    PROffset32 size;        
    PRTime creationTime;    
    PRTime modifyTime;      
};

struct PRFileInfo64 {
    PRFileType type;        
    PROffset64 size;        
    PRTime creationTime;    
    PRTime modifyTime;      
};

/****************************************************************************
 * FUNCTION: PR_GetFileInfo, PR_GetFileInfo64
 * DESCRIPTION:
 *     Get the information about the file with the given path name. This is
 *     applicable only to NSFileDesc describing 'file' types (see
 * INPUTS:
 *     const char *fn
 *         path name of the file
 * OUTPUTS:
 *     PRFileInfo *info
 *         Information about the given file is written into the file
 *         information object pointer to by 'info'.
 * RETURN: PRStatus
 *     PR_GetFileInfo returns PR_SUCCESS if file information is successfully
 *     obtained, otherwise it returns PR_FAILURE.
 ***************************************************************************
 */

NSPR_API(PRStatus) PR_GetFileInfo(const char *fn, PRFileInfo *info);
NSPR_API(PRStatus) PR_GetFileInfo64(const char *fn, PRFileInfo64 *info);

#ifdef MOZ_UNICODE
NSPR_API(PRStatus) PR_GetFileInfo64UTF16(const PRUnichar *fn, PRFileInfo64 *info);
#endif 


NSPR_API(PRStatus) PR_GetOpenFileInfo(PRFileDesc *fd, PRFileInfo *info);
NSPR_API(PRStatus) PR_GetOpenFileInfo64(PRFileDesc *fd, PRFileInfo64 *info);


NSPR_API(PRStatus)    PR_Rename(const char *from, const char *to);


typedef enum PRAccessHow {
    PR_ACCESS_EXISTS = 1,
    PR_ACCESS_WRITE_OK = 2,
    PR_ACCESS_READ_OK = 3
} PRAccessHow;

NSPR_API(PRStatus) PR_Access(const char *name, PRAccessHow how);


NSPR_API(PROffset32) PR_Seek(PRFileDesc *fd, PROffset32 offset, PRSeekWhence whence);
NSPR_API(PROffset64) PR_Seek64(PRFileDesc *fd, PROffset64 offset, PRSeekWhence whence);


NSPR_API(PRInt32) PR_Available(PRFileDesc *fd);
NSPR_API(PRInt64) PR_Available64(PRFileDesc *fd);


NSPR_API(PRStatus)	PR_Sync(PRFileDesc *fd);


struct PRDirEntry {
    const char *name;        
};

#ifdef MOZ_UNICODE
struct PRDirEntryUTF16 {
    const PRUnichar *name;   
};
#endif 

#if !defined(NO_NSPR_10_SUPPORT)
#define PR_DirName(dirEntry)	(dirEntry->name)
#endif


NSPR_API(PRDir*) PR_OpenDir(const char *name);

#ifdef MOZ_UNICODE
NSPR_API(PRDirUTF16*) PR_OpenDirUTF16(const PRUnichar *name);
#endif 


typedef enum PRDirFlags {
    PR_SKIP_NONE = 0x0,
    PR_SKIP_DOT = 0x1,
    PR_SKIP_DOT_DOT = 0x2,
    PR_SKIP_BOTH = 0x3,
    PR_SKIP_HIDDEN = 0x4
} PRDirFlags;

NSPR_API(PRDirEntry*) PR_ReadDir(PRDir *dir, PRDirFlags flags);

#ifdef MOZ_UNICODE
NSPR_API(PRDirEntryUTF16*) PR_ReadDirUTF16(PRDirUTF16 *dir, PRDirFlags flags);
#endif 


NSPR_API(PRStatus) PR_CloseDir(PRDir *dir);

#ifdef MOZ_UNICODE
NSPR_API(PRStatus) PR_CloseDirUTF16(PRDirUTF16 *dir);
#endif 


NSPR_API(PRStatus) PR_MkDir(const char *name, PRIntn mode);


NSPR_API(PRStatus) PR_MakeDir(const char *name, PRIntn mode);


NSPR_API(PRStatus) PR_RmDir(const char *name);


NSPR_API(PRFileDesc*)    PR_NewUDPSocket(void);


NSPR_API(PRFileDesc*)    PR_NewTCPSocket(void);


NSPR_API(PRFileDesc*)    PR_OpenUDPSocket(PRIntn af);


NSPR_API(PRFileDesc*)    PR_OpenTCPSocket(PRIntn af);


NSPR_API(PRStatus) PR_Connect(
    PRFileDesc *fd, const PRNetAddr *addr, PRIntervalTime timeout);


NSPR_API(PRStatus)    PR_ConnectContinue(PRFileDesc *fd, PRInt16 out_flags);


NSPR_API(PRStatus)    PR_GetConnectStatus(const PRPollDesc *pd);


NSPR_API(PRFileDesc*) PR_Accept(
    PRFileDesc *fd, PRNetAddr *addr, PRIntervalTime timeout);


NSPR_API(PRStatus) PR_Bind(PRFileDesc *fd, const PRNetAddr *addr);


NSPR_API(PRStatus) PR_Listen(PRFileDesc *fd, PRIntn backlog);


typedef enum PRShutdownHow
{
    PR_SHUTDOWN_RCV = 0,      
    PR_SHUTDOWN_SEND = 1,     
    PR_SHUTDOWN_BOTH = 2      
} PRShutdownHow;

NSPR_API(PRStatus)    PR_Shutdown(PRFileDesc *fd, PRShutdownHow how);


#define PR_MSG_PEEK 0x2

NSPR_API(PRInt32)    PR_Recv(PRFileDesc *fd, void *buf, PRInt32 amount,
                PRIntn flags, PRIntervalTime timeout);


NSPR_API(PRInt32)    PR_Send(PRFileDesc *fd, const void *buf, PRInt32 amount,
                                PRIntn flags, PRIntervalTime timeout);


NSPR_API(PRInt32) PR_RecvFrom(
    PRFileDesc *fd, void *buf, PRInt32 amount, PRIntn flags,
    PRNetAddr *addr, PRIntervalTime timeout);


NSPR_API(PRInt32) PR_SendTo(
    PRFileDesc *fd, const void *buf, PRInt32 amount, PRIntn flags,
    const PRNetAddr *addr, PRIntervalTime timeout);

/*
*************************************************************************
** FUNCTION: PR_TransmitFile
** DESCRIPTION:
**    Transmitfile sends a complete file (sourceFile) across a socket 
**    (networkSocket).  If headers is non-NULL, the headers will be sent across
**    the socket prior to sending the file.
** 
**    Optionally, the PR_TRANSMITFILE_CLOSE_SOCKET flag may be passed to
**    transmitfile.  This flag specifies that transmitfile should close the
**    socket after sending the data.
**
** INPUTS:
**    PRFileDesc *networkSocket
**        The socket to send data over
**    PRFileDesc *sourceFile
**        The file to send
**    const void *headers
**        A pointer to headers to be sent before sending data
**    PRInt32       hlen
**        length of header buffers in bytes.
**    PRTransmitFileFlags       flags
**        If the flags indicate that the connection should be closed,
**        it will be done immediately after transferring the file, unless
**        the operation is unsuccessful. 
.*     PRIntervalTime timeout
 *        Time limit for completion of the transmit operation.
**
** RETURNS:
**    Returns the number of bytes written or -1 if the operation failed.
**    If an error occurs while sending the file, the PR_TRANSMITFILE_CLOSE_
**    SOCKET flag is ignored. The reason for the failure is obtained
**    by calling PR_GetError().
**************************************************************************
*/

NSPR_API(PRInt32) PR_TransmitFile(
    PRFileDesc *networkSocket, PRFileDesc *sourceFile,
    const void *headers, PRInt32 hlen, PRTransmitFileFlags flags,
    PRIntervalTime timeout);

/*
*************************************************************************
** FUNCTION: PR_SendFile
** DESCRIPTION:
**    PR_SendFile sends data from a file (sendData->fd) across a socket 
**    (networkSocket).  If specified, a header and/or trailer buffer are sent
**	  before and after the file, respectively. The file offset, number of bytes
** 	  of file data to send, the header and trailer buffers are specified in the
**	  sendData argument.
** 
**    Optionally, if the PR_TRANSMITFILE_CLOSE_SOCKET flag is passed, the
**    socket is closed after successfully sending the data.
**
** INPUTS:
**    PRFileDesc *networkSocket
**        The socket to send data over
**    PRSendFileData *sendData
**        Contains the FD, file offset and length, header and trailer
**		  buffer specifications.
**    PRTransmitFileFlags       flags
**        If the flags indicate that the connection should be closed,
**        it will be done immediately after transferring the file, unless
**        the operation is unsuccessful. 
.*     PRIntervalTime timeout
 *        Time limit for completion of the send operation.
**
** RETURNS:
**    Returns the number of bytes written or -1 if the operation failed.
**    If an error occurs while sending the file, the PR_TRANSMITFILE_CLOSE_
**    SOCKET flag is ignored. The reason for the failure is obtained
**    by calling PR_GetError().
**************************************************************************
*/

struct PRSendFileData {
	PRFileDesc	*fd;			
	PRUint32	file_offset;	
	PRSize		file_nbytes;	
								
								
	const void	*header;		
	PRInt32		hlen;			
	const void	*trailer;		
	PRInt32		tlen;			
};


NSPR_API(PRInt32) PR_SendFile(
    PRFileDesc *networkSocket, PRSendFileData *sendData,
	PRTransmitFileFlags flags, PRIntervalTime timeout);

       
#define PR_ACCEPT_READ_BUF_OVERHEAD (32+(2*sizeof(PRNetAddr)))

NSPR_API(PRInt32) PR_AcceptRead(
    PRFileDesc *listenSock, PRFileDesc **acceptedSock,
    PRNetAddr **peerAddr, void *buf, PRInt32 amount, PRIntervalTime timeout);

/*
*************************************************************************
** FUNCTION: PR_NewTCPSocketPair
** DESCRIPTION:
**    Create a new TCP socket pair. The returned descriptors can be used
**    interchangeably; they are interconnected full-duplex descriptors: data
**    written to one can be read from the other and vice-versa.
**
** INPUTS:
**    None
** OUTPUTS:
**    PRFileDesc *fds[2]
**        The file descriptor pair for the newly created TCP sockets.
** RETURN: PRStatus
**     Upon successful completion of TCP socket pair, PR_NewTCPSocketPair 
**     returns PR_SUCCESS.  Otherwise, it returns PR_FAILURE.  Further
**     failure information can be obtained by calling PR_GetError().
** XXX can we implement this on windoze and mac?
**************************************************************************
**/
NSPR_API(PRStatus) PR_NewTCPSocketPair(PRFileDesc *fds[2]);

NSPR_API(PRStatus)	PR_GetSockName(PRFileDesc *fd, PRNetAddr *addr);

NSPR_API(PRStatus)	PR_GetPeerName(PRFileDesc *fd, PRNetAddr *addr);

NSPR_API(PRStatus)	PR_GetSocketOption(
    PRFileDesc *fd, PRSocketOptionData *data);

NSPR_API(PRStatus)	PR_SetSocketOption(
    PRFileDesc *fd, const PRSocketOptionData *data);


NSPR_API(PRStatus) PR_SetFDInheritable(
    PRFileDesc *fd,
    PRBool inheritable);

NSPR_API(PRFileDesc *) PR_GetInheritedFD(const char *name);


typedef struct PRFileMap PRFileMap;

typedef enum PRFileMapProtect {
    PR_PROT_READONLY,     
    PR_PROT_READWRITE,    
    PR_PROT_WRITECOPY     
} PRFileMapProtect;

NSPR_API(PRFileMap *) PR_CreateFileMap(
    PRFileDesc *fd,
    PRInt64 size,
    PRFileMapProtect prot);

NSPR_API(PRInt32) PR_GetMemMapAlignment(void);

NSPR_API(void *) PR_MemMap(
    PRFileMap *fmap,
    PROffset64 offset,  
    PRUint32 len);

NSPR_API(PRStatus) PR_MemUnmap(void *addr, PRUint32 len);

NSPR_API(PRStatus) PR_CloseFileMap(PRFileMap *fmap);



NSPR_API(PRStatus) PR_CreatePipe(
    PRFileDesc **readPipe,
    PRFileDesc **writePipe
);


struct PRPollDesc {
    PRFileDesc* fd;
    PRInt16 in_flags;
    PRInt16 out_flags;
};


#if defined(_PR_POLL_BACKCOMPAT)

#include <poll.h>
#define PR_POLL_READ    POLLIN
#define PR_POLL_WRITE   POLLOUT
#define PR_POLL_EXCEPT  POLLPRI
#define PR_POLL_ERR     POLLERR     
#define PR_POLL_NVAL    POLLNVAL    
#define PR_POLL_HUP     POLLHUP     

#else  

#define PR_POLL_READ    0x1
#define PR_POLL_WRITE   0x2
#define PR_POLL_EXCEPT  0x4
#define PR_POLL_ERR     0x8         
#define PR_POLL_NVAL    0x10        
#define PR_POLL_HUP     0x20        

#endif  

NSPR_API(PRInt32) PR_Poll(
    PRPollDesc *pds, PRIntn npds, PRIntervalTime timeout);


NSPR_API(PRFileDesc *) PR_NewPollableEvent(void);

NSPR_API(PRStatus) PR_DestroyPollableEvent(PRFileDesc *event);

NSPR_API(PRStatus) PR_SetPollableEvent(PRFileDesc *event);

NSPR_API(PRStatus) PR_WaitForPollableEvent(PRFileDesc *event);

PR_END_EXTERN_C

#endif 
