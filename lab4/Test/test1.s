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
	move $s8, $sp
	addi $sp, $sp, -4
	li $t0, 0
	sw $t0, -4($s8)
	addi $sp, $sp, -4
	li $t0, 1
	sw $t0, -8($s8)
	addi $sp, $sp, -4
	li $t0, 0
	sw $t0, -12($s8)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal read
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	addi $sp, $sp, -4
	sw $v0, -16($s8)
	addi $sp, $sp, -4
	lw $t0, -16($s8)
	sw $t0, -20($s8)
label1:
	lw $t0, -12($s8)
	lw $t1, -20($s8)
	blt $t0, $t1, label2
	j label3
label2:
	addi $sp, $sp, -4
	lw $t1, -4($s8)
	lw $t2, -8($s8)
	add $t0, $t1, $t2
	sw $t0, -24($s8)
	lw $a0, -8($s8)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $t0, -8($s8)
	sw $t0, -4($s8)
	lw $t0, -24($s8)
	sw $t0, -8($s8)
	lw $t1, -12($s8)
	li $t2, 1
	add $t0, $t1, $t2
	sw $t0, -12($s8)
	j label1
label3:
	li $v0, 0
	jr $ra
