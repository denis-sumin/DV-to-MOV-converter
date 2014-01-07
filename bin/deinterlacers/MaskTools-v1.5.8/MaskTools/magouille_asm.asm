BITS 32

;=============================================================================
; Macros and other preprocessor constants
;=============================================================================

%macro cglobal 1
    %ifdef PREFIX
        global _%1
        %define %1 _%1
    %else
        global %1
    %endif
%endmacro

;=============================================================================
; Read only data
;=============================================================================

SECTION .rodata data align=16

ALIGN 16
mmx_dw_one:
    times 4 dw 16

;=============================================================================
; Macros
;=============================================================================


;=============================================================================
; Code
;=============================================================================

SECTION .text

cglobal magouille_isse
cglobal magouille_sse2

;
; void magouille_isse(BYTE *pDst, const BYTE *pSrc
;                     int nDstPitch, int nSrcPitch,
;                     int nThreshold)
;

;
; void magouille_sse2(BYTE *pDst, const BYTE *pSrc
;                     int nDstPitch, int nSrcPitch,
;                     int nThreshold)
;


%assign pdst	20
%assign psrc	24
%assign dstp	28
%assign srcp	32
%assign thrd	36

%assign bufsize 24*24*2
%assign halfbuf 24*24

%define	min		cmovg
%define max		cmovl

; %1 : pdst
; %2 : psrc ( offset (-1, -1) relatively to pdst )
; %3 : dstp
; %4 : srcp
; %5 : min or max operand
%macro PIXEL 5
	mov		bl, [%2 + 1]
	mov		al, [%2 + %4]
	cmp		bl, al
	%5		ebx, eax
	mov		al, [%2 + %4 + 1]
	cmp		bl, al
	%5		ebx, eax
	mov		al, [%2 + %4 + 2]
	cmp		bl, al
	%5		ebx, eax
	mov		al, [%2 + 2 * %4 + 1]
	cmp		bl, al
	%5		ebx, eax
	mov		[%1], bl
%endmacro

; %1 : pdst
; %2 : psrc  ( offset (-1, -1) relatively to pdst )
; %3 : dstp
; %4 : srcp
; %5 : min or max operand
; mm0 - mm4 used
%macro PIXEL_8 5
	movq	mm0, [%2 + 1]
	movq	mm1, [%2 + %4]
	movq	mm2, [%2 + %4 + 1]
	movq	mm3, [%2 + %4 + 2]
	%5		mm0, mm1
	movq	mm4, [%2 + 2 * %4 + 1]
	%5		mm2, mm3
	%5		mm0, mm4
	%5		mm0, mm2
	movq	[%1], mm0
%endmacro

; %1 : pdst
; %2 : psrc  ( offset (-1, -1) relatively to pdst )
; %3 : dstp
; %4 : srcp
; %5 : min or max operand
; xmm0 - xmm4 used
%macro PIXEL_16 5
	movupd	xmm0, [%2 + 1]
	movupd	xmm1, [%2 + %4]
	movupd	xmm2, [%2 + %4 + 1]
	movupd	xmm3, [%2 + %4 + 2]
	%5		xmm0, xmm1
	movupd	xmm4, [%2 + 2 * %4 + 1]
	%5		xmm2, xmm3
	%5		xmm0, xmm4
	%5		xmm0, xmm2
	movupd	[%1], xmm0
%endmacro
	
ALIGN 16
magouille_sse2 :

	push	edi
	push	esi
	push	ebx
	push	ebp
	
	sub		esp, bufsize
	
	xor		eax, eax
	xor		ebx, ebx
	
	; first stage : erosion
	
	mov		ecx, [esp + bufsize + srcp]		; in_pitch
	mov		esi, [esp + bufsize + psrc]		; in
	sub		esi, 4							; in
	sub		esi, ecx						; in
	sub		esi, ecx						; in
	sub		esi, ecx						; in
	sub		esi, ecx						; in
	mov		edx, 24							; out_pitch
	mov		edi, esp						; out
	lea		edi, [edi + edx + 1]			; out
	
	mov		ebp, 22							; 22 rows to process
	
sse2_loop_stage_1:

	PIXEL_16	edi +  0, esi +  0, edx, ecx, pminub
	PIXEL_8		edi + 14, esi + 14, edx, ecx, pminub
	
	add		esi, ecx
	add		edi, edx
	
	dec		ebp
	jnz		sse2_loop_stage_1
	
	; second stage : dilatation
	
	mov		ecx, 24								; in_pitch
	mov		esi, esp							; in
	lea		esi, [esi + 1 + ecx]				; in
	mov		edx, 24								; out_pitch
	mov		edi, esp 							; out
	lea		edi, [edi + 2 * edx + 2 + halfbuf]	; out
	
	mov		ebp, 20								; 20 rows to process
	
sse2_loop_stage_2:

	PIXEL_16	edi +  0, esi +  0, edx, ecx, pmaxub
	PIXEL_8		edi + 12, esi + 12, edx, ecx, pmaxub
	
	add		esi, ecx
	add		edi, edx
	
	dec		ebp
	jnz		sse2_loop_stage_2
	
	; third stage : dilatation
	
	mov		ecx, 24								; in_pitch
	mov		esi, esp							; in
	lea		esi, [esi + 2 + 2 * ecx + halfbuf]	; in
	mov		edx, 24								; out_pitch
	mov		edi, esp 							; out
	lea		edi, [edi + 2 * edx + 3]			; out
	add		edi, edx							; out
	
	mov		ebp, 18								; 18 rows to process
	
sse2_loop_stage_3:

	PIXEL_16	edi +  0, esi +  0, edx, ecx, pmaxub
	PIXEL_8		edi + 10, esi + 10, edx, ecx, pmaxub
	
	add		esi, ecx
	add		edi, edx
	
	dec		ebp
	jnz		sse2_loop_stage_3
	
	; fourth stage : erosion
	
	mov		ecx, 24								; in_pitch
	mov		esi, esp							; in
	lea		esi, [esi + 3 + 2 * ecx]			; in
	add		esi, ecx
	mov		edx, [esp + bufsize + dstp]			; out_pitch
	mov		edi, [esp + bufsize + pdst]			; out
	
	mov		ebp, 16								; 16 rows to process
	
sse2_loop_stage_4:

	PIXEL_16	edi, esi, edx, ecx, pminub
	
	add		esi, ecx
	add		edi, edx
	
	dec		ebp
	jnz		sse2_loop_stage_4
	
	add		esp, bufsize
	
	pop		ebp
	pop		ebx
	pop		esi
	pop		edi

	ret
	
	
ALIGN 16
magouille_isse :

	push	edi
	push	esi
	push	ebx
	push	ebp
	
	sub		esp, bufsize
	
	xor		eax, eax
	xor		ebx, ebx
	
	; first stage : erosion
	
	mov		ecx, [esp + bufsize + srcp]		; in_pitch
	mov		esi, [esp + bufsize + psrc]		; in
	sub		esi, 4							; in
	sub		esi, ecx						; in
	sub		esi, ecx						; in
	sub		esi, ecx						; in
	sub		esi, ecx						; in
	mov		edx, 24							; out_pitch
	mov		edi, esp						; out
	lea		edi, [edi + edx + 1]			; out
	
	mov		ebp, 22							; 22 rows to process
	
isse_loop_stage_1:

	PIXEL_8		edi +  0, esi +  0, edx, ecx, pminub
	PIXEL_8		edi +  8, esi +  8, edx, ecx, pminub
	PIXEL_8		edi + 14, esi + 14, edx, ecx, pminub
	
	add		esi, ecx
	add		edi, edx
	
	dec		ebp
	jnz		isse_loop_stage_1
	
	; second stage : dilatation
	
	mov		ecx, 24								; in_pitch
	mov		esi, esp							; in
	lea		esi, [esi + 1 + ecx]				; in
	mov		edx, 24								; out_pitch
	mov		edi, esp 							; out
	lea		edi, [edi + 2 * edx + 2 + halfbuf]	; out
	
	mov		ebp, 20								; 20 rows to process
	
isse_loop_stage_2:

	PIXEL_8		edi +  0, esi +  0, edx, ecx, pmaxub
	PIXEL_8		edi +  8, esi +  8, edx, ecx, pmaxub
	PIXEL_8		edi + 12, esi + 12, edx, ecx, pmaxub
	
	add		esi, ecx
	add		edi, edx
	
	dec		ebp
	jnz		isse_loop_stage_2
	
	; third stage : dilatation
	
	mov		ecx, 24								; in_pitch
	mov		esi, esp							; in
	lea		esi, [esi + 2 + 2 * ecx + halfbuf]	; in
	mov		edx, 24								; out_pitch
	mov		edi, esp 							; out
	lea		edi, [edi + 2 * edx + 3]			; out
	add		edi, edx							; out
	
	mov		ebp, 18								; 18 rows to process
	
isse_loop_stage_3:

	PIXEL_8		edi +  0, esi +  0, edx, ecx, pmaxub
	PIXEL_8		edi +  8, esi +  8, edx, ecx, pmaxub
	PIXEL_8		edi + 10, esi + 10, edx, ecx, pmaxub
	
	add		esi, ecx
	add		edi, edx
	
	dec		ebp
	jnz		isse_loop_stage_3
	
	; fourth stage : erosion
	
	mov		ecx, 24								; in_pitch
	mov		esi, esp							; in
	lea		esi, [esi + 3 + 2 * ecx]			; in
	add		esi, ecx
	mov		edx, [esp + bufsize + dstp]			; out_pitch
	mov		edi, [esp + bufsize + pdst]			; out
	
	mov		ebp, 16								; 16 rows to process
	
isse_loop_stage_4:

	PIXEL_8		edi +  0, esi +  0, edx, ecx, pminub
	PIXEL_8		edi +  8, esi +  8, edx, ecx, pminub
	
	add		esi, ecx
	add		edi, edx
	
	dec		ebp
	jnz		isse_loop_stage_4
	
	add		esp, bufsize
	
	pop		ebp
	pop		ebx
	pop		esi
	pop		edi

	ret
	