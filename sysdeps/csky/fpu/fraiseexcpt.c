/* Raise given exceptions.
   Copyright (C) 2018 Free Software Foundation, Inc.
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
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include <fenv.h>
#include <fenv_libc.h>
#include <fpu_control.h>
#include <float.h>
#include <math.h>

int
__feraiseexcept (int excepts)
{
#ifdef __csky_hard_float__
    /* Raise exceptions represented by EXCEPTS.  But we must raise only one
     signal at a time.  It is important that if the overflow/underflow
     exception and the divide by zero exception are given at the same
     time, the overflow/underflow exception follows the divide by zero
     exception.  */

    /* First: invalid exception.  */
    if (FE_INVALID & excepts)
    {
      /* One example of a invalid operation is 0 * Infinity.  */
      float x = HUGE_VALF, y = 0.0f;
      __asm__ __volatile__ ("fmuls %0, %0, %1" : "+v" (x) : "v" (y));
    }

    /* Next: division by zero.  */
    if (FE_DIVBYZERO & excepts)
    {
      float x = 1.0f, y = 0.0f;
      __asm__ __volatile__ ("fdivs %0, %0, %1" : "+v" (x) : "v" (y));
    }

    /* Next: overflow.  */
    if (FE_OVERFLOW & excepts)
    {
      float x = FLT_MAX;
      __asm__ __volatile__ ("fmuls %0, %0, %0" : "+v" (x));
    }
    /* Next: underflow.  */
    if (FE_UNDERFLOW & excepts)
    {
      float x = -FLT_MIN;

      __asm__ __volatile__ ("fmuls %0, %0, %0" : "+v" (x));
    }

    /* Last: inexact.  */
    if (FE_INEXACT & excepts)
    {
      float x = 1.0f, y = 3.0f;
      __asm__ __volatile__ ("fdivs %0, %0, %1" : "+v" (x) : "v" (y));
    }

    if (FE_DENORMAL & excepts)
    {
      double x = 4.9406564584124654e-324;
      __asm__ __volatile__ ("fstod %0, %0" : "+v" (x));
    }

    /* Success.  */
    return 0;
#else /* __csky_hard_float__ */
  /* Unsupported, so fail unless nothing needs to be done.  */
  return (excepts != 0);
#endif
}
libm_hidden_def (__feraiseexcept)
weak_alias (__feraiseexcept, feraiseexcept)
libm_hidden_weak (feraiseexcept)
