/* Copyright (C) 1991-2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 8.0.0.  Version 8.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2014, plus Amendment 1 (published
   2015-05-15).  */
/* We do not support C11 <threads.h>.  */
int absVal(int);
int test_absVal(int);
int addOK(int, int);
int test_addOK(int, int);
int allEvenBits();
int test_allEvenBits();
int allOddBits();
int test_allOddBits();
int anyEvenBit();
int test_anyEvenBit();
int anyOddBit();
int test_anyOddBit();
int bang(int);
int test_bang(int);
int bitAnd(int, int);
int test_bitAnd(int, int);
int bitCount(int);
int test_bitCount(int);
int bitMask(int, int);
int test_bitMask(int, int);
int bitNor(int, int);
int test_bitNor(int, int);
int bitOr(int, int);
int test_bitOr(int, int);
int bitParity(int);
int test_bitParity(int);
int bitXor(int, int);
int test_bitXor(int, int);
int byteSwap(int, int, int);
int test_byteSwap(int, int, int);
int conditional(int, int, int);
int test_conditional(int, int, int);
int copyLSB(int);
int test_copyLSB(int);
int divpwr2(int, int);
int test_divpwr2(int, int);
int evenBits();
int test_evenBits();
int ezThreeFourths(int);
int test_ezThreeFourths(int);
int fitsBits(int, int);
int test_fitsBits(int, int);
int fitsShort(int);
int test_fitsShort(int);
unsigned float_abs(unsigned);
unsigned test_float_abs(unsigned);
int float_f2i(unsigned);
int test_float_f2i(unsigned);
unsigned float_half(unsigned);
unsigned test_float_half(unsigned);
unsigned float_i2f(int);
unsigned test_float_i2f(int);
unsigned float_neg(unsigned);
unsigned test_float_neg(unsigned);
unsigned float_twice(unsigned);
unsigned test_float_twice(unsigned);
int getByte(int, int);
int test_getByte(int, int);
int greatestBitPos(int);
int test_greatestBitPos(int);
int howManyBits(int);
int test_howManyBits(int);
int ilog2(int);
int test_ilog2(int);
int implication(int, int);
int test_implication(int, int);
int isAsciiDigit(int);
int test_isAsciiDigit(int);
int isEqual(int, int);
int test_isEqual(int, int);
int isGreater(int, int);
int test_isGreater(int, int);
int isLess(int, int);
int test_isLess(int, int);
int isLessOrEqual(int, int);
int test_isLessOrEqual(int, int);
int isNegative(int);
int test_isNegative(int);
int isNonNegative(int);
int test_isNonNegative(int);
int isNonZero(int);
int test_isNonZero(int);
int isNotEqual(int, int);
int test_isNotEqual(int, int);
int isPositive(int);
int test_isPositive(int);
int isPower2(int);
int test_isPower2(int);
int isTmax(int);
int test_isTmax(int);
int isTmin(int);
int test_isTmin(int);
int isZero(int);
int test_isZero(int);
int leastBitPos(int);
int test_leastBitPos(int);
int leftBitCount(int);
int test_leftBitCount(int);
int logicalNeg(int);
int test_logicalNeg(int);
int logicalShift(int, int);
int test_logicalShift(int, int);
int minusOne();
int test_minusOne();
int multFiveEighths(int);
int test_multFiveEighths(int);
int negate(int);
int test_negate(int);
int oddBits();
int test_oddBits();
int rempwr2(int, int);
int test_rempwr2(int, int);
int replaceByte(int, int, int);
int test_replaceByte(int, int, int);
int rotateLeft(int, int);
int test_rotateLeft(int, int);
int rotateRight(int, int);
int test_rotateRight(int, int);
int satAdd(int, int);
int test_satAdd(int, int);
int satMul2(int);
int test_satMul2(int);
int satMul3(int);
int test_satMul3(int);
int sign(int);
int test_sign(int);
int sm2tc(int);
int test_sm2tc(int);
int subOK(int, int);
int test_subOK(int, int);
int tc2sm(int);
int test_tc2sm(int);
int thirdBits();
int test_thirdBits();
int tmax();
int test_tmax();
int tmin();
int test_tmin();
int trueFiveEighths(int);
int test_trueFiveEighths(int);
int trueThreeFourths(int);
int test_trueThreeFourths(int);
int upperBits(int);
int test_upperBits(int);
