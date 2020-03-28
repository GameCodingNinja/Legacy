;******************************************************************************
;    PRODUCT:       Assembly functions
;   
;    FILE NAME:     FixDiv.asm
;   
;    DESCRIPTION:   Low-level assembly routines
;         
;    Copyright(c) by Howard C Rosenorn
;    All Rights Reserved
;      
;******************************************************************************

;*****************************************************************************
;   modual setup
;*****************************************************************************
.386                                            ; Specify target instruction set
locals @@                                         ; Enable block-scoped labels
jumps                                           ; Enable auto-conditional jumps
.model flat, stdcall                            ; 32-bit flat memory model

;*****************************************************************************
;   Publics/Externs
;
;   Use an "_" for a standard function call:  _memor
;   Use an "@" for a _fastcall function call: @memor
;*****************************************************************************

public _memor, _memand

;*****************************************************************************
;   data
;*****************************************************************************
;.data


;*****************************************************************************
;   All of this is 32-bit code in segment CODE32
;*****************************************************************************
.code

_memor proc c uses edx ebx ecx
    arg dest:dword, source:dword, sizeInDWORDs:dword

    mov edx,dest          ; Load the dest pointer into edi
    mov ebx,source        ; Load the source pointer into esi
    mov ecx,sizeInDWORDs  ; load the counter into ecx
	dec ecx               ; dec the counter

@@10:
    
    mov eax,[ebx+ecx*4]   ; move the value of the source into eax
    or [edx+ecx*4],eax    ; AND the source value onto the dest value
	dec ecx               ; dec the counter
    jnz @@10              ; jump if not 0

    mov eax,[ebx+ecx*4]   ; move the value of the source into eax
    or [edx+ecx*4],eax    ; AND the source value onto the dest value

    ret                   ; return to caller
_memor endp


_memand proc c uses edx ebx ecx
    arg dest:dword, source:dword, sizeInDWORDs:dword

    mov edx,dest          ; Load the dest pointer into edi
    mov ebx,source        ; Load the source pointer into esi
    mov ecx,sizeInDWORDs  ; load the counter into ecx
	dec ecx               ; dec the counter

@@10:
    mov eax,[ebx+ecx*4]   ; move the value of the source into eax
    and [edx+ecx*4],eax   ; AND the source value onto the dest value
	dec ecx               ; dec the counter
    jnz @@10              ; jump if not 0

    mov eax,[ebx+ecx*4]   ; move the value of the source into eax
    and [edx+ecx*4],eax   ; AND the source value onto the dest value

    ret                   ; return to caller
_memand endp


;_memand proc c uses esi edi
;    arg dest:dword, source:dword, sizeInDWORDs:dword

;    mov esi,source        ; Load the source pointer into esi
;    mov edi,dest          ; Load the dest pointer into edi
;    mov ecx,sizeInDWORDs  ; load the counter into ecx

;@@10:
;    mov eax,[esi]         ; move the value of the source into eax
;    and [edi],eax         ; AND the source value onto the dest value
;    add edi,4             ; inc the array to the next value
;    add esi,4             ; inc the array to the next value
;    dec ecx               ; dec the counter
;    jnz @@10              ; jump if not 0

;    ret                   ; return to caller
;_memand endp



; Original version

;_memand proc c uses esi edi
;    arg dest:dword, source:dword, sizeInDWORDs:dword

;    mov esi,source
;    mov edi,dest
;    mov ecx,sizeInDWORDs
;   cld 
;@@10:
;    lodsd
;    and eax,[edi]
;   stosd
;    loop @@10

;    ret
;_memand endp


;memor proc c uses ebx
;    arg dest:dword, source:dword, num_bytes:dword
;    mov edx,dest
;    mov ecx,source
;    mov eax,num_bytes
;    jmp short @@decr
;@@loop:
;    mov bl,[ecx+eax]
;    or [edx+eax],bl
;@@decr:
;    dec eax
;    jnz @@loop
;    mov bl,[ecx]
;    or [edx],bl
;    ret
;memor endp


end