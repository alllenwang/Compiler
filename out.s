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
	addi $sp, $sp, -2048
	li $t0, 8
	sw $t0, 4($sp)
	li $t0, 1
	sw $t0, 4($fp)
	li $t0, 2
	sw $t0, 8($fp)
	li $t0, 3
	sw $t0, 12($fp)
	lw $t0, 4($fp)
	lw $t1, 8($fp)
	add $t2, $t1, $t0
	sw $t2, 16($fp)
	lw $t0, 16($fp)
	lw $t1, 12($fp)
	add $t2, $t1, $t0
	sw $t2, 20($fp)
	lw $t0, 20($fp)
	sw $t0, 24($fp)
	sw $ra, 0($sp)
	addi $sp, $sp, 4
	lw $t0, 24($fp)
	move $a0, $t0
	jal write
	addi $sp, $sp, -4
	lw $ra, 0($sp)
	li $t0, 0
	sw $t0, 28($fp)
	lw $t0, 28($fp)
	move $v0, $t0
	jr $ra
