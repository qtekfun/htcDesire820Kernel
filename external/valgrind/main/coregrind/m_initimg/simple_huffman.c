/*************************************************************************
* Name:        huffman.c
* Author:      Marcus Geelnard
* Description: Huffman coder/decoder implementation.
* Reentrant:   Yes
* $Id: huffman.c,v 1.6 2004/12/14 18:59:40 marcus256 Exp $
*
* This is a very straight forward implementation of a Huffman coder and
* decoder.
*
* Primary flaws with this primitive implementation are:
*  - Slow bit stream implementation
*  - Fairly slow decoding (slower than encoding)
*  - Maximum tree depth of 32 (the coder aborts if any code exceeds a
*    size of 32 bits). If I'm not mistaking, this should not be possible
*    unless the input buffer is larger than 2^32 bytes, which is not
*    supported by the coder anyway (max 2^32-1 bytes can be specified with
*    an unsigned 32-bit integer).
*
* On the other hand, there are a few advantages of this implementation:
*  - The Huffman tree is stored in a very compact form, requiring only
*    12 bits per symbol (for 8 bit symbols), meaning a maximum of 384
*    bytes overhead.
*  - The Huffman coder does quite well in situations where the data is
*    noisy, in which case most dictionary based coders run into problems.
*
* Possible improvements (probably not worth it):
*  - Partition the input data stream into blocks, where each block has
*    its own Huffman tree. With variable block sizes, it should be
*    possible to find locally optimal Huffman trees, which in turn could
*    reduce the total size.
*  - Allow for a few different predefined Huffman trees, which could
*    reduce the size of a block even further.
*-------------------------------------------------------------------------
* Copyright (c) 2003-2011 Marcus Geelnard
*
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would
*    be appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not
*    be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source
*    distribution.
*
* Marcus Geelnard
* marcus.geelnard at home.se
*************************************************************************/



typedef struct {
    UInt Symbol;
    UInt Count;
    UInt Code;
    UInt Bits;
} huff_sym_t;

typedef struct {
    UChar *BytePtr;
    UInt  BitPos;
} huff_bitstream_t;






static void _Huffman_InitBitstream( huff_bitstream_t *stream,
    UChar *buf )
{
    stream->BytePtr  = buf;
    stream->BitPos   = 0;
}



static UInt _Huffman_ReadBits( huff_bitstream_t *stream,
    UInt bits )
{
    UInt  x, bit, count;
    UChar *buf;

    
    buf = stream->BytePtr;
    bit = stream->BitPos;

    
    x = 0;
    for( count = 0; count < bits; ++ count )
    {
        x = (x<<1) + (*buf & (1<<(7-bit)) ? 1 : 0);
        bit = (bit+1) & 7;
        if( !bit )
        {
            ++ buf;
        }
    }

    
    stream->BytePtr = buf;
    stream->BitPos  = bit;

    return x;
}



static void _Huffman_WriteBits( huff_bitstream_t *stream, UInt x,
    UInt bits )
{
    UInt  bit, count;
    UChar *buf;
    UInt  mask;

    
    buf = stream->BytePtr;
    bit = stream->BitPos;

    
    mask = 1 << (bits-1);
    for( count = 0; count < bits; ++ count )
    {
        if (bit == 0)
           *buf = 0;
        *buf = (*buf & (0xff^(1<<(7-bit)))) +
               ((x & mask ? 1 : 0) << (7-bit));
        x <<= 1;
        bit = (bit+1) & 7;
        if( !bit )
        {
            ++ buf;
        }
    }

    
    stream->BytePtr = buf;
    stream->BitPos  = bit;
}



static void _Huffman_Hist( UChar *in, huff_sym_t *sym,
    UInt size )
{
    Int k, swaps;
    huff_sym_t tmp;

    
    for( k = 0; k < 256; ++ k )
    {
        sym[k].Symbol = k;
        sym[k].Count  = 0;
        sym[k].Code   = 0;
        sym[k].Bits   = 0;
    }

    
    for( k = size; k; -- k )
    {
        sym[ *in ++ ].Count ++;
    }

    
    do
    {
        swaps = 0;
        for( k = 0; k < 255; ++ k )
        {
            if( sym[k].Count < sym[k+1].Count )
            {
                tmp      = sym[k];
                sym[k]   = sym[k+1];
                sym[k+1] = tmp;
                swaps    = 1;
            }
        }
    }
    while( swaps );
}



static void _Huffman_MakeTree( huff_sym_t *sym, huff_bitstream_t *stream,
    UInt code, UInt bits, UInt first,
    UInt last )
{
    UInt k, size, size_a, size_b, last_a, first_b;

    
    if( first == last )
    {
        
        _Huffman_WriteBits( stream, 1, 1 );
        _Huffman_WriteBits( stream, sym[first].Symbol, 8 );

        
        sym[first].Code = code;
        sym[first].Bits = bits;
        return;
    }
    else
    {
        
        _Huffman_WriteBits( stream, 0, 1 );
    }

    
    size = 0;
    for( k = first; k <= last; ++ k )
    {
        size += sym[k].Count;
    }

    
    size_a = 0;
    for( k = first; size_a < ((size+1)>>1) && k < last; ++ k )
    {
        size_a += sym[k].Count;
    }

    
    if( size_a > 0 )
    {
        
        _Huffman_WriteBits( stream, 1, 1 );

        
        last_a  = k-1;

        
        _Huffman_MakeTree( sym, stream, (code<<1)+0, bits+1,
                               first, last_a );
    }
    else
    {
        
        _Huffman_WriteBits( stream, 0, 1 );
    }

    
    size_b = size - size_a;

    
    if( size_b > 0 )
    {
        
        _Huffman_WriteBits( stream, 1, 1 );

        
        first_b = k;

        
        _Huffman_MakeTree( sym, stream, (code<<1)+1, bits+1,
                               first_b, last );
    }
    else
    {
        
        _Huffman_WriteBits( stream, 0, 1 );
    }
}



static void _Huffman_RecoverTree( huff_sym_t *sym,
    huff_bitstream_t *stream, UInt code, UInt bits,
    UInt *symnum )
{
    UInt symbol;

    
    if( _Huffman_ReadBits( stream, 1 ) )
    {
        
        symbol = _Huffman_ReadBits( stream, 8 );

        
        sym[*symnum].Symbol = symbol;
        sym[*symnum].Code   = code;
        sym[*symnum].Bits   = bits;

        
        *symnum = *symnum + 1;

        return;
    }

    
    if( _Huffman_ReadBits( stream, 1 ) )
    {
        
        _Huffman_RecoverTree( sym, stream, (code<<1)+0, bits+1,
                              symnum );
    }

    
    if( _Huffman_ReadBits( stream, 1 ) )
    {
        
        _Huffman_RecoverTree( sym, stream, (code<<1)+1, bits+1,
                              symnum );
    }
}






static
Int Huffman_Compress( UChar *in, UChar *out,
    UInt insize )
{
    huff_sym_t       sym[ 256 ], tmp;
    huff_bitstream_t stream;
    UInt     k, total_bytes, swaps, symbol, last_symbol;

    
    if( insize < 1 ) return 0;

    
    _Huffman_InitBitstream( &stream, out );

    
    _Huffman_Hist( in, sym, insize );

    
    for( last_symbol = 255; sym[last_symbol].Count == 0; -- last_symbol );

    if( last_symbol == 0 ) ++ last_symbol;

    
    _Huffman_MakeTree( sym, &stream, 0, 0, 0, last_symbol );

    
    for( k = 0; k < 255; ++ k )
    {
        if( sym[k].Bits > 32 )
        {
            return 0;
        }
    }

    
    do
    {
        swaps = 0;
        for( k = 0; k < 255; ++ k )
        {
            if( sym[k].Symbol > sym[k+1].Symbol )
            {
                tmp      = sym[k];
                sym[k]   = sym[k+1];
                sym[k+1] = tmp;
                swaps    = 1;
            }
        }
    }
    while( swaps );

    
    for( k = 0; k < insize; ++ k )
    {
        symbol = in[ k ];
        _Huffman_WriteBits( &stream, sym[symbol].Code,
                            sym[symbol].Bits );
    }

    
    total_bytes = (Int)(stream.BytePtr - out);
    if( stream.BitPos > 0 )
    {
        ++ total_bytes;
    }

    return total_bytes;
}



static
void Huffman_Uncompress( UChar *in, UChar *out,
    UInt insize, UInt outsize )
{
    huff_sym_t       sym[ 256 ], tmp;
    huff_bitstream_t stream;
    UInt     k, m, symbol_count, swaps;
    UChar    *buf;
    UInt     bits, delta_bits, new_bits, code;

    
    if( insize < 1 ) return;

    
    _Huffman_InitBitstream( &stream, in );

    
    for( k = 0; k < 256; ++ k )
    {
        sym[k].Bits = 0x7fffffff;
    }

    
    symbol_count = 0;
    _Huffman_RecoverTree( sym, &stream, 0, 0, &symbol_count );

    
    do
    {
        swaps = 0;
        for( k = 0; k < symbol_count-1; ++ k )
        {
            if( sym[k].Bits > sym[k+1].Bits )
            {
                tmp      = sym[k];
                sym[k]   = sym[k+1];
                sym[k+1] = tmp;
                swaps    = 1;
            }
        }
    }
    while( swaps );

    
    buf = out;
    for( k = 0; k < outsize; ++ k )
    {
        
        bits = 0;
        code = 0;
        for( m = 0; m < symbol_count; ++ m )
        {
            delta_bits = sym[m].Bits - bits;
            if( delta_bits )
            {
                new_bits = _Huffman_ReadBits( &stream, delta_bits );
                code = code | (new_bits << (32-bits-delta_bits));
                bits = sym[m].Bits;
            }
            if( code == (sym[m].Code << (32-sym[m].Bits)) )
            {
                *buf ++ = (UChar) sym[m].Symbol;
                break;
            }
        }
    }
}
