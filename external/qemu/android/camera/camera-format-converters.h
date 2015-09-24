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

#ifndef ANDROID_CAMERA_CAMERA_FORMAT_CONVERTERS_H
#define ANDROID_CAMERA_CAMERA_FORMAT_CONVERTERS_H


#include "camera-common.h"

extern int has_converter(uint32_t from, uint32_t to);

extern int convert_frame(const void* frame,
                         uint32_t pixel_format,
                         size_t framebuffer_size,
                         int width,
                         int height,
                         ClientFrameBuffer* framebuffers,
                         int fbs_num,
                         float r_scale,
                         float g_scale,
                         float b_scale,
                         float exp_comp);

#endif  
