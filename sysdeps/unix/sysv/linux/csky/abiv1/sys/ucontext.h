/* struct ucontext definition, C-SKY version.
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

#ifndef _SYS_UCONTEXT_H
#define _SYS_UCONTEXT_H	1

#include <features.h>

#include <bits/types/sigset_t.h>
#include <bits/types/stack_t.h>

#ifdef __USE_MISC
# define __ctx(fld) fld
#else
# define __ctx(fld) __ ## fld
#endif

typedef struct
  {
    unsigned long __ctx(tls);
    unsigned long __ctx(lr);
    unsigned long __ctx(pc);
    unsigned long __ctx(sr);
    unsigned long __ctx(usp);

    /*
     * a0, a1, a2, a3:
     * ABIV1: r2, r3, r4, r5
     */

    unsigned long __ctx(orig_a0);
    unsigned long __ctx(a0);
    unsigned long __ctx(a1);
    unsigned long __ctx(a2);
    unsigned long __ctx(a3);

    /*
     * ABIV1: r6 ~ r14, r1
     */
    unsigned long __ctx(regs)[10];

  } gregset_t;

typedef struct
  {
    unsigned long __ctx(vr)[64];
    unsigned long __ctx(fcr);
    unsigned long __ctx(fesr);
    unsigned long __ctx(fid);
    unsigned long __glibc_reserved;
  } fpregset_t;

/* Context to describe whole processor state. */
typedef struct
  {
    unsigned long __ctx(mask);
    gregset_t __ctx(gregs);
    fpregset_t __ctx(fpregs);
  } mcontext_t;

/* Userlevel context.  */
typedef struct ucontext_t
  {
    unsigned long int __ctx(uc_flags);
    struct ucontext_t *uc_link;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
    sigset_t uc_sigmask;
  } ucontext_t;

#undef __ctx


#endif /* sys/ucontext.h */
