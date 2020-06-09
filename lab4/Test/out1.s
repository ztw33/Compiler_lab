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
	addi $sp, $sp, -4
	li $t0, 0
	sw $t0, -4($fp)
	addi $sp, $sp, -4
	li $t0, 1
	sw $t0, -8($fp)
	addi $sp, $sp, -4
	li $t0, 0
	sw $t0, -12($fp)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal read
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	addi $sp, $sp, -4
	sw $v0, -16($fp)
	addi $sp, $sp, -4
	lw $t1, -16($fp)
	move $t0, $t1
	sw $t0, -20($fp)
label1:
	lw $t0, -12($fp)
	lw $t1, -20($fp)
	blt $t0, $t1, label2
	j label3
label2:
	addi $sp, $sp, -4
	lw $t1, -4($fp)
	lw $t2, -8($fp)
	add $t0, $t1, $t2
	sw $t0, -24($fp)
	lw $t1, -8($fp)
	move $t0, $t1
	sw $t0, -4($fp)
	lw $t1, -24($fp)
	move $t0, $t1
	sw $t0, -8($fp)
	lw $t1, -12($fp)
	addi $t0, $t1, 1
	sw $t0, -12($fp)
	j label1
label3:
	li $v0, 0
	jr $ra
