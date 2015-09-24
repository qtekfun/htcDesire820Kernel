
#ifndef CT_COSTS
#define CT_COSTS

#include "pub_tool_basics.h"

#define CLG_(str) VGAPPEND(vgCallgrind_,str)

extern UInt CLG_(costarray_entries);
extern UInt CLG_(costarray_chunks);

typedef struct _CostChunk CostChunk;
struct _CostChunk {
  Int size;
  Int used;
  CostChunk *next, *prev;
  ULong data[0];
};

ULong* CLG_(get_costarray)(Int size);
void CLG_(free_costarrays)(void);


#endif 
