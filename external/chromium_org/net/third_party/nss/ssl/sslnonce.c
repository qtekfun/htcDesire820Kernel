/* 
 * This file implements the CLIENT Session ID cache.  
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "cert.h"
#include "pk11pub.h"
#include "secitem.h"
#include "ssl.h"
#include "nss.h"

#include "sslimpl.h"
#include "sslproto.h"
#include "nssilock.h"
#if defined(XP_UNIX) || defined(XP_WIN) || defined(_WINDOWS) || defined(XP_BEOS)
#include <time.h>
#endif

PRUint32 ssl_sid_timeout = 100;
PRUint32 ssl3_sid_timeout = 86400L; 

sslSessionID *cache = NULL;
PZLock *      cacheLock = NULL;


#define LOCK_CACHE 	lock_cache()
#define UNLOCK_CACHE	PZ_Unlock(cacheLock)

static PRCallOnceType lockOnce;

static SECStatus
FreeSessionCacheLocks(void* appData, void* nssData)
{
    static const PRCallOnceType pristineCallOnce;
    SECStatus rv;

    if (!cacheLock) {
        PORT_SetError(SEC_ERROR_NOT_INITIALIZED);
        return SECFailure;
    }

    PZ_DestroyLock(cacheLock);
    cacheLock = NULL;

    rv = ssl_FreeSymWrapKeysLock();
    if (rv != SECSuccess) {
        return rv;
    }

    lockOnce = pristineCallOnce;
    return SECSuccess;
}

static PRStatus
InitSessionCacheLocks(void)
{
    SECStatus rv;

    cacheLock = PZ_NewLock(nssILockCache);
    if (cacheLock == NULL) {
        return PR_FAILURE;
    }
    rv = ssl_InitSymWrapKeysLock();
    if (rv != SECSuccess) {
        PRErrorCode error = PORT_GetError();
        PZ_DestroyLock(cacheLock);
        cacheLock = NULL;
        PORT_SetError(error);
        return PR_FAILURE;
    }

    rv = NSS_RegisterShutdown(FreeSessionCacheLocks, NULL);
    PORT_Assert(SECSuccess == rv);
    if (SECSuccess != rv) {
        return PR_FAILURE;
    }
    return PR_SUCCESS;
}

SECStatus
ssl_InitSessionCacheLocks(void)
{
    return (PR_SUCCESS ==
            PR_CallOnce(&lockOnce, InitSessionCacheLocks)) ?
           SECSuccess : SECFailure;
}

static void
lock_cache(void)
{
    ssl_InitSessionCacheLocks();
    PZ_Lock(cacheLock);
}

static void
ssl_DestroySID(sslSessionID *sid)
{
    int i;
    SSL_TRC(8, ("SSL: destroy sid: sid=0x%x cached=%d", sid, sid->cached));
    PORT_Assert((sid->references == 0));

    if (sid->cached == in_client_cache)
    	return;	

    if (sid->version < SSL_LIBRARY_VERSION_3_0) {
	SECITEM_ZfreeItem(&sid->u.ssl2.masterKey, PR_FALSE);
	SECITEM_ZfreeItem(&sid->u.ssl2.cipherArg, PR_FALSE);
    } else {
	if (sid->u.ssl3.sessionTicket.ticket.data) {
	    SECITEM_FreeItem(&sid->u.ssl3.sessionTicket.ticket, PR_FALSE);
	}
	if (sid->u.ssl3.srvName.data) {
	    SECITEM_FreeItem(&sid->u.ssl3.srvName, PR_FALSE);
	}
	if (sid->u.ssl3.signedCertTimestamps.data) {
	    SECITEM_FreeItem(&sid->u.ssl3.signedCertTimestamps, PR_FALSE);
	}
	if (sid->u.ssl3.originalHandshakeHash.data) {
	  SECITEM_FreeItem(&sid->u.ssl3.originalHandshakeHash, PR_FALSE);
	}
    }

    if (sid->peerID != NULL)
	PORT_Free((void *)sid->peerID);		

    if (sid->urlSvrName != NULL)
	PORT_Free((void *)sid->urlSvrName);	

    if ( sid->peerCert ) {
	CERT_DestroyCertificate(sid->peerCert);
    }
    for (i = 0; i < MAX_PEER_CERT_CHAIN_SIZE && sid->peerCertChain[i]; i++) {
	CERT_DestroyCertificate(sid->peerCertChain[i]);
    }
    if (sid->peerCertStatus.items) {
        SECITEM_FreeArray(&sid->peerCertStatus, PR_FALSE);
    }

    if ( sid->localCert ) {
	CERT_DestroyCertificate(sid->localCert);
    }

    PORT_ZFree(sid, sizeof(sslSessionID));
}

static void 
ssl_FreeLockedSID(sslSessionID *sid)
{
    PORT_Assert(sid->references >= 1);
    if (--sid->references == 0) {
	ssl_DestroySID(sid);
    }
}

void
ssl_FreeSID(sslSessionID *sid)
{
    LOCK_CACHE;
    ssl_FreeLockedSID(sid);
    UNLOCK_CACHE;
}



sslSessionID *
ssl_LookupSID(const PRIPv6Addr *addr, PRUint16 port, const char *peerID, 
              const char * urlSvrName)
{
    sslSessionID **sidp;
    sslSessionID * sid;
    PRUint32       now;

    if (!urlSvrName)
    	return NULL;
    now = ssl_Time();
    LOCK_CACHE;
    sidp = &cache;
    while ((sid = *sidp) != 0) {
	PORT_Assert(sid->cached == in_client_cache);
	PORT_Assert(sid->references >= 1);

	SSL_TRC(8, ("SSL: Lookup1: sid=0x%x", sid));

	if (sid->expirationTime < now || !sid->references) {
	    SSL_TRC(7, ("SSL: lookup1, throwing sid out, age=%d refs=%d",
			now - sid->creationTime, sid->references));

	    *sidp = sid->next; 			
	    sid->cached = invalid_cache;	
	    if (!sid->references)
	    	ssl_DestroySID(sid);
	    else
		ssl_FreeLockedSID(sid);		

	} else if (!memcmp(&sid->addr, addr, sizeof(PRIPv6Addr)) && 
	           (sid->port == port) && 
		   
		   (((peerID == NULL) && (sid->peerID == NULL)) ||
		    ((peerID != NULL) && (sid->peerID != NULL) &&
		     PORT_Strcmp(sid->peerID, peerID) == 0)) &&
		   
		   (sid->version < SSL_LIBRARY_VERSION_3_0 ||
		    sid->u.ssl3.keys.resumable) &&
		   
	           (sid->urlSvrName != NULL) &&
		   ((0 == PORT_Strcmp(urlSvrName, sid->urlSvrName)) ||
		    ((sid->peerCert != NULL) && (SECSuccess == 
		      CERT_VerifyCertName(sid->peerCert, urlSvrName))) )
		  ) {
	    
	    sid->lastAccessTime = now;
	    sid->references++;
	    break;
	} else {
	    sidp = &sid->next;
	}
    }
    UNLOCK_CACHE;
    return sid;
}

static void 
CacheSID(sslSessionID *sid)
{
    PRUint32  expirationPeriod;
    SSL_TRC(8, ("SSL: Cache: sid=0x%x cached=%d addr=0x%08x%08x%08x%08x port=0x%04x "
		"time=%x cached=%d",
		sid, sid->cached, sid->addr.pr_s6_addr32[0], 
		sid->addr.pr_s6_addr32[1], sid->addr.pr_s6_addr32[2],
		sid->addr.pr_s6_addr32[3],  sid->port, sid->creationTime,
		sid->cached));

    if (sid->cached == in_client_cache)
	return;

    if (!sid->urlSvrName) {
        
        return;
    }

    
    if (sid->version < SSL_LIBRARY_VERSION_3_0) {
	expirationPeriod = ssl_sid_timeout;
	PRINT_BUF(8, (0, "sessionID:",
		  sid->u.ssl2.sessionID, sizeof(sid->u.ssl2.sessionID)));
	PRINT_BUF(8, (0, "masterKey:",
		  sid->u.ssl2.masterKey.data, sid->u.ssl2.masterKey.len));
	PRINT_BUF(8, (0, "cipherArg:",
		  sid->u.ssl2.cipherArg.data, sid->u.ssl2.cipherArg.len));
    } else {
	if (sid->u.ssl3.sessionIDLength == 0 &&
	    sid->u.ssl3.sessionTicket.ticket.data == NULL)
	    return;
	
	if (sid->u.ssl3.sessionIDLength == 0) {
	    SECStatus rv;
	    rv = PK11_GenerateRandom(sid->u.ssl3.sessionID,
		SSL3_SESSIONID_BYTES);
	    if (rv != SECSuccess)
		return;
	    sid->u.ssl3.sessionIDLength = SSL3_SESSIONID_BYTES;
	}
	expirationPeriod = ssl3_sid_timeout;
	PRINT_BUF(8, (0, "sessionID:",
		      sid->u.ssl3.sessionID, sid->u.ssl3.sessionIDLength));
    }
    PORT_Assert(sid->creationTime != 0 && sid->expirationTime != 0);
    if (!sid->creationTime)
	sid->lastAccessTime = sid->creationTime = ssl_Time();
    if (!sid->expirationTime)
	sid->expirationTime = sid->creationTime + expirationPeriod;

    LOCK_CACHE;
    sid->references++;
    sid->cached = in_client_cache;
    sid->next   = cache;
    cache       = sid;
    UNLOCK_CACHE;
}

static void
UncacheSID(sslSessionID *zap)
{
    sslSessionID **sidp = &cache;
    sslSessionID *sid;

    if (zap->cached != in_client_cache) {
	return;
    }

    SSL_TRC(8,("SSL: Uncache: zap=0x%x cached=%d addr=0x%08x%08x%08x%08x port=0x%04x "
	       "time=%x cipher=%d",
	       zap, zap->cached, zap->addr.pr_s6_addr32[0],
	       zap->addr.pr_s6_addr32[1], zap->addr.pr_s6_addr32[2],
	       zap->addr.pr_s6_addr32[3], zap->port, zap->creationTime,
	       zap->u.ssl2.cipherType));
    if (zap->version < SSL_LIBRARY_VERSION_3_0) {
	PRINT_BUF(8, (0, "sessionID:",
		      zap->u.ssl2.sessionID, sizeof(zap->u.ssl2.sessionID)));
	PRINT_BUF(8, (0, "masterKey:",
		      zap->u.ssl2.masterKey.data, zap->u.ssl2.masterKey.len));
	PRINT_BUF(8, (0, "cipherArg:",
		      zap->u.ssl2.cipherArg.data, zap->u.ssl2.cipherArg.len));
    }

    
    while ((sid = *sidp) != 0) {
	if (sid == zap) {
	    *sidp = zap->next;
	    zap->cached = invalid_cache;
	    ssl_FreeLockedSID(zap);
	    return;
	}
	sidp = &sid->next;
    }
}

static void
LockAndUncacheSID(sslSessionID *zap)
{
    LOCK_CACHE;
    UncacheSID(zap);
    UNLOCK_CACHE;

}

void 
ssl_ChooseSessionIDProcs(sslSecurityInfo *sec)
{
    if (sec->isServer) {
	sec->cache   = ssl_sid_cache;
	sec->uncache = ssl_sid_uncache;
    } else {
	sec->cache   = CacheSID;
	sec->uncache = LockAndUncacheSID;
    }
}

void
SSL_ClearSessionCache(void)
{
    LOCK_CACHE;
    while(cache != NULL)
	UncacheSID(cache);
    UNLOCK_CACHE;
}

PRUint32
ssl_Time(void)
{
    PRUint32 myTime;
#if defined(XP_UNIX) || defined(XP_WIN) || defined(_WINDOWS) || defined(XP_BEOS)
    myTime = time(NULL);	
#else
    
    PRTime now;
    PRInt64 ll;

    now = PR_Now();
    LL_I2L(ll, 1000000L);
    LL_DIV(now, now, ll);
    LL_L2UI(myTime, now);
#endif
    return myTime;
}

SECStatus
ssl3_SetSIDSessionTicket(sslSessionID *sid, NewSessionTicket *session_ticket)
{
    SECStatus rv;

    
    LOCK_CACHE;

    
    if (sid->cached != never_cached) {
	UNLOCK_CACHE;
	return SECSuccess;
    }

    if (sid->u.ssl3.sessionTicket.ticket.data)
	SECITEM_FreeItem(&sid->u.ssl3.sessionTicket.ticket, PR_FALSE);
    if (session_ticket->ticket.len > 0) {
	rv = SECITEM_CopyItem(NULL, &sid->u.ssl3.sessionTicket.ticket,
	    &session_ticket->ticket);
	if (rv != SECSuccess) {
	    UNLOCK_CACHE;
	    return rv;
	}
    } else {
	sid->u.ssl3.sessionTicket.ticket.data = NULL;
	sid->u.ssl3.sessionTicket.ticket.len = 0;
    }
    sid->u.ssl3.sessionTicket.received_timestamp =
	session_ticket->received_timestamp;
    sid->u.ssl3.sessionTicket.ticket_lifetime_hint =
	session_ticket->ticket_lifetime_hint;

    UNLOCK_CACHE;
    return SECSuccess;
}
