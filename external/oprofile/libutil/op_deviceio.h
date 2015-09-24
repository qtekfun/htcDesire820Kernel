/**
 * @file op_deviceio.h
 * Reading from a special device
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#ifndef OP_DEVICEIO_H
#define OP_DEVICEIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "op_types.h"

#include <unistd.h>

fd_t op_open_device(char const * name);

ssize_t op_read_device(fd_t devfd, void * buf, size_t size);

#ifdef __cplusplus
}
#endif

#endif 