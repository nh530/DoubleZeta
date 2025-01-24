/*
 * Contains functions to set and restore the precision control in the
 * control word of a x86 FPU.  The algorithms in the double-word arithmetic
 * does not function with the extended precision mode found in these FPU.
 */
#ifndef FPU_FIX_H
#define FPU_FIX_H

/*
 * Set the round-to-double precision in the FPU, and save the old control word in old_cw.
 * This prevents the FPU coprocessor from using the extended precision, which is usually the
 * default precision on x86.
 * If old_cw is NULL, the old control word is not saved.
 */
void fpu_fix_start(unsigned int *old_cw);

/*
 * Restore the control word to original state
 */
void fpu_fix_end(unsigned int *old_cw);

#endif
