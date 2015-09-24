/*
 * This file contains prototypes for the public SSL functions.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __ssl_h_
#define __ssl_h_

#include "prtypes.h"
#include "prerror.h"
#include "prio.h"
#include "seccomon.h"
#include "cert.h"
#include "keyt.h"

#include "sslt.h"  

#if defined(_WIN32) && !defined(IN_LIBSSL) && !defined(NSS_USE_STATIC_LIBS)
#define SSL_IMPORT extern __declspec(dllimport)
#else
#define SSL_IMPORT extern
#endif

SEC_BEGIN_PROTOS

SSL_IMPORT const PRUint16 SSL_ImplementedCiphers[];

SSL_IMPORT const PRUint16 *SSL_GetImplementedCiphers(void);

SSL_IMPORT const PRUint16 SSL_NumImplementedCiphers;

SSL_IMPORT PRUint16 SSL_GetNumImplementedCiphers(void);

#define SSL_IS_SSL2_CIPHER(which) (((which) & 0xfff0) == 0xff00)

SSL_IMPORT PRFileDesc *SSL_ImportFD(PRFileDesc *model, PRFileDesc *fd);

SSL_IMPORT PRFileDesc *DTLS_ImportFD(PRFileDesc *model, PRFileDesc *fd);

#define SSL_SECURITY			1 
#define SSL_SOCKS			2 
#define SSL_REQUEST_CERTIFICATE		3 
#define SSL_HANDSHAKE_AS_CLIENT		5 
                               		  
#define SSL_HANDSHAKE_AS_SERVER		6 
                               		  

#define SSL_ENABLE_SSL2			7 

#define SSL_ENABLE_SSL3		        8 

#define SSL_NO_CACHE		        9 
                    		          
#define SSL_REQUIRE_CERTIFICATE        10 
                                          
#define SSL_ENABLE_FDX                 11 
                                          

#define SSL_V2_COMPATIBLE_HELLO        12 
                                          

#define SSL_ENABLE_TLS		       13 

#define SSL_ROLLBACK_DETECTION         14 
#define SSL_NO_STEP_DOWN               15 
                                          
					  
					  
#define SSL_BYPASS_PKCS11              16 
#define SSL_NO_LOCKS                   17 
#define SSL_ENABLE_SESSION_TICKETS     18 
                                          
#define SSL_ENABLE_DEFLATE             19 
                                          
#define SSL_ENABLE_RENEGOTIATION       20 
#define SSL_REQUIRE_SAFE_NEGOTIATION   21 
					  
                                          
					  
                                          
#define SSL_ENABLE_FALSE_START         22 
                                          
                                          

#define SSL_CBC_RANDOM_IV 23
#define SSL_ENABLE_OCSP_STAPLING       24 
#define SSL_ENABLE_SIGNED_CERT_TIMESTAMPS 25
#define SSL_ENABLE_FALLBACK_SCSV       26 

#ifdef SSL_DEPRECATED_FUNCTION 
SSL_IMPORT SECStatus SSL_Enable(PRFileDesc *fd, int option, PRBool on);
SSL_IMPORT SECStatus SSL_EnableDefault(int option, PRBool on);
#endif

SSL_IMPORT SECStatus SSL_OptionSet(PRFileDesc *fd, PRInt32 option, PRBool on);
SSL_IMPORT SECStatus SSL_OptionGet(PRFileDesc *fd, PRInt32 option, PRBool *on);
SSL_IMPORT SECStatus SSL_OptionSetDefault(PRInt32 option, PRBool on);
SSL_IMPORT SECStatus SSL_OptionGetDefault(PRInt32 option, PRBool *on);
SSL_IMPORT SECStatus SSL_CertDBHandleSet(PRFileDesc *fd, CERTCertDBHandle *dbHandle);

typedef SECStatus (PR_CALLBACK *SSLNextProtoCallback)(
    void *arg,
    PRFileDesc *fd,
    const unsigned char* protos,
    unsigned int protosLen,
    unsigned char* protoOut,
    unsigned int* protoOutLen,
    unsigned int protoMaxOut);

SSL_IMPORT SECStatus SSL_SetNextProtoCallback(PRFileDesc *fd,
                                              SSLNextProtoCallback callback,
                                              void *arg);

SSL_IMPORT SECStatus SSL_SetNextProtoNego(PRFileDesc *fd,
					  const unsigned char *data,
					  unsigned int length);

typedef enum SSLNextProtoState { 
  SSL_NEXT_PROTO_NO_SUPPORT = 0, 
  SSL_NEXT_PROTO_NEGOTIATED = 1, 
  SSL_NEXT_PROTO_NO_OVERLAP = 2, 
  SSL_NEXT_PROTO_SELECTED   = 3  
} SSLNextProtoState;

/* SSL_GetNextProto can be used in the HandshakeCallback or any time after
 * a handshake to retrieve the result of the Next Protocol negotiation.
 *
 * The length of the negotiated protocol, if any, is written into *bufLen.
 * If the negotiated protocol is longer than bufLenMax, then SECFailure is
 * returned. Otherwise, the negotiated protocol, if any, is written into buf,
 * and SECSuccess is returned. */
SSL_IMPORT SECStatus SSL_GetNextProto(PRFileDesc *fd,
				      SSLNextProtoState *state,
				      unsigned char *buf,
				      unsigned int *bufLen,
				      unsigned int bufLenMax);

#ifdef SSL_DEPRECATED_FUNCTION 
SSL_IMPORT SECStatus SSL_EnableCipher(long which, PRBool enabled);
SSL_IMPORT SECStatus SSL_SetPolicy(long which, int policy);
#endif

SSL_IMPORT SECStatus SSL_CipherPrefSet(PRFileDesc *fd, PRInt32 cipher, PRBool enabled);
SSL_IMPORT SECStatus SSL_CipherPrefGet(PRFileDesc *fd, PRInt32 cipher, PRBool *enabled);
SSL_IMPORT SECStatus SSL_CipherPrefSetDefault(PRInt32 cipher, PRBool enabled);
SSL_IMPORT SECStatus SSL_CipherPrefGetDefault(PRInt32 cipher, PRBool *enabled);
SSL_IMPORT SECStatus SSL_CipherPolicySet(PRInt32 cipher, PRInt32 policy);
SSL_IMPORT SECStatus SSL_CipherPolicyGet(PRInt32 cipher, PRInt32 *policy);

SSL_IMPORT SECStatus SSL_CipherOrderSet(PRFileDesc *fd, const PRUint16 *ciphers,
                                        unsigned int len);

typedef enum SSLChannelBindingType {
    SSL_CHANNEL_BINDING_TLS_UNIQUE = 1,
} SSLChannelBindingType;

/* SSL_GetChannelBinding copies the requested channel binding value, as defined
 * in RFC 5929, into |out|. The full length of the binding value is written
 * into |*outLen|.
 *
 * At most |outLenMax| bytes of data are copied. If |outLenMax| is
 * insufficient then the function returns SECFailure and sets the error to
 * SEC_ERROR_OUTPUT_LEN, but |*outLen| is still set.
 *
 * This call will fail if made during a renegotiation. */
SSL_IMPORT SECStatus SSL_GetChannelBinding(PRFileDesc *fd,
					   SSLChannelBindingType binding_type,
					   unsigned char *out,
					   unsigned int *outLen,
					   unsigned int outLenMax);


SSL_IMPORT SECStatus SSL_VersionRangeGetSupported(
    SSLProtocolVariant protocolVariant, SSLVersionRange *vrange);

SSL_IMPORT SECStatus SSL_VersionRangeGetDefault(
    SSLProtocolVariant protocolVariant, SSLVersionRange *vrange);

SSL_IMPORT SECStatus SSL_VersionRangeSetDefault(
    SSLProtocolVariant protocolVariant, const SSLVersionRange *vrange);

SSL_IMPORT SECStatus SSL_VersionRangeGet(PRFileDesc *fd,
					 SSLVersionRange *vrange);

SSL_IMPORT SECStatus SSL_VersionRangeSet(PRFileDesc *fd,
					 const SSLVersionRange *vrange);


#define SSL_NOT_ALLOWED		 0	      
#define SSL_ALLOWED		 1
#define SSL_RESTRICTED		 2	      

#define SSL_REQUIRE_NEVER           ((PRBool)0)
#define SSL_REQUIRE_ALWAYS          ((PRBool)1)
#define SSL_REQUIRE_FIRST_HANDSHAKE ((PRBool)2)
#define SSL_REQUIRE_NO_ERROR        ((PRBool)3)

#define SSL_RENEGOTIATE_NEVER        ((PRBool)0)
#define SSL_RENEGOTIATE_UNRESTRICTED ((PRBool)1)
#define SSL_RENEGOTIATE_REQUIRES_XTN ((PRBool)2) 
#define SSL_RENEGOTIATE_TRANSITIONAL ((PRBool)3)

SSL_IMPORT SECStatus SSL_ResetHandshake(PRFileDesc *fd, PRBool asServer);

SSL_IMPORT SECStatus SSL_ForceHandshake(PRFileDesc *fd);

SSL_IMPORT SECStatus SSL_ForceHandshakeWithTimeout(PRFileDesc *fd,
                                                   PRIntervalTime timeout);

SSL_IMPORT SECStatus SSL_RestartHandshakeAfterCertReq(PRFileDesc *fd,
					    CERTCertificate *cert,
					    SECKEYPrivateKey *key,
					    CERTCertificateList *certChain);

SSL_IMPORT SECStatus SSL_SecurityStatus(PRFileDesc *fd, int *on, char **cipher,
			                int *keySize, int *secretKeySize,
			                char **issuer, char **subject);

#define SSL_SECURITY_STATUS_NOOPT	-1
#define SSL_SECURITY_STATUS_OFF		0
#define SSL_SECURITY_STATUS_ON_HIGH	1
#define SSL_SECURITY_STATUS_ON_LOW	2
#define SSL_SECURITY_STATUS_FORTEZZA	3 

SSL_IMPORT CERTCertificate *SSL_PeerCertificate(PRFileDesc *fd);

SSL_IMPORT CERTCertList *SSL_PeerCertificateChain(PRFileDesc *fd);

SSL_IMPORT const SECItemArray * SSL_PeerStapledOCSPResponses(PRFileDesc *fd);

SSL_IMPORT const SECItem * SSL_PeerSignedCertTimestamps(PRFileDesc *fd);

SSL_IMPORT SECStatus
SSL_SetStapledOCSPResponses(PRFileDesc *fd, const SECItemArray *responses,
			    SSLKEAType kea);

typedef SECStatus (PR_CALLBACK *SSLAuthCertificate)(void *arg, PRFileDesc *fd, 
                                                    PRBool checkSig,
                                                    PRBool isServer);

SSL_IMPORT SECStatus SSL_AuthCertificateHook(PRFileDesc *fd, 
					     SSLAuthCertificate f,
				             void *arg);

SSL_IMPORT SECStatus SSL_AuthCertificate(void *arg, PRFileDesc *fd, 
					 PRBool checkSig, PRBool isServer);

typedef SECStatus (PR_CALLBACK *SSLGetClientAuthData)(void *arg,
                                PRFileDesc *fd,
                                CERTDistNames *caNames,
                                CERTCertificate **pRetCert,
                                SECKEYPrivateKey **pRetKey);

SSL_IMPORT SECStatus SSL_GetClientAuthDataHook(PRFileDesc *fd, 
			                       SSLGetClientAuthData f, void *a);

typedef SECStatus (PR_CALLBACK *SSLGetPlatformClientAuthData)(void *arg,
                                PRFileDesc *fd,
                                CERTDistNames *caNames,
                                CERTCertList **pRetCerts,
                                void **pRetKey,
                                CERTCertificate **pRetNSSCert,
                                SECKEYPrivateKey **pRetNSSKey);

SSL_IMPORT SECStatus
SSL_GetPlatformClientAuthDataHook(PRFileDesc *fd,
                                  SSLGetPlatformClientAuthData f, void *a);

typedef PRInt32 (PR_CALLBACK *SSLSNISocketConfig)(PRFileDesc *fd,
                                            const SECItem *srvNameArr,
                                                  PRUint32 srvNameArrSize,
                                                  void *arg);

#define SSL_SNI_CURRENT_CONFIG_IS_USED           -1
#define SSL_SNI_SEND_ALERT                       -2

SSL_IMPORT SECStatus SSL_SNISocketConfigHook(PRFileDesc *fd, 
                                             SSLSNISocketConfig f,
                                             void *arg);

SSL_IMPORT PRFileDesc *SSL_ReconfigFD(PRFileDesc *model, PRFileDesc *fd);

SSL_IMPORT SECStatus SSL_SetPKCS11PinArg(PRFileDesc *fd, void *a);

typedef SECStatus (PR_CALLBACK *SSLBadCertHandler)(void *arg, PRFileDesc *fd);
SSL_IMPORT SECStatus SSL_BadCertHook(PRFileDesc *fd, SSLBadCertHandler f, 
				     void *arg);

SSL_IMPORT SECStatus SSL_ConfigSecureServer(
				PRFileDesc *fd, CERTCertificate *cert,
				SECKEYPrivateKey *key, SSLKEAType kea);

SSL_IMPORT SECStatus
SSL_ConfigSecureServerWithCertChain(PRFileDesc *fd, CERTCertificate *cert,
                                    const CERTCertificateList *certChainOpt,
                                    SECKEYPrivateKey *key, SSLKEAType kea);

SSL_IMPORT SECStatus SSL_ConfigServerSessionIDCache(int      maxCacheEntries,
					            PRUint32 timeout,
					            PRUint32 ssl3_timeout,
				              const char *   directory);

SSL_IMPORT SECStatus SSL_ConfigServerSessionIDCacheWithOpt(
                                                           PRUint32 timeout,
                                                       PRUint32 ssl3_timeout,
                                                     const char *   directory,
                                                          int maxCacheEntries,
                                                      int maxCertCacheEntries,
                                                    int maxSrvNameCacheEntries,
                                                           PRBool enableMPCache);

SSL_IMPORT SECStatus SSL_ConfigMPServerSIDCache(int      maxCacheEntries, 
				                PRUint32 timeout,
			       	                PRUint32 ssl3_timeout, 
		                          const char *   directory);

SSL_IMPORT PRUint32  SSL_GetMaxServerCacheLocks(void);
SSL_IMPORT SECStatus SSL_SetMaxServerCacheLocks(PRUint32 maxLocks);

#define SSL_ENV_VAR_NAME            "SSL_INHERITANCE"

SSL_IMPORT SECStatus SSL_InheritMPServerSIDCache(const char * envString);

typedef void (PR_CALLBACK *SSLHandshakeCallback)(PRFileDesc *fd,
                                                 void *client_data);
SSL_IMPORT SECStatus SSL_HandshakeCallback(PRFileDesc *fd, 
			          SSLHandshakeCallback cb, void *client_data);

typedef SECStatus (PR_CALLBACK *SSLCanFalseStartCallback)(
    PRFileDesc *fd, void *arg, PRBool *canFalseStart);

SSL_IMPORT SECStatus SSL_SetCanFalseStartCallback(
    PRFileDesc *fd, SSLCanFalseStartCallback callback, void *arg);

SSL_IMPORT SECStatus SSL_RecommendedCanFalseStart(PRFileDesc *fd,
                                                  PRBool *canFalseStart);

SSL_IMPORT SECStatus SSL_ReHandshake(PRFileDesc *fd, PRBool flushCache);

SSL_IMPORT SECStatus SSL_ReHandshakeWithTimeout(PRFileDesc *fd,
                                                PRBool flushCache,
                                                PRIntervalTime timeout);

SSL_IMPORT const SECItem *
SSL_GetRequestedClientCertificateTypes(PRFileDesc *fd);

#ifdef SSL_DEPRECATED_FUNCTION 
SSL_IMPORT SECStatus SSL_RedoHandshake(PRFileDesc *fd);
#endif

SSL_IMPORT SECStatus SSL_SetURL(PRFileDesc *fd, const char *url);

SSL_IMPORT SECStatus SSL_SetTrustAnchors(PRFileDesc *fd, CERTCertList *list);

SSL_IMPORT int SSL_DataPending(PRFileDesc *fd);

SSL_IMPORT SECStatus SSL_InvalidateSession(PRFileDesc *fd);

SSL_IMPORT SECStatus SSL_CacheSession(PRFileDesc *fd);

SSL_IMPORT SECStatus SSL_CacheSessionUnlocked(PRFileDesc *fd);

SSL_IMPORT SECItem *SSL_GetSessionID(PRFileDesc *fd);

SSL_IMPORT void SSL_ClearSessionCache(void);

SSL_IMPORT SECStatus SSL_ShutdownServerSessionIDCache(void);

SSL_IMPORT SECStatus SSL_SetSockPeerID(PRFileDesc *fd, const char *peerID);

SSL_IMPORT CERTCertificate * SSL_RevealCert(PRFileDesc * socket);
SSL_IMPORT void * SSL_RevealPinArg(PRFileDesc * socket);
SSL_IMPORT char * SSL_RevealURL(PRFileDesc * socket);

SSL_IMPORT SECStatus
NSS_GetClientAuthData(void *                       arg,
                      PRFileDesc *                 socket,
                      struct CERTDistNamesStr *    caNames,
                      struct CERTCertificateStr ** pRetCert,
                      struct SECKEYPrivateKeyStr **pRetKey);

SSL_IMPORT SECStatus SSL_SetSRTPCiphers(PRFileDesc *fd,
					const PRUint16 *ciphers,
					unsigned int numCiphers);

SSL_IMPORT SECStatus SSL_GetSRTPCipher(PRFileDesc *fd,
				       PRUint16 *cipher);

SSL_IMPORT SECStatus NSS_CmpCertChainWCANames(CERTCertificate *cert, 
                                          CERTDistNames *caNames);

SSL_IMPORT SSLKEAType NSS_FindCertKEAType(CERTCertificate * cert);

SSL_IMPORT SECStatus NSS_SetDomesticPolicy(void);

/* Set cipher policies to a predefined Policy that is exportable from the USA
 *   according to present U.S. policies as we understand them.
 * See documentation for the list.
 * Note that your particular application program may be able to obtain
 *   an export license with more or fewer capabilities than those allowed
 *   by this function.  In that case, you should use SSL_SetPolicy()
 *   to explicitly allow those ciphers you may legally export.
 */
SSL_IMPORT SECStatus NSS_SetExportPolicy(void);

SSL_IMPORT SECStatus NSS_SetFrancePolicy(void);

SSL_IMPORT SSL3Statistics * SSL_GetStatistics(void);

SSL_IMPORT SECStatus SSL_GetChannelInfo(PRFileDesc *fd, SSLChannelInfo *info,
                                        PRUintn len);
SSL_IMPORT SECStatus SSL_GetCipherSuiteInfo(PRUint16 cipherSuite, 
                                        SSLCipherSuiteInfo *info, PRUintn len);

SSL_IMPORT SECItem *SSL_GetNegotiatedHostInfo(PRFileDesc *fd);

SSL_IMPORT SECStatus SSL_ExportKeyingMaterial(PRFileDesc *fd,
                                              const char *label,
                                              unsigned int labelLen,
                                              PRBool hasContext,
                                              const unsigned char *context,
                                              unsigned int contextLen,
                                              unsigned char *out,
                                              unsigned int outLen);

SSL_IMPORT CERTCertificate * SSL_LocalCertificate(PRFileDesc *fd);


#define SSL_CBP_SSL3	0x0001	        
#define SSL_CBP_TLS1_0	0x0002		

SSL_IMPORT SECStatus SSL_CanBypass(CERTCertificate *cert,
                                   SECKEYPrivateKey *privKey,
				   PRUint32 protocolmask,
				   PRUint16 *ciphers, int nciphers,
                                   PRBool *pcanbypass, void *pwArg);

SSL_IMPORT SECStatus SSL_HandshakeNegotiatedExtension(PRFileDesc * socket,
                                                      SSLExtensionType extId,
                                                      PRBool *yes);

SSL_IMPORT SECStatus SSL_HandshakeResumedSession(PRFileDesc *fd,
                                                 PRBool *last_handshake_resumed);

/* See SSL_SetClientChannelIDCallback for usage. If the callback returns
 * SECWouldBlock then SSL_RestartHandshakeAfterChannelIDReq should be called in
 * the future to restart the handshake.  On SECSuccess, the callback must have
 * written a P-256, EC key pair to |*out_public_key| and |*out_private_key|. */
typedef SECStatus (PR_CALLBACK *SSLClientChannelIDCallback)(
    void *arg,
    PRFileDesc *fd,
    SECKEYPublicKey **out_public_key,
    SECKEYPrivateKey **out_private_key);

SSL_IMPORT SECStatus SSL_RestartHandshakeAfterChannelIDReq(
    PRFileDesc *fd,
    SECKEYPublicKey *channelIDPub,
    SECKEYPrivateKey *channelID);

SSL_IMPORT SECStatus SSL_SetClientChannelIDCallback(
    PRFileDesc *fd,
    SSLClientChannelIDCallback callback,
    void *arg);

SSL_IMPORT SECStatus DTLS_GetHandshakeTimeout(PRFileDesc *socket,
                                              PRIntervalTime *timeout);

extern PRBool NSSSSL_VersionCheck(const char *importedVersion);

extern const char *NSSSSL_GetVersion(void);

SSL_IMPORT SECStatus SSL_AuthCertificateComplete(PRFileDesc *fd,
						 PRErrorCode error);
SEC_END_PROTOS

#endif 
