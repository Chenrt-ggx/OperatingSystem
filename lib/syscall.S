#include <unistd.h>
#include <asm/asm.h>
#include <stackframe.h>
#include <asm/regdef.h>
#include <asm/cp0regdef.h>

/* exercise 4.2 */
NESTED(handle_sys, TF_SIZE, sp)
	// Macro used to save trapframe
	.set noat
	SAVE_ALL
	// Clean Interrupt Mask
	CLI
	nop

	// Resume use of $at
	.set at
	// TODO: Fetch EPC from Trapframe, calculate a proper value and store it back to trapframe.
	lw t0, TF_EPC(sp)
	addiu t0, t0, 4
	sw t0, TF_EPC(sp)
	// TODO: Copy the syscall number into $a0.
	lw a0, TF_REG4(sp)

	addiu a0, a0, -__SYSCALL_BASE // a0 <- relative syscall number
	sll t0, a0, 2				  // t0 <- relative syscall number times 4
	la t1, sys_call_table		  // t1 <- syscall table base
	addu t1, t1, t0				  // t1 <- table entry of specific syscall
	lw t2, 0(t1)				  // t2 <- function entry of specific syscall
	lw t0, TF_REG29(sp)			  // t0 <- user stack pointer
	lw t3, 16(t0)				  // t3 <- the 5th argument of msyscall
	lw t4, 20(t0)				  // t4 <- the 6th argument of msyscall

	// TODO: Allocate a space of six arguments on current kernel stack and copy the six arguments to proper location
	lw a0, TF_REG4(sp)
	lw a1, TF_REG5(sp)
	lw a2, TF_REG6(sp)
	lw a3, TF_REG7(sp)
	sw t4, -4(sp)
	sw t3, -8(sp)
	sw a3, -12(sp)
	sw a2, -16(sp)
	sw a1, -20(sp)
	sw a0, -24(sp)
	addiu sp, sp, -24

	// Invoke sys_* function
	jalr t2
	nop
	// TODO: Resume current kernel stack
	addiu sp, sp, 24
	// Store return value of function sys_* (in $v0) into trapframe
	sw v0, TF_REG2(sp)
	// Return from exeception
	j ret_from_exception
	nop
END(handle_sys)

// Syscall Table
sys_call_table:
.align 2
	.word sys_putchar
	.word sys_getenvid
	.word sys_yield
	.word sys_env_destroy
	.word sys_set_pgfault_handler
	.word sys_mem_alloc
	.word sys_mem_map
	.word sys_mem_unmap
	.word sys_env_alloc
	.word sys_set_env_status
	.word sys_set_trapframe
	.word sys_panic
	.word sys_ipc_can_send
	.word sys_ipc_recv
	.word sys_cgetc
	.word sys_write_dev
	.word sys_read_dev
	.word sys_puts
