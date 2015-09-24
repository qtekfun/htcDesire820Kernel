/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */


#ifndef _MSC_VER
#warn "msvc/config.h shouldn't be included for your development environment."
#error "Please make sure the msvc/ directory is removed from your build path."
#endif

#pragma warning(disable:4200)
#pragma warning(disable: 6258)
#if defined(_PREFAST_)
#pragma warning(disable:28719)
#pragma warning(disable:28125)
#endif

#define DEFAULT_VISIBILITY 

#define ENABLE_LOGGING 1


#define POLL_NFDS_TYPE unsigned int

#if defined(_WIN32_WCE)
#define OS_WINCE 1
#define HAVE_MISSING_H
#else
#define OS_WINDOWS 1
#define HAVE_SIGNAL_H 1
#define HAVE_SYS_TYPES_H 1
#endif
