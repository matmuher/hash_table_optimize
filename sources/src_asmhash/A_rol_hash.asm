;	[ROL_HASH]

global A_rol_hash

section .text

A_rol_hash:
		xor rax, rax ; int hash = 0
		lea rsi, [rdi + 1]	; str -> rsi
		movzx rdi, byte [rdi]
	.for:
		lodsb
		test al, al
		jz .end
		rol edi, 1
		xor edi, eax  
		jmp .for
	.end:
		xchg eax, edi

		ret
