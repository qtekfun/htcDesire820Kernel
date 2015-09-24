/* Copyright (c) 2008 The Android Open Source Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */


#ifndef DYNLINK_FUNCTIONS
#error DYNLINK_FUNCTIONS should be defined when including this file
#endif

#ifndef DYNLINK_FUNCTIONS_INIT
#error DYNLINK_FUNCTIONS_INIT should be defined when including this file
#endif

#undef DYNLINK_FUNC

#define  DYNLINK_FUNC(ret,name,sig) \
      static ret  (*_dynlink_##name) sig ;

#define  DYNLINK_STR(name)   DYNLINK_STR_(name)
#define  DYNLINK_STR_(name)  #name

DYNLINK_FUNCTIONS
#undef DYNLINK_FUNC

static int
DYNLINK_FUNCTIONS_INIT(void*  library)
{
#define  DYNLINK_FUNC(ret,name,sig) \
    do { \
        _dynlink_##name = dlsym( library, DYNLINK_STR(name) ); \
        if (_dynlink_##name == NULL) goto Fail; \
    } while (0);

    DYNLINK_FUNCTIONS
#undef DYNLINK_FUNC

    return 0;
Fail:
    return -1;
}

#ifndef FF
#define FF(name)   (*_dynlink_##name)
#endif

#undef DYNLINK_FUNC
#undef DYNLINK_FUNCTIONS
#undef DYNLINK_FUNCTIONS_INIT
