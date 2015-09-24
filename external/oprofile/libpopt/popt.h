

#ifndef H_POPT
#define H_POPT

#include <stdio.h>			

#define POPT_OPTION_DEPTH	10

#define POPT_ARG_NONE		0	
#define POPT_ARG_STRING		1	
#define POPT_ARG_INT		2	
#define POPT_ARG_LONG		3	
#define POPT_ARG_INCLUDE_TABLE	4	
#define POPT_ARG_CALLBACK	5	
#define POPT_ARG_INTL_DOMAIN    6       
#define POPT_ARG_VAL		7	
#define	POPT_ARG_FLOAT		8	
#define	POPT_ARG_DOUBLE		9	

#define POPT_ARG_MASK		0x0000FFFF

#define POPT_ARGFLAG_ONEDASH	0x80000000  
#define POPT_ARGFLAG_DOC_HIDDEN 0x40000000  
#define POPT_ARGFLAG_STRIP	0x20000000  
#define	POPT_ARGFLAG_OPTIONAL	0x10000000  

#define	POPT_ARGFLAG_OR		0x08000000  
#define	POPT_ARGFLAG_NOR	0x09000000  
#define	POPT_ARGFLAG_AND	0x04000000  
#define	POPT_ARGFLAG_NAND	0x05000000  
#define	POPT_ARGFLAG_XOR	0x02000000  
#define	POPT_ARGFLAG_NOT	0x01000000  
#define POPT_ARGFLAG_LOGICALOPS \
        (POPT_ARGFLAG_OR|POPT_ARGFLAG_AND|POPT_ARGFLAG_XOR)

#define	POPT_BIT_SET	(POPT_ARG_VAL|POPT_ARGFLAG_OR)
					
#define	POPT_BIT_CLR	(POPT_ARG_VAL|POPT_ARGFLAG_NAND)
					

#define	POPT_ARGFLAG_SHOW_DEFAULT 0x00800000 


#define POPT_CBFLAG_PRE		0x80000000  
#define POPT_CBFLAG_POST	0x40000000  
#define POPT_CBFLAG_INC_DATA	0x20000000  
#define POPT_CBFLAG_SKIPOPTION	0x10000000  
#define POPT_CBFLAG_CONTINUE	0x08000000  

#define POPT_ERROR_NOARG	-10	
#define POPT_ERROR_BADOPT	-11	
#define POPT_ERROR_OPTSTOODEEP	-13	
#define POPT_ERROR_BADQUOTE	-15	
#define POPT_ERROR_ERRNO	-16	
#define POPT_ERROR_BADNUMBER	-17	
#define POPT_ERROR_OVERFLOW	-18	
#define	POPT_ERROR_BADOPERATION	-19	
#define	POPT_ERROR_NULLARG	-20	
#define	POPT_ERROR_MALLOC	-21	

#define POPT_BADOPTION_NOALIAS  (1 << 0)  

#define POPT_CONTEXT_NO_EXEC	(1 << 0)  
#define POPT_CONTEXT_KEEP_FIRST	(1 << 1)  
#define POPT_CONTEXT_POSIXMEHARDER (1 << 2) 
#define POPT_CONTEXT_ARG_OPTS	(1 << 4) 

struct poptOption {
 
    const char * longName;	
    char shortName;		
    int argInfo;
 
    void * arg;			
    int val;			
 
    const char * descrip;	
 
    const char * argDescrip;	
};

struct poptAlias {
 
    const char * longName;	
    char shortName;		
    int argc;
    const char ** argv;		
};

typedef struct poptItem_s {
    struct poptOption option;	
    int argc;			
    const char ** argv;		
} * poptItem;


 
extern struct poptOption poptAliasOptions[];
#define POPT_AUTOALIAS { NULL, '\0', POPT_ARG_INCLUDE_TABLE, poptAliasOptions, \
			0, "Options implemented via popt alias/exec:", NULL },

 
extern struct poptOption poptHelpOptions[];

 
extern struct poptOption * poptHelpOptionsI18N;

#define POPT_AUTOHELP { NULL, '\0', POPT_ARG_INCLUDE_TABLE, poptHelpOptions, \
			0, "Help options:", NULL },

#define POPT_TABLEEND { NULL, '\0', 0, 0, 0, NULL, NULL }

typedef  struct poptContext_s * poptContext;

#ifndef __cplusplus
typedef struct poptOption * poptOption;
#endif

enum poptCallbackReason {
    POPT_CALLBACK_REASON_PRE	= 0, 
    POPT_CALLBACK_REASON_POST	= 1,
    POPT_CALLBACK_REASON_OPTION = 2
};

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*poptCallbackType) (poptContext con, 
		enum poptCallbackReason reason,
		 const struct poptOption * opt,
		 const char * arg,
		 const void * data)
	
	;

 
poptContext poptGetContext(
		  const char * name,
		int argc,   const char ** argv,
		  const struct poptOption * options,
		int flags)
	;

void poptResetContext(poptContext con)
	;

int poptGetNextOpt(poptContext con)
	
	;

  
const char * poptGetOptArg(poptContext con)
	;

  
const char * poptGetArg(poptContext con)
	;

  
const char * poptPeekArg(poptContext con)
	;

 
const char ** poptGetArgs(poptContext con)
	;

const char * poptBadOption(poptContext con, int flags)
	;

poptContext poptFreeContext(   poptContext con)
	;

int poptStuffArgs(poptContext con,  const char ** argv)
	;

int poptAddAlias(poptContext con, struct poptAlias alias, int flags)
	;

int poptAddItem(poptContext con, poptItem newItem, int flags)
	;

int poptReadConfigFile(poptContext con, const char * fn)
	
;

int poptReadDefaultConfig(poptContext con,  int useEnv)
	
;

int poptDupArgv(int argc,  const char **argv,
		  int * argcPtr,
		  const char *** argvPtr)
	;

int poptParseArgvString(const char * s,
		 int * argcPtr,  const char *** argvPtr)
	;

int poptConfigFileToString(FILE *fp,  char ** argstrp, int flags)
	
	;

const char * poptStrerror(const int error)
	;

void poptSetExecPath(poptContext con, const char * path, int allowAbsolute)
	;

void poptPrintHelp(poptContext con, FILE * fp,  int flags)
	
	;

void poptPrintUsage(poptContext con, FILE * fp,  int flags)
	
	;

void poptSetOtherOptionHelp(poptContext con, const char * text)
	;

const char * poptGetInvocationName(poptContext con)
	;

int poptStrippedArgv(poptContext con, int argc, char ** argv)
	;

int poptSaveLong( long * arg, int argInfo, long aLong)
	
	;

int poptSaveInt( int * arg, int argInfo, long aLong)
	
	;

#ifdef  __cplusplus
}
#endif

#endif
