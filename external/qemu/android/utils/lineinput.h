/* Copyright (C) 2011 The Android Open Source Project
**
** This software is licensed under the terms of the GNU General Public
** License version 2, as published by the Free Software Foundation, and
** may be copied, distributed, and modified under those terms.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
*/
#ifndef _ANDROID_UTILS_LINEINPUT_H
#define _ANDROID_UTILS_LINEINPUT_H

#include <stdio.h>

typedef struct LineInput LineInput;

LineInput*  lineInput_newFromStdFile( FILE* file );

const char* lineInput_getLine( LineInput* input );

const char* lineInput_getLineAndSize( LineInput* input, size_t *pSize );

int lineInput_getLineNumber( LineInput* input );

int lineInput_isEof( LineInput* input );

int lineInput_getError( LineInput* input );

void lineInput_free( LineInput* input );

#endif 
