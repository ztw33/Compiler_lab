.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
	li $v0, 4
	la $a0, _prompt
	syscall
	li $v0, 5
	syscall
	jr $ra

write:
	li $v0, 1
	syscall
	li $v0, 4
	la $a0, _ret
	syscall
	move $v0, $0
	jr $ra

main:
	li $t1, 0
	li $t1, 1
	li $t1, 0
	move $t1, $t1
label1:
	blt $t1, $t1, label2
	j label3
label2:
	add $t1, $t1, $t1
	move $t1, $t1
	move $t1, $t1
	addi $t1, $t1, 1
	j label1
label3:
	li $t1, 0
	move $v0, $t1
	jr $ra
