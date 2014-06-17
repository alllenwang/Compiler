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
	lw $t0, 4($sp)
	li $t1, 1
	beq $t0, $t1, label1
	j label2
label1 :
	lw $t0, 4($sp)
	sw $t0, -4($fp)
	lw $t0, -4($fp)
	move $v0, $t0
	jr $ra
	j label3
label2 :
	lw $t0, 4($sp)
	li $t1, 1
	sub $t2, $t0, $t1
	sw $t2, -8($fp)
	sw $ra, 0($sp)
	addi $sp, $sp, 12
	lw $t0, -8($fp)
	sw $t0, 4($sp)
	li $t0, 12
	sw $t0, 8($sp)
	jal fact
	lw $t0, -4($sp)
	sub $sp, $sp, $t0
	lw $ra, 0($sp)
	move $t0, $v0
	sw $t0, -12($fp)
	lw $t0, 4($sp)
	lw $t1, -12($fp)
	mul $t2, $t1, $t0
	sw $t2, -16($fp)
	lw $t0, -16($fp)
	move $v0, $t0
	jr $ra
label3 :
main:
	move $fp, $sp
	addi $sp, $sp, -2048
	li $t0, 8
	sw $t0, 4($sp)
	sw $ra, 0($sp)
	addi $sp, $sp, 4
	jal read
	sub $sp, $sp, 4
	lw $ra, 0($sp)
	move $t0, $v0
	sw $t0, -20($fp)
	lw $t0, -20($fp)
	li $t1, 1
	bgt $t0, $t1, label4
	j label5
label4 :
	lw $t0, -20($fp)
	sw $t0, -24($fp)
	sw $ra, 0($sp)
	addi $sp, $sp, 8
	lw $t0, -24($fp)
	sw $t0, 4($sp)
	li $t0, 12
	sw $t0, 8($sp)
	jal fact
	lw $t0, -4($sp)
	sub $sp, $sp, $t0
	lw $ra, 0($sp)
	move $t0, $v0
	sw $t0, -28($fp)
	j label6
label5 :
	li $t0, 1
	sw $t0, -28($fp)
label6 :
	lw $t0, -28($fp)
	sw $t0, -32($fp)
	sw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $t0, -32($fp)
	move $a0, $t0
	jal write
	addi $sp, $sp, -4
	lw $ra, 0($sp)
	li $t0, 0
	sw $t0, -36($fp)
	lw $t0, -36($fp)
	move $v0, $t0
	jr $ra
