;******************************************************************************
;    PRODUCT:       WMS 3D Engine
;   
;    FILE NAME:     FixDiv.asm
;   
;    DESCRIPTION:   Low-level assembly routine
;         
;    Copyright(c) 2000 WMS Gaming, Inc.
;    All Rights Reserved
;      
;******************************************************************************
;*****************************************************************************
;   Module Name
;*****************************************************************************
NAME Fix_Division

;*****************************************************************************
;   Publics/Externs
;*****************************************************************************
PUBLIC  fixdiva

;*****************************************************************************
;   All of this is 32-bit code in segment CODE32
;*****************************************************************************
CODE32 SEGMENT USE32 EO public
  
;****************************************************************************
;       void fixdiv()
;
;       Fix the intensity interpolation division
;
;****************************************************************************
;extern void fixdiv( int *Result, int Num, int Denom, int Prec );
;fixdiv proc near
;    push ebx
;    push ecx
;    push edx
;    mov  eax, [esp+8]
;    mov  ecx, [esp+16]
;    mov  edx, eax
;    sar  edx, cl
;    shl  eax, cl
;    idiv DWORD PTR [esp+12]
;    mov  [esp+4], ebx
;    mov  [ebx], eax
;    pop edx
;    pop ecx
;    pop ebx
;    ret
;fixdiv endp

; int fixdiv( Num, Denom )
fixdiv proc near
    mov  eax, [esp+4]
    mov  edx, eax
    sar  edx, 16
    shl  eax, 16
    idiv DWORD PTR [esp+8]
    ;mov  [esp+4], eax
    ret
fixdiv endp

;#define fixdiv( Result, Num, Denom, int Prec )\
;  { asm { mov  eax, Num }\
;    asm { mov  edx, eax }\
;    asm { sar  edx, 16 }\
;    asm { shl  eax, 16 }\
;    asm { idiv Denom }\
;    asm { mov  Result, eax } }

;#define fixdiv( Result, Num, Denom, Prec )\
;  { asm { mov  eax, Num }\
;    asm { mov  edx, eax }\
;    asm { sar  edx, Prec }\
;    asm { shl  eax, Prec }\
;    asm { idiv Denom }\
;    asm { mov  Result, eax } }


CODE32 ENDS

END