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
	move $fp, $sp
	addi $fp, $fp, -512
	li $t0, 8
	sw $t0, 4($fp)
	li $t0, 5
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	li $t0, 6
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	li $t0, 7
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 8($sp)
	lw $t1, 4($sp)
	add $t2, $t1, $t0
	addi $sp, $sp, -4
	sw $t2, 0($sp)
	lw $t0, 0($sp)
	lw $t1, 4($sp)
	add $t2, $t1, $t0
	addi $sp, $sp, -4
	sw $t2, 0($sp)
	lw $t0, 0($sp)
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	sw $ra, 0($fp)
	li $t0, 8
	sw $t0, 4($fp)
	add $fp, $fp, $t0
	lw $t0, 0($sp)
	move $a0, $t0
	jal write
	lw $t0, -4($fp)
	sub $fp, $fp, $t0
	li $t0, 0
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	lw $t0, 0($sp)
	move $v0, $t0
	jr $ra
