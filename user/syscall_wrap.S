#include <asm/asm.h>
#include <asm/regdef.h>
#include <asm/cp0regdef.h>

/* exercise 4.1 */
LEAF(msyscall)
	// TODO: you JUST need to execute a `syscall` instruction and return from msyscall
	syscall
	jr ra
	nop
END(msyscall)
