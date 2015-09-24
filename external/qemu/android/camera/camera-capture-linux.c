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


#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include "android/camera/camera-capture.h"
#include "android/camera/camera-format-converters.h"

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

#define CLEAR(x) memset (&(x), 0, sizeof(x))

typedef struct QemuPixelFormat {
    
    uint32_t        format;
    
    CameraFrameDim* dims;
    
    int             dim_num;
} QemuPixelFormat;

typedef struct CameraFrameBuffer {
    
    uint8_t*    data;
    
    size_t      size;
} CameraFrameBuffer;

typedef enum CameraIoType {
    
    CAMERA_IO_MEMMAP,
    
    CAMERA_IO_USERPTR,
    
    CAMERA_IO_DIRECT
} CameraIoType;

typedef struct LinuxCameraDevice LinuxCameraDevice;
struct LinuxCameraDevice {
    
    CameraDevice                header;

    
    char*                       device_name;
    
    int                         input_channel;


    
    int                         handle;
    
    struct v4l2_capability      caps;
    
    struct v4l2_pix_format      actual_pixel_format;
    
    CameraIoType                io_type;
    
    struct CameraFrameBuffer*   framebuffers;
    
    int                         framebuffer_num;
};

static const uint32_t _preferred_formats[] =
{
    
    V4L2_PIX_FMT_YUV420,
    V4L2_PIX_FMT_YVU420,
    
    V4L2_PIX_FMT_NV12,
    V4L2_PIX_FMT_NV21,
    V4L2_PIX_FMT_YUYV,
    
    V4L2_PIX_FMT_RGB32,
    V4L2_PIX_FMT_RGB24,
    V4L2_PIX_FMT_RGB565,
};
static const int _preferred_format_num =
    sizeof(_preferred_formats)/sizeof(*_preferred_formats);


static int
_xioctl(int fd, int request, void *arg) {
  int r;
  do {
      r = ioctl(fd, request, arg);
  } while (-1 == r && EINTR == errno);
  return r;
}

static void _qemu_pixel_format_free(QemuPixelFormat* fmt)
{
    if (fmt != NULL) {
        if (fmt->dims != NULL)
            free(fmt->dims);
    }
}

static int
_get_format_index(uint32_t fmt, QemuPixelFormat* formats, int size)
{
    int f;
    for (f = 0; f < size && formats[f].format != fmt; f++);
    return f < size ? f : -1;
}


static void
_free_framebuffers(CameraFrameBuffer* fb, int num, CameraIoType io_type)
{
    if (fb != NULL) {
        int n;

        switch (io_type) {
            case CAMERA_IO_MEMMAP:
                
                for (n = 0; n < num; n++) {
                    if (fb[n].data != NULL) {
                        munmap(fb[n].data, fb[n].size);
                        fb[n].data = NULL;
                        fb[n].size = 0;
                    }
                }
                break;

            case CAMERA_IO_USERPTR:
            case CAMERA_IO_DIRECT:
                
                for (n = 0; n < num; n++) {
                    if (fb[n].data != NULL) {
                        free(fb[n].data);
                        fb[n].data = NULL;
                        fb[n].size = 0;
                    }
                }
                break;

            default:
                E("%s: Invalid I/O type %d", __FUNCTION__, io_type);
                break;
        }
    }
}


static LinuxCameraDevice*
_camera_device_alloc(void)
{
    LinuxCameraDevice* cd;

    ANEW0(cd);
    memset(cd, 0, sizeof(*cd));
    cd->header.opaque = cd;
    cd->handle = -1;

    return cd;
}

static void
_camera_device_free(LinuxCameraDevice* lcd)
{
    if (lcd != NULL) {
        
        if (lcd->handle >= 0) {
            close(lcd->handle);
        }
        if (lcd->device_name != NULL) {
            free(lcd->device_name);
        }
        if (lcd->framebuffers != NULL) {
            _free_framebuffers(lcd->framebuffers, lcd->framebuffer_num,
                               lcd->io_type);
            free(lcd->framebuffers);
        }
        AFREE(lcd);
    } else {
        E("%s: No descriptor", __FUNCTION__);
    }
}

static void
_camera_device_reset(LinuxCameraDevice* cd)
{
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;

    
    if (cd->framebuffers != NULL) {
        _free_framebuffers(cd->framebuffers, cd->framebuffer_num, cd->io_type);
        free(cd->framebuffers);
        cd->framebuffers = NULL;
        cd->framebuffer_num = 0;
    }

    
    close(cd->handle);
    cd->handle = open(cd->device_name, O_RDWR | O_NONBLOCK, 0);

    if (cd->handle >= 0) {
        
        cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        _xioctl(cd->handle, VIDIOC_CROPCAP, &cropcap);
        crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        crop.c = cropcap.defrect; 
        _xioctl (cd->handle, VIDIOC_S_CROP, &crop);
    }
}

static int
_camera_device_mmap_framebuffer(LinuxCameraDevice* cd)
{
    struct v4l2_requestbuffers req;
    CLEAR(req);
    req.count   = 4;
    req.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory  = V4L2_MEMORY_MMAP;

    if(_xioctl(cd->handle, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            D("%s: Device '%s' does not support memory mapping",
              __FUNCTION__, cd->device_name);
            return 1;
        } else {
            E("%s: VIDIOC_REQBUFS has failed: %s",
              __FUNCTION__, strerror(errno));
            return -1;
        }
    }

    
    cd->framebuffers = calloc(req.count, sizeof(CameraFrameBuffer));
    if (cd->framebuffers == NULL) {
        E("%s: Not enough memory to allocate framebuffer array", __FUNCTION__);
        return -1;
    }

    for(cd->framebuffer_num = 0; cd->framebuffer_num < req.count;
        cd->framebuffer_num++) {
        
        struct v4l2_buffer buf;
        CLEAR(buf);
        buf.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory  = V4L2_MEMORY_MMAP;
        buf.index   = cd->framebuffer_num;
        if(_xioctl(cd->handle, VIDIOC_QUERYBUF, &buf) < 0) {
            E("%s: VIDIOC_QUERYBUF has failed: %s",
              __FUNCTION__, strerror(errno));
            return -1;
        }
        cd->framebuffers[cd->framebuffer_num].size = buf.length;
        cd->framebuffers[cd->framebuffer_num].data =
            mmap(NULL, buf.length, PROT_READ | PROT_WRITE, MAP_SHARED,
                 cd->handle, buf.m.offset);
        if (MAP_FAILED == cd->framebuffers[cd->framebuffer_num].data) {
            E("%s: Memory mapping has failed: %s",
              __FUNCTION__, strerror(errno));
            return -1;
        }

        
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = cd->framebuffer_num;
        if (_xioctl(cd->handle, VIDIOC_QBUF, &buf) < 0) {
            E("%s: VIDIOC_QBUF has failed: %s", __FUNCTION__, strerror(errno));
            return -1;
        }
    }

    cd->io_type = CAMERA_IO_MEMMAP;

    return 0;
}

static int
_camera_device_user_framebuffer(LinuxCameraDevice* cd)
{
    struct v4l2_requestbuffers req;
    CLEAR (req);
    req.count   = 4;
    req.type    = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    req.memory  = V4L2_MEMORY_USERPTR;

    if(_xioctl(cd->handle, VIDIOC_REQBUFS, &req)) {
        if (EINVAL == errno) {
            D("%s: Device '%s' does not support user pointers",
              __FUNCTION__, cd->device_name);
            return 1;
        } else {
            E("%s: VIDIOC_REQBUFS has failed: %s",
              __FUNCTION__, strerror(errno));
            return -1;
        }
    }

    
    cd->framebuffers = calloc(req.count, sizeof(CameraFrameBuffer));
    if (cd->framebuffers == NULL) {
        E("%s: Not enough memory to allocate framebuffer array", __FUNCTION__);
        return -1;
    }

    
    for(cd->framebuffer_num = 0; cd->framebuffer_num < req.count;
        cd->framebuffer_num++) {
        cd->framebuffers[cd->framebuffer_num].size =
            cd->actual_pixel_format.sizeimage;
        cd->framebuffers[cd->framebuffer_num].data =
            malloc(cd->framebuffers[cd->framebuffer_num].size);
        if (cd->framebuffers[cd->framebuffer_num].data == NULL) {
            E("%s: Not enough memory to allocate framebuffer", __FUNCTION__);
            return -1;
        }

        
        struct v4l2_buffer buf;
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_USERPTR;
        buf.m.userptr = (unsigned long)cd->framebuffers[cd->framebuffer_num].data;
        buf.length = cd->framebuffers[cd->framebuffer_num].size;
        if (_xioctl(cd->handle, VIDIOC_QBUF, &buf) < 0) {
            E("%s: VIDIOC_QBUF has failed: %s", __FUNCTION__, strerror(errno));
            return -1;
        }
    }

    cd->io_type = CAMERA_IO_USERPTR;

    return 0;
}

static int
_camera_device_direct_framebuffer(LinuxCameraDevice* cd)
{
    
    cd->framebuffer_num = 1;
    cd->framebuffers = malloc(sizeof(CameraFrameBuffer));
    if (cd->framebuffers == NULL) {
        E("%s: Not enough memory to allocate framebuffer array", __FUNCTION__);
        return -1;
    }

    cd->framebuffers[0].size = cd->actual_pixel_format.sizeimage;
    cd->framebuffers[0].data = malloc(cd->framebuffers[0].size);
    if (cd->framebuffers[0].data == NULL) {
        E("%s: Not enough memory to allocate framebuffer", __FUNCTION__);
        return -1;
    }

    cd->io_type = CAMERA_IO_DIRECT;

    return 0;
}

static int
_camera_device_open(LinuxCameraDevice* cd)
{
    struct stat st;

    if (stat(cd->device_name, &st)) {
        return -1;
    }

    if (!S_ISCHR(st.st_mode)) {
        E("%s: '%s' is not a device", __FUNCTION__, cd->device_name);
        return -1;
    }

    
    cd->handle = open(cd->device_name, O_RDWR | O_NONBLOCK, 0);
    if (cd->handle < 0) {
        E("%s: Cannot open camera device '%s': %s",
          __FUNCTION__, cd->device_name, strerror(errno));
        return -1;
    }
    if (_xioctl(cd->handle, VIDIOC_QUERYCAP, &cd->caps) < 0) {
        if (EINVAL == errno) {
            E("%s: Camera '%s' is not a V4L2 device",
              __FUNCTION__, cd->device_name);
            close(cd->handle);
            cd->handle = -1;
            return -1;
        } else {
            E("%s: Unable to query capabilities for camera device '%s'",
              __FUNCTION__, cd->device_name);
            close(cd->handle);
            cd->handle = -1;
            return -1;
        }
    }

    
    if (!(cd->caps.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
        E("%s: Camera '%s' is not a video capture device",
          __FUNCTION__, cd->device_name);
        close(cd->handle);
        cd->handle = -1;
        return -1;
    }

    return 0;
}

static int
_camera_device_enum_format_sizes(LinuxCameraDevice* cd,
                                 uint32_t fmt,
                                 CameraFrameDim** sizes)
{
    int n;
    int sizes_num = 0;
    int out_num = 0;
    struct v4l2_frmsizeenum size_enum;
    CameraFrameDim* arr;

    
    for (n = 0; ; n++) {
        size_enum.index = n;
        size_enum.pixel_format = fmt;
        if(_xioctl(cd->handle, VIDIOC_ENUM_FRAMESIZES, &size_enum)) {
            break;
        }
        if (size_enum.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
            
            sizes_num++;
        } else if (size_enum.type == V4L2_FRMSIZE_TYPE_STEPWISE) {
            const uint32_t dif_widths =
                (size_enum.stepwise.max_width - size_enum.stepwise.min_width) /
                size_enum.stepwise.step_width + 1;
            const uint32_t dif_heights =
                (size_enum.stepwise.max_height - size_enum.stepwise.min_height) /
                size_enum.stepwise.step_height + 1;
            sizes_num += dif_widths * dif_heights;
        } else if (size_enum.type == V4L2_FRMSIZE_TYPE_CONTINUOUS) {
            sizes_num += 3;
        }

    }
    if (sizes_num == 0) {
        return 0;
    }

    
    *sizes = (CameraFrameDim*)malloc(sizes_num * sizeof(CameraFrameDim));
    if (*sizes == NULL) {
        E("%s: Memory allocation failure", __FUNCTION__);
        return -1;
    }
    arr = *sizes;
    for (n = 0; out_num < sizes_num; n++) {
        size_enum.index = n;
        size_enum.pixel_format = fmt;
        if(_xioctl(cd->handle, VIDIOC_ENUM_FRAMESIZES, &size_enum)) {
            
            E("%s: Unexpected failure while getting pixel dimensions: %s",
              __FUNCTION__, strerror(errno));
            free(arr);
            return -1;
        }

        if (size_enum.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
            arr[out_num].width = size_enum.discrete.width;
            arr[out_num].height = size_enum.discrete.height;
            out_num++;
        } else if (size_enum.type == V4L2_FRMSIZE_TYPE_STEPWISE) {
            uint32_t w;
            for (w = size_enum.stepwise.min_width;
                 w <= size_enum.stepwise.max_width;
                 w += size_enum.stepwise.step_width) {
                uint32_t h;
                for (h = size_enum.stepwise.min_height;
                     h <= size_enum.stepwise.max_height;
                     h += size_enum.stepwise.step_height) {
                    arr[out_num].width = w;
                    arr[out_num].height = h;
                    out_num++;
                }
            }
        } else if (size_enum.type == V4L2_FRMSIZE_TYPE_CONTINUOUS) {
            
            arr[out_num].width = size_enum.stepwise.min_width;
            arr[out_num].height = size_enum.stepwise.min_height;
            out_num++;
            
            arr[out_num].width =
                (size_enum.stepwise.min_width + size_enum.stepwise.max_width) / 2;
            arr[out_num].height =
                (size_enum.stepwise.min_height + size_enum.stepwise.max_height) / 2;
            out_num++;
            
            arr[out_num].width = size_enum.stepwise.max_width;
            arr[out_num].height = size_enum.stepwise.max_height;
            out_num++;
        }
    }

    return out_num;
}

static int
_camera_device_enum_pixel_formats(LinuxCameraDevice* cd, QemuPixelFormat** fmts)
{
    int n, max_fmt;
    int fmt_num = 0;
    int out_num = 0;
    struct v4l2_fmtdesc fmt_enum;
    QemuPixelFormat* arr;

    
    for (max_fmt = 0; ; max_fmt++) {
        memset(&fmt_enum, 0, sizeof(fmt_enum));
        fmt_enum.index = max_fmt;
        fmt_enum.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if(_xioctl(cd->handle, VIDIOC_ENUM_FMT, &fmt_enum)) {
            break;
        }
        
        if ((fmt_enum.flags & V4L2_FMT_FLAG_COMPRESSED) == 0) {
            fmt_num++;
        }
    }
    if (fmt_num == 0) {
        return 0;
    }

    
    *fmts = (QemuPixelFormat*)malloc(fmt_num * sizeof(QemuPixelFormat));
    if (*fmts == NULL) {
        E("%s: Memory allocation failure", __FUNCTION__);
        return -1;
    }
    arr = *fmts;
    memset(arr, 0, fmt_num * sizeof(QemuPixelFormat));
    for (n = 0; n < max_fmt && out_num < fmt_num; n++) {
        memset(&fmt_enum, 0, sizeof(fmt_enum));
        fmt_enum.index = n;
        fmt_enum.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if(_xioctl(cd->handle, VIDIOC_ENUM_FMT, &fmt_enum)) {
            int nn;
            
            E("%s: Unexpected failure while getting pixel format: %s",
              __FUNCTION__, strerror(errno));
            for (nn = 0; nn < out_num; nn++) {
                _qemu_pixel_format_free(arr + nn);
            }
            free(arr);
            return -1;
        }
        
        if ((fmt_enum.flags & V4L2_FMT_FLAG_COMPRESSED) == 0) {
            arr[out_num].format = fmt_enum.pixelformat;
            
            arr[out_num].dim_num =
                _camera_device_enum_format_sizes(cd, fmt_enum.pixelformat,
                                                 &arr[out_num].dims);
            if (arr[out_num].dim_num > 0) {
                out_num++;
            } else if (arr[out_num].dim_num < 0) {
                int nn;
                E("Unable to enumerate supported dimensions for pixel format %d",
                  fmt_enum.pixelformat);
                for (nn = 0; nn < out_num; nn++) {
                    _qemu_pixel_format_free(arr + nn);
                }
                free(arr);
                return -1;
            }
        }
    }

    return out_num;
}

static int
_camera_device_get_info(LinuxCameraDevice* cd, CameraInfo* cis)
{
    int f;
    int chosen = -1;
    QemuPixelFormat* formats = NULL;
    int num_pix_fmts = _camera_device_enum_pixel_formats(cd, &formats);
    if (num_pix_fmts <= 0) {
        return -1;
    }

    
    for (f = 0; f < _preferred_format_num; f++) {
        chosen = _get_format_index(_preferred_formats[f], formats, num_pix_fmts);
        if (chosen >= 0) {
            break;
        }
    }
    if (chosen < 0) {
        chosen = 0;
    }

    cis->device_name = ASTRDUP(cd->device_name);
    cis->inp_channel = cd->input_channel;
    cis->pixel_format = formats[chosen].format;
    cis->frame_sizes_num = formats[chosen].dim_num;
    
    cis->frame_sizes = formats[chosen].dims;
    formats[chosen].dims = NULL;
    cis->in_use = 0;

    for (f = 0; f < num_pix_fmts; f++) {
        _qemu_pixel_format_free(formats + f);
    }
    free(formats);

    return 0;
}


CameraDevice*
camera_device_open(const char* name, int inp_channel)
{
    struct v4l2_cropcap cropcap;
    struct v4l2_crop crop;
    LinuxCameraDevice* cd;

    
    cd = _camera_device_alloc();
    cd->device_name = name != NULL ? ASTRDUP(name) : ASTRDUP("/dev/video0");
    cd->input_channel = inp_channel;

    
    if (_camera_device_open(cd)) {
        _camera_device_free(cd);
        return NULL;
    }

    
    cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    _xioctl(cd->handle, VIDIOC_CROPCAP, &cropcap);
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c = cropcap.defrect; 
    _xioctl (cd->handle, VIDIOC_S_CROP, &crop);

    return &cd->header;
}

int
camera_device_start_capturing(CameraDevice* ccd,
                              uint32_t pixel_format,
                              int frame_width,
                              int frame_height)
{
    struct v4l2_format fmt;
    LinuxCameraDevice* cd;
    char fmt_str[5];
    int r;

    
    if (ccd == NULL || ccd->opaque == NULL) {
      E("%s: Invalid camera device descriptor", __FUNCTION__);
      return -1;
    }
    cd = (LinuxCameraDevice*)ccd->opaque;
    if (cd->handle < 0) {
      E("%s: Camera device is not opened", __FUNCTION__);
      return -1;
    }

    
    CLEAR(fmt);
    fmt.type                = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = frame_width;
    fmt.fmt.pix.height      = frame_height;
    fmt.fmt.pix.pixelformat = pixel_format;
    if (_xioctl(cd->handle, VIDIOC_S_FMT, &fmt) < 0) {
        memcpy(fmt_str, &pixel_format, 4);
        fmt_str[4] = '\0';
        E("%s: Camera '%s' does not support pixel format '%s' with dimensions %dx%d",
          __FUNCTION__, cd->device_name, fmt_str, frame_width, frame_height);
        _camera_device_reset(cd);
        return -1;
    }
    if (fmt.fmt.pix.width != frame_width || fmt.fmt.pix.height != frame_height) {
        memcpy(fmt_str, &pixel_format, 4);
        fmt_str[4] = '\0';
        E("%s: Dimensions %dx%d are wrong for pixel format '%s'",
          __FUNCTION__, frame_width, frame_height, fmt_str);
        _camera_device_reset(cd);
        return -1;
    }
    memcpy(&cd->actual_pixel_format, &fmt.fmt.pix, sizeof(struct v4l2_pix_format));


    
    r = _camera_device_mmap_framebuffer(cd);
    if (r < 0) {
        
        _camera_device_reset(cd);
        return -1;
    } else if (r > 0) {
        r = _camera_device_user_framebuffer(cd);
        if (r < 0) {
            
            _camera_device_reset(cd);
            return -1;
        } else if (r > 0) {
            
            if (!(cd->caps.capabilities & V4L2_CAP_READWRITE)) {
                E("%s: Don't know how to access frames on device '%s'",
                  __FUNCTION__, cd->device_name);
                _camera_device_reset(cd);
                return -1;
            }
            r = _camera_device_direct_framebuffer(cd);
            if (r != 0) {
                
                _camera_device_reset(cd);
                return -1;
            }
        }
    }

    
    if (cd->io_type != CAMERA_IO_DIRECT) {
        enum v4l2_buf_type type;
        type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if (_xioctl (cd->handle, VIDIOC_STREAMON, &type) < 0) {
            E("%s: VIDIOC_STREAMON on camera '%s' has failed: %s",
              __FUNCTION__, cd->device_name, strerror(errno));
            _camera_device_reset(cd);
            return -1;
        }
    }
    return 0;
}

int
camera_device_stop_capturing(CameraDevice* ccd)
{
    enum v4l2_buf_type type;
    LinuxCameraDevice* cd;

    
    if (ccd == NULL || ccd->opaque == NULL) {
      E("%s: Invalid camera device descriptor", __FUNCTION__);
      return -1;
    }
    cd = (LinuxCameraDevice*)ccd->opaque;
    if (cd->handle < 0) {
      E("%s: Camera device is not opened", __FUNCTION__);
      return -1;
    }

    switch (cd->io_type) {
        case CAMERA_IO_DIRECT:
            
            break;

        case CAMERA_IO_MEMMAP:
        case CAMERA_IO_USERPTR:
            type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            if (_xioctl(cd->handle, VIDIOC_STREAMOFF, &type) < 0) {
	            E("%s: VIDIOC_STREAMOFF on camera '%s' has failed: %s",
                  __FUNCTION__, cd->device_name, strerror(errno));
                return -1;
            }
            break;
        default:
            E("%s: Unknown I/O method: %d", __FUNCTION__, cd->io_type);
            return -1;
    }

    _camera_device_reset(cd);

    return 0;
}

int
camera_device_read_frame(CameraDevice* ccd,
                         ClientFrameBuffer* framebuffers,
                         int fbs_num,
                         float r_scale,
                         float g_scale,
                         float b_scale,
                         float exp_comp)
{
    LinuxCameraDevice* cd;

    
    if (ccd == NULL || ccd->opaque == NULL) {
      E("%s: Invalid camera device descriptor", __FUNCTION__);
      return -1;
    }
    cd = (LinuxCameraDevice*)ccd->opaque;
    if (cd->handle < 0) {
      E("%s: Camera device is not opened", __FUNCTION__);
      return -1;
    }

    if (cd->io_type == CAMERA_IO_DIRECT) {
        
        size_t total_read_bytes = 0;
        
        void* buff = cd->framebuffers[0].data;
        do {
            int read_bytes =
                read(cd->handle, buff + total_read_bytes,
                     cd->actual_pixel_format.sizeimage - total_read_bytes);
            if (read_bytes < 0) {
                switch (errno) {
                    case EIO:
                    case EAGAIN:
                        continue;
                    default:
                        E("%s: Unable to read from the camera device '%s': %s",
                          __FUNCTION__, cd->device_name, strerror(errno));
                        return -1;
                }
            }
            total_read_bytes += read_bytes;
        } while (total_read_bytes < cd->actual_pixel_format.sizeimage);
        
        return convert_frame(buff, cd->actual_pixel_format.pixelformat,
                             cd->actual_pixel_format.sizeimage,
                             cd->actual_pixel_format.width,
                             cd->actual_pixel_format.height,
                             framebuffers, fbs_num,
                             r_scale, g_scale, b_scale, exp_comp);
    } else {
        
        struct v4l2_buffer buf;
        int res;
        CLEAR(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = cd->io_type == CAMERA_IO_MEMMAP ? V4L2_MEMORY_MMAP :
                                                       V4L2_MEMORY_USERPTR;
        for (;;) {
            const int res = _xioctl(cd->handle, VIDIOC_DQBUF, &buf);
            if (res >= 0) {
                break;
            } else if (errno == EAGAIN) {
                return 1;   
            } else if (errno != EINTR && errno != EIO) {
                E("%s: VIDIOC_DQBUF on camera '%s' has failed: %s",
                  __FUNCTION__, cd->device_name, strerror(errno));
                return -1;
            }
        }

        
        res = convert_frame(cd->framebuffers[buf.index].data,
                            cd->actual_pixel_format.pixelformat,
                            cd->actual_pixel_format.sizeimage,
                            cd->actual_pixel_format.width,
                            cd->actual_pixel_format.height,
                            framebuffers, fbs_num,
                            r_scale, g_scale, b_scale, exp_comp);

        
        if (_xioctl(cd->handle, VIDIOC_QBUF, &buf) < 0) {
            W("%s: VIDIOC_QBUF on camera '%s' has failed: %s",
              __FUNCTION__, cd->device_name, strerror(errno));
        }

        return res;
    }
}

void
camera_device_close(CameraDevice* ccd)
{
    LinuxCameraDevice* cd;

    
    if (ccd != NULL && ccd->opaque != NULL) {
        cd = (LinuxCameraDevice*)ccd->opaque;
        _camera_device_free(cd);
    } else {
        E("%s: Invalid camera device descriptor", __FUNCTION__);
    }
}

int
enumerate_camera_devices(CameraInfo* cis, int max)
{
    char dev_name[24];
    int found = 0;
    int n;

    for (n = 0; n < max; n++) {
        CameraDevice* cd;

        sprintf(dev_name, "/dev/video%d", n);
        cd = camera_device_open(dev_name, 0);
        if (cd != NULL) {
            LinuxCameraDevice* lcd = (LinuxCameraDevice*)cd->opaque;
            if (!_camera_device_get_info(lcd, cis + found)) {
                char user_name[24];
                sprintf(user_name, "webcam%d", found);
                cis[found].display_name = ASTRDUP(user_name);
                cis[found].in_use = 0;
                found++;
            }
            camera_device_close(cd);
        } else {
            break;
        }
    }

    return found;
}
