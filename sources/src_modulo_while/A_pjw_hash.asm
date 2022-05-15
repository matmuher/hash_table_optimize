;	[PJW_HASH]

global A_pjw_hash

section .text

A_pjw_hash:

		xor rax, rax ; int hash = 0
		xor ecx, ecx

	.for:
		movzx r10, byte [rdi + rcx]
		shl eax, 4
		add rax, r10

		mov r9d, eax	
		and r9d, 0xf0000000	; g = hash & 0xf0000000;

		mov r8d, eax

		mov edx, r9d
		shr edx, 24	; (g >> 24)
		
		xor r8d, edx	; hash = hash ^ (g >> 24)
		xor r8d, r9d	; hash = hash ^ g

		test r9d, r9d ; (g != 0)
		cmovnz eax, r8d

		add ecx, 1
		cmp ecx, esi 
		jb .for

		ret
