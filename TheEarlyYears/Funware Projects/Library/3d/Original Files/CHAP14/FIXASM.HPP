//
// File name: FixASM.HPP
//
// Description: Several fixed-point assembly macros.
//
// Target: 32-bit
//
// Project: Cutting Edge 3-D Game Programming
//

#ifndef FIXASMHPP
#define FIXASMHPP

#define fixmul( Result, Fac1, Fac2, Prec )\
  { asm { mov  eax, Fac1 }\
    asm { imul Fac2 }\
    asm { shrd eax, edx, 32-Prec }\
    asm { mov  Result, eax } }

#define fixdiv( Result, Num, Denom, Prec )\
  { asm { mov  eax, Num }\
    asm { mov  edx, eax }\
    asm { sar  edx, Prec }\
    asm { shl  eax, 32-Prec }\
    asm { idiv Denom }\
    asm { mov  Result, eax } }

#endif