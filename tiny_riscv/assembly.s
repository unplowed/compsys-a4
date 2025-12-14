        .globl _start
_start:
	addi	a0,x0,100
	ori	a7,x0,3
	ecall
	addi	a4,a5,0
	addi	a5,a5,2047
	addi	a5,a5,-2048
	addi	a5,a5,0
	addi	a5,a5,1
	addi	a5,a5,-1

  sw a0, 0(x0)

  bne a0, a1, 0
  bne a0, a1, 2
  bne a0, a1, 8196
  bne a0, a1, -8196
  bne a0, a1, -2
