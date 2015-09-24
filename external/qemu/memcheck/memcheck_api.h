/* Copyright (C) 2007-2010 The Android Open Source Project
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


#ifndef QEMU_MEMCHECK_MEMCHECK_API_H
#define QEMU_MEMCHECK_MEMCHECK_API_H

#ifndef CONFIG_MEMCHECK
#error CONFIG_MEMCHECK is not defined.
#endif  

extern int memcheck_enabled;

extern int memcheck_instrument_mmu;

extern int memcheck_watch_call_stack;

extern void** gen_opc_tpc2gpc_ptr;

extern unsigned int gen_opc_tpc2gpc_pairs;

int memcheck_is_checked(target_ulong addr, uint32_t size);

int memcheck_validate_ld(target_ulong addr,
                         uint32_t data_size,
                         target_ulong retaddr);

/* Validates __stx_mmu operations.
 * Param:
 *  addr - Virtual address in the guest space where memory is written.
 *  data_size - Size of the write.
 *  value - Value to be written. Note that we typecast all values to 64 bits,
 *      since this will fit all data sizes.
 *  retaddr - Code address (in TB) that accesses memory.
 * Return:
 *  1 Address should be invalidated in TLB cache, in order to ensure that
 *  subsequent attempts to read from that page will launch __ld/__stx_mmu.
 *  If this routine returns zero, no page invalidation is requried.
 */
int memcheck_validate_st(target_ulong addr,
                         uint32_t data_size,
                         uint64_t value,
                         target_ulong retaddr);

void memcheck_on_call(target_ulong pc, target_ulong ret);

void memcheck_on_ret(target_ulong pc);

#endif  
