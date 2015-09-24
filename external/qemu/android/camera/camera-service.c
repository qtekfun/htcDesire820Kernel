/*
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include "qemu-common.h"
#include "android/globals.h"  
#include "android/hw-qemud.h"
#include "android/utils/misc.h"
#include "android/utils/system.h"
#include "android/utils/debug.h"
#include "android/camera/camera-capture.h"
#include "android/camera/camera-format-converters.h"
#include "android/camera/camera-service.h"

#define  E(...)    derror(__VA_ARGS__)
#define  W(...)    dwarning(__VA_ARGS__)
#define  D(...)    VERBOSE_PRINT(camera,__VA_ARGS__)
#define  D_ACTIVE  VERBOSE_CHECK(camera)

#define  T_ACTIVE   0

#if T_ACTIVE
#define  T(...)    VERBOSE_PRINT(camera,__VA_ARGS__)
#else
#define  T(...)    ((void)0)
#endif

#define SERVICE_NAME    "camera"

#define MAX_CAMERA      8

typedef struct CameraServiceDesc CameraServiceDesc;
struct CameraServiceDesc {
    CameraInfo  camera_info[MAX_CAMERA];
    
    int         camera_count;
};

static CameraServiceDesc    _camera_service_desc;


static int
_parse_query(const char* query,
             char* query_name,
             int query_name_size,
             const char** query_param)
{
    
    const char* qend = strchr(query, ' ');
    if (qend == NULL) {
        qend = query + strlen(query);
    }
    if ((qend - query) >= query_name_size) {
        return qend - query + 1;
    }
    memcpy(query_name, query, qend - query);
    query_name[qend - query] = '\0';

    
    if (query_param != NULL) {
        if (*qend == ' ') {
            qend++;
        }
        *query_param = (*qend == '\0') ? NULL : qend;
    }

    return 0;
}

static int
_append_string(char** str_buf, size_t* str_buf_size, const char* str)
{
    const size_t offset = (*str_buf != NULL) ? strlen(*str_buf) : 0;
    const size_t append_bytes = strlen(str) + 1;

    
    if (*str_buf == NULL) {
        *str_buf_size = 0;
    }

    if ((offset + append_bytes) > *str_buf_size) {
        const size_t required_mem = offset + append_bytes + 256;
        char* new_buf = (char*)realloc(*str_buf, required_mem);
        if (new_buf == NULL) {
            E("%s: Unable to allocate %d bytes for a string",
              __FUNCTION__, required_mem);
            return -1;
        }
        *str_buf = new_buf;
        *str_buf_size = required_mem;
    }
    memcpy(*str_buf + offset, str, append_bytes);

    return 0;
}

static int
_camera_info_to_string(const CameraInfo* ci, char** str, size_t* str_size) {
    int res;
    int n;
    char tmp[128];

    
    snprintf(tmp, sizeof(tmp), "name=%s ", ci->device_name);
    res = _append_string(str, str_size, tmp);
    if (res) {
        return res;
    }
    
    snprintf(tmp, sizeof(tmp), "channel=%d ", ci->inp_channel);
    res = _append_string(str, str_size, tmp);
    if (res) {
        return res;
    }
    
    snprintf(tmp, sizeof(tmp), "pix=%d ", ci->pixel_format);
    res = _append_string(str, str_size, tmp);
    if (res) {
        return res;
    }
    
    snprintf(tmp, sizeof(tmp), "dir=%s ", ci->direction);
    res = _append_string(str, str_size, tmp);
    if (res) {
        return res;
    }
    
    snprintf(tmp, sizeof(tmp), "framedims=%dx%d",
             ci->frame_sizes[0].width, ci->frame_sizes[0].height);
    res = _append_string(str, str_size, tmp);
    if (res) {
        return res;
    }
    for (n = 1; n < ci->frame_sizes_num; n++) {
        snprintf(tmp, sizeof(tmp), ",%dx%d",
                 ci->frame_sizes[n].width, ci->frame_sizes[n].height);
        res = _append_string(str, str_size, tmp);
        if (res) {
            return res;
        }
    }

    
    return _append_string(str, str_size, "\n");
}

static CameraInfo*
_camera_info_get_by_display_name(const char* disp_name, CameraInfo* arr, int num)
{
    int n;
    for (n = 0; n < num; n++) {
        if (!arr[n].in_use && arr[n].display_name != NULL &&
            !strcmp(arr[n].display_name, disp_name)) {
            return &arr[n];
        }
    }
    return NULL;
}

static CameraInfo*
_camera_info_get_by_device_name(const char* device_name, CameraInfo* arr, int num)
{
    int n;
    for (n = 0; n < num; n++) {
        if (arr[n].device_name != NULL && !strcmp(arr[n].device_name, device_name)) {
            return &arr[n];
        }
    }
    return NULL;
}


static void
_wecam_setup(CameraServiceDesc* csd,
             const char* disp_name,
             const char* dir,
             CameraInfo* ci,
             int ci_cnt)
{
    
    CameraInfo* found = _camera_info_get_by_display_name(disp_name, ci, ci_cnt);
    if (found == NULL) {
        W("Camera name '%s' is not found in the list of connected cameras.\n"
          "Use '-webcam-list' emulator option to obtain the list of connected camera names.\n",
          disp_name);
        return;
    }

    
    memcpy(csd->camera_info + csd->camera_count, found, sizeof(CameraInfo));
    
    found->in_use = 1;
    
    if (csd->camera_info[csd->camera_count].direction != NULL) {
        free(csd->camera_info[csd->camera_count].direction);
    }
    csd->camera_info[csd->camera_count].direction = ASTRDUP(dir);
    D("Camera %d '%s' connected to '%s' facing %s using %.4s pixel format",
      csd->camera_count, csd->camera_info[csd->camera_count].display_name,
      csd->camera_info[csd->camera_count].device_name,
      csd->camera_info[csd->camera_count].direction,
      (const char*)(&csd->camera_info[csd->camera_count].pixel_format));
      csd->camera_count++;
}

static void
_camera_service_init(CameraServiceDesc* csd)
{
    CameraInfo ci[MAX_CAMERA];
    int connected_cnt;

    
    memset(ci, 0, sizeof(CameraInfo) * MAX_CAMERA);
    memset(csd->camera_info, 0, sizeof(CameraInfo) * MAX_CAMERA);
    csd->camera_count = 0;

    
    if (memcmp(android_hw->hw_camera_back, "webcam", 6) &&
        memcmp(android_hw->hw_camera_front, "webcam", 6)) {
        
        return;
    }

    
    connected_cnt = enumerate_camera_devices(ci, MAX_CAMERA);
    if (connected_cnt <= 0) {
        
        return;
    }

    
    if (!memcmp(android_hw->hw_camera_back, "webcam", 6)) {
        _wecam_setup(csd, android_hw->hw_camera_back, "back", ci, connected_cnt);
    }

    
    if (!memcmp(android_hw->hw_camera_front, "webcam", 6)) {
        _wecam_setup(csd, android_hw->hw_camera_front, "front", ci, connected_cnt);
    }
}

static CameraInfo*
_camera_service_get_camera_info_by_device_name(CameraServiceDesc* cs,
                                               const char* device_name)
{
    return _camera_info_get_by_device_name(device_name, cs->camera_info,
                                           cs->camera_count);
}


static void
_qemu_client_reply_payload(QemudClient* qc, size_t payload_size)
{
    char payload_size_str[9];
    snprintf(payload_size_str, sizeof(payload_size_str), "%08zx", payload_size);
    qemud_client_send(qc, (const uint8_t*)payload_size_str, 8);
}


#define OK_REPLY        "ok"
#define KO_REPLY        "ko"
#define OK_REPLY_DATA   OK_REPLY ":"
#define KO_REPLY_DATA   KO_REPLY ":"

static void
_qemu_client_query_reply(QemudClient* qc,
                         int ok_ko,
                         const void* extra,
                         size_t extra_size)
{
    const char* ok_ko_str;
    size_t payload_size;

    
    if (extra == NULL && extra_size != 0) {
        W("%s: 'extra' = NULL, while 'extra_size' = %d",
          __FUNCTION__, (int)extra_size);
        extra_size = 0;
    }

    
    if (extra_size) {
        
        payload_size = extra_size + 3;
        ok_ko_str = ok_ko ? OK_REPLY_DATA : KO_REPLY_DATA;
    } else {
        
        payload_size = 3;
        ok_ko_str = ok_ko ? OK_REPLY : KO_REPLY;
    }

    
    _qemu_client_reply_payload(qc, payload_size);
    qemud_client_send(qc, (const uint8_t*)ok_ko_str, 3);
    
    if (extra != NULL) {
        qemud_client_send(qc, (const uint8_t*)extra, extra_size);
    }
}

static void
_qemu_client_reply_ok(QemudClient* qc, const char* ok_str)
{
    _qemu_client_query_reply(qc, 1, ok_str,
                             (ok_str != NULL) ? (strlen(ok_str) + 1) : 0);
}

static void
_qemu_client_reply_ko(QemudClient* qc, const char* ko_str)
{
    _qemu_client_query_reply(qc, 0, ko_str,
                             (ko_str != NULL) ? (strlen(ko_str) + 1) : 0);
}


static int
_factory_client_list_cameras(CameraServiceDesc* csd, QemudClient* client)
{
    int n;
    size_t reply_size = 0;
    char* reply = NULL;

    
    if (csd->camera_count == 0) {
        
        _qemu_client_reply_ok(client, "\n");
        return 0;
    }

    
    for (n = 0; n < csd->camera_count; n++) {
        const int res =
            _camera_info_to_string(csd->camera_info + n, &reply, &reply_size);
        if (res) {
            if (reply != NULL) {
                free(reply);
            }
            _qemu_client_reply_ko(client, "Memory allocation error");
            return res;
        }
    }

    D("%s Replied: %s", __FUNCTION__, reply);
    _qemu_client_reply_ok(client, reply);
    free(reply);

    return 0;
}

static void
_factory_client_recv(void*         opaque,
                     uint8_t*      msg,
                     int           msglen,
                     QemudClient*  client)
{

    
    static const char _query_list[]     = "list";

    CameraServiceDesc* csd = (CameraServiceDesc*)opaque;
    char query_name[64];
    const char* query_param = NULL;

    
    if (_parse_query((const char*)msg, query_name, sizeof(query_name),
                     &query_param)) {
        E("%s: Invalid format in query '%s'", __FUNCTION__, (const char*)msg);
        _qemu_client_reply_ko(client, "Invalid query format");
        return;
    }

    D("%s Camera factory query '%s'", __FUNCTION__, query_name);

    
    if (!strcmp(query_name, _query_list)) {
        
        _factory_client_list_cameras(csd, client);
    } else {
        E("%s: Unknown camera factory query name in '%s'",
          __FUNCTION__, (const char*)msg);
        _qemu_client_reply_ko(client, "Unknown query name");
    }
}

static void
_factory_client_close(void*  opaque)
{
}


typedef struct CameraClient CameraClient;
struct CameraClient
{
    char*               device_name;
    
    int                 inp_channel;
    
    const CameraInfo*   camera_info;
    
    CameraDevice*       camera;
    uint8_t*            video_frame;
    uint16_t*           preview_frame;
    
    size_t              video_frame_size;
    
    size_t              preview_frame_size;
    
    uint32_t            pixel_format;
    
    int                 width;
    
    int                 height;
    
    int                 pixel_num;
    
    int                 frames_cached;
};

static void
_camera_client_free(CameraClient* cc)
{
    if (cc->camera_info != NULL) {
        ((CameraInfo*)cc->camera_info)->in_use = 0;
    }
    if (cc->camera != NULL) {
        camera_device_close(cc->camera);
    }
    if (cc->video_frame != NULL) {
        free(cc->video_frame);
    }
    if (cc->device_name != NULL) {
        free(cc->device_name);
    }

    AFREE(cc);
}

static CameraClient*
_camera_client_create(CameraServiceDesc* csd, const char* param)
{
    CameraClient* cc;
    CameraInfo* ci;
    int res;
    ANEW0(cc);


    
    if (get_token_value_alloc(param, "name", &cc->device_name)) {
        E("%s: Allocation failure, or required 'name' parameter is missing, or misformed in '%s'",
          __FUNCTION__, param);
        return NULL;
    }

    
    res = get_token_value_int(param, "inp_channel", &cc->inp_channel);
    if (res != 0) {
        if (res == -1) {
            cc->inp_channel = 0;
        } else {
            E("%s: 'inp_channel' parameter is misformed in '%s'",
              __FUNCTION__, param);
            return NULL;
        }
    }

    ci = _camera_service_get_camera_info_by_device_name(csd, cc->device_name);
    if (ci == NULL) {
        E("%s: Cannot find camera info for device '%s'",
          __FUNCTION__, cc->device_name);
        _camera_client_free(cc);
        return NULL;
    }

    if (ci->in_use) {
        E("%s: Camera device '%s' is in use", __FUNCTION__, cc->device_name);
        _camera_client_free(cc);
        return NULL;
    }

    
    ci->in_use = 1;
    cc->camera_info = ci;

    D("%s: Camera service is created for device '%s' using input channel %d",
      __FUNCTION__, cc->device_name, cc->inp_channel);

    return cc;
}


static void
_camera_client_query_connect(CameraClient* cc, QemudClient* qc, const char* param)
{
    if (cc->camera != NULL) {
        
        W("%s: Camera '%s' is already connected", __FUNCTION__, cc->device_name);
        _qemu_client_reply_ok(qc, "Camera is already connected");
        return;
    }

    
    cc->camera = camera_device_open(cc->device_name, cc->inp_channel);
    if (cc->camera == NULL) {
        E("%s: Unable to open camera device '%s'", __FUNCTION__, cc->device_name);
        _qemu_client_reply_ko(qc, "Unable to open camera device.");
        return;
    }

    D("%s: Camera device '%s' is now connected", __FUNCTION__, cc->device_name);

    _qemu_client_reply_ok(qc, NULL);
}

static void
_camera_client_query_disconnect(CameraClient* cc,
                                QemudClient* qc,
                                const char* param)
{
    if (cc->camera == NULL) {
        
        W("%s: Camera '%s' is already disconnected", __FUNCTION__, cc->device_name);
        _qemu_client_reply_ok(qc, "Camera is not connected");
        return;
    }

    if (cc->video_frame != NULL) {
        E("%s: Cannot disconnect camera '%s' while it is not stopped",
          __FUNCTION__, cc->device_name);
        _qemu_client_reply_ko(qc, "Camera is not stopped");
        return;
    }

    
    camera_device_close(cc->camera);
    cc->camera = NULL;

    D("Camera device '%s' is now disconnected", cc->device_name);

    _qemu_client_reply_ok(qc, NULL);
}

static void
_camera_client_query_start(CameraClient* cc, QemudClient* qc, const char* param)
{
    char* w;
    char dim[64];
    int width, height, pix_format;

    
    if (cc->camera == NULL) {
        
        E("%s: Camera '%s' is not connected", __FUNCTION__, cc->device_name);
        _qemu_client_reply_ko(qc, "Camera is not connected");
        return;
    }


    if (param == NULL) {
        E("%s: Missing parameters for the query", __FUNCTION__);
        _qemu_client_reply_ko(qc, "Missing parameters for the query");
        return;
    }

    
    if (get_token_value(param, "dim", dim, sizeof(dim))) {
        E("%s: Invalid or missing 'dim' parameter in '%s'", __FUNCTION__, param);
        _qemu_client_reply_ko(qc, "Invalid or missing 'dim' parameter");
        return;
    }

    
    if (get_token_value_int(param, "pix", &pix_format)) {
        E("%s: Invalid or missing 'pix' parameter in '%s'", __FUNCTION__, param);
        _qemu_client_reply_ko(qc, "Invalid or missing 'pix' parameter");
        return;
    }

    
    w = strchr(dim, 'x');
    if (w == NULL || w[1] == '\0') {
        E("%s: Invalid 'dim' parameter in '%s'", __FUNCTION__, param);
        _qemu_client_reply_ko(qc, "Invalid 'dim' parameter");
        return;
    }
    *w = '\0'; w++;
    errno = 0;
    width = strtoi(dim, NULL, 10);
    height = strtoi(w, NULL, 10);
    if (errno) {
        E("%s: Invalid 'dim' parameter in '%s'", __FUNCTION__, param);
        _qemu_client_reply_ko(qc, "Invalid 'dim' parameter");
        return;
    }

    if (cc->video_frame != NULL) {
        
        if (cc->pixel_format != pix_format ||cc->width != width ||
            cc->height != height) {
            
            W("%s: Camera '%s' is already started", __FUNCTION__, cc->device_name);
            _qemu_client_reply_ok(qc, "Camera is already started");
        } else {
            
            E("%s: Camera '%s' is already started, and parameters don't match:\n"
              "Current %.4s[%dx%d] != requested %.4s[%dx%d]",
              __FUNCTION__, cc->device_name, (const char*)&cc->pixel_format,
              cc->width, cc->height, (const char*)&pix_format, width, height);
            _qemu_client_reply_ko(qc,
                "Camera is already started with different capturing parameters");
        }
        return;
    }


    
    cc->pixel_format = pix_format;
    cc->width = width;
    cc->height = height;
    cc->pixel_num = cc->width * cc->height;
    cc->frames_cached = 0;

    switch (cc->pixel_format) {
        case V4L2_PIX_FMT_YUV420:
        case V4L2_PIX_FMT_YVU420:
        case V4L2_PIX_FMT_NV12:
        case V4L2_PIX_FMT_NV21:
            cc->video_frame_size = (cc->pixel_num * 12) / 8;
            break;

        default:
            E("%s: Unknown pixel format %.4s",
              __FUNCTION__, (char*)&cc->pixel_format);
            _qemu_client_reply_ko(qc, "Pixel format is unknown");
            return;
    }

    if (!has_converter(cc->camera_info->pixel_format, cc->pixel_format) ||
        !has_converter(cc->camera_info->pixel_format, V4L2_PIX_FMT_RGB32)) {
        E("%s: No conversion exist between %.4s and %.4s (or RGB32) pixel formats",
          __FUNCTION__, (char*)&cc->camera_info->pixel_format, (char*)&cc->pixel_format);
        _qemu_client_reply_ko(qc, "No conversion exist for the requested pixel format");
        return;
    }

    cc->preview_frame_size = cc->pixel_num * 4;

    cc->video_frame =
        (uint8_t*)malloc(cc->video_frame_size + cc->preview_frame_size);
    if (cc->video_frame == NULL) {
        E("%s: Not enough memory for framebuffers %d + %d",
          __FUNCTION__, cc->video_frame_size, cc->preview_frame_size);
        _qemu_client_reply_ko(qc, "Out of memory");
        return;
    }

    
    cc->preview_frame = (uint16_t*)(cc->video_frame + cc->video_frame_size);

    
    if (camera_device_start_capturing(cc->camera, cc->camera_info->pixel_format,
                                      cc->width, cc->height)) {
        E("%s: Cannot start camera '%s' for %.4s[%dx%d]: %s",
          __FUNCTION__, cc->device_name, (const char*)&cc->pixel_format,
          cc->width, cc->height, strerror(errno));
        free(cc->video_frame);
        cc->video_frame = NULL;
        _qemu_client_reply_ko(qc, "Cannot start the camera");
        return;
    }

    D("%s: Camera '%s' is now started for %.4s[%dx%d]",
      __FUNCTION__, cc->device_name, (char*)&cc->pixel_format, cc->width,
      cc->height);

    _qemu_client_reply_ok(qc, NULL);
}

static void
_camera_client_query_stop(CameraClient* cc, QemudClient* qc, const char* param)
{
    if (cc->video_frame == NULL) {
        
        W("%s: Camera '%s' is not started", __FUNCTION__, cc->device_name);
        _qemu_client_reply_ok(qc, "Camera is not started");
        return;
    }

    
    if (camera_device_stop_capturing(cc->camera)) {
        E("%s: Cannot stop camera device '%s': %s",
          __FUNCTION__, cc->device_name, strerror(errno));
        _qemu_client_reply_ko(qc, "Cannot stop camera device");
        return;
    }

    free(cc->video_frame);
    cc->video_frame = NULL;

    D("%s: Camera device '%s' is now stopped.", __FUNCTION__, cc->device_name);
    _qemu_client_reply_ok(qc, NULL);
}

static void
_camera_client_query_frame(CameraClient* cc, QemudClient* qc, const char* param)
{
    int video_size = 0;
    int preview_size = 0;
    int repeat;
    ClientFrameBuffer fbs[2];
    int fbs_num = 0;
    size_t payload_size;
    uint64_t tick;
    float r_scale = 1.0f, g_scale = 1.0f, b_scale = 1.0f, exp_comp = 1.0f;
    char tmp[256];

    
    if (cc->video_frame == NULL) {
        
        E("%s: Camera '%s' is not started", __FUNCTION__, cc->device_name);
        _qemu_client_reply_ko(qc, "Camera is not started");
        return;
    }

    
    if (get_token_value_int(param, "video", &video_size) ||
        get_token_value_int(param, "preview", &preview_size)) {
        E("%s: Invalid or missing 'video', or 'preview' parameter in '%s'",
          __FUNCTION__, param);
        _qemu_client_reply_ko(qc,
            "Invalid or missing 'video', or 'preview' parameter");
        return;
    }

    
    if (!get_token_value(param, "whiteb", tmp, sizeof(tmp))) {
        if (sscanf(tmp, "%g,%g,%g", &r_scale, &g_scale, &b_scale) != 3) {
            D("Invalid value '%s' for parameter 'whiteb'", tmp);
            r_scale = g_scale = b_scale = 1.0f;
        }
    }

    
    if (!get_token_value(param, "expcomp", tmp, sizeof(tmp))) {
        if (sscanf(tmp, "%g", &exp_comp) != 1) {
            D("Invalid value '%s' for parameter 'whiteb'", tmp);
            exp_comp = 1.0f;
        }
    }

    if ((video_size != 0 && cc->video_frame_size != video_size) ||
        (preview_size != 0 && cc->preview_frame_size != preview_size)) {
        E("%s: Frame sizes don't match for camera '%s':\n"
          "Expected %d for video, and %d for preview. Requested %d, and %d",
          __FUNCTION__, cc->device_name, cc->video_frame_size,
          cc->preview_frame_size, video_size, preview_size);
        _qemu_client_reply_ko(qc, "Frame size mismatch");
        return;
    }


    if (video_size) {
        fbs[fbs_num].pixel_format = cc->pixel_format;
        fbs[fbs_num].framebuffer = cc->video_frame;
        fbs_num++;
    }
    if (preview_size) {
        
        fbs[fbs_num].pixel_format = V4L2_PIX_FMT_RGB32;
        fbs[fbs_num].framebuffer = cc->preview_frame;
        fbs_num++;
    }

    
    tick = _get_timestamp();
    repeat = camera_device_read_frame(cc->camera, fbs, fbs_num,
                                      r_scale, g_scale, b_scale, exp_comp);

    while (repeat == 1 && !cc->frames_cached &&
           (_get_timestamp() - tick) < 2000000LL) {
        
        _camera_sleep(10);
        repeat = camera_device_read_frame(cc->camera, fbs, fbs_num,
                                          r_scale, g_scale, b_scale, exp_comp);
    }
    if (repeat == 1 && !cc->frames_cached) {
        
        E("%s: Unable to obtain first video frame from the camera '%s' in %d milliseconds: %s.",
          __FUNCTION__, cc->device_name,
          (uint32_t)(_get_timestamp() - tick) / 1000, strerror(errno));
        _qemu_client_reply_ko(qc, "Unable to obtain video frame from the camera");
        return;
    } else if (repeat < 0) {
        
        E("%s: Unable to obtain video frame from the camera '%s': %s.",
          __FUNCTION__, cc->device_name, strerror(errno));
        _qemu_client_reply_ko(qc, strerror(errno));
        return;
    }

    
    cc->frames_cached = 1;


    
    payload_size = 3 + video_size + preview_size;

    
    _qemu_client_reply_payload(qc, payload_size);

    if (video_size || preview_size) {
        qemud_client_send(qc, (const uint8_t*)"ok:", 3);
    } else {
        
        qemud_client_send(qc, (const uint8_t*)"ok", 3);
    }

    
    if (video_size) {
        qemud_client_send(qc, cc->video_frame, video_size);
    }

    
    if (preview_size) {
        qemud_client_send(qc, (const uint8_t*)cc->preview_frame, preview_size);
    }
}

static void
_camera_client_recv(void*         opaque,
                    uint8_t*      msg,
                    int           msglen,
                    QemudClient*  client)
{

    
    static const char _query_connect[]    = "connect";
    
    static const char _query_disconnect[] = "disconnect";
    
    static const char _query_start[]      = "start";
    
    static const char _query_stop[]       = "stop";
    
    static const char _query_frame[]      = "frame";

    char query_name[64];
    const char* query_param = NULL;
    CameraClient* cc = (CameraClient*)opaque;


    T("%s: Camera client query: '%s'", __FUNCTION__, (char*)msg);
    if (_parse_query((const char*)msg, query_name, sizeof(query_name),
        &query_param)) {
        E("%s: Invalid query '%s'", __FUNCTION__, (char*)msg);
        _qemu_client_reply_ko(client, "Invalid query");
        return;
    }

    
    if (!strcmp(query_name, _query_frame)) {
        
        _camera_client_query_frame(cc, client, query_param);
    } else if (!strcmp(query_name, _query_connect)) {
        
        _camera_client_query_connect(cc, client, query_param);
    } else if (!strcmp(query_name, _query_disconnect)) {
        
        _camera_client_query_disconnect(cc, client, query_param);
    } else if (!strcmp(query_name, _query_start)) {
        
        _camera_client_query_start(cc, client, query_param);
    } else if (!strcmp(query_name, _query_stop)) {
        
        _camera_client_query_stop(cc, client, query_param);
    } else {
        E("%s: Unknown query '%s'", __FUNCTION__, (char*)msg);
        _qemu_client_reply_ko(client, "Unknown query");
    }
}

static void
_camera_client_close(void* opaque)
{
    CameraClient* cc = (CameraClient*)opaque;

    D("%s: Camera client for device '%s' on input channel %d is now closed",
      __FUNCTION__, cc->device_name, cc->inp_channel);

    _camera_client_free(cc);
}


static QemudClient*
_camera_service_connect(void*          opaque,
                        QemudService*  serv,
                        int            channel,
                        const char*    client_param)
{
    QemudClient*  client = NULL;
    CameraServiceDesc* csd = (CameraServiceDesc*)opaque;

    D("%s: Connecting camera client '%s'",
      __FUNCTION__, client_param ? client_param : "Factory");
    if (client_param == NULL || *client_param == '\0') {
        
        client = qemud_client_new(serv, channel, client_param, csd,
                                  _factory_client_recv, _factory_client_close,
                                  NULL, NULL);
    } else {
        
        CameraClient* cc = _camera_client_create(csd, client_param);
        if (cc != NULL) {
            client = qemud_client_new(serv, channel, client_param, cc,
                                      _camera_client_recv, _camera_client_close,
                                      NULL, NULL);
        }
    }

    return client;
}

void
android_camera_service_init(void)
{
    static int _inited = 0;

    if (!_inited) {
        _camera_service_init(&_camera_service_desc);
        QemudService*  serv = qemud_service_register( SERVICE_NAME, 0,
                                                      &_camera_service_desc,
                                                      _camera_service_connect,
                                                      NULL, NULL);
        if (serv == NULL) {
            derror("%s: Could not register '%s' service",
                   __FUNCTION__, SERVICE_NAME);
            return;
        }
        D("%s: Registered '%s' qemud service", __FUNCTION__, SERVICE_NAME);
    }
}

void
android_list_web_cameras(void)
{
    CameraInfo ci[MAX_CAMERA];
    int connected_cnt;
    int i;

    
    connected_cnt = enumerate_camera_devices(ci, MAX_CAMERA);
    if (connected_cnt <= 0) {
        return;
    }

    printf("List of web cameras connected to the computer:\n");
    for (i = 0; i < connected_cnt; i++) {
        printf(" Camera '%s' is connected to device '%s' on channel %d using pixel format '%.4s'\n",
               ci[i].display_name, ci[i].device_name, ci[i].inp_channel,
               (const char*)&ci[i].pixel_format);
    }
    printf("\n");
}
