# clang complier
This package requires very explicit settings to be applied in order to ensure the correctness of low-level floating point arithmetic and algorithms.
The following flags need to be added during compilation:
- `-mfma` or `-march=native`

On the other hand, the FP\_CONTRACT Pragma needs to be turned off or disabled.


# `__FMA__` Macro
This macro is used to determine the availability of FMA hardware, which is specific hardware used to optimize and provide instruction sets for the
FMA algorithm. When hardware is available, then this MACRO will be defined and have a value of 1.

# `__AVX__` and `__AVX2__` Macro
This macro is present if AVX2 is available. Advanced Vector Extensions (AVX) are extensions to the x86 instruction set architecture for 
microprocessors from Intel and Advanced Micro Devices (AMD), and these extensions provide hardware support for the FMA algorithm. The __AVX__ preprocessor symbol is defined when the /arch:AVX, /arch:AVX2 or /arch:AVX512 compiler option is specified.
The __AVX2__ preprocessor symbol is defined when the /arch:AVX2 or /arch:AVX512 compiler option is specified. 


# References
- https://stackoverflow.com/questions/16348909/how-do-i-know-if-i-can-compile-with-fma-instruction-sets#:~:text=To%20reliably%20detect%20FMA%20(or,macro%20for%20AMD%20FMA4%20support.
- https://stackoverflow.com/questions/18868235/preventing-gcc-from-automatically-using-avx-and-fma-instructions-when-compiled-w
