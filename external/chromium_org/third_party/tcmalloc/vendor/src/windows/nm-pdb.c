/* Copyright (c) 2008, Google Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ---
 * Author: David Vitek
 *
 * Dump function addresses using Microsoft debug symbols.  This works
 * on PDB files.  Note that this program will download symbols to
 * c:\websymbols without asking.
 */

#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>   

#include <windows.h>
#include <dbghelp.h>


#define SEARCH_CAP (1024*1024)
#define WEBSYM "SRV*c:\\websymbols*http://msdl.microsoft.com/download/symbols"

typedef struct {
  char *name;
  ULONG64 addr;
  ULONG flags;
} SYM;

typedef struct {
  ULONG64 module_base;
  SYM *syms;
  DWORD syms_len;
  DWORD syms_cap;
} SYM_CONTEXT;

static int sym_cmp(const void *_s1, const void *_s2) {
  const SYM *s1 = (const SYM *)_s1;
  const SYM *s2 = (const SYM *)_s2;

  if (s1->addr < s2->addr)
    return -1;
  if (s1->addr > s2->addr)
    return 1;
  return 0;
}

static BOOL CALLBACK EnumSymProc(PSYMBOL_INFO symbol_info,
                                 ULONG symbol_size,
                                 PVOID user_context) {
  SYM_CONTEXT *ctx = (SYM_CONTEXT*)user_context;
  if (symbol_info->Address < ctx->module_base ||
      (symbol_info->Flags & SYMFLAG_TLSREL)) {
    return TRUE;
  }
  if (ctx->syms_len == ctx->syms_cap) {
    if (!ctx->syms_cap)
      ctx->syms_cap++;
    ctx->syms_cap *= 2;
    ctx->syms = realloc(ctx->syms, sizeof(ctx->syms[0]) * ctx->syms_cap);
  }
  ctx->syms[ctx->syms_len].name = _strdup(symbol_info->Name);
  ctx->syms[ctx->syms_len].addr = symbol_info->Address;
  ctx->syms[ctx->syms_len].flags = symbol_info->Flags;
  ctx->syms_len++;
  return TRUE;
}

static void MaybePrint(const char* var, const char* description) {
  if (var[0])
    printf("%s: %s\n", description, var);
}

static void PrintAvailability(BOOL var, const char *description) {
  printf("s: %s\n", description, (var ? "Available" : "Not available"));
}

static void ShowSymbolInfo(HANDLE process, ULONG64 module_base) {
  
  IMAGEHLP_MODULE64 module_info;
  BOOL getmoduleinfo_rv;
  printf("Load Address: %I64x\n", module_base);
  memset(&module_info, 0, sizeof(module_info));
  module_info.SizeOfStruct = sizeof(module_info);
  getmoduleinfo_rv = SymGetModuleInfo64(process, module_base, &module_info);
  if (!getmoduleinfo_rv)  {
    printf("Error: SymGetModuleInfo64() failed. Error code: %u\n",
           GetLastError());
    return;
  }
  
  switch (module_info.SymType)  {
    case SymNone:
      printf(("No symbols available for the module.\n"));
      break;
    case SymExport:
      printf(("Loaded symbols: Exports\n"));
      break;
    case SymCoff:
      printf(("Loaded symbols: COFF\n"));
      break;
    case SymCv:
      printf(("Loaded symbols: CodeView\n"));
      break;
    case SymSym:
      printf(("Loaded symbols: SYM\n"));
      break;
    case SymVirtual:
      printf(("Loaded symbols: Virtual\n"));
      break;
    case SymPdb:
      printf(("Loaded symbols: PDB\n"));
      break;
    case SymDia:
      printf(("Loaded symbols: DIA\n"));
      break;
    case SymDeferred:
      printf(("Loaded symbols: Deferred\n"));  
      break;
    default:
      printf(("Loaded symbols: Unknown format.\n"));
      break;
  }

  MaybePrint("Image name", module_info.ImageName);
  MaybePrint("Loaded image name", module_info.LoadedImageName);
#ifdef VC8_OR_ABOVE   
  MaybePrint("PDB file name", module_info.LoadedPdbName);
  if (module_info.PdbUnmatched || module_info.DbgUnmatched)  {
    printf(("Warning: Unmatched symbols.\n"));
  }
#endif

  
#ifdef VC8_OR_ABOVE   
  PrintAvailability("Line numbers", module_info.LineNumbers);
  PrintAvailability("Global symbols", module_info.GlobalSymbols);
  PrintAvailability("Type information", module_info.TypeInfo);
#endif
}

void usage() {
  fprintf(stderr, "usage: nm-pdb [-C|--demangle] <module or filename>\n");
}

int main(int argc, char *argv[]) {
  DWORD  error;
  HANDLE process;
  ULONG64 module_base;
  SYM_CONTEXT ctx;
  int i;
  char* search;
  char* filename = NULL;
  int rv = 0;
  
  DWORD symopts = SYMOPT_DEFERRED_LOADS | SYMOPT_DEBUG;

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--demangle") == 0 || strcmp(argv[i], "-C") == 0) {
      symopts |= SYMOPT_UNDNAME;
    } else if (strcmp(argv[i], "--help") == 0) {
      usage();
      exit(0);
    } else {
      break;
    }
  }
  if (i != argc - 1) {
    usage();
    exit(1);
  }
  filename = argv[i];

  process = GetCurrentProcess();

  if (!SymInitialize(process, NULL, FALSE)) {
    error = GetLastError();
    fprintf(stderr, "SymInitialize returned error : %d\n", error);
    return 1;
  }

  search = malloc(SEARCH_CAP);
  if (SymGetSearchPath(process, search, SEARCH_CAP)) {
    if (strlen(search) + sizeof(";" WEBSYM) > SEARCH_CAP) {
      fprintf(stderr, "Search path too long\n");
      SymCleanup(process);
      return 1;
    }
    strcat(search, ";" WEBSYM);
  } else {
    error = GetLastError();
    fprintf(stderr, "SymGetSearchPath returned error : %d\n", error);
    rv = 1;                   
    strcpy(search, WEBSYM);   
  }
  if (!SymSetSearchPath(process, search)) {
    error = GetLastError();
    fprintf(stderr, "SymSetSearchPath returned error : %d\n", error);
    rv = 1;                   
 }

  SymSetOptions(symopts);
  module_base = SymLoadModuleEx(process, NULL, filename, NULL, 0, 0, NULL, 0);
  if (!module_base) {
    
    error = GetLastError();
    fprintf(stderr, "SymLoadModuleEx returned error : %d for %s\n",
            error, filename);
    SymCleanup(process);
    return 1;
  }

  ShowSymbolInfo(process, module_base);

  memset(&ctx, 0, sizeof(ctx));
  ctx.module_base = module_base;
  if (!SymEnumSymbols(process, module_base, NULL, EnumSymProc, &ctx)) {
    error = GetLastError();
    fprintf(stderr, "SymEnumSymbols returned error: %d\n", error);
    rv = 1;
  } else {
    DWORD j;
    qsort(ctx.syms, ctx.syms_len, sizeof(ctx.syms[0]), sym_cmp);
    for (j = 0; j < ctx.syms_len; j++) {
      printf("%016I64x X %s\n", ctx.syms[j].addr, ctx.syms[j].name);
    }
    
  }
  SymUnloadModule64(process, module_base);
  SymCleanup(process);
  return rv;
}
