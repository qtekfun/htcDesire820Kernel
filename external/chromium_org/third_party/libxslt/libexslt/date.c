/*
 * date.c: Implementation of the EXSLT -- Dates and Times module
 *
 * References:
 *   http://www.exslt.org/date/date.html
 *
 * See Copyright for the status of this software.
 *
 * Authors:
 *   Charlie Bozeman <cbozeman@HiWAAY.net>
 *   Thomas Broyer <tbroyer@ltgt.net>
 *
 * TODO:
 * elements:
 *   date-format
 * functions:
 *   format-date
 *   parse-date
 *   sum
 */

#define IN_LIBEXSLT
#include "libexslt/libexslt.h"

#if defined(WIN32) && !defined (__CYGWIN__) && (!__MINGW32__)
#include <win32config.h>
#else
#include "config.h"
#endif

#if HAVE_LOCALTIME_R	
#ifndef _AIX51		
#define _POSIX_SOURCE
#endif
#endif

#include <libxml/tree.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>

#include <libxslt/xsltconfig.h>
#include <libxslt/xsltutils.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/extensions.h>

#include "exslt.h"

#include <string.h>

#ifdef HAVE_MATH_H
#include <math.h>
#endif

#ifdef __sun
#ifndef __EXTENSIONS__
#define __EXTENSIONS__
#endif
#endif

#ifdef HAVE_TIME_H
#include <time.h>
#endif

typedef enum {
    EXSLT_UNKNOWN  =    0,
    XS_TIME        =    1,       
    XS_GDAY        = (XS_TIME   << 1),
    XS_GMONTH      = (XS_GDAY   << 1),
    XS_GMONTHDAY   = (XS_GMONTH | XS_GDAY),
    XS_GYEAR       = (XS_GMONTH << 1),
    XS_GYEARMONTH  = (XS_GYEAR  | XS_GMONTH),
    XS_DATE        = (XS_GYEAR  | XS_GMONTH | XS_GDAY),
    XS_DATETIME    = (XS_DATE   | XS_TIME),
    XS_DURATION    = (XS_GYEAR  << 1)
} exsltDateType;

typedef struct _exsltDateValDate exsltDateValDate;
typedef exsltDateValDate *exsltDateValDatePtr;
struct _exsltDateValDate {
    long		year;
    unsigned int	mon	:4;	
    unsigned int	day	:5;	
    unsigned int	hour	:5;	
    unsigned int	min	:6;	
    double		sec;
    unsigned int	tz_flag	:1;	
    signed int		tzo	:12;	
};

typedef struct _exsltDateValDuration exsltDateValDuration;
typedef exsltDateValDuration *exsltDateValDurationPtr;
struct _exsltDateValDuration {
    long	        mon;		
    long        	day;
    double		sec;            
};

typedef struct _exsltDateVal exsltDateVal;
typedef exsltDateVal *exsltDateValPtr;
struct _exsltDateVal {
    exsltDateType       type;
    union {
        exsltDateValDate        date;
        exsltDateValDuration    dur;
    } value;
};


#if defined(HAVE_TIME_H) 					\
    && (defined(HAVE_LOCALTIME) || defined(HAVE_LOCALTIME_R))	\
    && (defined(HAVE_GMTIME) || defined(HAVE_GMTIME_R))		\
    && defined(HAVE_TIME)
#define WITH_TIME
#endif


#define IS_TZO_CHAR(c)						\
	((c == 0) || (c == 'Z') || (c == '+') || (c == '-'))

#define VALID_ALWAYS(num)	(num >= 0)
#define VALID_YEAR(yr)          (yr != 0)
#define VALID_MONTH(mon)        ((mon >= 1) && (mon <= 12))
#define VALID_DAY(day)          ((day >= 1) && (day <= 31))
#define VALID_HOUR(hr)          ((hr >= 0) && (hr <= 23))
#define VALID_MIN(min)          ((min >= 0) && (min <= 59))
#define VALID_SEC(sec)          ((sec >= 0) && (sec < 60))
#define VALID_TZO(tzo)          ((tzo > -1440) && (tzo < 1440))
#define IS_LEAP(y)						\
	(((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0))

static const unsigned long daysInMonth[12] =
	{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
static const unsigned long daysInMonthLeap[12] =
	{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

#define MAX_DAYINMONTH(yr,mon)                                  \
        (IS_LEAP(yr) ? daysInMonthLeap[mon - 1] : daysInMonth[mon - 1])

#define VALID_MDAY(dt)						\
	(IS_LEAP(dt->year) ?				        \
	    (dt->day <= daysInMonthLeap[dt->mon - 1]) :	        \
	    (dt->day <= daysInMonth[dt->mon - 1]))

#define VALID_DATE(dt)						\
	(VALID_YEAR(dt->year) && VALID_MONTH(dt->mon) && VALID_MDAY(dt))

#define VALID_TIME(dt)						\
	((dt->hour <=23 ) && (dt->min <= 59) &&			\
	 VALID_SEC(dt->sec) && VALID_TZO(dt->tzo))

#define VALID_DATETIME(dt)					\
	(VALID_DATE(dt) && VALID_TIME(dt))

#define SECS_PER_MIN            (60)
#define SECS_PER_HOUR           (60 * SECS_PER_MIN)
#define SECS_PER_DAY            (24 * SECS_PER_HOUR)

static const unsigned long dayInYearByMonth[12] =
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
static const unsigned long dayInLeapYearByMonth[12] =
	{ 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };

#define DAY_IN_YEAR(day, month, year)				\
        ((IS_LEAP(year) ?					\
                dayInLeapYearByMonth[month - 1] :		\
                dayInYearByMonth[month - 1]) + day)

static int
_exsltDateParseGYear (exsltDateValDatePtr dt, const xmlChar **str)
{
    const xmlChar *cur = *str, *firstChar;
    int isneg = 0, digcnt = 0;

    if (((*cur < '0') || (*cur > '9')) &&
	(*cur != '-') && (*cur != '+'))
	return -1;

    if (*cur == '-') {
	isneg = 1;
	cur++;
    }

    firstChar = cur;

    while ((*cur >= '0') && (*cur <= '9')) {
	dt->year = dt->year * 10 + (*cur - '0');
	cur++;
	digcnt++;
    }

    if ((digcnt < 4) || ((digcnt > 4) && (*firstChar == '0')))
	return 1;

    if (isneg)
	dt->year = - dt->year;

    if (!VALID_YEAR(dt->year))
	return 2;

    *str = cur;

#ifdef DEBUG_EXSLT_DATE
    xsltGenericDebug(xsltGenericDebugContext,
		     "Parsed year %04i\n", dt->year);
#endif

    return 0;
}

#define FORMAT_GYEAR(yr, cur)					\
	if (yr < 0) {					        \
	    *cur = '-';						\
	    cur++;						\
	}							\
	{							\
	    long year = (yr < 0) ? - yr : yr;                   \
	    xmlChar tmp_buf[100], *tmp = tmp_buf;		\
	    			\
	    while (year > 0) {					\
		*tmp = '0' + (xmlChar)(year % 10);		\
		year /= 10;					\
		tmp++;						\
	    }							\
	    			\
	    while ((tmp - tmp_buf) < 4)				\
		*tmp++ = '0';					\
	    			\
	    while (tmp > tmp_buf) {				\
		tmp--;						\
		*cur = *tmp;					\
		cur++;						\
	    }							\
	}

#define PARSE_2_DIGITS(num, cur, func, invalid)			\
	if ((cur[0] < '0') || (cur[0] > '9') ||			\
	    (cur[1] < '0') || (cur[1] > '9'))			\
	    invalid = 1;					\
	else {							\
	    int val;						\
	    val = (cur[0] - '0') * 10 + (cur[1] - '0');		\
	    if (!func(val))					\
	        invalid = 2;					\
	    else						\
	        num = val;					\
	}							\
	cur += 2;

#define FORMAT_2_DIGITS(num, cur)				\
	*cur = '0' + ((num / 10) % 10);				\
	cur++;							\
	*cur = '0' + (num % 10);				\
	cur++;

#define PARSE_FLOAT(num, cur, invalid)				\
	PARSE_2_DIGITS(num, cur, VALID_ALWAYS, invalid);	\
	if (!invalid && (*cur == '.')) {			\
	    double mult = 1;				        \
	    cur++;						\
	    if ((*cur < '0') || (*cur > '9'))			\
		invalid = 1;					\
	    while ((*cur >= '0') && (*cur <= '9')) {		\
		mult /= 10;					\
		num += (*cur - '0') * mult;			\
		cur++;						\
	    }							\
	}

#define FORMAT_FLOAT(num, cur, pad)				\
	{							\
            xmlChar *sav, *str;                                 \
            if ((pad) && (num < 10.0))                          \
                *cur++ = '0';                                   \
            str = xmlXPathCastNumberToString(num);              \
            sav = str;                                          \
            while (*str != 0)                                   \
                *cur++ = *str++;                                \
            xmlFree(sav);                                       \
	}

static int
_exsltDateParseGMonth (exsltDateValDatePtr dt, const xmlChar **str)
{
    const xmlChar *cur = *str;
    int ret = 0;

    PARSE_2_DIGITS(dt->mon, cur, VALID_MONTH, ret);
    if (ret != 0)
	return ret;

    *str = cur;

#ifdef DEBUG_EXSLT_DATE
    xsltGenericDebug(xsltGenericDebugContext,
		     "Parsed month %02i\n", dt->mon);
#endif

    return 0;
}

#define FORMAT_GMONTH(mon, cur)					\
	FORMAT_2_DIGITS(mon, cur)

static int
_exsltDateParseGDay (exsltDateValDatePtr dt, const xmlChar **str)
{
    const xmlChar *cur = *str;
    int ret = 0;

    PARSE_2_DIGITS(dt->day, cur, VALID_DAY, ret);
    if (ret != 0)
	return ret;

    *str = cur;

#ifdef DEBUG_EXSLT_DATE
    xsltGenericDebug(xsltGenericDebugContext,
		     "Parsed day %02i\n", dt->day);
#endif

    return 0;
}

#define FORMAT_GDAY(dt, cur)					\
	FORMAT_2_DIGITS(dt->day, cur)

#define FORMAT_DATE(dt, cur)					\
	FORMAT_GYEAR(dt->year, cur);				\
	*cur = '-';						\
	cur++;							\
	FORMAT_GMONTH(dt->mon, cur);				\
	*cur = '-';						\
	cur++;							\
	FORMAT_GDAY(dt, cur);

static int
_exsltDateParseTime (exsltDateValDatePtr dt, const xmlChar **str)
{
    const xmlChar *cur = *str;
    unsigned int hour = 0; 
    int ret = 0;

    PARSE_2_DIGITS(hour, cur, VALID_HOUR, ret);
    if (ret != 0)
	return ret;

    if (*cur != ':')
	return 1;
    cur++;

    
    dt->hour = hour;

    PARSE_2_DIGITS(dt->min, cur, VALID_MIN, ret);
    if (ret != 0)
	return ret;

    if (*cur != ':')
	return 1;
    cur++;

    PARSE_FLOAT(dt->sec, cur, ret);
    if (ret != 0)
	return ret;

    if (!VALID_TIME(dt))
	return 2;

    *str = cur;

#ifdef DEBUG_EXSLT_DATE
    xsltGenericDebug(xsltGenericDebugContext,
		     "Parsed time %02i:%02i:%02.f\n",
		     dt->hour, dt->min, dt->sec);
#endif

    return 0;
}

#define FORMAT_TIME(dt, cur)					\
	FORMAT_2_DIGITS(dt->hour, cur);				\
	*cur = ':';						\
	cur++;							\
	FORMAT_2_DIGITS(dt->min, cur);				\
	*cur = ':';						\
	cur++;							\
	FORMAT_FLOAT(dt->sec, cur, 1);

static int
_exsltDateParseTimeZone (exsltDateValDatePtr dt, const xmlChar **str)
{
    const xmlChar *cur;
    int ret = 0;

    if (str == NULL)
	return -1;
    cur = *str;
    switch (*cur) {
    case 0:
	dt->tz_flag = 0;
	dt->tzo = 0;
	break;

    case 'Z':
	dt->tz_flag = 1;
	dt->tzo = 0;
	cur++;
	break;

    case '+':
    case '-': {
	int isneg = 0, tmp = 0;
	isneg = (*cur == '-');

	cur++;

	PARSE_2_DIGITS(tmp, cur, VALID_HOUR, ret);
	if (ret != 0)
	    return ret;

	if (*cur != ':')
	    return 1;
	cur++;

	dt->tzo = tmp * 60;

	PARSE_2_DIGITS(tmp, cur, VALID_MIN, ret);
	if (ret != 0)
	    return ret;

	dt->tzo += tmp;
	if (isneg)
	    dt->tzo = - dt->tzo;

	if (!VALID_TZO(dt->tzo))
	    return 2;

	break;
      }
    default:
	return 1;
    }

    *str = cur;

#ifdef DEBUG_EXSLT_DATE
    xsltGenericDebug(xsltGenericDebugContext,
		     "Parsed time zone offset (%s) %i\n",
		     dt->tz_flag ? "explicit" : "implicit", dt->tzo);
#endif

    return 0;
}

#define FORMAT_TZ(tzo, cur)					\
	if (tzo == 0) {					        \
	    *cur = 'Z';						\
	    cur++;						\
	} else {						\
	    int aTzo = (tzo < 0) ? - tzo : tzo;                 \
	    int tzHh = aTzo / 60, tzMm = aTzo % 60;		\
	    *cur = (tzo < 0) ? '-' : '+' ;			\
	    cur++;						\
	    FORMAT_2_DIGITS(tzHh, cur);				\
	    *cur = ':';						\
	    cur++;						\
	    FORMAT_2_DIGITS(tzMm, cur);				\
	}


static exsltDateValPtr
exsltDateCreateDate (exsltDateType type)
{
    exsltDateValPtr ret;

    ret = (exsltDateValPtr) xmlMalloc(sizeof(exsltDateVal));
    if (ret == NULL) {
	xsltGenericError(xsltGenericErrorContext,
			 "exsltDateCreateDate: out of memory\n");
	return (NULL);
    }
    memset (ret, 0, sizeof(exsltDateVal));

    if (type != EXSLT_UNKNOWN)
        ret->type = type;

    return ret;
}

static void
exsltDateFreeDate (exsltDateValPtr date) {
    if (date == NULL)
	return;

    xmlFree(date);
}

#define PARSE_DIGITS(num, cur, num_type)	                \
	if ((*cur < '0') || (*cur > '9'))			\
	    num_type = -1;					\
        else                                                    \
	    while ((*cur >= '0') && (*cur <= '9')) {		\
	        num = num * 10 + (*cur - '0');		        \
	        cur++;                                          \
            }

#define PARSE_NUM(num, cur, num_type)				\
        num = 0;                                                \
	PARSE_DIGITS(num, cur, num_type);	                \
	if (!num_type && (*cur == '.')) {			\
	    double mult = 1;				        \
	    cur++;						\
	    if ((*cur < '0') || (*cur > '9'))			\
		num_type = -1;					\
            else                                                \
                num_type = 1;                                   \
	    while ((*cur >= '0') && (*cur <= '9')) {		\
		mult /= 10;					\
		num += (*cur - '0') * mult;			\
		cur++;						\
	    }							\
	}

#ifdef WITH_TIME
static exsltDateValPtr
exsltDateCurrent (void)
{
    struct tm localTm, gmTm;
    time_t secs;
    int local_s, gm_s;
    exsltDateValPtr ret;

    ret = exsltDateCreateDate(XS_DATETIME);
    if (ret == NULL)
        return NULL;

    
    secs    = time(NULL);
#if HAVE_LOCALTIME_R
    localtime_r(&secs, &localTm);
#else
    localTm = *localtime(&secs);
#endif

    
    ret->value.date.year = localTm.tm_year + 1900;

    ret->value.date.mon  = localTm.tm_mon + 1;
    ret->value.date.day  = localTm.tm_mday;
    ret->value.date.hour = localTm.tm_hour;
    ret->value.date.min  = localTm.tm_min;

    
    ret->value.date.sec  = (double) localTm.tm_sec;

    
#if HAVE_GMTIME_R
    gmtime_r(&secs, &gmTm);
#else
    gmTm = *gmtime(&secs);
#endif
    ret->value.date.tz_flag = 0;
#if 0
    ret->value.date.tzo = (((ret->value.date.day * 1440) +
                            (ret->value.date.hour * 60) +
                             ret->value.date.min) -
                           ((gmTm.tm_mday * 1440) + (gmTm.tm_hour * 60) +
                             gmTm.tm_min));
#endif
    local_s = localTm.tm_hour * SECS_PER_HOUR +
        localTm.tm_min * SECS_PER_MIN +
        localTm.tm_sec;
    
    gm_s = gmTm.tm_hour * SECS_PER_HOUR +
        gmTm.tm_min * SECS_PER_MIN +
        gmTm.tm_sec;
    
    if (localTm.tm_year < gmTm.tm_year) {
 	ret->value.date.tzo = -((SECS_PER_DAY - local_s) + gm_s)/60;
    } else if (localTm.tm_year > gmTm.tm_year) {
 	ret->value.date.tzo = ((SECS_PER_DAY - gm_s) + local_s)/60;
    } else if (localTm.tm_mon < gmTm.tm_mon) {
 	ret->value.date.tzo = -((SECS_PER_DAY - local_s) + gm_s)/60;
    } else if (localTm.tm_mon > gmTm.tm_mon) {
 	ret->value.date.tzo = ((SECS_PER_DAY - gm_s) + local_s)/60;
    } else if (localTm.tm_mday < gmTm.tm_mday) {
 	ret->value.date.tzo = -((SECS_PER_DAY - local_s) + gm_s)/60;
    } else if (localTm.tm_mday > gmTm.tm_mday) {
 	ret->value.date.tzo = ((SECS_PER_DAY - gm_s) + local_s)/60;
    } else  {
 	ret->value.date.tzo = (local_s - gm_s)/60;
    }
 
    return ret;
}
#endif

static exsltDateValPtr
exsltDateParse (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    int ret;
    const xmlChar *cur = dateTime;

#define RETURN_TYPE_IF_VALID(t)					\
    if (IS_TZO_CHAR(*cur)) {					\
	ret = _exsltDateParseTimeZone(&(dt->value.date), &cur);	\
	if (ret == 0) {						\
	    if (*cur != 0)					\
		goto error;					\
	    dt->type = t;					\
	    return dt;						\
	}							\
    }

    if (dateTime == NULL)
	return NULL;

    if ((*cur != '-') && (*cur < '0') && (*cur > '9'))
	return NULL;

    dt = exsltDateCreateDate(EXSLT_UNKNOWN);
    if (dt == NULL)
	return NULL;

    if ((cur[0] == '-') && (cur[1] == '-')) {
	cur += 2;

	
	if (*cur == '-') {
	  ++cur;
	    ret = _exsltDateParseGDay(&(dt->value.date), &cur);
	    if (ret != 0)
		goto error;

	    RETURN_TYPE_IF_VALID(XS_GDAY);

	    goto error;
	}

	ret = _exsltDateParseGMonth(&(dt->value.date), &cur);
	if (ret != 0)
	    goto error;

	if (*cur != '-')
	    goto error;
	cur++;

	
	if (*cur == '-') {
	    cur++;
	    RETURN_TYPE_IF_VALID(XS_GMONTH);
	    goto error;
	}

	
	ret = _exsltDateParseGDay(&(dt->value.date), &cur);
	if (ret != 0)
	    goto error;

	RETURN_TYPE_IF_VALID(XS_GMONTHDAY);

	goto error;
    }

    if ((*cur >= '0') && (*cur <= '9')) {
	ret = _exsltDateParseTime(&(dt->value.date), &cur);
	if (ret == 0) {
	    
	    RETURN_TYPE_IF_VALID(XS_TIME);
	}
    }

    
    cur = dateTime;

    ret = _exsltDateParseGYear(&(dt->value.date), &cur);
    if (ret != 0)
	goto error;

    
    RETURN_TYPE_IF_VALID(XS_GYEAR);

    if (*cur != '-')
	goto error;
    cur++;

    ret = _exsltDateParseGMonth(&(dt->value.date), &cur);
    if (ret != 0)
	goto error;

    
    RETURN_TYPE_IF_VALID(XS_GYEARMONTH);

    if (*cur != '-')
	goto error;
    cur++;

    ret = _exsltDateParseGDay(&(dt->value.date), &cur);
    if ((ret != 0) || !VALID_DATE((&(dt->value.date))))
	goto error;

    
    RETURN_TYPE_IF_VALID(XS_DATE);

    if (*cur != 'T')
	goto error;
    cur++;

    
    ret = _exsltDateParseTime(&(dt->value.date), &cur);
    if (ret != 0)
	goto error;

    ret = _exsltDateParseTimeZone(&(dt->value.date), &cur);
    if ((ret != 0) || (*cur != 0) || !VALID_DATETIME((&(dt->value.date))))
	goto error;

    dt->type = XS_DATETIME;

    return dt;

error:
    if (dt != NULL)
	exsltDateFreeDate(dt);
    return NULL;
}

static exsltDateValPtr
exsltDateParseDuration (const xmlChar *duration)
{
    const xmlChar  *cur = duration;
    exsltDateValPtr dur;
    int isneg = 0;
    unsigned int seq = 0;

    if (duration == NULL)
	return NULL;

    if (*cur == '-') {
        isneg = 1;
        cur++;
    }

    
    if (*cur++ != 'P')
	return NULL;

    dur = exsltDateCreateDate(XS_DURATION);
    if (dur == NULL)
	return NULL;

    while (*cur != 0) {
        double         num;
        int            num_type = 0;  
        const xmlChar  desig[] = {'Y', 'M', 'D', 'H', 'M', 'S'};
        const double   multi[] = { 0.0, 0.0, 86400.0, 3600.0, 60.0, 1.0, 0.0};

        
        if (seq >= sizeof(desig))
            goto error;

        
        if (*cur == 'T') {
            if (seq <= 3) {
                seq = 3;
                cur++;
            } else
                return NULL;
        } else if (seq == 3)
            goto error;

        
        PARSE_NUM(num, cur, num_type);

        if ((num_type == -1) || (*cur == 0))
            goto error;

        
        while (seq < sizeof(desig)) {
            if (*cur == desig[seq]) {

                
                if ((num_type != 0) && (seq < (sizeof(desig)-1)))
                    goto error;

                switch (seq) {
                    case 0:
                        dur->value.dur.mon = (long)num * 12;
                        break;
                    case 1:
                        dur->value.dur.mon += (long)num;
                        break;
                    default:
                        
                        dur->value.dur.sec += num * multi[seq];
                        seq++;
                        break;
                }

                break;          
            }
            
            if (++seq == 3)
                goto error;
        }
        cur++;
    }

    if (isneg) {
        dur->value.dur.mon = -dur->value.dur.mon;
        dur->value.dur.day = -dur->value.dur.day;
        dur->value.dur.sec = -dur->value.dur.sec;
    }

#ifdef DEBUG_EXSLT_DATE
    xsltGenericDebug(xsltGenericDebugContext,
		     "Parsed duration %f\n", dur->value.dur.sec);
#endif

    return dur;

error:
    if (dur != NULL)
	exsltDateFreeDate(dur);
    return NULL;
}

#define FORMAT_ITEM(num, cur, limit, item)                      \
        if (num != 0) {                                         \
            long comp = (long)num / limit;                      \
            if (comp != 0) {                                    \
                FORMAT_FLOAT((double)comp, cur, 0);             \
                *cur++ = item;                                  \
                num -= (double)(comp * limit);                  \
            }                                                   \
        }

static xmlChar *
exsltDateFormatDuration (const exsltDateValDurationPtr dt)
{
    xmlChar buf[100], *cur = buf;
    double secs, days;
    double years, months;

    if (dt == NULL)
	return NULL;

    
    if ((dt->sec == 0.0) && (dt->day == 0) && (dt->mon == 0)) 
        return xmlStrdup((xmlChar*)"P0D");
        
    secs   = dt->sec;
    days   = (double)dt->day;
    years  = (double)(dt->mon / 12);
    months = (double)(dt->mon % 12);

    *cur = '\0';
    if (secs < 0.0) {
        secs = -secs;
        *cur = '-';
    } 
    if (days < 0) {
        days = -days;
        *cur = '-';
    } 
    if (years < 0) {
        years = -years;
        *cur = '-';
    } 
    if (months < 0) {
        months = -months;
        *cur = '-';
    }
    if (*cur == '-')
	cur++;

    *cur++ = 'P';

    if (years != 0.0) {
        FORMAT_ITEM(years, cur, 1, 'Y');
    }

    if (months != 0.0) {
        FORMAT_ITEM(months, cur, 1, 'M');
    }

    if (secs >= SECS_PER_DAY) {
        double tmp = floor(secs / SECS_PER_DAY);
        days += tmp;
        secs -= (tmp * SECS_PER_DAY);
    }

    FORMAT_ITEM(days, cur, 1, 'D');
    if (secs > 0.0) {
        *cur++ = 'T';
    }
    FORMAT_ITEM(secs, cur, SECS_PER_HOUR, 'H');
    FORMAT_ITEM(secs, cur, SECS_PER_MIN, 'M');
    if (secs > 0.0) {
        FORMAT_FLOAT(secs, cur, 0);
        *cur++ = 'S';
    }

    *cur = 0;

    return xmlStrdup(buf);
}

static xmlChar *
exsltDateFormatDateTime (const exsltDateValDatePtr dt)
{
    xmlChar buf[100], *cur = buf;

    if ((dt == NULL) ||	!VALID_DATETIME(dt))
	return NULL;

    FORMAT_DATE(dt, cur);
    *cur = 'T';
    cur++;
    FORMAT_TIME(dt, cur);
    FORMAT_TZ(dt->tzo, cur);
    *cur = 0;

    return xmlStrdup(buf);
}

static xmlChar *
exsltDateFormatDate (const exsltDateValDatePtr dt)
{
    xmlChar buf[100], *cur = buf;

    if ((dt == NULL) || !VALID_DATETIME(dt))
	return NULL;

    FORMAT_DATE(dt, cur);
    if (dt->tz_flag || (dt->tzo != 0)) {
	FORMAT_TZ(dt->tzo, cur);
    }
    *cur = 0;

    return xmlStrdup(buf);
}

static xmlChar *
exsltDateFormatTime (const exsltDateValDatePtr dt)
{
    xmlChar buf[100], *cur = buf;

    if ((dt == NULL) || !VALID_TIME(dt))
	return NULL;

    FORMAT_TIME(dt, cur);
    if (dt->tz_flag || (dt->tzo != 0)) {
	FORMAT_TZ(dt->tzo, cur);
    }
    *cur = 0;

    return xmlStrdup(buf);
}

static xmlChar *
exsltDateFormat (const exsltDateValPtr dt)
{

    if (dt == NULL)
	return NULL;

    switch (dt->type) {
    case XS_DURATION:
        return exsltDateFormatDuration(&(dt->value.dur));
    case XS_DATETIME:
        return exsltDateFormatDateTime(&(dt->value.date));
    case XS_DATE:
        return exsltDateFormatDate(&(dt->value.date));
    case XS_TIME:
        return exsltDateFormatTime(&(dt->value.date));
    default:
        break;
    }

    if (dt->type & XS_GYEAR) {
        xmlChar buf[20], *cur = buf;

        FORMAT_GYEAR(dt->value.date.year, cur);
        if (dt->type == XS_GYEARMONTH) {
	    *cur = '-';
	    cur++;
	    FORMAT_GMONTH(dt->value.date.mon, cur);
        }

        if (dt->value.date.tz_flag || (dt->value.date.tzo != 0)) {
	    FORMAT_TZ(dt->value.date.tzo, cur);
        }
        *cur = 0;
        return xmlStrdup(buf);
    }

    return NULL;
}

static long
_exsltDateCastYMToDays (const exsltDateValPtr dt)
{
    long ret;

    if (dt->value.date.year < 0)
        ret = (dt->value.date.year * 365) +
              (((dt->value.date.year+1)/4)-((dt->value.date.year+1)/100)+
               ((dt->value.date.year+1)/400)) +
              DAY_IN_YEAR(0, dt->value.date.mon, dt->value.date.year);
    else
        ret = ((dt->value.date.year-1) * 365) +
              (((dt->value.date.year-1)/4)-((dt->value.date.year-1)/100)+
               ((dt->value.date.year-1)/400)) +
              DAY_IN_YEAR(0, dt->value.date.mon, dt->value.date.year);

    return ret;
}

#define TIME_TO_NUMBER(dt)                              \
    ((double)((dt->value.date.hour * SECS_PER_HOUR) +   \
              (dt->value.date.min * SECS_PER_MIN)) + dt->value.date.sec)

static double
exsltDateCastDateToNumber (const exsltDateValPtr dt)
{
    double ret = 0.0;

    if (dt == NULL)
        return 0.0;

    if ((dt->type & XS_GYEAR) == XS_GYEAR) {
        ret = (double)_exsltDateCastYMToDays(dt) * SECS_PER_DAY;
    }

    
    if (dt->type == XS_DURATION) {
        ret += (double)dt->value.dur.day * SECS_PER_DAY;
        ret += dt->value.dur.sec;
    } else {
        ret += (double)dt->value.date.day * SECS_PER_DAY;
        
        ret += TIME_TO_NUMBER(dt);
    }


    return ret;
}

static int
_exsltDateTruncateDate (exsltDateValPtr dt, exsltDateType type)
{
    if (dt == NULL)
        return 1;

    if ((type & XS_TIME) != XS_TIME) {
        dt->value.date.hour = 0;
        dt->value.date.min  = 0;
        dt->value.date.sec  = 0.0;
    }

    if ((type & XS_GDAY) != XS_GDAY)
        dt->value.date.day = 0;

    if ((type & XS_GMONTH) != XS_GMONTH)
        dt->value.date.mon = 0;

    if ((type & XS_GYEAR) != XS_GYEAR)
        dt->value.date.year = 0;

    dt->type = type;

    return 0;
}

static long
_exsltDateDayInWeek(long yday, long yr)
{
    long ret;

    if (yr < 0) {
        ret = ((yr + (((yr+1)/4)-((yr+1)/100)+((yr+1)/400)) + yday) % 7);
        if (ret < 0) 
            ret += 7;
    } else
        ret = (((yr-1) + (((yr-1)/4)-((yr-1)/100)+((yr-1)/400)) + yday) % 7);

    return ret;
}

#define FQUOTIENT(a,b)                  ((floor(((double)a/(double)b))))
#define MODULO(a,b)                     ((a - FQUOTIENT(a,b) * b))
#define FQUOTIENT_RANGE(a,low,high)     (FQUOTIENT((a-low),(high-low)))
#define MODULO_RANGE(a,low,high)        ((MODULO((a-low),(high-low)))+low)

static exsltDateValPtr
_exsltDateAdd (exsltDateValPtr dt, exsltDateValPtr dur)
{
    exsltDateValPtr ret;
    long carry, tempdays, temp;
    exsltDateValDatePtr r, d;
    exsltDateValDurationPtr u;

    if ((dt == NULL) || (dur == NULL))
        return NULL;

    ret = exsltDateCreateDate(dt->type);
    if (ret == NULL)
        return NULL;

    r = &(ret->value.date);
    d = &(dt->value.date);
    u = &(dur->value.dur);

    
    if (d->mon == 0)
        d->mon = 1;

    
    u->sec -= (d->tzo * 60);	
    d->tzo = 0;

    
    if (d->day == 0)
        d->day = 1;

    
    carry  = d->mon + u->mon;
    r->mon = (unsigned int)MODULO_RANGE(carry, 1, 13);
    carry  = (long)FQUOTIENT_RANGE(carry, 1, 13);

    
    r->year = d->year + carry;
    if (r->year == 0) {
        if (d->year > 0)
            r->year--;
        else
            r->year++;
    }

    
    r->tzo     = d->tzo;
    r->tz_flag = d->tz_flag;

    
    r->sec = d->sec + u->sec;
    carry  = (long)FQUOTIENT((long)r->sec, 60);
    if (r->sec != 0.0) {
        r->sec = MODULO(r->sec, 60.0);
    }

    
    carry += d->min;
    r->min = (unsigned int)MODULO(carry, 60);
    carry  = (long)FQUOTIENT(carry, 60);

    
    carry  += d->hour;
    r->hour = (unsigned int)MODULO(carry, 24);
    carry   = (long)FQUOTIENT(carry, 24);

    if ((VALID_YEAR(r->year)) && (VALID_MONTH(r->mon)) &&
                  (d->day > MAX_DAYINMONTH(r->year, r->mon)))
        tempdays = MAX_DAYINMONTH(r->year, r->mon);
    else if (d->day < 1)
        tempdays = 1;
    else
        tempdays = d->day;

    tempdays += u->day + carry;

    while (1) {
        if (tempdays < 1) {
            long tmon = (long)MODULO_RANGE((int)r->mon-1, 1, 13);
            long tyr  = r->year + (long)FQUOTIENT_RANGE((int)r->mon-1, 1, 13);
            if (tyr == 0)
                tyr--;
	    if (tmon < 0)
	        tmon = 0;
	    if (tmon > 12)
	        tmon = 12;
            tempdays += MAX_DAYINMONTH(tyr, tmon);
            carry = -1;
        } else if (tempdays > (long)MAX_DAYINMONTH(r->year, r->mon)) {
            tempdays = tempdays - MAX_DAYINMONTH(r->year, r->mon);
            carry = 1;
        } else
            break;

        temp = r->mon + carry;
        r->mon = (unsigned int)MODULO_RANGE(temp, 1, 13);
        r->year = r->year + (long)FQUOTIENT_RANGE(temp, 1, 13);
        if (r->year == 0) {
            if (temp < 1)
                r->year--;
            else
                r->year++;
	}
    }
    
    r->day = tempdays;

    if (ret->type != XS_DATETIME) {
        if ((r->hour) || (r->min) || (r->sec))
            ret->type = XS_DATETIME;
        else if (ret->type != XS_DATE) {
            if ((r->mon != 1) && (r->day != 1))
                ret->type = XS_DATE;
            else if ((ret->type != XS_GYEARMONTH) && (r->mon != 1))
                ret->type = XS_GYEARMONTH;
        }
    }

    return ret;
}

static void
exsltDateNormalize (exsltDateValPtr dt)
{
    exsltDateValPtr dur, tmp;

    if (dt == NULL)
        return;

    if (((dt->type & XS_TIME) != XS_TIME) || (dt->value.date.tzo == 0))
        return;

    dur = exsltDateCreateDate(XS_DURATION);
    if (dur == NULL)
        return;

    tmp = _exsltDateAdd(dt, dur);
    if (tmp == NULL)
        return;

    memcpy(dt, tmp, sizeof(exsltDateVal));

    exsltDateFreeDate(tmp);
    exsltDateFreeDate(dur);

    dt->value.date.tzo = 0;
}

static exsltDateValPtr
_exsltDateDifference (exsltDateValPtr x, exsltDateValPtr y, int flag)
{
    exsltDateValPtr ret;

    if ((x == NULL) || (y == NULL))
        return NULL;

    if (((x->type < XS_GYEAR) || (x->type > XS_DATETIME)) ||
        ((y->type < XS_GYEAR) || (y->type > XS_DATETIME))) 
        return NULL;

    exsltDateNormalize(x);
    exsltDateNormalize(y);

    if (x->type != y->type) {
        if (x->type < y->type) {
            _exsltDateTruncateDate(y, x->type);
        } else {
            _exsltDateTruncateDate(x, y->type);
        }
    }

    ret = exsltDateCreateDate(XS_DURATION);
    if (ret == NULL)
        return NULL;

    if (((x->type == XS_GYEAR) || (x->type == XS_GYEARMONTH)) && (!flag)) {
        
        ret->value.dur.mon = ((y->value.date.year * 12) + y->value.date.mon) -
                             ((x->value.date.year * 12) + x->value.date.mon);
    } else {
        ret->value.dur.day  = _exsltDateCastYMToDays(y) -
                              _exsltDateCastYMToDays(x);
        ret->value.dur.day += y->value.date.day - x->value.date.day;
        ret->value.dur.sec  = TIME_TO_NUMBER(y) - TIME_TO_NUMBER(x);
	if (ret->value.dur.day > 0.0 && ret->value.dur.sec < 0.0) {
	    ret->value.dur.day -= 1;
	    ret->value.dur.sec = ret->value.dur.sec + SECS_PER_DAY;
	} else if (ret->value.dur.day < 0.0 && ret->value.dur.sec > 0.0) {
	    ret->value.dur.day += 1;
	    ret->value.dur.sec = ret->value.dur.sec - SECS_PER_DAY;
	}
    }

    return ret;
}

static int
_exsltDateAddDurCalc (exsltDateValPtr ret, exsltDateValPtr x,
		      exsltDateValPtr y)
{
    long carry;

    
    ret->value.dur.mon = x->value.dur.mon + y->value.dur.mon;

    
    ret->value.dur.sec = x->value.dur.sec + y->value.dur.sec;
    carry = (long)FQUOTIENT(ret->value.dur.sec, SECS_PER_DAY);
    if (ret->value.dur.sec != 0.0) {
        ret->value.dur.sec = MODULO(ret->value.dur.sec, SECS_PER_DAY);
	if ((carry < 0) && (ret->value.dur.sec != 0)) {
	    
	    ret->value.dur.sec = ret->value.dur.sec - SECS_PER_DAY;
	    carry++;
	}
    }

    
    ret->value.dur.day = x->value.dur.day + y->value.dur.day + carry;

    if ((((ret->value.dur.day > 0) || (ret->value.dur.sec > 0)) &&
         (ret->value.dur.mon < 0)) ||
        (((ret->value.dur.day < 0) || (ret->value.dur.sec < 0)) &&
         (ret->value.dur.mon > 0))) {
        return 0;
    }
    return 1;
}

static exsltDateValPtr
_exsltDateAddDuration (exsltDateValPtr x, exsltDateValPtr y)
{
    exsltDateValPtr ret;

    if ((x == NULL) || (y == NULL))
        return NULL;

    ret = exsltDateCreateDate(XS_DURATION);
    if (ret == NULL)
        return NULL;

    if (_exsltDateAddDurCalc(ret, x, y))
        return ret;

    exsltDateFreeDate(ret);
    return NULL;
}


static xmlChar *
exsltDateDateTime (void)
{
    xmlChar *ret = NULL;
#ifdef WITH_TIME
    exsltDateValPtr cur;

    cur = exsltDateCurrent();
    if (cur != NULL) {
	ret = exsltDateFormatDateTime(&(cur->value.date));
	exsltDateFreeDate(cur);
    }
#endif

    return ret;
}

static xmlChar *
exsltDateDate (const xmlChar *dateTime)
{
    exsltDateValPtr dt = NULL;
    xmlChar *ret = NULL;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return NULL;
    } else {
	dt = exsltDateParse(dateTime);
	if (dt == NULL)
	    return NULL;
	if ((dt->type != XS_DATETIME) && (dt->type != XS_DATE)) {
	    exsltDateFreeDate(dt);
	    return NULL;
	}
    }

    ret = exsltDateFormatDate(&(dt->value.date));
    exsltDateFreeDate(dt);

    return ret;
}

static xmlChar *
exsltDateTime (const xmlChar *dateTime)
{
    exsltDateValPtr dt = NULL;
    xmlChar *ret = NULL;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return NULL;
    } else {
	dt = exsltDateParse(dateTime);
	if (dt == NULL)
	    return NULL;
	if ((dt->type != XS_DATETIME) && (dt->type != XS_TIME)) {
	    exsltDateFreeDate(dt);
	    return NULL;
	}
    }

    ret = exsltDateFormatTime(&(dt->value.date));
    exsltDateFreeDate(dt);

    return ret;
}

static double
exsltDateYear (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    double ret;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return xmlXPathNAN;
    } else {
	dt = exsltDateParse(dateTime);
	if (dt == NULL)
	    return xmlXPathNAN;
	if ((dt->type != XS_DATETIME) && (dt->type != XS_DATE) &&
	    (dt->type != XS_GYEARMONTH) && (dt->type != XS_GYEAR)) {
	    exsltDateFreeDate(dt);
	    return xmlXPathNAN;
	}
    }

    ret = (double) dt->value.date.year;
    exsltDateFreeDate(dt);

    return ret;
}

static xmlXPathObjectPtr
exsltDateLeapYear (const xmlChar *dateTime)
{
    double year;

    year = exsltDateYear(dateTime);
    if (xmlXPathIsNaN(year))
	return xmlXPathNewFloat(xmlXPathNAN);

    if (IS_LEAP((long)year))
	return xmlXPathNewBoolean(1);

    return xmlXPathNewBoolean(0);
}

static double
exsltDateMonthInYear (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    double ret;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return xmlXPathNAN;
    } else {
	dt = exsltDateParse(dateTime);
	if (dt == NULL)
	    return xmlXPathNAN;
	if ((dt->type != XS_DATETIME) && (dt->type != XS_DATE) &&
	    (dt->type != XS_GYEARMONTH) && (dt->type != XS_GMONTH) &&
	    (dt->type != XS_GMONTHDAY)) {
	    exsltDateFreeDate(dt);
	    return xmlXPathNAN;
	}
    }

    ret = (double) dt->value.date.mon;
    exsltDateFreeDate(dt);

    return ret;
}

static const xmlChar *
exsltDateMonthName (const xmlChar *dateTime)
{
    static const xmlChar monthNames[13][10] = {
        { 0 },
	{ 'J', 'a', 'n', 'u', 'a', 'r', 'y', 0 },
	{ 'F', 'e', 'b', 'r', 'u', 'a', 'r', 'y', 0 },
	{ 'M', 'a', 'r', 'c', 'h', 0 },
	{ 'A', 'p', 'r', 'i', 'l', 0 },
	{ 'M', 'a', 'y', 0 },
	{ 'J', 'u', 'n', 'e', 0 },
	{ 'J', 'u', 'l', 'y', 0 },
	{ 'A', 'u', 'g', 'u', 's', 't', 0 },
	{ 'S', 'e', 'p', 't', 'e', 'm', 'b', 'e', 'r', 0 },
	{ 'O', 'c', 't', 'o', 'b', 'e', 'r', 0 },
	{ 'N', 'o', 'v', 'e', 'm', 'b', 'e', 'r', 0 },
	{ 'D', 'e', 'c', 'e', 'm', 'b', 'e', 'r', 0 }
    };
    int month;
    month = (int) exsltDateMonthInYear(dateTime);
    if (!VALID_MONTH(month))
      month = 0;
    return monthNames[month];
}

static const xmlChar *
exsltDateMonthAbbreviation (const xmlChar *dateTime)
{
    static const xmlChar monthAbbreviations[13][4] = {
        { 0 },
	{ 'J', 'a', 'n', 0 },
	{ 'F', 'e', 'b', 0 },
	{ 'M', 'a', 'r', 0 },
	{ 'A', 'p', 'r', 0 },
	{ 'M', 'a', 'y', 0 },
	{ 'J', 'u', 'n', 0 },
	{ 'J', 'u', 'l', 0 },
	{ 'A', 'u', 'g', 0 },
	{ 'S', 'e', 'p', 0 },
	{ 'O', 'c', 't', 0 },
	{ 'N', 'o', 'v', 0 },
	{ 'D', 'e', 'c', 0 }
    };
    int month;
    month = (int) exsltDateMonthInYear(dateTime);
    if(!VALID_MONTH(month))
      month = 0;
    return monthAbbreviations[month];
}

static double
exsltDateWeekInYear (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    long diy, diw, year, ret;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return xmlXPathNAN;
    } else {
	dt = exsltDateParse(dateTime);
	if (dt == NULL)
	    return xmlXPathNAN;
	if ((dt->type != XS_DATETIME) && (dt->type != XS_DATE)) {
	    exsltDateFreeDate(dt);
	    return xmlXPathNAN;
	}
    }

    diy = DAY_IN_YEAR(dt->value.date.day, dt->value.date.mon,
                      dt->value.date.year);

    diw = (_exsltDateDayInWeek(diy, dt->value.date.year) + 6) % 7;

    
    diy += (3 - diw);
    if(diy < 1) {
	year = dt->value.date.year - 1;
	if(year == 0) year--;
	diy = DAY_IN_YEAR(31, 12, year) + diy;
    } else if (diy > (long)DAY_IN_YEAR(31, 12, dt->value.date.year)) {
	diy -= DAY_IN_YEAR(31, 12, dt->value.date.year);
    }

    ret = ((diy - 1) / 7) + 1;

    exsltDateFreeDate(dt);

    return (double) ret;
}

static double
exsltDateWeekInMonth (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    long fdiy, fdiw, ret;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return xmlXPathNAN;
    } else {
	dt = exsltDateParse(dateTime);
	if (dt == NULL)
	    return xmlXPathNAN;
	if ((dt->type != XS_DATETIME) && (dt->type != XS_DATE)) {
	    exsltDateFreeDate(dt);
	    return xmlXPathNAN;
	}
    }

    fdiy = DAY_IN_YEAR(1, dt->value.date.mon, dt->value.date.year);
    fdiw = (_exsltDateDayInWeek(fdiy, dt->value.date.year) + 6) % 7;

    ret = ((dt->value.date.day + fdiw - 1) / 7) + 1;

    exsltDateFreeDate(dt);

    return (double) ret;
}

static double
exsltDateDayInYear (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    long ret;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return xmlXPathNAN;
    } else {
	dt = exsltDateParse(dateTime);
	if (dt == NULL)
	    return xmlXPathNAN;
	if ((dt->type != XS_DATETIME) && (dt->type != XS_DATE)) {
	    exsltDateFreeDate(dt);
	    return xmlXPathNAN;
	}
    }

    ret = DAY_IN_YEAR(dt->value.date.day, dt->value.date.mon,
                      dt->value.date.year);

    exsltDateFreeDate(dt);

    return (double) ret;
}

static double
exsltDateDayInMonth (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    double ret;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return xmlXPathNAN;
    } else {
	dt = exsltDateParse(dateTime);
	if (dt == NULL)
	    return xmlXPathNAN;
	if ((dt->type != XS_DATETIME) && (dt->type != XS_DATE) &&
	    (dt->type != XS_GMONTHDAY) && (dt->type != XS_GDAY)) {
	    exsltDateFreeDate(dt);
	    return xmlXPathNAN;
	}
    }

    ret = (double) dt->value.date.day;
    exsltDateFreeDate(dt);

    return ret;
}

static double
exsltDateDayOfWeekInMonth (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    long ret;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return xmlXPathNAN;
    } else {
	dt = exsltDateParse(dateTime);
	if (dt == NULL)
	    return xmlXPathNAN;
	if ((dt->type != XS_DATETIME) && (dt->type != XS_DATE)) {
	    exsltDateFreeDate(dt);
	    return xmlXPathNAN;
	}
    }

    ret = ((dt->value.date.day -1) / 7) + 1;

    exsltDateFreeDate(dt);

    return (double) ret;
}

static double
exsltDateDayInWeek (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    long diy, ret;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return xmlXPathNAN;
    } else {
	dt = exsltDateParse(dateTime);
	if (dt == NULL)
	    return xmlXPathNAN;
	if ((dt->type != XS_DATETIME) && (dt->type != XS_DATE)) {
	    exsltDateFreeDate(dt);
	    return xmlXPathNAN;
	}
    }

    diy = DAY_IN_YEAR(dt->value.date.day, dt->value.date.mon,
                      dt->value.date.year);

    ret = _exsltDateDayInWeek(diy, dt->value.date.year) + 1;

    exsltDateFreeDate(dt);

    return (double) ret;
}

static const xmlChar *
exsltDateDayName (const xmlChar *dateTime)
{
    static const xmlChar dayNames[8][10] = {
        { 0 },
	{ 'S', 'u', 'n', 'd', 'a', 'y', 0 },
	{ 'M', 'o', 'n', 'd', 'a', 'y', 0 },
	{ 'T', 'u', 'e', 's', 'd', 'a', 'y', 0 },
	{ 'W', 'e', 'd', 'n', 'e', 's', 'd', 'a', 'y', 0 },
	{ 'T', 'h', 'u', 'r', 's', 'd', 'a', 'y', 0 },
	{ 'F', 'r', 'i', 'd', 'a', 'y', 0 },
	{ 'S', 'a', 't', 'u', 'r', 'd', 'a', 'y', 0 }
    };
    int day;
    day = (int) exsltDateDayInWeek(dateTime);
    if((day < 1) || (day > 7))
      day = 0;
    return dayNames[day];
}

static const xmlChar *
exsltDateDayAbbreviation (const xmlChar *dateTime)
{
    static const xmlChar dayAbbreviations[8][4] = {
        { 0 },
	{ 'S', 'u', 'n', 0 },
	{ 'M', 'o', 'n', 0 },
	{ 'T', 'u', 'e', 0 },
	{ 'W', 'e', 'd', 0 },
	{ 'T', 'h', 'u', 0 },
	{ 'F', 'r', 'i', 0 },
	{ 'S', 'a', 't', 0 }
    };
    int day;
    day = (int) exsltDateDayInWeek(dateTime);
    if((day < 1) || (day > 7))
      day = 0;
    return dayAbbreviations[day];
}

static double
exsltDateHourInDay (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    double ret;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return xmlXPathNAN;
    } else {
	dt = exsltDateParse(dateTime);
	if (dt == NULL)
	    return xmlXPathNAN;
	if ((dt->type != XS_DATETIME) && (dt->type != XS_TIME)) {
	    exsltDateFreeDate(dt);
	    return xmlXPathNAN;
	}
    }

    ret = (double) dt->value.date.hour;
    exsltDateFreeDate(dt);

    return ret;
}

static double
exsltDateMinuteInHour (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    double ret;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return xmlXPathNAN;
    } else {
	dt = exsltDateParse(dateTime);
	if (dt == NULL)
	    return xmlXPathNAN;
	if ((dt->type != XS_DATETIME) && (dt->type != XS_TIME)) {
	    exsltDateFreeDate(dt);
	    return xmlXPathNAN;
	}
    }

    ret = (double) dt->value.date.min;
    exsltDateFreeDate(dt);

    return ret;
}

static double
exsltDateSecondInMinute (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    double ret;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return xmlXPathNAN;
    } else {
	dt = exsltDateParse(dateTime);
	if (dt == NULL)
	    return xmlXPathNAN;
	if ((dt->type != XS_DATETIME) && (dt->type != XS_TIME)) {
	    exsltDateFreeDate(dt);
	    return xmlXPathNAN;
	}
    }

    ret = dt->value.date.sec;
    exsltDateFreeDate(dt);

    return ret;
}

static xmlChar *
exsltDateAdd (const xmlChar *xstr, const xmlChar *ystr)
{
    exsltDateValPtr dt, dur, res;
    xmlChar     *ret;   

    if ((xstr == NULL) || (ystr == NULL))
        return NULL;

    dt = exsltDateParse(xstr);
    if (dt == NULL)
        return NULL;
    else if ((dt->type < XS_GYEAR) || (dt->type > XS_DATETIME)) {
        exsltDateFreeDate(dt);
        return NULL;
    }

    dur = exsltDateParseDuration(ystr);
    if (dur == NULL) {
        exsltDateFreeDate(dt);
        return NULL;
    }

    res = _exsltDateAdd(dt, dur);

    exsltDateFreeDate(dt);
    exsltDateFreeDate(dur);

    if (res == NULL)
        return NULL;

    ret = exsltDateFormat(res);
    exsltDateFreeDate(res);

    return ret;
}

static xmlChar *
exsltDateAddDuration (const xmlChar *xstr, const xmlChar *ystr)
{
    exsltDateValPtr x, y, res;
    xmlChar     *ret;   

    if ((xstr == NULL) || (ystr == NULL))
        return NULL;

    x = exsltDateParseDuration(xstr);
    if (x == NULL)
        return NULL;

    y = exsltDateParseDuration(ystr);
    if (y == NULL) {
        exsltDateFreeDate(x);
        return NULL;
    }

    res = _exsltDateAddDuration(x, y);

    exsltDateFreeDate(x);
    exsltDateFreeDate(y);

    if (res == NULL)
        return NULL;

    ret = exsltDateFormatDuration(&(res->value.dur));
    exsltDateFreeDate(res);

    return ret;
}

static void
exsltDateSumFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlNodeSetPtr ns;
    void *user = NULL;
    xmlChar *tmp;
    exsltDateValPtr x, total;
    xmlChar *ret;
    int i;

    if (nargs != 1) {
	xmlXPathSetArityError (ctxt);
	return;
    }

    
    if ((ctxt->value != NULL) && ctxt->value->boolval != 0) {
	user = ctxt->value->user;
	ctxt->value->boolval = 0;
	ctxt->value->user = NULL;
    }

    ns = xmlXPathPopNodeSet (ctxt);
    if (xmlXPathCheckError (ctxt))
	return;

    if ((ns == NULL) || (ns->nodeNr == 0)) {
	xmlXPathReturnEmptyString (ctxt);
	if (ns != NULL)
	    xmlXPathFreeNodeSet (ns);
	return;
    }

    total = exsltDateCreateDate (XS_DURATION);
    if (total == NULL) {
        xmlXPathFreeNodeSet (ns);
        return;
    }

    for (i = 0; i < ns->nodeNr; i++) {
    	int result;
	tmp = xmlXPathCastNodeToString (ns->nodeTab[i]);
	if (tmp == NULL) {
	    xmlXPathFreeNodeSet (ns);
	    exsltDateFreeDate (total);
	    return;
	}

	x = exsltDateParseDuration (tmp);
	if (x == NULL) {
	    xmlFree (tmp);
	    exsltDateFreeDate (total);
	    xmlXPathFreeNodeSet (ns);
	    xmlXPathReturnEmptyString (ctxt);
	    return;
	}

	result = _exsltDateAddDurCalc(total, total, x);

	exsltDateFreeDate (x);
	xmlFree (tmp);
	if (!result) {
	    exsltDateFreeDate (total);
	    xmlXPathFreeNodeSet (ns);
	    xmlXPathReturnEmptyString (ctxt);
	    return;
	}
    }

    ret = exsltDateFormatDuration (&(total->value.dur));
    exsltDateFreeDate (total);

    xmlXPathFreeNodeSet (ns);
    if (user != NULL)
	xmlFreeNodeList ((xmlNodePtr) user);

    if (ret == NULL)
	xmlXPathReturnEmptyString (ctxt);
    else
	xmlXPathReturnString (ctxt, ret);
}

static double
exsltDateSeconds (const xmlChar *dateTime)
{
    exsltDateValPtr dt;
    double ret = xmlXPathNAN;

    if (dateTime == NULL) {
#ifdef WITH_TIME
	dt = exsltDateCurrent();
	if (dt == NULL)
#endif
	    return xmlXPathNAN;
    } else {
        dt = exsltDateParseDuration(dateTime);
        if (dt == NULL)
            dt = exsltDateParse(dateTime);
    }

    if (dt == NULL)
        return xmlXPathNAN;

    if ((dt->type <= XS_DATETIME) && (dt->type >= XS_GYEAR)) {
        exsltDateValPtr y, dur;

        y = exsltDateCreateDate(XS_DATETIME);
        if (y != NULL) {
            y->value.date.year = 1970;
            y->value.date.mon  = 1;
            y->value.date.day  = 1;
            y->value.date.tz_flag = 1;

            dur = _exsltDateDifference(y, dt, 1);
            if (dur != NULL) {
                ret = exsltDateCastDateToNumber(dur); 
                exsltDateFreeDate(dur);
            }
            exsltDateFreeDate(y);
        }

    } else if ((dt->type == XS_DURATION) && (dt->value.dur.mon == 0))
        ret = exsltDateCastDateToNumber(dt);

    exsltDateFreeDate(dt);

    return ret;
}

static xmlChar *
exsltDateDifference (const xmlChar *xstr, const xmlChar *ystr)
{
    exsltDateValPtr x, y, dur;
    xmlChar        *ret = NULL;   

    if ((xstr == NULL) || (ystr == NULL))
        return NULL;

    x = exsltDateParse(xstr);
    if (x == NULL)
        return NULL;

    y = exsltDateParse(ystr);
    if (y == NULL) {
        exsltDateFreeDate(x);
        return NULL;
    }

    if (((x->type < XS_GYEAR) || (x->type > XS_DATETIME)) ||
        ((y->type < XS_GYEAR) || (y->type > XS_DATETIME)))  {
	exsltDateFreeDate(x);
	exsltDateFreeDate(y);
        return NULL;
    }

    dur = _exsltDateDifference(x, y, 0);

    exsltDateFreeDate(x);
    exsltDateFreeDate(y);

    if (dur == NULL)
        return NULL;

    ret = exsltDateFormatDuration(&(dur->value.dur));
    exsltDateFreeDate(dur);

    return ret;
}

static xmlChar *
exsltDateDuration (const xmlChar *number)
{
    exsltDateValPtr dur;
    double       secs;
    xmlChar     *ret;

    if (number == NULL)
        secs = exsltDateSeconds(number);
    else
        secs = xmlXPathCastStringToNumber(number);

    if ((xmlXPathIsNaN(secs)) || (xmlXPathIsInf(secs)))
        return NULL;

    dur = exsltDateCreateDate(XS_DURATION);
    if (dur == NULL)
        return NULL;

    dur->value.dur.sec = secs;

    ret = exsltDateFormatDuration(&(dur->value.dur));
    exsltDateFreeDate(dur);

    return ret;
}


#ifdef WITH_TIME
static void
exsltDateDateTimeFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *ret;

    if (nargs != 0) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    ret = exsltDateDateTime();
    if (ret == NULL)
        xmlXPathReturnEmptyString(ctxt);
    else
        xmlXPathReturnString(ctxt, ret);
}
#endif

static void
exsltDateDateFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *ret, *dt = NULL;

    if ((nargs < 0) || (nargs > 1)) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    if (nargs == 1) {
	dt = xmlXPathPopString(ctxt);
	if (xmlXPathCheckError(ctxt)) {
	    xmlXPathSetTypeError(ctxt);
	    return;
	}
    }

    ret = exsltDateDate(dt);

    if (ret == NULL) {
	xsltGenericDebug(xsltGenericDebugContext,
			 "{http://exslt.org/dates-and-times}date: "
			 "invalid date or format %s\n", dt);
	xmlXPathReturnEmptyString(ctxt);
    } else {
	xmlXPathReturnString(ctxt, ret);
    }

    if (dt != NULL)
	xmlFree(dt);
}

static void
exsltDateTimeFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *ret, *dt = NULL;

    if ((nargs < 0) || (nargs > 1)) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    if (nargs == 1) {
	dt = xmlXPathPopString(ctxt);
	if (xmlXPathCheckError(ctxt)) {
	    xmlXPathSetTypeError(ctxt);
	    return;
	}
    }

    ret = exsltDateTime(dt);

    if (ret == NULL) {
	xsltGenericDebug(xsltGenericDebugContext,
			 "{http://exslt.org/dates-and-times}time: "
			 "invalid date or format %s\n", dt);
	xmlXPathReturnEmptyString(ctxt);
    } else {
	xmlXPathReturnString(ctxt, ret);
    }

    if (dt != NULL)
	xmlFree(dt);
}

static void
exsltDateYearFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *dt = NULL;
    double ret;

    if ((nargs < 0) || (nargs > 1)) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs == 1) {
	dt = xmlXPathPopString(ctxt);
	if (xmlXPathCheckError(ctxt)) {
	    xmlXPathSetTypeError(ctxt);
	    return;
	}
    }

    ret = exsltDateYear(dt);

    if (dt != NULL)
	xmlFree(dt);

    xmlXPathReturnNumber(ctxt, ret);
}

static void
exsltDateLeapYearFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *dt = NULL;
    xmlXPathObjectPtr ret;

    if ((nargs < 0) || (nargs > 1)) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs == 1) {
	dt = xmlXPathPopString(ctxt);
	if (xmlXPathCheckError(ctxt)) {
	    xmlXPathSetTypeError(ctxt);
	    return;
	}
    }

    ret = exsltDateLeapYear(dt);

    if (dt != NULL)
	xmlFree(dt);

    valuePush(ctxt, ret);
}

#define X_IN_Y(x, y)						\
static void							\
exsltDate##x##In##y##Function (xmlXPathParserContextPtr ctxt,	\
			      int nargs) {			\
    xmlChar *dt = NULL;						\
    double ret;							\
								\
    if ((nargs < 0) || (nargs > 1)) {				\
	xmlXPathSetArityError(ctxt);				\
	return;							\
    }								\
								\
    if (nargs == 1) {						\
	dt = xmlXPathPopString(ctxt);				\
	if (xmlXPathCheckError(ctxt)) {				\
	    xmlXPathSetTypeError(ctxt);				\
	    return;						\
	}							\
    }								\
								\
    ret = exsltDate##x##In##y(dt);				\
								\
    if (dt != NULL)						\
	xmlFree(dt);						\
								\
    xmlXPathReturnNumber(ctxt, ret);				\
}

X_IN_Y(Month,Year)

static void
exsltDateMonthNameFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *dt = NULL;
    const xmlChar *ret;

    if ((nargs < 0) || (nargs > 1)) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs == 1) {
	dt = xmlXPathPopString(ctxt);
	if (xmlXPathCheckError(ctxt)) {
	    xmlXPathSetTypeError(ctxt);
	    return;
	}
    }

    ret = exsltDateMonthName(dt);

    if (dt != NULL)
	xmlFree(dt);

    if (ret == NULL)
	xmlXPathReturnEmptyString(ctxt);
    else
	xmlXPathReturnString(ctxt, xmlStrdup(ret));
}

static void
exsltDateMonthAbbreviationFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *dt = NULL;
    const xmlChar *ret;

    if ((nargs < 0) || (nargs > 1)) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs == 1) {
	dt = xmlXPathPopString(ctxt);
	if (xmlXPathCheckError(ctxt)) {
	    xmlXPathSetTypeError(ctxt);
	    return;
	}
    }

    ret = exsltDateMonthAbbreviation(dt);

    if (dt != NULL)
	xmlFree(dt);

    if (ret == NULL)
	xmlXPathReturnEmptyString(ctxt);
    else
	xmlXPathReturnString(ctxt, xmlStrdup(ret));
}

X_IN_Y(Week,Year)

X_IN_Y(Week,Month)

X_IN_Y(Day,Year)

X_IN_Y(Day,Month)

X_IN_Y(DayOfWeek,Month)

X_IN_Y(Day,Week)

static void
exsltDateDayNameFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *dt = NULL;
    const xmlChar *ret;

    if ((nargs < 0) || (nargs > 1)) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs == 1) {
	dt = xmlXPathPopString(ctxt);
	if (xmlXPathCheckError(ctxt)) {
	    xmlXPathSetTypeError(ctxt);
	    return;
	}
    }

    ret = exsltDateDayName(dt);

    if (dt != NULL)
	xmlFree(dt);

    if (ret == NULL)
	xmlXPathReturnEmptyString(ctxt);
    else
	xmlXPathReturnString(ctxt, xmlStrdup(ret));
}

static void
exsltDateDayAbbreviationFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *dt = NULL;
    const xmlChar *ret;

    if ((nargs < 0) || (nargs > 1)) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs == 1) {
	dt = xmlXPathPopString(ctxt);
	if (xmlXPathCheckError(ctxt)) {
	    xmlXPathSetTypeError(ctxt);
	    return;
	}
    }

    ret = exsltDateDayAbbreviation(dt);

    if (dt != NULL)
	xmlFree(dt);

    if (ret == NULL)
	xmlXPathReturnEmptyString(ctxt);
    else
	xmlXPathReturnString(ctxt, xmlStrdup(ret));
}


X_IN_Y(Hour,Day)

X_IN_Y(Minute,Hour)

X_IN_Y(Second,Minute)

static void
exsltDateSecondsFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *str = NULL;
    double   ret;

    if (nargs > 1) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs == 1) {
	str = xmlXPathPopString(ctxt);
	if (xmlXPathCheckError(ctxt)) {
	    xmlXPathSetTypeError(ctxt);
	    return;
	}
    }

    ret = exsltDateSeconds(str);
    if (str != NULL)
	xmlFree(str);

    xmlXPathReturnNumber(ctxt, ret);
}

static void
exsltDateAddFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *ret, *xstr, *ystr;

    if (nargs != 2) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    ystr = xmlXPathPopString(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    xstr = xmlXPathPopString(ctxt);
    if (xmlXPathCheckError(ctxt)) {
        xmlFree(ystr);
	return;
    }

    ret = exsltDateAdd(xstr, ystr);

    xmlFree(ystr);
    xmlFree(xstr);

    if (ret == NULL)
        xmlXPathReturnEmptyString(ctxt);
    else
	xmlXPathReturnString(ctxt, ret);
}

static void
exsltDateAddDurationFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *ret, *xstr, *ystr;

    if (nargs != 2) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    ystr = xmlXPathPopString(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    xstr = xmlXPathPopString(ctxt);
    if (xmlXPathCheckError(ctxt)) {
        xmlFree(ystr);
	return;
    }

    ret = exsltDateAddDuration(xstr, ystr);

    xmlFree(ystr);
    xmlFree(xstr);

    if (ret == NULL)
        xmlXPathReturnEmptyString(ctxt);
    else
	xmlXPathReturnString(ctxt, ret);
}

static void
exsltDateDifferenceFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *ret, *xstr, *ystr;

    if (nargs != 2) {
	xmlXPathSetArityError(ctxt);
	return;
    }
    ystr = xmlXPathPopString(ctxt);
    if (xmlXPathCheckError(ctxt))
	return;

    xstr = xmlXPathPopString(ctxt);
    if (xmlXPathCheckError(ctxt)) {
        xmlFree(ystr);
	return;
    }

    ret = exsltDateDifference(xstr, ystr);

    xmlFree(ystr);
    xmlFree(xstr);

    if (ret == NULL)
        xmlXPathReturnEmptyString(ctxt);
    else
	xmlXPathReturnString(ctxt, ret);
}

static void
exsltDateDurationFunction (xmlXPathParserContextPtr ctxt, int nargs)
{
    xmlChar *ret;
    xmlChar *number = NULL;

    if ((nargs < 0) || (nargs > 1)) {
	xmlXPathSetArityError(ctxt);
	return;
    }

    if (nargs == 1) {
	number = xmlXPathPopString(ctxt);
	if (xmlXPathCheckError(ctxt)) {
	    xmlXPathSetTypeError(ctxt);
	    return;
	}
    }

    ret = exsltDateDuration(number);

    if (number != NULL)
	xmlFree(number);

    if (ret == NULL)
	xmlXPathReturnEmptyString(ctxt);
    else
	xmlXPathReturnString(ctxt, ret);
}

void
exsltDateRegister (void)
{
    xsltRegisterExtModuleFunction ((const xmlChar *) "add",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateAddFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "add-duration",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateAddDurationFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "date",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDateFunction);
#ifdef WITH_TIME
    xsltRegisterExtModuleFunction ((const xmlChar *) "date-time",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDateTimeFunction);
#endif
    xsltRegisterExtModuleFunction ((const xmlChar *) "day-abbreviation",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDayAbbreviationFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "day-in-month",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDayInMonthFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "day-in-week",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDayInWeekFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "day-in-year",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDayInYearFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "day-name",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDayNameFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "day-of-week-in-month",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDayOfWeekInMonthFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "difference",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDifferenceFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "duration",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateDurationFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "hour-in-day",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateHourInDayFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "leap-year",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateLeapYearFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "minute-in-hour",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateMinuteInHourFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "month-abbreviation",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateMonthAbbreviationFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "month-in-year",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateMonthInYearFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "month-name",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateMonthNameFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "second-in-minute",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateSecondInMinuteFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "seconds",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateSecondsFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "sum",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateSumFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "time",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateTimeFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "week-in-month",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateWeekInMonthFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "week-in-year",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateWeekInYearFunction);
    xsltRegisterExtModuleFunction ((const xmlChar *) "year",
				   (const xmlChar *) EXSLT_DATE_NAMESPACE,
				   exsltDateYearFunction);
}

int
exsltDateXpathCtxtRegister (xmlXPathContextPtr ctxt, const xmlChar *prefix)
{
    if (ctxt
        && prefix
        && !xmlXPathRegisterNs(ctxt,
                               prefix,
                               (const xmlChar *) EXSLT_DATE_NAMESPACE)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "add",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateAddFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "add-duration",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateAddDurationFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "date",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDateFunction)
#ifdef WITH_TIME
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "date-time",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDateTimeFunction)
#endif
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "day-abbreviation",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDayAbbreviationFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "day-in-month",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDayInMonthFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "day-in-week",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDayInWeekFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "day-in-year",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDayInYearFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "day-name",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDayNameFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "day-of-week-in-month",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDayOfWeekInMonthFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "difference",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDifferenceFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "duration",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateDurationFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "hour-in-day",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateHourInDayFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "leap-year",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateLeapYearFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "minute-in-hour",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateMinuteInHourFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "month-abbreviation",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateMonthAbbreviationFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "month-in-year",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateMonthInYearFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "month-name",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateMonthNameFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "second-in-minute",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateSecondInMinuteFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "seconds",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateSecondsFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "sum",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateSumFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "time",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateTimeFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "week-in-month",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateWeekInMonthFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "week-in-year",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateWeekInYearFunction)
        && !xmlXPathRegisterFuncNS(ctxt,
                                   (const xmlChar *) "year",
                                   (const xmlChar *) EXSLT_DATE_NAMESPACE,
                                   exsltDateYearFunction)) {
        return 0;
    }
    return -1;
}
