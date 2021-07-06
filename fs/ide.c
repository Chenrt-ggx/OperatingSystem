// operations on IDE disk.

#include "fs.h"
#include "lib.h"
#include <mmu.h>

/* Overview:
 *  read data from IDE disk. First issue a read request through
 *  disk register and then copy data from disk buffer
 *  (512 bytes, a sector) to destination array.
 *
 * Parameters:
 *	diskno: disk number.
 *  secno: start sector number.
 *  dst: destination for data read from IDE disk.
 *  nsecs: the number of sectors to read.
 *
 * Post-Condition:
 *  If error occurred during read the IDE disk, panic.
 *
 * Hint: use syscalls to access device registers and buffers
 */
void ide_read(u_int diskno, u_int secno, void *dst, u_int nsecs)
{
	int offset = 0, read = 0, f;
	int offset_begin = secno << 9;
	int offset_end = offset_begin + (nsecs << 9);

	for (; offset_begin + offset < offset_end; offset += 0x200)
	{
		// Write: Select the IDE ID to be used in the next read/write operation.
		if (syscall_write_dev((u_int)&diskno, 0x13000000 + 0x10, 4))
			user_panic("failed in read in step 1");
		// Write: Set the offset (in bytes) from the beginning of the disk image. This offset will be used for the next read/write operation.
		f = offset_begin + offset;
		if (syscall_write_dev((u_int)&f, 0x13000000 + 0x0, 4))
			user_panic("failed in read in step 2");
		// Write: Start a read or write operation. (Writing 0 means a Read operation, a 1 means a Write operation.)
		if (syscall_write_dev((u_int)&read, 0x13000000 + 0x20, 4))
			user_panic("failed in read in step 3");
		// Read: Get status of the last operation. (Status 0 means failure, non-zero means success.)
		if (syscall_read_dev((u_int)&f, 0x13000000 + 0x30, 4) || !f)
			user_panic("failed in read in step 4");
		// Read/Write: 512 bytes data buffer.
		if (syscall_read_dev((u_int)(dst + offset), 0x13000000 + 0x4000, 0x200))
			user_panic("failed in read in step 5");
	}
}

/* Overview:
 *  write data to IDE disk.
 *
 * Parameters:
 *	diskno: disk number.
 *	secno: start sector number.
 *  src: the source data to write into IDE disk.
 *	nsecs: the number of sectors to write.
 *
 * Post-Condition:
 *	If error occurred during read the IDE disk, panic.
 *
 * Hint: use syscalls to access device registers and buffers
 */
void ide_write(u_int diskno, u_int secno, void *src, u_int nsecs)
{
	int offset = 0, write = 1, f;
	int offset_begin = secno << 9;
	int offset_end = offset_begin + (nsecs << 9);

	// DO NOT DELETE WRITEF !!!
	writef("diskno: %d\n", diskno);

	for (; offset_begin + offset < offset_end; offset += 0x200)
	{
		// Write: Select the IDE ID to be used in the next read/write operation.
		if (syscall_write_dev((u_int)&diskno, 0x13000000 + 0x10, 4))
			user_panic("failed in write in step 1");
		// Write: Set the offset (in bytes) from the beginning of the disk image. This offset will be used for the next read/write operation.
		f = offset_begin + offset;
		if (syscall_write_dev((u_int)&f, 0x13000000 + 0x0, 4))
			user_panic("failed in write in step 2");
		// Read/Write: 512 bytes data buffer.
		if (syscall_write_dev((u_int)(src + offset), 0x13000000 + 0x4000, 0x200))
			user_panic("failed in write in step 3");
		// Write: Start a read or write operation. (Writing 0 means a Read operation, a 1 means a Write operation.)
		if (syscall_write_dev((u_int)&write, 0x13000000 + 0x20, 4))
			user_panic("failed in write in step 4");
		// Read: Get status of the last operation. (Status 0 means failure, non-zero means success.)
		if (syscall_read_dev((u_int)&f, 0x13000000 + 0x30, 4) || !f)
			user_panic("failed in write in step 5");
	}
}
