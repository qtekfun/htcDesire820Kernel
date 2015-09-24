
/**
 * \file
 * <PRE>
 * High performance base64 encoder / decoder
 * Version 1.3 -- 17-Mar-2006
 *
 * Copyright &copy; 2005, 2006, Nick Galbreath -- nickg [at] modp [dot] com
 * All rights reserved.
 *
 * http://modp.com/release/base64
 *
 * Released under bsd license.  See modp_b64.c for details.
 * </pre>
 *
 * The default implementation is the standard b64 encoding with padding.
 * It's easy to change this to use "URL safe" characters and to remove
 * padding.  See the modp_b64.c source code for details.
 *
 */

#ifndef MODP_B64
#define MODP_B64

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t modp_b64_encode(char* dest, const char* str, size_t len);

size_t modp_b64_decode(char* dest, const char* src, size_t len);

#define modp_b64_encode_len(A) ((A+2)/3 * 4 + 1)

/**
 * Given a base64 string of length len,
 *   this returns the amount of memory required for output string
 *  It maybe be more than the actual number of bytes written.
 * NOTE: remember this is integer math
 * this allocates a bit more memory than traditional versions of b64
 * decode  4 chars turn into 3 bytes
 * floor[len * 3/4] + 2
 */
#define modp_b64_decode_len(A) (A / 4 * 3 + 2)

#define modp_b64_encode_strlen(A) ((A + 2)/ 3 * 4)

#define MODP_B64_ERROR ((size_t)-1)

#ifdef __cplusplus
}

#include <string>

inline std::string& modp_b64_encode(std::string& s)
{
    std::string x(modp_b64_encode_len(s.size()), '\0');
    size_t d = modp_b64_encode(const_cast<char*>(x.data()), s.data(), (int)s.size());
    x.erase(d, std::string::npos);
    s.swap(x);
    return s;
}

inline std::string& modp_b64_decode(std::string& s)
{
    std::string x(modp_b64_decode_len(s.size()), '\0');
    size_t d = modp_b64_decode(const_cast<char*>(x.data()), s.data(), (int)s.size());
    if (d == MODP_B64_ERROR) {
        x.clear();
    } else {
        x.erase(d, std::string::npos);
    }
    s.swap(x);
    return s;
}

#endif 

#endif 
