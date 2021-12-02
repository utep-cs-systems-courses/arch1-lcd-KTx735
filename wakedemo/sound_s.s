	.arch msp430g2553
	.p2align 1,0
	.text

	.extern cycle
	.extern state
	.global sound
jt:
	.word case2		;|case0  | JT[3]
	.word case1		;|case1  | JT[2]
	.word case0		;|case2  | JT[1]
	.word default		;|default| JT[0]
song:
	mov &state, r12		; Note: action source, direction
	cmp #4, r12		; state < 4 | state >= 4
	jnc default

	add r12, r12		;2*state
	mov jt(r12), r0		;jump jt[state]
case0:
	mov #5000, &cycle	; cycle = 5000;
	mov #1, &state		; state = 1;
	jmp end
case1:
	mov #4000, &cycle	; cycle = 4000;
	mov #2, &state		; state = 2;
	jmp end
case2:
	mov #2000, &cycle	; cycle = 2000;
	mov #0, &state		; state = 0;
	jmp end
default:
	mov #1000, &cycle	; cycle = 1000;
	mov #0, &state		; state = 0;
	jmp end
end:
	mov &cycle, r12
	call #buzzer_set_period
	pop r0
