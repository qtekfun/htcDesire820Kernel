

/*
   This file is part of Valgrind, a dynamic binary instrumentation
   framework.

   Copyright (C) 2000-2012 Julian Seward 
      jseward@acm.org

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307, USA.

   The GNU General Public License is contained in the file COPYING.
*/

#include "pub_core_basics.h"
#include "pub_core_debuglog.h"
#include "pub_core_libcassert.h"
#include "pub_core_libcprint.h"
#include "pub_core_mallocfree.h"
#include "pub_core_options.h"
#include "pub_core_stacks.h"
#include "pub_core_tooliface.h"


typedef struct _Stack {
   UWord id;
   Addr start;
   Addr end;
   struct _Stack *next;
} Stack;

static Stack *stacks;
static UWord next_id;  

static Stack *current_stack;

static void move_Stack_one_step_forward ( Stack* st )
{
   Stack *st0, *st1, *st2;
   if (st == stacks)
      return; 
   vg_assert(st != NULL);
   st0 = stacks;
   st1 = NULL;
   st2 = NULL;
   while (True) {
      if (st0 == NULL || st0 == st) break;
      st2 = st1;
      st1 = st0;
      st0 = st0->next;
   }
   vg_assert(st0 == st);
   if (st0 != NULL && st1 != NULL && st2 != NULL) {
      Stack* tmp;
      vg_assert(st2->next == st1);
      vg_assert(st1->next == st0);
      tmp = st0->next;
      st2->next = st0;
      st0->next = st1;
      st1->next = tmp;
   }
   else
   if (st0 != NULL && st1 != NULL && st2 == NULL) {
      
      vg_assert(stacks == st1);
      vg_assert(st1->next == st0);
      st1->next = st0->next;
      st0->next = st1;
      stacks = st0;
   }
}

static Stack* find_stack_by_addr(Addr sp)
{
   static UWord n_fails = 0;
   static UWord n_searches = 0;
   static UWord n_steps = 0;
   Stack *i = stacks;
   n_searches++;
   if (0 && 0 == (n_searches % 10000))
      VG_(printf)("(hgdev) %lu searches, %lu steps, %lu fails\n",
                  n_searches, n_steps+1, n_fails);
   
   if (i && sp >= i->start && sp <= i->end)
      return i;
   
   while (i) {
      n_steps++;
      if (sp >= i->start && sp <= i->end) {
         if (1 && (n_searches & 0x3F) == 0) {
            move_Stack_one_step_forward( i );
         }
         return i;
      }
      i = i->next;
   }
   n_fails++;
   return NULL;
}

UWord VG_(register_stack)(Addr start, Addr end)
{
   Stack *i;

   if (start > end) {
      Addr t = end;
      end = start;
      start = t;
   }

   i = (Stack *)VG_(arena_malloc)(VG_AR_CORE, "stacks.rs.1", sizeof(Stack));
   i->start = start;
   i->end = end;
   i->id = next_id++;
   i->next = stacks;
   stacks = i;

   if (i->id == 0) {
      current_stack = i;
   }

   VG_(debugLog)(2, "stacks", "register %p-%p as stack %lu\n",
                    (void*)start, (void*)end, i->id);

   return i->id;
}

void VG_(deregister_stack)(UWord id)
{
   Stack *i = stacks;
   Stack *prev = NULL;

   VG_(debugLog)(2, "stacks", "deregister stack %lu\n", id);

   if (current_stack && current_stack->id == id) { 
      current_stack = NULL;
   }

   while(i) {
      if (i->id == id) {
         if(prev == NULL) {
            stacks = i->next;
         } else {
            prev->next = i->next;
         }
         VG_(arena_free)(VG_AR_CORE, i);
         return;
      }
      prev = i;
      i = i->next;
   }
}

void VG_(change_stack)(UWord id, Addr start, Addr end)
{
   Stack *i = stacks;

   while (i) {
      if (i->id == id) {
         VG_(debugLog)(2, "stacks", "change stack %lu from %p-%p to %p-%p\n",
                       id, (void*)i->start, (void*)i->end,
                           (void*)start,    (void*)end);
         i->start = start;
         i->end = end;
         return;
      }
      i = i->next;
   }
}

void VG_(stack_limits)(Addr SP, Addr *start, Addr *end )
{
   Stack* stack = find_stack_by_addr(SP);

   if (stack) {
      *start = stack->start;
      *end = stack->end;
   }
}

VG_REGPARM(3)
void VG_(unknown_SP_update)( Addr old_SP, Addr new_SP, UInt ecu )
{
   static Int moans = 3;
   Word delta  = (Word)new_SP - (Word)old_SP;

   
   if (current_stack == NULL ||
       new_SP < current_stack->start || new_SP > current_stack->end) {
      Stack* new_stack = find_stack_by_addr(new_SP);
      if (new_stack 
          && (current_stack == NULL || new_stack->id != current_stack->id)) { 
         current_stack = new_stack;
         return;
      }
   }

   if (delta < -VG_(clo_max_stackframe) || VG_(clo_max_stackframe) < delta) {
      if (VG_(clo_verbosity) > 0 && moans > 0 && !VG_(clo_xml)) {
         moans--;
         VG_(message)(Vg_UserMsg,
            "Warning: client switching stacks?  "
            "SP change: 0x%lx --> 0x%lx\n", old_SP, new_SP);
         VG_(message)(Vg_UserMsg,
            "         to suppress, use: --max-stackframe=%ld or greater\n",
            (delta < 0 ? -delta : delta));
         if (moans == 0)
            VG_(message)(Vg_UserMsg,
                "         further instances of this message "
                "will not be shown.\n");
      }
   } else if (delta < 0) {
      VG_TRACK( new_mem_stack_w_ECU, new_SP, -delta, ecu );
      VG_TRACK( new_mem_stack,       new_SP, -delta );

   } else if (delta > 0) {
      VG_TRACK( die_mem_stack, old_SP,  delta );
   }
}


