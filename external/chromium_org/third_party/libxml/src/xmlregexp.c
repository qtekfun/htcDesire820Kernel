/*
 * regexp.c: generic and extensible Regular Expression engine
 *
 * Basically designed with the purpose of compiling regexps for 
 * the variety of validation/shemas mechanisms now available in
 * XML related specifications these include:
 *    - XML-1.0 DTD validation
 *    - XML Schemas structure part 1
 *    - XML Schemas Datatypes part 2 especially Appendix F
 *    - RELAX-NG/TREX i.e. the counter proposal
 *
 * See Copyright for the status of this software.
 *
 * Daniel Veillard <veillard@redhat.com>
 */

#define IN_LIBXML
#include "libxml.h"

#ifdef LIBXML_REGEXP_ENABLED


#include <stdio.h>
#include <string.h>
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#include <libxml/tree.h>
#include <libxml/parserInternals.h>
#include <libxml/xmlregexp.h>
#include <libxml/xmlautomata.h>
#include <libxml/xmlunicode.h>

#ifndef INT_MAX
#define INT_MAX 123456789 
#endif


#define MAX_PUSH 10000000

#define ERROR(str)							\
    ctxt->error = XML_REGEXP_COMPILE_ERROR;				\
    xmlRegexpErrCompile(ctxt, str);
#define NEXT ctxt->cur++
#define CUR (*(ctxt->cur))
#define NXT(index) (ctxt->cur[index])

#define CUR_SCHAR(s, l) xmlStringCurrentChar(NULL, s, &l)
#define NEXTL(l) ctxt->cur += l;
#define XML_REG_STRING_SEPARATOR '|'
#define PREV (ctxt->cur[-1])

#define TODO 								\
    xmlGenericError(xmlGenericErrorContext,				\
	    "Unimplemented block at %s:%d\n",				\
            __FILE__, __LINE__);


typedef enum {
    XML_REGEXP_EPSILON = 1,
    XML_REGEXP_CHARVAL,
    XML_REGEXP_RANGES,
    XML_REGEXP_SUBREG,  
    XML_REGEXP_STRING,
    XML_REGEXP_ANYCHAR, 
    XML_REGEXP_ANYSPACE, 
    XML_REGEXP_NOTSPACE, 
    XML_REGEXP_INITNAME, 
    XML_REGEXP_NOTINITNAME, 
    XML_REGEXP_NAMECHAR, 
    XML_REGEXP_NOTNAMECHAR, 
    XML_REGEXP_DECIMAL, 
    XML_REGEXP_NOTDECIMAL, 
    XML_REGEXP_REALCHAR, 
    XML_REGEXP_NOTREALCHAR, 
    XML_REGEXP_LETTER = 100,
    XML_REGEXP_LETTER_UPPERCASE,
    XML_REGEXP_LETTER_LOWERCASE,
    XML_REGEXP_LETTER_TITLECASE,
    XML_REGEXP_LETTER_MODIFIER,
    XML_REGEXP_LETTER_OTHERS,
    XML_REGEXP_MARK,
    XML_REGEXP_MARK_NONSPACING,
    XML_REGEXP_MARK_SPACECOMBINING,
    XML_REGEXP_MARK_ENCLOSING,
    XML_REGEXP_NUMBER,
    XML_REGEXP_NUMBER_DECIMAL,
    XML_REGEXP_NUMBER_LETTER,
    XML_REGEXP_NUMBER_OTHERS,
    XML_REGEXP_PUNCT,
    XML_REGEXP_PUNCT_CONNECTOR,
    XML_REGEXP_PUNCT_DASH,
    XML_REGEXP_PUNCT_OPEN,
    XML_REGEXP_PUNCT_CLOSE,
    XML_REGEXP_PUNCT_INITQUOTE,
    XML_REGEXP_PUNCT_FINQUOTE,
    XML_REGEXP_PUNCT_OTHERS,
    XML_REGEXP_SEPAR,
    XML_REGEXP_SEPAR_SPACE,
    XML_REGEXP_SEPAR_LINE,
    XML_REGEXP_SEPAR_PARA,
    XML_REGEXP_SYMBOL,
    XML_REGEXP_SYMBOL_MATH,
    XML_REGEXP_SYMBOL_CURRENCY,
    XML_REGEXP_SYMBOL_MODIFIER,
    XML_REGEXP_SYMBOL_OTHERS,
    XML_REGEXP_OTHER,
    XML_REGEXP_OTHER_CONTROL,
    XML_REGEXP_OTHER_FORMAT,
    XML_REGEXP_OTHER_PRIVATE,
    XML_REGEXP_OTHER_NA,
    XML_REGEXP_BLOCK_NAME
} xmlRegAtomType;

typedef enum {
    XML_REGEXP_QUANT_EPSILON = 1,
    XML_REGEXP_QUANT_ONCE,
    XML_REGEXP_QUANT_OPT,
    XML_REGEXP_QUANT_MULT,
    XML_REGEXP_QUANT_PLUS,
    XML_REGEXP_QUANT_ONCEONLY,
    XML_REGEXP_QUANT_ALL,
    XML_REGEXP_QUANT_RANGE
} xmlRegQuantType;

typedef enum {
    XML_REGEXP_START_STATE = 1,
    XML_REGEXP_FINAL_STATE,
    XML_REGEXP_TRANS_STATE,
    XML_REGEXP_SINK_STATE,
    XML_REGEXP_UNREACH_STATE
} xmlRegStateType;

typedef enum {
    XML_REGEXP_MARK_NORMAL = 0,
    XML_REGEXP_MARK_START,
    XML_REGEXP_MARK_VISITED
} xmlRegMarkedType;

typedef struct _xmlRegRange xmlRegRange;
typedef xmlRegRange *xmlRegRangePtr;

struct _xmlRegRange {
    int neg;		
    xmlRegAtomType type;
    int start;
    int end;
    xmlChar *blockName;
};

typedef struct _xmlRegAtom xmlRegAtom;
typedef xmlRegAtom *xmlRegAtomPtr;

typedef struct _xmlAutomataState xmlRegState;
typedef xmlRegState *xmlRegStatePtr;

struct _xmlRegAtom {
    int no;
    xmlRegAtomType type;
    xmlRegQuantType quant;
    int min;
    int max;

    void *valuep;
    void *valuep2;
    int neg;
    int codepoint;
    xmlRegStatePtr start;
    xmlRegStatePtr start0;
    xmlRegStatePtr stop;
    int maxRanges;
    int nbRanges;
    xmlRegRangePtr *ranges;
    void *data;
};

typedef struct _xmlRegCounter xmlRegCounter;
typedef xmlRegCounter *xmlRegCounterPtr;

struct _xmlRegCounter {
    int min;
    int max;
};

typedef struct _xmlRegTrans xmlRegTrans;
typedef xmlRegTrans *xmlRegTransPtr;

struct _xmlRegTrans {
    xmlRegAtomPtr atom;
    int to;
    int counter;
    int count;
    int nd;
};

struct _xmlAutomataState {
    xmlRegStateType type;
    xmlRegMarkedType mark;
    xmlRegMarkedType reached;
    int no;
    int maxTrans;
    int nbTrans;
    xmlRegTrans *trans;
    
    int maxTransTo;
    int nbTransTo;
    int *transTo;
};

typedef struct _xmlAutomata xmlRegParserCtxt;
typedef xmlRegParserCtxt *xmlRegParserCtxtPtr;

#define AM_AUTOMATA_RNG 1

struct _xmlAutomata {
    xmlChar *string;
    xmlChar *cur;

    int error;
    int neg;

    xmlRegStatePtr start;
    xmlRegStatePtr end;
    xmlRegStatePtr state;

    xmlRegAtomPtr atom;

    int maxAtoms;
    int nbAtoms;
    xmlRegAtomPtr *atoms;

    int maxStates;
    int nbStates;
    xmlRegStatePtr *states;

    int maxCounters;
    int nbCounters;
    xmlRegCounter *counters;

    int determinist;
    int negs;
    int flags;
};

struct _xmlRegexp {
    xmlChar *string;
    int nbStates;
    xmlRegStatePtr *states;
    int nbAtoms;
    xmlRegAtomPtr *atoms;
    int nbCounters;
    xmlRegCounter *counters;
    int determinist;
    int flags;
    int nbstates;
    int *compact;
    void **transdata;
    int nbstrings;
    xmlChar **stringMap;
};

typedef struct _xmlRegExecRollback xmlRegExecRollback;
typedef xmlRegExecRollback *xmlRegExecRollbackPtr;

struct _xmlRegExecRollback {
    xmlRegStatePtr state;
    int index;		
    int nextbranch;	
    int *counts;	
};

typedef struct _xmlRegInputToken xmlRegInputToken;
typedef xmlRegInputToken *xmlRegInputTokenPtr;

struct _xmlRegInputToken {
    xmlChar *value;
    void *data;
};

struct _xmlRegExecCtxt {
    int status;		
    int determinist;	
    xmlRegexpPtr comp;	
    xmlRegExecCallbacks callback;
    void *data;

    xmlRegStatePtr state;
    int transno;	
    int transcount;	

    int maxRollbacks;
    int nbRollbacks;
    xmlRegExecRollback *rollbacks;

    int *counts;

    int inputStackMax;
    int inputStackNr;
    int index;
    int *charStack;
    const xmlChar *inputString; 
    xmlRegInputTokenPtr inputStack;

    int errStateNo;		
    xmlRegStatePtr errState;    
    xmlChar *errString;		
    int *errCounts;		
    int nbPush;
};

#define REGEXP_ALL_COUNTER	0x123456
#define REGEXP_ALL_LAX_COUNTER	0x123457

static void xmlFAParseRegExp(xmlRegParserCtxtPtr ctxt, int top);
static void xmlRegFreeState(xmlRegStatePtr state);
static void xmlRegFreeAtom(xmlRegAtomPtr atom);
static int xmlRegStrEqualWildcard(const xmlChar *expStr, const xmlChar *valStr);
static int xmlRegCheckCharacter(xmlRegAtomPtr atom, int codepoint);
static int xmlRegCheckCharacterRange(xmlRegAtomType type, int codepoint,
                  int neg, int start, int end, const xmlChar *blockName);

void xmlAutomataSetFlags(xmlAutomataPtr am, int flags);

static void
xmlRegexpErrMemory(xmlRegParserCtxtPtr ctxt, const char *extra)
{
    const char *regexp = NULL;
    if (ctxt != NULL) {
        regexp = (const char *) ctxt->string;
	ctxt->error = XML_ERR_NO_MEMORY;
    }
    __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_REGEXP,
		    XML_ERR_NO_MEMORY, XML_ERR_FATAL, NULL, 0, extra,
		    regexp, NULL, 0, 0,
		    "Memory allocation failed : %s\n", extra);
}

static void
xmlRegexpErrCompile(xmlRegParserCtxtPtr ctxt, const char *extra)
{
    const char *regexp = NULL;
    int idx = 0;

    if (ctxt != NULL) {
        regexp = (const char *) ctxt->string;
	idx = ctxt->cur - ctxt->string;
	ctxt->error = XML_REGEXP_COMPILE_ERROR;
    }
    __xmlRaiseError(NULL, NULL, NULL, NULL, NULL, XML_FROM_REGEXP,
		    XML_REGEXP_COMPILE_ERROR, XML_ERR_FATAL, NULL, 0, extra,
		    regexp, NULL, idx, 0,
		    "failed to compile: %s\n", extra);
}


static int xmlFAComputesDeterminism(xmlRegParserCtxtPtr ctxt);
static xmlRegexpPtr
xmlRegEpxFromParse(xmlRegParserCtxtPtr ctxt) {
    xmlRegexpPtr ret;

    ret = (xmlRegexpPtr) xmlMalloc(sizeof(xmlRegexp));
    if (ret == NULL) {
	xmlRegexpErrMemory(ctxt, "compiling regexp");
	return(NULL);
    }
    memset(ret, 0, sizeof(xmlRegexp));
    ret->string = ctxt->string;
    ret->nbStates = ctxt->nbStates;
    ret->states = ctxt->states;
    ret->nbAtoms = ctxt->nbAtoms;
    ret->atoms = ctxt->atoms;
    ret->nbCounters = ctxt->nbCounters;
    ret->counters = ctxt->counters;
    ret->determinist = ctxt->determinist;
    ret->flags = ctxt->flags;
    if (ret->determinist == -1) {
        xmlRegexpIsDeterminist(ret);
    }

    if ((ret->determinist != 0) &&
	(ret->nbCounters == 0) &&
	(ctxt->negs == 0) &&
	(ret->atoms != NULL) &&
	(ret->atoms[0] != NULL) &&
	(ret->atoms[0]->type == XML_REGEXP_STRING)) {
	int i, j, nbstates = 0, nbatoms = 0;
	int *stateRemap;
	int *stringRemap;
	int *transitions;
	void **transdata;
	xmlChar **stringMap;
        xmlChar *value;


	stateRemap = xmlMalloc(ret->nbStates * sizeof(int));
	if (stateRemap == NULL) {
	    xmlRegexpErrMemory(ctxt, "compiling regexp");
	    xmlFree(ret);
	    return(NULL);
	}
	for (i = 0;i < ret->nbStates;i++) {
	    if (ret->states[i] != NULL) {
		stateRemap[i] = nbstates;
		nbstates++;
	    } else {
		stateRemap[i] = -1;
	    }
	}
#ifdef DEBUG_COMPACTION
	printf("Final: %d states\n", nbstates);
#endif
	stringMap = xmlMalloc(ret->nbAtoms * sizeof(char *));
	if (stringMap == NULL) {
	    xmlRegexpErrMemory(ctxt, "compiling regexp");
	    xmlFree(stateRemap);
	    xmlFree(ret);
	    return(NULL);
	}
	stringRemap = xmlMalloc(ret->nbAtoms * sizeof(int));
	if (stringRemap == NULL) {
	    xmlRegexpErrMemory(ctxt, "compiling regexp");
	    xmlFree(stringMap);
	    xmlFree(stateRemap);
	    xmlFree(ret);
	    return(NULL);
	}
	for (i = 0;i < ret->nbAtoms;i++) {
	    if ((ret->atoms[i]->type == XML_REGEXP_STRING) &&
		(ret->atoms[i]->quant == XML_REGEXP_QUANT_ONCE)) {
		value = ret->atoms[i]->valuep;
                for (j = 0;j < nbatoms;j++) {
		    if (xmlStrEqual(stringMap[j], value)) {
			stringRemap[i] = j;
			break;
		    }
		}
		if (j >= nbatoms) {
		    stringRemap[i] = nbatoms;
		    stringMap[nbatoms] = xmlStrdup(value);
		    if (stringMap[nbatoms] == NULL) {
			for (i = 0;i < nbatoms;i++)
			    xmlFree(stringMap[i]);
			xmlFree(stringRemap);
			xmlFree(stringMap);
			xmlFree(stateRemap);
			xmlFree(ret);
			return(NULL);
		    }
		    nbatoms++;
		}
	    } else {
		xmlFree(stateRemap);
		xmlFree(stringRemap);
		for (i = 0;i < nbatoms;i++)
		    xmlFree(stringMap[i]);
		xmlFree(stringMap);
		xmlFree(ret);
		return(NULL);
	    }
	}
#ifdef DEBUG_COMPACTION
	printf("Final: %d atoms\n", nbatoms);
#endif
	transitions = (int *) xmlMalloc((nbstates + 1) *
	                                (nbatoms + 1) * sizeof(int));
	if (transitions == NULL) {
	    xmlFree(stateRemap);
	    xmlFree(stringRemap);
	    xmlFree(stringMap);
	    xmlFree(ret);
	    return(NULL);
	}
	memset(transitions, 0, (nbstates + 1) * (nbatoms + 1) * sizeof(int));

	transdata = NULL;

	for (i = 0;i < ret->nbStates;i++) {
	    int stateno, atomno, targetno, prev;
	    xmlRegStatePtr state;
	    xmlRegTransPtr trans;

	    stateno = stateRemap[i];
	    if (stateno == -1)
		continue;
	    state = ret->states[i];

	    transitions[stateno * (nbatoms + 1)] = state->type;

	    for (j = 0;j < state->nbTrans;j++) {
		trans = &(state->trans[j]);
		if ((trans->to == -1) || (trans->atom == NULL))
		    continue;
                atomno = stringRemap[trans->atom->no];
		if ((trans->atom->data != NULL) && (transdata == NULL)) {
		    transdata = (void **) xmlMalloc(nbstates * nbatoms *
			                            sizeof(void *));
		    if (transdata != NULL)
			memset(transdata, 0,
			       nbstates * nbatoms * sizeof(void *));
		    else {
			xmlRegexpErrMemory(ctxt, "compiling regexp");
			break;
		    }
		}
		targetno = stateRemap[trans->to];
		prev = transitions[stateno * (nbatoms + 1) + atomno + 1];
		if (prev != 0) {
		    if (prev != targetno + 1) {
			ret->determinist = 0;
#ifdef DEBUG_COMPACTION
			printf("Indet: state %d trans %d, atom %d to %d : %d to %d\n",
			       i, j, trans->atom->no, trans->to, atomno, targetno);
			printf("       previous to is %d\n", prev);
#endif
			if (transdata != NULL)
			    xmlFree(transdata);
			xmlFree(transitions);
			xmlFree(stateRemap);
			xmlFree(stringRemap);
			for (i = 0;i < nbatoms;i++)
			    xmlFree(stringMap[i]);
			xmlFree(stringMap);
			goto not_determ;
		    }
		} else {
#if 0
		    printf("State %d trans %d: atom %d to %d : %d to %d\n",
			   i, j, trans->atom->no, trans->to, atomno, targetno);
#endif
		    transitions[stateno * (nbatoms + 1) + atomno + 1] =
			targetno + 1; 
		    if (transdata != NULL)
			transdata[stateno * nbatoms + atomno] =
			    trans->atom->data;
		}
	    }
	}
	ret->determinist = 1;
#ifdef DEBUG_COMPACTION
	for (i = 0;i < nbstates;i++) {
	    for (j = 0;j < nbatoms + 1;j++) {
                printf("%02d ", transitions[i * (nbatoms + 1) + j]);
	    }
	    printf("\n");
	}
	printf("\n");
#endif
	if (ret->states != NULL) {
	    for (i = 0;i < ret->nbStates;i++)
		xmlRegFreeState(ret->states[i]);
	    xmlFree(ret->states);
	}
	ret->states = NULL;
	ret->nbStates = 0;
	if (ret->atoms != NULL) {
	    for (i = 0;i < ret->nbAtoms;i++)
		xmlRegFreeAtom(ret->atoms[i]);
	    xmlFree(ret->atoms);
	}
	ret->atoms = NULL;
	ret->nbAtoms = 0;

	ret->compact = transitions;
	ret->transdata = transdata;
	ret->stringMap = stringMap;
	ret->nbstrings = nbatoms;
	ret->nbstates = nbstates;
	xmlFree(stateRemap);
	xmlFree(stringRemap);
    }
not_determ:
    ctxt->string = NULL;
    ctxt->nbStates = 0;
    ctxt->states = NULL;
    ctxt->nbAtoms = 0;
    ctxt->atoms = NULL;
    ctxt->nbCounters = 0;
    ctxt->counters = NULL;
    return(ret);
}

static xmlRegParserCtxtPtr
xmlRegNewParserCtxt(const xmlChar *string) {
    xmlRegParserCtxtPtr ret;

    ret = (xmlRegParserCtxtPtr) xmlMalloc(sizeof(xmlRegParserCtxt));
    if (ret == NULL)
	return(NULL);
    memset(ret, 0, sizeof(xmlRegParserCtxt));
    if (string != NULL)
	ret->string = xmlStrdup(string);
    ret->cur = ret->string;
    ret->neg = 0;
    ret->negs = 0;
    ret->error = 0;
    ret->determinist = -1;
    return(ret);
}

static xmlRegRangePtr
xmlRegNewRange(xmlRegParserCtxtPtr ctxt,
	       int neg, xmlRegAtomType type, int start, int end) {
    xmlRegRangePtr ret;

    ret = (xmlRegRangePtr) xmlMalloc(sizeof(xmlRegRange));
    if (ret == NULL) {
	xmlRegexpErrMemory(ctxt, "allocating range");
	return(NULL);
    }
    ret->neg = neg;
    ret->type = type;
    ret->start = start;
    ret->end = end;
    return(ret);
}

static void
xmlRegFreeRange(xmlRegRangePtr range) {
    if (range == NULL)
	return;

    if (range->blockName != NULL)
	xmlFree(range->blockName);
    xmlFree(range);
}

static xmlRegRangePtr
xmlRegCopyRange(xmlRegParserCtxtPtr ctxt, xmlRegRangePtr range) {
    xmlRegRangePtr ret;

    if (range == NULL)
	return(NULL);

    ret = xmlRegNewRange(ctxt, range->neg, range->type, range->start,
                         range->end);
    if (ret == NULL)
        return(NULL);
    if (range->blockName != NULL) {
	ret->blockName = xmlStrdup(range->blockName);
	if (ret->blockName == NULL) {
	    xmlRegexpErrMemory(ctxt, "allocating range");
	    xmlRegFreeRange(ret);
	    return(NULL);
	}
    }
    return(ret);
}

static xmlRegAtomPtr
xmlRegNewAtom(xmlRegParserCtxtPtr ctxt, xmlRegAtomType type) {
    xmlRegAtomPtr ret;

    ret = (xmlRegAtomPtr) xmlMalloc(sizeof(xmlRegAtom));
    if (ret == NULL) {
	xmlRegexpErrMemory(ctxt, "allocating atom");
	return(NULL);
    }
    memset(ret, 0, sizeof(xmlRegAtom));
    ret->type = type;
    ret->quant = XML_REGEXP_QUANT_ONCE;
    ret->min = 0;
    ret->max = 0;
    return(ret);
}

static void
xmlRegFreeAtom(xmlRegAtomPtr atom) {
    int i;

    if (atom == NULL)
	return;

    for (i = 0;i < atom->nbRanges;i++)
	xmlRegFreeRange(atom->ranges[i]);
    if (atom->ranges != NULL)
	xmlFree(atom->ranges);
    if ((atom->type == XML_REGEXP_STRING) && (atom->valuep != NULL))
	xmlFree(atom->valuep);
    if ((atom->type == XML_REGEXP_STRING) && (atom->valuep2 != NULL))
	xmlFree(atom->valuep2);
    if ((atom->type == XML_REGEXP_BLOCK_NAME) && (atom->valuep != NULL))
	xmlFree(atom->valuep);
    xmlFree(atom);
}

static xmlRegAtomPtr
xmlRegCopyAtom(xmlRegParserCtxtPtr ctxt, xmlRegAtomPtr atom) {
    xmlRegAtomPtr ret;

    ret = (xmlRegAtomPtr) xmlMalloc(sizeof(xmlRegAtom));
    if (ret == NULL) {
	xmlRegexpErrMemory(ctxt, "copying atom");
	return(NULL);
    }
    memset(ret, 0, sizeof(xmlRegAtom));
    ret->type = atom->type;
    ret->quant = atom->quant;
    ret->min = atom->min;
    ret->max = atom->max;
    if (atom->nbRanges > 0) {
        int i;

        ret->ranges = (xmlRegRangePtr *) xmlMalloc(sizeof(xmlRegRangePtr) *
	                                           atom->nbRanges);
	if (ret->ranges == NULL) {
	    xmlRegexpErrMemory(ctxt, "copying atom");
	    goto error;
	}
	for (i = 0;i < atom->nbRanges;i++) {
	    ret->ranges[i] = xmlRegCopyRange(ctxt, atom->ranges[i]);
	    if (ret->ranges[i] == NULL)
	        goto error;
	    ret->nbRanges = i + 1;
	}
    }
    return(ret);

error:
    xmlRegFreeAtom(ret);
    return(NULL);
}

static xmlRegStatePtr
xmlRegNewState(xmlRegParserCtxtPtr ctxt) {
    xmlRegStatePtr ret;

    ret = (xmlRegStatePtr) xmlMalloc(sizeof(xmlRegState));
    if (ret == NULL) {
	xmlRegexpErrMemory(ctxt, "allocating state");
	return(NULL);
    }
    memset(ret, 0, sizeof(xmlRegState));
    ret->type = XML_REGEXP_TRANS_STATE;
    ret->mark = XML_REGEXP_MARK_NORMAL;
    return(ret);
}

static void
xmlRegFreeState(xmlRegStatePtr state) {
    if (state == NULL)
	return;

    if (state->trans != NULL)
	xmlFree(state->trans);
    if (state->transTo != NULL)
	xmlFree(state->transTo);
    xmlFree(state);
}

static void
xmlRegFreeParserCtxt(xmlRegParserCtxtPtr ctxt) {
    int i;
    if (ctxt == NULL)
	return;

    if (ctxt->string != NULL)
	xmlFree(ctxt->string);
    if (ctxt->states != NULL) {
	for (i = 0;i < ctxt->nbStates;i++)
	    xmlRegFreeState(ctxt->states[i]);
	xmlFree(ctxt->states);
    }
    if (ctxt->atoms != NULL) {
	for (i = 0;i < ctxt->nbAtoms;i++)
	    xmlRegFreeAtom(ctxt->atoms[i]);
	xmlFree(ctxt->atoms);
    }
    if (ctxt->counters != NULL)
	xmlFree(ctxt->counters);
    xmlFree(ctxt);
}


static void
xmlRegPrintAtomType(FILE *output, xmlRegAtomType type) {
    switch (type) {
        case XML_REGEXP_EPSILON:
	    fprintf(output, "epsilon "); break;
        case XML_REGEXP_CHARVAL:
	    fprintf(output, "charval "); break;
        case XML_REGEXP_RANGES:
	    fprintf(output, "ranges "); break;
        case XML_REGEXP_SUBREG:
	    fprintf(output, "subexpr "); break;
        case XML_REGEXP_STRING:
	    fprintf(output, "string "); break;
        case XML_REGEXP_ANYCHAR:
	    fprintf(output, "anychar "); break;
        case XML_REGEXP_ANYSPACE:
	    fprintf(output, "anyspace "); break;
        case XML_REGEXP_NOTSPACE:
	    fprintf(output, "notspace "); break;
        case XML_REGEXP_INITNAME:
	    fprintf(output, "initname "); break;
        case XML_REGEXP_NOTINITNAME:
	    fprintf(output, "notinitname "); break;
        case XML_REGEXP_NAMECHAR:
	    fprintf(output, "namechar "); break;
        case XML_REGEXP_NOTNAMECHAR:
	    fprintf(output, "notnamechar "); break;
        case XML_REGEXP_DECIMAL:
	    fprintf(output, "decimal "); break;
        case XML_REGEXP_NOTDECIMAL:
	    fprintf(output, "notdecimal "); break;
        case XML_REGEXP_REALCHAR:
	    fprintf(output, "realchar "); break;
        case XML_REGEXP_NOTREALCHAR:
	    fprintf(output, "notrealchar "); break;
        case XML_REGEXP_LETTER:
            fprintf(output, "LETTER "); break;
        case XML_REGEXP_LETTER_UPPERCASE:
            fprintf(output, "LETTER_UPPERCASE "); break;
        case XML_REGEXP_LETTER_LOWERCASE:
            fprintf(output, "LETTER_LOWERCASE "); break;
        case XML_REGEXP_LETTER_TITLECASE:
            fprintf(output, "LETTER_TITLECASE "); break;
        case XML_REGEXP_LETTER_MODIFIER:
            fprintf(output, "LETTER_MODIFIER "); break;
        case XML_REGEXP_LETTER_OTHERS:
            fprintf(output, "LETTER_OTHERS "); break;
        case XML_REGEXP_MARK:
            fprintf(output, "MARK "); break;
        case XML_REGEXP_MARK_NONSPACING:
            fprintf(output, "MARK_NONSPACING "); break;
        case XML_REGEXP_MARK_SPACECOMBINING:
            fprintf(output, "MARK_SPACECOMBINING "); break;
        case XML_REGEXP_MARK_ENCLOSING:
            fprintf(output, "MARK_ENCLOSING "); break;
        case XML_REGEXP_NUMBER:
            fprintf(output, "NUMBER "); break;
        case XML_REGEXP_NUMBER_DECIMAL:
            fprintf(output, "NUMBER_DECIMAL "); break;
        case XML_REGEXP_NUMBER_LETTER:
            fprintf(output, "NUMBER_LETTER "); break;
        case XML_REGEXP_NUMBER_OTHERS:
            fprintf(output, "NUMBER_OTHERS "); break;
        case XML_REGEXP_PUNCT:
            fprintf(output, "PUNCT "); break;
        case XML_REGEXP_PUNCT_CONNECTOR:
            fprintf(output, "PUNCT_CONNECTOR "); break;
        case XML_REGEXP_PUNCT_DASH:
            fprintf(output, "PUNCT_DASH "); break;
        case XML_REGEXP_PUNCT_OPEN:
            fprintf(output, "PUNCT_OPEN "); break;
        case XML_REGEXP_PUNCT_CLOSE:
            fprintf(output, "PUNCT_CLOSE "); break;
        case XML_REGEXP_PUNCT_INITQUOTE:
            fprintf(output, "PUNCT_INITQUOTE "); break;
        case XML_REGEXP_PUNCT_FINQUOTE:
            fprintf(output, "PUNCT_FINQUOTE "); break;
        case XML_REGEXP_PUNCT_OTHERS:
            fprintf(output, "PUNCT_OTHERS "); break;
        case XML_REGEXP_SEPAR:
            fprintf(output, "SEPAR "); break;
        case XML_REGEXP_SEPAR_SPACE:
            fprintf(output, "SEPAR_SPACE "); break;
        case XML_REGEXP_SEPAR_LINE:
            fprintf(output, "SEPAR_LINE "); break;
        case XML_REGEXP_SEPAR_PARA:
            fprintf(output, "SEPAR_PARA "); break;
        case XML_REGEXP_SYMBOL:
            fprintf(output, "SYMBOL "); break;
        case XML_REGEXP_SYMBOL_MATH:
            fprintf(output, "SYMBOL_MATH "); break;
        case XML_REGEXP_SYMBOL_CURRENCY:
            fprintf(output, "SYMBOL_CURRENCY "); break;
        case XML_REGEXP_SYMBOL_MODIFIER:
            fprintf(output, "SYMBOL_MODIFIER "); break;
        case XML_REGEXP_SYMBOL_OTHERS:
            fprintf(output, "SYMBOL_OTHERS "); break;
        case XML_REGEXP_OTHER:
            fprintf(output, "OTHER "); break;
        case XML_REGEXP_OTHER_CONTROL:
            fprintf(output, "OTHER_CONTROL "); break;
        case XML_REGEXP_OTHER_FORMAT:
            fprintf(output, "OTHER_FORMAT "); break;
        case XML_REGEXP_OTHER_PRIVATE:
            fprintf(output, "OTHER_PRIVATE "); break;
        case XML_REGEXP_OTHER_NA:
            fprintf(output, "OTHER_NA "); break;
        case XML_REGEXP_BLOCK_NAME:
	    fprintf(output, "BLOCK "); break;
    }
}

static void
xmlRegPrintQuantType(FILE *output, xmlRegQuantType type) {
    switch (type) {
        case XML_REGEXP_QUANT_EPSILON:
	    fprintf(output, "epsilon "); break;
        case XML_REGEXP_QUANT_ONCE:
	    fprintf(output, "once "); break;
        case XML_REGEXP_QUANT_OPT:
	    fprintf(output, "? "); break;
        case XML_REGEXP_QUANT_MULT:
	    fprintf(output, "* "); break;
        case XML_REGEXP_QUANT_PLUS:
	    fprintf(output, "+ "); break;
	case XML_REGEXP_QUANT_RANGE:
	    fprintf(output, "range "); break;
	case XML_REGEXP_QUANT_ONCEONLY:
	    fprintf(output, "onceonly "); break;
	case XML_REGEXP_QUANT_ALL:
	    fprintf(output, "all "); break;
    }
}
static void
xmlRegPrintRange(FILE *output, xmlRegRangePtr range) {
    fprintf(output, "  range: ");
    if (range->neg)
	fprintf(output, "negative ");
    xmlRegPrintAtomType(output, range->type);
    fprintf(output, "%c - %c\n", range->start, range->end);
}

static void
xmlRegPrintAtom(FILE *output, xmlRegAtomPtr atom) {
    fprintf(output, " atom: ");
    if (atom == NULL) {
	fprintf(output, "NULL\n");
	return;
    }
    if (atom->neg)
        fprintf(output, "not ");
    xmlRegPrintAtomType(output, atom->type);
    xmlRegPrintQuantType(output, atom->quant);
    if (atom->quant == XML_REGEXP_QUANT_RANGE)
	fprintf(output, "%d-%d ", atom->min, atom->max);
    if (atom->type == XML_REGEXP_STRING)
	fprintf(output, "'%s' ", (char *) atom->valuep);
    if (atom->type == XML_REGEXP_CHARVAL)
	fprintf(output, "char %c\n", atom->codepoint);
    else if (atom->type == XML_REGEXP_RANGES) {
	int i;
	fprintf(output, "%d entries\n", atom->nbRanges);
	for (i = 0; i < atom->nbRanges;i++)
	    xmlRegPrintRange(output, atom->ranges[i]);
    } else if (atom->type == XML_REGEXP_SUBREG) {
	fprintf(output, "start %d end %d\n", atom->start->no, atom->stop->no);
    } else {
	fprintf(output, "\n");
    }
}

static void
xmlRegPrintTrans(FILE *output, xmlRegTransPtr trans) {
    fprintf(output, "  trans: ");
    if (trans == NULL) {
	fprintf(output, "NULL\n");
	return;
    }
    if (trans->to < 0) {
	fprintf(output, "removed\n");
	return;
    }
    if (trans->nd != 0) {
	if (trans->nd == 2)
	    fprintf(output, "last not determinist, ");
	else
	    fprintf(output, "not determinist, ");
    }
    if (trans->counter >= 0) {
	fprintf(output, "counted %d, ", trans->counter);
    }
    if (trans->count == REGEXP_ALL_COUNTER) {
	fprintf(output, "all transition, ");
    } else if (trans->count >= 0) {
	fprintf(output, "count based %d, ", trans->count);
    }
    if (trans->atom == NULL) {
	fprintf(output, "epsilon to %d\n", trans->to);
	return;
    }
    if (trans->atom->type == XML_REGEXP_CHARVAL)
	fprintf(output, "char %c ", trans->atom->codepoint);
    fprintf(output, "atom %d, to %d\n", trans->atom->no, trans->to);
}
    
static void
xmlRegPrintState(FILE *output, xmlRegStatePtr state) {
    int i;

    fprintf(output, " state: ");
    if (state == NULL) {
	fprintf(output, "NULL\n");
	return;
    }
    if (state->type == XML_REGEXP_START_STATE)
	fprintf(output, "START ");
    if (state->type == XML_REGEXP_FINAL_STATE)
	fprintf(output, "FINAL ");
    
    fprintf(output, "%d, %d transitions:\n", state->no, state->nbTrans);
    for (i = 0;i < state->nbTrans; i++) {
	xmlRegPrintTrans(output, &(state->trans[i]));
    }
}

#ifdef DEBUG_REGEXP_GRAPH
static void
xmlRegPrintCtxt(FILE *output, xmlRegParserCtxtPtr ctxt) {
    int i;

    fprintf(output, " ctxt: ");
    if (ctxt == NULL) {
	fprintf(output, "NULL\n");
	return;
    }
    fprintf(output, "'%s' ", ctxt->string);
    if (ctxt->error)
	fprintf(output, "error ");
    if (ctxt->neg)
	fprintf(output, "neg ");
    fprintf(output, "\n");
    fprintf(output, "%d atoms:\n", ctxt->nbAtoms);
    for (i = 0;i < ctxt->nbAtoms; i++) {
	fprintf(output, " %02d ", i);
	xmlRegPrintAtom(output, ctxt->atoms[i]);
    }
    if (ctxt->atom != NULL) {
	fprintf(output, "current atom:\n");
	xmlRegPrintAtom(output, ctxt->atom);
    }
    fprintf(output, "%d states:", ctxt->nbStates);
    if (ctxt->start != NULL)
	fprintf(output, " start: %d", ctxt->start->no);
    if (ctxt->end != NULL)
	fprintf(output, " end: %d", ctxt->end->no);
    fprintf(output, "\n");
    for (i = 0;i < ctxt->nbStates; i++) {
	xmlRegPrintState(output, ctxt->states[i]);
    }
    fprintf(output, "%d counters:\n", ctxt->nbCounters);
    for (i = 0;i < ctxt->nbCounters; i++) {
	fprintf(output, " %d: min %d max %d\n", i, ctxt->counters[i].min,
		                                ctxt->counters[i].max);
    }
}
#endif


static void 
xmlRegAtomAddRange(xmlRegParserCtxtPtr ctxt, xmlRegAtomPtr atom,
	           int neg, xmlRegAtomType type, int start, int end,
		   xmlChar *blockName) {
    xmlRegRangePtr range;

    if (atom == NULL) {
	ERROR("add range: atom is NULL");
	return;
    }
    if (atom->type != XML_REGEXP_RANGES) {
	ERROR("add range: atom is not ranges");
	return;
    }
    if (atom->maxRanges == 0) {
	atom->maxRanges = 4;
	atom->ranges = (xmlRegRangePtr *) xmlMalloc(atom->maxRanges *
		                             sizeof(xmlRegRangePtr));
	if (atom->ranges == NULL) {
	    xmlRegexpErrMemory(ctxt, "adding ranges");
	    atom->maxRanges = 0;
	    return;
	}
    } else if (atom->nbRanges >= atom->maxRanges) {
	xmlRegRangePtr *tmp;
	atom->maxRanges *= 2;
	tmp = (xmlRegRangePtr *) xmlRealloc(atom->ranges, atom->maxRanges *
		                             sizeof(xmlRegRangePtr));
	if (tmp == NULL) {
	    xmlRegexpErrMemory(ctxt, "adding ranges");
	    atom->maxRanges /= 2;
	    return;
	}
	atom->ranges = tmp;
    }
    range = xmlRegNewRange(ctxt, neg, type, start, end);
    if (range == NULL)
	return;
    range->blockName = blockName;
    atom->ranges[atom->nbRanges++] = range;
    
}

static int
xmlRegGetCounter(xmlRegParserCtxtPtr ctxt) {
    if (ctxt->maxCounters == 0) {
	ctxt->maxCounters = 4;
	ctxt->counters = (xmlRegCounter *) xmlMalloc(ctxt->maxCounters *
		                             sizeof(xmlRegCounter));
	if (ctxt->counters == NULL) {
	    xmlRegexpErrMemory(ctxt, "allocating counter");
	    ctxt->maxCounters = 0;
	    return(-1);
	}
    } else if (ctxt->nbCounters >= ctxt->maxCounters) {
	xmlRegCounter *tmp;
	ctxt->maxCounters *= 2;
	tmp = (xmlRegCounter *) xmlRealloc(ctxt->counters, ctxt->maxCounters *
		                           sizeof(xmlRegCounter));
	if (tmp == NULL) {
	    xmlRegexpErrMemory(ctxt, "allocating counter");
	    ctxt->maxCounters /= 2;
	    return(-1);
	}
	ctxt->counters = tmp;
    }
    ctxt->counters[ctxt->nbCounters].min = -1;
    ctxt->counters[ctxt->nbCounters].max = -1;
    return(ctxt->nbCounters++);
}

static int 
xmlRegAtomPush(xmlRegParserCtxtPtr ctxt, xmlRegAtomPtr atom) {
    if (atom == NULL) {
	ERROR("atom push: atom is NULL");
	return(-1);
    }
    if (ctxt->maxAtoms == 0) {
	ctxt->maxAtoms = 4;
	ctxt->atoms = (xmlRegAtomPtr *) xmlMalloc(ctxt->maxAtoms *
		                             sizeof(xmlRegAtomPtr));
	if (ctxt->atoms == NULL) {
	    xmlRegexpErrMemory(ctxt, "pushing atom");
	    ctxt->maxAtoms = 0;
	    return(-1);
	}
    } else if (ctxt->nbAtoms >= ctxt->maxAtoms) {
	xmlRegAtomPtr *tmp;
	ctxt->maxAtoms *= 2;
	tmp = (xmlRegAtomPtr *) xmlRealloc(ctxt->atoms, ctxt->maxAtoms *
		                             sizeof(xmlRegAtomPtr));
	if (tmp == NULL) {
	    xmlRegexpErrMemory(ctxt, "allocating counter");
	    ctxt->maxAtoms /= 2;
	    return(-1);
	}
	ctxt->atoms = tmp;
    }
    atom->no = ctxt->nbAtoms;
    ctxt->atoms[ctxt->nbAtoms++] = atom;
    return(0);
}

static void 
xmlRegStateAddTransTo(xmlRegParserCtxtPtr ctxt, xmlRegStatePtr target,
                      int from) {
    if (target->maxTransTo == 0) {
	target->maxTransTo = 8;
	target->transTo = (int *) xmlMalloc(target->maxTransTo *
		                             sizeof(int));
	if (target->transTo == NULL) {
	    xmlRegexpErrMemory(ctxt, "adding transition");
	    target->maxTransTo = 0;
	    return;
	}
    } else if (target->nbTransTo >= target->maxTransTo) {
	int *tmp;
	target->maxTransTo *= 2;
	tmp = (int *) xmlRealloc(target->transTo, target->maxTransTo *
		                             sizeof(int));
	if (tmp == NULL) {
	    xmlRegexpErrMemory(ctxt, "adding transition");
	    target->maxTransTo /= 2;
	    return;
	}
	target->transTo = tmp;
    }
    target->transTo[target->nbTransTo] = from;
    target->nbTransTo++;
}

static void 
xmlRegStateAddTrans(xmlRegParserCtxtPtr ctxt, xmlRegStatePtr state,
	            xmlRegAtomPtr atom, xmlRegStatePtr target,
		    int counter, int count) {

    int nrtrans;

    if (state == NULL) {
	ERROR("add state: state is NULL");
	return;
    }
    if (target == NULL) {
	ERROR("add state: target is NULL");
	return;
    }

    for (nrtrans = state->nbTrans - 1; nrtrans >= 0; nrtrans--) {
	xmlRegTransPtr trans = &(state->trans[nrtrans]);
	if ((trans->atom == atom) &&
	    (trans->to == target->no) &&
	    (trans->counter == counter) &&
	    (trans->count == count)) {
#ifdef DEBUG_REGEXP_GRAPH
	    printf("Ignoring duplicate transition from %d to %d\n",
		    state->no, target->no);
#endif
	    return;
	}
    }

    if (state->maxTrans == 0) {
	state->maxTrans = 8;
	state->trans = (xmlRegTrans *) xmlMalloc(state->maxTrans *
		                             sizeof(xmlRegTrans));
	if (state->trans == NULL) {
	    xmlRegexpErrMemory(ctxt, "adding transition");
	    state->maxTrans = 0;
	    return;
	}
    } else if (state->nbTrans >= state->maxTrans) {
	xmlRegTrans *tmp;
	state->maxTrans *= 2;
	tmp = (xmlRegTrans *) xmlRealloc(state->trans, state->maxTrans *
		                             sizeof(xmlRegTrans));
	if (tmp == NULL) {
	    xmlRegexpErrMemory(ctxt, "adding transition");
	    state->maxTrans /= 2;
	    return;
	}
	state->trans = tmp;
    }
#ifdef DEBUG_REGEXP_GRAPH
    printf("Add trans from %d to %d ", state->no, target->no);
    if (count == REGEXP_ALL_COUNTER)
	printf("all transition\n");
    else if (count >= 0)
	printf("count based %d\n", count);
    else if (counter >= 0)
	printf("counted %d\n", counter);
    else if (atom == NULL)
	printf("epsilon transition\n");
    else if (atom != NULL) 
        xmlRegPrintAtom(stdout, atom);
#endif

    state->trans[state->nbTrans].atom = atom;
    state->trans[state->nbTrans].to = target->no;
    state->trans[state->nbTrans].counter = counter;
    state->trans[state->nbTrans].count = count;
    state->trans[state->nbTrans].nd = 0;
    state->nbTrans++;
    xmlRegStateAddTransTo(ctxt, target, state->no);
}

static int
xmlRegStatePush(xmlRegParserCtxtPtr ctxt, xmlRegStatePtr state) {
    if (state == NULL) return(-1);
    if (ctxt->maxStates == 0) {
	ctxt->maxStates = 4;
	ctxt->states = (xmlRegStatePtr *) xmlMalloc(ctxt->maxStates *
		                             sizeof(xmlRegStatePtr));
	if (ctxt->states == NULL) {
	    xmlRegexpErrMemory(ctxt, "adding state");
	    ctxt->maxStates = 0;
	    return(-1);
	}
    } else if (ctxt->nbStates >= ctxt->maxStates) {
	xmlRegStatePtr *tmp;
	ctxt->maxStates *= 2;
	tmp = (xmlRegStatePtr *) xmlRealloc(ctxt->states, ctxt->maxStates *
		                             sizeof(xmlRegStatePtr));
	if (tmp == NULL) {
	    xmlRegexpErrMemory(ctxt, "adding state");
	    ctxt->maxStates /= 2;
	    return(-1);
	}
	ctxt->states = tmp;
    }
    state->no = ctxt->nbStates;
    ctxt->states[ctxt->nbStates++] = state;
    return(0);
}

static void
xmlFAGenerateAllTransition(xmlRegParserCtxtPtr ctxt,
			   xmlRegStatePtr from, xmlRegStatePtr to,
			   int lax) {
    if (to == NULL) {
	to = xmlRegNewState(ctxt);
	xmlRegStatePush(ctxt, to);
	ctxt->state = to;
    }
    if (lax)
	xmlRegStateAddTrans(ctxt, from, NULL, to, -1, REGEXP_ALL_LAX_COUNTER);
    else
	xmlRegStateAddTrans(ctxt, from, NULL, to, -1, REGEXP_ALL_COUNTER);
}

static void
xmlFAGenerateEpsilonTransition(xmlRegParserCtxtPtr ctxt,
			       xmlRegStatePtr from, xmlRegStatePtr to) {
    if (to == NULL) {
	to = xmlRegNewState(ctxt);
	xmlRegStatePush(ctxt, to);
	ctxt->state = to;
    }
    xmlRegStateAddTrans(ctxt, from, NULL, to, -1, -1);
}

static void
xmlFAGenerateCountedEpsilonTransition(xmlRegParserCtxtPtr ctxt,
	    xmlRegStatePtr from, xmlRegStatePtr to, int counter) {
    if (to == NULL) {
	to = xmlRegNewState(ctxt);
	xmlRegStatePush(ctxt, to);
	ctxt->state = to;
    }
    xmlRegStateAddTrans(ctxt, from, NULL, to, counter, -1);
}

static void
xmlFAGenerateCountedTransition(xmlRegParserCtxtPtr ctxt,
	    xmlRegStatePtr from, xmlRegStatePtr to, int counter) {
    if (to == NULL) {
	to = xmlRegNewState(ctxt);
	xmlRegStatePush(ctxt, to);
	ctxt->state = to;
    }
    xmlRegStateAddTrans(ctxt, from, NULL, to, -1, counter);
}

static int
xmlFAGenerateTransitions(xmlRegParserCtxtPtr ctxt, xmlRegStatePtr from,
	                 xmlRegStatePtr to, xmlRegAtomPtr atom) {
    xmlRegStatePtr end;

    if (atom == NULL) {
	ERROR("genrate transition: atom == NULL");
	return(-1);
    }
    if (atom->type == XML_REGEXP_SUBREG) {
	if (xmlRegAtomPush(ctxt, atom) < 0) {
	    return(-1);
	}
	if ((to != NULL) && (atom->stop != to) &&
	    (atom->quant != XML_REGEXP_QUANT_RANGE)) {
	    xmlFAGenerateEpsilonTransition(ctxt, atom->stop, to);
#ifdef DV
	} else if ((to == NULL) && (atom->quant != XML_REGEXP_QUANT_RANGE) && 
		   (atom->quant != XML_REGEXP_QUANT_ONCE)) {
	    to = xmlRegNewState(ctxt);
	    xmlRegStatePush(ctxt, to);
	    ctxt->state = to;
	    xmlFAGenerateEpsilonTransition(ctxt, atom->stop, to);
#endif
	}
	switch (atom->quant) {
	    case XML_REGEXP_QUANT_OPT:
		atom->quant = XML_REGEXP_QUANT_ONCE;
                if (to == NULL) {
                    xmlFAGenerateEpsilonTransition(ctxt, atom->start, 0);
                    xmlFAGenerateEpsilonTransition(ctxt, atom->stop,
                                                   ctxt->state);
                } else {
                    xmlFAGenerateEpsilonTransition(ctxt, atom->start, to);
                }
		break;
	    case XML_REGEXP_QUANT_MULT:
		atom->quant = XML_REGEXP_QUANT_ONCE;
		xmlFAGenerateEpsilonTransition(ctxt, atom->start, atom->stop);
		xmlFAGenerateEpsilonTransition(ctxt, atom->stop, atom->start);
		break;
	    case XML_REGEXP_QUANT_PLUS:
		atom->quant = XML_REGEXP_QUANT_ONCE;
		xmlFAGenerateEpsilonTransition(ctxt, atom->stop, atom->start);
		break;
	    case XML_REGEXP_QUANT_RANGE: {
		int counter;
		xmlRegStatePtr inter, newstate;

		if (to != NULL) {
		    newstate = to;
		} else {
		    newstate = xmlRegNewState(ctxt);
		    xmlRegStatePush(ctxt, newstate);
		}

		if ((atom->min == 0) && (atom->start0 == NULL)) {
		    xmlRegAtomPtr copy;
		    copy = xmlRegCopyAtom(ctxt, atom);
		    if (copy == NULL)
		        return(-1);
		    copy->quant = XML_REGEXP_QUANT_ONCE;
		    copy->min = 0;
		    copy->max = 0;

		    if (xmlFAGenerateTransitions(ctxt, atom->start, NULL, copy)
		        < 0)
			return(-1);
		    inter = ctxt->state;
		    counter = xmlRegGetCounter(ctxt);
		    ctxt->counters[counter].min = atom->min - 1;
		    ctxt->counters[counter].max = atom->max - 1;
		    
		    xmlFAGenerateCountedEpsilonTransition(ctxt, inter,
						   atom->stop, counter);
		    
		    xmlFAGenerateCountedTransition(ctxt, inter,
			                           newstate, counter);
		    
		    xmlFAGenerateEpsilonTransition(ctxt, atom->start,
		                                   newstate);
		} else {
		    counter = xmlRegGetCounter(ctxt);
		    ctxt->counters[counter].min = atom->min - 1;
		    ctxt->counters[counter].max = atom->max - 1;
		    
		    xmlFAGenerateCountedEpsilonTransition(ctxt, atom->stop,
						   atom->start, counter);
		    
		    xmlFAGenerateCountedTransition(ctxt, atom->stop,
			                           newstate, counter);
		    
		    if (atom->min == 0)
			xmlFAGenerateEpsilonTransition(ctxt, atom->start0,
						       newstate);

		}
		atom->min = 0;
		atom->max = 0;
		atom->quant = XML_REGEXP_QUANT_ONCE;
		ctxt->state = newstate;
	    }
	    default:
		break;
	}
	return(0);
    } 
    if ((atom->min == 0) && (atom->max == 0) &&
               (atom->quant == XML_REGEXP_QUANT_RANGE)) {
	if (to == NULL) {
	    to = xmlRegNewState(ctxt);
	    if (to != NULL)
		xmlRegStatePush(ctxt, to);
	    else {
		return(-1);
	    }
	}
	xmlFAGenerateEpsilonTransition(ctxt, from, to);
	ctxt->state = to;
	xmlRegFreeAtom(atom);
	return(0);
    }
    if (to == NULL) {
	to = xmlRegNewState(ctxt);
	if (to != NULL)
	    xmlRegStatePush(ctxt, to);
	else {
	    return(-1);
	}
    } 
    end = to;
    if ((atom->quant == XML_REGEXP_QUANT_MULT) || 
        (atom->quant == XML_REGEXP_QUANT_PLUS)) {
        xmlRegStatePtr tmp;
	
	tmp = xmlRegNewState(ctxt);
	if (tmp != NULL)
	    xmlRegStatePush(ctxt, tmp);
	else {
	    return(-1);
	}
	xmlFAGenerateEpsilonTransition(ctxt, tmp, to);
	to = tmp;
    }
    if (xmlRegAtomPush(ctxt, atom) < 0) {
	return(-1);
    }
    xmlRegStateAddTrans(ctxt, from, atom, to, -1, -1);
    ctxt->state = end;
    switch (atom->quant) {
	case XML_REGEXP_QUANT_OPT:
	    atom->quant = XML_REGEXP_QUANT_ONCE;
	    xmlFAGenerateEpsilonTransition(ctxt, from, to);
	    break;
	case XML_REGEXP_QUANT_MULT:
	    atom->quant = XML_REGEXP_QUANT_ONCE;
	    xmlFAGenerateEpsilonTransition(ctxt, from, to);
	    xmlRegStateAddTrans(ctxt, to, atom, to, -1, -1);
	    break;
	case XML_REGEXP_QUANT_PLUS:
	    atom->quant = XML_REGEXP_QUANT_ONCE;
	    xmlRegStateAddTrans(ctxt, to, atom, to, -1, -1);
	    break;
	case XML_REGEXP_QUANT_RANGE: 
#if DV_test
	    if (atom->min == 0) {
		xmlFAGenerateEpsilonTransition(ctxt, from, to);
	    }
#endif
	    break;
	default:
	    break;
    }
    return(0);
}

static void
xmlFAReduceEpsilonTransitions(xmlRegParserCtxtPtr ctxt, int fromnr,
	                      int tonr, int counter) {
    int transnr;
    xmlRegStatePtr from;
    xmlRegStatePtr to;

#ifdef DEBUG_REGEXP_GRAPH
    printf("xmlFAReduceEpsilonTransitions(%d, %d)\n", fromnr, tonr);
#endif
    from = ctxt->states[fromnr];
    if (from == NULL)
	return;
    to = ctxt->states[tonr];
    if (to == NULL)
	return;
    if ((to->mark == XML_REGEXP_MARK_START) ||
	(to->mark == XML_REGEXP_MARK_VISITED))
	return;

    to->mark = XML_REGEXP_MARK_VISITED;
    if (to->type == XML_REGEXP_FINAL_STATE) {
#ifdef DEBUG_REGEXP_GRAPH
	printf("State %d is final, so %d becomes final\n", tonr, fromnr);
#endif
	from->type = XML_REGEXP_FINAL_STATE;
    }
    for (transnr = 0;transnr < to->nbTrans;transnr++) {
        if (to->trans[transnr].to < 0)
	    continue;
	if (to->trans[transnr].atom == NULL) {
	    if (to->trans[transnr].to != fromnr) {
		if (to->trans[transnr].count >= 0) {
		    int newto = to->trans[transnr].to;

		    xmlRegStateAddTrans(ctxt, from, NULL,
					ctxt->states[newto], 
					-1, to->trans[transnr].count);
		} else {
#ifdef DEBUG_REGEXP_GRAPH
		    printf("Found epsilon trans %d from %d to %d\n",
			   transnr, tonr, to->trans[transnr].to);
#endif
		    if (to->trans[transnr].counter >= 0) {
			xmlFAReduceEpsilonTransitions(ctxt, fromnr,
					      to->trans[transnr].to,
					      to->trans[transnr].counter);
		    } else {
			xmlFAReduceEpsilonTransitions(ctxt, fromnr,
					      to->trans[transnr].to,
					      counter);
		    }
		}
	    }
	} else {
	    int newto = to->trans[transnr].to;

	    if (to->trans[transnr].counter >= 0) {
		xmlRegStateAddTrans(ctxt, from, to->trans[transnr].atom, 
				    ctxt->states[newto], 
				    to->trans[transnr].counter, -1);
	    } else {
		xmlRegStateAddTrans(ctxt, from, to->trans[transnr].atom, 
				    ctxt->states[newto], counter, -1);
	    }
	}
    }
    to->mark = XML_REGEXP_MARK_NORMAL;
}

static void
xmlFAEliminateSimpleEpsilonTransitions(xmlRegParserCtxtPtr ctxt) {
    int statenr, i, j, newto;
    xmlRegStatePtr state, tmp;

    for (statenr = 0;statenr < ctxt->nbStates;statenr++) {
	state = ctxt->states[statenr];
	if (state == NULL)
	    continue;
	if (state->nbTrans != 1)
	    continue;
	if (state->type == XML_REGEXP_UNREACH_STATE)
	    continue;
	
	if ((state->trans[0].atom == NULL) &&
	    (state->trans[0].to >= 0) &&
	    (state->trans[0].to != statenr) &&
	    (state->trans[0].counter < 0) &&
	    (state->trans[0].count < 0)) {
	    newto = state->trans[0].to;

            if (state->type == XML_REGEXP_START_STATE) {
#ifdef DEBUG_REGEXP_GRAPH
		printf("Found simple epsilon trans from start %d to %d\n",
		       statenr, newto);
#endif     
            } else {
#ifdef DEBUG_REGEXP_GRAPH
		printf("Found simple epsilon trans from %d to %d\n",
		       statenr, newto);
#endif     
	        for (i = 0;i < state->nbTransTo;i++) {
		    tmp = ctxt->states[state->transTo[i]];
		    for (j = 0;j < tmp->nbTrans;j++) {
			if (tmp->trans[j].to == statenr) {
#ifdef DEBUG_REGEXP_GRAPH
			    printf("Changed transition %d on %d to go to %d\n",
				   j, tmp->no, newto);
#endif     
			    tmp->trans[j].to = -1;
			    xmlRegStateAddTrans(ctxt, tmp, tmp->trans[j].atom,
			    			ctxt->states[newto],
					        tmp->trans[j].counter,
						tmp->trans[j].count);
			}
		    }
		}
		if (state->type == XML_REGEXP_FINAL_STATE)
		    ctxt->states[newto]->type = XML_REGEXP_FINAL_STATE;
		
		state->nbTrans = 0;

                state->type = XML_REGEXP_UNREACH_STATE;

	    }
            
	}
    }
}
static void
xmlFAEliminateEpsilonTransitions(xmlRegParserCtxtPtr ctxt) {
    int statenr, transnr;
    xmlRegStatePtr state;
    int has_epsilon;

    if (ctxt->states == NULL) return;

    xmlFAEliminateSimpleEpsilonTransitions(ctxt);
    for (statenr = 0;statenr < ctxt->nbStates;statenr++) {
	state = ctxt->states[statenr];
	if ((state != NULL) && (state->type == XML_REGEXP_UNREACH_STATE)) {
#ifdef DEBUG_REGEXP_GRAPH
	    printf("Removed unreachable state %d\n", statenr);
#endif
	    xmlRegFreeState(state);
	    ctxt->states[statenr] = NULL;
	}
    }

    has_epsilon = 0;

    for (statenr = ctxt->nbStates - 1;statenr >= 0;statenr--) {
	state = ctxt->states[statenr];
	if (state == NULL)
	    continue;
	if ((state->nbTrans == 0) &&
	    (state->type != XML_REGEXP_FINAL_STATE)) {
	    state->type = XML_REGEXP_SINK_STATE;
	}
	for (transnr = 0;transnr < state->nbTrans;transnr++) {
	    if ((state->trans[transnr].atom == NULL) &&
		(state->trans[transnr].to >= 0)) {
		if (state->trans[transnr].to == statenr) {
		    state->trans[transnr].to = -1;
#ifdef DEBUG_REGEXP_GRAPH
		    printf("Removed loopback epsilon trans %d on %d\n",
			   transnr, statenr);
#endif
		} else if (state->trans[transnr].count < 0) {
		    int newto = state->trans[transnr].to;

#ifdef DEBUG_REGEXP_GRAPH
		    printf("Found epsilon trans %d from %d to %d\n",
			   transnr, statenr, newto);
#endif
		    has_epsilon = 1;
		    state->trans[transnr].to = -2;
		    state->mark = XML_REGEXP_MARK_START;
		    xmlFAReduceEpsilonTransitions(ctxt, statenr,
				      newto, state->trans[transnr].counter);
		    state->mark = XML_REGEXP_MARK_NORMAL;
#ifdef DEBUG_REGEXP_GRAPH
		} else {
		    printf("Found counted transition %d on %d\n",
			   transnr, statenr);
#endif
	        }
	    }
	}
    }
    if (has_epsilon) {
	for (statenr = 0;statenr < ctxt->nbStates;statenr++) {
	    state = ctxt->states[statenr];
	    if (state == NULL)
		continue;
	    for (transnr = 0;transnr < state->nbTrans;transnr++) {
		xmlRegTransPtr trans = &(state->trans[transnr]);
		if ((trans->atom == NULL) &&
		    (trans->count < 0) &&
		    (trans->to >= 0)) {
		    trans->to = -1;
		}
	    }
	}
    }

    for (statenr = 0;statenr < ctxt->nbStates;statenr++) {
	state = ctxt->states[statenr];
	if (state != NULL)
	    state->reached = XML_REGEXP_MARK_NORMAL;
    }
    state = ctxt->states[0];
    if (state != NULL)
	state->reached = XML_REGEXP_MARK_START;
    while (state != NULL) {
	xmlRegStatePtr target = NULL;
	state->reached = XML_REGEXP_MARK_VISITED;
	for (transnr = 0;transnr < state->nbTrans;transnr++) {
	    if ((state->trans[transnr].to >= 0) &&
		((state->trans[transnr].atom != NULL) ||
		 (state->trans[transnr].count >= 0))) {
		int newto = state->trans[transnr].to;

		if (ctxt->states[newto] == NULL)
		    continue;
		if (ctxt->states[newto]->reached == XML_REGEXP_MARK_NORMAL) {
		    ctxt->states[newto]->reached = XML_REGEXP_MARK_START;
		    target = ctxt->states[newto];
		}
	    }
	}

	if (target == NULL) {
	    for (statenr = 1;statenr < ctxt->nbStates;statenr++) {
		state = ctxt->states[statenr];
		if ((state != NULL) && (state->reached ==
			XML_REGEXP_MARK_START)) {
		    target = state;
		    break;
		}
	    }
	}
	state = target;
    }
    for (statenr = 0;statenr < ctxt->nbStates;statenr++) {
	state = ctxt->states[statenr];
	if ((state != NULL) && (state->reached == XML_REGEXP_MARK_NORMAL)) {
#ifdef DEBUG_REGEXP_GRAPH
	    printf("Removed unreachable state %d\n", statenr);
#endif
	    xmlRegFreeState(state);
	    ctxt->states[statenr] = NULL;
	}
    }

}

static int
xmlFACompareRanges(xmlRegRangePtr range1, xmlRegRangePtr range2) {
    int ret = 0;

    if ((range1->type == XML_REGEXP_RANGES) ||
        (range2->type == XML_REGEXP_RANGES) ||
        (range2->type == XML_REGEXP_SUBREG) ||
        (range1->type == XML_REGEXP_SUBREG) ||
        (range1->type == XML_REGEXP_STRING) ||
        (range2->type == XML_REGEXP_STRING))
	return(-1);

    
    if (range1->type > range2->type) {
        xmlRegRangePtr tmp;

	tmp = range1;
	range1 = range2;
	range2 = tmp;
    }
    if ((range1->type == XML_REGEXP_ANYCHAR) ||
        (range2->type == XML_REGEXP_ANYCHAR)) {
	ret = 1;
    } else if ((range1->type == XML_REGEXP_EPSILON) ||
               (range2->type == XML_REGEXP_EPSILON)) {
	return(0);
    } else if (range1->type == range2->type) {
        if (range1->type != XML_REGEXP_CHARVAL)
            ret = 1;
        else if ((range1->end < range2->start) ||
	         (range2->end < range1->start))
	    ret = 0;
	else
	    ret = 1;
    } else if (range1->type == XML_REGEXP_CHARVAL) {
        int codepoint;
	int neg = 0;

	if (((range1->neg == 0) && (range2->neg != 0)) ||
	    ((range1->neg != 0) && (range2->neg == 0)))
	    neg = 1;

	for (codepoint = range1->start;codepoint <= range1->end ;codepoint++) {
	    ret = xmlRegCheckCharacterRange(range2->type, codepoint,
					    0, range2->start, range2->end,
					    range2->blockName);
	    if (ret < 0)
	        return(-1);
	    if (((neg == 1) && (ret == 0)) ||
	        ((neg == 0) && (ret == 1)))
		return(1);
	}
	return(0);
    } else if ((range1->type == XML_REGEXP_BLOCK_NAME) ||
               (range2->type == XML_REGEXP_BLOCK_NAME)) {
	if (range1->type == range2->type) {
	    ret = xmlStrEqual(range1->blockName, range2->blockName);
	} else {
	    return(1);
	}
    } else if ((range1->type < XML_REGEXP_LETTER) ||
               (range2->type < XML_REGEXP_LETTER)) {
	if ((range1->type == XML_REGEXP_ANYSPACE) &&
	    (range2->type == XML_REGEXP_NOTSPACE))
	    ret = 0;
	else if ((range1->type == XML_REGEXP_INITNAME) &&
	         (range2->type == XML_REGEXP_NOTINITNAME))
	    ret = 0;
	else if ((range1->type == XML_REGEXP_NAMECHAR) &&
	         (range2->type == XML_REGEXP_NOTNAMECHAR))
	    ret = 0;
	else if ((range1->type == XML_REGEXP_DECIMAL) &&
	         (range2->type == XML_REGEXP_NOTDECIMAL))
	    ret = 0;
	else if ((range1->type == XML_REGEXP_REALCHAR) &&
	         (range2->type == XML_REGEXP_NOTREALCHAR))
	    ret = 0;
	else {
	    
	    return(1);
	}
    } else {
        ret = 0;
        
        switch (range1->type) {
	    case XML_REGEXP_LETTER:
	         
	         if ((range2->type == XML_REGEXP_LETTER_UPPERCASE) ||
		     (range2->type == XML_REGEXP_LETTER_LOWERCASE) ||
		     (range2->type == XML_REGEXP_LETTER_TITLECASE) ||
		     (range2->type == XML_REGEXP_LETTER_MODIFIER) ||
		     (range2->type == XML_REGEXP_LETTER_OTHERS))
		     ret = 1;
		 break;
	    case XML_REGEXP_MARK:
	         if ((range2->type == XML_REGEXP_MARK_NONSPACING) ||
		     (range2->type == XML_REGEXP_MARK_SPACECOMBINING) ||
		     (range2->type == XML_REGEXP_MARK_ENCLOSING))
		     ret = 1;
		 break;
	    case XML_REGEXP_NUMBER:
	         if ((range2->type == XML_REGEXP_NUMBER_DECIMAL) ||
		     (range2->type == XML_REGEXP_NUMBER_LETTER) ||
		     (range2->type == XML_REGEXP_NUMBER_OTHERS))
		     ret = 1;
		 break;
	    case XML_REGEXP_PUNCT:
	         if ((range2->type == XML_REGEXP_PUNCT_CONNECTOR) ||
		     (range2->type == XML_REGEXP_PUNCT_DASH) ||
		     (range2->type == XML_REGEXP_PUNCT_OPEN) ||
		     (range2->type == XML_REGEXP_PUNCT_CLOSE) ||
		     (range2->type == XML_REGEXP_PUNCT_INITQUOTE) ||
		     (range2->type == XML_REGEXP_PUNCT_FINQUOTE) ||
		     (range2->type == XML_REGEXP_PUNCT_OTHERS))
		     ret = 1;
		 break;
	    case XML_REGEXP_SEPAR:
	         if ((range2->type == XML_REGEXP_SEPAR_SPACE) ||
		     (range2->type == XML_REGEXP_SEPAR_LINE) ||
		     (range2->type == XML_REGEXP_SEPAR_PARA))
		     ret = 1;
		 break;
	    case XML_REGEXP_SYMBOL:
	         if ((range2->type == XML_REGEXP_SYMBOL_MATH) ||
		     (range2->type == XML_REGEXP_SYMBOL_CURRENCY) ||
		     (range2->type == XML_REGEXP_SYMBOL_MODIFIER) ||
		     (range2->type == XML_REGEXP_SYMBOL_OTHERS))
		     ret = 1;
		 break;
	    case XML_REGEXP_OTHER:
	         if ((range2->type == XML_REGEXP_OTHER_CONTROL) ||
		     (range2->type == XML_REGEXP_OTHER_FORMAT) ||
		     (range2->type == XML_REGEXP_OTHER_PRIVATE))
		     ret = 1;
		 break;
            default:
	         if ((range2->type >= XML_REGEXP_LETTER) &&
		     (range2->type < XML_REGEXP_BLOCK_NAME))
		     ret = 0;
		 else {
		     
		     return(1);
		 }
	}
    }
    if (((range1->neg == 0) && (range2->neg != 0)) ||
        ((range1->neg != 0) && (range2->neg == 0)))
	ret = !ret;
    return(ret);
}

static int
xmlFACompareAtomTypes(xmlRegAtomType type1, xmlRegAtomType type2) {
    if ((type1 == XML_REGEXP_EPSILON) ||
        (type1 == XML_REGEXP_CHARVAL) ||
	(type1 == XML_REGEXP_RANGES) ||
	(type1 == XML_REGEXP_SUBREG) ||
	(type1 == XML_REGEXP_STRING) ||
	(type1 == XML_REGEXP_ANYCHAR))
	return(1);
    if ((type2 == XML_REGEXP_EPSILON) ||
        (type2 == XML_REGEXP_CHARVAL) ||
	(type2 == XML_REGEXP_RANGES) ||
	(type2 == XML_REGEXP_SUBREG) ||
	(type2 == XML_REGEXP_STRING) ||
	(type2 == XML_REGEXP_ANYCHAR))
	return(1);

    if (type1 == type2) return(1);

    
    if (type1 > type2) {
        xmlRegAtomType tmp = type1;
	type1 = type2;
	type2 = tmp;
    }
    switch (type1) {
        case XML_REGEXP_ANYSPACE: 
	    
	    if ((type2 == XML_REGEXP_NOTSPACE) ||
		((type2 >= XML_REGEXP_LETTER) &&
		 (type2 <= XML_REGEXP_LETTER_OTHERS)) ||
	        ((type2 >= XML_REGEXP_NUMBER) &&
		 (type2 <= XML_REGEXP_NUMBER_OTHERS)) ||
	        ((type2 >= XML_REGEXP_MARK) &&
		 (type2 <= XML_REGEXP_MARK_ENCLOSING)) ||
	        ((type2 >= XML_REGEXP_PUNCT) &&
		 (type2 <= XML_REGEXP_PUNCT_OTHERS)) ||
	        ((type2 >= XML_REGEXP_SYMBOL) &&
		 (type2 <= XML_REGEXP_SYMBOL_OTHERS))
	        ) return(0);
	    break;
        case XML_REGEXP_NOTSPACE: 
	    break;
        case XML_REGEXP_INITNAME: 
	    
	    if ((type2 == XML_REGEXP_NOTINITNAME) ||
	        ((type2 >= XML_REGEXP_NUMBER) &&
		 (type2 <= XML_REGEXP_NUMBER_OTHERS)) ||
	        ((type2 >= XML_REGEXP_MARK) &&
		 (type2 <= XML_REGEXP_MARK_ENCLOSING)) ||
	        ((type2 >= XML_REGEXP_SEPAR) &&
		 (type2 <= XML_REGEXP_SEPAR_PARA)) ||
	        ((type2 >= XML_REGEXP_PUNCT) &&
		 (type2 <= XML_REGEXP_PUNCT_OTHERS)) ||
	        ((type2 >= XML_REGEXP_SYMBOL) &&
		 (type2 <= XML_REGEXP_SYMBOL_OTHERS)) ||
	        ((type2 >= XML_REGEXP_OTHER) &&
		 (type2 <= XML_REGEXP_OTHER_NA))
		) return(0);
	    break;
        case XML_REGEXP_NOTINITNAME: 
	    break;
        case XML_REGEXP_NAMECHAR: 
	    
	    if ((type2 == XML_REGEXP_NOTNAMECHAR) ||
	        ((type2 >= XML_REGEXP_MARK) &&
		 (type2 <= XML_REGEXP_MARK_ENCLOSING)) ||
	        ((type2 >= XML_REGEXP_PUNCT) &&
		 (type2 <= XML_REGEXP_PUNCT_OTHERS)) ||
	        ((type2 >= XML_REGEXP_SEPAR) &&
		 (type2 <= XML_REGEXP_SEPAR_PARA)) ||
	        ((type2 >= XML_REGEXP_SYMBOL) &&
		 (type2 <= XML_REGEXP_SYMBOL_OTHERS)) ||
	        ((type2 >= XML_REGEXP_OTHER) &&
		 (type2 <= XML_REGEXP_OTHER_NA))
		) return(0);
	    break;
        case XML_REGEXP_NOTNAMECHAR: 
	    break;
        case XML_REGEXP_DECIMAL: 
	    
	    if ((type2 == XML_REGEXP_NOTDECIMAL) ||
	        (type2 == XML_REGEXP_REALCHAR) ||
		((type2 >= XML_REGEXP_LETTER) &&
		 (type2 <= XML_REGEXP_LETTER_OTHERS)) ||
	        ((type2 >= XML_REGEXP_MARK) &&
		 (type2 <= XML_REGEXP_MARK_ENCLOSING)) ||
	        ((type2 >= XML_REGEXP_PUNCT) &&
		 (type2 <= XML_REGEXP_PUNCT_OTHERS)) ||
	        ((type2 >= XML_REGEXP_SEPAR) &&
		 (type2 <= XML_REGEXP_SEPAR_PARA)) ||
	        ((type2 >= XML_REGEXP_SYMBOL) &&
		 (type2 <= XML_REGEXP_SYMBOL_OTHERS)) ||
	        ((type2 >= XML_REGEXP_OTHER) &&
		 (type2 <= XML_REGEXP_OTHER_NA))
		)return(0);
	    break;
        case XML_REGEXP_NOTDECIMAL: 
	    break;
        case XML_REGEXP_REALCHAR: 
	    
	    if ((type2 == XML_REGEXP_NOTDECIMAL) ||
	        ((type2 >= XML_REGEXP_MARK) &&
		 (type2 <= XML_REGEXP_MARK_ENCLOSING)) ||
	        ((type2 >= XML_REGEXP_PUNCT) &&
		 (type2 <= XML_REGEXP_PUNCT_OTHERS)) ||
	        ((type2 >= XML_REGEXP_SEPAR) &&
		 (type2 <= XML_REGEXP_SEPAR_PARA)) ||
	        ((type2 >= XML_REGEXP_SYMBOL) &&
		 (type2 <= XML_REGEXP_SYMBOL_OTHERS)) ||
	        ((type2 >= XML_REGEXP_OTHER) &&
		 (type2 <= XML_REGEXP_OTHER_NA))
		)return(0);
	    break;
        case XML_REGEXP_NOTREALCHAR: 
	    break;
        case XML_REGEXP_LETTER:
	    if (type2 <= XML_REGEXP_LETTER_OTHERS)
	        return(1);
	    return(0);
        case XML_REGEXP_LETTER_UPPERCASE:
        case XML_REGEXP_LETTER_LOWERCASE:
        case XML_REGEXP_LETTER_TITLECASE:
        case XML_REGEXP_LETTER_MODIFIER:
        case XML_REGEXP_LETTER_OTHERS:
	    return(0);
        case XML_REGEXP_MARK:
	    if (type2 <= XML_REGEXP_MARK_ENCLOSING)
	        return(1);
	    return(0);
        case XML_REGEXP_MARK_NONSPACING:
        case XML_REGEXP_MARK_SPACECOMBINING:
        case XML_REGEXP_MARK_ENCLOSING:
	    return(0);
        case XML_REGEXP_NUMBER:
	    if (type2 <= XML_REGEXP_NUMBER_OTHERS)
	        return(1);
	    return(0);
        case XML_REGEXP_NUMBER_DECIMAL:
        case XML_REGEXP_NUMBER_LETTER:
        case XML_REGEXP_NUMBER_OTHERS:
	    return(0);
        case XML_REGEXP_PUNCT:
	    if (type2 <= XML_REGEXP_PUNCT_OTHERS)
	        return(1);
	    return(0);
        case XML_REGEXP_PUNCT_CONNECTOR:
        case XML_REGEXP_PUNCT_DASH:
        case XML_REGEXP_PUNCT_OPEN:
        case XML_REGEXP_PUNCT_CLOSE:
        case XML_REGEXP_PUNCT_INITQUOTE:
        case XML_REGEXP_PUNCT_FINQUOTE:
        case XML_REGEXP_PUNCT_OTHERS:
	    return(0);
        case XML_REGEXP_SEPAR:
	    if (type2 <= XML_REGEXP_SEPAR_PARA)
	        return(1);
	    return(0);
        case XML_REGEXP_SEPAR_SPACE:
        case XML_REGEXP_SEPAR_LINE:
        case XML_REGEXP_SEPAR_PARA:
	    return(0);
        case XML_REGEXP_SYMBOL:
	    if (type2 <= XML_REGEXP_SYMBOL_OTHERS)
	        return(1);
	    return(0);
        case XML_REGEXP_SYMBOL_MATH:
        case XML_REGEXP_SYMBOL_CURRENCY:
        case XML_REGEXP_SYMBOL_MODIFIER:
        case XML_REGEXP_SYMBOL_OTHERS:
	    return(0);
        case XML_REGEXP_OTHER:
	    if (type2 <= XML_REGEXP_OTHER_NA)
	        return(1);
	    return(0);
        case XML_REGEXP_OTHER_CONTROL:
        case XML_REGEXP_OTHER_FORMAT:
        case XML_REGEXP_OTHER_PRIVATE:
        case XML_REGEXP_OTHER_NA:
	    return(0);
	default:
	    break;
    }
    return(1);
}

static int
xmlFAEqualAtoms(xmlRegAtomPtr atom1, xmlRegAtomPtr atom2, int deep) {
    int ret = 0;

    if (atom1 == atom2)
	return(1);
    if ((atom1 == NULL) || (atom2 == NULL))
	return(0);

    if (atom1->type != atom2->type)
        return(0);
    switch (atom1->type) {
        case XML_REGEXP_EPSILON:
	    ret = 0;
	    break;
        case XML_REGEXP_STRING:
            if (!deep)
                ret = (atom1->valuep == atom2->valuep);
            else
                ret = xmlStrEqual((xmlChar *)atom1->valuep,
                                  (xmlChar *)atom2->valuep);
	    break;
        case XML_REGEXP_CHARVAL:
	    ret = (atom1->codepoint == atom2->codepoint);
	    break;
	case XML_REGEXP_RANGES:
	    
	    ret = 0;
	default:
	    break;
    }
    return(ret);
}

static int
xmlFACompareAtoms(xmlRegAtomPtr atom1, xmlRegAtomPtr atom2, int deep) {
    int ret = 1;

    if (atom1 == atom2)
	return(1);
    if ((atom1 == NULL) || (atom2 == NULL))
	return(0);

    if ((atom1->type == XML_REGEXP_ANYCHAR) ||
        (atom2->type == XML_REGEXP_ANYCHAR))
	return(1);

    if (atom1->type > atom2->type) {
	xmlRegAtomPtr tmp;
	tmp = atom1;
	atom1 = atom2;
	atom2 = tmp;
    }
    if (atom1->type != atom2->type) {
        ret = xmlFACompareAtomTypes(atom1->type, atom2->type);
	
	if (ret == 0)
	    return(0);
    }
    switch (atom1->type) {
        case XML_REGEXP_STRING:
            if (!deep)
                ret = (atom1->valuep != atom2->valuep);
            else
                ret = xmlRegStrEqualWildcard((xmlChar *)atom1->valuep,
                                             (xmlChar *)atom2->valuep);
	    break;
        case XML_REGEXP_EPSILON:
	    goto not_determinist;
        case XML_REGEXP_CHARVAL:
	    if (atom2->type == XML_REGEXP_CHARVAL) {
		ret = (atom1->codepoint == atom2->codepoint);
	    } else {
	        ret = xmlRegCheckCharacter(atom2, atom1->codepoint);
		if (ret < 0)
		    ret = 1;
	    }
	    break;
        case XML_REGEXP_RANGES:
	    if (atom2->type == XML_REGEXP_RANGES) {
	        int i, j, res;
		xmlRegRangePtr r1, r2;

		for (i = 0;i < atom1->nbRanges;i++) {
		    for (j = 0;j < atom2->nbRanges;j++) {
			r1 = atom1->ranges[i];
			r2 = atom2->ranges[j];
			res = xmlFACompareRanges(r1, r2);
			if (res == 1) {
			    ret = 1;
			    goto done;
			}
		    }
		}
		ret = 0;
	    }
	    break;
	default:
	    goto not_determinist;
    }
done:
    if (atom1->neg != atom2->neg) {
        ret = !ret;
    }
    if (ret == 0)
        return(0);
not_determinist:
    return(1);
}

static int
xmlFARecurseDeterminism(xmlRegParserCtxtPtr ctxt, xmlRegStatePtr state,
	                 int to, xmlRegAtomPtr atom) {
    int ret = 1;
    int res;
    int transnr, nbTrans;
    xmlRegTransPtr t1;
    int deep = 1;

    if (state == NULL)
	return(ret);

    if (ctxt->flags & AM_AUTOMATA_RNG)
        deep = 0;

    nbTrans = state->nbTrans;
    for (transnr = 0;transnr < nbTrans;transnr++) {
	t1 = &(state->trans[transnr]);
	if (t1->atom == NULL) {
	    if (t1->to < 0)
		continue;
	    res = xmlFARecurseDeterminism(ctxt, ctxt->states[t1->to],
		                           to, atom);
	    if (res == 0) {
	        ret = 0;
		
	    }
	    continue;
	}
	if (t1->to != to)
	    continue;
	if (xmlFACompareAtoms(t1->atom, atom, deep)) {
	    ret = 0;
	    
	    t1->nd = 1;
	}
    }
    return(ret);
}

static int
xmlFAComputesDeterminism(xmlRegParserCtxtPtr ctxt) {
    int statenr, transnr;
    xmlRegStatePtr state;
    xmlRegTransPtr t1, t2, last;
    int i;
    int ret = 1;
    int deep = 1;

#ifdef DEBUG_REGEXP_GRAPH
    printf("xmlFAComputesDeterminism\n");
    xmlRegPrintCtxt(stdout, ctxt);
#endif
    if (ctxt->determinist != -1)
	return(ctxt->determinist);

    if (ctxt->flags & AM_AUTOMATA_RNG)
        deep = 0;

    for (statenr = 0;statenr < ctxt->nbStates;statenr++) {
	state = ctxt->states[statenr];
	if (state == NULL)
	    continue;
	if (state->nbTrans < 2)
	    continue;
	for (transnr = 0;transnr < state->nbTrans;transnr++) {
	    t1 = &(state->trans[transnr]);
	    if (t1->atom == NULL) {
		
		continue;
	    }
	    if (t1->to == -1) 
		continue;
	    for (i = 0;i < transnr;i++) {
		t2 = &(state->trans[i]);
		if (t2->to == -1) 
		    continue;
		if (t2->atom != NULL) {
		    if (t1->to == t2->to) {
			if (xmlFAEqualAtoms(t1->atom, t2->atom, deep) &&
                            (t1->counter == t2->counter) &&
                            (t1->count == t2->count))
			    t2->to = -1; 
		    }
		}
	    }
	}
    }

    for (statenr = 0;statenr < ctxt->nbStates;statenr++) {
	state = ctxt->states[statenr];
	if (state == NULL)
	    continue;
	if (state->nbTrans < 2)
	    continue;
	last = NULL;
	for (transnr = 0;transnr < state->nbTrans;transnr++) {
	    t1 = &(state->trans[transnr]);
	    if (t1->atom == NULL) {
		continue;
	    }
	    if (t1->to == -1) 
		continue;
	    for (i = 0;i < transnr;i++) {
		t2 = &(state->trans[i]);
		if (t2->to == -1) 
		    continue;
		if (t2->atom != NULL) {
		    if (xmlFACompareAtoms(t1->atom, t2->atom, 1)) {
			ret = 0;
			
			t1->nd = 1;
			t2->nd = 1;
			last = t1;
		    }
		} else if (t1->to != -1) {
		    ret = xmlFARecurseDeterminism(ctxt, ctxt->states[t1->to],
						   t2->to, t2->atom);
		    if (ret == 0) {
			t1->nd = 1;
			
			last = t1;
		    }
		}
	    }
	}

	if (last != NULL) {
	    last->nd = 2;
	}

    }

    ctxt->determinist = ret;
    return(ret);
}


static int
xmlRegCheckCharacterRange(xmlRegAtomType type, int codepoint, int neg,
	                  int start, int end, const xmlChar *blockName) {
    int ret = 0;

    switch (type) {
        case XML_REGEXP_STRING:
        case XML_REGEXP_SUBREG:
        case XML_REGEXP_RANGES:
        case XML_REGEXP_EPSILON:
	    return(-1);
        case XML_REGEXP_ANYCHAR:
	    ret = ((codepoint != '\n') && (codepoint != '\r'));
	    break;
        case XML_REGEXP_CHARVAL:
	    ret = ((codepoint >= start) && (codepoint <= end));
	    break;
        case XML_REGEXP_NOTSPACE:
	    neg = !neg;
        case XML_REGEXP_ANYSPACE:
	    ret = ((codepoint == '\n') || (codepoint == '\r') ||
		   (codepoint == '\t') || (codepoint == ' '));
	    break;
        case XML_REGEXP_NOTINITNAME:
	    neg = !neg;
        case XML_REGEXP_INITNAME:
	    ret = (IS_LETTER(codepoint) || 
		   (codepoint == '_') || (codepoint == ':'));
	    break;
        case XML_REGEXP_NOTNAMECHAR:
	    neg = !neg;
        case XML_REGEXP_NAMECHAR:
	    ret = (IS_LETTER(codepoint) || IS_DIGIT(codepoint) ||
		   (codepoint == '.') || (codepoint == '-') ||
		   (codepoint == '_') || (codepoint == ':') ||
		   IS_COMBINING(codepoint) || IS_EXTENDER(codepoint));
	    break;
        case XML_REGEXP_NOTDECIMAL:
	    neg = !neg;
        case XML_REGEXP_DECIMAL:
	    ret = xmlUCSIsCatNd(codepoint);
	    break;
        case XML_REGEXP_REALCHAR:
	    neg = !neg;
        case XML_REGEXP_NOTREALCHAR:
	    ret = xmlUCSIsCatP(codepoint);
	    if (ret == 0)
		ret = xmlUCSIsCatZ(codepoint);
	    if (ret == 0)
		ret = xmlUCSIsCatC(codepoint);
	    break;
        case XML_REGEXP_LETTER:
	    ret = xmlUCSIsCatL(codepoint);
	    break;
        case XML_REGEXP_LETTER_UPPERCASE:
	    ret = xmlUCSIsCatLu(codepoint);
	    break;
        case XML_REGEXP_LETTER_LOWERCASE:
	    ret = xmlUCSIsCatLl(codepoint);
	    break;
        case XML_REGEXP_LETTER_TITLECASE:
	    ret = xmlUCSIsCatLt(codepoint);
	    break;
        case XML_REGEXP_LETTER_MODIFIER:
	    ret = xmlUCSIsCatLm(codepoint);
	    break;
        case XML_REGEXP_LETTER_OTHERS:
	    ret = xmlUCSIsCatLo(codepoint);
	    break;
        case XML_REGEXP_MARK:
	    ret = xmlUCSIsCatM(codepoint);
	    break;
        case XML_REGEXP_MARK_NONSPACING:
	    ret = xmlUCSIsCatMn(codepoint);
	    break;
        case XML_REGEXP_MARK_SPACECOMBINING:
	    ret = xmlUCSIsCatMc(codepoint);
	    break;
        case XML_REGEXP_MARK_ENCLOSING:
	    ret = xmlUCSIsCatMe(codepoint);
	    break;
        case XML_REGEXP_NUMBER:
	    ret = xmlUCSIsCatN(codepoint);
	    break;
        case XML_REGEXP_NUMBER_DECIMAL:
	    ret = xmlUCSIsCatNd(codepoint);
	    break;
        case XML_REGEXP_NUMBER_LETTER:
	    ret = xmlUCSIsCatNl(codepoint);
	    break;
        case XML_REGEXP_NUMBER_OTHERS:
	    ret = xmlUCSIsCatNo(codepoint);
	    break;
        case XML_REGEXP_PUNCT:
	    ret = xmlUCSIsCatP(codepoint);
	    break;
        case XML_REGEXP_PUNCT_CONNECTOR:
	    ret = xmlUCSIsCatPc(codepoint);
	    break;
        case XML_REGEXP_PUNCT_DASH:
	    ret = xmlUCSIsCatPd(codepoint);
	    break;
        case XML_REGEXP_PUNCT_OPEN:
	    ret = xmlUCSIsCatPs(codepoint);
	    break;
        case XML_REGEXP_PUNCT_CLOSE:
	    ret = xmlUCSIsCatPe(codepoint);
	    break;
        case XML_REGEXP_PUNCT_INITQUOTE:
	    ret = xmlUCSIsCatPi(codepoint);
	    break;
        case XML_REGEXP_PUNCT_FINQUOTE:
	    ret = xmlUCSIsCatPf(codepoint);
	    break;
        case XML_REGEXP_PUNCT_OTHERS:
	    ret = xmlUCSIsCatPo(codepoint);
	    break;
        case XML_REGEXP_SEPAR:
	    ret = xmlUCSIsCatZ(codepoint);
	    break;
        case XML_REGEXP_SEPAR_SPACE:
	    ret = xmlUCSIsCatZs(codepoint);
	    break;
        case XML_REGEXP_SEPAR_LINE:
	    ret = xmlUCSIsCatZl(codepoint);
	    break;
        case XML_REGEXP_SEPAR_PARA:
	    ret = xmlUCSIsCatZp(codepoint);
	    break;
        case XML_REGEXP_SYMBOL:
	    ret = xmlUCSIsCatS(codepoint);
	    break;
        case XML_REGEXP_SYMBOL_MATH:
	    ret = xmlUCSIsCatSm(codepoint);
	    break;
        case XML_REGEXP_SYMBOL_CURRENCY:
	    ret = xmlUCSIsCatSc(codepoint);
	    break;
        case XML_REGEXP_SYMBOL_MODIFIER:
	    ret = xmlUCSIsCatSk(codepoint);
	    break;
        case XML_REGEXP_SYMBOL_OTHERS:
	    ret = xmlUCSIsCatSo(codepoint);
	    break;
        case XML_REGEXP_OTHER:
	    ret = xmlUCSIsCatC(codepoint);
	    break;
        case XML_REGEXP_OTHER_CONTROL:
	    ret = xmlUCSIsCatCc(codepoint);
	    break;
        case XML_REGEXP_OTHER_FORMAT:
	    ret = xmlUCSIsCatCf(codepoint);
	    break;
        case XML_REGEXP_OTHER_PRIVATE:
	    ret = xmlUCSIsCatCo(codepoint);
	    break;
        case XML_REGEXP_OTHER_NA:
	    
	    
	    ret = 0;
	    break;
        case XML_REGEXP_BLOCK_NAME:
	    ret = xmlUCSIsBlock(codepoint, (const char *) blockName);
	    break;
    }
    if (neg)
	return(!ret);
    return(ret);
}

static int
xmlRegCheckCharacter(xmlRegAtomPtr atom, int codepoint) {
    int i, ret = 0;
    xmlRegRangePtr range;

    if ((atom == NULL) || (!IS_CHAR(codepoint)))
	return(-1);

    switch (atom->type) {
        case XML_REGEXP_SUBREG:
        case XML_REGEXP_EPSILON:
	    return(-1);
        case XML_REGEXP_CHARVAL:
            return(codepoint == atom->codepoint);
        case XML_REGEXP_RANGES: {
	    int accept = 0;

	    for (i = 0;i < atom->nbRanges;i++) {
		range = atom->ranges[i];
		if (range->neg == 2) {
		    ret = xmlRegCheckCharacterRange(range->type, codepoint,
						0, range->start, range->end,
						range->blockName);
		    if (ret != 0)
			return(0); 
		} else if (range->neg) {
		    ret = xmlRegCheckCharacterRange(range->type, codepoint,
						0, range->start, range->end,
						range->blockName);
		    if (ret == 0)
		        accept = 1;
		    else
		        return(0);
		} else {
		    ret = xmlRegCheckCharacterRange(range->type, codepoint,
						0, range->start, range->end,
						range->blockName);
		    if (ret != 0)
			accept = 1; 
		}
	    }
	    return(accept);
	}
        case XML_REGEXP_STRING:
	    printf("TODO: XML_REGEXP_STRING\n");
	    return(-1);
        case XML_REGEXP_ANYCHAR:
        case XML_REGEXP_ANYSPACE:
        case XML_REGEXP_NOTSPACE:
        case XML_REGEXP_INITNAME:
        case XML_REGEXP_NOTINITNAME:
        case XML_REGEXP_NAMECHAR:
        case XML_REGEXP_NOTNAMECHAR:
        case XML_REGEXP_DECIMAL:
        case XML_REGEXP_NOTDECIMAL:
        case XML_REGEXP_REALCHAR:
        case XML_REGEXP_NOTREALCHAR:
        case XML_REGEXP_LETTER:
        case XML_REGEXP_LETTER_UPPERCASE:
        case XML_REGEXP_LETTER_LOWERCASE:
        case XML_REGEXP_LETTER_TITLECASE:
        case XML_REGEXP_LETTER_MODIFIER:
        case XML_REGEXP_LETTER_OTHERS:
        case XML_REGEXP_MARK:
        case XML_REGEXP_MARK_NONSPACING:
        case XML_REGEXP_MARK_SPACECOMBINING:
        case XML_REGEXP_MARK_ENCLOSING:
        case XML_REGEXP_NUMBER:
        case XML_REGEXP_NUMBER_DECIMAL:
        case XML_REGEXP_NUMBER_LETTER:
        case XML_REGEXP_NUMBER_OTHERS:
        case XML_REGEXP_PUNCT:
        case XML_REGEXP_PUNCT_CONNECTOR:
        case XML_REGEXP_PUNCT_DASH:
        case XML_REGEXP_PUNCT_OPEN:
        case XML_REGEXP_PUNCT_CLOSE:
        case XML_REGEXP_PUNCT_INITQUOTE:
        case XML_REGEXP_PUNCT_FINQUOTE:
        case XML_REGEXP_PUNCT_OTHERS:
        case XML_REGEXP_SEPAR:
        case XML_REGEXP_SEPAR_SPACE:
        case XML_REGEXP_SEPAR_LINE:
        case XML_REGEXP_SEPAR_PARA:
        case XML_REGEXP_SYMBOL:
        case XML_REGEXP_SYMBOL_MATH:
        case XML_REGEXP_SYMBOL_CURRENCY:
        case XML_REGEXP_SYMBOL_MODIFIER:
        case XML_REGEXP_SYMBOL_OTHERS:
        case XML_REGEXP_OTHER:
        case XML_REGEXP_OTHER_CONTROL:
        case XML_REGEXP_OTHER_FORMAT:
        case XML_REGEXP_OTHER_PRIVATE:
        case XML_REGEXP_OTHER_NA:
	case XML_REGEXP_BLOCK_NAME:
	    ret = xmlRegCheckCharacterRange(atom->type, codepoint, 0, 0, 0,
		                            (const xmlChar *)atom->valuep);
	    if (atom->neg)
		ret = !ret;
	    break;
    }
    return(ret);
}


#ifdef DEBUG_REGEXP_EXEC
static void
xmlFARegDebugExec(xmlRegExecCtxtPtr exec) {
    printf("state: %d:%d:idx %d", exec->state->no, exec->transno, exec->index);
    if (exec->inputStack != NULL) {
	int i;
	printf(": ");
	for (i = 0;(i < 3) && (i < exec->inputStackNr);i++)
	    printf("%s ", (const char *)
	           exec->inputStack[exec->inputStackNr - (i + 1)].value);
    } else {
	printf(": %s", &(exec->inputString[exec->index]));
    }
    printf("\n");
}
#endif

static void
xmlFARegExecSave(xmlRegExecCtxtPtr exec) {
#ifdef DEBUG_REGEXP_EXEC
    printf("saving ");
    exec->transno++;
    xmlFARegDebugExec(exec);
    exec->transno--;
#endif
#ifdef MAX_PUSH
    if (exec->nbPush > MAX_PUSH) {
        return;
    }
    exec->nbPush++;
#endif

    if (exec->maxRollbacks == 0) {
	exec->maxRollbacks = 4;
	exec->rollbacks = (xmlRegExecRollback *) xmlMalloc(exec->maxRollbacks *
		                             sizeof(xmlRegExecRollback));
	if (exec->rollbacks == NULL) {
	    xmlRegexpErrMemory(NULL, "saving regexp");
	    exec->maxRollbacks = 0;
	    return;
	}
	memset(exec->rollbacks, 0,
	       exec->maxRollbacks * sizeof(xmlRegExecRollback));
    } else if (exec->nbRollbacks >= exec->maxRollbacks) {
	xmlRegExecRollback *tmp;
	int len = exec->maxRollbacks;

	exec->maxRollbacks *= 2;
	tmp = (xmlRegExecRollback *) xmlRealloc(exec->rollbacks,
			exec->maxRollbacks * sizeof(xmlRegExecRollback));
	if (tmp == NULL) {
	    xmlRegexpErrMemory(NULL, "saving regexp");
	    exec->maxRollbacks /= 2;
	    return;
	}
	exec->rollbacks = tmp;
	tmp = &exec->rollbacks[len];
	memset(tmp, 0, (exec->maxRollbacks - len) * sizeof(xmlRegExecRollback));
    }
    exec->rollbacks[exec->nbRollbacks].state = exec->state;
    exec->rollbacks[exec->nbRollbacks].index = exec->index;
    exec->rollbacks[exec->nbRollbacks].nextbranch = exec->transno + 1;
    if (exec->comp->nbCounters > 0) {
	if (exec->rollbacks[exec->nbRollbacks].counts == NULL) {
	    exec->rollbacks[exec->nbRollbacks].counts = (int *)
		xmlMalloc(exec->comp->nbCounters * sizeof(int));
	    if (exec->rollbacks[exec->nbRollbacks].counts == NULL) {
		xmlRegexpErrMemory(NULL, "saving regexp");
		exec->status = -5;
		return;
	    }
	}
	memcpy(exec->rollbacks[exec->nbRollbacks].counts, exec->counts,
	       exec->comp->nbCounters * sizeof(int));
    }
    exec->nbRollbacks++;
}

static void
xmlFARegExecRollBack(xmlRegExecCtxtPtr exec) {
    if (exec->nbRollbacks <= 0) {
	exec->status = -1;
#ifdef DEBUG_REGEXP_EXEC
	printf("rollback failed on empty stack\n");
#endif
	return;
    }
    exec->nbRollbacks--;
    exec->state = exec->rollbacks[exec->nbRollbacks].state;
    exec->index = exec->rollbacks[exec->nbRollbacks].index;
    exec->transno = exec->rollbacks[exec->nbRollbacks].nextbranch;
    if (exec->comp->nbCounters > 0) {
	if (exec->rollbacks[exec->nbRollbacks].counts == NULL) {
	    fprintf(stderr, "exec save: allocation failed");
	    exec->status = -6;
	    return;
	}
	memcpy(exec->counts, exec->rollbacks[exec->nbRollbacks].counts,
	       exec->comp->nbCounters * sizeof(int));
    }

#ifdef DEBUG_REGEXP_EXEC
    printf("restored ");
    xmlFARegDebugExec(exec);
#endif
}


static int
xmlFARegExec(xmlRegexpPtr comp, const xmlChar *content) {
    xmlRegExecCtxt execval;
    xmlRegExecCtxtPtr exec = &execval;
    int ret, codepoint = 0, len, deter;

    exec->inputString = content;
    exec->index = 0;
    exec->nbPush = 0;
    exec->determinist = 1;
    exec->maxRollbacks = 0;
    exec->nbRollbacks = 0;
    exec->rollbacks = NULL;
    exec->status = 0;
    exec->comp = comp;
    exec->state = comp->states[0];
    exec->transno = 0;
    exec->transcount = 0;
    exec->inputStack = NULL;
    exec->inputStackMax = 0;
    if (comp->nbCounters > 0) {
	exec->counts = (int *) xmlMalloc(comp->nbCounters * sizeof(int));
	if (exec->counts == NULL) {
	    xmlRegexpErrMemory(NULL, "running regexp");
	    return(-1);
	}
        memset(exec->counts, 0, comp->nbCounters * sizeof(int));
    } else
	exec->counts = NULL;
    while ((exec->status == 0) &&
	   ((exec->inputString[exec->index] != 0) ||
	    ((exec->state != NULL) &&
	     (exec->state->type != XML_REGEXP_FINAL_STATE)))) {
	xmlRegTransPtr trans;
	xmlRegAtomPtr atom;

	len = 1;
	if ((exec->inputString[exec->index] == 0) && (exec->counts == NULL)) {
	    if (exec->transno < exec->state->nbTrans) {
	        trans = &exec->state->trans[exec->transno];
		if (trans->to >=0) {
		    atom = trans->atom;
		    if (!((atom->min == 0) && (atom->max > 0)))
		        goto rollback;
		}
	    } else
	        goto rollback;
	}

	exec->transcount = 0;
	for (;exec->transno < exec->state->nbTrans;exec->transno++) {
	    trans = &exec->state->trans[exec->transno];
	    if (trans->to < 0)
		continue;
	    atom = trans->atom;
	    ret = 0;
	    deter = 1;
	    if (trans->count >= 0) {
		int count;
		xmlRegCounterPtr counter;

		if (exec->counts == NULL) {
		    exec->status = -1;
		    goto error;
		}

		count = exec->counts[trans->count];
		counter = &exec->comp->counters[trans->count];
#ifdef DEBUG_REGEXP_EXEC
		printf("testing count %d: val %d, min %d, max %d\n",
		       trans->count, count, counter->min,  counter->max);
#endif
		ret = ((count >= counter->min) && (count <= counter->max));
		if ((ret) && (counter->min != counter->max))
		    deter = 0;
	    } else if (atom == NULL) {
		fprintf(stderr, "epsilon transition left at runtime\n");
		exec->status = -2;
		break;
	    } else if (exec->inputString[exec->index] != 0) {
                codepoint = CUR_SCHAR(&(exec->inputString[exec->index]), len);
		ret = xmlRegCheckCharacter(atom, codepoint);
		if ((ret == 1) && (atom->min >= 0) && (atom->max > 0)) {
		    xmlRegStatePtr to = comp->states[trans->to];

		    if (trans->counter >= 0) {
			xmlRegCounterPtr counter;

			if ((exec->counts == NULL) ||
			    (exec->comp == NULL) ||
			    (exec->comp->counters == NULL)) {
			    exec->status = -1;
			    goto error;
			}
			counter = &exec->comp->counters[trans->counter];
			if (exec->counts[trans->counter] >= counter->max)
			    continue; 

#ifdef DEBUG_REGEXP_EXEC
			printf("Increasing count %d\n", trans->counter);
#endif
			exec->counts[trans->counter]++;
		    }
		    if (exec->state->nbTrans > exec->transno + 1) {
			xmlFARegExecSave(exec);
		    }
		    exec->transcount = 1;
		    do {
			if (exec->transcount == atom->max) {
			    break;
			}
			exec->index += len;
			if (exec->inputString[exec->index] == 0) {
			    exec->index -= len;
			    break;
			}
			if (exec->transcount >= atom->min) {
			    int transno = exec->transno;
			    xmlRegStatePtr state = exec->state;

			    exec->transno = -1; 
			    exec->state = to;
			    xmlFARegExecSave(exec);
			    exec->transno = transno;
			    exec->state = state;
			}
			codepoint = CUR_SCHAR(&(exec->inputString[exec->index]),
				              len);
			ret = xmlRegCheckCharacter(atom, codepoint);
			exec->transcount++;
		    } while (ret == 1);
		    if (exec->transcount < atom->min)
			ret = 0;

		    if (ret < 0)
			ret = 0;
		    if (ret == 0) {
			goto rollback;
		    }
		    if (trans->counter >= 0) {
			if (exec->counts == NULL) {
			    exec->status = -1;
			    goto error;
			}
#ifdef DEBUG_REGEXP_EXEC
			printf("Decreasing count %d\n", trans->counter);
#endif
			exec->counts[trans->counter]--;
		    }
		} else if ((ret == 0) && (atom->min == 0) && (atom->max > 0)) {
		    exec->transcount = 1;
		    len = 0;
		    ret = 1;
		}
	    } else if ((atom->min == 0) && (atom->max > 0)) {
	        
		exec->transcount = 1;
		len = 0;
		ret = 1;
	    }
	    if (ret == 1) {
		if ((trans->nd == 1) ||
		    ((trans->count >= 0) && (deter == 0) &&
		     (exec->state->nbTrans > exec->transno + 1))) {
#ifdef DEBUG_REGEXP_EXEC
		    if (trans->nd == 1)
		        printf("Saving on nd transition atom %d for %c at %d\n",
			       trans->atom->no, codepoint, exec->index);
		    else
		        printf("Saving on counted transition count %d for %c at %d\n",
			       trans->count, codepoint, exec->index);
#endif
		    xmlFARegExecSave(exec);
		}
		if (trans->counter >= 0) {
		    xmlRegCounterPtr counter;

                    
		    if ((exec->counts == NULL) ||
			(exec->comp == NULL) ||
			(exec->comp->counters == NULL)) {
			exec->status = -1;
			goto error;
		    }
		    counter = &exec->comp->counters[trans->counter];
		    if (exec->counts[trans->counter] >= counter->max)
			continue; 
#ifdef DEBUG_REGEXP_EXEC
		    printf("Increasing count %d\n", trans->counter);
#endif
		    exec->counts[trans->counter]++;
		}
		if ((trans->count >= 0) &&
		    (trans->count < REGEXP_ALL_COUNTER)) {
		    if (exec->counts == NULL) {
		        exec->status = -1;
			goto error;
		    }
#ifdef DEBUG_REGEXP_EXEC
		    printf("resetting count %d on transition\n",
		           trans->count);
#endif
		    exec->counts[trans->count] = 0;
		}
#ifdef DEBUG_REGEXP_EXEC
		printf("entering state %d\n", trans->to);
#endif
		exec->state = comp->states[trans->to];
		exec->transno = 0;
		if (trans->atom != NULL) {
		    exec->index += len;
		}
		goto progress;
	    } else if (ret < 0) {
		exec->status = -4;
		break;
	    }
	}
	if ((exec->transno != 0) || (exec->state->nbTrans == 0)) {
rollback:
	    exec->determinist = 0;
#ifdef DEBUG_REGEXP_EXEC
	    printf("rollback from state %d on %d:%c\n", exec->state->no,
	           codepoint,codepoint);
#endif
	    xmlFARegExecRollBack(exec);
	}
progress:
	continue;
    }
error:
    if (exec->rollbacks != NULL) {
	if (exec->counts != NULL) {
	    int i;

	    for (i = 0;i < exec->maxRollbacks;i++)
		if (exec->rollbacks[i].counts != NULL)
		    xmlFree(exec->rollbacks[i].counts);
	}
	xmlFree(exec->rollbacks);
    }
    if (exec->counts != NULL)
	xmlFree(exec->counts);
    if (exec->status == 0)
	return(1);
    if (exec->status == -1) {
	if (exec->nbPush > MAX_PUSH)
	    return(-1);
	return(0);
    }
    return(exec->status);
}

#ifdef DEBUG_ERR
static void testerr(xmlRegExecCtxtPtr exec);
#endif

xmlRegExecCtxtPtr
xmlRegNewExecCtxt(xmlRegexpPtr comp, xmlRegExecCallbacks callback, void *data) {
    xmlRegExecCtxtPtr exec;

    if (comp == NULL)
	return(NULL);
    if ((comp->compact == NULL) && (comp->states == NULL))
        return(NULL);
    exec = (xmlRegExecCtxtPtr) xmlMalloc(sizeof(xmlRegExecCtxt));
    if (exec == NULL) {
	xmlRegexpErrMemory(NULL, "creating execution context");
	return(NULL);
    }
    memset(exec, 0, sizeof(xmlRegExecCtxt));
    exec->inputString = NULL;
    exec->index = 0;
    exec->determinist = 1;
    exec->maxRollbacks = 0;
    exec->nbRollbacks = 0;
    exec->rollbacks = NULL;
    exec->status = 0;
    exec->comp = comp;
    if (comp->compact == NULL)
	exec->state = comp->states[0];
    exec->transno = 0;
    exec->transcount = 0;
    exec->callback = callback;
    exec->data = data;
    if (comp->nbCounters > 0) {
	exec->counts = (int *) xmlMalloc(comp->nbCounters * sizeof(int)
	                                 * 2);
	if (exec->counts == NULL) {
	    xmlRegexpErrMemory(NULL, "creating execution context");
	    xmlFree(exec);
	    return(NULL);
	}
        memset(exec->counts, 0, comp->nbCounters * sizeof(int) * 2);
	exec->errCounts = &exec->counts[comp->nbCounters];
    } else {
	exec->counts = NULL;
	exec->errCounts = NULL;
    }
    exec->inputStackMax = 0;
    exec->inputStackNr = 0;
    exec->inputStack = NULL;
    exec->errStateNo = -1;
    exec->errString = NULL;
    exec->nbPush = 0;
    return(exec);
}

void
xmlRegFreeExecCtxt(xmlRegExecCtxtPtr exec) {
    if (exec == NULL)
	return;

    if (exec->rollbacks != NULL) {
	if (exec->counts != NULL) {
	    int i;

	    for (i = 0;i < exec->maxRollbacks;i++)
		if (exec->rollbacks[i].counts != NULL)
		    xmlFree(exec->rollbacks[i].counts);
	}
	xmlFree(exec->rollbacks);
    }
    if (exec->counts != NULL)
	xmlFree(exec->counts);
    if (exec->inputStack != NULL) {
	int i;

	for (i = 0;i < exec->inputStackNr;i++) {
	    if (exec->inputStack[i].value != NULL)
		xmlFree(exec->inputStack[i].value);
	}
	xmlFree(exec->inputStack);
    }
    if (exec->errString != NULL)
        xmlFree(exec->errString);
    xmlFree(exec);
}

static void
xmlFARegExecSaveInputString(xmlRegExecCtxtPtr exec, const xmlChar *value,
	                    void *data) {
#ifdef DEBUG_PUSH
    printf("saving value: %d:%s\n", exec->inputStackNr, value);
#endif
    if (exec->inputStackMax == 0) {
	exec->inputStackMax = 4;
	exec->inputStack = (xmlRegInputTokenPtr) 
	    xmlMalloc(exec->inputStackMax * sizeof(xmlRegInputToken));
	if (exec->inputStack == NULL) {
	    xmlRegexpErrMemory(NULL, "pushing input string");
	    exec->inputStackMax = 0;
	    return;
	}
    } else if (exec->inputStackNr + 1 >= exec->inputStackMax) {
	xmlRegInputTokenPtr tmp;

	exec->inputStackMax *= 2;
	tmp = (xmlRegInputTokenPtr) xmlRealloc(exec->inputStack,
			exec->inputStackMax * sizeof(xmlRegInputToken));
	if (tmp == NULL) {
	    xmlRegexpErrMemory(NULL, "pushing input string");
	    exec->inputStackMax /= 2;
	    return;
	}
	exec->inputStack = tmp;
    }
    exec->inputStack[exec->inputStackNr].value = xmlStrdup(value);
    exec->inputStack[exec->inputStackNr].data = data;
    exec->inputStackNr++;
    exec->inputStack[exec->inputStackNr].value = NULL;
    exec->inputStack[exec->inputStackNr].data = NULL;
}


static int
xmlRegStrEqualWildcard(const xmlChar *expStr, const xmlChar *valStr) {
    if (expStr == valStr) return(1);
    if (expStr == NULL) return(0);
    if (valStr == NULL) return(0);
    do {
        if (*expStr != *valStr) {
	    
	    if (*valStr == '*') {
	        const xmlChar *tmp;

		tmp = valStr;
		valStr = expStr;
		expStr = tmp;
	    }
	    if ((*valStr != 0) && (*expStr != 0) && (*expStr++ == '*')) {
		do {
		    if (*valStr == XML_REG_STRING_SEPARATOR)
			break;
		    valStr++;
		} while (*valStr != 0);
		continue;
	    } else
		return(0);
	}
	expStr++;
	valStr++;
    } while (*valStr != 0);
    if (*expStr != 0)
	return (0);
    else
	return (1);
}

static int
xmlRegCompactPushString(xmlRegExecCtxtPtr exec,
	                xmlRegexpPtr comp,
	                const xmlChar *value,
	                void *data) {
    int state = exec->index;
    int i, target;

    if ((comp == NULL) || (comp->compact == NULL) || (comp->stringMap == NULL))
	return(-1);
    
    if (value == NULL) {
	if (comp->compact[state * (comp->nbstrings + 1)] ==
            XML_REGEXP_FINAL_STATE)
	    return(1);
	return(0);
    }

#ifdef DEBUG_PUSH
    printf("value pushed: %s\n", value);
#endif

    for (i = 0;i < comp->nbstrings;i++) {
	target = comp->compact[state * (comp->nbstrings + 1) + i + 1];
	if ((target > 0) && (target <= comp->nbstates)) {
	    target--;     
	    if (xmlRegStrEqualWildcard(comp->stringMap[i], value)) {
		exec->index = target;		
		if ((exec->callback != NULL) && (comp->transdata != NULL)) {
		    exec->callback(exec->data, value,
			  comp->transdata[state * comp->nbstrings + i], data);
		}
#ifdef DEBUG_PUSH
		printf("entering state %d\n", target);
#endif
		if (comp->compact[target * (comp->nbstrings + 1)] ==
		    XML_REGEXP_SINK_STATE)
		    goto error;

		if (comp->compact[target * (comp->nbstrings + 1)] ==
		    XML_REGEXP_FINAL_STATE)
		    return(1);
		return(0);
	    }
	}
    }
#ifdef DEBUG_PUSH
    printf("failed to find a transition for %s on state %d\n", value, state);
#endif
error:
    if (exec->errString != NULL)
        xmlFree(exec->errString);
    exec->errString = xmlStrdup(value);
    exec->errStateNo = state;
    exec->status = -1;
#ifdef DEBUG_ERR
    testerr(exec);
#endif
    return(-1);
}

static int
xmlRegExecPushStringInternal(xmlRegExecCtxtPtr exec, const xmlChar *value,
	                     void *data, int compound) {
    xmlRegTransPtr trans;
    xmlRegAtomPtr atom;
    int ret;
    int final = 0;
    int progress = 1;

    if (exec == NULL)
	return(-1);
    if (exec->comp == NULL)
	return(-1);
    if (exec->status != 0)
	return(exec->status);

    if (exec->comp->compact != NULL)
	return(xmlRegCompactPushString(exec, exec->comp, value, data));

    if (value == NULL) {
        if (exec->state->type == XML_REGEXP_FINAL_STATE)
	    return(1);
	final = 1;
    }

#ifdef DEBUG_PUSH
    printf("value pushed: %s\n", value);
#endif
    if ((value != NULL) && (exec->inputStackNr > 0)) {
	xmlFARegExecSaveInputString(exec, value, data);
	value = exec->inputStack[exec->index].value;
	data = exec->inputStack[exec->index].data;
#ifdef DEBUG_PUSH
	printf("value loaded: %s\n", value);
#endif
    }

    while ((exec->status == 0) &&
	   ((value != NULL) ||
	    ((final == 1) &&
	     (exec->state->type != XML_REGEXP_FINAL_STATE)))) {

	if ((value == NULL) && (exec->counts == NULL))
	    goto rollback;

	exec->transcount = 0;
	for (;exec->transno < exec->state->nbTrans;exec->transno++) {
	    trans = &exec->state->trans[exec->transno];
	    if (trans->to < 0)
		continue;
	    atom = trans->atom;
	    ret = 0;
	    if (trans->count == REGEXP_ALL_LAX_COUNTER) {
		int i;
		int count;
		xmlRegTransPtr t;
		xmlRegCounterPtr counter;

		ret = 0;

#ifdef DEBUG_PUSH
		printf("testing all lax %d\n", trans->count);
#endif
		if ((value == NULL) && (final)) {
		    ret = 1;
		} else if (value != NULL) {
		    for (i = 0;i < exec->state->nbTrans;i++) {
			t = &exec->state->trans[i];
			if ((t->counter < 0) || (t == trans))
			    continue;
			counter = &exec->comp->counters[t->counter];
			count = exec->counts[t->counter];
			if ((count < counter->max) && 
		            (t->atom != NULL) &&
			    (xmlStrEqual(value, t->atom->valuep))) {
			    ret = 0;
			    break;
			}
			if ((count >= counter->min) &&
			    (count < counter->max) &&
			    (t->atom != NULL) &&
			    (xmlStrEqual(value, t->atom->valuep))) {
			    ret = 1;
			    break;
			}
		    }
		}
	    } else if (trans->count == REGEXP_ALL_COUNTER) {
		int i;
		int count;
		xmlRegTransPtr t;
		xmlRegCounterPtr counter;

		ret = 1;

#ifdef DEBUG_PUSH
		printf("testing all %d\n", trans->count);
#endif
		for (i = 0;i < exec->state->nbTrans;i++) {
                    t = &exec->state->trans[i];
		    if ((t->counter < 0) || (t == trans))
			continue;
                    counter = &exec->comp->counters[t->counter];
		    count = exec->counts[t->counter];
		    if ((count < counter->min) || (count > counter->max)) {
			ret = 0;
			break;
		    }
		}
	    } else if (trans->count >= 0) {
		int count;
		xmlRegCounterPtr counter;


		count = exec->counts[trans->count];
		counter = &exec->comp->counters[trans->count];
#ifdef DEBUG_PUSH
		printf("testing count %d: val %d, min %d, max %d\n",
		       trans->count, count, counter->min,  counter->max);
#endif
		ret = ((count >= counter->min) && (count <= counter->max));
	    } else if (atom == NULL) {
		fprintf(stderr, "epsilon transition left at runtime\n");
		exec->status = -2;
		break;
	    } else if (value != NULL) {
		ret = xmlRegStrEqualWildcard(atom->valuep, value);
		if (atom->neg) {
		    ret = !ret;
		    if (!compound)
		        ret = 0;
		}
		if ((ret == 1) && (trans->counter >= 0)) {
		    xmlRegCounterPtr counter;
		    int count;

		    count = exec->counts[trans->counter];
		    counter = &exec->comp->counters[trans->counter];
		    if (count >= counter->max)
			ret = 0;
		}

		if ((ret == 1) && (atom->min > 0) && (atom->max > 0)) {
		    xmlRegStatePtr to = exec->comp->states[trans->to];

		    if (exec->state->nbTrans > exec->transno + 1) {
			if (exec->inputStackNr <= 0) {
			    xmlFARegExecSaveInputString(exec, value, data);
			}
			xmlFARegExecSave(exec);
		    }
		    exec->transcount = 1;
		    do {
			if (exec->transcount == atom->max) {
			    break;
			}
			exec->index++;
			value = exec->inputStack[exec->index].value;
			data = exec->inputStack[exec->index].data;
#ifdef DEBUG_PUSH
			printf("value loaded: %s\n", value);
#endif

			if (value == NULL) {
			    exec->index --;
			    break;
			}
			if (exec->transcount >= atom->min) {
			    int transno = exec->transno;
			    xmlRegStatePtr state = exec->state;

			    exec->transno = -1; 
			    exec->state = to;
			    if (exec->inputStackNr <= 0) {
				xmlFARegExecSaveInputString(exec, value, data);
			    }
			    xmlFARegExecSave(exec);
			    exec->transno = transno;
			    exec->state = state;
			}
			ret = xmlStrEqual(value, atom->valuep);
			exec->transcount++;
		    } while (ret == 1);
		    if (exec->transcount < atom->min)
			ret = 0;

		    if (ret < 0)
			ret = 0;
		    if (ret == 0) {
			goto rollback;
		    }
		}
	    }
	    if (ret == 1) {
		if ((exec->callback != NULL) && (atom != NULL) &&
			(data != NULL)) {
		    exec->callback(exec->data, atom->valuep,
			           atom->data, data);
		}
		if (exec->state->nbTrans > exec->transno + 1) {
		    if (exec->inputStackNr <= 0) {
			xmlFARegExecSaveInputString(exec, value, data);
		    }
		    xmlFARegExecSave(exec);
		}
		if (trans->counter >= 0) {
#ifdef DEBUG_PUSH
		    printf("Increasing count %d\n", trans->counter);
#endif
		    exec->counts[trans->counter]++;
		}
		if ((trans->count >= 0) &&
		    (trans->count < REGEXP_ALL_COUNTER)) {
#ifdef DEBUG_REGEXP_EXEC
		    printf("resetting count %d on transition\n",
		           trans->count);
#endif
		    exec->counts[trans->count] = 0;
		}
#ifdef DEBUG_PUSH
		printf("entering state %d\n", trans->to);
#endif
                if ((exec->comp->states[trans->to] != NULL) &&
		    (exec->comp->states[trans->to]->type ==
		     XML_REGEXP_SINK_STATE)) {
		    if (exec->errString != NULL)
			xmlFree(exec->errString);
		    exec->errString = xmlStrdup(value);
		    exec->errState = exec->state;
		    memcpy(exec->errCounts, exec->counts,
			   exec->comp->nbCounters * sizeof(int));
		}
		exec->state = exec->comp->states[trans->to];
		exec->transno = 0;
		if (trans->atom != NULL) {
		    if (exec->inputStack != NULL) {
			exec->index++;
			if (exec->index < exec->inputStackNr) {
			    value = exec->inputStack[exec->index].value;
			    data = exec->inputStack[exec->index].data;
#ifdef DEBUG_PUSH
			    printf("value loaded: %s\n", value);
#endif
			} else {
			    value = NULL;
			    data = NULL;
#ifdef DEBUG_PUSH
			    printf("end of input\n");
#endif
			}
		    } else {
			value = NULL;
			data = NULL;
#ifdef DEBUG_PUSH
			printf("end of input\n");
#endif
		    }
		}
		goto progress;
	    } else if (ret < 0) {
		exec->status = -4;
		break;
	    }
	}
	if ((exec->transno != 0) || (exec->state->nbTrans == 0)) {
rollback:
	    if ((progress) && (exec->state != NULL) &&
	        (exec->state->type != XML_REGEXP_SINK_STATE)) {
	        progress = 0;
		if (exec->errString != NULL)
		    xmlFree(exec->errString);
		exec->errString = xmlStrdup(value);
		exec->errState = exec->state;
		memcpy(exec->errCounts, exec->counts,
		       exec->comp->nbCounters * sizeof(int));
	    }

	    exec->determinist = 0;
	    xmlFARegExecRollBack(exec);
	    if (exec->status == 0) {
		value = exec->inputStack[exec->index].value;
		data = exec->inputStack[exec->index].data;
#ifdef DEBUG_PUSH
		printf("value loaded: %s\n", value);
#endif
	    }
	}
	continue;
progress:
        progress = 1;
	continue;
    }
    if (exec->status == 0) {
        return(exec->state->type == XML_REGEXP_FINAL_STATE);
    }
#ifdef DEBUG_ERR
    if (exec->status < 0) {
	testerr(exec);
    }
#endif
    return(exec->status);
}

int
xmlRegExecPushString(xmlRegExecCtxtPtr exec, const xmlChar *value,
	             void *data) {
    return(xmlRegExecPushStringInternal(exec, value, data, 0));
}

int
xmlRegExecPushString2(xmlRegExecCtxtPtr exec, const xmlChar *value,
                      const xmlChar *value2, void *data) {
    xmlChar buf[150];
    int lenn, lenp, ret;
    xmlChar *str;

    if (exec == NULL)
	return(-1);
    if (exec->comp == NULL)
	return(-1);
    if (exec->status != 0)
	return(exec->status);

    if (value2 == NULL)
        return(xmlRegExecPushString(exec, value, data));

    lenn = strlen((char *) value2);
    lenp = strlen((char *) value);

    if (150 < lenn + lenp + 2) {
	str = (xmlChar *) xmlMallocAtomic(lenn + lenp + 2);
	if (str == NULL) {
	    exec->status = -1;
	    return(-1);
	}
    } else {
	str = buf;
    }
    memcpy(&str[0], value, lenp);
    str[lenp] = XML_REG_STRING_SEPARATOR;
    memcpy(&str[lenp + 1], value2, lenn);
    str[lenn + lenp + 1] = 0;

    if (exec->comp->compact != NULL)
	ret = xmlRegCompactPushString(exec, exec->comp, str, data);
    else
        ret = xmlRegExecPushStringInternal(exec, str, data, 1);

    if (str != buf)
        xmlFree(str);
    return(ret);
}

static int
xmlRegExecGetValues(xmlRegExecCtxtPtr exec, int err,
                    int *nbval, int *nbneg,
		    xmlChar **values, int *terminal) {
    int maxval;
    int nb = 0;

    if ((exec == NULL) || (nbval == NULL) || (nbneg == NULL) || 
        (values == NULL) || (*nbval <= 0))
        return(-1);

    maxval = *nbval;
    *nbval = 0;
    *nbneg = 0;
    if ((exec->comp != NULL) && (exec->comp->compact != NULL)) {
        xmlRegexpPtr comp;
	int target, i, state;

        comp = exec->comp;

	if (err) {
	    if (exec->errStateNo == -1) return(-1);
	    state = exec->errStateNo;
	} else {
	    state = exec->index;
	}
	if (terminal != NULL) {
	    if (comp->compact[state * (comp->nbstrings + 1)] ==
	        XML_REGEXP_FINAL_STATE)
		*terminal = 1;
	    else
		*terminal = 0;
	}
	for (i = 0;(i < comp->nbstrings) && (nb < maxval);i++) {
	    target = comp->compact[state * (comp->nbstrings + 1) + i + 1];
	    if ((target > 0) && (target <= comp->nbstates) &&
	        (comp->compact[(target - 1) * (comp->nbstrings + 1)] !=
		 XML_REGEXP_SINK_STATE)) {
	        values[nb++] = comp->stringMap[i];
		(*nbval)++;
	    }
	}
	for (i = 0;(i < comp->nbstrings) && (nb < maxval);i++) {
	    target = comp->compact[state * (comp->nbstrings + 1) + i + 1];
	    if ((target > 0) && (target <= comp->nbstates) &&
	        (comp->compact[(target - 1) * (comp->nbstrings + 1)] ==
		 XML_REGEXP_SINK_STATE)) {
	        values[nb++] = comp->stringMap[i];
		(*nbneg)++;
	    }
	}
    } else {
        int transno;
	xmlRegTransPtr trans;
	xmlRegAtomPtr atom;
	xmlRegStatePtr state;

	if (terminal != NULL) {
	    if (exec->state->type == XML_REGEXP_FINAL_STATE)
		*terminal = 1;
	    else
		*terminal = 0;
	}

	if (err) {
	    if (exec->errState == NULL) return(-1);
	    state = exec->errState;
	} else {
	    if (exec->state == NULL) return(-1);
	    state = exec->state;
	}
	for (transno = 0;
	     (transno < state->nbTrans) && (nb < maxval);
	     transno++) {
	    trans = &state->trans[transno];
	    if (trans->to < 0)
		continue;
	    atom = trans->atom;
	    if ((atom == NULL) || (atom->valuep == NULL))
		continue;
	    if (trans->count == REGEXP_ALL_LAX_COUNTER) {
	        
	        TODO;
	    } else if (trans->count == REGEXP_ALL_COUNTER) {
	        
	        TODO;
	    } else if (trans->counter >= 0) {
		xmlRegCounterPtr counter = NULL;
		int count;

		if (err)
		    count = exec->errCounts[trans->counter];
		else
		    count = exec->counts[trans->counter];
		if (exec->comp != NULL)
		    counter = &exec->comp->counters[trans->counter];
		if ((counter == NULL) || (count < counter->max)) {
		    if (atom->neg)
			values[nb++] = (xmlChar *) atom->valuep2;
		    else
			values[nb++] = (xmlChar *) atom->valuep;
		    (*nbval)++;
		}
	    } else {
                if ((exec->comp->states[trans->to] != NULL) &&
		    (exec->comp->states[trans->to]->type !=
		     XML_REGEXP_SINK_STATE)) {
		    if (atom->neg)
			values[nb++] = (xmlChar *) atom->valuep2;
		    else
			values[nb++] = (xmlChar *) atom->valuep;
		    (*nbval)++;
		}
	    } 
	}
	for (transno = 0;
	     (transno < state->nbTrans) && (nb < maxval);
	     transno++) {
	    trans = &state->trans[transno];
	    if (trans->to < 0)
		continue;
	    atom = trans->atom;
	    if ((atom == NULL) || (atom->valuep == NULL))
		continue;
	    if (trans->count == REGEXP_ALL_LAX_COUNTER) {
	        continue;
	    } else if (trans->count == REGEXP_ALL_COUNTER) {
	        continue;
	    } else if (trans->counter >= 0) {
	        continue;
	    } else {
                if ((exec->comp->states[trans->to] != NULL) &&
		    (exec->comp->states[trans->to]->type ==
		     XML_REGEXP_SINK_STATE)) {
		    if (atom->neg)
			values[nb++] = (xmlChar *) atom->valuep2;
		    else
			values[nb++] = (xmlChar *) atom->valuep;
		    (*nbneg)++;
		}
	    } 
	}
    }
    return(0);
}

int
xmlRegExecNextValues(xmlRegExecCtxtPtr exec, int *nbval, int *nbneg,
                     xmlChar **values, int *terminal) {
    return(xmlRegExecGetValues(exec, 0, nbval, nbneg, values, terminal));
}

int
xmlRegExecErrInfo(xmlRegExecCtxtPtr exec, const xmlChar **string,
                  int *nbval, int *nbneg, xmlChar **values, int *terminal) {
    if (exec == NULL)
        return(-1);
    if (string != NULL) {
        if (exec->status != 0)
	    *string = exec->errString;
	else
	    *string = NULL;
    }
    return(xmlRegExecGetValues(exec, 1, nbval, nbneg, values, terminal));
}

#ifdef DEBUG_ERR
static void testerr(xmlRegExecCtxtPtr exec) {
    const xmlChar *string;
    xmlChar *values[5];
    int nb = 5;
    int nbneg;
    int terminal;
    xmlRegExecErrInfo(exec, &string, &nb, &nbneg, &values[0], &terminal);
}
#endif

#if 0
static int
xmlRegExecPushChar(xmlRegExecCtxtPtr exec, int UCS) {
    xmlRegTransPtr trans;
    xmlRegAtomPtr atom;
    int ret;
    int codepoint, len;

    if (exec == NULL)
	return(-1);
    if (exec->status != 0)
	return(exec->status);

    while ((exec->status == 0) &&
	   ((exec->inputString[exec->index] != 0) ||
	    (exec->state->type != XML_REGEXP_FINAL_STATE))) {

	if ((exec->inputString[exec->index] == 0) && (exec->counts == NULL))
	    goto rollback;

	exec->transcount = 0;
	for (;exec->transno < exec->state->nbTrans;exec->transno++) {
	    trans = &exec->state->trans[exec->transno];
	    if (trans->to < 0)
		continue;
	    atom = trans->atom;
	    ret = 0;
	    if (trans->count >= 0) {
		int count;
		xmlRegCounterPtr counter;


		count = exec->counts[trans->count];
		counter = &exec->comp->counters[trans->count];
#ifdef DEBUG_REGEXP_EXEC
		printf("testing count %d: val %d, min %d, max %d\n",
		       trans->count, count, counter->min,  counter->max);
#endif
		ret = ((count >= counter->min) && (count <= counter->max));
	    } else if (atom == NULL) {
		fprintf(stderr, "epsilon transition left at runtime\n");
		exec->status = -2;
		break;
	    } else if (exec->inputString[exec->index] != 0) {
                codepoint = CUR_SCHAR(&(exec->inputString[exec->index]), len);
		ret = xmlRegCheckCharacter(atom, codepoint);
		if ((ret == 1) && (atom->min > 0) && (atom->max > 0)) {
		    xmlRegStatePtr to = exec->comp->states[trans->to];

		    if (exec->state->nbTrans > exec->transno + 1) {
			xmlFARegExecSave(exec);
		    }
		    exec->transcount = 1;
		    do {
			if (exec->transcount == atom->max) {
			    break;
			}
			exec->index += len;
			if (exec->inputString[exec->index] == 0) {
			    exec->index -= len;
			    break;
			}
			if (exec->transcount >= atom->min) {
			    int transno = exec->transno;
			    xmlRegStatePtr state = exec->state;

			    exec->transno = -1; 
			    exec->state = to;
			    xmlFARegExecSave(exec);
			    exec->transno = transno;
			    exec->state = state;
			}
			codepoint = CUR_SCHAR(&(exec->inputString[exec->index]),
				              len);
			ret = xmlRegCheckCharacter(atom, codepoint);
			exec->transcount++;
		    } while (ret == 1);
		    if (exec->transcount < atom->min)
			ret = 0;

		    if (ret < 0)
			ret = 0;
		    if (ret == 0) {
			goto rollback;
		    }
		}
	    }
	    if (ret == 1) {
		if (exec->state->nbTrans > exec->transno + 1) {
		    xmlFARegExecSave(exec);
		}
		if (trans->count >= 0) {
#ifdef DEBUG_REGEXP_EXEC
		    printf("Reset count %d\n", trans->count);
#endif
		    exec->counts[trans->count] = 0;
		}
		if (trans->counter >= 0) {
#ifdef DEBUG_REGEXP_EXEC
		    printf("Increasing count %d\n", trans->counter);
#endif
		    exec->counts[trans->counter]++;
		}
#ifdef DEBUG_REGEXP_EXEC
		printf("entering state %d\n", trans->to);
#endif
		exec->state = exec->comp->states[trans->to];
		exec->transno = 0;
		if (trans->atom != NULL) {
		    exec->index += len;
		}
		goto progress;
	    } else if (ret < 0) {
		exec->status = -4;
		break;
	    }
	}
	if ((exec->transno != 0) || (exec->state->nbTrans == 0)) {
rollback:
	    exec->determinist = 0;
	    xmlFARegExecRollBack(exec);
	}
progress:
	continue;
    }
}
#endif

static int
xmlFAIsChar(xmlRegParserCtxtPtr ctxt) {
    int cur;
    int len;

    cur = CUR_SCHAR(ctxt->cur, len);
    if ((cur == '.') || (cur == '\\') || (cur == '?') ||
	(cur == '*') || (cur == '+') || (cur == '(') ||
	(cur == ')') || (cur == '|') || (cur == 0x5B) ||
	(cur == 0x5D) || (cur == 0))
	return(-1);
    return(cur);
}

static void
xmlFAParseCharProp(xmlRegParserCtxtPtr ctxt) {
    int cur;
    xmlRegAtomType type = (xmlRegAtomType) 0;
    xmlChar *blockName = NULL;
    
    cur = CUR;
    if (cur == 'L') {
	NEXT;
	cur = CUR;
	if (cur == 'u') {
	    NEXT;
	    type = XML_REGEXP_LETTER_UPPERCASE;
	} else if (cur == 'l') {
	    NEXT;
	    type = XML_REGEXP_LETTER_LOWERCASE;
	} else if (cur == 't') {
	    NEXT;
	    type = XML_REGEXP_LETTER_TITLECASE;
	} else if (cur == 'm') {
	    NEXT;
	    type = XML_REGEXP_LETTER_MODIFIER;
	} else if (cur == 'o') {
	    NEXT;
	    type = XML_REGEXP_LETTER_OTHERS;
	} else {
	    type = XML_REGEXP_LETTER;
	}
    } else if (cur == 'M') {
	NEXT;
	cur = CUR;
	if (cur == 'n') {
	    NEXT;
	    
	    type = XML_REGEXP_MARK_NONSPACING;
	} else if (cur == 'c') {
	    NEXT;
	    
	    type = XML_REGEXP_MARK_SPACECOMBINING;
	} else if (cur == 'e') {
	    NEXT;
	    
	    type = XML_REGEXP_MARK_ENCLOSING;
	} else {
	    
	    type = XML_REGEXP_MARK;
	}
    } else if (cur == 'N') {
	NEXT;
	cur = CUR;
	if (cur == 'd') {
	    NEXT;
	    
	    type = XML_REGEXP_NUMBER_DECIMAL;
	} else if (cur == 'l') {
	    NEXT;
	    
	    type = XML_REGEXP_NUMBER_LETTER;
	} else if (cur == 'o') {
	    NEXT;
	    
	    type = XML_REGEXP_NUMBER_OTHERS;
	} else {
	    
	    type = XML_REGEXP_NUMBER;
	}
    } else if (cur == 'P') {
	NEXT;
	cur = CUR;
	if (cur == 'c') {
	    NEXT;
	    
	    type = XML_REGEXP_PUNCT_CONNECTOR;
	} else if (cur == 'd') {
	    NEXT;
	    
	    type = XML_REGEXP_PUNCT_DASH;
	} else if (cur == 's') {
	    NEXT;
	    
	    type = XML_REGEXP_PUNCT_OPEN;
	} else if (cur == 'e') {
	    NEXT;
	    
	    type = XML_REGEXP_PUNCT_CLOSE;
	} else if (cur == 'i') {
	    NEXT;
	    
	    type = XML_REGEXP_PUNCT_INITQUOTE;
	} else if (cur == 'f') {
	    NEXT;
	    
	    type = XML_REGEXP_PUNCT_FINQUOTE;
	} else if (cur == 'o') {
	    NEXT;
	    
	    type = XML_REGEXP_PUNCT_OTHERS;
	} else {
	    
	    type = XML_REGEXP_PUNCT;
	}
    } else if (cur == 'Z') {
	NEXT;
	cur = CUR;
	if (cur == 's') {
	    NEXT;
	    
	    type = XML_REGEXP_SEPAR_SPACE;
	} else if (cur == 'l') {
	    NEXT;
	    
	    type = XML_REGEXP_SEPAR_LINE;
	} else if (cur == 'p') {
	    NEXT;
	    
	    type = XML_REGEXP_SEPAR_PARA;
	} else {
	    
	    type = XML_REGEXP_SEPAR;
	}
    } else if (cur == 'S') {
	NEXT;
	cur = CUR;
	if (cur == 'm') {
	    NEXT;
	    type = XML_REGEXP_SYMBOL_MATH;
	    
	} else if (cur == 'c') {
	    NEXT;
	    type = XML_REGEXP_SYMBOL_CURRENCY;
	    
	} else if (cur == 'k') {
	    NEXT;
	    type = XML_REGEXP_SYMBOL_MODIFIER;
	    
	} else if (cur == 'o') {
	    NEXT;
	    type = XML_REGEXP_SYMBOL_OTHERS;
	    
	} else {
	    
	    type = XML_REGEXP_SYMBOL;
	}
    } else if (cur == 'C') {
	NEXT;
	cur = CUR;
	if (cur == 'c') {
	    NEXT;
	    
	    type = XML_REGEXP_OTHER_CONTROL;
	} else if (cur == 'f') {
	    NEXT;
	    
	    type = XML_REGEXP_OTHER_FORMAT;
	} else if (cur == 'o') {
	    NEXT;
	    
	    type = XML_REGEXP_OTHER_PRIVATE;
	} else if (cur == 'n') {
	    NEXT;
	    
	    type = XML_REGEXP_OTHER_NA;
	} else {
	    
	    type = XML_REGEXP_OTHER;
	}
    } else if (cur == 'I') {
	const xmlChar *start;
	NEXT;
	cur = CUR;
	if (cur != 's') {
	    ERROR("IsXXXX expected");
	    return;
	}
	NEXT;
	start = ctxt->cur;
	cur = CUR;
	if (((cur >= 'a') && (cur <= 'z')) || 
	    ((cur >= 'A') && (cur <= 'Z')) || 
	    ((cur >= '0') && (cur <= '9')) || 
	    (cur == 0x2D)) {
	    NEXT;
	    cur = CUR;
	    while (((cur >= 'a') && (cur <= 'z')) || 
		((cur >= 'A') && (cur <= 'Z')) || 
		((cur >= '0') && (cur <= '9')) || 
		(cur == 0x2D)) {
		NEXT;
		cur = CUR;
	    }
	}
	type = XML_REGEXP_BLOCK_NAME;
	blockName = xmlStrndup(start, ctxt->cur - start);
    } else {
	ERROR("Unknown char property");
	return;
    }
    if (ctxt->atom == NULL) {
	ctxt->atom = xmlRegNewAtom(ctxt, type);
	if (ctxt->atom != NULL)
	    ctxt->atom->valuep = blockName;
    } else if (ctxt->atom->type == XML_REGEXP_RANGES) {
        xmlRegAtomAddRange(ctxt, ctxt->atom, ctxt->neg,
		           type, 0, 0, blockName);
    }
}

static void
xmlFAParseCharClassEsc(xmlRegParserCtxtPtr ctxt) {
    int cur;

    if (CUR == '.') {
	if (ctxt->atom == NULL) {
	    ctxt->atom = xmlRegNewAtom(ctxt, XML_REGEXP_ANYCHAR);
	} else if (ctxt->atom->type == XML_REGEXP_RANGES) {
	    xmlRegAtomAddRange(ctxt, ctxt->atom, ctxt->neg,
			       XML_REGEXP_ANYCHAR, 0, 0, NULL);
	}
	NEXT;
	return;
    }
    if (CUR != '\\') {
	ERROR("Escaped sequence: expecting \\");
	return;
    }
    NEXT;
    cur = CUR;
    if (cur == 'p') {
	NEXT;
	if (CUR != '{') {
	    ERROR("Expecting '{'");
	    return;
	}
	NEXT;
	xmlFAParseCharProp(ctxt);
	if (CUR != '}') {
	    ERROR("Expecting '}'");
	    return;
	}
	NEXT;
    } else if (cur == 'P') {
	NEXT;
	if (CUR != '{') {
	    ERROR("Expecting '{'");
	    return;
	}
	NEXT;
	xmlFAParseCharProp(ctxt);
	ctxt->atom->neg = 1;
	if (CUR != '}') {
	    ERROR("Expecting '}'");
	    return;
	}
	NEXT;
    } else if ((cur == 'n') || (cur == 'r') || (cur == 't') || (cur == '\\') ||
	(cur == '|') || (cur == '.') || (cur == '?') || (cur == '*') ||
	(cur == '+') || (cur == '(') || (cur == ')') || (cur == '{') ||
	(cur == '}') || (cur == 0x2D) || (cur == 0x5B) || (cur == 0x5D) ||
	(cur == 0x5E)) {
	if (ctxt->atom == NULL) {
	    ctxt->atom = xmlRegNewAtom(ctxt, XML_REGEXP_CHARVAL);
	    if (ctxt->atom != NULL) {
	        switch (cur) {
		    case 'n':
		        ctxt->atom->codepoint = '\n';
			break;
		    case 'r':
		        ctxt->atom->codepoint = '\r';
			break;
		    case 't':
		        ctxt->atom->codepoint = '\t';
			break;
		    default:
			ctxt->atom->codepoint = cur;
		}
	    }
	} else if (ctxt->atom->type == XML_REGEXP_RANGES) {
            switch (cur) {
                case 'n':
                    cur = '\n';
                    break;
                case 'r':
                    cur = '\r';
                    break;
                case 't':
                    cur = '\t';
                    break;
            }
	    xmlRegAtomAddRange(ctxt, ctxt->atom, ctxt->neg,
			       XML_REGEXP_CHARVAL, cur, cur, NULL);
	}
	NEXT;
    } else if ((cur == 's') || (cur == 'S') || (cur == 'i') || (cur == 'I') ||
	(cur == 'c') || (cur == 'C') || (cur == 'd') || (cur == 'D') ||
	(cur == 'w') || (cur == 'W')) {
	xmlRegAtomType type = XML_REGEXP_ANYSPACE;

	switch (cur) {
	    case 's': 
		type = XML_REGEXP_ANYSPACE;
		break;
	    case 'S': 
		type = XML_REGEXP_NOTSPACE;
		break;
	    case 'i': 
		type = XML_REGEXP_INITNAME;
		break;
	    case 'I': 
		type = XML_REGEXP_NOTINITNAME;
		break;
	    case 'c': 
		type = XML_REGEXP_NAMECHAR;
		break;
	    case 'C': 
		type = XML_REGEXP_NOTNAMECHAR;
		break;
	    case 'd': 
		type = XML_REGEXP_DECIMAL;
		break;
	    case 'D': 
		type = XML_REGEXP_NOTDECIMAL;
		break;
	    case 'w': 
		type = XML_REGEXP_REALCHAR;
		break;
	    case 'W': 
		type = XML_REGEXP_NOTREALCHAR;
		break;
	}
	NEXT;
	if (ctxt->atom == NULL) {
	    ctxt->atom = xmlRegNewAtom(ctxt, type);
	} else if (ctxt->atom->type == XML_REGEXP_RANGES) {
	    xmlRegAtomAddRange(ctxt, ctxt->atom, ctxt->neg,
			       type, 0, 0, NULL);
	}
    } else {
	ERROR("Wrong escape sequence, misuse of character '\\'");
    }
}

static void
xmlFAParseCharRange(xmlRegParserCtxtPtr ctxt) {
    int cur, len;
    int start = -1;
    int end = -1;

    if (CUR == '\0') {
        ERROR("Expecting ']'");
	return;
    }

    cur = CUR;
    if (cur == '\\') {
	NEXT;
	cur = CUR;
	switch (cur) {
	    case 'n': start = 0xA; break;
	    case 'r': start = 0xD; break;
	    case 't': start = 0x9; break;
	    case '\\': case '|': case '.': case '-': case '^': case '?':
	    case '*': case '+': case '{': case '}': case '(': case ')':
	    case '[': case ']':
		start = cur; break;
	    default:
		ERROR("Invalid escape value");
		return;
	}
	end = start;
        len = 1;
    } else if ((cur != 0x5B) && (cur != 0x5D)) {
        end = start = CUR_SCHAR(ctxt->cur, len);
    } else {
	ERROR("Expecting a char range");
	return;
    }
    if ((start == '-') && (NXT(1) != ']') && (PREV != '[') && (PREV != '^')) {
	NEXTL(len);
	return;
    }
    NEXTL(len);
    cur = CUR;
    if ((cur != '-') || (NXT(1) == ']')) {
        xmlRegAtomAddRange(ctxt, ctxt->atom, ctxt->neg,
		              XML_REGEXP_CHARVAL, start, end, NULL);
	return;
    }
    NEXT;
    cur = CUR;
    if (cur == '\\') {
	NEXT;
	cur = CUR;
	switch (cur) {
	    case 'n': end = 0xA; break;
	    case 'r': end = 0xD; break;
	    case 't': end = 0x9; break;
	    case '\\': case '|': case '.': case '-': case '^': case '?':
	    case '*': case '+': case '{': case '}': case '(': case ')':
	    case '[': case ']':
		end = cur; break;
	    default:
		ERROR("Invalid escape value");
		return;
	}
        len = 1;
    } else if ((cur != 0x5B) && (cur != 0x5D)) {
        end = CUR_SCHAR(ctxt->cur, len);
    } else {
	ERROR("Expecting the end of a char range");
	return;
    }
    NEXTL(len);
    
    if (end < start) {
	ERROR("End of range is before start of range");
    } else {
        xmlRegAtomAddRange(ctxt, ctxt->atom, ctxt->neg,
		           XML_REGEXP_CHARVAL, start, end, NULL);
    }
    return;
}

static void
xmlFAParsePosCharGroup(xmlRegParserCtxtPtr ctxt) {
    do {
	if (CUR == '\\') {
	    xmlFAParseCharClassEsc(ctxt);
	} else {
	    xmlFAParseCharRange(ctxt);
	}
    } while ((CUR != ']') && (CUR != '^') && (CUR != '-') &&
             (CUR != 0) && (ctxt->error == 0));
}

static void
xmlFAParseCharGroup(xmlRegParserCtxtPtr ctxt) {
    int n = ctxt->neg;
    while ((CUR != ']') && (ctxt->error == 0)) {
	if (CUR == '^') {
	    int neg = ctxt->neg;

	    NEXT;
	    ctxt->neg = !ctxt->neg;
	    xmlFAParsePosCharGroup(ctxt);
	    ctxt->neg = neg;
	} else if ((CUR == '-') && (NXT(1) == '[')) {
	    int neg = ctxt->neg;
	    ctxt->neg = 2;
	    NEXT;	
	    NEXT;	
	    xmlFAParseCharGroup(ctxt);
	    if (CUR == ']') {
		NEXT;
	    } else {
		ERROR("charClassExpr: ']' expected");
		break;
	    }
	    ctxt->neg = neg;
	    break;
	} else if (CUR != ']') {
	    xmlFAParsePosCharGroup(ctxt);
	}
    }
    ctxt->neg = n;
}

static void
xmlFAParseCharClass(xmlRegParserCtxtPtr ctxt) {
    if (CUR == '[') {
	NEXT;
	ctxt->atom = xmlRegNewAtom(ctxt, XML_REGEXP_RANGES);
	if (ctxt->atom == NULL)
	    return;
	xmlFAParseCharGroup(ctxt);
	if (CUR == ']') {
	    NEXT;
	} else {
	    ERROR("xmlFAParseCharClass: ']' expected");
	}
    } else {
	xmlFAParseCharClassEsc(ctxt);
    }
}

static int
xmlFAParseQuantExact(xmlRegParserCtxtPtr ctxt) {
    int ret = 0;
    int ok = 0;

    while ((CUR >= '0') && (CUR <= '9')) {
	ret = ret * 10 + (CUR - '0');
	ok = 1;
	NEXT;
    }
    if (ok != 1) {
	return(-1);
    }
    return(ret);
}

static int
xmlFAParseQuantifier(xmlRegParserCtxtPtr ctxt) {
    int cur;

    cur = CUR;
    if ((cur == '?') || (cur == '*') || (cur == '+')) {
	if (ctxt->atom != NULL) {
	    if (cur == '?')
		ctxt->atom->quant = XML_REGEXP_QUANT_OPT;
	    else if (cur == '*')
		ctxt->atom->quant = XML_REGEXP_QUANT_MULT;
	    else if (cur == '+')
		ctxt->atom->quant = XML_REGEXP_QUANT_PLUS;
	}
	NEXT;
	return(1);
    }
    if (cur == '{') {
	int min = 0, max = 0;

	NEXT;
	cur = xmlFAParseQuantExact(ctxt);
	if (cur >= 0)
	    min = cur;
	if (CUR == ',') {
	    NEXT;
	    if (CUR == '}')
	        max = INT_MAX;
	    else {
	        cur = xmlFAParseQuantExact(ctxt);
	        if (cur >= 0)
		    max = cur;
		else {
		    ERROR("Improper quantifier");
		}
	    }
	}
	if (CUR == '}') {
	    NEXT;
	} else {
	    ERROR("Unterminated quantifier");
	}
	if (max == 0)
	    max = min;
	if (ctxt->atom != NULL) {
	    ctxt->atom->quant = XML_REGEXP_QUANT_RANGE;
	    ctxt->atom->min = min;
	    ctxt->atom->max = max;
	}
	return(1);
    }
    return(0);
}

static int
xmlFAParseAtom(xmlRegParserCtxtPtr ctxt) {
    int codepoint, len;

    codepoint = xmlFAIsChar(ctxt);
    if (codepoint > 0) {
	ctxt->atom = xmlRegNewAtom(ctxt, XML_REGEXP_CHARVAL);
	if (ctxt->atom == NULL)
	    return(-1);
	codepoint = CUR_SCHAR(ctxt->cur, len);
	ctxt->atom->codepoint = codepoint;
	NEXTL(len);
	return(1);
    } else if (CUR == '|') {
	return(0);
    } else if (CUR == 0) {
	return(0);
    } else if (CUR == ')') {
	return(0);
    } else if (CUR == '(') {
	xmlRegStatePtr start, oldend, start0;

	NEXT;
	xmlFAGenerateEpsilonTransition(ctxt, ctxt->state, NULL);
	start0 = ctxt->state;
	xmlFAGenerateEpsilonTransition(ctxt, ctxt->state, NULL);
	start = ctxt->state;
	oldend = ctxt->end;
	ctxt->end = NULL;
	ctxt->atom = NULL;
	xmlFAParseRegExp(ctxt, 0);
	if (CUR == ')') {
	    NEXT;
	} else {
	    ERROR("xmlFAParseAtom: expecting ')'");
	}
	ctxt->atom = xmlRegNewAtom(ctxt, XML_REGEXP_SUBREG);
	if (ctxt->atom == NULL)
	    return(-1);
	ctxt->atom->start = start;
	ctxt->atom->start0 = start0;
	ctxt->atom->stop = ctxt->state;
	ctxt->end = oldend;
	return(1);
    } else if ((CUR == '[') || (CUR == '\\') || (CUR == '.')) {
	xmlFAParseCharClass(ctxt);
	return(1);
    }
    return(0);
}

static int
xmlFAParsePiece(xmlRegParserCtxtPtr ctxt) {
    int ret;

    ctxt->atom = NULL;
    ret = xmlFAParseAtom(ctxt);
    if (ret == 0)
	return(0);
    if (ctxt->atom == NULL) {
	ERROR("internal: no atom generated");
    }
    xmlFAParseQuantifier(ctxt);
    return(1);
}

static int
xmlFAParseBranch(xmlRegParserCtxtPtr ctxt, xmlRegStatePtr to) {
    xmlRegStatePtr previous;
    int ret;

    previous = ctxt->state;
    ret = xmlFAParsePiece(ctxt);
    if (ret != 0) {
	if (xmlFAGenerateTransitions(ctxt, previous, 
	        (CUR=='|' || CUR==')') ? to : NULL, ctxt->atom) < 0)
	    return(-1);
	previous = ctxt->state;
	ctxt->atom = NULL;
    }
    while ((ret != 0) && (ctxt->error == 0)) {
	ret = xmlFAParsePiece(ctxt);
	if (ret != 0) {
	    if (xmlFAGenerateTransitions(ctxt, previous, 
	            (CUR=='|' || CUR==')') ? to : NULL, ctxt->atom) < 0)
		    return(-1);
	    previous = ctxt->state;
	    ctxt->atom = NULL;
	}
    }
    return(0);
}

static void
xmlFAParseRegExp(xmlRegParserCtxtPtr ctxt, int top) {
    xmlRegStatePtr start, end;

    
    start = ctxt->state;
    ctxt->end = NULL;
    xmlFAParseBranch(ctxt, NULL);
    if (top) {
#ifdef DEBUG_REGEXP_GRAPH
	printf("State %d is final\n", ctxt->state->no);
#endif
	ctxt->state->type = XML_REGEXP_FINAL_STATE;
    }
    if (CUR != '|') {
	ctxt->end = ctxt->state;
	return;
    }
    end = ctxt->state;
    while ((CUR == '|') && (ctxt->error == 0)) {
	NEXT;
	ctxt->state = start;
	ctxt->end = NULL;
	xmlFAParseBranch(ctxt, end);
    }
    if (!top) {
	ctxt->state = end;
	ctxt->end = end;
    }
}


void
xmlRegexpPrint(FILE *output, xmlRegexpPtr regexp) {
    int i;

    if (output == NULL)
        return;
    fprintf(output, " regexp: ");
    if (regexp == NULL) {
	fprintf(output, "NULL\n");
	return;
    }
    fprintf(output, "'%s' ", regexp->string);
    fprintf(output, "\n");
    fprintf(output, "%d atoms:\n", regexp->nbAtoms);
    for (i = 0;i < regexp->nbAtoms; i++) {
	fprintf(output, " %02d ", i);
	xmlRegPrintAtom(output, regexp->atoms[i]);
    }
    fprintf(output, "%d states:", regexp->nbStates);
    fprintf(output, "\n");
    for (i = 0;i < regexp->nbStates; i++) {
	xmlRegPrintState(output, regexp->states[i]);
    }
    fprintf(output, "%d counters:\n", regexp->nbCounters);
    for (i = 0;i < regexp->nbCounters; i++) {
	fprintf(output, " %d: min %d max %d\n", i, regexp->counters[i].min,
		                                regexp->counters[i].max);
    }
}

xmlRegexpPtr
xmlRegexpCompile(const xmlChar *regexp) {
    xmlRegexpPtr ret;
    xmlRegParserCtxtPtr ctxt;

    ctxt = xmlRegNewParserCtxt(regexp);
    if (ctxt == NULL)
	return(NULL);

    
    ctxt->end = NULL;
    ctxt->start = ctxt->state = xmlRegNewState(ctxt);
    xmlRegStatePush(ctxt, ctxt->start);

    
    xmlFAParseRegExp(ctxt, 1);
    if (CUR != 0) {
	ERROR("xmlFAParseRegExp: extra characters");
    }
    if (ctxt->error != 0) {
	xmlRegFreeParserCtxt(ctxt);
	return(NULL);
    }
    ctxt->end = ctxt->state;
    ctxt->start->type = XML_REGEXP_START_STATE;
    ctxt->end->type = XML_REGEXP_FINAL_STATE;

    
    xmlFAEliminateEpsilonTransitions(ctxt);


    if (ctxt->error != 0) {
	xmlRegFreeParserCtxt(ctxt);
	return(NULL);
    }
    ret = xmlRegEpxFromParse(ctxt);
    xmlRegFreeParserCtxt(ctxt);
    return(ret);
}

int
xmlRegexpExec(xmlRegexpPtr comp, const xmlChar *content) {
    if ((comp == NULL) || (content == NULL))
	return(-1);
    return(xmlFARegExec(comp, content));
}

int
xmlRegexpIsDeterminist(xmlRegexpPtr comp) {
    xmlAutomataPtr am;
    int ret;

    if (comp == NULL)
	return(-1);
    if (comp->determinist != -1)
	return(comp->determinist);

    am = xmlNewAutomata();
    if (am->states != NULL) {
	int i;

	for (i = 0;i < am->nbStates;i++)
	    xmlRegFreeState(am->states[i]);
	xmlFree(am->states);
    }
    am->nbAtoms = comp->nbAtoms;
    am->atoms = comp->atoms;
    am->nbStates = comp->nbStates;
    am->states = comp->states;
    am->determinist = -1;
    am->flags = comp->flags;
    ret = xmlFAComputesDeterminism(am);
    am->atoms = NULL;
    am->states = NULL;
    xmlFreeAutomata(am);
    comp->determinist = ret;
    return(ret);
}

void
xmlRegFreeRegexp(xmlRegexpPtr regexp) {
    int i;
    if (regexp == NULL)
	return;

    if (regexp->string != NULL)
	xmlFree(regexp->string);
    if (regexp->states != NULL) {
	for (i = 0;i < regexp->nbStates;i++)
	    xmlRegFreeState(regexp->states[i]);
	xmlFree(regexp->states);
    }
    if (regexp->atoms != NULL) {
	for (i = 0;i < regexp->nbAtoms;i++)
	    xmlRegFreeAtom(regexp->atoms[i]);
	xmlFree(regexp->atoms);
    }
    if (regexp->counters != NULL)
	xmlFree(regexp->counters);
    if (regexp->compact != NULL)
	xmlFree(regexp->compact);
    if (regexp->transdata != NULL)
	xmlFree(regexp->transdata);
    if (regexp->stringMap != NULL) {
	for (i = 0; i < regexp->nbstrings;i++)
	    xmlFree(regexp->stringMap[i]);
	xmlFree(regexp->stringMap);
    }

    xmlFree(regexp);
}

#ifdef LIBXML_AUTOMATA_ENABLED

xmlAutomataPtr
xmlNewAutomata(void) {
    xmlAutomataPtr ctxt;

    ctxt = xmlRegNewParserCtxt(NULL);
    if (ctxt == NULL)
	return(NULL);

    
    ctxt->end = NULL;
    ctxt->start = ctxt->state = xmlRegNewState(ctxt);
    if (ctxt->start == NULL) {
	xmlFreeAutomata(ctxt);
	return(NULL);
    }
    ctxt->start->type = XML_REGEXP_START_STATE;
    if (xmlRegStatePush(ctxt, ctxt->start) < 0) {
        xmlRegFreeState(ctxt->start);
	xmlFreeAutomata(ctxt);
	return(NULL);
    }
    ctxt->flags = 0;

    return(ctxt);
}

void
xmlFreeAutomata(xmlAutomataPtr am) {
    if (am == NULL)
	return;
    xmlRegFreeParserCtxt(am);
}

void
xmlAutomataSetFlags(xmlAutomataPtr am, int flags) {
    if (am == NULL)
	return;
    am->flags |= flags;
}

xmlAutomataStatePtr
xmlAutomataGetInitState(xmlAutomataPtr am) {
    if (am == NULL)
	return(NULL);
    return(am->start);
}

int
xmlAutomataSetFinalState(xmlAutomataPtr am, xmlAutomataStatePtr state) {
    if ((am == NULL) || (state == NULL))
	return(-1);
    state->type = XML_REGEXP_FINAL_STATE;
    return(0);
}

xmlAutomataStatePtr
xmlAutomataNewTransition(xmlAutomataPtr am, xmlAutomataStatePtr from,
			 xmlAutomataStatePtr to, const xmlChar *token,
			 void *data) {
    xmlRegAtomPtr atom;

    if ((am == NULL) || (from == NULL) || (token == NULL))
	return(NULL);
    atom = xmlRegNewAtom(am, XML_REGEXP_STRING);
    if (atom == NULL)
        return(NULL);
    atom->data = data;
    if (atom == NULL)
	return(NULL);
    atom->valuep = xmlStrdup(token);

    if (xmlFAGenerateTransitions(am, from, to, atom) < 0) {
        xmlRegFreeAtom(atom);
	return(NULL);
    }
    if (to == NULL)
	return(am->state);
    return(to);
}

xmlAutomataStatePtr
xmlAutomataNewTransition2(xmlAutomataPtr am, xmlAutomataStatePtr from,
			  xmlAutomataStatePtr to, const xmlChar *token,
			  const xmlChar *token2, void *data) {
    xmlRegAtomPtr atom;

    if ((am == NULL) || (from == NULL) || (token == NULL))
	return(NULL);
    atom = xmlRegNewAtom(am, XML_REGEXP_STRING);
    if (atom == NULL)
	return(NULL);
    atom->data = data;
    if ((token2 == NULL) || (*token2 == 0)) {
	atom->valuep = xmlStrdup(token);
    } else {
	int lenn, lenp;
	xmlChar *str;

	lenn = strlen((char *) token2);
	lenp = strlen((char *) token);

	str = (xmlChar *) xmlMallocAtomic(lenn + lenp + 2);
	if (str == NULL) {
	    xmlRegFreeAtom(atom);
	    return(NULL);
	}
	memcpy(&str[0], token, lenp);
	str[lenp] = '|';
	memcpy(&str[lenp + 1], token2, lenn);
	str[lenn + lenp + 1] = 0;

	atom->valuep = str;
    }

    if (xmlFAGenerateTransitions(am, from, to, atom) < 0) {
        xmlRegFreeAtom(atom);
	return(NULL);
    }
    if (to == NULL)
	return(am->state);
    return(to);
}

xmlAutomataStatePtr
xmlAutomataNewNegTrans(xmlAutomataPtr am, xmlAutomataStatePtr from,
		       xmlAutomataStatePtr to, const xmlChar *token,
		       const xmlChar *token2, void *data) {
    xmlRegAtomPtr atom;
    xmlChar err_msg[200];

    if ((am == NULL) || (from == NULL) || (token == NULL))
	return(NULL);
    atom = xmlRegNewAtom(am, XML_REGEXP_STRING);
    if (atom == NULL)
	return(NULL);
    atom->data = data;
    atom->neg = 1;
    if ((token2 == NULL) || (*token2 == 0)) {
	atom->valuep = xmlStrdup(token);
    } else {
	int lenn, lenp;
	xmlChar *str;

	lenn = strlen((char *) token2);
	lenp = strlen((char *) token);

	str = (xmlChar *) xmlMallocAtomic(lenn + lenp + 2);
	if (str == NULL) {
	    xmlRegFreeAtom(atom);
	    return(NULL);
	}
	memcpy(&str[0], token, lenp);
	str[lenp] = '|';
	memcpy(&str[lenp + 1], token2, lenn);
	str[lenn + lenp + 1] = 0;

	atom->valuep = str;
    }
    snprintf((char *) err_msg, 199, "not %s", (const char *) atom->valuep);
    err_msg[199] = 0;
    atom->valuep2 = xmlStrdup(err_msg);

    if (xmlFAGenerateTransitions(am, from, to, atom) < 0) {
        xmlRegFreeAtom(atom);
	return(NULL);
    }
    am->negs++;
    if (to == NULL)
	return(am->state);
    return(to);
}

xmlAutomataStatePtr
xmlAutomataNewCountTrans2(xmlAutomataPtr am, xmlAutomataStatePtr from,
			 xmlAutomataStatePtr to, const xmlChar *token,
			 const xmlChar *token2,
			 int min, int max, void *data) {
    xmlRegAtomPtr atom;
    int counter;

    if ((am == NULL) || (from == NULL) || (token == NULL))
	return(NULL);
    if (min < 0)
	return(NULL);
    if ((max < min) || (max < 1))
	return(NULL);
    atom = xmlRegNewAtom(am, XML_REGEXP_STRING);
    if (atom == NULL)
	return(NULL);
    if ((token2 == NULL) || (*token2 == 0)) {
	atom->valuep = xmlStrdup(token);
    } else {
	int lenn, lenp;
	xmlChar *str;

	lenn = strlen((char *) token2);
	lenp = strlen((char *) token);

	str = (xmlChar *) xmlMallocAtomic(lenn + lenp + 2);
	if (str == NULL) {
	    xmlRegFreeAtom(atom);
	    return(NULL);
	}
	memcpy(&str[0], token, lenp);
	str[lenp] = '|';
	memcpy(&str[lenp + 1], token2, lenn);
	str[lenn + lenp + 1] = 0;

	atom->valuep = str;
    }
    atom->data = data;
    if (min == 0)
	atom->min = 1;
    else
	atom->min = min;
    atom->max = max;

    counter = xmlRegGetCounter(am);
    am->counters[counter].min = min;
    am->counters[counter].max = max;

    
    if (to == NULL) {
        to = xmlRegNewState(am);
	xmlRegStatePush(am, to);
    }
    xmlRegStateAddTrans(am, from, atom, to, counter, -1);
    xmlRegAtomPush(am, atom);
    am->state = to;

    if (to == NULL)
	to = am->state;
    if (to == NULL)
	return(NULL);
    if (min == 0)
	xmlFAGenerateEpsilonTransition(am, from, to);
    return(to);
}

xmlAutomataStatePtr
xmlAutomataNewCountTrans(xmlAutomataPtr am, xmlAutomataStatePtr from,
			 xmlAutomataStatePtr to, const xmlChar *token,
			 int min, int max, void *data) {
    xmlRegAtomPtr atom;
    int counter;

    if ((am == NULL) || (from == NULL) || (token == NULL))
	return(NULL);
    if (min < 0)
	return(NULL);
    if ((max < min) || (max < 1))
	return(NULL);
    atom = xmlRegNewAtom(am, XML_REGEXP_STRING);
    if (atom == NULL)
	return(NULL);
    atom->valuep = xmlStrdup(token);
    atom->data = data;
    if (min == 0)
	atom->min = 1;
    else
	atom->min = min;
    atom->max = max;

    counter = xmlRegGetCounter(am);
    am->counters[counter].min = min;
    am->counters[counter].max = max;

    
    if (to == NULL) {
        to = xmlRegNewState(am);
	xmlRegStatePush(am, to);
    }
    xmlRegStateAddTrans(am, from, atom, to, counter, -1);
    xmlRegAtomPush(am, atom);
    am->state = to;

    if (to == NULL)
	to = am->state;
    if (to == NULL)
	return(NULL);
    if (min == 0)
	xmlFAGenerateEpsilonTransition(am, from, to);
    return(to);
}

xmlAutomataStatePtr
xmlAutomataNewOnceTrans2(xmlAutomataPtr am, xmlAutomataStatePtr from,
			 xmlAutomataStatePtr to, const xmlChar *token,
			 const xmlChar *token2,
			 int min, int max, void *data) {
    xmlRegAtomPtr atom;
    int counter;

    if ((am == NULL) || (from == NULL) || (token == NULL))
	return(NULL);
    if (min < 1)
	return(NULL);
    if ((max < min) || (max < 1))
	return(NULL);
    atom = xmlRegNewAtom(am, XML_REGEXP_STRING);
    if (atom == NULL)
	return(NULL);
    if ((token2 == NULL) || (*token2 == 0)) {
	atom->valuep = xmlStrdup(token);
    } else {
	int lenn, lenp;
	xmlChar *str;

	lenn = strlen((char *) token2);
	lenp = strlen((char *) token);

	str = (xmlChar *) xmlMallocAtomic(lenn + lenp + 2);
	if (str == NULL) {
	    xmlRegFreeAtom(atom);
	    return(NULL);
	}
	memcpy(&str[0], token, lenp);
	str[lenp] = '|';
	memcpy(&str[lenp + 1], token2, lenn);
	str[lenn + lenp + 1] = 0;

	atom->valuep = str;
    }    
    atom->data = data;
    atom->quant = XML_REGEXP_QUANT_ONCEONLY;
    atom->min = min;
    atom->max = max;
    counter = xmlRegGetCounter(am);
    am->counters[counter].min = 1;
    am->counters[counter].max = 1;

    
    if (to == NULL) {
	to = xmlRegNewState(am);
	xmlRegStatePush(am, to);
    }
    xmlRegStateAddTrans(am, from, atom, to, counter, -1);
    xmlRegAtomPush(am, atom);
    am->state = to;
    return(to);
}

    

xmlAutomataStatePtr
xmlAutomataNewOnceTrans(xmlAutomataPtr am, xmlAutomataStatePtr from,
			 xmlAutomataStatePtr to, const xmlChar *token,
			 int min, int max, void *data) {
    xmlRegAtomPtr atom;
    int counter;

    if ((am == NULL) || (from == NULL) || (token == NULL))
	return(NULL);
    if (min < 1)
	return(NULL);
    if ((max < min) || (max < 1))
	return(NULL);
    atom = xmlRegNewAtom(am, XML_REGEXP_STRING);
    if (atom == NULL)
	return(NULL);
    atom->valuep = xmlStrdup(token);
    atom->data = data;
    atom->quant = XML_REGEXP_QUANT_ONCEONLY;
    atom->min = min;
    atom->max = max;
    counter = xmlRegGetCounter(am);
    am->counters[counter].min = 1;
    am->counters[counter].max = 1;

    
    if (to == NULL) {
	to = xmlRegNewState(am);
	xmlRegStatePush(am, to);
    }
    xmlRegStateAddTrans(am, from, atom, to, counter, -1);
    xmlRegAtomPush(am, atom);
    am->state = to;
    return(to);
}

xmlAutomataStatePtr
xmlAutomataNewState(xmlAutomataPtr am) {
    xmlAutomataStatePtr to; 

    if (am == NULL)
	return(NULL);
    to = xmlRegNewState(am);
    xmlRegStatePush(am, to);
    return(to);
}

xmlAutomataStatePtr
xmlAutomataNewEpsilon(xmlAutomataPtr am, xmlAutomataStatePtr from,
		      xmlAutomataStatePtr to) {
    if ((am == NULL) || (from == NULL))
	return(NULL);
    xmlFAGenerateEpsilonTransition(am, from, to);
    if (to == NULL)
	return(am->state);
    return(to);
}

xmlAutomataStatePtr
xmlAutomataNewAllTrans(xmlAutomataPtr am, xmlAutomataStatePtr from,
		       xmlAutomataStatePtr to, int lax) {
    if ((am == NULL) || (from == NULL))
	return(NULL);
    xmlFAGenerateAllTransition(am, from, to, lax);
    if (to == NULL)
	return(am->state);
    return(to);
}

int		
xmlAutomataNewCounter(xmlAutomataPtr am, int min, int max) {
    int ret;

    if (am == NULL)
	return(-1);

    ret = xmlRegGetCounter(am);
    if (ret < 0)
	return(-1);
    am->counters[ret].min = min;
    am->counters[ret].max = max;
    return(ret);
}

xmlAutomataStatePtr
xmlAutomataNewCountedTrans(xmlAutomataPtr am, xmlAutomataStatePtr from,
		xmlAutomataStatePtr to, int counter) {
    if ((am == NULL) || (from == NULL) || (counter < 0))
	return(NULL);
    xmlFAGenerateCountedEpsilonTransition(am, from, to, counter);
    if (to == NULL)
	return(am->state);
    return(to);
}

xmlAutomataStatePtr
xmlAutomataNewCounterTrans(xmlAutomataPtr am, xmlAutomataStatePtr from,
		xmlAutomataStatePtr to, int counter) {
    if ((am == NULL) || (from == NULL) || (counter < 0))
	return(NULL);
    xmlFAGenerateCountedTransition(am, from, to, counter);
    if (to == NULL)
	return(am->state);
    return(to);
}

xmlRegexpPtr          
xmlAutomataCompile(xmlAutomataPtr am) {
    xmlRegexpPtr ret;

    if ((am == NULL) || (am->error != 0)) return(NULL);
    xmlFAEliminateEpsilonTransitions(am);
    
    ret = xmlRegEpxFromParse(am);

    return(ret);
}

int          
xmlAutomataIsDeterminist(xmlAutomataPtr am) {
    int ret;

    if (am == NULL)
	return(-1);

    ret = xmlFAComputesDeterminism(am);
    return(ret);
}
#endif 

#ifdef LIBXML_EXPR_ENABLED

struct _xmlExpCtxt {
    xmlDictPtr dict;
    xmlExpNodePtr *table;
    int size;
    int nbElems;
    int nb_nodes;
    int maxNodes;
    const char *expr;
    const char *cur;
    int nb_cons;
    int tabSize;
};

xmlExpCtxtPtr
xmlExpNewCtxt(int maxNodes, xmlDictPtr dict) {
    xmlExpCtxtPtr ret;
    int size = 256;

    if (maxNodes <= 4096)
        maxNodes = 4096;
    
    ret = (xmlExpCtxtPtr) xmlMalloc(sizeof(xmlExpCtxt));
    if (ret == NULL)
        return(NULL);
    memset(ret, 0, sizeof(xmlExpCtxt));
    ret->size = size;
    ret->nbElems = 0;
    ret->maxNodes = maxNodes;
    ret->table = xmlMalloc(size * sizeof(xmlExpNodePtr));
    if (ret->table == NULL) {
        xmlFree(ret);
	return(NULL);
    }
    memset(ret->table, 0, size * sizeof(xmlExpNodePtr));
    if (dict == NULL) {
        ret->dict = xmlDictCreate();
	if (ret->dict == NULL) {
	    xmlFree(ret->table);
	    xmlFree(ret);
	    return(NULL);
	}
    } else {
        ret->dict = dict;
	xmlDictReference(ret->dict);
    }
    return(ret);
}

void
xmlExpFreeCtxt(xmlExpCtxtPtr ctxt) {
    if (ctxt == NULL)
        return;
    xmlDictFree(ctxt->dict);
    if (ctxt->table != NULL)
	xmlFree(ctxt->table);
    xmlFree(ctxt);
}

#define MAX_NODES 10000



typedef enum {
    XML_EXP_NILABLE = (1 << 0)
} xmlExpNodeInfo;

#define IS_NILLABLE(node) ((node)->info & XML_EXP_NILABLE)

struct _xmlExpNode {
    unsigned char type;
    unsigned char info;
    unsigned short key;	
    unsigned int ref;	
    int c_max;		
    xmlExpNodePtr exp_left;
    xmlExpNodePtr next;
    union {
	struct {
	    int f_min;
	    int f_max;
	} count;
	struct {
	    xmlExpNodePtr f_right;
	} children;
        const xmlChar *f_str;
    } field;
};

#define exp_min field.count.f_min
#define exp_max field.count.f_max
#define exp_right field.children.f_right
#define exp_str field.f_str

static xmlExpNodePtr xmlExpNewNode(xmlExpCtxtPtr ctxt, xmlExpNodeType type);
static xmlExpNode forbiddenExpNode = {
    XML_EXP_FORBID, 0, 0, 0, 0, NULL, NULL, {{ 0, 0}}
};
xmlExpNodePtr forbiddenExp = &forbiddenExpNode;
static xmlExpNode emptyExpNode = {
    XML_EXP_EMPTY, 1, 0, 0, 0, NULL, NULL, {{ 0, 0}}
};
xmlExpNodePtr emptyExp = &emptyExpNode;

static unsigned short
xmlExpHashNameComputeKey(const xmlChar *name) {
    unsigned short value = 0L;
    char ch;
    
    if (name != NULL) {
	value += 30 * (*name);
	while ((ch = *name++) != 0) {
	    value = value ^ ((value << 5) + (value >> 3) + (unsigned short)ch);
	}
    }
    return (value);
}

static unsigned short
xmlExpHashComputeKey(xmlExpNodeType type, xmlExpNodePtr left,
                     xmlExpNodePtr right) {
    unsigned long value;
    unsigned short ret;
    
    switch (type) {
        case XML_EXP_SEQ:
	    value = left->key;
	    value += right->key;
	    value *= 3;
	    ret = (unsigned short) value;
	    break;
        case XML_EXP_OR:
	    value = left->key;
	    value += right->key;
	    value *= 7;
	    ret = (unsigned short) value;
	    break;
        case XML_EXP_COUNT:
	    value = left->key;
	    value += right->key;
	    ret = (unsigned short) value;
	    break;
	default:
	    ret = 0;
    }
    return(ret);
}


static xmlExpNodePtr
xmlExpNewNode(xmlExpCtxtPtr ctxt, xmlExpNodeType type) {
    xmlExpNodePtr ret;

    if (ctxt->nb_nodes >= MAX_NODES)
        return(NULL);
    ret = (xmlExpNodePtr) xmlMalloc(sizeof(xmlExpNode));
    if (ret == NULL)
        return(NULL);
    memset(ret, 0, sizeof(xmlExpNode));
    ret->type = type;
    ret->next = NULL;
    ctxt->nb_nodes++;
    ctxt->nb_cons++;
    return(ret);
}

static xmlExpNodePtr
xmlExpHashGetEntry(xmlExpCtxtPtr ctxt, xmlExpNodeType type,
                   xmlExpNodePtr left, xmlExpNodePtr right,
		   const xmlChar *name, int min, int max) {
    unsigned short kbase, key;
    xmlExpNodePtr entry;
    xmlExpNodePtr insert;

    if (ctxt == NULL)
	return(NULL);

    if (type == XML_EXP_ATOM) {
	kbase = xmlExpHashNameComputeKey(name);
    } else if (type == XML_EXP_COUNT) {
        
	
	if (min == max) {
	    if (min == 1) {
		return(left);
	    }
	    if (min == 0) {
		xmlExpFree(ctxt, left);
	        return(emptyExp);
	    }
	}
	if (min < 0) {
	    xmlExpFree(ctxt, left);
	    return(forbiddenExp);
	}
        if (max == -1)
	    kbase = min + 79;
	else
	    kbase = max - min;
	kbase += left->key;
    } else if (type == XML_EXP_OR) {
        
        if (left->type == XML_EXP_FORBID) {
	    xmlExpFree(ctxt, left);
	    return(right);
	}
        if (right->type == XML_EXP_FORBID) {
	    xmlExpFree(ctxt, right);
	    return(left);
	}

        
	
        if (left == right) {
	    left->ref--;
	    return(left);
	}
        
	
        if ((left->type == XML_EXP_OR) && (right->type != XML_EXP_OR)) {
	    xmlExpNodePtr tmp = left;
            left = right;
	    right = tmp;
	}
        
	
        if (right->type == XML_EXP_OR) {
	    if ((left == right->exp_left) ||
	        (left == right->exp_right)) {
		xmlExpFree(ctxt, left);
		return(right);
	    }
	}
        
	
        if (left->type == XML_EXP_OR) {
	    xmlExpNodePtr tmp;

	    
	    if ((left->exp_right->type != XML_EXP_OR) &&
	        (left->exp_right->key < left->exp_left->key)) {
	        tmp = left->exp_right;
		left->exp_right = left->exp_left;
		left->exp_left = tmp;
	    }
	    left->exp_right->ref++;
	    tmp = xmlExpHashGetEntry(ctxt, XML_EXP_OR, left->exp_right, right,
	                             NULL, 0, 0);
	    left->exp_left->ref++;
	    tmp = xmlExpHashGetEntry(ctxt, XML_EXP_OR, left->exp_left, tmp,
	                             NULL, 0, 0);
	
	    xmlExpFree(ctxt, left);
	    return(tmp);
	}
	if (right->type == XML_EXP_OR) {
	    
	    
	    if (left->key > right->exp_right->key) {
		xmlExpNodePtr tmp;
		right->exp_right->ref++;
		tmp = xmlExpHashGetEntry(ctxt, XML_EXP_OR, right->exp_right,
		                         left, NULL, 0, 0);
		right->exp_left->ref++;
		tmp = xmlExpHashGetEntry(ctxt, XML_EXP_OR, right->exp_left,
		                         tmp, NULL, 0, 0);
		xmlExpFree(ctxt, right);
		return(tmp);
	    }
	    
	    
	    if (left->key > right->exp_left->key) {
		xmlExpNodePtr tmp;
		right->exp_right->ref++;
		tmp = xmlExpHashGetEntry(ctxt, XML_EXP_OR, left,
		                         right->exp_right, NULL, 0, 0);
		right->exp_left->ref++;
		tmp = xmlExpHashGetEntry(ctxt, XML_EXP_OR, right->exp_left,
		                         tmp, NULL, 0, 0);
		xmlExpFree(ctxt, right);
		return(tmp);
	    }
	}
	
        else if (left->key > right->key) {
	    xmlExpNodePtr tmp = left;
            left = right;
	    right = tmp;
	}
	kbase = xmlExpHashComputeKey(type, left, right);
    } else if (type == XML_EXP_SEQ) {
        
        if (left->type == XML_EXP_FORBID) {
	    xmlExpFree(ctxt, right);
	    return(left);
	}
        if (right->type == XML_EXP_FORBID) {
	    xmlExpFree(ctxt, left);
	    return(right);
	}
        
        if (right->type == XML_EXP_EMPTY) {
	    return(left);
	}
        if (left->type == XML_EXP_EMPTY) {
	    return(right);
	}
	kbase = xmlExpHashComputeKey(type, left, right);
    } else 
        return(NULL);

    key = kbase % ctxt->size;
    if (ctxt->table[key] != NULL) {
	for (insert = ctxt->table[key]; insert != NULL;
	     insert = insert->next) {
	    if ((insert->key == kbase) &&
	        (insert->type == type)) {
		if (type == XML_EXP_ATOM) {
		    if (name == insert->exp_str) {
			insert->ref++;
			return(insert);
		    }
		} else if (type == XML_EXP_COUNT) {
		    if ((insert->exp_min == min) && (insert->exp_max == max) &&
		        (insert->exp_left == left)) {
			insert->ref++;
			left->ref--;
			return(insert);
		    }
		} else if ((insert->exp_left == left) &&
			   (insert->exp_right == right)) {
		    insert->ref++;
		    left->ref--;
		    right->ref--;
		    return(insert);
		}
	    }
	}
    }

    entry = xmlExpNewNode(ctxt, type);
    if (entry == NULL)
        return(NULL);
    entry->key = kbase;
    if (type == XML_EXP_ATOM) {
	entry->exp_str = name;
	entry->c_max = 1;
    } else if (type == XML_EXP_COUNT) {
        entry->exp_min = min;
        entry->exp_max = max;
	entry->exp_left = left;
	if ((min == 0) || (IS_NILLABLE(left)))
	    entry->info |= XML_EXP_NILABLE;
	if (max < 0)
	    entry->c_max = -1;
	else
	    entry->c_max = max * entry->exp_left->c_max;
    } else {
	entry->exp_left = left;
	entry->exp_right = right;
	if (type == XML_EXP_OR) {
	    if ((IS_NILLABLE(left)) || (IS_NILLABLE(right)))
		entry->info |= XML_EXP_NILABLE;
	    if ((entry->exp_left->c_max == -1) ||
	        (entry->exp_right->c_max == -1))
		entry->c_max = -1;
	    else if (entry->exp_left->c_max > entry->exp_right->c_max)
	        entry->c_max = entry->exp_left->c_max;
	    else
	        entry->c_max = entry->exp_right->c_max;
	} else {
	    if ((IS_NILLABLE(left)) && (IS_NILLABLE(right)))
		entry->info |= XML_EXP_NILABLE;
	    if ((entry->exp_left->c_max == -1) ||
	        (entry->exp_right->c_max == -1))
		entry->c_max = -1;
	    else
	        entry->c_max = entry->exp_left->c_max + entry->exp_right->c_max;
	}
    }
    entry->ref = 1;
    if (ctxt->table[key] != NULL)
        entry->next = ctxt->table[key];

    ctxt->table[key] = entry;
    ctxt->nbElems++;

    return(entry);
}

void
xmlExpFree(xmlExpCtxtPtr ctxt, xmlExpNodePtr exp) {
    if ((exp == NULL) || (exp == forbiddenExp) || (exp == emptyExp))
        return;
    exp->ref--;
    if (exp->ref == 0) {
        unsigned short key;

        
	key = exp->key % ctxt->size;
	if (ctxt->table[key] == exp) {
	    ctxt->table[key] = exp->next;
	} else {
	    xmlExpNodePtr tmp;

	    tmp = ctxt->table[key];
	    while (tmp != NULL) {
	        if (tmp->next == exp) {
		    tmp->next = exp->next;
		    break;
		}
	        tmp = tmp->next;
	    }
	}

        if ((exp->type == XML_EXP_SEQ) || (exp->type == XML_EXP_OR)) {
	    xmlExpFree(ctxt, exp->exp_left);
	    xmlExpFree(ctxt, exp->exp_right);
	} else if (exp->type == XML_EXP_COUNT) {
	    xmlExpFree(ctxt, exp->exp_left);
	}
        xmlFree(exp);
	ctxt->nb_nodes--;
    }
}

void
xmlExpRef(xmlExpNodePtr exp) {
    if (exp != NULL)
        exp->ref++;
}

xmlExpNodePtr
xmlExpNewAtom(xmlExpCtxtPtr ctxt, const xmlChar *name, int len) {
    if ((ctxt == NULL) || (name == NULL))
        return(NULL);
    name = xmlDictLookup(ctxt->dict, name, len);
    if (name == NULL)
        return(NULL);
    return(xmlExpHashGetEntry(ctxt, XML_EXP_ATOM, NULL, NULL, name, 0, 0));
}

xmlExpNodePtr
xmlExpNewOr(xmlExpCtxtPtr ctxt, xmlExpNodePtr left, xmlExpNodePtr right) {
    if (ctxt == NULL)
        return(NULL);
    if ((left == NULL) || (right == NULL)) {
        xmlExpFree(ctxt, left);
        xmlExpFree(ctxt, right);
        return(NULL);
    }
    return(xmlExpHashGetEntry(ctxt, XML_EXP_OR, left, right, NULL, 0, 0));
}

xmlExpNodePtr
xmlExpNewSeq(xmlExpCtxtPtr ctxt, xmlExpNodePtr left, xmlExpNodePtr right) {
    if (ctxt == NULL)
        return(NULL);
    if ((left == NULL) || (right == NULL)) {
        xmlExpFree(ctxt, left);
        xmlExpFree(ctxt, right);
        return(NULL);
    }
    return(xmlExpHashGetEntry(ctxt, XML_EXP_SEQ, left, right, NULL, 0, 0));
}

xmlExpNodePtr
xmlExpNewRange(xmlExpCtxtPtr ctxt, xmlExpNodePtr subset, int min, int max) {
    if (ctxt == NULL)
        return(NULL);
    if ((subset == NULL) || (min < 0) || (max < -1) ||
        ((max >= 0) && (min > max))) {
	xmlExpFree(ctxt, subset);
        return(NULL);
    }
    return(xmlExpHashGetEntry(ctxt, XML_EXP_COUNT, subset,
                              NULL, NULL, min, max));
}


static int
xmlExpGetLanguageInt(xmlExpCtxtPtr ctxt, xmlExpNodePtr exp, 
                     const xmlChar**list, int len, int nb) {
    int tmp, tmp2;
tail:
    switch (exp->type) {
        case XML_EXP_EMPTY:
	    return(0);
        case XML_EXP_ATOM:
	    for (tmp = 0;tmp < nb;tmp++)
	        if (list[tmp] == exp->exp_str)
		    return(0);
            if (nb >= len)
	        return(-2);
	    list[nb] = exp->exp_str;
	    return(1);
        case XML_EXP_COUNT:
	    exp = exp->exp_left;
	    goto tail;
        case XML_EXP_SEQ:
        case XML_EXP_OR:
	    tmp = xmlExpGetLanguageInt(ctxt, exp->exp_left, list, len, nb);
	    if (tmp < 0)
	        return(tmp);
	    tmp2 = xmlExpGetLanguageInt(ctxt, exp->exp_right, list, len,
	                                nb + tmp);
	    if (tmp2 < 0)
	        return(tmp2);
            return(tmp + tmp2);
    }
    return(-1);
}

int
xmlExpGetLanguage(xmlExpCtxtPtr ctxt, xmlExpNodePtr exp, 
                  const xmlChar**langList, int len) {
    if ((ctxt == NULL) || (exp == NULL) || (langList == NULL) || (len <= 0))
        return(-1);
    return(xmlExpGetLanguageInt(ctxt, exp, langList, len, 0));
}

static int
xmlExpGetStartInt(xmlExpCtxtPtr ctxt, xmlExpNodePtr exp, 
                  const xmlChar**list, int len, int nb) {
    int tmp, tmp2;
tail:
    switch (exp->type) {
        case XML_EXP_FORBID:
	    return(0);
        case XML_EXP_EMPTY:
	    return(0);
        case XML_EXP_ATOM:
	    for (tmp = 0;tmp < nb;tmp++)
	        if (list[tmp] == exp->exp_str)
		    return(0);
            if (nb >= len)
	        return(-2);
	    list[nb] = exp->exp_str;
	    return(1);
        case XML_EXP_COUNT:
	    exp = exp->exp_left;
	    goto tail;
        case XML_EXP_SEQ:
	    tmp = xmlExpGetStartInt(ctxt, exp->exp_left, list, len, nb);
	    if (tmp < 0)
	        return(tmp);
	    if (IS_NILLABLE(exp->exp_left)) {
		tmp2 = xmlExpGetStartInt(ctxt, exp->exp_right, list, len,
					    nb + tmp);
		if (tmp2 < 0)
		    return(tmp2);
		tmp += tmp2;
	    }
            return(tmp);
        case XML_EXP_OR:
	    tmp = xmlExpGetStartInt(ctxt, exp->exp_left, list, len, nb);
	    if (tmp < 0)
	        return(tmp);
	    tmp2 = xmlExpGetStartInt(ctxt, exp->exp_right, list, len,
	                                nb + tmp);
	    if (tmp2 < 0)
	        return(tmp2);
            return(tmp + tmp2);
    }
    return(-1);
}

int
xmlExpGetStart(xmlExpCtxtPtr ctxt, xmlExpNodePtr exp, 
               const xmlChar**tokList, int len) {
    if ((ctxt == NULL) || (exp == NULL) || (tokList == NULL) || (len <= 0))
        return(-1);
    return(xmlExpGetStartInt(ctxt, exp, tokList, len, 0));
}

int
xmlExpIsNillable(xmlExpNodePtr exp) {
    if (exp == NULL)
        return(-1);
    return(IS_NILLABLE(exp) != 0);
}

static xmlExpNodePtr
xmlExpStringDeriveInt(xmlExpCtxtPtr ctxt, xmlExpNodePtr exp, const xmlChar *str)
{
    xmlExpNodePtr ret;

    switch (exp->type) {
	case XML_EXP_EMPTY:
	    return(forbiddenExp);
	case XML_EXP_FORBID:
	    return(forbiddenExp);
	case XML_EXP_ATOM:
	    if (exp->exp_str == str) {
#ifdef DEBUG_DERIV
		printf("deriv atom: equal => Empty\n");
#endif
	        ret = emptyExp;
	    } else {
#ifdef DEBUG_DERIV
		printf("deriv atom: mismatch => forbid\n");
#endif
	        
		ret = forbiddenExp;
	    }
	    return(ret);
	case XML_EXP_OR: {
	    xmlExpNodePtr tmp;

#ifdef DEBUG_DERIV
	    printf("deriv or: => or(derivs)\n");
#endif
	    tmp = xmlExpStringDeriveInt(ctxt, exp->exp_left, str);
	    if (tmp == NULL) {
		return(NULL);
	    }
	    ret = xmlExpStringDeriveInt(ctxt, exp->exp_right, str);
	    if (ret == NULL) {
	        xmlExpFree(ctxt, tmp);
		return(NULL);
	    }
            ret = xmlExpHashGetEntry(ctxt, XML_EXP_OR, tmp, ret,
			     NULL, 0, 0);
	    return(ret);
	}
	case XML_EXP_SEQ:
#ifdef DEBUG_DERIV
	    printf("deriv seq: starting with left\n");
#endif
	    ret = xmlExpStringDeriveInt(ctxt, exp->exp_left, str);
	    if (ret == NULL) {
	        return(NULL);
	    } else if (ret == forbiddenExp) {
	        if (IS_NILLABLE(exp->exp_left)) {
#ifdef DEBUG_DERIV
		    printf("deriv seq: left failed but nillable\n");
#endif
		    ret = xmlExpStringDeriveInt(ctxt, exp->exp_right, str);
		}
	    } else {
#ifdef DEBUG_DERIV
		printf("deriv seq: left match => sequence\n");
#endif
	        exp->exp_right->ref++;
	        ret = xmlExpHashGetEntry(ctxt, XML_EXP_SEQ, ret, exp->exp_right,
		                         NULL, 0, 0);
	    }
	    return(ret);
	case XML_EXP_COUNT: {
	    int min, max;
	    xmlExpNodePtr tmp;

	    if (exp->exp_max == 0)
		return(forbiddenExp);
	    ret = xmlExpStringDeriveInt(ctxt, exp->exp_left, str);
	    if (ret == NULL)
	        return(NULL);
	    if (ret == forbiddenExp) {
#ifdef DEBUG_DERIV
		printf("deriv count: pattern mismatch => forbid\n");
#endif
	        return(ret);
	    }
	    if (exp->exp_max == 1)
		return(ret);
	    if (exp->exp_max < 0) 
		max = -1;
	    else
		max = exp->exp_max - 1;
	    if (exp->exp_min > 0)
		min = exp->exp_min - 1;
	    else
		min = 0;
	    exp->exp_left->ref++;
	    tmp = xmlExpHashGetEntry(ctxt, XML_EXP_COUNT, exp->exp_left, NULL,
				     NULL, min, max);
	    if (ret == emptyExp) {
#ifdef DEBUG_DERIV
		printf("deriv count: match to empty => new count\n");
#endif
	        return(tmp);
	    }
#ifdef DEBUG_DERIV
	    printf("deriv count: match => sequence with new count\n");
#endif
	    return(xmlExpHashGetEntry(ctxt, XML_EXP_SEQ, ret, tmp,
	                              NULL, 0, 0));
	}
    }
    return(NULL);
}

xmlExpNodePtr
xmlExpStringDerive(xmlExpCtxtPtr ctxt, xmlExpNodePtr exp,
                   const xmlChar *str, int len) {
    const xmlChar *input;

    if ((exp == NULL) || (ctxt == NULL) || (str == NULL)) {
        return(NULL);
    }
    input = xmlDictExists(ctxt->dict, str, len);
    if (input == NULL) {
        return(forbiddenExp);
    }
    return(xmlExpStringDeriveInt(ctxt, exp, input));
}

static int
xmlExpCheckCard(xmlExpNodePtr exp, xmlExpNodePtr sub) {
    int ret = 1;

    if (sub->c_max == -1) {
        if (exp->c_max != -1)
	    ret = 0;
    } else if ((exp->c_max >= 0) && (exp->c_max < sub->c_max)) {
        ret = 0;
    }
#if 0
    if ((IS_NILLABLE(sub)) && (!IS_NILLABLE(exp)))
        ret = 0;
#endif
    return(ret);
}

static xmlExpNodePtr xmlExpExpDeriveInt(xmlExpCtxtPtr ctxt, xmlExpNodePtr exp,
                                        xmlExpNodePtr sub);

static int
xmlExpDivide(xmlExpCtxtPtr ctxt, xmlExpNodePtr exp, xmlExpNodePtr sub,
             xmlExpNodePtr *mult, xmlExpNodePtr *remain) {
    int i;
    xmlExpNodePtr tmp, tmp2;

    if (mult != NULL) *mult = NULL;
    if (remain != NULL) *remain = NULL;
    if (exp->c_max == -1) return(0);
    if (IS_NILLABLE(exp) && (!IS_NILLABLE(sub))) return(0);

    for (i = 1;i <= exp->c_max;i++) {
        sub->ref++;
        tmp = xmlExpHashGetEntry(ctxt, XML_EXP_COUNT,
				 sub, NULL, NULL, i, i);
	if (tmp == NULL) {
	    return(-1);
	}
	if (!xmlExpCheckCard(tmp, exp)) {
	    xmlExpFree(ctxt, tmp);
	    continue;
	}
	tmp2 = xmlExpExpDeriveInt(ctxt, tmp, exp);
	if (tmp2 == NULL) {
	    xmlExpFree(ctxt, tmp);
	    return(-1);
	}
	if ((tmp2 != forbiddenExp) && (IS_NILLABLE(tmp2))) {
	    if (remain != NULL)
	        *remain = tmp2;
	    else
	        xmlExpFree(ctxt, tmp2);
	    if (mult != NULL)
	        *mult = tmp;
	    else
	        xmlExpFree(ctxt, tmp);
#ifdef DEBUG_DERIV
	    printf("Divide succeeded %d\n", i);
#endif
	    return(i);
	}
	xmlExpFree(ctxt, tmp);
	xmlExpFree(ctxt, tmp2);
    }
#ifdef DEBUG_DERIV
    printf("Divide failed\n");
#endif
    return(0);
}

static xmlExpNodePtr
xmlExpExpDeriveInt(xmlExpCtxtPtr ctxt, xmlExpNodePtr exp, xmlExpNodePtr sub) {
    xmlExpNodePtr ret, tmp, tmp2, tmp3;
    const xmlChar **tab;
    int len, i;

    if ((exp == sub) && (exp->c_max >= 0)) {
#ifdef DEBUG_DERIV
        printf("Equal(exp, sub) and finite -> Empty\n");
#endif
        return(emptyExp);
    }
    if (sub->type == XML_EXP_EMPTY) {
#ifdef DEBUG_DERIV
        printf("Empty(sub) -> Empty\n");
#endif
	exp->ref++;
        return(exp);
    }
    if (sub->type == XML_EXP_SEQ) {
#ifdef DEBUG_DERIV
        printf("Seq(sub) -> decompose\n");
#endif
        tmp = xmlExpExpDeriveInt(ctxt, exp, sub->exp_left);
	if (tmp == NULL)
	    return(NULL);
	if (tmp == forbiddenExp)
	    return(tmp);
	ret = xmlExpExpDeriveInt(ctxt, tmp, sub->exp_right);
	xmlExpFree(ctxt, tmp);
	return(ret);
    }
    if (sub->type == XML_EXP_OR) {
#ifdef DEBUG_DERIV
        printf("Or(sub) -> decompose\n");
#endif
        tmp = xmlExpExpDeriveInt(ctxt, exp, sub->exp_left);
	if (tmp == forbiddenExp)
	    return(tmp);
	if (tmp == NULL)
	    return(NULL);
	ret = xmlExpExpDeriveInt(ctxt, exp, sub->exp_right);
	if ((ret == NULL) || (ret == forbiddenExp)) {
	    xmlExpFree(ctxt, tmp);
	    return(ret);
	}
	return(xmlExpHashGetEntry(ctxt, XML_EXP_OR, tmp, ret, NULL, 0, 0));
    }
    if (!xmlExpCheckCard(exp, sub)) {
#ifdef DEBUG_DERIV
        printf("CheckCard(exp, sub) failed -> Forbid\n");
#endif
        return(forbiddenExp);
    }
    switch (exp->type) {
        case XML_EXP_EMPTY:
	    if (sub == emptyExp)
	        return(emptyExp);
#ifdef DEBUG_DERIV
	    printf("Empty(exp) -> Forbid\n");
#endif
	    return(forbiddenExp);
        case XML_EXP_FORBID:
#ifdef DEBUG_DERIV
	    printf("Forbid(exp) -> Forbid\n");
#endif
	    return(forbiddenExp);
        case XML_EXP_ATOM:
	    if (sub->type == XML_EXP_ATOM) {
	        
	        if (exp->exp_str == sub->exp_str) {
#ifdef DEBUG_DERIV
		    printf("Atom match -> Empty\n");
#endif
		    return(emptyExp);
                }
#ifdef DEBUG_DERIV
		printf("Atom mismatch -> Forbid\n");
#endif
	        return(forbiddenExp);
	    }
	    if ((sub->type == XML_EXP_COUNT) &&
	        (sub->exp_max == 1) &&
	        (sub->exp_left->type == XML_EXP_ATOM)) {
	        
	        if (exp->exp_str == sub->exp_left->exp_str) {
#ifdef DEBUG_DERIV
		    printf("Atom match -> Empty\n");
#endif
		    return(emptyExp);
		}
#ifdef DEBUG_DERIV
		printf("Atom mismatch -> Forbid\n");
#endif
	        return(forbiddenExp);
	    }
#ifdef DEBUG_DERIV
	    printf("Compex exp vs Atom -> Forbid\n");
#endif
	    return(forbiddenExp);
        case XML_EXP_SEQ:
	    
	    if (xmlExpCheckCard(exp->exp_left, sub)) {
		
#ifdef DEBUG_DERIV
		printf("Seq trying left only\n");
#endif
		ret = xmlExpExpDeriveInt(ctxt, exp->exp_left, sub);
		if ((ret != forbiddenExp) && (ret != NULL)) {
#ifdef DEBUG_DERIV
		    printf("Seq trying left only worked\n");
#endif
		    exp->exp_right->ref++;
		    return(xmlExpHashGetEntry(ctxt, XML_EXP_SEQ, ret,
					      exp->exp_right, NULL, 0, 0));
		}
#ifdef DEBUG_DERIV
	    } else {
		printf("Seq: left too short\n");
#endif
	    }
	    
	    if (sub->type == XML_EXP_COUNT) {
		int min, max;

#ifdef DEBUG_DERIV
		printf("Seq: sub is a count\n");
#endif
	        ret = xmlExpExpDeriveInt(ctxt, exp->exp_left, sub->exp_left);
		if (ret == NULL)
		    return(NULL);
		if (ret != forbiddenExp) {
#ifdef DEBUG_DERIV
		    printf("Seq , Count match on left\n");
#endif
		    if (sub->exp_max < 0)
		        max = -1;
	            else
		        max = sub->exp_max -1;
		    if (sub->exp_min > 0)
		        min = sub->exp_min -1;
		    else
		        min = 0;
		    exp->exp_right->ref++;
		    tmp = xmlExpHashGetEntry(ctxt, XML_EXP_SEQ, ret,
		                             exp->exp_right, NULL, 0, 0);
		    if (tmp == NULL)
		        return(NULL);

		    sub->exp_left->ref++;
		    tmp2 = xmlExpHashGetEntry(ctxt, XML_EXP_COUNT,
				      sub->exp_left, NULL, NULL, min, max);
		    if (tmp2 == NULL) {
		        xmlExpFree(ctxt, tmp);
			return(NULL);
		    }
		    ret = xmlExpExpDeriveInt(ctxt, tmp, tmp2);
		    xmlExpFree(ctxt, tmp);
		    xmlExpFree(ctxt, tmp2);
		    return(ret);
		}
	    }
	    
	    break;
        case XML_EXP_OR:
#ifdef DEBUG_DERIV
	    printf("Or , trying both side\n");
#endif
	    ret = xmlExpExpDeriveInt(ctxt, exp->exp_left, sub);
	    if (ret == NULL)
	        return(NULL);
	    tmp = xmlExpExpDeriveInt(ctxt, exp->exp_right, sub);
	    if (tmp == NULL) {
		xmlExpFree(ctxt, ret);
	        return(NULL);
	    }
	    return(xmlExpHashGetEntry(ctxt, XML_EXP_OR, ret, tmp, NULL, 0, 0));
        case XML_EXP_COUNT: {
	    int min, max;

	    if (sub->type == XML_EXP_COUNT) {
	        tmp = xmlExpExpDeriveInt(ctxt, exp->exp_left, sub->exp_left);
		if (tmp == NULL)
		    return(NULL);
		if (tmp == forbiddenExp) {
		    int mult;

#ifdef DEBUG_DERIV
		    printf("Count, Count inner don't subsume\n");
#endif
		    mult = xmlExpDivide(ctxt, sub->exp_left, exp->exp_left,
		                        NULL, &tmp);
		    if (mult <= 0) {
#ifdef DEBUG_DERIV
			printf("Count, Count not multiple => forbidden\n");
#endif
                        return(forbiddenExp);
		    }
		    if (sub->exp_max == -1) {
		        max = -1;
			if (exp->exp_max == -1) {
			    if (exp->exp_min <= sub->exp_min * mult)
			        min = 0;
			    else
			        min = exp->exp_min - sub->exp_min * mult;
			} else {
#ifdef DEBUG_DERIV
			    printf("Count, Count finite can't subsume infinite\n");
#endif
                            xmlExpFree(ctxt, tmp);
			    return(forbiddenExp);
			}
		    } else {
			if (exp->exp_max == -1) {
#ifdef DEBUG_DERIV
			    printf("Infinite loop consume mult finite loop\n");
#endif
			    if (exp->exp_min > sub->exp_min * mult) {
				max = -1;
				min = exp->exp_min - sub->exp_min * mult;
			    } else {
				max = -1;
				min = 0;
			    }
			} else {
			    if (exp->exp_max < sub->exp_max * mult) {
#ifdef DEBUG_DERIV
				printf("loops max mult mismatch => forbidden\n");
#endif
				xmlExpFree(ctxt, tmp);
				return(forbiddenExp);
			    }
			    if (sub->exp_max * mult > exp->exp_min)
				min = 0;
			    else
				min = exp->exp_min - sub->exp_max * mult;
			    max = exp->exp_max - sub->exp_max * mult;
			}
		    }
		} else if (!IS_NILLABLE(tmp)) {
#ifdef DEBUG_DERIV
		    printf("Count, Count remain not nillable => forbidden\n");
#endif
		    xmlExpFree(ctxt, tmp);
		    return(forbiddenExp);
		} else if (sub->exp_max == -1) {
		    if (exp->exp_max == -1) {
		        if (exp->exp_min <= sub->exp_min) {
#ifdef DEBUG_DERIV
			    printf("Infinite loops Okay => COUNT(0,Inf)\n");
#endif
                            max = -1;
			    min = 0;
			} else {
#ifdef DEBUG_DERIV
			    printf("Infinite loops min => Count(X,Inf)\n");
#endif
                            max = -1;
			    min = exp->exp_min - sub->exp_min;
			}
		    } else if (exp->exp_min > sub->exp_min) {
#ifdef DEBUG_DERIV
			printf("loops min mismatch 1 => forbidden ???\n");
#endif
		        xmlExpFree(ctxt, tmp);
		        return(forbiddenExp);
		    } else {
			max = -1;
			min = 0;
		    }
		} else {
		    if (exp->exp_max == -1) {
#ifdef DEBUG_DERIV
			printf("Infinite loop consume finite loop\n");
#endif
		        if (exp->exp_min > sub->exp_min) {
			    max = -1;
			    min = exp->exp_min - sub->exp_min;
			} else {
			    max = -1;
			    min = 0;
			}
		    } else {
		        if (exp->exp_max < sub->exp_max) {
#ifdef DEBUG_DERIV
			    printf("loops max mismatch => forbidden\n");
#endif
			    xmlExpFree(ctxt, tmp);
			    return(forbiddenExp);
			}
			if (sub->exp_max > exp->exp_min)
			    min = 0;
			else
			    min = exp->exp_min - sub->exp_max;
			max = exp->exp_max - sub->exp_max;
		    }
		}
#ifdef DEBUG_DERIV
		printf("loops match => SEQ(COUNT())\n");
#endif
		exp->exp_left->ref++;
		tmp2 = xmlExpHashGetEntry(ctxt, XML_EXP_COUNT, exp->exp_left,
		                          NULL, NULL, min, max);
		if (tmp2 == NULL) {
		    return(NULL);
		}
                ret = xmlExpHashGetEntry(ctxt, XML_EXP_SEQ, tmp, tmp2,
		                         NULL, 0, 0);
		return(ret);
	    }
	    tmp = xmlExpExpDeriveInt(ctxt, exp->exp_left, sub);
	    if (tmp == NULL)
		return(NULL);
	    if (tmp == forbiddenExp) {
#ifdef DEBUG_DERIV
		printf("loop mismatch => forbidden\n");
#endif
		return(forbiddenExp);
	    }
	    if (exp->exp_min > 0)
		min = exp->exp_min - 1;
	    else
		min = 0;
	    if (exp->exp_max < 0)
		max = -1;
	    else
		max = exp->exp_max - 1;

#ifdef DEBUG_DERIV
	    printf("loop match => SEQ(COUNT())\n");
#endif
	    exp->exp_left->ref++;
	    tmp2 = xmlExpHashGetEntry(ctxt, XML_EXP_COUNT, exp->exp_left,
				      NULL, NULL, min, max);
	    if (tmp2 == NULL)
		return(NULL);
	    ret = xmlExpHashGetEntry(ctxt, XML_EXP_SEQ, tmp, tmp2,
				     NULL, 0, 0);
	    return(ret);
	}
    }

#ifdef DEBUG_DERIV
    printf("Fallback to derivative\n");
#endif
    if (IS_NILLABLE(sub)) {
        if (!(IS_NILLABLE(exp)))
	    return(forbiddenExp);
	else
	    ret = emptyExp;
    } else
	ret = NULL;
    if (ctxt->tabSize == 0)
        ctxt->tabSize = 40;

    tab = (const xmlChar **) xmlMalloc(ctxt->tabSize *
	                               sizeof(const xmlChar *));
    if (tab == NULL) {
	return(NULL);
    }

    len = xmlExpGetStartInt(ctxt, sub, tab, ctxt->tabSize, 0);
    while (len < 0) {
        const xmlChar **temp;
	temp = (const xmlChar **) xmlRealloc((xmlChar **) tab, ctxt->tabSize * 2 *
	                                     sizeof(const xmlChar *));
	if (temp == NULL) {
	    xmlFree((xmlChar **) tab);
	    return(NULL);
	}
	tab = temp;
	ctxt->tabSize *= 2;
	len = xmlExpGetStartInt(ctxt, sub, tab, ctxt->tabSize, 0);
    }
    for (i = 0;i < len;i++) {
        tmp = xmlExpStringDeriveInt(ctxt, exp, tab[i]);
	if ((tmp == NULL) || (tmp == forbiddenExp)) {
	    xmlExpFree(ctxt, ret);
	    xmlFree((xmlChar **) tab);
	    return(tmp);
	}
	tmp2 = xmlExpStringDeriveInt(ctxt, sub, tab[i]);
	if ((tmp2 == NULL) || (tmp2 == forbiddenExp)) {
	    xmlExpFree(ctxt, tmp);
	    xmlExpFree(ctxt, ret);
	    xmlFree((xmlChar **) tab);
	    return(tmp);
	}
	tmp3 = xmlExpExpDeriveInt(ctxt, tmp, tmp2);
	xmlExpFree(ctxt, tmp);
	xmlExpFree(ctxt, tmp2);

	if ((tmp3 == NULL) || (tmp3 == forbiddenExp)) {
	    xmlExpFree(ctxt, ret);
	    xmlFree((xmlChar **) tab);
	    return(tmp3);
	}

	if (ret == NULL)
	    ret = tmp3;
	else {
	    ret = xmlExpHashGetEntry(ctxt, XML_EXP_OR, ret, tmp3, NULL, 0, 0);
	    if (ret == NULL) {
		xmlFree((xmlChar **) tab);
	        return(NULL);
	    }
	}
    }
    xmlFree((xmlChar **) tab);
    return(ret);
}
    
xmlExpNodePtr
xmlExpExpDerive(xmlExpCtxtPtr ctxt, xmlExpNodePtr exp, xmlExpNodePtr sub) {
    if ((exp == NULL) || (ctxt == NULL) || (sub == NULL))
        return(NULL);

    if (IS_NILLABLE(sub) && (!IS_NILLABLE(exp))) {
#ifdef DEBUG_DERIV
	printf("Sub nillable and not exp : can't subsume\n");
#endif
        return(forbiddenExp);
    }
    if (xmlExpCheckCard(exp, sub) == 0) {
#ifdef DEBUG_DERIV
	printf("sub generate longuer sequances than exp : can't subsume\n");
#endif
        return(forbiddenExp);
    }
    return(xmlExpExpDeriveInt(ctxt, exp, sub));
}

int
xmlExpSubsume(xmlExpCtxtPtr ctxt, xmlExpNodePtr exp, xmlExpNodePtr sub) {
    xmlExpNodePtr tmp;
    
    if ((exp == NULL) || (ctxt == NULL) || (sub == NULL))
        return(-1);

    if (IS_NILLABLE(sub) && (!IS_NILLABLE(exp))) {
#ifdef DEBUG_DERIV
	printf("Sub nillable and not exp : can't subsume\n");
#endif
        return(0);
    }
    if (xmlExpCheckCard(exp, sub) == 0) {
#ifdef DEBUG_DERIV
	printf("sub generate longuer sequances than exp : can't subsume\n");
#endif
        return(0);
    }
    tmp = xmlExpExpDeriveInt(ctxt, exp, sub);
#ifdef DEBUG_DERIV
    printf("Result derivation :\n");
    PRINT_EXP(tmp);
#endif
    if (tmp == NULL)
        return(-1);
    if (tmp == forbiddenExp)
	return(0);
    if (tmp == emptyExp)
	return(1);
    if ((tmp != NULL) && (IS_NILLABLE(tmp))) {
        xmlExpFree(ctxt, tmp);
        return(1);
    }
    xmlExpFree(ctxt, tmp);
    return(0);
}


static xmlExpNodePtr xmlExpParseExpr(xmlExpCtxtPtr ctxt);

#undef CUR
#define CUR (*ctxt->cur)
#undef NEXT
#define NEXT ctxt->cur++;
#undef IS_BLANK
#define IS_BLANK(c) ((c == ' ') || (c == '\n') || (c == '\r') || (c == '\t'))
#define SKIP_BLANKS while (IS_BLANK(*ctxt->cur)) ctxt->cur++;

static int
xmlExpParseNumber(xmlExpCtxtPtr ctxt) {
    int ret = 0;

    SKIP_BLANKS
    if (CUR == '*') {
	NEXT
	return(-1);
    }
    if ((CUR < '0') || (CUR > '9'))
        return(-1);
    while ((CUR >= '0') && (CUR <= '9')) {
        ret = ret * 10 + (CUR - '0');
	NEXT
    }
    return(ret);
}

static xmlExpNodePtr
xmlExpParseOr(xmlExpCtxtPtr ctxt) {
    const char *base;
    xmlExpNodePtr ret;
    const xmlChar *val;

    SKIP_BLANKS
    base = ctxt->cur;
    if (*ctxt->cur == '(') {
        NEXT
	ret = xmlExpParseExpr(ctxt);
	SKIP_BLANKS
	if (*ctxt->cur != ')') {
	    fprintf(stderr, "unbalanced '(' : %s\n", base);
	    xmlExpFree(ctxt, ret);
	    return(NULL);
	}
	NEXT;
	SKIP_BLANKS
	goto parse_quantifier;
    }
    while ((CUR != 0) && (!(IS_BLANK(CUR))) && (CUR != '(') &&
           (CUR != ')') && (CUR != '|') && (CUR != ',') && (CUR != '{') &&
	   (CUR != '*') && (CUR != '+') && (CUR != '?') && (CUR != '}'))
	NEXT;
    val = xmlDictLookup(ctxt->dict, BAD_CAST base, ctxt->cur - base);
    if (val == NULL)
        return(NULL);
    ret = xmlExpHashGetEntry(ctxt, XML_EXP_ATOM, NULL, NULL, val, 0, 0);
    if (ret == NULL)
        return(NULL);
    SKIP_BLANKS
parse_quantifier:
    if (CUR == '{') {
        int min, max;

        NEXT
	min = xmlExpParseNumber(ctxt);
	if (min < 0) {
	    xmlExpFree(ctxt, ret);
	    return(NULL);
	}
	SKIP_BLANKS
	if (CUR == ',') {
	    NEXT
	    max = xmlExpParseNumber(ctxt);
	    SKIP_BLANKS
	} else
	    max = min;
	if (CUR != '}') {
	    xmlExpFree(ctxt, ret);
	    return(NULL);
	}
        NEXT
	ret = xmlExpHashGetEntry(ctxt, XML_EXP_COUNT, ret, NULL, NULL,
	                         min, max);
	SKIP_BLANKS
    } else if (CUR == '?') {
        NEXT
	ret = xmlExpHashGetEntry(ctxt, XML_EXP_COUNT, ret, NULL, NULL,
	                         0, 1);
	SKIP_BLANKS
    } else if (CUR == '+') {
        NEXT
	ret = xmlExpHashGetEntry(ctxt, XML_EXP_COUNT, ret, NULL, NULL,
	                         1, -1);
	SKIP_BLANKS
    } else if (CUR == '*') {
        NEXT
	ret = xmlExpHashGetEntry(ctxt, XML_EXP_COUNT, ret, NULL, NULL,
	                         0, -1);
	SKIP_BLANKS
    } 
    return(ret);
}


static xmlExpNodePtr
xmlExpParseSeq(xmlExpCtxtPtr ctxt) {
    xmlExpNodePtr ret, right;

    ret = xmlExpParseOr(ctxt);
    SKIP_BLANKS
    while (CUR == '|') {
        NEXT
	right = xmlExpParseOr(ctxt);
	if (right == NULL) {
	    xmlExpFree(ctxt, ret);
	    return(NULL);
	}
	ret = xmlExpHashGetEntry(ctxt, XML_EXP_OR, ret, right, NULL, 0, 0);
	if (ret == NULL)
	    return(NULL);
    }
    return(ret);
}

static xmlExpNodePtr
xmlExpParseExpr(xmlExpCtxtPtr ctxt) {
    xmlExpNodePtr ret, right;

    ret = xmlExpParseSeq(ctxt);
    SKIP_BLANKS
    while (CUR == ',') {
        NEXT
	right = xmlExpParseSeq(ctxt);
	if (right == NULL) {
	    xmlExpFree(ctxt, ret);
	    return(NULL);
	}
	ret = xmlExpHashGetEntry(ctxt, XML_EXP_SEQ, ret, right, NULL, 0, 0);
	if (ret == NULL)
	    return(NULL);
    }
    return(ret);
}

xmlExpNodePtr
xmlExpParse(xmlExpCtxtPtr ctxt, const char *expr) {
    xmlExpNodePtr ret;

    ctxt->expr = expr;
    ctxt->cur = expr;

    ret = xmlExpParseExpr(ctxt);
    SKIP_BLANKS
    if (*ctxt->cur != 0) {
        xmlExpFree(ctxt, ret);
        return(NULL);
    }
    return(ret);
}

static void
xmlExpDumpInt(xmlBufferPtr buf, xmlExpNodePtr expr, int glob) {
    xmlExpNodePtr c;

    if (expr == NULL) return;
    if (glob) xmlBufferWriteChar(buf, "(");
    switch (expr->type) {
        case XML_EXP_EMPTY:
	    xmlBufferWriteChar(buf, "empty");
	    break;
        case XML_EXP_FORBID:
	    xmlBufferWriteChar(buf, "forbidden");
	    break;
        case XML_EXP_ATOM:
	    xmlBufferWriteCHAR(buf, expr->exp_str);
	    break;
        case XML_EXP_SEQ:
	    c = expr->exp_left;
	    if ((c->type == XML_EXP_SEQ) || (c->type == XML_EXP_OR))
	        xmlExpDumpInt(buf, c, 1);
	    else
	        xmlExpDumpInt(buf, c, 0);
	    xmlBufferWriteChar(buf, " , ");
	    c = expr->exp_right;
	    if ((c->type == XML_EXP_SEQ) || (c->type == XML_EXP_OR))
	        xmlExpDumpInt(buf, c, 1);
	    else
	        xmlExpDumpInt(buf, c, 0);
            break;
        case XML_EXP_OR:
	    c = expr->exp_left;
	    if ((c->type == XML_EXP_SEQ) || (c->type == XML_EXP_OR))
	        xmlExpDumpInt(buf, c, 1);
	    else
	        xmlExpDumpInt(buf, c, 0);
	    xmlBufferWriteChar(buf, " | ");
	    c = expr->exp_right;
	    if ((c->type == XML_EXP_SEQ) || (c->type == XML_EXP_OR))
	        xmlExpDumpInt(buf, c, 1);
	    else
	        xmlExpDumpInt(buf, c, 0);
            break;
        case XML_EXP_COUNT: {
	    char rep[40];
	    
	    c = expr->exp_left;
	    if ((c->type == XML_EXP_SEQ) || (c->type == XML_EXP_OR))
	        xmlExpDumpInt(buf, c, 1);
	    else
	        xmlExpDumpInt(buf, c, 0);
	    if ((expr->exp_min == 0) && (expr->exp_max == 1)) {
		rep[0] = '?';
		rep[1] = 0;
	    } else if ((expr->exp_min == 0) && (expr->exp_max == -1)) {
		rep[0] = '*';
		rep[1] = 0;
	    } else if ((expr->exp_min == 1) && (expr->exp_max == -1)) {
		rep[0] = '+';
		rep[1] = 0;
	    } else if (expr->exp_max == expr->exp_min) {
	        snprintf(rep, 39, "{%d}", expr->exp_min);
	    } else if (expr->exp_max < 0) {
	        snprintf(rep, 39, "{%d,inf}", expr->exp_min);
	    } else {
	        snprintf(rep, 39, "{%d,%d}", expr->exp_min, expr->exp_max);
	    }
	    rep[39] = 0;
	    xmlBufferWriteChar(buf, rep);
	    break;
	}
	default:
	    fprintf(stderr, "Error in tree\n");
    }
    if (glob)
        xmlBufferWriteChar(buf, ")");
}
void
xmlExpDump(xmlBufferPtr buf, xmlExpNodePtr expr) {
    if ((buf == NULL) || (expr == NULL))
        return;
    xmlExpDumpInt(buf, expr, 0);
}

int
xmlExpMaxToken(xmlExpNodePtr expr) {
    if (expr == NULL)
        return(-1);
    return(expr->c_max);
}

int
xmlExpCtxtNbNodes(xmlExpCtxtPtr ctxt) {
    if (ctxt == NULL)
        return(-1);
    return(ctxt->nb_nodes);
}

int
xmlExpCtxtNbCons(xmlExpCtxtPtr ctxt) {
    if (ctxt == NULL)
        return(-1);
    return(ctxt->nb_cons);
}

#endif 
#define bottom_xmlregexp
#include "elfgcchack.h"
#endif 
