/*
 * hash.c: chained hash tables
 *
 * Reference: Your favorite introductory book on algorithms
 *
 * Copyright (C) 2000 Bjorn Reese and Daniel Veillard.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. THE AUTHORS AND
 * CONTRIBUTORS ACCEPT NO RESPONSIBILITY IN ANY CONCEIVABLE MANNER.
 *
 * Author: breese@users.sourceforge.net
 */

#define IN_LIBXML
#include "libxml.h"

#include <string.h>
#include <libxml/parser.h>
#include <libxml/hash.h>
#include <libxml/xmlmemory.h>
#include <libxml/xmlerror.h>
#include <libxml/globals.h>

#define MAX_HASH_LEN 8


typedef struct _xmlHashEntry xmlHashEntry;
typedef xmlHashEntry *xmlHashEntryPtr;
struct _xmlHashEntry {
    struct _xmlHashEntry *next;
    xmlChar *name;
    xmlChar *name2;
    xmlChar *name3;
    void *payload;
    int valid;
};

struct _xmlHashTable {
    struct _xmlHashEntry *table;
    int size;
    int nbElems;
    xmlDictPtr dict;
};

static unsigned long
xmlHashComputeKey(xmlHashTablePtr table, const xmlChar *name,
	          const xmlChar *name2, const xmlChar *name3) {
    unsigned long value = 0L;
    char ch;
    
    if (name != NULL) {
	value += 30 * (*name);
	while ((ch = *name++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
    }
    if (name2 != NULL) {
	while ((ch = *name2++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
    }
    if (name3 != NULL) {
	while ((ch = *name3++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
    }
    return (value % table->size);
}

static unsigned long
xmlHashComputeQKey(xmlHashTablePtr table,
		   const xmlChar *prefix, const xmlChar *name,
		   const xmlChar *prefix2, const xmlChar *name2,
		   const xmlChar *prefix3, const xmlChar *name3) {
    unsigned long value = 0L;
    char ch;
    
    if (prefix != NULL)
	value += 30 * (*prefix);
    else
	value += 30 * (*name);

    if (prefix != NULL) {
	while ((ch = *prefix++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
	value = value ^ ((value << 5) + (value >> 3) + (unsigned long)':');
    }
    if (name != NULL) {
	while ((ch = *name++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
    }
    if (prefix2 != NULL) {
	while ((ch = *prefix2++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
	value = value ^ ((value << 5) + (value >> 3) + (unsigned long)':');
    }
    if (name2 != NULL) {
	while ((ch = *name2++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
    }
    if (prefix3 != NULL) {
	while ((ch = *prefix3++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
	value = value ^ ((value << 5) + (value >> 3) + (unsigned long)':');
    }
    if (name3 != NULL) {
	while ((ch = *name3++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned long)ch);
	}
    }
    return (value % table->size);
}

xmlHashTablePtr
xmlHashCreate(int size) {
    xmlHashTablePtr table;
  
    if (size <= 0)
        size = 256;
  
    table = xmlMalloc(sizeof(xmlHashTable));
    if (table) {
        table->dict = NULL;
        table->size = size;
	table->nbElems = 0;
        table->table = xmlMalloc(size * sizeof(xmlHashEntry));
        if (table->table) {
  	    memset(table->table, 0, size * sizeof(xmlHashEntry));
  	    return(table);
        }
        xmlFree(table);
    }
    return(NULL);
}

xmlHashTablePtr
xmlHashCreateDict(int size, xmlDictPtr dict) {
    xmlHashTablePtr table;

    table = xmlHashCreate(size);
    if (table != NULL) {
        table->dict = dict;
	xmlDictReference(dict);
    }
    return(table);
}

static int
xmlHashGrow(xmlHashTablePtr table, int size) {
    unsigned long key;
    int oldsize, i;
    xmlHashEntryPtr iter, next;
    struct _xmlHashEntry *oldtable;
#ifdef DEBUG_GROW
    unsigned long nbElem = 0;
#endif
  
    if (table == NULL)
	return(-1);
    if (size < 8)
        return(-1);
    if (size > 8 * 2048)
	return(-1);

    oldsize = table->size;
    oldtable = table->table;
    if (oldtable == NULL)
        return(-1);
  
    table->table = xmlMalloc(size * sizeof(xmlHashEntry));
    if (table->table == NULL) {
	table->table = oldtable;
	return(-1);
    }
    memset(table->table, 0, size * sizeof(xmlHashEntry));
    table->size = size;

    for (i = 0; i < oldsize; i++) {
	if (oldtable[i].valid == 0) 
	    continue;
	key = xmlHashComputeKey(table, oldtable[i].name, oldtable[i].name2,
				oldtable[i].name3);
	memcpy(&(table->table[key]), &(oldtable[i]), sizeof(xmlHashEntry));
	table->table[key].next = NULL;
    }

    for (i = 0; i < oldsize; i++) {
	iter = oldtable[i].next;
	while (iter) {
	    next = iter->next;


	    key = xmlHashComputeKey(table, iter->name, iter->name2,
		                    iter->name3);
	    if (table->table[key].valid == 0) {
		memcpy(&(table->table[key]), iter, sizeof(xmlHashEntry));
		table->table[key].next = NULL;
		xmlFree(iter);
	    } else {
	    	iter->next = table->table[key].next;
	    	table->table[key].next = iter;
	    }

#ifdef DEBUG_GROW
	    nbElem++;
#endif

	    iter = next;
	}
    }

    xmlFree(oldtable);

#ifdef DEBUG_GROW
    xmlGenericError(xmlGenericErrorContext,
	    "xmlHashGrow : from %d to %d, %d elems\n", oldsize, size, nbElem);
#endif

    return(0);
}

void
xmlHashFree(xmlHashTablePtr table, xmlHashDeallocator f) {
    int i;
    xmlHashEntryPtr iter;
    xmlHashEntryPtr next;
    int inside_table = 0;
    int nbElems;

    if (table == NULL)
	return;
    if (table->table) {
	nbElems = table->nbElems;
	for(i = 0; (i < table->size) && (nbElems > 0); i++) {
	    iter = &(table->table[i]);
	    if (iter->valid == 0)
		continue;
	    inside_table = 1;
	    while (iter) {
		next = iter->next;
		if ((f != NULL) && (iter->payload != NULL))
		    f(iter->payload, iter->name);
		if (table->dict == NULL) {
		    if (iter->name)
			xmlFree(iter->name);
		    if (iter->name2)
			xmlFree(iter->name2);
		    if (iter->name3)
			xmlFree(iter->name3);
		}
		iter->payload = NULL;
		if (!inside_table)
		    xmlFree(iter);
		nbElems--;
		inside_table = 0;
		iter = next;
	    }
	}
	xmlFree(table->table);
    }
    if (table->dict)
        xmlDictFree(table->dict);
    xmlFree(table);
}

int
xmlHashAddEntry(xmlHashTablePtr table, const xmlChar *name, void *userdata) {
    return(xmlHashAddEntry3(table, name, NULL, NULL, userdata));
}

int
xmlHashAddEntry2(xmlHashTablePtr table, const xmlChar *name,
	        const xmlChar *name2, void *userdata) {
    return(xmlHashAddEntry3(table, name, name2, NULL, userdata));
}

int
xmlHashUpdateEntry(xmlHashTablePtr table, const xmlChar *name,
	           void *userdata, xmlHashDeallocator f) {
    return(xmlHashUpdateEntry3(table, name, NULL, NULL, userdata, f));
}

int
xmlHashUpdateEntry2(xmlHashTablePtr table, const xmlChar *name,
	           const xmlChar *name2, void *userdata,
		   xmlHashDeallocator f) {
    return(xmlHashUpdateEntry3(table, name, name2, NULL, userdata, f));
}

void *
xmlHashLookup(xmlHashTablePtr table, const xmlChar *name) {
    return(xmlHashLookup3(table, name, NULL, NULL));
}

void *
xmlHashLookup2(xmlHashTablePtr table, const xmlChar *name,
	      const xmlChar *name2) {
    return(xmlHashLookup3(table, name, name2, NULL));
}

void *
xmlHashQLookup(xmlHashTablePtr table, const xmlChar *prefix,
               const xmlChar *name) {
    return(xmlHashQLookup3(table, prefix, name, NULL, NULL, NULL, NULL));
}

void *
xmlHashQLookup2(xmlHashTablePtr table, const xmlChar *prefix,
                const xmlChar *name, const xmlChar *prefix2,
	        const xmlChar *name2) {
    return(xmlHashQLookup3(table, prefix, name, prefix2, name2, NULL, NULL));
}

int
xmlHashAddEntry3(xmlHashTablePtr table, const xmlChar *name,
	         const xmlChar *name2, const xmlChar *name3,
		 void *userdata) {
    unsigned long key, len = 0;
    xmlHashEntryPtr entry;
    xmlHashEntryPtr insert;

    if ((table == NULL) || (name == NULL))
	return(-1);

    if (table->dict) {
        if (!xmlDictOwns(table->dict, name)) {
	    name = xmlDictLookup(table->dict, name, -1);
	    if (name == NULL)
	        return(-1);
	}
        if ((name2 != NULL) && (!xmlDictOwns(table->dict, name2))) {
	    name2 = xmlDictLookup(table->dict, name2, -1);
	    if (name2 == NULL)
	        return(-1);
	}
        if ((name3 != NULL) && (!xmlDictOwns(table->dict, name3))) {
	    name3 = xmlDictLookup(table->dict, name3, -1);
	    if (name3 == NULL)
	        return(-1);
	}
    }

    key = xmlHashComputeKey(table, name, name2, name3);
    if (table->table[key].valid == 0) {
	insert = NULL;
    } else {
        if (table->dict) {
	    for (insert = &(table->table[key]); insert->next != NULL;
		 insert = insert->next) {
		if ((insert->name == name) &&
		    (insert->name2 == name2) &&
		    (insert->name3 == name3))
		    return(-1);
		len++;
	    }
	    if ((insert->name == name) &&
		(insert->name2 == name2) &&
		(insert->name3 == name3))
		return(-1);
	} else {
	    for (insert = &(table->table[key]); insert->next != NULL;
		 insert = insert->next) {
		if ((xmlStrEqual(insert->name, name)) &&
		    (xmlStrEqual(insert->name2, name2)) &&
		    (xmlStrEqual(insert->name3, name3)))
		    return(-1);
		len++;
	    }
	    if ((xmlStrEqual(insert->name, name)) &&
		(xmlStrEqual(insert->name2, name2)) &&
		(xmlStrEqual(insert->name3, name3)))
		return(-1);
	}
    }

    if (insert == NULL) {
	entry = &(table->table[key]);
    } else {
	entry = xmlMalloc(sizeof(xmlHashEntry));
	if (entry == NULL)
	     return(-1);
    }

    if (table->dict != NULL) {
        entry->name = (xmlChar *) name;
        entry->name2 = (xmlChar *) name2;
        entry->name3 = (xmlChar *) name3;
    } else {
	entry->name = xmlStrdup(name);
	entry->name2 = xmlStrdup(name2);
	entry->name3 = xmlStrdup(name3);
    }
    entry->payload = userdata;
    entry->next = NULL;
    entry->valid = 1;


    if (insert != NULL) 
	insert->next = entry;

    table->nbElems++;

    if (len > MAX_HASH_LEN)
	xmlHashGrow(table, MAX_HASH_LEN * table->size);

    return(0);
}

int
xmlHashUpdateEntry3(xmlHashTablePtr table, const xmlChar *name,
	           const xmlChar *name2, const xmlChar *name3,
		   void *userdata, xmlHashDeallocator f) {
    unsigned long key;
    xmlHashEntryPtr entry;
    xmlHashEntryPtr insert;

    if ((table == NULL) || name == NULL)
	return(-1);

    if (table->dict) {
        if (!xmlDictOwns(table->dict, name)) {
	    name = xmlDictLookup(table->dict, name, -1);
	    if (name == NULL)
	        return(-1);
	}
        if ((name2 != NULL) && (!xmlDictOwns(table->dict, name2))) {
	    name2 = xmlDictLookup(table->dict, name2, -1);
	    if (name2 == NULL)
	        return(-1);
	}
        if ((name3 != NULL) && (!xmlDictOwns(table->dict, name3))) {
	    name3 = xmlDictLookup(table->dict, name3, -1);
	    if (name3 == NULL)
	        return(-1);
	}
    }

    key = xmlHashComputeKey(table, name, name2, name3);
    if (table->table[key].valid == 0) {
	insert = NULL;
    } else {
        if (table ->dict) {
	    for (insert = &(table->table[key]); insert->next != NULL;
		 insert = insert->next) {
		if ((insert->name == name) &&
		    (insert->name2 == name2) &&
		    (insert->name3 == name3)) {
		    if (f)
			f(insert->payload, insert->name);
		    insert->payload = userdata;
		    return(0);
		}
	    }
	    if ((insert->name == name) &&
		(insert->name2 == name2) &&
		(insert->name3 == name3)) {
		if (f)
		    f(insert->payload, insert->name);
		insert->payload = userdata;
		return(0);
	    }
	} else {
	    for (insert = &(table->table[key]); insert->next != NULL;
		 insert = insert->next) {
		if ((xmlStrEqual(insert->name, name)) &&
		    (xmlStrEqual(insert->name2, name2)) &&
		    (xmlStrEqual(insert->name3, name3))) {
		    if (f)
			f(insert->payload, insert->name);
		    insert->payload = userdata;
		    return(0);
		}
	    }
	    if ((xmlStrEqual(insert->name, name)) &&
		(xmlStrEqual(insert->name2, name2)) &&
		(xmlStrEqual(insert->name3, name3))) {
		if (f)
		    f(insert->payload, insert->name);
		insert->payload = userdata;
		return(0);
	    }
	}
    }

    if (insert == NULL) {
	entry =  &(table->table[key]);
    } else {
	entry = xmlMalloc(sizeof(xmlHashEntry));
	if (entry == NULL)
	     return(-1);
    }

    if (table->dict != NULL) {
        entry->name = (xmlChar *) name;
        entry->name2 = (xmlChar *) name2;
        entry->name3 = (xmlChar *) name3;
    } else {
	entry->name = xmlStrdup(name);
	entry->name2 = xmlStrdup(name2);
	entry->name3 = xmlStrdup(name3);
    }
    entry->payload = userdata;
    entry->next = NULL;
    entry->valid = 1;
    table->nbElems++;


    if (insert != NULL) {
	insert->next = entry;
    }
    return(0);
}

void *
xmlHashLookup3(xmlHashTablePtr table, const xmlChar *name, 
	       const xmlChar *name2, const xmlChar *name3) {
    unsigned long key;
    xmlHashEntryPtr entry;

    if (table == NULL)
	return(NULL);
    if (name == NULL)
	return(NULL);
    key = xmlHashComputeKey(table, name, name2, name3);
    if (table->table[key].valid == 0)
	return(NULL);
    if (table->dict) {
	for (entry = &(table->table[key]); entry != NULL; entry = entry->next) {
	    if ((entry->name == name) &&
		(entry->name2 == name2) &&
		(entry->name3 == name3))
		return(entry->payload);
	}
    }
    for (entry = &(table->table[key]); entry != NULL; entry = entry->next) {
	if ((xmlStrEqual(entry->name, name)) &&
	    (xmlStrEqual(entry->name2, name2)) &&
	    (xmlStrEqual(entry->name3, name3)))
	    return(entry->payload);
    }
    return(NULL);
}

void *
xmlHashQLookup3(xmlHashTablePtr table,
                const xmlChar *prefix, const xmlChar *name,
		const xmlChar *prefix2, const xmlChar *name2,
		const xmlChar *prefix3, const xmlChar *name3) {
    unsigned long key;
    xmlHashEntryPtr entry;

    if (table == NULL)
	return(NULL);
    if (name == NULL)
	return(NULL);
    key = xmlHashComputeQKey(table, prefix, name, prefix2,
                             name2, prefix3, name3);
    if (table->table[key].valid == 0)
	return(NULL);
    for (entry = &(table->table[key]); entry != NULL; entry = entry->next) {
	if ((xmlStrQEqual(prefix, name, entry->name)) &&
	    (xmlStrQEqual(prefix2, name2, entry->name2)) &&
	    (xmlStrQEqual(prefix3, name3, entry->name3)))
	    return(entry->payload);
    }
    return(NULL);
}

typedef struct {
    xmlHashScanner hashscanner;
    void *data;
} stubData;

static void 
stubHashScannerFull (void *payload, void *data, const xmlChar *name, 
                     const xmlChar *name2 ATTRIBUTE_UNUSED,
		     const xmlChar *name3 ATTRIBUTE_UNUSED) {
    stubData *stubdata = (stubData *) data;
    stubdata->hashscanner (payload, stubdata->data, (xmlChar *) name);
}                                  
 
void
xmlHashScan(xmlHashTablePtr table, xmlHashScanner f, void *data) {
    stubData stubdata;
    stubdata.data = data;
    stubdata.hashscanner = f; 
    xmlHashScanFull (table, stubHashScannerFull, &stubdata);
}

void
xmlHashScanFull(xmlHashTablePtr table, xmlHashScannerFull f, void *data) {
    int i, nb;
    xmlHashEntryPtr iter;
    xmlHashEntryPtr next;

    if (table == NULL)
	return;
    if (f == NULL)
	return;

    if (table->table) {
	for(i = 0; i < table->size; i++) {
	    if (table->table[i].valid == 0) 
		continue;
	    iter = &(table->table[i]);
	    while (iter) {
		next = iter->next;
                nb = table->nbElems;
		if ((f != NULL) && (iter->payload != NULL))
		    f(iter->payload, data, iter->name,
		      iter->name2, iter->name3);
                if (nb != table->nbElems) {
                    
                    if (iter == &(table->table[i])) {
                        if (table->table[i].valid == 0)
                            iter = NULL;
                        if (table->table[i].next != next)
			    iter = &(table->table[i]);
                    } else
		        iter = next;
                } else
		    iter = next;
	    }
	}
    }
}

void
xmlHashScan3(xmlHashTablePtr table, const xmlChar *name, 
	     const xmlChar *name2, const xmlChar *name3,
	     xmlHashScanner f, void *data) {
    xmlHashScanFull3 (table, name, name2, name3,
		      (xmlHashScannerFull) f, data);
}

void
xmlHashScanFull3(xmlHashTablePtr table, const xmlChar *name, 
		 const xmlChar *name2, const xmlChar *name3,
		 xmlHashScannerFull f, void *data) {
    int i;
    xmlHashEntryPtr iter;
    xmlHashEntryPtr next;

    if (table == NULL)
	return;
    if (f == NULL)
	return;

    if (table->table) {
	for(i = 0; i < table->size; i++) {
	    if (table->table[i].valid == 0)
		continue;
	    iter = &(table->table[i]);
	    while (iter) {
		next = iter->next;
		if (((name == NULL) || (xmlStrEqual(name, iter->name))) &&
		    ((name2 == NULL) || (xmlStrEqual(name2, iter->name2))) &&
		    ((name3 == NULL) || (xmlStrEqual(name3, iter->name3))) &&
		    (iter->payload != NULL)) {
		    f(iter->payload, data, iter->name,
		      iter->name2, iter->name3);
		}
		iter = next;
	    }
	}
    }
}

xmlHashTablePtr
xmlHashCopy(xmlHashTablePtr table, xmlHashCopier f) {
    int i;
    xmlHashEntryPtr iter;
    xmlHashEntryPtr next;
    xmlHashTablePtr ret;

    if (table == NULL)
	return(NULL);
    if (f == NULL)
	return(NULL);

    ret = xmlHashCreate(table->size);
    if (table->table) {
	for(i = 0; i < table->size; i++) {
	    if (table->table[i].valid == 0)
		continue;
	    iter = &(table->table[i]);
	    while (iter) {
		next = iter->next;
		xmlHashAddEntry3(ret, iter->name, iter->name2,
			         iter->name3, f(iter->payload, iter->name));
		iter = next;
	    }
	}
    }
    ret->nbElems = table->nbElems;
    return(ret);
}

int
xmlHashSize(xmlHashTablePtr table) {
    if (table == NULL)
	return(-1);
    return(table->nbElems);
}

int xmlHashRemoveEntry(xmlHashTablePtr table, const xmlChar *name,
		       xmlHashDeallocator f) {
    return(xmlHashRemoveEntry3(table, name, NULL, NULL, f));
}

int
xmlHashRemoveEntry2(xmlHashTablePtr table, const xmlChar *name,
			const xmlChar *name2, xmlHashDeallocator f) {
    return(xmlHashRemoveEntry3(table, name, name2, NULL, f));
}

int
xmlHashRemoveEntry3(xmlHashTablePtr table, const xmlChar *name,
    const xmlChar *name2, const xmlChar *name3, xmlHashDeallocator f) {
    unsigned long key;
    xmlHashEntryPtr entry;
    xmlHashEntryPtr prev = NULL;

    if (table == NULL || name == NULL)
        return(-1);

    key = xmlHashComputeKey(table, name, name2, name3);
    if (table->table[key].valid == 0) {
        return(-1);
    } else {
        for (entry = &(table->table[key]); entry != NULL; entry = entry->next) {
            if (xmlStrEqual(entry->name, name) &&
                    xmlStrEqual(entry->name2, name2) &&
                    xmlStrEqual(entry->name3, name3)) {
                if ((f != NULL) && (entry->payload != NULL))
                    f(entry->payload, entry->name);
                entry->payload = NULL;
		if (table->dict == NULL) {
		    if(entry->name)
			xmlFree(entry->name);
		    if(entry->name2)
			xmlFree(entry->name2);
		    if(entry->name3)
			xmlFree(entry->name3);
		}
                if(prev) {
                    prev->next = entry->next;
		    xmlFree(entry);
		} else {
		    if (entry->next == NULL) {
			entry->valid = 0;
		    } else {
			entry = entry->next;
			memcpy(&(table->table[key]), entry, sizeof(xmlHashEntry));
			xmlFree(entry);
		    }
		}
                table->nbElems--;
                return(0);
            }
            prev = entry;
        }
        return(-1);
    }
}

#define bottom_hash
#include "elfgcchack.h"
