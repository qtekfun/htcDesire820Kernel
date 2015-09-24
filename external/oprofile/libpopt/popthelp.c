


#include "system.h"

#ifdef 	POPT_WCHAR_HACK
#include <wchar.h>			
#endif
#include "poptint.h"


static void displayArgs(poptContext con,
		 enum poptCallbackReason foo,
		struct poptOption * key, 
		 const char * arg,  void * data)
	
	
{
    if (key->shortName == '?')
	poptPrintHelp(con, stdout, 0);
    else
	poptPrintUsage(con, stdout, 0);
    exit(0);
}

#ifdef	NOTYET
static int show_option_defaults = 0;
#endif

 
struct poptOption poptAliasOptions[] = {
    POPT_TABLEEND
};

 
struct poptOption poptHelpOptions[] = {
  { NULL, '\0', POPT_ARG_CALLBACK, (void *)&displayArgs, '\0', NULL, NULL },
  { "help", '?', 0, NULL, '?', N_("Show this help message"), NULL },
  { "usage", '\0', 0, NULL, 'u', N_("Display brief usage message"), NULL },
    POPT_TABLEEND
} ;

 
static struct poptOption poptHelpOptions2[] = {
  { NULL, '\0', POPT_ARG_INTL_DOMAIN, PACKAGE, 0, NULL, NULL},
  { NULL, '\0', POPT_ARG_CALLBACK, (void *)&displayArgs, '\0', NULL, NULL },
  { "help", '?', 0, NULL, '?', N_("Show this help message"), NULL },
  { "usage", '\0', 0, NULL, 'u', N_("Display brief usage message"), NULL },
#ifdef	NOTYET
  { "defaults", '\0', POPT_ARG_NONE, &show_option_defaults, 0,
	N_("Display option defaults in message"), NULL },
#endif
    POPT_TABLEEND
} ;

 
struct poptOption * poptHelpOptionsI18N = poptHelpOptions2;

  static const char *
getTableTranslationDomain( const struct poptOption *table)
	
{
    const struct poptOption *opt;

    if (table != NULL)
    for (opt = table; opt->longName || opt->shortName || opt->arg; opt++) {
	if (opt->argInfo == POPT_ARG_INTL_DOMAIN)
	    return opt->arg;
    }
    return NULL;
}

  static const char *
getArgDescrip(const struct poptOption * opt,
		 
		 const char * translation_domain)
		
	
{
    if (!(opt->argInfo & POPT_ARG_MASK)) return NULL;

    if (opt == (poptHelpOptions + 1) || opt == (poptHelpOptions + 2))
	if (opt->argDescrip) return POPT_(opt->argDescrip);

    if (opt->argDescrip) return D_(translation_domain, opt->argDescrip);

    switch (opt->argInfo & POPT_ARG_MASK) {
    case POPT_ARG_NONE:		return POPT_("NONE");
#ifdef	DYING
    case POPT_ARG_VAL:		return POPT_("VAL");
#else
    case POPT_ARG_VAL:		return NULL;
#endif
    case POPT_ARG_INT:		return POPT_("INT");
    case POPT_ARG_LONG:		return POPT_("LONG");
    case POPT_ARG_STRING:	return POPT_("STRING");
    case POPT_ARG_FLOAT:	return POPT_("FLOAT");
    case POPT_ARG_DOUBLE:	return POPT_("DOUBLE");
    default:			return POPT_("ARG");
    }
}

static   char *
singleOptionDefaultValue(size_t lineLength,
		const struct poptOption * opt,
		 
		 const char * translation_domain)
		
	
{
    const char * defstr = D_(translation_domain, "default");
    char * le = malloc(4*lineLength + 1);
    char * l = le;

    if (le == NULL) return NULL;	
    *le = '\0';
    *le++ = '(';
    strcpy(le, defstr);	le += strlen(le);
    *le++ = ':';
    *le++ = ' ';
    if (opt->arg)	
    switch (opt->argInfo & POPT_ARG_MASK) {
    case POPT_ARG_VAL:
    case POPT_ARG_INT:
    {	long aLong = *((int *)opt->arg);
	le += sprintf(le, "%ld", aLong);
    }	break;
    case POPT_ARG_LONG:
    {	long aLong = *((long *)opt->arg);
	le += sprintf(le, "%ld", aLong);
    }	break;
    case POPT_ARG_FLOAT:
    {	double aDouble = *((float *)opt->arg);
	le += sprintf(le, "%g", aDouble);
    }	break;
    case POPT_ARG_DOUBLE:
    {	double aDouble = *((double *)opt->arg);
	le += sprintf(le, "%g", aDouble);
    }	break;
    case POPT_ARG_STRING:
    {	const char * s = *(const char **)opt->arg;
	if (s == NULL) {
	    strcpy(le, "null");	le += strlen(le);
	} else {
	    size_t slen = 4*lineLength - (le - l) - sizeof("\"...\")");
	    *le++ = '"';
	    strncpy(le, s, slen); le[slen] = '\0'; le += strlen(le);	
	    if (slen < strlen(s)) {
		strcpy(le, "...");	le += strlen(le);
	    }
	    *le++ = '"';
	}
    }	break;
    case POPT_ARG_NONE:
    default:
	l = _free(l);
	return NULL;
	 break;
    }
    *le++ = ')';
    *le = '\0';

    return l;
}

static void singleOptionHelp(FILE * fp, size_t maxLeftCol, 
		const struct poptOption * opt,
		 const char * translation_domain)
	
	
{
    size_t indentLength = maxLeftCol + 5;
    size_t lineLength = 79 - indentLength;
    const char * help = D_(translation_domain, opt->descrip);
    const char * argDescrip = getArgDescrip(opt, translation_domain);
    size_t helpLength;
    char * defs = NULL;
    char * left;
    size_t nb = maxLeftCol + 1;
    int displaypad = 0;

    
    if (opt->longName)	nb += strlen(opt->longName);
    if (argDescrip)	nb += strlen(argDescrip);

    left = malloc(nb);
    if (left == NULL) return;	
    left[0] = '\0';
    left[maxLeftCol] = '\0';

    if (opt->longName && opt->shortName)
	sprintf(left, "-%c, %s%s", opt->shortName,
		((opt->argInfo & POPT_ARGFLAG_ONEDASH) ? "-" : "--"),
		opt->longName);
    else if (opt->shortName != '\0') 
	sprintf(left, "-%c", opt->shortName);
    else if (opt->longName)
	sprintf(left, "%s%s",
		((opt->argInfo & POPT_ARGFLAG_ONEDASH) ? "-" : "--"),
		opt->longName);
    if (!*left) goto out;

    if (argDescrip) {
	char * le = left + strlen(left);

	if (opt->argInfo & POPT_ARGFLAG_OPTIONAL)
	    *le++ = '[';

	
	
	if (opt->argInfo & POPT_ARGFLAG_SHOW_DEFAULT) {
	    defs = singleOptionDefaultValue(lineLength, opt, translation_domain);
	    if (defs) {
		char * t = malloc((help ? strlen(help) : 0) +
				strlen(defs) + sizeof(" "));
		if (t) {
		    char * te = t;
		    *te = '\0';
		    if (help) {
			strcpy(te, help);	te += strlen(te);
		    }
		    *te++ = ' ';
		    strcpy(te, defs);
		    defs = _free(defs);
		}
		defs = t;
	    }
	}
	

	if (opt->argDescrip == NULL) {
	    switch (opt->argInfo & POPT_ARG_MASK) {
	    case POPT_ARG_NONE:
		break;
	    case POPT_ARG_VAL:
#ifdef	NOTNOW	
	    {	long aLong = opt->val;
		int ops = (opt->argInfo & POPT_ARGFLAG_LOGICALOPS);
		int negate = (opt->argInfo & POPT_ARGFLAG_NOT);

		
		if (!ops && (aLong == 0L || aLong == 1L || aLong == -1L))
		    break;
		*le++ = '[';
		switch (ops) {
		case POPT_ARGFLAG_OR:
		    *le++ = '|';
		     break;
		case POPT_ARGFLAG_AND:
		    *le++ = '&';
		     break;
		case POPT_ARGFLAG_XOR:
		    *le++ = '^';
		     break;
		default:
		     break;
		}
		*le++ = (opt->longName != NULL ? '=' : ' ');
		if (negate) *le++ = '~';
		
		le += sprintf(le, (ops ? "0x%lx" : "%ld"), aLong);
		
		*le++ = ']';
	    }
#endif
		break;
	    case POPT_ARG_INT:
	    case POPT_ARG_LONG:
	    case POPT_ARG_FLOAT:
	    case POPT_ARG_DOUBLE:
	    case POPT_ARG_STRING:
		*le++ = (opt->longName != NULL ? '=' : ' ');
		strcpy(le, argDescrip);		le += strlen(le);
		break;
	    default:
		break;
	    }
	} else {
	    size_t lelen;

	    *le++ = '=';
	    strcpy(le, argDescrip);
	    lelen = strlen(le);
	    le += lelen;

#ifdef	POPT_WCHAR_HACK
	    {	const char * scopy = argDescrip;
		mbstate_t t;
		size_t n;

		memset ((void *)&t, '\0', sizeof (t));	
		
		n = mbsrtowcs (NULL, &scopy, strlen(scopy), &t);

		displaypad = (int) (lelen-n);
	    }
#endif
	}
	if (opt->argInfo & POPT_ARGFLAG_OPTIONAL)
	    *le++ = ']';
	*le = '\0';
    }

    if (help)
	fprintf(fp,"  %-*s   ", maxLeftCol+displaypad, left);
    else {
	fprintf(fp,"  %s\n", left); 
	goto out;
    }

    left = _free(left);
    if (defs) {
	help = defs;
	defs = NULL;
    }

    helpLength = strlen(help);
    while (helpLength > lineLength) {
	const char * ch;
	char format[16];

	ch = help + lineLength - 1;
	while (ch > help && !isspace(*ch)) ch--;
	if (ch == help) break;		
	while (ch > (help + 1) && isspace(*ch)) ch--;
	ch++;

	sprintf(format, "%%.%ds\n%%%ds", (int) (ch - help), (int) indentLength);
	
	fprintf(fp, format, help, " ");
	
	help = ch;
	while (isspace(*help) && *help) help++;
	helpLength = strlen(help);
    }

    if (helpLength) fprintf(fp, "%s\n", help);

out:
    
    defs = _free(defs);
    
    left = _free(left);
}

static size_t maxArgWidth(const struct poptOption * opt,
		        const char * translation_domain)
	
{
    size_t max = 0;
    size_t len = 0;
    const char * s;
    
    if (opt != NULL)
    while (opt->longName || opt->shortName || opt->arg) {
	if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_INCLUDE_TABLE) {
	    if (opt->arg)	
	    len = maxArgWidth(opt->arg, translation_domain);
	    if (len > max) max = len;
	} else if (!(opt->argInfo & POPT_ARGFLAG_DOC_HIDDEN)) {
	    len = sizeof("  ")-1;
	    if (opt->shortName != '\0') len += sizeof("-X")-1;
	    if (opt->shortName != '\0' && opt->longName) len += sizeof(", ")-1;
	    if (opt->longName) {
		len += ((opt->argInfo & POPT_ARGFLAG_ONEDASH)
			? sizeof("-")-1 : sizeof("--")-1);
		len += strlen(opt->longName);
	    }

	    s = getArgDescrip(opt, translation_domain);

#ifdef POPT_WCHAR_HACK
	    
	    if (s) {
		const char * scopy = s;
		mbstate_t t;
		size_t n;

		memset ((void *)&t, '\0', sizeof (t));	
		
		n = mbsrtowcs (NULL, &scopy, strlen(scopy), &t);
		len += sizeof("=")-1 + n;
	    }
#else
	    if (s)
		len += sizeof("=")-1 + strlen(s);
#endif

	    if (opt->argInfo & POPT_ARGFLAG_OPTIONAL) len += sizeof("[]")-1;
	    if (len > max) max = len;
	}

	opt++;
    }
    
    return max;
}

static void itemHelp(FILE * fp,
		 poptItem items, int nitems, size_t left,
		 const char * translation_domain)
	
	
{
    poptItem item;
    int i;

    if (items != NULL)
    for (i = 0, item = items; i < nitems; i++, item++) {
	const struct poptOption * opt;
	opt = &item->option;
	if ((opt->longName || opt->shortName) && 
	    !(opt->argInfo & POPT_ARGFLAG_DOC_HIDDEN))
	    singleOptionHelp(fp, left, opt, translation_domain);
    }
}

static void singleTableHelp(poptContext con, FILE * fp,
		 const struct poptOption * table, size_t left,
		 const char * translation_domain)
	
	
{
    const struct poptOption * opt;
    const char *sub_transdom;

    if (table == poptAliasOptions) {
	itemHelp(fp, con->aliases, con->numAliases, left, NULL);
	itemHelp(fp, con->execs, con->numExecs, left, NULL);
	return;
    }

    if (table != NULL)
    for (opt = table; (opt->longName || opt->shortName || opt->arg); opt++) {
	if ((opt->longName || opt->shortName) && 
	    !(opt->argInfo & POPT_ARGFLAG_DOC_HIDDEN))
	    singleOptionHelp(fp, left, opt, translation_domain);
    }

    if (table != NULL)
    for (opt = table; (opt->longName || opt->shortName || opt->arg); opt++) {
	if ((opt->argInfo & POPT_ARG_MASK) != POPT_ARG_INCLUDE_TABLE)
	    continue;
	sub_transdom = getTableTranslationDomain(opt->arg);
	if (sub_transdom == NULL)
	    sub_transdom = translation_domain;
	    
	if (opt->descrip)
	    fprintf(fp, "\n%s\n", D_(sub_transdom, opt->descrip));

	singleTableHelp(con, fp, opt->arg, left, sub_transdom);
    }
}

static int showHelpIntro(poptContext con, FILE * fp)
	
	
{
    int len = 6;
    const char * fn;

    fprintf(fp, POPT_("Usage:"));
    if (!(con->flags & POPT_CONTEXT_KEEP_FIRST)) {
		
	fn = con->optionStack->argv[0];
	
	if (fn == NULL) return len;
	if (strchr(fn, '/')) fn = strrchr(fn, '/') + 1;
	fprintf(fp, " %s", fn);
	len += strlen(fn) + 1;
    }

    return len;
}

void poptPrintHelp(poptContext con, FILE * fp,  int flags)
{
    size_t leftColWidth;

    (void) showHelpIntro(con, fp);
    if (con->otherHelp)
	fprintf(fp, " %s\n", con->otherHelp);
    else
	fprintf(fp, " %s\n", POPT_("[OPTION...]"));

    leftColWidth = maxArgWidth(con->options, NULL);
    singleTableHelp(con, fp, con->options, leftColWidth, NULL);
}

static size_t singleOptionUsage(FILE * fp, size_t cursor, 
		const struct poptOption * opt,
		 const char *translation_domain)
	
	
{
    size_t len = 4;
    char shortStr[2] = { '\0', '\0' };
    const char * item = shortStr;
    const char * argDescrip = getArgDescrip(opt, translation_domain);

    if (opt->shortName != '\0' && opt->longName != NULL) {
	len += 2;
	if (!(opt->argInfo & POPT_ARGFLAG_ONEDASH)) len++;
	len += strlen(opt->longName);
    } else if (opt->shortName != '\0') {
	len++;
	shortStr[0] = opt->shortName;
	shortStr[1] = '\0';
    } else if (opt->longName) {
	len += strlen(opt->longName);
	if (!(opt->argInfo & POPT_ARGFLAG_ONEDASH)) len++;
	item = opt->longName;
    }

    if (len == 4) return cursor;

#ifdef POPT_WCHAR_HACK
    
    if (argDescrip) {
	const char * scopy = argDescrip;
	mbstate_t t;
	size_t n;

	memset ((void *)&t, '\0', sizeof (t));	
	
	n = mbsrtowcs (NULL, &scopy, strlen(scopy), &t);
	len += sizeof("=")-1 + n;
    }
#else
    if (argDescrip) 
	len += sizeof("=")-1 + strlen(argDescrip);
#endif

    if ((cursor + len) > 79) {
	fprintf(fp, "\n       ");
	cursor = 7;
    } 

    if (opt->longName && opt->shortName) {
	fprintf(fp, " [-%c|-%s%s%s%s]",
	    opt->shortName, ((opt->argInfo & POPT_ARGFLAG_ONEDASH) ? "" : "-"),
	    opt->longName,
	    (argDescrip ? " " : ""),
	    (argDescrip ? argDescrip : ""));
    } else {
	fprintf(fp, " [-%s%s%s%s]",
	    ((opt->shortName || (opt->argInfo & POPT_ARGFLAG_ONEDASH)) ? "" : "-"),
	    item,
	    (argDescrip ? (opt->shortName != '\0' ? " " : "=") : ""),
	    (argDescrip ? argDescrip : ""));
    }

    return cursor + len + 1;
}

static size_t itemUsage(FILE * fp, size_t cursor,
		 poptItem item, int nitems,
		 const char * translation_domain)
	
	
{
    int i;

    		
    if (item != NULL)
    for (i = 0; i < nitems; i++, item++) {
	const struct poptOption * opt;
	opt = &item->option;
        if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_INTL_DOMAIN) {
	    translation_domain = (const char *)opt->arg;
	} else if ((opt->longName || opt->shortName) &&
		 !(opt->argInfo & POPT_ARGFLAG_DOC_HIDDEN)) {
	    cursor = singleOptionUsage(fp, cursor, opt, translation_domain);
	}
    }
    

    return cursor;
}

typedef struct poptDone_s {
    int nopts;
    int maxopts;
    const void ** opts;
} * poptDone;

static size_t singleTableUsage(poptContext con, FILE * fp, size_t cursor,
		 const struct poptOption * opt,
		 const char * translation_domain,
		 poptDone done)
	
	
{
    		
    if (opt != NULL)
    for (; (opt->longName || opt->shortName || opt->arg) ; opt++) {
        if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_INTL_DOMAIN) {
	    translation_domain = (const char *)opt->arg;
	} else if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_INCLUDE_TABLE) {
	    if (done) {
		int i = 0;
		for (i = 0; i < done->nopts; i++) {
		    const void * that = done->opts[i];
		    if (that == NULL || that != opt->arg)
			 continue;
		     break;
		}
		
		if (opt->arg == NULL || i < done->nopts)
		    continue;
		if (done->nopts < done->maxopts)
		    done->opts[done->nopts++] = (const void *) opt->arg;
	    }
	    cursor = singleTableUsage(con, fp, cursor, opt->arg,
			translation_domain, done);
	} else if ((opt->longName || opt->shortName) &&
		 !(opt->argInfo & POPT_ARGFLAG_DOC_HIDDEN)) {
	    cursor = singleOptionUsage(fp, cursor, opt, translation_domain);
	}
    }
    

    return cursor;
}

static int showShortOptions(const struct poptOption * opt, FILE * fp,
		 char * str)
	
	
	
{
    
    char * s = (str != NULL ? str : memset(alloca(300), 0, 300));
    int len = 0;

    if (opt != NULL)
    for (; (opt->longName || opt->shortName || opt->arg); opt++) {
	if (opt->shortName && !(opt->argInfo & POPT_ARG_MASK))
	    s[strlen(s)] = opt->shortName;
	else if ((opt->argInfo & POPT_ARG_MASK) == POPT_ARG_INCLUDE_TABLE)
	    if (opt->arg)	
		len = showShortOptions(opt->arg, fp, s);
    } 

    
    if (s == str && *s != '\0') {
	fprintf(fp, " [-%s]", s);
	len = strlen(s) + sizeof(" [-]")-1;
    }
    return len;
}

void poptPrintUsage(poptContext con, FILE * fp,  int flags)
{
    poptDone done = memset(alloca(sizeof(*done)), 0, sizeof(*done));
    size_t cursor;

    done->nopts = 0;
    done->maxopts = 64;
    cursor = done->maxopts * sizeof(*done->opts);
    done->opts = memset(alloca(cursor), 0, cursor);
    
    done->opts[done->nopts++] = (const void *) con->options;
    

    cursor = showHelpIntro(con, fp);
    cursor += showShortOptions(con->options, fp, NULL);
    cursor = singleTableUsage(con, fp, cursor, con->options, NULL, done);
    cursor = itemUsage(fp, cursor, con->aliases, con->numAliases, NULL);
    cursor = itemUsage(fp, cursor, con->execs, con->numExecs, NULL);

    if (con->otherHelp) {
	cursor += strlen(con->otherHelp) + 1;
	if (cursor > 79) fprintf(fp, "\n       ");
	fprintf(fp, " %s", con->otherHelp);
    }

    fprintf(fp, "\n");
}

void poptSetOtherOptionHelp(poptContext con, const char * text)
{
    con->otherHelp = _free(con->otherHelp);
    con->otherHelp = xstrdup(text);
}
