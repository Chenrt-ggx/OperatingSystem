#include "lib.h"
#include <mmu.h>
#include <env.h>
#include <kerelf.h>

#define debug 0
#define TMPPAGE (BY2PG)
#define TMPPAGETOP (TMPPAGE + BY2PG)

int init_stack(u_int child, char **argv, u_int *init_esp)
{
	int argc, i, r, tot;
	char *strings;
	u_int *args;

	// Count the number of arguments (argc)
	// and the total amount of space needed for strings (tot)
	tot = 0;
	for (argc = 0; argv[argc]; argc++)
		tot += strlen(argv[argc]) + 1;

	// Make sure everything will fit in the initial stack page
	if (ROUND(tot, 4) + 4 * (argc + 3) > BY2PG)
		return -E_NO_MEM;

	// Determine where to place the strings and the args array
	strings = (char *)TMPPAGETOP - tot;
	args = (u_int *)(TMPPAGETOP - ROUND(tot, 4) - 4 * (argc + 1));
	if ((r = syscall_mem_alloc(0, TMPPAGE, PTE_V | PTE_R)) < 0)
		return r;

	// Replace this with your code to:
	//   copy the argument strings into the stack page at 'strings'

	u_int j;
	char *ctemp, *argv_temp;
	ctemp = strings;
	for (i = 0; i < argc; i++)
	{
		argv_temp = argv[i];
		for (j = 0; j < strlen(argv[i]); j++)
		{
			*ctemp = *argv_temp;
			ctemp++, argv_temp++;
		}
		*ctemp = 0;
		ctemp++;
	}

	// initialize args[0..argc-1] to be pointers to these strings
	// that will be valid addresses for the child environment
	// (for whom this page will be at USTACKTOP-BY2PG!).
	ctemp = (char *)(USTACKTOP - TMPPAGETOP + (u_int)strings);
	for (i = 0; i < argc; i++)
	{
		args[i] = (u_int)ctemp;
		ctemp += strlen(argv[i]) + 1;
	}

	// set args[argc] to 0 to null-terminate the args array.
	args[argc] = (u_int)--ctemp;

	// push two more words onto the child's stack below 'args',
	// containing the argc and argv parameters to be passed
	// to the child's umain() function.
	u_int *pargv_ptr;
	pargv_ptr = args - 1;
	*pargv_ptr = USTACKTOP - TMPPAGETOP + (u_int)args;
	pargv_ptr--;
	*pargv_ptr = argc;

	// set *init_esp to the initial stack pointer for the child
	*init_esp = USTACKTOP - TMPPAGETOP + (u_int)pargv_ptr;
	// *init_esp = USTACKTOP; // Change this!
	if ((r = syscall_mem_map(0, TMPPAGE, child, USTACKTOP - BY2PG, PTE_V | PTE_R)) < 0)
		goto error;
	if ((r = syscall_mem_unmap(0, TMPPAGE)) < 0)
		goto error;
	return 0;
error:
	syscall_mem_unmap(0, TMPPAGE);
	return r;
}

int usr_is_elf_format(u_char *binary)
{
	Elf32_Ehdr *ehdr = (Elf32_Ehdr *)binary;
	if (ehdr->e_ident[0] == ELFMAG0 &&
		ehdr->e_ident[1] == ELFMAG1 &&
		ehdr->e_ident[2] == ELFMAG2 &&
		ehdr->e_ident[3] == ELFMAG3)
		return 1;
	return 0;
}

int usr_load_elf(int fd, Elf32_Phdr *ph, int child_envid)
{
	// Hint: maybe this function is useful
	//  If you want to use this func, you should fill it ,it's not hard
	u_char *bin;
	int r, i = 0;
	u_int32_t sgsize = ph->p_memsz, bin_size = ph->p_filesz;
	u_long va = ph->p_vaddr, va_align = ROUNDDOWN(va, BY2PG), offset = va - va_align;
	if ((r = read_map(fd, ph->p_offset, (void **)&bin)) < 0)
		return r;

	if (offset > 0)
	{
		if ((r = syscall_mem_alloc(child_envid, va_align, PTE_V | PTE_R)) < 0)
			return r;
		if ((r = syscall_mem_map(child_envid, va_align, 0, USTACKTOP, PTE_V | PTE_R)) < 0)
			return r;
		user_bcopy((void *)bin, (void *)(USTACKTOP + offset), MIN(bin_size, i = (BY2PG - offset)));
		if ((r = syscall_mem_unmap(0, USTACKTOP)) < 0)
			return r;
	}

	for (; i < bin_size; i += BY2PG)
	{
		if ((r = syscall_mem_alloc(child_envid, va + i, PTE_V | PTE_R)) < 0)
			return r;
		if ((r = syscall_mem_map(child_envid, va + i, 0, USTACKTOP, PTE_V | PTE_R)) < 0)
			return r;
		user_bcopy((void *)(bin + i), (void *)(USTACKTOP + offset), MIN(bin_size - i, BY2PG));
		if ((r = syscall_mem_unmap(0, USTACKTOP)) < 0)
			return r;
	}

	for (; i < sgsize; i += BY2PG)
		if ((r = syscall_mem_alloc(child_envid, va + i, PTE_V | PTE_R)) < 0)
			return r;
	return 0;
}

int spawn(char *prog, char **argv)
{
	int r, fd, size;
	Elf32_Phdr *phdr;
	u_char *ptr_ph_table = NULL;
	u_int esp, child_envid, *blk;
	Elf32_Half ph_entry_size, ph_entry_count;

	// Note 0: some variable may be not used,you can cancel them as you like
	// Step 1: Open the file specified by `prog` (prog is the path of the program)
	if ((fd = open(prog, O_RDONLY)) < 0)
	{
		user_panic("spawn ::open line 102 RDONLY wrong !\n");
		return fd;
	}

	/*
	 * Before Step 2 , You had better check the "target" spawned is a execute bin
	 * Step 2: Allocate an env (Hint: using syscall_env_alloc())
	 */
	if ((r = read_map(fd, 0, (void **)&blk)) < 0)
		return r;
	size = ((struct Filefd *)num2fd(fd))->f_file.f_size;
	if (size < 4 || !usr_is_elf_format((u_char *)blk))
		return -1;
	if ((child_envid = syscall_env_alloc()) < 0)
		return child_envid;

	/*
	 * Step 3: Using init_stack(...) to initialize the stack of the allocated env
	 * Step 3: Map file's content to new env's text segment
	 *        Hint 1: what is the offset of the text segment in file? try to use objdump to find out.
	 *        Hint 2: using read_map(...)
	 *        Hint 3: Important!!! sometimes, its not safe to use read_map ,guess why
	 *            If you understand, you can achieve the "load APP" with any method
	 * Note1: Step 1 and 2 need sanity check. In other words, you should check whether
	 *       the file is opened successfully, and env is allocated successfully.
	 * Note2: You can achieve this func in any way ，remember to ensure the correctness
	 *        Maybe you can review lab3
	 */
	if ((r = init_stack(child_envid, argv, &esp)) < 0)
		return r;
	ptr_ph_table = (u_char *)blk + ((Elf32_Ehdr *)blk)->e_phoff;
	ph_entry_count = ((Elf32_Ehdr *)blk)->e_phnum;
	ph_entry_size = ((Elf32_Ehdr *)blk)->e_phentsize;
	while (ph_entry_count--)
	{
		phdr = (Elf32_Phdr *)ptr_ph_table;
		if (phdr->p_type == PT_LOAD)
		{
			/* Your task here! */
			/* Real map all section at correct virtual address.Return < 0 if error. */
			/* Hint: Call the callback function you have achieved before. */
			if ((r = usr_load_elf(fd, phdr, (int)child_envid)) < 0)
				return r;
		}
		ptr_ph_table += ph_entry_size;
	}

	struct Trapframe *tf;
	writef("\n::::::::::spawn size : %x  sp : %x::::::::\n", size, esp);
	tf = &(envs[ENVX(child_envid)].env_tf);
	tf->regs[29] = esp;
	tf->pc = UTEXT;

	// Share memory
	u_int pdeno = 0, pteno = 0, pn = 0, va = 0;
	for (pdeno = 0; pdeno < PDX(UTOP); pdeno++)
	{
		if (!((*vpd)[pdeno] & PTE_V))
			continue;
		for (pteno = 0; pteno <= PTX(~0); pteno++)
		{
			pn = (pdeno << 10) + pteno;
			if (((*vpt)[pn] & PTE_V) && ((*vpt)[pn] & PTE_LIBRARY))
			{
				va = pn * BY2PG;
				if ((r = syscall_mem_map(0, va, child_envid, va, (PTE_V | PTE_R | PTE_LIBRARY))) < 0)
				{
					writef("va: %x   child_envid: %x   \n", va, child_envid);
					user_panic("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
					return r;
				}
			}
		}
	}
	if ((r = syscall_set_env_status(child_envid, ENV_RUNNABLE)) < 0)
	{
		writef("set child runnable is wrong\n");
		return r;
	}
	return child_envid;
}

int spawnl(char *prog, char *args, ...)
{
	return spawn(prog, &args);
}
