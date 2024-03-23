#ifndef SETTINGS_H
#define SETTINGS_H

// Some compilers define isnan, isfinite, and isinf as macros, even for
// C++ codes, which cause havoc when overloading these functions.  We undef
// them here.
#ifdef isnan
#undef isnan
#endif

#ifdef isfinite
#undef isfinite
#endif

#ifdef isinf
#undef isinf
#endif

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

#ifndef ISNAN
/* Define this macro to be the isnan(x) function. */
#define ISNAN(x) std::isnan(x)
#endif

#ifndef ISFINITE
/* Define this macro to be the isfinite(x) function. */
#define ISFINITE(x) std::isfinite(x)
#endif

#ifndef ISINF
/* Define this macro to be the isinf(x) function. */
#define ISINF(x) std::isinf(x)
#endif

// Disables floating expression contraction. By default most compilers disable floating expression contraction and
// also disable support for FP_CONTRACT pragma. Support for this pragma can be enabled through compiler flags.
// This is included in the event that the user has enabled FP_CONTRACT.
#pragma STDC FP_CONTRACT OFF

// Controls the addition routine tradeoff. Do we go for more accurate addition or faster but less accurate.
#ifndef ACCURATE_ADD
#define ACCURATE_ADD 1
#endif

#define _SPLITTER 134217729.0               // = 2^27 + 1
#define _SPLIT_THRESH 6.69692879491417e+299 // = 2^996

// TODO: Put these in some top most level config file possibly.
// Detects if FMA hardware instruction set is available for use on a computer.
// __FMA__ is defined if the instruction set is available.
#ifndef __FMA__
#if defined(__AVX2__) || defined(__AVX__)
#define __FMA__ 1
#endif
#endif

#endif // SETTINGS_H
