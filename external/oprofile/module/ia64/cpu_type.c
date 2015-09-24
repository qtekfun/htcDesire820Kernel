/**
 * @file cpu_type.c
 * CPU determination
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 * @author Will Cohen
 */

#include "oprofile.h"

EXPORT_NO_SYMBOLS;

__init op_cpu get_cpu_type(void)
{
	__u8 family = local_cpu_data->family;

	
	switch (family) {
	
	case 0x07:
		return CPU_IA64_1;
		break;
	
	case 0x1f:
		return CPU_IA64_2;
		break;
	}
	
	return CPU_IA64;
}
