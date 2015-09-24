/* Copyright (c) 2012 The Chromium Authors. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */


#ifndef PPAPI_C_PRIVATE_PPP_CONTENT_DECRYPTOR_PRIVATE_H_
#define PPAPI_C_PRIVATE_PPP_CONTENT_DECRYPTOR_PRIVATE_H_

#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_macros.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/private/pp_content_decryptor.h"

#define PPP_CONTENTDECRYPTOR_PRIVATE_INTERFACE_0_9 \
    "PPP_ContentDecryptor_Private;0.9"
#define PPP_CONTENTDECRYPTOR_PRIVATE_INTERFACE \
    PPP_CONTENTDECRYPTOR_PRIVATE_INTERFACE_0_9



struct PPP_ContentDecryptor_Private_0_9 {
  void (*Initialize)(PP_Instance instance, struct PP_Var key_system);
  void (*CreateSession)(PP_Instance instance,
                        uint32_t session_id,
                        struct PP_Var type,
                        struct PP_Var init_data);
  /**
   * Provides a license or other message to the decryptor.
   *
   * When the CDM needs more information, it must call
   * <code>SessionMessage()</code> on the
   * <code>PPB_ContentDecryptor_Private</code> interface, and the browser
   * must notify the web application. When the CDM has finished processing
   * <code>response</code> and needs no more information, it must call
   * <code>SessionReady()</code> on the
   * <code>PPB_ContentDecryptor_Private</code> interface, and the browser
   * must notify the web application.
   *
   * @param[in] session_id A reference for the session to update.
   *
   * @param[in] response A <code>PP_Var</code> of type
   * <code>PP_VARTYPE_ARRAYBUFFER</code> containing the license or other
   * message for the given session ID.
   */
  void (*UpdateSession)(PP_Instance instance,
                        uint32_t session_id,
                        struct PP_Var response);
  void (*ReleaseSession)(PP_Instance instance, uint32_t session_id);
  void (*Decrypt)(PP_Instance instance,
                  PP_Resource encrypted_block,
                  const struct PP_EncryptedBlockInfo* encrypted_block_info);
  void (*InitializeAudioDecoder)(
      PP_Instance instance,
      const struct PP_AudioDecoderConfig* decoder_config,
      PP_Resource codec_extra_data);
  void (*InitializeVideoDecoder)(
      PP_Instance instance,
      const struct PP_VideoDecoderConfig* decoder_config,
      PP_Resource codec_extra_data);
  void (*DeinitializeDecoder)(PP_Instance instance,
                              PP_DecryptorStreamType decoder_type,
                              uint32_t request_id);
  void (*ResetDecoder)(PP_Instance instance,
                       PP_DecryptorStreamType decoder_type,
                       uint32_t request_id);
  void (*DecryptAndDecode)(
      PP_Instance instance,
      PP_DecryptorStreamType decoder_type,
      PP_Resource encrypted_buffer,
      const struct PP_EncryptedBlockInfo* encrypted_block_info);
};

typedef struct PPP_ContentDecryptor_Private_0_9 PPP_ContentDecryptor_Private;

#endif  

