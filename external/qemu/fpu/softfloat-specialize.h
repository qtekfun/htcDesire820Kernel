
/*============================================================================

This C source fragment is part of the SoftFloat IEC/IEEE Floating-point
Arithmetic Package, Release 2b.

Written by John R. Hauser.  This work was made possible in part by the
International Computer Science Institute, located at Suite 600, 1947 Center
Street, Berkeley, California 94704.  Funding was partially provided by the
National Science Foundation under grant MIP-9311980.  The original version
of this code was written as part of a project to build a fixed-point vector
processor in collaboration with the University of California at Berkeley,
overseen by Profs. Nelson Morgan and John Wawrzynek.  More information
is available through the Web page `http://www.cs.berkeley.edu/~jhauser/
arithmetic/SoftFloat.html'.

THIS SOFTWARE IS DISTRIBUTED AS IS, FOR FREE.  Although reasonable effort has
been made to avoid it, THIS SOFTWARE MAY CONTAIN FAULTS THAT WILL AT TIMES
RESULT IN INCORRECT BEHAVIOR.  USE OF THIS SOFTWARE IS RESTRICTED TO PERSONS
AND ORGANIZATIONS WHO CAN AND WILL TAKE FULL RESPONSIBILITY FOR ALL LOSSES,
COSTS, OR OTHER PROBLEMS THEY INCUR DUE TO THE SOFTWARE, AND WHO FURTHERMORE
EFFECTIVELY INDEMNIFY JOHN HAUSER AND THE INTERNATIONAL COMPUTER SCIENCE
INSTITUTE (possibly via similar legal warning) AGAINST ALL LOSSES, COSTS, OR
OTHER PROBLEMS INCURRED BY THEIR CUSTOMERS AND CLIENTS DUE TO THE SOFTWARE.

Derivative works are acceptable, even for commercial purposes, so long as
(1) the source code for the derivative work includes prominent notice that
the work is derivative, and (2) the source code includes prominent notice with
these four paragraphs for those parts of this code that are retained.

=============================================================================*/


void float_raise( int8 flags STATUS_PARAM )
{
    STATUS(float_exception_flags) |= flags;
}

typedef struct {
    flag sign;
    uint64_t high, low;
} commonNaNT;


int float16_is_quiet_nan(float16 a_)
{
    uint16_t a = float16_val(a_);
#if SNAN_BIT_IS_ONE
    return (((a >> 9) & 0x3F) == 0x3E) && (a & 0x1FF);
#else
    return ((a & ~0x8000) >= 0x7c80);
#endif
}


int float16_is_signaling_nan(float16 a_)
{
    uint16_t a = float16_val(a_);
#if SNAN_BIT_IS_ONE
    return ((a & ~0x8000) >= 0x7c80);
#else
    return (((a >> 9) & 0x3F) == 0x3E) && (a & 0x1FF);
#endif
}

float16 float16_maybe_silence_nan(float16 a_)
{
    if (float16_is_signaling_nan(a_)) {
#if SNAN_BIT_IS_ONE
#  if defined(TARGET_MIPS) || defined(TARGET_SH4) || defined(TARGET_UNICORE32)
        return float16_default_nan;
#  else
#    error Rules for silencing a signaling NaN are target-specific
#  endif
#else
        uint16_t a = float16_val(a_);
        a |= (1 << 9);
        return make_float16(a);
#endif
    }
    return a_;
}


static commonNaNT float16ToCommonNaN( float16 a STATUS_PARAM )
{
    commonNaNT z;

    if ( float16_is_signaling_nan( a ) ) float_raise( float_flag_invalid STATUS_VAR );
    z.sign = float16_val(a) >> 15;
    z.low = 0;
    z.high = ((uint64_t) float16_val(a))<<54;
    return z;
}


static float16 commonNaNToFloat16(commonNaNT a STATUS_PARAM)
{
    uint16_t mantissa = a.high>>54;

    if (STATUS(default_nan_mode)) {
        return float16_default_nan;
    }

    if (mantissa) {
        return make_float16(((((uint16_t) a.sign) << 15)
                             | (0x1F << 10) | mantissa));
    } else {
        return float16_default_nan;
    }
}


int float32_is_quiet_nan( float32 a_ )
{
    uint32_t a = float32_val(a_);
#if SNAN_BIT_IS_ONE
    return ( ( ( a>>22 ) & 0x1FF ) == 0x1FE ) && ( a & 0x003FFFFF );
#else
    return ( 0xFF800000 <= (uint32_t) ( a<<1 ) );
#endif
}


int float32_is_signaling_nan( float32 a_ )
{
    uint32_t a = float32_val(a_);
#if SNAN_BIT_IS_ONE
    return ( 0xFF800000 <= (uint32_t) ( a<<1 ) );
#else
    return ( ( ( a>>22 ) & 0x1FF ) == 0x1FE ) && ( a & 0x003FFFFF );
#endif
}


float32 float32_maybe_silence_nan( float32 a_ )
{
    if (float32_is_signaling_nan(a_)) {
#if SNAN_BIT_IS_ONE
#  if defined(TARGET_MIPS) || defined(TARGET_SH4) || defined(TARGET_UNICORE32)
        return float32_default_nan;
#  else
#    error Rules for silencing a signaling NaN are target-specific
#  endif
#else
        uint32_t a = float32_val(a_);
        a |= (1 << 22);
        return make_float32(a);
#endif
    }
    return a_;
}


static commonNaNT float32ToCommonNaN( float32 a STATUS_PARAM )
{
    commonNaNT z;

    if ( float32_is_signaling_nan( a ) ) float_raise( float_flag_invalid STATUS_VAR );
    z.sign = float32_val(a)>>31;
    z.low = 0;
    z.high = ( (uint64_t) float32_val(a) )<<41;
    return z;
}


static float32 commonNaNToFloat32( commonNaNT a STATUS_PARAM)
{
    uint32_t mantissa = a.high>>41;

    if ( STATUS(default_nan_mode) ) {
        return float32_default_nan;
    }

    if ( mantissa )
        return make_float32(
            ( ( (uint32_t) a.sign )<<31 ) | 0x7F800000 | ( a.high>>41 ) );
    else
        return float32_default_nan;
}


#if defined(TARGET_ARM)
static int pickNaN(flag aIsQNaN, flag aIsSNaN, flag bIsQNaN, flag bIsSNaN,
                    flag aIsLargerSignificand)
{
    if (aIsSNaN) {
        return 0;
    } else if (bIsSNaN) {
        return 1;
    } else if (aIsQNaN) {
        return 0;
    } else {
        return 1;
    }
}
#elif defined(TARGET_MIPS)
static int pickNaN(flag aIsQNaN, flag aIsSNaN, flag bIsQNaN, flag bIsSNaN,
                    flag aIsLargerSignificand)
{
    if (aIsSNaN) {
        return 0;
    } else if (bIsSNaN) {
        return 1;
    } else if (aIsQNaN) {
        return 0;
    } else {
        return 1;
    }
}
#elif defined(TARGET_PPC)
static int pickNaN(flag aIsQNaN, flag aIsSNaN, flag bIsQNaN, flag bIsSNaN,
                   flag aIsLargerSignificand)
{
    if (aIsSNaN || aIsQNaN) {
        return 0;
    } else {
        return 1;
    }
}
#else
static int pickNaN(flag aIsQNaN, flag aIsSNaN, flag bIsQNaN, flag bIsSNaN,
                    flag aIsLargerSignificand)
{
    if (aIsSNaN) {
        if (bIsSNaN) {
            return aIsLargerSignificand ? 0 : 1;
        }
        return bIsQNaN ? 1 : 0;
    }
    else if (aIsQNaN) {
        if (bIsSNaN || !bIsQNaN)
            return 0;
        else {
            return aIsLargerSignificand ? 0 : 1;
        }
    } else {
        return 1;
    }
}
#endif


static float32 propagateFloat32NaN( float32 a, float32 b STATUS_PARAM)
{
    flag aIsQuietNaN, aIsSignalingNaN, bIsQuietNaN, bIsSignalingNaN;
    flag aIsLargerSignificand;
    uint32_t av, bv;

    aIsQuietNaN = float32_is_quiet_nan( a );
    aIsSignalingNaN = float32_is_signaling_nan( a );
    bIsQuietNaN = float32_is_quiet_nan( b );
    bIsSignalingNaN = float32_is_signaling_nan( b );
    av = float32_val(a);
    bv = float32_val(b);

    if ( aIsSignalingNaN | bIsSignalingNaN ) float_raise( float_flag_invalid STATUS_VAR);

    if ( STATUS(default_nan_mode) )
        return float32_default_nan;

    if ((uint32_t)(av<<1) < (uint32_t)(bv<<1)) {
        aIsLargerSignificand = 0;
    } else if ((uint32_t)(bv<<1) < (uint32_t)(av<<1)) {
        aIsLargerSignificand = 1;
    } else {
        aIsLargerSignificand = (av < bv) ? 1 : 0;
    }

    if (pickNaN(aIsQuietNaN, aIsSignalingNaN, bIsQuietNaN, bIsSignalingNaN,
                aIsLargerSignificand)) {
        return float32_maybe_silence_nan(b);
    } else {
        return float32_maybe_silence_nan(a);
    }
}


int float64_is_quiet_nan( float64 a_ )
{
    uint64_t a = float64_val(a_);
#if SNAN_BIT_IS_ONE
    return
           ( ( ( a>>51 ) & 0xFFF ) == 0xFFE )
        && ( a & LIT64( 0x0007FFFFFFFFFFFF ) );
#else
    return ( LIT64( 0xFFF0000000000000 ) <= (uint64_t) ( a<<1 ) );
#endif
}


int float64_is_signaling_nan( float64 a_ )
{
    uint64_t a = float64_val(a_);
#if SNAN_BIT_IS_ONE
    return ( LIT64( 0xFFF0000000000000 ) <= (uint64_t) ( a<<1 ) );
#else
    return
           ( ( ( a>>51 ) & 0xFFF ) == 0xFFE )
        && ( a & LIT64( 0x0007FFFFFFFFFFFF ) );
#endif
}


float64 float64_maybe_silence_nan( float64 a_ )
{
    if (float64_is_signaling_nan(a_)) {
#if SNAN_BIT_IS_ONE
#  if defined(TARGET_MIPS) || defined(TARGET_SH4) || defined(TARGET_UNICORE32)
        return float64_default_nan;
#  else
#    error Rules for silencing a signaling NaN are target-specific
#  endif
#else
        uint64_t a = float64_val(a_);
        a |= LIT64( 0x0008000000000000 );
        return make_float64(a);
#endif
    }
    return a_;
}


static commonNaNT float64ToCommonNaN( float64 a STATUS_PARAM)
{
    commonNaNT z;

    if ( float64_is_signaling_nan( a ) ) float_raise( float_flag_invalid STATUS_VAR);
    z.sign = float64_val(a)>>63;
    z.low = 0;
    z.high = float64_val(a)<<12;
    return z;
}


static float64 commonNaNToFloat64( commonNaNT a STATUS_PARAM)
{
    uint64_t mantissa = a.high>>12;

    if ( STATUS(default_nan_mode) ) {
        return float64_default_nan;
    }

    if ( mantissa )
        return make_float64(
              ( ( (uint64_t) a.sign )<<63 )
            | LIT64( 0x7FF0000000000000 )
            | ( a.high>>12 ));
    else
        return float64_default_nan;
}


static float64 propagateFloat64NaN( float64 a, float64 b STATUS_PARAM)
{
    flag aIsQuietNaN, aIsSignalingNaN, bIsQuietNaN, bIsSignalingNaN;
    flag aIsLargerSignificand;
    uint64_t av, bv;

    aIsQuietNaN = float64_is_quiet_nan( a );
    aIsSignalingNaN = float64_is_signaling_nan( a );
    bIsQuietNaN = float64_is_quiet_nan( b );
    bIsSignalingNaN = float64_is_signaling_nan( b );
    av = float64_val(a);
    bv = float64_val(b);

    if ( aIsSignalingNaN | bIsSignalingNaN ) float_raise( float_flag_invalid STATUS_VAR);

    if ( STATUS(default_nan_mode) )
        return float64_default_nan;

    if ((uint64_t)(av<<1) < (uint64_t)(bv<<1)) {
        aIsLargerSignificand = 0;
    } else if ((uint64_t)(bv<<1) < (uint64_t)(av<<1)) {
        aIsLargerSignificand = 1;
    } else {
        aIsLargerSignificand = (av < bv) ? 1 : 0;
    }

    if (pickNaN(aIsQuietNaN, aIsSignalingNaN, bIsQuietNaN, bIsSignalingNaN,
                aIsLargerSignificand)) {
        return float64_maybe_silence_nan(b);
    } else {
        return float64_maybe_silence_nan(a);
    }
}

#ifdef FLOATX80


int floatx80_is_quiet_nan( floatx80 a )
{
#if SNAN_BIT_IS_ONE
    uint64_t aLow;

    aLow = a.low & ~ LIT64( 0x4000000000000000 );
    return
           ( ( a.high & 0x7FFF ) == 0x7FFF )
        && (uint64_t) ( aLow<<1 )
        && ( a.low == aLow );
#else
    return ( ( a.high & 0x7FFF ) == 0x7FFF )
        && (LIT64( 0x8000000000000000 ) <= ((uint64_t) ( a.low<<1 )));
#endif
}


int floatx80_is_signaling_nan( floatx80 a )
{
#if SNAN_BIT_IS_ONE
    return ( ( a.high & 0x7FFF ) == 0x7FFF )
        && (LIT64( 0x8000000000000000 ) <= ((uint64_t) ( a.low<<1 )));
#else
    uint64_t aLow;

    aLow = a.low & ~ LIT64( 0x4000000000000000 );
    return
           ( ( a.high & 0x7FFF ) == 0x7FFF )
        && (uint64_t) ( aLow<<1 )
        && ( a.low == aLow );
#endif
}


floatx80 floatx80_maybe_silence_nan( floatx80 a )
{
    if (floatx80_is_signaling_nan(a)) {
#if SNAN_BIT_IS_ONE
#  if defined(TARGET_MIPS) || defined(TARGET_SH4) || defined(TARGET_UNICORE32)
        a.low = floatx80_default_nan_low;
        a.high = floatx80_default_nan_high;
#  else
#    error Rules for silencing a signaling NaN are target-specific
#  endif
#else
        a.low |= LIT64( 0xC000000000000000 );
        return a;
#endif
    }
    return a;
}


static commonNaNT floatx80ToCommonNaN( floatx80 a STATUS_PARAM)
{
    commonNaNT z;

    if ( floatx80_is_signaling_nan( a ) ) float_raise( float_flag_invalid STATUS_VAR);
    if ( a.low >> 63 ) {
        z.sign = a.high >> 15;
        z.low = 0;
        z.high = a.low << 1;
    } else {
        z.sign = floatx80_default_nan_high >> 15;
        z.low = 0;
        z.high = floatx80_default_nan_low << 1;
    }
    return z;
}


static floatx80 commonNaNToFloatx80( commonNaNT a STATUS_PARAM)
{
    floatx80 z;

    if ( STATUS(default_nan_mode) ) {
        z.low = floatx80_default_nan_low;
        z.high = floatx80_default_nan_high;
        return z;
    }

    if (a.high >> 1) {
        z.low = LIT64( 0x8000000000000000 ) | a.high >> 1;
        z.high = ( ( (uint16_t) a.sign )<<15 ) | 0x7FFF;
    } else {
        z.low = floatx80_default_nan_low;
        z.high = floatx80_default_nan_high;
    }

    return z;
}


static floatx80 propagateFloatx80NaN( floatx80 a, floatx80 b STATUS_PARAM)
{
    flag aIsQuietNaN, aIsSignalingNaN, bIsQuietNaN, bIsSignalingNaN;
    flag aIsLargerSignificand;

    aIsQuietNaN = floatx80_is_quiet_nan( a );
    aIsSignalingNaN = floatx80_is_signaling_nan( a );
    bIsQuietNaN = floatx80_is_quiet_nan( b );
    bIsSignalingNaN = floatx80_is_signaling_nan( b );

    if ( aIsSignalingNaN | bIsSignalingNaN ) float_raise( float_flag_invalid STATUS_VAR);

    if ( STATUS(default_nan_mode) ) {
        a.low = floatx80_default_nan_low;
        a.high = floatx80_default_nan_high;
        return a;
    }

    if (a.low < b.low) {
        aIsLargerSignificand = 0;
    } else if (b.low < a.low) {
        aIsLargerSignificand = 1;
    } else {
        aIsLargerSignificand = (a.high < b.high) ? 1 : 0;
    }

    if (pickNaN(aIsQuietNaN, aIsSignalingNaN, bIsQuietNaN, bIsSignalingNaN,
                aIsLargerSignificand)) {
        return floatx80_maybe_silence_nan(b);
    } else {
        return floatx80_maybe_silence_nan(a);
    }
}

#endif

#ifdef FLOAT128


int float128_is_quiet_nan( float128 a )
{
#if SNAN_BIT_IS_ONE
    return
           ( ( ( a.high>>47 ) & 0xFFFF ) == 0xFFFE )
        && ( a.low || ( a.high & LIT64( 0x00007FFFFFFFFFFF ) ) );
#else
    return
           ( LIT64( 0xFFFE000000000000 ) <= (uint64_t) ( a.high<<1 ) )
        && ( a.low || ( a.high & LIT64( 0x0000FFFFFFFFFFFF ) ) );
#endif
}


int float128_is_signaling_nan( float128 a )
{
#if SNAN_BIT_IS_ONE
    return
           ( LIT64( 0xFFFE000000000000 ) <= (uint64_t) ( a.high<<1 ) )
        && ( a.low || ( a.high & LIT64( 0x0000FFFFFFFFFFFF ) ) );
#else
    return
           ( ( ( a.high>>47 ) & 0xFFFF ) == 0xFFFE )
        && ( a.low || ( a.high & LIT64( 0x00007FFFFFFFFFFF ) ) );
#endif
}


float128 float128_maybe_silence_nan( float128 a )
{
    if (float128_is_signaling_nan(a)) {
#if SNAN_BIT_IS_ONE
#  if defined(TARGET_MIPS) || defined(TARGET_SH4) || defined(TARGET_UNICORE32)
        a.low = float128_default_nan_low;
        a.high = float128_default_nan_high;
#  else
#    error Rules for silencing a signaling NaN are target-specific
#  endif
#else
        a.high |= LIT64( 0x0000800000000000 );
        return a;
#endif
    }
    return a;
}


static commonNaNT float128ToCommonNaN( float128 a STATUS_PARAM)
{
    commonNaNT z;

    if ( float128_is_signaling_nan( a ) ) float_raise( float_flag_invalid STATUS_VAR);
    z.sign = a.high>>63;
    shortShift128Left( a.high, a.low, 16, &z.high, &z.low );
    return z;
}


static float128 commonNaNToFloat128( commonNaNT a STATUS_PARAM)
{
    float128 z;

    if ( STATUS(default_nan_mode) ) {
        z.low = float128_default_nan_low;
        z.high = float128_default_nan_high;
        return z;
    }

    shift128Right( a.high, a.low, 16, &z.high, &z.low );
    z.high |= ( ( (uint64_t) a.sign )<<63 ) | LIT64( 0x7FFF000000000000 );
    return z;
}


static float128 propagateFloat128NaN( float128 a, float128 b STATUS_PARAM)
{
    flag aIsQuietNaN, aIsSignalingNaN, bIsQuietNaN, bIsSignalingNaN;
    flag aIsLargerSignificand;

    aIsQuietNaN = float128_is_quiet_nan( a );
    aIsSignalingNaN = float128_is_signaling_nan( a );
    bIsQuietNaN = float128_is_quiet_nan( b );
    bIsSignalingNaN = float128_is_signaling_nan( b );

    if ( aIsSignalingNaN | bIsSignalingNaN ) float_raise( float_flag_invalid STATUS_VAR);

    if ( STATUS(default_nan_mode) ) {
        a.low = float128_default_nan_low;
        a.high = float128_default_nan_high;
        return a;
    }

    if (lt128(a.high<<1, a.low, b.high<<1, b.low)) {
        aIsLargerSignificand = 0;
    } else if (lt128(b.high<<1, b.low, a.high<<1, a.low)) {
        aIsLargerSignificand = 1;
    } else {
        aIsLargerSignificand = (a.high < b.high) ? 1 : 0;
    }

    if (pickNaN(aIsQuietNaN, aIsSignalingNaN, bIsQuietNaN, bIsSignalingNaN,
                aIsLargerSignificand)) {
        return float128_maybe_silence_nan(b);
    } else {
        return float128_maybe_silence_nan(a);
    }
}

#endif
