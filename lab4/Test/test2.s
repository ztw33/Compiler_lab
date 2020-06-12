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

fact:
	addi $sp, $sp, -4
	sw $a0, 0($sp)
	lw $t0, -12($s8)
	li $t1, 1
	beq $t0, $t1, label1
	j label2
label1:
	lw $v0, -12($s8)
	jr $ra
	j label3
label2:
	addi $sp, $sp, -4
	lw $t1, -12($s8)
	li $t2, 1
	sub $t0, $t1, $t2
	sw $t0, -16($s8)
	lw $a0, -16($s8)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	sw $s8, 0($sp)
	addi $s8, $sp, 8
	jal fact
	lw $ra, -4($s8)
	lw $s8, -8($s8)
	addi $sp, $s8, -16
	addi $sp, $sp, -4
	sw $v0, -20($s8)
	addi $sp, $sp, -4
	lw $t1, -12($s8)
	lw $t2, -20($s8)
	mul $t0, $t1, $t2
	sw $t0, -24($s8)
	lw $v0, -24($s8)
	jr $ra
label3:

main:
	move $s8, $sp
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal read
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	addi $sp, $sp, -4
	sw $v0, -4($s8)
	addi $sp, $sp, -4
	lw $t0, -4($s8)
	sw $t0, -8($s8)
	lw $t0, -8($s8)
	li $t1, 1
	bgt $t0, $t1, label4
	j label5
label4:
	lw $a0, -8($s8)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	addi $sp, $sp, -4
	sw $s8, 0($sp)
	addi $s8, $sp, 8
	jal fact
	lw $ra, -4($s8)
	lw $s8, -8($s8)
	addi $sp, $s8, -8
	addi $sp, $sp, -4
	sw $v0, -12($s8)
	addi $sp, $sp, -4
	lw $t0, -12($s8)
	sw $t0, -16($s8)
	j label6
label5:
	li $t0, 1
	sw $t0, -16($s8)
label6:
	lw $a0, -16($s8)
	addi $sp, $sp, -4
	sw $ra, 0($sp)
	jal write
	lw $ra, 0($sp)
	addi $sp, $sp, 4
	li $v0, 0
	jr $ra
