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

#ifndef ANDROID_SENSORS_PORT_H_
#define ANDROID_SENSORS_PORT_H_


typedef struct AndroidSensorsPort AndroidSensorsPort;

extern AndroidSensorsPort* sensors_port_create(void* opaque);

extern void sensors_port_destroy(AndroidSensorsPort* asp);

extern int sensors_port_enable_sensor(AndroidSensorsPort* asp, const char* name);


extern int sensors_port_disable_sensor(AndroidSensorsPort* asp, const char* name);

#endif  
