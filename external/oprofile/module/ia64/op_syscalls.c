/**
 * @file op_syscalls.c
 * Tracing of system calls
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author Bob Montgomery
 * @author Will Cohen
 * @author John Levon
 * @author Philippe Elie
 */

#include <linux/sched.h>
#include <linux/unistd.h>
#include <linux/mman.h>
#include <linux/file.h>

#include "oprofile.h"
#include "op_dcache.h"
#include "op_util.h"

uint dname_top;
struct qstr **dname_stack;
char * pool_pos;
char * pool_start;
char * pool_end;
 
void oprof_put_note(struct op_note * samp);


struct mmap_arg_struct {
	unsigned long addr;
        unsigned long len;
        unsigned long prot;
        unsigned long flags;
        unsigned long fd;
        unsigned long offset;
};

asmlinkage static int (*old_sys_clone)(long, long);
asmlinkage static int (*old_sys_clone2)(long, long, long);
asmlinkage static int (*old_sys_execve)(char *, char **, char **);
asmlinkage static unsigned long (*old_sys_mmap)(unsigned long,
				unsigned long, int, int, int, long);
asmlinkage static unsigned long (*old_sys_mmap2)(unsigned long,
				unsigned long, int, int, int, long);
asmlinkage static long (*old_sys_init_module)(char const *, struct module *);
asmlinkage static long (*old_sys_exit)(int);

asmlinkage long post_stub_clone(long, long);
asmlinkage long post_stub_clone2(long, long, long);
asmlinkage long my_ia64_execve(char *, char **, char **);
asmlinkage unsigned long post_stub_mmap(unsigned long,
					unsigned long, int, int, int, long);
asmlinkage unsigned long post_stub_mmap2(unsigned long,
					unsigned long, int, int, int, long);
asmlinkage long post_stub_init_module(char const *, struct module *);
asmlinkage long pre_stub_exit(int);


struct fdesc {
	void * ip;
	void * gp;
};

struct fdesc fdesc_clone;
struct fdesc fdesc_clone2;
struct fdesc fdesc_execve;
struct fdesc fdesc_mmap;
struct fdesc fdesc_mmap2;
struct fdesc fdesc_init_module;
struct fdesc fdesc_exit;

spinlock_t map_lock = SPIN_LOCK_UNLOCKED;

static void oprof_output_map(ulong addr, ulong len,
	ulong offset, struct file * file, int is_execve)
{
	struct op_note note;

	
	if (!len)
		return;

	note.pid = current->pid;
	note.tgid = op_get_tgid();
	note.addr = addr;
	note.len = len;
	note.offset = offset;
	note.type = is_execve ? OP_EXEC : OP_MAP;
	note.hash = hash_path(file);
	if (note.hash == -1)
		return;
	oprof_put_note(&note);
}

static int oprof_output_maps(struct task_struct * task)
{
	int size=0;
	struct mm_struct * mm;
	struct vm_area_struct * map;


	if (!(mm = task->mm))
		goto out;

	lock_mmap(mm);
	spin_lock(&map_lock);

	for (map = mm->mmap; map; map = map->vm_next) {
		if (!(map->vm_flags & VM_EXEC) || !map->vm_file)
			continue;
		if (!(map->vm_flags & VM_EXECUTABLE))
			continue;

		oprof_output_map(map->vm_start, map->vm_end-map->vm_start,
			GET_VM_OFFSET(map), map->vm_file, 1);
	}
	for (map = mm->mmap; map; map = map->vm_next) {
		if (!(map->vm_flags & VM_EXEC) || !map->vm_file)
			continue;
		if (map->vm_flags & VM_EXECUTABLE)
			continue;

		oprof_output_map(map->vm_start, map->vm_end-map->vm_start,
			GET_VM_OFFSET(map), map->vm_file, 0);
	}
	spin_unlock(&map_lock);
	unlock_mmap(mm);

out:
	return size;
}



asmlinkage long
my_sys_execve (char * filename, char **argv, char **envp, struct pt_regs * regs)
{
	int error;

	MOD_INC_USE_COUNT;

	filename = getname(filename);
	error = PTR_ERR(filename);
	if (IS_ERR(filename))
		goto out;
	error = do_execve(filename, argv, envp, regs);

	if (!error) {
		PTRACE_OFF(current);
		oprof_output_maps(current);
	}
	putname(filename);
out:
	unlock_execve();
	MOD_DEC_USE_COUNT;
	return error;
}


static void out_mmap(ulong addr, ulong len, ulong prot, ulong flags,
	ulong fd, ulong offset)
{
	struct file * file;

	lock_out_mmap();
 
	file = fget(fd);
	if (!file)
		goto out;

	spin_lock(&map_lock);
	oprof_output_map(addr, len, offset, file, 0);
	spin_unlock(&map_lock);

	fput(file);

out:
	unlock_out_mmap();
}


asmlinkage void post_sys_mmap2(ulong ret, ulong addr, ulong len,
	ulong prot, ulong flags, ulong fd, ulong pgoff)
{
	
	MOD_INC_USE_COUNT;

	if ((prot & PROT_EXEC) && ret >= 0)
		out_mmap(ret, len, prot, flags, fd, pgoff << PAGE_SHIFT);
	goto out;
out:
	MOD_DEC_USE_COUNT;
}

asmlinkage void post_sys_mmap(ulong ret, ulong addr, ulong len,
	ulong prot, ulong flags, ulong fd, ulong off)
{
	
	MOD_INC_USE_COUNT;

	if ((prot & PROT_EXEC) && ret >= 0)
		out_mmap(ret, len, prot, flags, fd, off);
	goto out;
out:
	MOD_DEC_USE_COUNT;
}


inline static void oprof_report_fork(u32 old_pid, u32 new_pid, u32 old_tgid, u32 new_tgid)
{
	struct op_note note;

	note.type = OP_FORK;
	note.pid = old_pid;
	note.tgid = old_tgid;
	note.addr = new_pid;
	note.len = new_tgid;
	oprof_put_note(&note);
}


asmlinkage void post_sys_clone(long ret, long arg0, long arg1)
{
	u32 pid = current->pid;
	u32 tgid = op_get_tgid();

	
	MOD_INC_USE_COUNT;

	if (ret)
		oprof_report_fork(pid, ret, pid, tgid);
	MOD_DEC_USE_COUNT;
}

asmlinkage void post_sys_clone2(long ret, long arg0, long arg1, long arg2)
{
	u32 pid = current->pid;
	u32 tgid = op_get_tgid();
	long clone_flags = arg0;

	
	MOD_INC_USE_COUNT;

	if (ret) {
		if (clone_flags & CLONE_THREAD)
			oprof_report_fork(pid, ret, tgid, tgid);
		else
			oprof_report_fork(pid, ret, tgid, ret);
	}
	MOD_DEC_USE_COUNT;
}

asmlinkage void
post_sys_init_module(long ret, char const * name_user,
                     struct module * mod_user)
{
	
	MOD_INC_USE_COUNT;

	if (ret >= 0) {
		struct op_note note;

		note.type = OP_DROP_MODULES;
		oprof_put_note(&note);
	}
	MOD_DEC_USE_COUNT;
}

asmlinkage void pre_sys_exit(int error_code)
{
	struct op_note note;

	MOD_INC_USE_COUNT;

	note.addr = current->times.tms_utime;
	note.len = current->times.tms_stime;
	note.offset = current->start_time;
	note.type = OP_EXIT;
	note.pid = current->pid;
	note.tgid = op_get_tgid();
	oprof_put_note(&note);

	MOD_DEC_USE_COUNT;
}

extern void * sys_call_table[];


void op_save_syscalls(void)
{
	fdesc_clone.ip = sys_call_table[__NR_clone - __NR_ni_syscall];
	old_sys_clone = (void *)&fdesc_clone;
	fdesc_clone2.ip = sys_call_table[__NR_clone2 - __NR_ni_syscall];
	old_sys_clone2 = (void *)&fdesc_clone2;
	fdesc_execve.ip = sys_call_table[__NR_execve - __NR_ni_syscall];
	old_sys_execve = (void *)&fdesc_execve;
	fdesc_mmap.ip = sys_call_table[__NR_mmap - __NR_ni_syscall];
	old_sys_mmap = (void *)&fdesc_mmap;
	fdesc_mmap2.ip = sys_call_table[__NR_mmap2 - __NR_ni_syscall];
	old_sys_mmap2 = (void *)&fdesc_mmap2;
	fdesc_init_module.ip = sys_call_table[__NR_init_module - __NR_ni_syscall];
	old_sys_init_module = (void *)&fdesc_init_module;
	fdesc_exit.ip = sys_call_table[__NR_exit - __NR_ni_syscall];
	old_sys_exit = (void *)&fdesc_exit;
}

void op_intercept_syscalls(void)
{
	sys_call_table[__NR_clone - __NR_ni_syscall] = 
		((struct fdesc *)post_stub_clone)->ip;
	sys_call_table[__NR_clone2 - __NR_ni_syscall] = 
		((struct fdesc *)post_stub_clone2)->ip;
	sys_call_table[__NR_execve - __NR_ni_syscall] = 
		((struct fdesc *)my_ia64_execve)->ip;
	sys_call_table[__NR_mmap - __NR_ni_syscall] = 
		((struct fdesc *)post_stub_mmap)->ip;
	sys_call_table[__NR_mmap2 - __NR_ni_syscall] = 
		((struct fdesc *)post_stub_mmap2)->ip;
	sys_call_table[__NR_init_module - __NR_ni_syscall] = 
		((struct fdesc *)post_stub_init_module)->ip;
	sys_call_table[__NR_exit - __NR_ni_syscall] = 
		((struct fdesc *)pre_stub_exit)->ip;
}

void op_restore_syscalls(void)
{
	sys_call_table[__NR_clone - __NR_ni_syscall] = 
		((struct fdesc *)old_sys_clone)->ip;
	sys_call_table[__NR_clone2 - __NR_ni_syscall] = 
		((struct fdesc *)old_sys_clone2)->ip;
	sys_call_table[__NR_execve - __NR_ni_syscall] = 
		((struct fdesc *)old_sys_execve)->ip;
	sys_call_table[__NR_mmap - __NR_ni_syscall] = 
		((struct fdesc *)old_sys_mmap)->ip;
	sys_call_table[__NR_mmap2 - __NR_ni_syscall] = 
		((struct fdesc *)old_sys_mmap2)->ip;
	sys_call_table[__NR_init_module - __NR_ni_syscall] = 
		((struct fdesc *)old_sys_init_module)->ip;
	sys_call_table[__NR_exit - __NR_ni_syscall] = 
		((struct fdesc *)old_sys_exit)->ip;
}
