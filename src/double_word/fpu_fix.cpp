/*
 * Contains functions to set the round-to-double flag for the precision control bits in the
 * control word of a x86 FPU and restores the precision control flag to
 * original state.
 *
 * Reference: https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
 * http://www.ray.masmcode.com/tutorial/fpuchap1.htm
 * https://www.website.masmforum.com/tutorials/fptute/fpuchap2.htm
 * https://www.felixcloutier.com/x86/fstcw:fnstcw
 * https://www.felixcloutier.com/x86/fldcw
 */

#include <double_word/fpu_fix.h>

#ifdef X86
#ifdef _WIN32

#include <float.h>
#else
#ifdef HAVE_FPU_CONTROL_H
#include <fpu_control.h>
#endif // HAVE_FPU_CONTROL_H

#ifndef _FPU_GETCW
// Store the FPU control word in memory in variable x.
#define _FPU_GETCW(x) asm volatile("fnstcw %0" : "=m"(x));
#endif // _FPU_GETCW

#ifndef _FPU_SETCW
// Loads the 16-bit source operand into the FPU control word. The source operand is a memory location.
// This instruction is typically used to establish or change the FPU’s mode of operation.
#define _FPU_SETCW(x) asm volatile("fldcw %0" : : "m"(x));
#endif //_FPU_SETCW

#ifndef _FPU_EXTENDED
// 0x0300 (hex number) = 768 (decimal)
#define _FPU_EXTENDED 0x0300
#endif // _FPU_EXTENDED

#ifndef _FPU_DOUBLE
// 0x0200 (hex number) = 512 (decimal)
#define _FPU_DOUBLE 0x0200
#endif // _FPU_DOUBLE

#endif // _WIN32
#endif /* X86 */

void fpu_fix_start(unsigned int *old_cw) {
#ifdef X86
#ifdef _WIN32
#ifdef __BORLANDC__
  /* Win 32 Borland C */
  unsigned short cw = _control87(0, 0); // Get current control word.
  _control87(0x0200, 0x0300);
  if (old_cw) { // Old control word is not saved if NULL pointer.
    *old_cw = cw;
  }
#else
  /* Win 32 MSVC */
  unsigned int cw = _control87(0, 0);
  _control87(0x00010000, 0x00030000);
  if (old_cw) {
    *old_cw = cw;
  }
#endif
#else
  /* Linux */
  volatile unsigned short cw, new_cw;
  _FPU_GETCW(cw);

  // ~ operator is the Binary One's complement operator
  // Getting all the control word bits except for the precision control bits.
  // Then setting the precision control bits to 10, which corresponds to Double precision.
  // This is done to turn off the use of extended precision in the floating point unit.
  new_cw = (cw & ~_FPU_EXTENDED) | _FPU_DOUBLE;
  _FPU_SETCW(new_cw);

  if (old_cw) {
    *old_cw = cw;
  }
#endif
#endif
}

void fpu_fix_end(unsigned int *old_cw) {
#ifdef X86
#ifdef _WIN32

#ifdef __BORLANDC__
  /* Win 32 Borland C */
  if (old_cw) {
    unsigned short cw = (unsigned short)*old_cw;
    _control87(cw, 0xFFFF);
  }
#else
  /* Win 32 MSVC */
  if (old_cw) {
    _control87(*old_cw, 0xFFFFFFFF);
  }
#endif

#else
  /* Linux */
  if (old_cw) {
    int cw;
    cw = *old_cw;
    _FPU_SETCW(cw);
  }
#endif
#endif
}
