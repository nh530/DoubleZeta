#include <cfloat>
#include <cmath>

bool cmpf(float A, float B, float epsilon) { return (fabs(A - B) < epsilon); }

bool cmp_f_rel(float A, float B, float maxRelDiff = FLT_EPSILON) {
  /* Description
   * -----------
   * This function implements the relative float epsilon comparison algorithm.
   * To compare f1 and f2 calculate diff = fabs(f1-f2). If diff is smaller
   * than nmaxRelDiff% of max(abs(f1),abs(f2)), then f1 and f2 can be considered equal.
   *
   * Notes
   * -----
   * When doing a relative comparison of floats it works pretty well to set maxRelDiff to FLT_EPSILON,
   * or some small integer multiple of FLT_EPSILON. Anything smaller than that and it risks being equivalent to no epsilon
   * */
  // Calculate the difference.
  float diff = fabs(A - B);
  A = fabs(A);
  B = fabs(B);
  // Find the largest
  float largest = (B > A) ? B : A;

  if (diff <= largest * maxRelDiff)
    return true;
  return false;
}
