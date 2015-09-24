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
#ifndef _ANDROID_UTILS_LIST_H
#define _ANDROID_UTILS_LIST_H

#include <inttypes.h>

typedef struct ACList ACList;
struct ACList {
    
    ACList*  next;
    
    ACList*  prev;
};

AINLINED void
alist_init(ACList* list)
{
    list->next = list->prev = list;
}

AINLINED int
alist_is_empty(const ACList* list)
{
    return list->next == list;
}

AINLINED void
alist_insert_head(ACList* list, ACList* entry)
{
    ACList* const next = list->next;
    entry->next = next;
    entry->prev = list;
    next->prev = entry;
    list->next = entry;
}
AINLINED void
alist_insert_tail(ACList* list, ACList* entry)
{
    ACList* const prev = list->prev;
    entry->next = list;
    entry->prev = prev;
    prev->next = entry;
    list->prev = entry;
}

AINLINED void
alist_remove(ACList* entry)
{
    ACList* const next = entry->next;
    ACList* const prev = entry->prev;
    prev->next = next;
    next->prev = prev;
    entry->next = entry->prev = entry;
}

AINLINED ACList*
alist_remove_head(ACList* list)
{
    ACList* entry = NULL;
    if (!alist_is_empty(list)) {
        entry = list->next;
        list->next = entry->next;
        entry->next->prev = list;
        entry->next = entry->prev = entry;
    }
    return entry;
}

AINLINED ACList*
alist_remove_tail(ACList* list)
{
    ACList* entry = NULL;
    if (!alist_is_empty(list)) {
        entry = list->prev;
        list->prev = entry->prev;
        entry->prev->next = list;
        entry->next = entry->prev = entry;
    }
    return entry;
}

#endif  
