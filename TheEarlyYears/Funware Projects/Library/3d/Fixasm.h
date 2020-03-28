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

/*
// Fix point multiplacation
#define fixmul( Result, Fac1, Fac2, Prec )\
  { asm { mov  eax, Fac1 }\
    asm { imul Fac2 }\
    asm { shrd eax, edx, 32-Prec }\
    asm { mov  Result, eax } }

// Fix point division
// Result = ( Num / Denom ) << Prec
// Prec = 16 or 0x10
// For speed reasons, I have hard coded the Prec value
// so that it in not read from memory.
#define fixdiv( Result, Num, Denom )\
  { asm { mov  eax, Num }\
    asm { mov  edx, eax }\
    asm { sar  edx, 0x10 }\
    asm { shl  eax, 0x10 }\
    asm { idiv Denom }\
    asm { mov  Result, eax } }
*/

#define fixdiv( Result, Num, Denom, Prec )\
  { _asm { mov  eax, Num }\
    _asm { cdq }\
    _asm { mov  cl, Prec }\
    _asm { shld  edx, eax, cl }\
    _asm { sal  eax, cl }\
    _asm { idiv Denom }\
    _asm { mov  Result, eax } }

#endif
