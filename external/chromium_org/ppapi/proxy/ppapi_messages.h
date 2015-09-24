// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <string>
#include <vector>

#include "base/basictypes.h"
#include "base/files/file_path.h"
#include "base/memory/shared_memory.h"
#include "base/process/process.h"
#include "base/strings/string16.h"
#include "base/sync_socket.h"
#include "base/values.h"
#include "gpu/command_buffer/common/command_buffer.h"
#include "gpu/ipc/gpu_command_buffer_traits.h"
#include "ipc/ipc_channel_handle.h"
#include "ipc/ipc_message_macros.h"
#include "ipc/ipc_message_utils.h"
#include "ipc/ipc_platform_file.h"
#include "ppapi/c/dev/pp_video_capture_dev.h"
#include "ppapi/c/dev/pp_video_dev.h"
#include "ppapi/c/dev/ppb_graphics_2d_dev.h"
#include "ppapi/c/dev/ppb_truetype_font_dev.h"
#include "ppapi/c/dev/ppb_url_util_dev.h"
#include "ppapi/c/dev/ppp_printing_dev.h"
#include "ppapi/c/pp_bool.h"
#include "ppapi/c/pp_file_info.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_point.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_size.h"
#include "ppapi/c/pp_time.h"
#include "ppapi/c/ppb_audio_config.h"
#include "ppapi/c/ppb_image_data.h"
#include "ppapi/c/ppb_tcp_socket.h"
#include "ppapi/c/ppb_text_input_controller.h"
#include "ppapi/c/ppb_udp_socket.h"
#include "ppapi/c/private/pp_content_decryptor.h"
#include "ppapi/c/private/pp_private_font_charset.h"
#include "ppapi/c/private/ppb_flash.h"
#include "ppapi/c/private/ppb_host_resolver_private.h"
#include "ppapi/c/private/ppb_isolated_file_system_private.h"
#include "ppapi/c/private/ppb_net_address_private.h"
#include "ppapi/c/private/ppb_pdf.h"
#include "ppapi/c/private/ppb_talk_private.h"
#include "ppapi/c/private/ppp_flash_browser_operations.h"
#include "ppapi/proxy/host_resolver_private_resource.h"
#include "ppapi/proxy/network_list_resource.h"
#include "ppapi/proxy/ppapi_param_traits.h"
#include "ppapi/proxy/ppapi_proxy_export.h"
#include "ppapi/proxy/resource_message_params.h"
#include "ppapi/proxy/serialized_flash_menu.h"
#include "ppapi/proxy/serialized_handle.h"
#include "ppapi/proxy/serialized_structs.h"
#include "ppapi/proxy/serialized_var.h"
#include "ppapi/shared_impl/dir_contents.h"
#include "ppapi/shared_impl/file_path.h"
#include "ppapi/shared_impl/file_ref_create_info.h"
#include "ppapi/shared_impl/ppapi_nacl_channel_args.h"
#include "ppapi/shared_impl/ppapi_preferences.h"
#include "ppapi/shared_impl/ppb_device_ref_shared.h"
#include "ppapi/shared_impl/ppb_input_event_shared.h"
#include "ppapi/shared_impl/ppb_tcp_socket_shared.h"
#include "ppapi/shared_impl/ppb_view_shared.h"
#include "ppapi/shared_impl/ppp_flash_browser_operations_shared.h"
#include "ppapi/shared_impl/private/ppb_x509_certificate_private_shared.h"
#include "ppapi/shared_impl/socket_option_data.h"
#include "ppapi/shared_impl/url_request_info_data.h"
#include "ppapi/shared_impl/url_response_info_data.h"

#undef IPC_MESSAGE_EXPORT
#define IPC_MESSAGE_EXPORT PPAPI_PROXY_EXPORT

#define IPC_MESSAGE_START PpapiMsgStart

IPC_ENUM_TRAITS_MAX_VALUE(ppapi::TCPSocketVersion,
                          ppapi::TCP_SOCKET_VERSION_1_1_OR_ABOVE)
IPC_ENUM_TRAITS(PP_AudioSampleRate)
IPC_ENUM_TRAITS(PP_DeviceType_Dev)
IPC_ENUM_TRAITS(PP_DecryptorStreamType)
IPC_ENUM_TRAITS_MAX_VALUE(PP_FileSystemType, PP_FILESYSTEMTYPE_ISOLATED)
IPC_ENUM_TRAITS_MAX_VALUE(PP_FileType, PP_FILETYPE_OTHER)
IPC_ENUM_TRAITS(PP_Flash_BrowserOperations_Permission)
IPC_ENUM_TRAITS(PP_Flash_BrowserOperations_SettingType)
IPC_ENUM_TRAITS(PP_FlashSetting)
IPC_ENUM_TRAITS_MAX_VALUE(PP_Graphics2D_Dev_ResizeMode,
                          PP_GRAPHICS2D_DEV_RESIZEMODE_STRETCH)
IPC_ENUM_TRAITS(PP_ImageDataFormat)
IPC_ENUM_TRAITS(PP_InputEvent_MouseButton)
IPC_ENUM_TRAITS(PP_InputEvent_Type)
IPC_ENUM_TRAITS_MAX_VALUE(PP_IsolatedFileSystemType_Private,
                          PP_ISOLATEDFILESYSTEMTYPE_PRIVATE_PLUGINPRIVATE)
IPC_ENUM_TRAITS_MAX_VALUE(PP_NetAddressFamily_Private,
                          PP_NETADDRESSFAMILY_PRIVATE_IPV6)
IPC_ENUM_TRAITS_MAX_VALUE(PP_NetworkList_State, PP_NETWORKLIST_STATE_UP)
IPC_ENUM_TRAITS_MAX_VALUE(PP_NetworkList_Type, PP_NETWORKLIST_TYPE_CELLULAR)
IPC_ENUM_TRAITS(PP_PrintOrientation_Dev)
IPC_ENUM_TRAITS(PP_PrintOutputFormat_Dev)
IPC_ENUM_TRAITS(PP_PrintScalingOption_Dev)
IPC_ENUM_TRAITS(PP_PrivateFontCharset)
IPC_ENUM_TRAITS(PP_ResourceImage)
IPC_ENUM_TRAITS(PP_ResourceString)
IPC_ENUM_TRAITS_MAX_VALUE(PP_TalkEvent, PP_TALKEVENT_NUM_EVENTS - 1)
IPC_ENUM_TRAITS_MAX_VALUE(PP_TalkPermission,
                          PP_TALKPERMISSION_NUM_PERMISSIONS - 1)
IPC_ENUM_TRAITS_MAX_VALUE(PP_TCPSocket_Option,
                          PP_TCPSOCKET_OPTION_RECV_BUFFER_SIZE)
IPC_ENUM_TRAITS(PP_TextInput_Type)
IPC_ENUM_TRAITS(PP_TrueTypeFontFamily_Dev)
IPC_ENUM_TRAITS(PP_TrueTypeFontStyle_Dev)
IPC_ENUM_TRAITS(PP_TrueTypeFontWeight_Dev)
IPC_ENUM_TRAITS(PP_TrueTypeFontWidth_Dev)
IPC_ENUM_TRAITS(PP_TrueTypeFontCharset_Dev)
IPC_ENUM_TRAITS_MAX_VALUE(PP_UDPSocket_Option,
                          PP_UDPSOCKET_OPTION_RECV_BUFFER_SIZE)
IPC_ENUM_TRAITS(PP_VideoDecodeError_Dev)
IPC_ENUM_TRAITS(PP_VideoDecoder_Profile)

IPC_STRUCT_TRAITS_BEGIN(PP_Point)
  IPC_STRUCT_TRAITS_MEMBER(x)
  IPC_STRUCT_TRAITS_MEMBER(y)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(PP_FloatPoint)
  IPC_STRUCT_TRAITS_MEMBER(x)
  IPC_STRUCT_TRAITS_MEMBER(y)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(PP_Size)
  IPC_STRUCT_TRAITS_MEMBER(height)
  IPC_STRUCT_TRAITS_MEMBER(width)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(PP_Rect)
  IPC_STRUCT_TRAITS_MEMBER(point)
  IPC_STRUCT_TRAITS_MEMBER(size)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(PP_ImageDataDesc)
  IPC_STRUCT_TRAITS_MEMBER(format)
  IPC_STRUCT_TRAITS_MEMBER(size)
  IPC_STRUCT_TRAITS_MEMBER(stride)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(PP_PictureBuffer_Dev)
  IPC_STRUCT_TRAITS_MEMBER(id)
  IPC_STRUCT_TRAITS_MEMBER(size)
  IPC_STRUCT_TRAITS_MEMBER(texture_id)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(PP_Picture_Dev)
  IPC_STRUCT_TRAITS_MEMBER(picture_buffer_id)
  IPC_STRUCT_TRAITS_MEMBER(bitstream_buffer_id)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(PP_PrintPageNumberRange_Dev)
  IPC_STRUCT_TRAITS_MEMBER(first_page_number)
  IPC_STRUCT_TRAITS_MEMBER(last_page_number)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(PP_VideoCaptureDeviceInfo_Dev)
  IPC_STRUCT_TRAITS_MEMBER(width)
  IPC_STRUCT_TRAITS_MEMBER(height)
  IPC_STRUCT_TRAITS_MEMBER(frames_per_second)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(PP_HostResolver_Private_Hint)
  IPC_STRUCT_TRAITS_MEMBER(family)
  IPC_STRUCT_TRAITS_MEMBER(flags)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(PP_PrintSettings_Dev)
  IPC_STRUCT_TRAITS_MEMBER(printable_area)
  IPC_STRUCT_TRAITS_MEMBER(content_area)
  IPC_STRUCT_TRAITS_MEMBER(paper_size)
  IPC_STRUCT_TRAITS_MEMBER(dpi)
  IPC_STRUCT_TRAITS_MEMBER(orientation)
  IPC_STRUCT_TRAITS_MEMBER(print_scaling_option)
  IPC_STRUCT_TRAITS_MEMBER(grayscale)
  IPC_STRUCT_TRAITS_MEMBER(format)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(PP_URLComponent_Dev)
  IPC_STRUCT_TRAITS_MEMBER(begin)
  IPC_STRUCT_TRAITS_MEMBER(len)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(PP_URLComponents_Dev)
  IPC_STRUCT_TRAITS_MEMBER(scheme)
  IPC_STRUCT_TRAITS_MEMBER(username)
  IPC_STRUCT_TRAITS_MEMBER(password)
  IPC_STRUCT_TRAITS_MEMBER(host)
  IPC_STRUCT_TRAITS_MEMBER(port)
  IPC_STRUCT_TRAITS_MEMBER(path)
  IPC_STRUCT_TRAITS_MEMBER(query)
  IPC_STRUCT_TRAITS_MEMBER(ref)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(PP_FileInfo)
  IPC_STRUCT_TRAITS_MEMBER(size)
  IPC_STRUCT_TRAITS_MEMBER(type)
  IPC_STRUCT_TRAITS_MEMBER(system_type)
  IPC_STRUCT_TRAITS_MEMBER(creation_time)
  IPC_STRUCT_TRAITS_MEMBER(last_access_time)
  IPC_STRUCT_TRAITS_MEMBER(last_modified_time)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(ppapi::DeviceRefData)
  IPC_STRUCT_TRAITS_MEMBER(type)
  IPC_STRUCT_TRAITS_MEMBER(name)
  IPC_STRUCT_TRAITS_MEMBER(id)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(ppapi::DirEntry)
  IPC_STRUCT_TRAITS_MEMBER(name)
  IPC_STRUCT_TRAITS_MEMBER(is_dir)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(ppapi::FileRefCreateInfo)
  IPC_STRUCT_TRAITS_MEMBER(file_system_type)
  IPC_STRUCT_TRAITS_MEMBER(internal_path)
  IPC_STRUCT_TRAITS_MEMBER(display_name)
  IPC_STRUCT_TRAITS_MEMBER(browser_pending_host_resource_id)
  IPC_STRUCT_TRAITS_MEMBER(renderer_pending_host_resource_id)
  IPC_STRUCT_TRAITS_MEMBER(file_system_plugin_resource)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(ppapi::FlashSiteSetting)
  IPC_STRUCT_TRAITS_MEMBER(site)
  IPC_STRUCT_TRAITS_MEMBER(permission)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(ppapi::ViewData)
  IPC_STRUCT_TRAITS_MEMBER(rect)
  IPC_STRUCT_TRAITS_MEMBER(is_fullscreen)
  IPC_STRUCT_TRAITS_MEMBER(is_page_visible)
  IPC_STRUCT_TRAITS_MEMBER(clip_rect)
  IPC_STRUCT_TRAITS_MEMBER(device_scale)
  IPC_STRUCT_TRAITS_MEMBER(css_scale)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(PP_TouchPoint)
  IPC_STRUCT_TRAITS_MEMBER(id)
  IPC_STRUCT_TRAITS_MEMBER(position)
  IPC_STRUCT_TRAITS_MEMBER(radius)
  IPC_STRUCT_TRAITS_MEMBER(rotation_angle)
  IPC_STRUCT_TRAITS_MEMBER(pressure)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(ppapi::Preferences)
  IPC_STRUCT_TRAITS_MEMBER(standard_font_family_map)
  IPC_STRUCT_TRAITS_MEMBER(fixed_font_family_map)
  IPC_STRUCT_TRAITS_MEMBER(serif_font_family_map)
  IPC_STRUCT_TRAITS_MEMBER(sans_serif_font_family_map)
  IPC_STRUCT_TRAITS_MEMBER(default_font_size)
  IPC_STRUCT_TRAITS_MEMBER(default_fixed_font_size)
  IPC_STRUCT_TRAITS_MEMBER(number_of_cpu_cores)
  IPC_STRUCT_TRAITS_MEMBER(is_3d_supported)
  IPC_STRUCT_TRAITS_MEMBER(is_stage3d_supported)
  IPC_STRUCT_TRAITS_MEMBER(is_stage3d_baseline_supported)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(ppapi::InputEventData)
  IPC_STRUCT_TRAITS_MEMBER(is_filtered)
  IPC_STRUCT_TRAITS_MEMBER(event_type)
  IPC_STRUCT_TRAITS_MEMBER(event_time_stamp)
  IPC_STRUCT_TRAITS_MEMBER(event_modifiers)
  IPC_STRUCT_TRAITS_MEMBER(mouse_button)
  IPC_STRUCT_TRAITS_MEMBER(mouse_position)
  IPC_STRUCT_TRAITS_MEMBER(mouse_click_count)
  IPC_STRUCT_TRAITS_MEMBER(mouse_movement)
  IPC_STRUCT_TRAITS_MEMBER(wheel_delta)
  IPC_STRUCT_TRAITS_MEMBER(wheel_ticks)
  IPC_STRUCT_TRAITS_MEMBER(wheel_scroll_by_page)
  IPC_STRUCT_TRAITS_MEMBER(key_code)
  IPC_STRUCT_TRAITS_MEMBER(usb_key_code)
  IPC_STRUCT_TRAITS_MEMBER(code)
  IPC_STRUCT_TRAITS_MEMBER(character_text)
  IPC_STRUCT_TRAITS_MEMBER(composition_segment_offsets)
  IPC_STRUCT_TRAITS_MEMBER(composition_target_segment)
  IPC_STRUCT_TRAITS_MEMBER(composition_selection_start)
  IPC_STRUCT_TRAITS_MEMBER(composition_selection_end)
  IPC_STRUCT_TRAITS_MEMBER(touches)
  IPC_STRUCT_TRAITS_MEMBER(changed_touches)
  IPC_STRUCT_TRAITS_MEMBER(target_touches)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(ppapi::HostPortPair)
  IPC_STRUCT_TRAITS_MEMBER(host)
  IPC_STRUCT_TRAITS_MEMBER(port)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(ppapi::URLRequestInfoData)
  IPC_STRUCT_TRAITS_MEMBER(url)
  IPC_STRUCT_TRAITS_MEMBER(method)
  IPC_STRUCT_TRAITS_MEMBER(headers)
  IPC_STRUCT_TRAITS_MEMBER(stream_to_file)
  IPC_STRUCT_TRAITS_MEMBER(follow_redirects)
  IPC_STRUCT_TRAITS_MEMBER(record_download_progress)
  IPC_STRUCT_TRAITS_MEMBER(record_upload_progress)
  IPC_STRUCT_TRAITS_MEMBER(has_custom_referrer_url)
  IPC_STRUCT_TRAITS_MEMBER(custom_referrer_url)
  IPC_STRUCT_TRAITS_MEMBER(allow_cross_origin_requests)
  IPC_STRUCT_TRAITS_MEMBER(allow_credentials)
  IPC_STRUCT_TRAITS_MEMBER(has_custom_content_transfer_encoding)
  IPC_STRUCT_TRAITS_MEMBER(custom_content_transfer_encoding)
  IPC_STRUCT_TRAITS_MEMBER(prefetch_buffer_upper_threshold)
  IPC_STRUCT_TRAITS_MEMBER(prefetch_buffer_lower_threshold)
  IPC_STRUCT_TRAITS_MEMBER(has_custom_user_agent)
  IPC_STRUCT_TRAITS_MEMBER(custom_user_agent)
  IPC_STRUCT_TRAITS_MEMBER(body)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(ppapi::URLRequestInfoData::BodyItem)
  IPC_STRUCT_TRAITS_MEMBER(is_file)
  IPC_STRUCT_TRAITS_MEMBER(data)
  IPC_STRUCT_TRAITS_MEMBER(file_ref_pp_resource)
  IPC_STRUCT_TRAITS_MEMBER(start_offset)
  IPC_STRUCT_TRAITS_MEMBER(number_of_bytes)
  IPC_STRUCT_TRAITS_MEMBER(expected_last_modified_time)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(ppapi::URLResponseInfoData)
  IPC_STRUCT_TRAITS_MEMBER(url)
  IPC_STRUCT_TRAITS_MEMBER(headers)
  IPC_STRUCT_TRAITS_MEMBER(status_code)
  IPC_STRUCT_TRAITS_MEMBER(status_text)
  IPC_STRUCT_TRAITS_MEMBER(redirect_url)
  IPC_STRUCT_TRAITS_MEMBER(body_as_file_ref)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(ppapi::proxy::SerializedNetworkInfo)
  IPC_STRUCT_TRAITS_MEMBER(name)
  IPC_STRUCT_TRAITS_MEMBER(type)
  IPC_STRUCT_TRAITS_MEMBER(state)
  IPC_STRUCT_TRAITS_MEMBER(addresses)
  IPC_STRUCT_TRAITS_MEMBER(display_name)
  IPC_STRUCT_TRAITS_MEMBER(mtu)
IPC_STRUCT_TRAITS_END()

IPC_STRUCT_TRAITS_BEGIN(ppapi::PpapiNaClChannelArgs)
  IPC_STRUCT_TRAITS_MEMBER(off_the_record)
  IPC_STRUCT_TRAITS_MEMBER(permissions)
  IPC_STRUCT_TRAITS_MEMBER(switch_names)
  IPC_STRUCT_TRAITS_MEMBER(switch_values)
IPC_STRUCT_TRAITS_END()

#if !defined(OS_NACL) && !defined(NACL_WIN64)

IPC_STRUCT_TRAITS_BEGIN(ppapi::proxy::PPPDecryptor_Buffer)
  IPC_STRUCT_TRAITS_MEMBER(resource)
  IPC_STRUCT_TRAITS_MEMBER(handle)
  IPC_STRUCT_TRAITS_MEMBER(size)
IPC_STRUCT_TRAITS_END()

#endif  

IPC_MESSAGE_CONTROL3(PpapiMsg_LoadPlugin,
                     base::FilePath ,
                     ppapi::PpapiPermissions ,
                     bool )

IPC_MESSAGE_CONTROL3(PpapiMsg_CreateChannel,
                     base::ProcessId ,
                     int ,
                     bool )

IPC_MESSAGE_CONTROL3(PpapiMsg_CreateNaClChannel,
                     int ,
                     ppapi::PpapiNaClChannelArgs ,
                     ppapi::proxy::SerializedHandle )

IPC_MESSAGE_CONTROL0(PpapiMsg_Crash)

IPC_MESSAGE_CONTROL0(PpapiMsg_Hang)

IPC_SYNC_MESSAGE_CONTROL1_1(PpapiMsg_ReserveInstanceId,
                            PP_Instance ,
                            bool )

IPC_MESSAGE_CONTROL1(PpapiMsg_SetPreferences,
                     ppapi::Preferences)

IPC_SYNC_MESSAGE_CONTROL1_1(PpapiMsg_SupportsInterface,
                            std::string ,
                            bool )

#if !defined(OS_NACL) && !defined(NACL_WIN64)
IPC_MESSAGE_CONTROL1(PpapiMsg_SetNetworkState,
                     bool )

IPC_MESSAGE_CONTROL2(PpapiMsg_GetSitesWithData,
                     uint32 ,
                     base::FilePath )
IPC_MESSAGE_CONTROL2(PpapiHostMsg_GetSitesWithDataResult,
                     uint32 ,
                     std::vector<std::string> )

IPC_MESSAGE_CONTROL5(PpapiMsg_ClearSiteData,
                     uint32 ,
                     base::FilePath ,
                     std::string ,
                     uint64 ,
                     uint64 )
IPC_MESSAGE_CONTROL2(PpapiHostMsg_ClearSiteDataResult,
                     uint32 ,
                     bool )

IPC_MESSAGE_CONTROL2(PpapiMsg_DeauthorizeContentLicenses,
                     uint32 ,
                     base::FilePath )
IPC_MESSAGE_CONTROL2(PpapiHostMsg_DeauthorizeContentLicensesResult,
                     uint32 ,
                     bool )

IPC_MESSAGE_CONTROL3(PpapiMsg_GetPermissionSettings,
                     uint32 ,
                     base::FilePath ,
                     PP_Flash_BrowserOperations_SettingType )
IPC_MESSAGE_CONTROL4(
    PpapiHostMsg_GetPermissionSettingsResult,
    uint32 ,
    bool ,
    PP_Flash_BrowserOperations_Permission ,
    ppapi::FlashSiteSettings )

IPC_MESSAGE_CONTROL5(PpapiMsg_SetDefaultPermission,
                     uint32 ,
                     base::FilePath ,
                     PP_Flash_BrowserOperations_SettingType ,
                     PP_Flash_BrowserOperations_Permission ,
                     bool )
IPC_MESSAGE_CONTROL2(PpapiHostMsg_SetDefaultPermissionResult,
                     uint32 ,
                     bool )

IPC_MESSAGE_CONTROL4(PpapiMsg_SetSitePermission,
                     uint32 ,
                     base::FilePath ,
                     PP_Flash_BrowserOperations_SettingType ,
                     ppapi::FlashSiteSettings )
IPC_MESSAGE_CONTROL2(PpapiHostMsg_SetSitePermissionResult,
                     uint32 ,
                     bool )

IPC_SYNC_MESSAGE_CONTROL2_1(PpapiMsg_ConnectToPlugin,
                            PP_Instance ,
                            IPC::PlatformFileForTransit ,
                            int32_t )
#endif  


IPC_MESSAGE_ROUTED4(PpapiMsg_PPBAudio_NotifyAudioStreamCreated,
                    ppapi::HostResource ,
                    int32_t ,
                    ppapi::proxy::SerializedHandle ,
                    ppapi::proxy::SerializedHandle )

IPC_MESSAGE_ROUTED2(PpapiMsg_PPBGraphics3D_SwapBuffersACK,
                    ppapi::HostResource ,
                    int32_t )

IPC_MESSAGE_ROUTED1(PpapiMsg_PPBImageData_NotifyUnusedImageData,
                    ppapi::HostResource )

IPC_MESSAGE_ROUTED2(PpapiMsg_PPBInstance_MouseLockComplete,
                    PP_Instance ,
                    int32_t )

IPC_SYNC_MESSAGE_ROUTED3_2(PpapiMsg_PPPClass_HasProperty,
                           int64 ,
                           int64 ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar ,
                           bool )
IPC_SYNC_MESSAGE_ROUTED3_2(PpapiMsg_PPPClass_HasMethod,
                           int64 ,
                           int64 ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar ,
                           bool )
IPC_SYNC_MESSAGE_ROUTED3_2(PpapiMsg_PPPClass_GetProperty,
                           int64 ,
                           int64 ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar )
IPC_SYNC_MESSAGE_ROUTED2_2(PpapiMsg_PPPClass_EnumerateProperties,
                           int64 ,
                           int64 ,
                           std::vector<ppapi::proxy::SerializedVar> ,
                           ppapi::proxy::SerializedVar )
IPC_SYNC_MESSAGE_ROUTED4_1(PpapiMsg_PPPClass_SetProperty,
                           int64 ,
                           int64 ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar )
IPC_SYNC_MESSAGE_ROUTED3_1(PpapiMsg_PPPClass_RemoveProperty,
                           int64 ,
                           int64 ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar )
IPC_SYNC_MESSAGE_ROUTED4_2(PpapiMsg_PPPClass_Call,
                           int64 ,
                           int64 ,
                           ppapi::proxy::SerializedVar ,
                           std::vector<ppapi::proxy::SerializedVar> ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar )
IPC_SYNC_MESSAGE_ROUTED3_2(PpapiMsg_PPPClass_Construct,
                           int64 ,
                           int64 ,
                           std::vector<ppapi::proxy::SerializedVar> ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar )
IPC_MESSAGE_ROUTED2(PpapiMsg_PPPClass_Deallocate,
                    int64 ,
                    int64 )

IPC_MESSAGE_ROUTED1(PpapiMsg_PPPGraphics3D_ContextLost,
                    PP_Instance )

IPC_MESSAGE_ROUTED2(PpapiMsg_PPPInputEvent_HandleInputEvent,
                    PP_Instance ,
                    ppapi::InputEventData )
IPC_SYNC_MESSAGE_ROUTED2_1(PpapiMsg_PPPInputEvent_HandleFilteredInputEvent,
                           PP_Instance ,
                           ppapi::InputEventData ,
                           PP_Bool )

IPC_SYNC_MESSAGE_ROUTED3_1(PpapiMsg_PPPInstance_DidCreate,
                           PP_Instance ,
                           std::vector<std::string> ,
                           std::vector<std::string> ,
                           PP_Bool )
IPC_SYNC_MESSAGE_ROUTED1_0(PpapiMsg_PPPInstance_DidDestroy,
                           PP_Instance )
IPC_MESSAGE_ROUTED3(PpapiMsg_PPPInstance_DidChangeView,
                    PP_Instance ,
                    ppapi::ViewData ,
                    PP_Bool )
IPC_MESSAGE_ROUTED2(PpapiMsg_PPPInstance_DidChangeFocus,
                    PP_Instance ,
                    PP_Bool )
IPC_MESSAGE_ROUTED3(PpapiMsg_PPPInstance_HandleDocumentLoad,
    PP_Instance ,
    int ,
    ppapi::URLResponseInfoData )

IPC_MESSAGE_ROUTED2(PpapiMsg_PPPMessaging_HandleMessage,
                    PP_Instance ,
                    ppapi::proxy::SerializedVar )

IPC_MESSAGE_ROUTED1(PpapiMsg_PPPMouseLock_MouseLockLost,
                    PP_Instance )

IPC_SYNC_MESSAGE_ROUTED1_1(PpapiMsg_PPPPrinting_QuerySupportedFormats,
                           PP_Instance ,
                           uint32_t )
IPC_SYNC_MESSAGE_ROUTED2_1(PpapiMsg_PPPPrinting_Begin,
                           PP_Instance ,
                           std::string ,
                           int32_t )
IPC_SYNC_MESSAGE_ROUTED2_1(PpapiMsg_PPPPrinting_PrintPages,
                           PP_Instance ,
                           std::vector<PP_PrintPageNumberRange_Dev> ,
                           ppapi::HostResource )
IPC_MESSAGE_ROUTED1(PpapiMsg_PPPPrinting_End,
                    PP_Instance )
IPC_SYNC_MESSAGE_ROUTED1_1(PpapiMsg_PPPPrinting_IsScalingDisabled,
                           PP_Instance ,
                           bool )

IPC_MESSAGE_ROUTED2(PpapiMsg_PPPTextInput_RequestSurroundingText,
                   PP_Instance ,
                   uint32_t )

#if !defined(OS_NACL) && !defined(NACL_WIN64)
IPC_MESSAGE_ROUTED3(
    PpapiMsg_PPBBroker_ConnectComplete,
    ppapi::HostResource ,
    IPC::PlatformFileForTransit ,
    int32_t )

IPC_MESSAGE_ROUTED2(PpapiMsg_PPPContentDecryptor_Initialize,
                    PP_Instance ,
                    ppapi::proxy::SerializedVar )
IPC_MESSAGE_ROUTED4(PpapiMsg_PPPContentDecryptor_CreateSession,
                    PP_Instance ,
                    uint32_t ,
                    ppapi::proxy::SerializedVar ,
                    ppapi::proxy::SerializedVar )
IPC_MESSAGE_ROUTED3(PpapiMsg_PPPContentDecryptor_UpdateSession,
                    PP_Instance ,
                    uint32_t ,
                    ppapi::proxy::SerializedVar )
IPC_MESSAGE_ROUTED2(PpapiMsg_PPPContentDecryptor_ReleaseSession,
                    PP_Instance ,
                    uint32_t )
IPC_MESSAGE_ROUTED3(PpapiMsg_PPPContentDecryptor_Decrypt,
                    PP_Instance ,
                    ppapi::proxy::PPPDecryptor_Buffer ,
                    std::string )
IPC_MESSAGE_ROUTED3(
    PpapiMsg_PPPContentDecryptor_InitializeAudioDecoder,
    PP_Instance ,
    std::string ,
    ppapi::proxy::PPPDecryptor_Buffer )
IPC_MESSAGE_ROUTED3(
    PpapiMsg_PPPContentDecryptor_InitializeVideoDecoder,
    PP_Instance ,
    std::string ,
    ppapi::proxy::PPPDecryptor_Buffer )
IPC_MESSAGE_ROUTED3(PpapiMsg_PPPContentDecryptor_DeinitializeDecoder,
                    PP_Instance ,
                    PP_DecryptorStreamType ,
                    uint32_t )
IPC_MESSAGE_ROUTED3(PpapiMsg_PPPContentDecryptor_ResetDecoder,
                    PP_Instance ,
                    PP_DecryptorStreamType ,
                    uint32_t )
IPC_MESSAGE_ROUTED4(PpapiMsg_PPPContentDecryptor_DecryptAndDecode,
                    PP_Instance ,
                    PP_DecryptorStreamType ,
                    ppapi::proxy::PPPDecryptor_Buffer ,
                    std::string )
#endif  

#if !defined(OS_NACL) && !defined(NACL_WIN64)
IPC_SYNC_MESSAGE_ROUTED1_1(PpapiMsg_PPPInstancePrivate_GetInstanceObject,
                           PP_Instance ,
                           ppapi::proxy::SerializedVar )

IPC_MESSAGE_ROUTED3(PpapiMsg_PPBVideoDecoder_EndOfBitstreamACK,
                    ppapi::HostResource ,
                    int32_t ,
                    int32_t )
IPC_MESSAGE_ROUTED2(PpapiMsg_PPBVideoDecoder_FlushACK,
                    ppapi::HostResource ,
                    int32_t )
IPC_MESSAGE_ROUTED2(PpapiMsg_PPBVideoDecoder_ResetACK,
                    ppapi::HostResource ,
                    int32_t )

IPC_MESSAGE_ROUTED4(PpapiMsg_PPPVideoDecoder_ProvidePictureBuffers,
                    ppapi::HostResource ,
                    uint32_t ,
                    PP_Size ,
                    uint32_t )
IPC_MESSAGE_ROUTED2(PpapiMsg_PPPVideoDecoder_DismissPictureBuffer,
                    ppapi::HostResource ,
                    int32_t )
IPC_MESSAGE_ROUTED2(PpapiMsg_PPPVideoDecoder_PictureReady,
                    ppapi::HostResource ,
                    PP_Picture_Dev )
IPC_MESSAGE_ROUTED2(PpapiMsg_PPPVideoDecoder_NotifyError,
                    ppapi::HostResource ,
                    PP_VideoDecodeError_Dev )
#endif  


IPC_MESSAGE_CONTROL1(PpapiHostMsg_ChannelCreated,
                     IPC::ChannelHandle )

IPC_MESSAGE_CONTROL4(PpapiHostMsg_LogWithSource,
                     PP_Instance ,
                     int ,
                     std::string ,
                     std::string )

IPC_SYNC_MESSAGE_ROUTED3_1(PpapiHostMsg_PPBAudio_Create,
                           PP_Instance ,
                           int32_t ,
                           uint32_t ,
                           ppapi::HostResource )
IPC_MESSAGE_ROUTED2(PpapiHostMsg_PPBAudio_StartOrStop,
                    ppapi::HostResource ,
                    bool )

IPC_MESSAGE_ROUTED1(PpapiHostMsg_PPBCore_AddRefResource,
                    ppapi::HostResource)
IPC_MESSAGE_ROUTED1(PpapiHostMsg_PPBCore_ReleaseResource,
                    ppapi::HostResource)

IPC_SYNC_MESSAGE_ROUTED3_1(PpapiHostMsg_PPBGraphics3D_Create,
                           PP_Instance ,
                           ppapi::HostResource ,
                           std::vector<int32_t> ,
                           ppapi::HostResource )
IPC_SYNC_MESSAGE_ROUTED2_0(PpapiHostMsg_PPBGraphics3D_SetGetBuffer,
                           ppapi::HostResource ,
                           int32 )
IPC_SYNC_MESSAGE_ROUTED1_2(PpapiHostMsg_PPBGraphics3D_GetState,
                           ppapi::HostResource ,
                           gpu::CommandBuffer::State ,
                           bool )
IPC_SYNC_MESSAGE_ROUTED3_2(PpapiHostMsg_PPBGraphics3D_Flush,
                           ppapi::HostResource ,
                           int32 ,
                           int32 ,
                           gpu::CommandBuffer::State ,
                           bool )
IPC_MESSAGE_ROUTED2(PpapiHostMsg_PPBGraphics3D_AsyncFlush,
                    ppapi::HostResource ,
                    int32 )
IPC_SYNC_MESSAGE_ROUTED2_1(PpapiHostMsg_PPBGraphics3D_CreateTransferBuffer,
                           ppapi::HostResource ,
                           uint32 ,
                           int32 )
IPC_SYNC_MESSAGE_ROUTED2_0(PpapiHostMsg_PPBGraphics3D_DestroyTransferBuffer,
                           ppapi::HostResource ,
                           int32 )
IPC_SYNC_MESSAGE_ROUTED2_1(PpapiHostMsg_PPBGraphics3D_GetTransferBuffer,
                           ppapi::HostResource ,
                           int32 ,
                           ppapi::proxy::SerializedHandle )
IPC_MESSAGE_ROUTED1(PpapiHostMsg_PPBGraphics3D_SwapBuffers,
                    ppapi::HostResource )
IPC_SYNC_MESSAGE_ROUTED1_1(PpapiHostMsg_PPBGraphics3D_InsertSyncPoint,
                           ppapi::HostResource ,
                           uint32 )

IPC_SYNC_MESSAGE_ROUTED4_3(PpapiHostMsg_PPBImageData_CreatePlatform,
                           PP_Instance ,
                           int32 ,
                           PP_Size ,
                           PP_Bool ,
                           ppapi::HostResource ,
                           PP_ImageDataDesc ,
                           ppapi::proxy::ImageHandle )
IPC_SYNC_MESSAGE_ROUTED4_3(PpapiHostMsg_PPBImageData_CreateSimple,
                           PP_Instance ,
                           int32 ,
                           PP_Size ,
                           PP_Bool ,
                           ppapi::HostResource ,
                           PP_ImageDataDesc ,
                           ppapi::proxy::SerializedHandle )

IPC_SYNC_MESSAGE_ROUTED1_1(PpapiHostMsg_PPBInstance_GetWindowObject,
                           PP_Instance ,
                           ppapi::proxy::SerializedVar )
IPC_SYNC_MESSAGE_ROUTED1_1(PpapiHostMsg_PPBInstance_GetOwnerElementObject,
                           PP_Instance ,
                           ppapi::proxy::SerializedVar )
IPC_MESSAGE_ROUTED2(PpapiHostMsg_PPBInstance_BindGraphics,
                    PP_Instance ,
                    PP_Resource )
IPC_SYNC_MESSAGE_ROUTED1_1(
    PpapiHostMsg_PPBInstance_GetAudioHardwareOutputSampleRate,
                           PP_Instance ,
                           uint32_t )
IPC_SYNC_MESSAGE_ROUTED1_1(
    PpapiHostMsg_PPBInstance_GetAudioHardwareOutputBufferSize,
                           PP_Instance ,
                           uint32_t )
IPC_SYNC_MESSAGE_ROUTED1_1(PpapiHostMsg_PPBInstance_IsFullFrame,
                           PP_Instance ,
                           PP_Bool )
IPC_SYNC_MESSAGE_ROUTED2_2(PpapiHostMsg_PPBInstance_ExecuteScript,
                           PP_Instance ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar )
IPC_SYNC_MESSAGE_ROUTED1_1(PpapiHostMsg_PPBInstance_GetDefaultCharSet,
                           PP_Instance ,
                           ppapi::proxy::SerializedVar )
IPC_SYNC_MESSAGE_ROUTED2_1(PpapiHostMsg_PPBInstance_SetFullscreen,
                           PP_Instance ,
                           PP_Bool ,
                           PP_Bool )
IPC_SYNC_MESSAGE_ROUTED1_2(PpapiHostMsg_PPBInstance_GetScreenSize,
                           PP_Instance ,
                           PP_Bool ,
                           PP_Size )
IPC_MESSAGE_ROUTED3(PpapiHostMsg_PPBInstance_RequestInputEvents,
                    PP_Instance ,
                    bool ,
                    uint32_t )
IPC_MESSAGE_ROUTED2(PpapiHostMsg_PPBInstance_ClearInputEvents,
                    PP_Instance ,
                    uint32_t )
IPC_MESSAGE_ROUTED2(PpapiHostMsg_PPBInstance_PostMessage,
                    PP_Instance ,
                    ppapi::proxy::SerializedVar )
IPC_MESSAGE_ROUTED1(PpapiHostMsg_PPBInstance_LockMouse,
                    PP_Instance )
IPC_MESSAGE_ROUTED1(PpapiHostMsg_PPBInstance_UnlockMouse,
                    PP_Instance )
IPC_SYNC_MESSAGE_ROUTED2_1(PpapiHostMsg_PPBInstance_ResolveRelativeToDocument,
                           PP_Instance ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar )
IPC_SYNC_MESSAGE_ROUTED2_1(PpapiHostMsg_PPBInstance_DocumentCanRequest,
                           PP_Instance ,
                           ppapi::proxy::SerializedVar ,
                           PP_Bool )
IPC_SYNC_MESSAGE_ROUTED2_1(PpapiHostMsg_PPBInstance_DocumentCanAccessDocument,
                           PP_Instance ,
                           PP_Instance ,
                           PP_Bool )
IPC_SYNC_MESSAGE_ROUTED1_2(PpapiHostMsg_PPBInstance_GetDocumentURL,
                           PP_Instance ,
                           PP_URLComponents_Dev ,
                           ppapi::proxy::SerializedVar )
IPC_SYNC_MESSAGE_ROUTED1_1(PpapiHostMsg_PPBInstance_GetPluginInstanceURL,
                           PP_Instance ,
                           ppapi::proxy::SerializedVar )
IPC_SYNC_MESSAGE_ROUTED1_1(PpapiHostMsg_PPBInstance_GetPluginReferrerURL,
                           PP_Instance ,
                           ppapi::proxy::SerializedVar )
IPC_MESSAGE_ROUTED4(PpapiHostMsg_PPBInstance_SetCursor,
                    PP_Instance ,
                    int32_t ,
                    ppapi::HostResource ,
                    PP_Point )
IPC_MESSAGE_ROUTED2(PpapiHostMsg_PPBInstance_SetTextInputType,
                    PP_Instance ,
                    PP_TextInput_Type )
IPC_MESSAGE_ROUTED3(PpapiHostMsg_PPBInstance_UpdateCaretPosition,
                    PP_Instance ,
                    PP_Rect ,
                    PP_Rect )
IPC_MESSAGE_ROUTED1(PpapiHostMsg_PPBInstance_CancelCompositionText,
                    PP_Instance )
IPC_MESSAGE_ROUTED4(PpapiHostMsg_PPBInstance_UpdateSurroundingText,
                    PP_Instance ,
                    std::string ,
                    uint32_t ,
                    uint32_t )

IPC_SYNC_MESSAGE_ROUTED1_1(PpapiHostMsg_PPBVar_AddRefObject,
                           int64 ,
                           int )
IPC_MESSAGE_ROUTED1(PpapiHostMsg_PPBVar_ReleaseObject,
                    int64 )
IPC_SYNC_MESSAGE_ROUTED2_2(PpapiHostMsg_PPBVar_HasProperty,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar ,
                           PP_Bool )
IPC_SYNC_MESSAGE_ROUTED2_2(PpapiHostMsg_PPBVar_HasMethodDeprecated,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar ,
                           PP_Bool )
IPC_SYNC_MESSAGE_ROUTED2_2(PpapiHostMsg_PPBVar_GetProperty,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar )
IPC_SYNC_MESSAGE_ROUTED2_2(PpapiHostMsg_PPBVar_DeleteProperty,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar ,
                           PP_Bool )
IPC_SYNC_MESSAGE_ROUTED1_2(PpapiHostMsg_PPBVar_EnumerateProperties,
                           ppapi::proxy::SerializedVar ,
                           std::vector<ppapi::proxy::SerializedVar> ,
                           ppapi::proxy::SerializedVar )
IPC_SYNC_MESSAGE_ROUTED3_1(PpapiHostMsg_PPBVar_SetPropertyDeprecated,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar )
IPC_SYNC_MESSAGE_ROUTED3_2(PpapiHostMsg_PPBVar_CallDeprecated,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar ,
                           std::vector<ppapi::proxy::SerializedVar> ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar )
IPC_SYNC_MESSAGE_ROUTED2_2(PpapiHostMsg_PPBVar_Construct,
                           ppapi::proxy::SerializedVar ,
                           std::vector<ppapi::proxy::SerializedVar> ,
                           ppapi::proxy::SerializedVar ,
                           ppapi::proxy::SerializedVar )
IPC_SYNC_MESSAGE_ROUTED2_2(PpapiHostMsg_PPBVar_IsInstanceOfDeprecated,
                           ppapi::proxy::SerializedVar ,
                           int64 ,
                           int64 ,
                           PP_Bool )
IPC_SYNC_MESSAGE_ROUTED3_1(PpapiHostMsg_PPBVar_CreateObjectDeprecated,
                           PP_Instance ,
                           int64 ,
                           int64 ,
                           ppapi::proxy::SerializedVar )

#if !defined(OS_NACL) && !defined(NACL_WIN64)
IPC_SYNC_MESSAGE_ROUTED1_1(PpapiHostMsg_PPBBroker_Create,
                           PP_Instance ,
                           ppapi::HostResource )
IPC_MESSAGE_ROUTED1(PpapiHostMsg_PPBBroker_Connect,
                    ppapi::HostResource )

IPC_SYNC_MESSAGE_ROUTED2_2(
    PpapiHostMsg_PPBBuffer_Create,
    PP_Instance ,
    uint32_t ,
    ppapi::HostResource ,
    ppapi::proxy::SerializedHandle )

IPC_MESSAGE_ROUTED3(PpapiHostMsg_PPBInstance_SessionCreated,
                    PP_Instance ,
                    uint32_t ,
                    ppapi::proxy::SerializedVar )
IPC_MESSAGE_ROUTED4(PpapiHostMsg_PPBInstance_SessionMessage,
                    PP_Instance ,
                    uint32_t ,
                    ppapi::proxy::SerializedVar ,
                    ppapi::proxy::SerializedVar )
IPC_MESSAGE_ROUTED2(PpapiHostMsg_PPBInstance_SessionReady,
                    PP_Instance ,
                    uint32_t )
IPC_MESSAGE_ROUTED2(PpapiHostMsg_PPBInstance_SessionClosed,
                    PP_Instance ,
                    uint32_t )
IPC_MESSAGE_ROUTED4(PpapiHostMsg_PPBInstance_SessionError,
                    PP_Instance ,
                    uint32_t ,
                    int32_t ,
                    int32_t )
IPC_MESSAGE_ROUTED3(PpapiHostMsg_PPBInstance_DeliverBlock,
                    PP_Instance ,
                    PP_Resource ,
                    std::string )
IPC_MESSAGE_ROUTED4(PpapiHostMsg_PPBInstance_DecoderInitializeDone,
                    PP_Instance ,
                    PP_DecryptorStreamType ,
                    uint32_t ,
                    PP_Bool )
IPC_MESSAGE_ROUTED3(PpapiHostMsg_PPBInstance_DecoderDeinitializeDone,
                    PP_Instance ,
                    PP_DecryptorStreamType ,
                    uint32_t )
IPC_MESSAGE_ROUTED3(PpapiHostMsg_PPBInstance_DecoderResetDone,
                    PP_Instance ,
                    PP_DecryptorStreamType ,
                    uint32_t )
IPC_MESSAGE_ROUTED3(PpapiHostMsg_PPBInstance_DeliverFrame,
                    PP_Instance ,
                    PP_Resource ,
                    std::string )
IPC_MESSAGE_ROUTED3(PpapiHostMsg_PPBInstance_DeliverSamples,
                    PP_Instance ,
                    PP_Resource ,
                    std::string )
#endif  

IPC_SYNC_MESSAGE_ROUTED3_1(
    PpapiHostMsg_PPBTesting_ReadImageData,
    ppapi::HostResource ,
    ppapi::HostResource ,
    PP_Point ,
    PP_Bool )
IPC_SYNC_MESSAGE_ROUTED1_1(PpapiHostMsg_PPBTesting_GetLiveObjectsForInstance,
                           PP_Instance ,
                           uint32 )
IPC_SYNC_MESSAGE_ROUTED2_0(PpapiHostMsg_PPBTesting_SimulateInputEvent,
                           PP_Instance ,
                           ppapi::InputEventData )
IPC_SYNC_MESSAGE_ROUTED1_0(
    PpapiHostMsg_PPBTesting_SetMinimumArrayBufferSizeForShmem,
    uint32_t )

#if !defined(OS_NACL) && !defined(NACL_WIN64)

IPC_SYNC_MESSAGE_ROUTED3_1(PpapiHostMsg_PPBVideoDecoder_Create,
                           PP_Instance ,
                           ppapi::HostResource ,
                           PP_VideoDecoder_Profile ,
                           ppapi::HostResource )
IPC_MESSAGE_ROUTED4(PpapiHostMsg_PPBVideoDecoder_Decode,
                    ppapi::HostResource ,
                    ppapi::HostResource ,
                    int32 ,
                    uint32 )
IPC_MESSAGE_ROUTED2(PpapiHostMsg_PPBVideoDecoder_AssignPictureBuffers,
                    ppapi::HostResource ,
                    std::vector<PP_PictureBuffer_Dev> )
IPC_MESSAGE_ROUTED2(PpapiHostMsg_PPBVideoDecoder_ReusePictureBuffer,
                    ppapi::HostResource ,
                    int32_t )
IPC_MESSAGE_ROUTED1(PpapiHostMsg_PPBVideoDecoder_Flush,
                    ppapi::HostResource )
IPC_MESSAGE_ROUTED1(PpapiHostMsg_PPBVideoDecoder_Reset,
                    ppapi::HostResource )
IPC_SYNC_MESSAGE_ROUTED1_0(PpapiHostMsg_PPBVideoDecoder_Destroy,
                           ppapi::HostResource )

IPC_SYNC_MESSAGE_ROUTED1_1(PpapiHostMsg_PPBFlashMessageLoop_Create,
                           PP_Instance ,
                           ppapi::HostResource )
IPC_SYNC_MESSAGE_ROUTED1_1(PpapiHostMsg_PPBFlashMessageLoop_Run,
                           ppapi::HostResource ,
                           int32_t )
IPC_SYNC_MESSAGE_ROUTED1_0(PpapiHostMsg_PPBFlashMessageLoop_Quit,
                           ppapi::HostResource )
#endif  

IPC_SYNC_MESSAGE_CONTROL1_2(PpapiHostMsg_PPBX509Certificate_ParseDER,
                            std::vector<char> ,
                            bool ,
                            ppapi::PPB_X509Certificate_Fields )


IPC_MESSAGE_CONTROL3(PpapiHostMsg_ResourceCreated,
                     ppapi::proxy::ResourceMessageCallParams ,
                     PP_Instance  ,
                     IPC::Message )

IPC_MESSAGE_CONTROL1(PpapiHostMsg_ResourceDestroyed,
                     PP_Resource )

IPC_MESSAGE_CONTROL2(PpapiHostMsg_AttachToPendingHost,
                     PP_Resource ,
                     int )

IPC_MESSAGE_CONTROL2(PpapiHostMsg_ResourceCall,
                     ppapi::proxy::ResourceMessageCallParams ,
                     IPC::Message )
IPC_MESSAGE_CONTROL3(PpapiHostMsg_InProcessResourceCall,
                     int ,
                     ppapi::proxy::ResourceMessageCallParams ,
                     IPC::Message )

IPC_MESSAGE_CONTROL2(
    PpapiPluginMsg_ResourceReply,
    ppapi::proxy::ResourceMessageReplyParams ,
    IPC::Message )
IPC_MESSAGE_ROUTED2(
    PpapiHostMsg_InProcessResourceReply,
    ppapi::proxy::ResourceMessageReplyParams ,
    IPC::Message )

IPC_SYNC_MESSAGE_CONTROL2_2(PpapiHostMsg_ResourceSyncCall,
    ppapi::proxy::ResourceMessageCallParams ,
    IPC::Message ,
    ppapi::proxy::ResourceMessageReplyParams ,
    IPC::Message )

IPC_MESSAGE_CONTROL5(
    PpapiHostMsg_CreateResourceHostsFromHost,
    int ,
    int ,
    ppapi::proxy::ResourceMessageCallParams ,
    PP_Instance ,
    std::vector<IPC::Message> )

IPC_MESSAGE_ROUTED2(
    PpapiHostMsg_CreateResourceHostsFromHostReply,
    int32_t ,
    std::vector<int> )


IPC_MESSAGE_CONTROL0(PpapiHostMsg_Broker_Create)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_Broker_IsAllowed)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_ExtensionsCommon_Create)

IPC_MESSAGE_CONTROL2(PpapiHostMsg_ExtensionsCommon_Post,
                     std::string ,
                     base::ListValue )

IPC_MESSAGE_CONTROL2(PpapiHostMsg_ExtensionsCommon_Call,
                     std::string ,
                     base::ListValue )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_ExtensionsCommon_CallReply,
                     base::ListValue )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_FileChooser_Create)
IPC_MESSAGE_CONTROL4(PpapiHostMsg_FileChooser_Show,
                     bool ,
                     bool ,
                     std::string ,
                     std::vector<std::string> )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_FileChooser_ShowReply,
                     std::vector<ppapi::FileRefCreateInfo> )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_FileIO_Create)
IPC_MESSAGE_CONTROL2(PpapiHostMsg_FileIO_Open,
                     PP_Resource ,
                     int32_t )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_FileIO_OpenReply)
IPC_MESSAGE_CONTROL0(PpapiHostMsg_FileIO_Close)
IPC_MESSAGE_CONTROL2(PpapiHostMsg_FileIO_Touch,
                     PP_Time ,
                     PP_Time )
IPC_MESSAGE_CONTROL2(PpapiHostMsg_FileIO_Write,
                     int64_t ,
                     std::string )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_FileIO_SetLength,
                     int64_t )
IPC_MESSAGE_CONTROL0(PpapiHostMsg_FileIO_Flush)
IPC_MESSAGE_CONTROL0(PpapiHostMsg_FileIO_RequestOSFileHandle)
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_FileIO_RequestOSFileHandleReply)
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_FileIO_GeneralReply)

IPC_MESSAGE_CONTROL1(PpapiHostMsg_FileRef_CreateExternal,
                     base::FilePath )

IPC_MESSAGE_CONTROL2(PpapiHostMsg_FileRef_CreateInternal,
                     PP_Resource ,
                     std::string )

IPC_MESSAGE_CONTROL1(PpapiHostMsg_FileRef_MakeDirectory,
                     bool )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_FileRef_MakeDirectoryReply)

IPC_MESSAGE_CONTROL2(PpapiHostMsg_FileRef_Touch,
                     PP_Time ,
                     PP_Time )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_FileRef_TouchReply)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_FileRef_Delete)
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_FileRef_DeleteReply)

IPC_MESSAGE_CONTROL1(PpapiHostMsg_FileRef_Rename,
                     PP_Resource )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_FileRef_RenameReply)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_FileRef_Query)
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_FileRef_QueryReply,
                     PP_FileInfo )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_FileRef_ReadDirectoryEntries)

IPC_MESSAGE_CONTROL2(PpapiPluginMsg_FileRef_ReadDirectoryEntriesReply,
                     std::vector<ppapi::FileRefCreateInfo> ,
                     std::vector<PP_FileType> )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_FileRef_GetAbsolutePath)
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_FileRef_GetAbsolutePathReply,
                     std::string )

IPC_MESSAGE_CONTROL1(PpapiHostMsg_FileSystem_Create,
                     PP_FileSystemType )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_FileSystem_Open,
                     int64_t )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_FileSystem_OpenReply)
IPC_MESSAGE_CONTROL2(PpapiHostMsg_FileSystem_InitIsolatedFileSystem,
                     std::string ,
                     PP_IsolatedFileSystemType_Private )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_FileSystem_InitIsolatedFileSystemReply)
IPC_MESSAGE_CONTROL2(PpapiHostMsg_FileSystem_CreateFromRenderer,
                     std::string ,
                     PP_FileSystemType )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_FileSystem_CreateFromPendingHost,
                     PP_FileSystemType )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_FlashDRM_Create)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_FlashDRM_GetDeviceID)
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_FlashDRM_GetDeviceIDReply,
                     std::string )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_FlashDRM_GetHmonitor)
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_FlashDRM_GetHmonitorReply,
                     int64_t )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_FlashDRM_GetVoucherFile)
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_FlashDRM_GetVoucherFileReply,
                     ppapi::FileRefCreateInfo )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_FlashDRM_MonitorIsExternal)
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_FlashDRM_MonitorIsExternalReply,
                     PP_Bool )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_Gamepad_Create)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_Gamepad_RequestMemory)

IPC_MESSAGE_CONTROL0(PpapiPluginMsg_Gamepad_SendMemory)


IPC_MESSAGE_CONTROL2(PpapiHostMsg_Graphics2D_Create,
                     PP_Size ,
                     PP_Bool )
IPC_MESSAGE_CONTROL4(PpapiHostMsg_Graphics2D_PaintImageData,
                     ppapi::HostResource ,
                     PP_Point ,
                     bool ,
                     PP_Rect )
IPC_MESSAGE_CONTROL3(PpapiHostMsg_Graphics2D_Scroll,
                     bool ,
                     PP_Rect ,
                     PP_Point )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_Graphics2D_ReplaceContents,
                     ppapi::HostResource )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_Graphics2D_Dev_SetScale,
                     float )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_Graphics2D_SetOffset,
                     PP_Point )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_Graphics2D_SetResizeMode,
                     PP_Graphics2D_Dev_ResizeMode )

IPC_MESSAGE_CONTROL1(PpapiHostMsg_Graphics2D_Flush,
                     ppapi::ViewData )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_Graphics2D_FlushAck)

IPC_MESSAGE_CONTROL2(PpapiHostMsg_Graphics2D_ReadImageData,
                     PP_Resource ,
                     PP_Point )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_Graphics2D_ReadImageDataAck)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_IsolatedFileSystem_Create)
IPC_MESSAGE_CONTROL1(PpapiHostMsg_IsolatedFileSystem_BrowserOpen,
                     PP_IsolatedFileSystemType_Private )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_IsolatedFileSystem_BrowserOpenReply,
                     std::string )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_NetworkMonitor_Create)
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_NetworkMonitor_NetworkList,
                     ppapi::proxy::SerializedNetworkList )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_NetworkMonitor_Forbidden)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_NetworkProxy_Create)

IPC_MESSAGE_CONTROL1(PpapiHostMsg_NetworkProxy_GetProxyForURL,
                     std::string )

IPC_MESSAGE_CONTROL1(PpapiPluginMsg_NetworkProxy_GetProxyForURLReply,
                     std::string )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_TrueTypeFontSingleton_Create)
IPC_MESSAGE_CONTROL0(PpapiHostMsg_TrueTypeFontSingleton_GetFontFamilies)
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_TrueTypeFontSingleton_GetFontFamiliesReply,
                     std::vector<std::string> )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_TrueTypeFontSingleton_GetFontsInFamily,
                     std::string )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_TrueTypeFontSingleton_GetFontsInFamilyReply,
                     std::vector<ppapi::proxy::SerializedTrueTypeFontDesc>
                         )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_TrueTypeFont_Create,
                     ppapi::proxy::SerializedTrueTypeFontDesc )
IPC_MESSAGE_CONTROL0(PpapiHostMsg_TrueTypeFont_Describe)
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_TrueTypeFont_DescribeReply,
                     ppapi::proxy::SerializedTrueTypeFontDesc )
IPC_MESSAGE_CONTROL0(PpapiHostMsg_TrueTypeFont_GetTableTags)
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_TrueTypeFont_GetTableTagsReply,
                     std::vector<uint32_t> )
IPC_MESSAGE_CONTROL3(PpapiHostMsg_TrueTypeFont_GetTable,
                     uint32_t ,
                     int32_t ,
                     int32_t )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_TrueTypeFont_GetTableReply,
                     std::string )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_HostResolver_Create)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_HostResolver_CreatePrivate)

IPC_MESSAGE_CONTROL2(PpapiHostMsg_HostResolver_Resolve,
                     ppapi::HostPortPair ,
                     PP_HostResolver_Private_Hint )

IPC_MESSAGE_CONTROL2(PpapiPluginMsg_HostResolver_ResolveReply,
                     std::string ,
                     std::vector<PP_NetAddress_Private> )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_PlatformVerification_Create)
IPC_MESSAGE_CONTROL2(PpapiHostMsg_PlatformVerification_ChallengePlatform,
                     std::string ,
                     std::vector<uint8_t> )
IPC_MESSAGE_CONTROL3(PpapiHostMsg_PlatformVerification_ChallengePlatformReply,
                     std::vector<uint8_t> ,
                     std::vector<uint8_t> ,
                     std::string )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_Printing_Create)
IPC_MESSAGE_CONTROL0(PpapiHostMsg_Printing_GetDefaultPrintSettings)
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_Printing_GetDefaultPrintSettingsReply,
                     PP_PrintSettings_Dev )

IPC_MESSAGE_CONTROL1(PpapiHostMsg_TCPSocket_Create,
                     ppapi::TCPSocketVersion )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_TCPSocket_CreatePrivate)

IPC_MESSAGE_CONTROL1(PpapiHostMsg_TCPSocket_Bind,
                     PP_NetAddress_Private )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_TCPSocket_BindReply,
                     PP_NetAddress_Private )
IPC_MESSAGE_CONTROL2(PpapiHostMsg_TCPSocket_Connect,
                     std::string ,
                     uint16_t )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_TCPSocket_ConnectWithNetAddress,
                     PP_NetAddress_Private )
IPC_MESSAGE_CONTROL2(PpapiPluginMsg_TCPSocket_ConnectReply,
                     PP_NetAddress_Private ,
                     PP_NetAddress_Private )
IPC_MESSAGE_CONTROL4(PpapiHostMsg_TCPSocket_SSLHandshake,
                     std::string ,
                     uint16_t ,
                     std::vector<std::vector<char> > ,
                     std::vector<std::vector<char> > )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_TCPSocket_SSLHandshakeReply,
                     ppapi::PPB_X509Certificate_Fields )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_TCPSocket_Read,
                     int32_t )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_TCPSocket_ReadReply,
                     std::string )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_TCPSocket_Write,
                     std::string )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_TCPSocket_WriteReply)
IPC_MESSAGE_CONTROL1(PpapiHostMsg_TCPSocket_Listen,
                     int32_t )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_TCPSocket_ListenReply)
IPC_MESSAGE_CONTROL0(PpapiHostMsg_TCPSocket_Accept)
IPC_MESSAGE_CONTROL3(PpapiPluginMsg_TCPSocket_AcceptReply,
                     int ,
                     PP_NetAddress_Private ,
                     PP_NetAddress_Private )
IPC_MESSAGE_CONTROL0(PpapiHostMsg_TCPSocket_Close)
IPC_MESSAGE_CONTROL2(PpapiHostMsg_TCPSocket_SetOption,
                     PP_TCPSocket_Option ,
                     ppapi::SocketOptionData )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_TCPSocket_SetOptionReply)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_TCPServerSocket_CreatePrivate)

IPC_MESSAGE_CONTROL2(PpapiHostMsg_TCPServerSocket_Listen,
                     PP_NetAddress_Private ,
                     int32_t )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_TCPServerSocket_ListenReply,
                     PP_NetAddress_Private )
IPC_MESSAGE_CONTROL0(PpapiHostMsg_TCPServerSocket_Accept)
IPC_MESSAGE_CONTROL3(PpapiPluginMsg_TCPServerSocket_AcceptReply,
                     int ,
                     PP_NetAddress_Private ,
                     PP_NetAddress_Private )
IPC_MESSAGE_CONTROL0(PpapiHostMsg_TCPServerSocket_StopListening)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_UDPSocket_Create)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_UDPSocket_CreatePrivate)

IPC_MESSAGE_CONTROL2(PpapiHostMsg_UDPSocket_SetOption,
                     PP_UDPSocket_Option ,
                     ppapi::SocketOptionData )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_UDPSocket_SetOptionReply)
IPC_MESSAGE_CONTROL1(PpapiHostMsg_UDPSocket_Bind,
                     PP_NetAddress_Private )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_UDPSocket_BindReply,
                     PP_NetAddress_Private )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_UDPSocket_RecvFrom,
                     int32_t )
IPC_MESSAGE_CONTROL2(PpapiPluginMsg_UDPSocket_RecvFromReply,
                     std::string ,
                     PP_NetAddress_Private )
IPC_MESSAGE_CONTROL2(PpapiHostMsg_UDPSocket_SendTo,
                     std::string ,
                     PP_NetAddress_Private )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_UDPSocket_SendToReply,
                     int32_t /* bytes_written */)
IPC_MESSAGE_CONTROL0(PpapiHostMsg_UDPSocket_Close)


IPC_MESSAGE_CONTROL0(PpapiHostMsg_URLLoader_Create)

IPC_MESSAGE_CONTROL1(PpapiHostMsg_URLLoader_Open,
                     ppapi::URLRequestInfoData )

IPC_MESSAGE_CONTROL1(PpapiHostMsg_URLLoader_SetDeferLoading,
                     bool )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_URLLoader_Close)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_URLLoader_GrantUniversalAccess)

IPC_MESSAGE_CONTROL1(PpapiPluginMsg_URLLoader_ReceivedResponse,
                     ppapi::URLResponseInfoData )

IPC_MESSAGE_CONTROL0(PpapiPluginMsg_URLLoader_SendData)

IPC_MESSAGE_CONTROL1(PpapiPluginMsg_URLLoader_FinishedLoading,
                     int32_t )

IPC_MESSAGE_CONTROL4(PpapiPluginMsg_URLLoader_UpdateProgress,
                     int64_t ,
                     int64_t ,
                     int64_t ,
                     int64_t )


IPC_SYNC_MESSAGE_CONTROL2_2(PpapiHostMsg_SharedMemory_CreateSharedMemory,
                            PP_Instance ,
                            uint32_t ,
                            int ,
                            ppapi::proxy::SerializedHandle )


IPC_MESSAGE_CONTROL0(PpapiHostMsg_VideoDestination_Create)
IPC_MESSAGE_CONTROL1(PpapiHostMsg_VideoDestination_Open,
                     std::string )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_VideoDestination_OpenReply)
IPC_MESSAGE_CONTROL2(PpapiHostMsg_VideoDestination_PutFrame,
                     ppapi::HostResource ,
                     PP_TimeTicks )
IPC_MESSAGE_CONTROL0(PpapiHostMsg_VideoDestination_Close)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_VideoSource_Create)
IPC_MESSAGE_CONTROL1(PpapiHostMsg_VideoSource_Open,
                     std::string )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_VideoSource_OpenReply)
IPC_MESSAGE_CONTROL0(PpapiHostMsg_VideoSource_GetFrame)
IPC_MESSAGE_CONTROL3(PpapiPluginMsg_VideoSource_GetFrameReply,
                     ppapi::HostResource ,
                     PP_ImageDataDesc ,
                     PP_TimeTicks )
IPC_MESSAGE_CONTROL0(PpapiHostMsg_VideoSource_Close)


IPC_MESSAGE_CONTROL0(PpapiHostMsg_WebSocket_Create)

IPC_MESSAGE_CONTROL2(PpapiHostMsg_WebSocket_Connect,
                     std::string ,
                     std::vector<std::string> )

IPC_MESSAGE_CONTROL2(PpapiHostMsg_WebSocket_Close,
                     int32_t ,
                     std::string )

IPC_MESSAGE_CONTROL1(PpapiHostMsg_WebSocket_SendText,
                     std::string )

IPC_MESSAGE_CONTROL1(PpapiHostMsg_WebSocket_SendBinary,
                     std::vector<uint8_t> )

IPC_MESSAGE_CONTROL1(PpapiHostMsg_WebSocket_Fail,
                     std::string )

IPC_MESSAGE_CONTROL2(PpapiPluginMsg_WebSocket_ConnectReply,
                     std::string ,
                     std::string )

IPC_MESSAGE_CONTROL4(PpapiPluginMsg_WebSocket_CloseReply,
                     uint64_t ,
                     bool ,
                     uint16_t ,
                     std::string )

IPC_MESSAGE_CONTROL1(PpapiPluginMsg_WebSocket_ReceiveTextReply,
                     std::string )

IPC_MESSAGE_CONTROL1(PpapiPluginMsg_WebSocket_ReceiveBinaryReply,
                     std::vector<uint8_t> )

IPC_MESSAGE_CONTROL0(PpapiPluginMsg_WebSocket_ErrorReply)

IPC_MESSAGE_CONTROL1(PpapiPluginMsg_WebSocket_BufferedAmountReply,
                     uint64_t )

IPC_MESSAGE_CONTROL1(PpapiPluginMsg_WebSocket_StateReply,
                     int32_t )

IPC_MESSAGE_CONTROL4(PpapiPluginMsg_WebSocket_ClosedReply,
                     uint64_t ,
                     bool ,
                     uint16_t ,
                     std::string )


IPC_MESSAGE_CONTROL0(PpapiHostMsg_OutputProtection_Create)
IPC_MESSAGE_CONTROL1(PpapiHostMsg_OutputProtection_EnableProtection,
                     uint32_t )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_OutputProtection_EnableProtectionReply)
IPC_MESSAGE_CONTROL0(PpapiHostMsg_OutputProtection_QueryStatus)
IPC_MESSAGE_CONTROL2(PpapiPluginMsg_OutputProtection_QueryStatusReply,
                     uint32_t ,
                     uint32_t )

#if !defined(OS_NACL) && !defined(NACL_WIN64)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_AudioInput_Create)
IPC_MESSAGE_CONTROL3(PpapiHostMsg_AudioInput_Open,
                     std::string ,
                     PP_AudioSampleRate ,
                     uint32_t )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_AudioInput_OpenReply)
IPC_MESSAGE_CONTROL1(PpapiHostMsg_AudioInput_StartOrStop, bool )
IPC_MESSAGE_CONTROL0(PpapiHostMsg_AudioInput_Close)


IPC_MESSAGE_CONTROL0(PpapiHostMsg_BrowserFontSingleton_Create)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_BrowserFontSingleton_GetFontFamilies)

IPC_MESSAGE_CONTROL1(PpapiPluginMsg_BrowserFontSingleton_GetFontFamiliesReply,
                     std::string )


IPC_MESSAGE_CONTROL0(PpapiHostMsg_Flash_Create)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_Flash_UpdateActivity)

IPC_MESSAGE_CONTROL1(PpapiHostMsg_Flash_GetProxyForURL, std::string )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_Flash_GetProxyForURLReply,
                     std::string )

IPC_MESSAGE_CONTROL1(PpapiHostMsg_Flash_GetLocalTimeZoneOffset,
                     base::Time )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_Flash_GetLocalTimeZoneOffsetReply,
                     double )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_Flash_GetLocalDataRestrictions)
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_Flash_GetLocalDataRestrictionsReply,
                     int32_t )

IPC_MESSAGE_CONTROL1(PpapiHostMsg_Flash_SetInstanceAlwaysOnTop,
                     bool )

IPC_MESSAGE_CONTROL1(PpapiHostMsg_Flash_DrawGlyphs,
                     ppapi::proxy::PPBFlash_DrawGlyphs_Params )

IPC_MESSAGE_CONTROL3(PpapiHostMsg_Flash_Navigate,
                     ppapi::URLRequestInfoData ,
                     std::string ,
                     bool )

IPC_MESSAGE_CONTROL1(PpapiHostMsg_Flash_IsRectTopmost,
                     PP_Rect )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_Flash_InvokePrinting)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_DeviceEnumeration_EnumerateDevices)
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_DeviceEnumeration_EnumerateDevicesReply,
                     std::vector<ppapi::DeviceRefData> )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_DeviceEnumeration_MonitorDeviceChange,
                     uint32_t )
IPC_MESSAGE_CONTROL0(PpapiHostMsg_DeviceEnumeration_StopMonitoringDeviceChange)
IPC_MESSAGE_CONTROL2(PpapiPluginMsg_DeviceEnumeration_NotifyDeviceChange,
                     uint32_t ,
                     std::vector<ppapi::DeviceRefData> )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_FlashClipboard_Create)
IPC_MESSAGE_CONTROL1(PpapiHostMsg_FlashClipboard_RegisterCustomFormat,
                     std::string )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_FlashClipboard_RegisterCustomFormatReply,
                     uint32_t )
IPC_MESSAGE_CONTROL2(PpapiHostMsg_FlashClipboard_IsFormatAvailable,
                     uint32_t ,
                     uint32_t )
IPC_MESSAGE_CONTROL2(PpapiHostMsg_FlashClipboard_ReadData,
                     uint32_t ,
                     uint32_t )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_FlashClipboard_ReadDataReply,
                     std::string )
IPC_MESSAGE_CONTROL3(PpapiHostMsg_FlashClipboard_WriteData,
                     uint32_t ,
                     std::vector<uint32_t> ,
                     std::vector<std::string> )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_FlashFile_Create)
IPC_MESSAGE_CONTROL2(PpapiHostMsg_FlashFile_OpenFile,
                     ppapi::PepperFilePath ,
                     int )
IPC_MESSAGE_CONTROL2(PpapiHostMsg_FlashFile_RenameFile,
                     ppapi::PepperFilePath ,
                     ppapi::PepperFilePath )
IPC_MESSAGE_CONTROL2(PpapiHostMsg_FlashFile_DeleteFileOrDir,
                     ppapi::PepperFilePath ,
                     bool )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_FlashFile_CreateDir,
                     ppapi::PepperFilePath )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_FlashFile_QueryFile,
                     ppapi::PepperFilePath )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_FlashFile_QueryFileReply,
                     base::PlatformFileInfo )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_FlashFile_GetDirContents,
                     ppapi::PepperFilePath )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_FlashFile_GetDirContentsReply,
                     ppapi::DirContents )
IPC_MESSAGE_CONTROL0(PpapiHostMsg_FlashFile_CreateTemporaryFile)

IPC_MESSAGE_CONTROL2(PpapiHostMsg_FlashFontFile_Create,
                     ppapi::proxy::SerializedFontDescription ,
                     PP_PrivateFontCharset )
IPC_MESSAGE_CONTROL1(PpapiHostMsg_FlashFontFile_GetFontTable,
                     uint32_t )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_FlashFontFile_GetFontTableReply,
                     std::string )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_FlashFullscreen_Create)
IPC_MESSAGE_CONTROL1(PpapiHostMsg_FlashFullscreen_SetFullscreen,
                     bool )


IPC_MESSAGE_CONTROL1(PpapiHostMsg_FlashMenu_Create,
                     ppapi::proxy::SerializedFlashMenu )

IPC_MESSAGE_CONTROL1(PpapiHostMsg_FlashMenu_Show,
                     PP_Point )

IPC_MESSAGE_CONTROL1(PpapiPluginMsg_FlashMenu_ShowReply,
                     int32_t )


IPC_MESSAGE_CONTROL0(PpapiHostMsg_PDF_Create)

IPC_MESSAGE_CONTROL1(PpapiHostMsg_PDF_GetLocalizedString,
                     PP_ResourceString )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_PDF_GetLocalizedStringReply,
                     std::string )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_PDF_DidStartLoading)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_PDF_DidStopLoading)

IPC_MESSAGE_CONTROL1(PpapiHostMsg_PDF_SetContentRestriction,
                     int )

IPC_MESSAGE_CONTROL1(PpapiHostMsg_PDF_UserMetricsRecordAction,
                     std::string )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_PDF_HasUnsupportedFeature)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_PDF_Print)

IPC_MESSAGE_CONTROL0(PpapiHostMsg_PDF_SaveAs)

IPC_MESSAGE_CONTROL2(PpapiHostMsg_PDF_GetResourceImage,
                     PP_ResourceImage ,
                     float )

IPC_MESSAGE_CONTROL2(PpapiPluginMsg_PDF_GetResourceImageReply,
                     ppapi::HostResource ,
                     PP_ImageDataDesc )

IPC_MESSAGE_CONTROL0(PpapiHostMsg_VideoCapture_Create)
IPC_MESSAGE_CONTROL0(PpapiHostMsg_VideoCapture_StartCapture)
IPC_MESSAGE_CONTROL1(PpapiHostMsg_VideoCapture_ReuseBuffer,
                     uint32_t )
IPC_MESSAGE_CONTROL0(PpapiHostMsg_VideoCapture_StopCapture)
IPC_MESSAGE_CONTROL0(PpapiHostMsg_VideoCapture_Close)

IPC_MESSAGE_CONTROL3(PpapiHostMsg_VideoCapture_Open,
                     std::string ,
                     PP_VideoCaptureDeviceInfo_Dev ,
                     uint32_t )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_VideoCapture_OpenReply)

IPC_MESSAGE_CONTROL3(PpapiPluginMsg_VideoCapture_OnDeviceInfo,
                     PP_VideoCaptureDeviceInfo_Dev ,
                     std::vector<ppapi::HostResource> ,
                     uint32_t )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_VideoCapture_OnStatus,
                     uint32_t )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_VideoCapture_OnError,
                     uint32_t )
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_VideoCapture_OnBufferReady,
                     uint32_t )


IPC_MESSAGE_CONTROL0(PpapiHostMsg_Talk_Create)
IPC_MESSAGE_CONTROL1(PpapiHostMsg_Talk_RequestPermission,
                     PP_TalkPermission )
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_Talk_RequestPermissionReply)
IPC_MESSAGE_CONTROL0(PpapiHostMsg_Talk_StartRemoting)
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_Talk_StartRemotingReply)
IPC_MESSAGE_CONTROL0(PpapiHostMsg_Talk_StopRemoting)
IPC_MESSAGE_CONTROL0(PpapiPluginMsg_Talk_StopRemotingReply)
IPC_MESSAGE_CONTROL1(PpapiPluginMsg_Talk_NotifyEvent, PP_TalkEvent )

#endif  
