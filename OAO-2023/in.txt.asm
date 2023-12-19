;========================= шапка ==============================
.586
	.model flat, stdcall
	includelib libucrt.lib
	includelib kernel32.lib
	ExitProcess PROTO : DWORD

	EXTRN outUs : proc
	EXTRN outStr : proc
	EXTRN outSymb : proc
	EXTRN outUsNewLine : proc
	EXTRN outStrNewLine : proc
	EXTRN outSymbNewLine : proc
	EXTRN DATE : proc 
	EXTRN RANDOM : proc

.stack 4096
;=================== сегмент констант =========================
.const
	L0 DWORD 10
	L1 DWORD 'a'
	L2 DWORD 8
	L3 DWORD 1
	L4 DWORD 2
	L5 DWORD 5
;=================== сегмент данных ============================
.data
	shiftleftans3 DWORD ?
	shiftrightans7 DWORD ?
	HEADa8 DWORD ?
	HEADb10 DWORD ?
	HEADc11 DWORD ?
	HEADd13 DWORD ?
	HEADs15 DWORD ?
	HEADx17 DWORD ?
	HEADi21 DWORD ?
;=================== сегмент кода ==============================

.code

shiftleft0 PROC shiftleftsrc1 : SDWORD, shiftleftpos2 : SDWORD
	push shiftleftsrc1
	push shiftleftpos2
	pop ebx
	pop eax
	cmp ebx, 8
	jg NOK0
	mov cl, bl
	shl eax, cl
	cmp eax, 256
	jl OK0
NOK0:
	push 9
	push 4
	push eax
	call outUsNewLine
	push 0
	call ExitProcess
OK0:
	push eax
	pop shiftleftans3
	mov eax, shiftleftans3
	ret
shiftleft0 ENDP

shiftright4 PROC shiftrightsrc5 : SDWORD, shiftrightpos6 : SDWORD
	push shiftrightsrc5
	push shiftrightpos6
	pop ebx
	pop eax
	cmp ebx, 8
	jg NOK1
	mov cl, bl
	shr eax, cl
	jmp OK1
NOK1:
	xor eax, eax
OK1:
	push eax
	pop shiftrightans7
	mov eax, shiftrightans7
	ret
shiftright4 ENDP

main PROC
	push L0
	pop HEADa8
	push HEADa8
		call outUsNewLine
	push L0
	pop HEADb10
	push HEADb10
		call outUsNewLine
	push L1
	pop HEADc11
	push HEADc11
		call outSymbNewLine
	pop edx
	push L1
		call RANDOM
	push eax
	pop HEADd13
	push HEADd13
		call outUsNewLine
	pop edx
	push L0
		call DATE
	push eax
	pop HEADs15
	push  HEADs15
		call outStrNewLine
	push L2
	pop HEADx17
	push HEADx17
	push L3
	pop edx
	pop edx
	push L3
	push HEADx17
		call shiftleft0
	push eax
	pop HEADx17
	push HEADx17
		call outUsNewLine
	push HEADx17
	push L4
	pop edx
	pop edx
	push L4
	push HEADx17
		call shiftright4
	push eax
	pop HEADx17
	push HEADx17
		call outUsNewLine
	push L3
	pop HEADi21
;--------------------------блок цикла------------------------------
CYCLE_START0:
	mov eax, HEADi21
	cmp eax, L5
	jge CYCLE_END0
	push HEADi21
	push L3
	pop ebx
	pop eax
	cmp ebx, 8
	jg NOK2
	mov cl, bl
	shl eax, cl
	cmp eax, 256
	jl OK2
NOK2:
	push 7
	push 39
	push eax
	call outUsNewLine
	jmp ERROR
OK2:
	push eax
	pop HEADi21
	push HEADi21
		call outUs
jmp CYCLE_START0
CYCLE_END0:
;----------------------------------------------------------------------
ERROR:
	push 0
	call ExitProcess
main ENDP
end main
;=============================================================
