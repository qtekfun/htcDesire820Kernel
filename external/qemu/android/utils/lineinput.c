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
#include <errno.h>
#include "android/utils/system.h"
#include "android/utils/assert.h"
#include "android/utils/lineinput.h"

struct LineInput {
    char*   line;
    size_t  line_size;
    int     line_num;
    int     error;
    int     eof;

    struct {
        FILE*  file;
    } std;

    char    line0[128];
};

enum {
    LINEINPUT_ERROR = -1,
    LINEINPUT_EOF = -2,
};


static LineInput*
_lineInput_new( void )
{
    LineInput*  input;

    ANEW0(input);
    input->line      = input->line0;
    input->line_size = sizeof(input->line0);

    return input;
}

LineInput*
lineInput_newFromStdFile( FILE* file )
{
    LineInput* input = _lineInput_new();

    input->std.file = file;
    return input;
}

static void
_lineInput_grow( LineInput* input )
{
    char*  line;

    input->line_size += input->line_size >> 1;
    line = input->line;
    if (line == input->line0)
        line = NULL;

    AARRAY_RENEW(line, input->line_size);
    input->line = line;
}

static int _lineInput_getLineFromStdFile( LineInput* input, FILE* file );

const char*
lineInput_getLine( LineInput* input )
{
    return lineInput_getLineAndSize(input, NULL);
}

const char*
lineInput_getLineAndSize( LineInput* input, size_t *pSize )
{
    int ret;

    
    if (pSize)
        *pSize = 0;

    
    if (input == NULL) {
        errno = EINVAL;
        return NULL;
    }

    
    if (input->error) {
        return NULL;
    }
    if (input->eof) {
        return NULL;
    }

    ret = _lineInput_getLineFromStdFile(input, input->std.file);
    if (ret >= 0) {
        input->line_num += 1;
        if (pSize != NULL) {
            *pSize = ret;
            return input->line;
        }
        return input->line;
    }
    if (ret == LINEINPUT_EOF) {
        input->line_num += 1;
        input->eof = 1;
        return NULL;
    }
    if (ret == LINEINPUT_ERROR) {
        input->error = errno;
        return NULL;
    }
    AASSERT_UNREACHED();
    return NULL;
}

int
lineInput_getLineNumber( LineInput* input )
{
    return input->line_num;
}

int
lineInput_isEof( LineInput* input )
{
    return (input->eof != 0);
}

int
lineInput_getError( LineInput* input )
{
    return input->error;
}

void
lineInput_free( LineInput* input )
{
    if (input != NULL) {
        if (input->line != NULL) {
            if (input->line != input->line0)
                AFREE(input->line);
            input->line = NULL;
            input->line_size = 0;
        }
        AFREE(input);
    }
}


static int
_lineInput_getLineFromStdFile( LineInput* input, FILE* file )
{
    int   offset = 0;
    char* p;

    input->line[0] = '\0';

    for (;;) {
        char* buffer = input->line + offset;
        int   avail  = input->line_size - offset;

        if (!fgets(buffer, avail, file)) {
            if (offset > 0) {
                return offset;
            }
            goto INPUT_ERROR;
        }

        
        p = memchr(buffer, '\0', avail);
        AASSERT(p != NULL);

        if (p == buffer) {
            input->eof = 1;
            if (offset > 0)
                return offset;
            else
                return LINEINPUT_EOF;
        }

        if (p[-1] != '\n' && p[-1] != '\r') {
            offset = p - input->line;
            _lineInput_grow(input);
            continue;
        }

        break;
    }

    
    if (p[-1] == '\n') {
        p -= 1;
        if (p > input->line && p[-1] == '\r') {
            p -= 1;
        }
        p[0] = '\0';
    }
    else if (p[-1] == '\r') {
        p -= 1;
        p[0] = '\0';
    }

    
    return (p - input->line);

INPUT_ERROR:
    if (feof(file)) {
        input->eof = 1;
        return LINEINPUT_EOF;
    }
    input->error = errno;
    return LINEINPUT_ERROR;
}

