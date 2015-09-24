

#undef	MYDEBUG

#include "system.h"

#if HAVE_FLOAT_H
#include <float.h>
#endif
#include <math.h>

#include "findme.h"
#include "poptint.h"

#ifdef	MYDEBUG
int _popt_debug = 0;
#endif

#if !defined(HAVE_STRERROR) && !defined(__LCLINT__)
static char * strerror(int errno)
{
    extern int sys_nerr;
    extern char * sys_errlist[];

    if ((0 <= errno) && (errno < sys_nerr))
	return sys_errlist[errno];
    else
	return POPT_("unknown errno");
}
#endif

#ifdef MYDEBUG
static void prtcon(const char *msg, poptContext con)
{
    if (msg) fprintf(stderr, "%s", msg);
    fprintf(stderr, "\tcon %p os %p nextCharArg \"%s\" nextArg \"%s\" argv[%d] \"%s\"\n",
	con, con->os,
	(con->os->nextCharArg ? con->os->nextCharArg : ""),
	(con->os->nextArg ? con->os->nextArg : ""),
	con->os->next,
	(con->os->argv && con->os->argv[con->os->next]
		? con->os->argv[con->os->next] : ""));
}
#endif

void poptSetExecPath(poptContext con, const char * path, int allowAbsolute)
{
    con->execPath = _free(con->execPath);
    con->execPath = xstrdup(path);
    con->execAbsolute = allowAbsolute;
     
    return;
    
}

static void invokeCallbacksPRE(poptContext con, const struct poptOption * opt)
	
	
{
    if (opt != NULL)
    for (; opt->longName || opt->shortName || opt->arg; opt++) {
	if (opt->arg == NULL) continue;		
	if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_INCLUDE_TABLE) {
	    void * arg = opt->arg;
	    
	    if (arg == poptHelpOptions) arg = poptHelpOptionsI18N;
	    
	    invokeCallbacksPRE(con, arg);
	} else if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_CALLBACK &&
		   (opt->argInfo & POPT_CBFLAG_PRE))
	{   
	    poptCallbackType cb = (poptCallbackType)opt->arg;
	    
	    
	    
	    cb(con, POPT_CALLBACK_REASON_PRE, NULL, NULL, opt->descrip);
	    
	}
    }
}

static void invokeCallbacksPOST(poptContext con, const struct poptOption * opt)
	
	
{
    if (opt != NULL)
    for (; opt->longName || opt->shortName || opt->arg; opt++) {
	if (opt->arg == NULL) continue;		
	if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_INCLUDE_TABLE) {
	    void * arg = opt->arg;
	    
	    if (arg == poptHelpOptions) arg = poptHelpOptionsI18N;
	    
	    invokeCallbacksPOST(con, arg);
	} else if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_CALLBACK &&
		   (opt->argInfo & POPT_CBFLAG_POST))
	{   
	    poptCallbackType cb = (poptCallbackType)opt->arg;
	    
	    
	    
	    cb(con, POPT_CALLBACK_REASON_POST, NULL, NULL, opt->descrip);
	    
	}
    }
}

static void invokeCallbacksOPTION(poptContext con,
				  const struct poptOption * opt,
				  const struct poptOption * myOpt,
				   const void * myData, int shorty)
	
	
{
    const struct poptOption * cbopt = NULL;

    if (opt != NULL)
    for (; opt->longName || opt->shortName || opt->arg; opt++) {
	if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_INCLUDE_TABLE) {
	    void * arg = opt->arg;
	    
	    if (arg == poptHelpOptions) arg = poptHelpOptionsI18N;
	    
	    if (opt->arg != NULL)	
		invokeCallbacksOPTION(con, opt->arg, myOpt, myData, shorty);
	} else if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_CALLBACK &&
		  !(opt->argInfo & POPT_CBFLAG_SKIPOPTION)) {
	    
	    cbopt = opt;
	} else if (cbopt != NULL &&
		   ((myOpt->shortName && opt->shortName && shorty &&
			myOpt->shortName == opt->shortName) ||
		    (myOpt->longName && opt->longName &&
				
			!strcmp(myOpt->longName, opt->longName)))
		
		   )
	{   
	    poptCallbackType cb = (poptCallbackType)cbopt->arg;
	    
	    const void * cbData = (cbopt->descrip ? cbopt->descrip : myData);
	    
	    if (cb != NULL) {	
		
		cb(con, POPT_CALLBACK_REASON_OPTION, myOpt,
			con->os->nextArg, cbData);
		
	    }
	    
	    if (!(cbopt->argInfo & POPT_CBFLAG_CONTINUE))
		return;
	}
    }
}

poptContext poptGetContext(const char * name, int argc, const char ** argv,
			   const struct poptOption * options, int flags)
{
    poptContext con = malloc(sizeof(*con));

    if (con == NULL) return NULL;	
    memset(con, 0, sizeof(*con));

    con->os = con->optionStack;
    con->os->argc = argc;
    	
    con->os->argv = argv;
    
    con->os->argb = NULL;

    if (!(flags & POPT_CONTEXT_KEEP_FIRST))
	con->os->next = 1;			

    con->leftovers = calloc( (argc + 1), sizeof(*con->leftovers) );
    	
    con->options = options;
    
    con->aliases = NULL;
    con->numAliases = 0;
    con->flags = flags;
    con->execs = NULL;
    con->numExecs = 0;
    con->finalArgvAlloced = argc * 2;
    con->finalArgv = calloc( con->finalArgvAlloced, sizeof(*con->finalArgv) );
    con->execAbsolute = 1;
    con->arg_strip = NULL;

    if (getenv("POSIXLY_CORRECT") || getenv("POSIX_ME_HARDER"))
	con->flags |= POPT_CONTEXT_POSIXMEHARDER;

    if (name) {
	char * t = malloc(strlen(name) + 1);
	if (t) con->appName = strcpy(t, name);
    }

    
    invokeCallbacksPRE(con, con->options);
    

    return con;
}

static void cleanOSE( struct optionStackEntry *os)
	
	
	
{
    os->nextArg = _free(os->nextArg);
    os->argv = _free(os->argv);
    os->argb = PBM_FREE(os->argb);
}

void poptResetContext(poptContext con)
{
    int i;

    if (con == NULL) return;
    while (con->os > con->optionStack) {
	cleanOSE(con->os--);
    }
    con->os->argb = PBM_FREE(con->os->argb);
    con->os->currAlias = NULL;
    con->os->nextCharArg = NULL;
    con->os->nextArg = NULL;
    con->os->next = 1;			

    con->numLeftovers = 0;
    con->nextLeftover = 0;
    con->restLeftover = 0;
    con->doExec = NULL;

    if (con->finalArgv != NULL)
    for (i = 0; i < con->finalArgvCount; i++) {
			
	con->finalArgv[i] = _free(con->finalArgv[i]);
	
    }

    con->finalArgvCount = 0;
    con->arg_strip = PBM_FREE(con->arg_strip);
    	
    return;
    
}

static int handleExec( poptContext con,
		 const char * longName, char shortName)
	
{
    poptItem item;
    int i;

    if (con->execs == NULL || con->numExecs <= 0) 
	return 0;

    for (i = con->numExecs - 1; i >= 0; i--) {
	item = con->execs + i;
	if (longName && !(item->option.longName &&
			!strcmp(longName, item->option.longName)))
	    continue;
	else if (shortName != item->option.shortName)
	    continue;
	break;
    }
    if (i < 0) return 0;


    if (con->flags & POPT_CONTEXT_NO_EXEC)
	return 1;

    if (con->doExec == NULL) {
	con->doExec = con->execs + i;
	return 1;
    }

    if ((con->finalArgvCount + 1) >= (con->finalArgvAlloced)) {
	con->finalArgvAlloced += 10;
	con->finalArgv = realloc(con->finalArgv,
			sizeof(*con->finalArgv) * con->finalArgvAlloced);
    }

    i = con->finalArgvCount++;
    if (con->finalArgv != NULL)	
    {	char *s  = malloc((longName ? strlen(longName) : 0) + 3);
	if (s != NULL) {	
	    if (longName)
		sprintf(s, "--%s", longName);
	    else
		sprintf(s, "-%c", shortName);
	    con->finalArgv[i] = s;
	} else
	    con->finalArgv[i] = NULL;
    }

    	
    return 1;
    
}

static int handleAlias( poptContext con,
		 const char * longName, char shortName,
		  const char * nextCharArg)
	
{
    poptItem item = con->os->currAlias;
    int rc;
    int i;

    if (item) {
	if (longName && (item->option.longName &&
		!strcmp(longName, item->option.longName)))
	    return 0;
	if (shortName && shortName == item->option.shortName)
	    return 0;
    }

    if (con->aliases == NULL || con->numAliases <= 0) 
	return 0;

    for (i = con->numAliases - 1; i >= 0; i--) {
	item = con->aliases + i;
	if (longName && !(item->option.longName &&
			!strcmp(longName, item->option.longName)))
	    continue;
	else if (shortName != item->option.shortName)
	    continue;
	break;
    }
    if (i < 0) return 0;

    if ((con->os - con->optionStack + 1) == POPT_OPTION_DEPTH)
	return POPT_ERROR_OPTSTOODEEP;

    if (nextCharArg && *nextCharArg)
	con->os->nextCharArg = nextCharArg;

    con->os++;
    con->os->next = 0;
    con->os->stuffed = 0;
    con->os->nextArg = NULL;
    con->os->nextCharArg = NULL;
    con->os->currAlias = con->aliases + i;
    rc = poptDupArgv(con->os->currAlias->argc, con->os->currAlias->argv,
		&con->os->argc, &con->os->argv);
    con->os->argb = NULL;

    return (rc ? rc : 1);
}

static int execCommand(poptContext con)
	
	
{
    poptItem item = con->doExec;
    const char ** argv;
    int argc = 0;
    int rc;

    if (item == NULL) 
	return POPT_ERROR_NOARG;

    if (item->argv == NULL || item->argc < 1 ||
	(!con->execAbsolute && strchr(item->argv[0], '/')))
	    return POPT_ERROR_NOARG;

    argv = malloc(sizeof(*argv) *
			(6 + item->argc + con->numLeftovers + con->finalArgvCount));
    if (argv == NULL) return POPT_ERROR_MALLOC;

    if (!strchr(item->argv[0], '/') && con->execPath != NULL) {
	char *s = alloca(strlen(con->execPath) + strlen(item->argv[0]) + sizeof("/"));
	sprintf(s, "%s/%s", con->execPath, item->argv[0]);
	argv[argc] = s;
    } else
	argv[argc] = findProgramPath(item->argv[0]);
    if (argv[argc++] == NULL) return POPT_ERROR_NOARG;

    if (item->argc > 1) {
	memcpy(argv + argc, item->argv + 1, sizeof(*argv) * (item->argc - 1));
	argc += (item->argc - 1);
    }

    if (con->finalArgv != NULL && con->finalArgvCount > 0) {
	memcpy(argv + argc, con->finalArgv,
		sizeof(*argv) * con->finalArgvCount);
	argc += con->finalArgvCount;
    }

    if (con->leftovers != NULL && con->numLeftovers > 0) {
	memcpy(argv + argc, con->leftovers, sizeof(*argv) * con->numLeftovers);
	argc += con->numLeftovers;
    }

    argv[argc] = NULL;

#if defined(hpux) || defined(__hpux)
    rc = setresgid(getgid(), getgid(),-1);
    if (rc) return POPT_ERROR_ERRNO;
    rc = setresuid(getuid(), getuid(),-1);
    if (rc) return POPT_ERROR_ERRNO;
#else
#if defined(HAVE_SETUID)
    rc = setgid(getgid());
    if (rc) return POPT_ERROR_ERRNO;
    rc = setuid(getuid());
    if (rc) return POPT_ERROR_ERRNO;
#elif defined (HAVE_SETREUID)
    rc = setregid(getgid(), getgid());
    if (rc) return POPT_ERROR_ERRNO;
    rc = setreuid(getuid(), getuid());
    if (rc) return POPT_ERROR_ERRNO;
#else
    ; 
#endif
#endif

    if (argv[0] == NULL)
	return POPT_ERROR_NOARG;

#ifdef	MYDEBUG
if (_popt_debug)
    {	const char ** avp;
	fprintf(stderr, "==> execvp(%s) argv[%d]:", argv[0], argc);
	for (avp = argv; *avp; avp++)
	    fprintf(stderr, " '%s'", *avp);
	fprintf(stderr, "\n");
    }
#endif

    rc = execvp(argv[0], (char *const *)argv);

    return POPT_ERROR_ERRNO;
}

  static const struct poptOption *
findOption(const struct poptOption * opt,  const char * longName,
		char shortName,
		  poptCallbackType * callback,
		  const void ** callbackData,
		int singleDash)
	
{
    const struct poptOption * cb = NULL;

    
    if (singleDash && !shortName && (longName && *longName == '\0'))
	shortName = '-';

    for (; opt->longName || opt->shortName || opt->arg; opt++) {

	if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_INCLUDE_TABLE) {
	    const struct poptOption * opt2;
	    void * arg = opt->arg;

	    
	    if (arg == poptHelpOptions) arg = poptHelpOptionsI18N;
	    
	    if (arg == NULL) continue;	
	    opt2 = findOption(arg, longName, shortName, callback,
			      callbackData, singleDash);
	    if (opt2 == NULL) continue;
	    
	    if (!(callback && *callback)) return opt2;
	    if (!(callbackData && *callbackData == NULL)) return opt2;
	    
	    *callbackData = opt->descrip;
	    
	    return opt2;
	} else if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_CALLBACK) {
	    cb = opt;
	} else if (longName && opt->longName &&
		   (!singleDash || (opt->argInfo & POPT_ARGFLAG_ONEDASH)) &&
				
		   !strcmp(longName, opt->longName))
		
	{
	    break;
	} else if (shortName && shortName == opt->shortName) {
	    break;
	}
    }

    if (!opt->longName && !opt->shortName)
	return NULL;
    
    if (callback) *callback = NULL;
    if (callbackData) *callbackData = NULL;
    if (cb) {
	if (callback)
	
	    *callback = (poptCallbackType)cb->arg;
	
	if (!(cb->argInfo & POPT_CBFLAG_INC_DATA)) {
	    if (callbackData)
			
		*callbackData = cb->descrip;
		
	}
    }
    

    return opt;
}

static const char * findNextArg( poptContext con,
		unsigned argx, int delete_arg)
	
{
    struct optionStackEntry * os = con->os;
    const char * arg;

    do {
	int i;
	arg = NULL;
	while (os->next == os->argc && os > con->optionStack) os--;
	if (os->next == os->argc && os == con->optionStack) break;
	if (os->argv != NULL)
	for (i = os->next; i < os->argc; i++) {
	    
	    if (os->argb && PBM_ISSET(i, os->argb))
		 continue;
	    if (*os->argv[i] == '-')
		 continue;
	    if (--argx > 0)
		 continue;
	    arg = os->argv[i];
	    if (delete_arg) {
		if (os->argb == NULL) os->argb = PBM_ALLOC(os->argc);
		if (os->argb != NULL)	
		PBM_SET(i, os->argb);
	    }
	     break;
	    
	}
	if (os > con->optionStack) os--;
    } while (arg == NULL);
    return arg;
}

static   const char *
expandNextArg( poptContext con, const char * s)
	
{
    const char * a = NULL;
    size_t alen;
    char *t, *te;
    size_t tn = strlen(s) + 1;
    char c;

    te = t = malloc(tn);;
    if (t == NULL) return NULL;		
    while ((c = *s++) != '\0') {
	switch (c) {
#if 0	
	case '\\':	
	    c = *s++;
	     break;
#endif
	case '!':
	    if (!(s[0] == '#' && s[1] == ':' && s[2] == '+'))
		 break;
	    
	    if (a == NULL) {
		if ((a = findNextArg(con, 1, 1)) == NULL)
		     break;
	    }
	    s += 3;

	    alen = strlen(a);
	    tn += alen;
	    *te = '\0';
	    t = realloc(t, tn);
	    te = t + strlen(t);
	    strncpy(te, a, alen); te += alen;
	    continue;
	      break;
	default:
	     break;
	}
	*te++ = c;
    }
    *te = '\0';
    t = realloc(t, strlen(t) + 1);	
    return t;
}

static void poptStripArg( poptContext con, int which)
	
	
	
{
    
    if (con->arg_strip == NULL)
	con->arg_strip = PBM_ALLOC(con->optionStack[0].argc);
    if (con->arg_strip != NULL)		
    PBM_SET(which, con->arg_strip);
    
     
    return;
    
}

int poptSaveLong(long * arg, int argInfo, long aLong)
{
    
    if (arg == NULL || (((unsigned long)arg) & (sizeof(*arg)-1)))
	return POPT_ERROR_NULLARG;

    if (argInfo & POPT_ARGFLAG_NOT)
	aLong = ~aLong;
    switch (argInfo & POPT_ARGFLAG_LOGICALOPS) {
    case 0:
	*arg = aLong;
	break;
    case POPT_ARGFLAG_OR:
	*arg |= aLong;
	break;
    case POPT_ARGFLAG_AND:
	*arg &= aLong;
	break;
    case POPT_ARGFLAG_XOR:
	*arg ^= aLong;
	break;
    default:
	return POPT_ERROR_BADOPERATION;
	 break;
    }
    return 0;
}

int poptSaveInt( int * arg, int argInfo, long aLong)
{
    
    if (arg == NULL || (((unsigned long)arg) & (sizeof(*arg)-1)))
	return POPT_ERROR_NULLARG;

    if (argInfo & POPT_ARGFLAG_NOT)
	aLong = ~aLong;
    switch (argInfo & POPT_ARGFLAG_LOGICALOPS) {
    case 0:
	*arg = aLong;
	break;
    case POPT_ARGFLAG_OR:
	*arg |= aLong;
	break;
    case POPT_ARGFLAG_AND:
	*arg &= aLong;
	break;
    case POPT_ARGFLAG_XOR:
	*arg ^= aLong;
	break;
    default:
	return POPT_ERROR_BADOPERATION;
	 break;
    }
    return 0;
}

int poptGetNextOpt(poptContext con)
{
    const struct poptOption * opt = NULL;
    int done = 0;

    if (con == NULL)
	return -1;
    while (!done) {
	const char * origOptString = NULL;
	poptCallbackType cb = NULL;
	const void * cbData = NULL;
	const char * longArg = NULL;
	int canstrip = 0;
	int shorty = 0;

	while (!con->os->nextCharArg && con->os->next == con->os->argc
		&& con->os > con->optionStack) {
	    cleanOSE(con->os--);
	}
	if (!con->os->nextCharArg && con->os->next == con->os->argc) {
	    
	    invokeCallbacksPOST(con, con->options);
	    
	    if (con->doExec) return execCommand(con);
	    return -1;
	}

	
	if (!con->os->nextCharArg) {
	    char * localOptString, * optString;
	    int thisopt;

	    
	    if (con->os->argb && PBM_ISSET(con->os->next, con->os->argb)) {
		con->os->next++;
		continue;
	    }
	    
	    thisopt = con->os->next;
	    if (con->os->argv != NULL)	
	    origOptString = con->os->argv[con->os->next++];

	    if (origOptString == NULL)	
		return POPT_ERROR_BADOPT;

	    if (con->restLeftover || *origOptString != '-' ||
		(*origOptString == '-' && origOptString[1] == '\0'))
	    {
		if (con->flags & POPT_CONTEXT_POSIXMEHARDER)
		    con->restLeftover = 1;
		if (con->flags & POPT_CONTEXT_ARG_OPTS) {
		    con->os->nextArg = xstrdup(origOptString);
		    return 0;
		}
		if (con->leftovers != NULL)	
		    con->leftovers[con->numLeftovers++] = origOptString;
		continue;
	    }

	    
	    localOptString = optString =
		strcpy(alloca(strlen(origOptString) + 1), origOptString);

	    if (optString[0] == '\0')
		return POPT_ERROR_BADOPT;

	    if (optString[1] == '-' && !optString[2]) {
		con->restLeftover = 1;
		continue;
	    } else {
		char *oe;
		int singleDash;

		optString++;
		if (*optString == '-')
		    singleDash = 0, optString++;
		else
		    singleDash = 1;

		
		if (handleAlias(con, optString, '\0', NULL))
		    continue;

		if (handleExec(con, optString, '\0'))
		    continue;

		
		for (oe = optString; *oe && *oe != '='; oe++)
		    {};
		if (*oe == '=') {
		    *oe++ = '\0';
		    
		    longArg = origOptString + (oe - localOptString);
		}

		opt = findOption(con->options, optString, '\0', &cb, &cbData,
				 singleDash);
		if (!opt && !singleDash)
		    return POPT_ERROR_BADOPT;
	    }

	    if (!opt) {
		con->os->nextCharArg = origOptString + 1;
	    } else {
		if (con->os == con->optionStack &&
		   opt->argInfo & POPT_ARGFLAG_STRIP)
		{
		    canstrip = 1;
		    poptStripArg(con, thisopt);
		}
		shorty = 0;
	    }
	}

	
			
	if (con->os->nextCharArg) {
	    origOptString = con->os->nextCharArg;

	    con->os->nextCharArg = NULL;

	    if (handleAlias(con, NULL, *origOptString, origOptString + 1))
		continue;

	    if (handleExec(con, NULL, *origOptString)) {
		
		origOptString++;
		if (*origOptString != '\0')
		    con->os->nextCharArg = origOptString;
		continue;
	    }

	    opt = findOption(con->options, NULL, *origOptString, &cb,
			     &cbData, 0);
	    if (!opt)
		return POPT_ERROR_BADOPT;
	    shorty = 1;

	    origOptString++;
	    if (*origOptString != '\0')
		con->os->nextCharArg = origOptString;
	}
	

	if (opt == NULL) return POPT_ERROR_BADOPT;	
	if (opt->arg && (opt->argInfo & POPT_ARG_MASK) == POPT_ARG_NONE) {
	    if (poptSaveInt((int *)opt->arg, opt->argInfo, 1L))
		return POPT_ERROR_BADOPERATION;
	} else if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_VAL) {
	    if (opt->arg) {
		if (poptSaveInt((int *)opt->arg, opt->argInfo, (long)opt->val))
		    return POPT_ERROR_BADOPERATION;
	    }
	} else if ((opt->argInfo & POPT_ARG_MASK) != POPT_ARG_NONE) {
	    con->os->nextArg = _free(con->os->nextArg);
	    	
	    if (longArg) {
	    
		longArg = expandNextArg(con, longArg);
		con->os->nextArg = longArg;
	    } else if (con->os->nextCharArg) {
		longArg = expandNextArg(con, con->os->nextCharArg);
		con->os->nextArg = longArg;
		con->os->nextCharArg = NULL;
	    } else {
		while (con->os->next == con->os->argc &&
		       con->os > con->optionStack) {
		    cleanOSE(con->os--);
		}
		if (con->os->next == con->os->argc) {
		    if (!(opt->argInfo & POPT_ARGFLAG_OPTIONAL))
				
			return POPT_ERROR_NOARG;
			
		    con->os->nextArg = NULL;
		} else {

		    if (con->os == con->optionStack &&
			(opt->argInfo & POPT_ARGFLAG_STRIP) &&
			canstrip) {
			poptStripArg(con, con->os->next);
		    }
		
		    if (con->os->argv != NULL) {	
			
			longArg = con->os->argv[con->os->next++];
			longArg = expandNextArg(con, longArg);
			con->os->nextArg = longArg;
		    }
		}
	    }
	    longArg = NULL;

	    if (opt->arg) {
		switch (opt->argInfo & POPT_ARG_MASK) {
		case POPT_ARG_STRING:
		    
		    *((const char **) opt->arg) = (con->os->nextArg)
			? xstrdup(con->os->nextArg) : NULL;
		     break;

		case POPT_ARG_INT:
		case POPT_ARG_LONG:
		{   long aLong = 0;
		    char *end;

		    if (con->os->nextArg) {
			aLong = strtol(con->os->nextArg, &end, 0);
			if (!(end && *end == '\0'))
			    return POPT_ERROR_BADNUMBER;
		    }

		    if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_LONG) {
			if (aLong == LONG_MIN || aLong == LONG_MAX)
			    return POPT_ERROR_OVERFLOW;
			if (poptSaveLong((long *)opt->arg, opt->argInfo, aLong))
			    return POPT_ERROR_BADOPERATION;
		    } else {
			if (aLong > INT_MAX || aLong < INT_MIN)
			    return POPT_ERROR_OVERFLOW;
			if (poptSaveInt((int *)opt->arg, opt->argInfo, aLong))
			    return POPT_ERROR_BADOPERATION;
		    }
		}    break;

		case POPT_ARG_FLOAT:
		case POPT_ARG_DOUBLE:
		{   double aDouble = 0.0;
		    char *end;

		    if (con->os->nextArg) {
			
			int saveerrno = errno;
			errno = 0;
			aDouble = strtod(con->os->nextArg, &end);
			if (errno == ERANGE)
			    return POPT_ERROR_OVERFLOW;
			errno = saveerrno;
			
			if (*end != '\0')
			    return POPT_ERROR_BADNUMBER;
		    }

		    if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_DOUBLE) {
			*((double *) opt->arg) = aDouble;
		    } else {
#define _ABS(a)	((((a) - 0.0) < DBL_EPSILON) ? -(a) : (a))
			if ((_ABS(aDouble) - FLT_MAX) > DBL_EPSILON)
			    return POPT_ERROR_OVERFLOW;
			if ((FLT_MIN - _ABS(aDouble)) > DBL_EPSILON)
			    return POPT_ERROR_OVERFLOW;
			*((float *) opt->arg) = aDouble;
		    }
		}    break;
		default:
		    fprintf(stdout,
			POPT_("option type (%d) not implemented in popt\n"),
			(opt->argInfo & POPT_ARG_MASK));
		    exit(EXIT_FAILURE);
		      break;
		}
	    }
	}

	if (cb) {
	    
	    invokeCallbacksOPTION(con, con->options, opt, cbData, shorty);
	    
	} else if (opt->val && ((opt->argInfo & POPT_ARG_MASK) != POPT_ARG_VAL))
	    done = 1;

	if ((con->finalArgvCount + 2) >= (con->finalArgvAlloced)) {
	    con->finalArgvAlloced += 10;
	    con->finalArgv = realloc(con->finalArgv,
			    sizeof(*con->finalArgv) * con->finalArgvAlloced);
	}

	if (con->finalArgv != NULL)
	{   char *s = malloc((opt->longName ? strlen(opt->longName) : 0) + 3);
	    if (s != NULL) {	
		if (opt->longName)
		    sprintf(s, "%s%s",
			((opt->argInfo & POPT_ARGFLAG_ONEDASH) ? "-" : "--"),
			opt->longName);
		else
		    sprintf(s, "-%c", opt->shortName);
		con->finalArgv[con->finalArgvCount++] = s;
	    } else
		con->finalArgv[con->finalArgvCount++] = NULL;
	}

	if (opt->arg && (opt->argInfo & POPT_ARG_MASK) == POPT_ARG_NONE)
	     ; 
	else if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_VAL)
	     ; 
	else if ((opt->argInfo & POPT_ARG_MASK) != POPT_ARG_NONE) {
	    if (con->finalArgv != NULL && con->os->nextArg)
	        con->finalArgv[con->finalArgvCount++] =
				
			xstrdup(con->os->nextArg);
			
	}
    }

    return (opt ? opt->val : -1);	
}

const char * poptGetOptArg(poptContext con)
{
    const char * ret = NULL;
    
    if (con) {
	ret = con->os->nextArg;
	con->os->nextArg = NULL;
    }
    
    return ret;
}

const char * poptGetArg(poptContext con)
{
    const char * ret = NULL;
    if (con && con->leftovers != NULL && con->nextLeftover < con->numLeftovers)
	ret = con->leftovers[con->nextLeftover++];
    return ret;
}

const char * poptPeekArg(poptContext con)
{
    const char * ret = NULL;
    if (con && con->leftovers != NULL && con->nextLeftover < con->numLeftovers)
	ret = con->leftovers[con->nextLeftover];
    return ret;
}

const char ** poptGetArgs(poptContext con)
{
    if (con == NULL ||
	con->leftovers == NULL || con->numLeftovers == con->nextLeftover)
	return NULL;

    
    con->leftovers[con->numLeftovers] = NULL;

    	
    return (con->leftovers + con->nextLeftover);
    
}

poptContext poptFreeContext(poptContext con)
{
    poptItem item;
    int i;

    if (con == NULL) return con;
    poptResetContext(con);
    con->os->argb = _free(con->os->argb);

    if (con->aliases != NULL)
    for (i = 0; i < con->numAliases; i++) {
	item = con->aliases + i;
	
	item->option.longName = _free(item->option.longName);
	item->option.descrip = _free(item->option.descrip);
	item->option.argDescrip = _free(item->option.argDescrip);
	
	item->argv = _free(item->argv);
    }
    con->aliases = _free(con->aliases);

    if (con->execs != NULL)
    for (i = 0; i < con->numExecs; i++) {
	item = con->execs + i;
	
	item->option.longName = _free(item->option.longName);
	item->option.descrip = _free(item->option.descrip);
	item->option.argDescrip = _free(item->option.argDescrip);
	
	item->argv = _free(item->argv);
    }
    con->execs = _free(con->execs);

    con->leftovers = _free(con->leftovers);
    con->finalArgv = _free(con->finalArgv);
    con->appName = _free(con->appName);
    con->otherHelp = _free(con->otherHelp);
    con->execPath = _free(con->execPath);
    con->arg_strip = PBM_FREE(con->arg_strip);
    
    con = _free(con);
    return con;
}

int poptAddAlias(poptContext con, struct poptAlias alias,
		 int flags)
{
    poptItem item = alloca(sizeof(*item));
    memset(item, 0, sizeof(*item));
    item->option.longName = alias.longName;
    item->option.shortName = alias.shortName;
    item->option.argInfo = POPT_ARGFLAG_DOC_HIDDEN;
    item->option.arg = 0;
    item->option.val = 0;
    item->option.descrip = NULL;
    item->option.argDescrip = NULL;
    item->argc = alias.argc;
    item->argv = alias.argv;
    return poptAddItem(con, item, 0);
}

 
int poptAddItem(poptContext con, poptItem newItem, int flags)
{
    poptItem * items, item;
    int * nitems;

    switch (flags) {
    case 1:
	items = &con->execs;
	nitems = &con->numExecs;
	break;
    case 0:
	items = &con->aliases;
	nitems = &con->numAliases;
	break;
    default:
	return 1;
	 break;
    }

    *items = realloc((*items), ((*nitems) + 1) * sizeof(**items));
    if ((*items) == NULL)
	return 1;

    item = (*items) + (*nitems);

    item->option.longName =
	(newItem->option.longName ? xstrdup(newItem->option.longName) : NULL);
    item->option.shortName = newItem->option.shortName;
    item->option.argInfo = newItem->option.argInfo;
    item->option.arg = newItem->option.arg;
    item->option.val = newItem->option.val;
    item->option.descrip =
	(newItem->option.descrip ? xstrdup(newItem->option.descrip) : NULL);
    item->option.argDescrip =
       (newItem->option.argDescrip ? xstrdup(newItem->option.argDescrip) : NULL);
    item->argc = newItem->argc;
    item->argv = newItem->argv;

    (*nitems)++;

    return 0;
}

const char * poptBadOption(poptContext con, int flags)
{
    struct optionStackEntry * os = NULL;

    if (con != NULL)
	os = (flags & POPT_BADOPTION_NOALIAS) ? con->optionStack : con->os;

    	
    return (os && os->argv ? os->argv[os->next - 1] : NULL);
    
}

const char * poptStrerror(const int error)
{
    switch (error) {
      case POPT_ERROR_NOARG:
	return POPT_("missing argument");
      case POPT_ERROR_BADOPT:
	return POPT_("unknown option");
      case POPT_ERROR_BADOPERATION:
	return POPT_("mutually exclusive logical operations requested");
      case POPT_ERROR_NULLARG:
	return POPT_("opt->arg should not be NULL");
      case POPT_ERROR_OPTSTOODEEP:
	return POPT_("aliases nested too deeply");
      case POPT_ERROR_BADQUOTE:
	return POPT_("error in parameter quoting");
      case POPT_ERROR_BADNUMBER:
	return POPT_("invalid numeric value");
      case POPT_ERROR_OVERFLOW:
	return POPT_("number too large or too small");
      case POPT_ERROR_MALLOC:
	return POPT_("memory allocation failed");
      case POPT_ERROR_ERRNO:
	return strerror(errno);
      default:
	return POPT_("unknown error");
    }
}

int poptStuffArgs(poptContext con, const char ** argv)
{
    int argc;
    int rc;

    if ((con->os - con->optionStack) == POPT_OPTION_DEPTH)
	return POPT_ERROR_OPTSTOODEEP;

    for (argc = 0; argv[argc]; argc++)
	{};

    con->os++;
    con->os->next = 0;
    con->os->nextArg = NULL;
    con->os->nextCharArg = NULL;
    con->os->currAlias = NULL;
    rc = poptDupArgv(argc, argv, &con->os->argc, &con->os->argv);
    con->os->argb = NULL;
    con->os->stuffed = 1;

    return rc;
}

const char * poptGetInvocationName(poptContext con)
{
    return (con->os->argv ? con->os->argv[0] : "");
}

int poptStrippedArgv(poptContext con, int argc, char ** argv)
{
    int numargs = argc;
    int j = 1;
    int i;
    
    
    if (con->arg_strip)
    for (i = 1; i < argc; i++) {
	if (PBM_ISSET(i, con->arg_strip))
	    numargs--;
    }
    
    for (i = 1; i < argc; i++) {
	if (con->arg_strip && PBM_ISSET(i, con->arg_strip))
	    continue;
	argv[j] = (j < numargs) ? argv[i] : NULL;
	j++;
    }
    
    
    return numargs;
}
