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

#ifndef ANDROID_CAMERA_CAMERA_CAPTURE_H
#define ANDROID_CAMERA_CAMERA_CAPTURE_H


#include "camera-common.h"

extern CameraDevice* camera_device_open(const char* name, int inp_channel);

extern int camera_device_start_capturing(CameraDevice* cd,
                                         uint32_t pixel_format,
                                         int frame_width,
                                         int frame_height);

extern int camera_device_stop_capturing(CameraDevice* cd);

extern int camera_device_read_frame(CameraDevice* cd,
                                    ClientFrameBuffer* framebuffers,
                                    int fbs_num,
                                    float r_scale,
                                    float g_scale,
                                    float b_scale,
                                    float exp_comp);

extern void camera_device_close(CameraDevice* cd);

extern int enumerate_camera_devices(CameraInfo* cis, int max);

#endif  
