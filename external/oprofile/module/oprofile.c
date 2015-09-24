/**
 * @file oprofile.c
 * Main driver code
 *
 * @remark Copyright 2002 OProfile authors
 * @remark Read the file COPYING
 *
 * @author John Levon
 * @author Philippe Elie
 */

#include "oprofile.h"
#include "op_util.h"
#include "config.h"

EXPORT_NO_SYMBOLS;
 
MODULE_AUTHOR("John Levon (levon@movementarian.org)");
MODULE_DESCRIPTION("Continuous Profiling Module");
MODULE_LICENSE("GPL");

MODULE_PARM(allow_unload, "i");
MODULE_PARM_DESC(allow_unload, "Allow module to be unloaded.");
#ifdef CONFIG_SMP
static int allow_unload;
#else
static int allow_unload = 1;
#endif

struct oprof_sysctl sysctl_parms;
struct oprof_sysctl sysctl;

static enum oprof_state state __cacheline_aligned_in_smp = STOPPED;
	 
static int op_major;

static volatile ulong oprof_opened __cacheline_aligned_in_smp;
static volatile ulong oprof_note_opened __cacheline_aligned_in_smp;
static DECLARE_WAIT_QUEUE_HEAD(oprof_wait);

static u32 oprof_ready[NR_CPUS] __cacheline_aligned_in_smp;
struct _oprof_data oprof_data[NR_CPUS] __cacheline_aligned;

struct op_note * note_buffer __cacheline_aligned_in_smp;
u32 note_pos __cacheline_aligned_in_smp;

static struct op_int_operations const * int_ops;

static char const * op_version = PACKAGE " " VERSION;
 

inline static int need_wakeup(uint cpu, struct _oprof_data * data)
{
	return data->nextbuf >= (data->buf_size - data->buf_watermark) && !oprof_ready[cpu];
}

inline static void next_sample(struct _oprof_data * data)
{
	if (unlikely(++data->nextbuf == data->buf_size))
		data->nextbuf = 0;
}

inline static void evict_op_entry(uint cpu, struct _oprof_data * data, long irq_enabled)
{
	next_sample(data);
	if (likely(!need_wakeup(cpu, data)))
		return;

	if (likely(irq_enabled)) {
		oprof_ready[cpu] = 1;
		wake_up(&oprof_wait);
	}
}

inline static void
fill_op_entry(struct op_sample * ops, long eip, pid_t pid, pid_t tgid, int ctr)
{
	ops->eip = eip;
	ops->pid = pid;
	ops->tgid = tgid;
	ops->counter = ctr;
}

void op_do_profile(uint cpu, long eip, long irq_enabled, int ctr)
{
	struct _oprof_data * data = &oprof_data[cpu];
	pid_t const pid = current->pid;
	pid_t const tgid = op_get_tgid();
	struct op_sample * samples = &data->buffer[data->nextbuf];

	data->nr_irq++;

	fill_op_entry(samples, eip, pid, tgid, ctr);
	evict_op_entry(cpu, data, irq_enabled);
}


static void stop_cpu_perfctr(int cpu)
{
	if (state == RUNNING)
		int_ops->stop_cpu(cpu);
}

static void start_cpu_perfctr(int cpu)
{
	if (state == RUNNING)
		int_ops->start_cpu(cpu);
}
 
spinlock_t note_lock __cacheline_aligned_in_smp = SPIN_LOCK_UNLOCKED;
int cpu_buffer_waiting;
 
static int is_ready(void)
{
	uint cpu_nr;
	for (cpu_nr = 0 ; cpu_nr < smp_num_cpus; cpu_nr++) {
		if (oprof_ready[cpu_nr]) {
			cpu_buffer_waiting = cpu_nr;
			return 1;
		}
	}
	return 0;
}

inline static void up_and_check_note(void)
{
	note_pos++;
	if (likely(note_pos < (sysctl.note_size - OP_PRE_NOTE_WATERMARK(sysctl.note_size)) && !is_ready()))
		return;

	if (unlikely(note_pos == sysctl.note_size)) {
		static int warned;
		if (!warned) {
			printk(KERN_WARNING "note buffer overflow: restart "
			       "oprofile with a larger note buffer.\n");
			warned = 1;
		}
		sysctl.nr_note_buffer_overflow++;
		note_pos = sysctl.note_size - 1;
	}

	
	oprof_ready[0] = 2;
	oprof_wake_up(&oprof_wait);
}

void __oprof_put_note(struct op_note * onote)
{
	
	if (state != RUNNING)
		return;

	memcpy(&note_buffer[note_pos], onote, sizeof(struct op_note));
	up_and_check_note();
}

void oprof_put_note(struct op_note * onote)
{
	spin_lock(&note_lock);
	__oprof_put_note(onote);
	spin_unlock(&note_lock);
}

static ssize_t oprof_note_read(char * buf, size_t count, loff_t * ppos)
{
	struct op_note * mybuf;
	uint num;
	ssize_t max;

	max = sizeof(struct op_note) * sysctl.note_size;

	if (*ppos || count != max)
		return -EINVAL;

	mybuf = vmalloc(max);
	if (!mybuf)
		return -EFAULT;

	spin_lock(&note_lock);

	num = note_pos;

	count = note_pos * sizeof(struct op_note);

	if (count)
		memcpy(mybuf, note_buffer, count);

	note_pos = 0;

	spin_unlock(&note_lock);

	if (count && copy_to_user(buf, mybuf, count))
		count = -EFAULT;

	vfree(mybuf);
	return count;
}

static int oprof_note_open(void)
{
	if (test_and_set_bit(0, &oprof_note_opened))
		return -EBUSY;
	INC_USE_COUNT_MAYBE;
	return 0;
}

static int oprof_note_release(void)
{
	BUG_ON(!oprof_note_opened);
	clear_bit(0, &oprof_note_opened);
	DEC_USE_COUNT_MAYBE;
	return 0;
}

static int check_buffer_amount(int cpu_nr)
{
	struct _oprof_data * data = &oprof_data[cpu_nr]; 
	int size = data->buf_size;
	int num = data->nextbuf;
	if (num < size - data->buf_watermark && oprof_ready[cpu_nr] != 2) {
		printk(KERN_WARNING "oprofile: Detected overflow of size %d. "
		       "You must increase the module buffer size with\n"
		       "opcontrol --setup --bufer-size= or reduce the "
		       "interrupt frequency\n", num);
		data->nr_buffer_overflow += num;
		num = size;
	} else
		data->nextbuf = 0;
	return num;
}

static int copy_buffer(char * buf, int cpu_nr)
{
	struct op_buffer_head head;
	int ret = -EFAULT;

	stop_cpu_perfctr(cpu_nr);
 
	head.cpu_nr = cpu_nr;
	head.count = check_buffer_amount(cpu_nr);
	head.state = state;

	oprof_ready[cpu_nr] = 0;

	if (copy_to_user(buf, &head, sizeof(struct op_buffer_head)))
		goto out;
 
	if (head.count) {
		size_t const size = head.count * sizeof(struct op_sample);
		if (copy_to_user(buf + sizeof(struct op_buffer_head),
			oprof_data[cpu_nr].buffer, size))
			goto out;
		ret = size + sizeof(struct op_buffer_head);
	} else {
		ret = sizeof(struct op_buffer_head);
	}
 
out:
	start_cpu_perfctr(cpu_nr);
	return ret;
}
 
static ssize_t oprof_read(struct file * file, char * buf, size_t count, loff_t * ppos)
{
	ssize_t max;

	if (!capable(CAP_SYS_PTRACE))
		return -EPERM;

	switch (MINOR(file->f_dentry->d_inode->i_rdev)) {
		case 2: return oprof_note_read(buf, count, ppos);
		case 0: break;
		default: return -EINVAL;
	}

	max = sizeof(struct op_buffer_head) + sizeof(struct op_sample) * sysctl.buf_size;

	if (*ppos || count != max)
		return -EINVAL;

	switch (state) {
		case RUNNING:
			wait_event_interruptible(oprof_wait, is_ready());
			if (signal_pending(current))
				return -EINTR;
			break;

		case STOPPING: {
			int cpu;

			if (!(file->f_flags & O_NONBLOCK))
				break;

			for (cpu = 0; cpu < smp_num_cpus; ++cpu) {
				if (oprof_data[cpu].nextbuf) {
					cpu_buffer_waiting = cpu;
					oprof_ready[cpu] = 2;
					break;
				}
			}
 
			if (cpu == smp_num_cpus)
				return -EAGAIN;
 
		}
			break;

		case STOPPED: BUG();
	}

	return copy_buffer(buf, cpu_buffer_waiting);
}


static int oprof_start(void);
static int oprof_stop(void);

static int oprof_open(struct inode * ino, struct file * file)
{
	int err;

	if (!capable(CAP_SYS_PTRACE))
		return -EPERM;

	switch (MINOR(file->f_dentry->d_inode->i_rdev)) {
		case 1: return oprof_hash_map_open();
		case 2: return oprof_note_open();
		case 0:
			
			if (is_map_ready())
				break;
		default:
			return -EINVAL;
	}

	if (test_and_set_bit(0, &oprof_opened))
		return -EBUSY;

	err = oprof_start();
	if (err)
		clear_bit(0, &oprof_opened);
	return err;
}

static int oprof_release(struct inode * ino, struct file * file)
{
	switch (MINOR(file->f_dentry->d_inode->i_rdev)) {
		case 1: return oprof_hash_map_release();
		case 2: return oprof_note_release();
		case 0: break;
		default: return -EINVAL;
	}

	BUG_ON(!oprof_opened);

	clear_bit(0, &oprof_opened);

	
	return oprof_stop();
}

static int oprof_mmap(struct file * file, struct vm_area_struct * vma)
{
	if (MINOR(file->f_dentry->d_inode->i_rdev) == 1)
		return oprof_hash_map_mmap(file, vma);
	return -EINVAL;
}

static void oprof_free_mem(uint num)
{
	uint i;
	for (i=0; i < num; i++) {
		if (oprof_data[i].buffer)
			vfree(oprof_data[i].buffer);
		oprof_data[i].buffer = NULL;
	}
	vfree(note_buffer);
	note_buffer = NULL;
}

static int oprof_init_data(void)
{
	uint i, notebufsize;
	ulong buf_size;
	struct _oprof_data * data;

	sysctl.nr_note_buffer_overflow = 0;
	notebufsize = sizeof(struct op_note) * sysctl.note_size;
	note_buffer = vmalloc(notebufsize);
 	if (!note_buffer) {
		printk(KERN_ERR "oprofile: failed to allocate note buffer of %u bytes\n",
			notebufsize);
		return -EFAULT;
	}
	note_pos = 0;

	
	for (i = 0; i < smp_num_cpus; ++i) {
		data = &oprof_data[i];
		data->buf_size = 0;
		data->buffer = 0;
		data->buf_watermark = 0;
		data->nr_buffer_overflow = 0;
	}
 
	buf_size = (sizeof(struct op_sample) * sysctl.buf_size);

	for (i = 0 ; i < smp_num_cpus ; ++i) {
		data = &oprof_data[i];

		data->buffer = vmalloc(buf_size);
		if (!data->buffer) {
			printk(KERN_ERR "oprofile: failed to allocate eviction buffer of %lu bytes\n", buf_size);
			oprof_free_mem(i);
			return -EFAULT;
		}

		memset(data->buffer, 0, buf_size);

		data->buf_size = sysctl.buf_size;
		data->buf_watermark = OP_PRE_WATERMARK(data->buf_size);
		data->nextbuf = 0;
	}

	return 0;
}

static int parms_check(void)
{
	int err;

	if ((err = check_range(sysctl.buf_size, OP_MIN_BUF_SIZE, OP_MAX_BUF_SIZE,
		"sysctl.buf_size value %d not in range (%d %d)\n")))
		return err;
	if ((err = check_range(sysctl.note_size, OP_MIN_NOTE_TABLE_SIZE, OP_MAX_NOTE_TABLE_SIZE,
		"sysctl.note_size value %d not in range (%d %d)\n")))
		return err;

	if ((err = int_ops->check_params()))
		return err;

	return 0;
}


static DECLARE_MUTEX(sysctlsem);


static int oprof_start(void)
{
	int err = 0;

	down(&sysctlsem);

	sysctl_parms.cpu_type = sysctl.cpu_type;
	sysctl = sysctl_parms;

	if ((err = oprof_init_data()))
		goto out;

	if ((err = parms_check())) {
		oprof_free_mem(smp_num_cpus);
		goto out;
	}

	if ((err = int_ops->setup())) {
		oprof_free_mem(smp_num_cpus);
		goto out;
	}

	op_intercept_syscalls();

	int_ops->start();

	state = RUNNING;

out:
	up(&sysctlsem);
	return err;
}

static void oprof_partial_stop(void)
{
	BUG_ON(state == STOPPED);

	if (state == RUNNING) {
		op_restore_syscalls();
		int_ops->stop();
	}

	state = STOPPING;
}

static int oprof_stop(void)
{
	uint i;
	
	int err = -EINVAL;

	down(&sysctlsem);

	BUG_ON(state == STOPPED);


	oprof_partial_stop();

	spin_lock(&note_lock);

	for (i = 0 ; i < smp_num_cpus; i++) {
		struct _oprof_data * data = &oprof_data[i];
		oprof_ready[i] = 0;
		data->nextbuf = 0;
	}

	oprof_free_mem(smp_num_cpus);

	spin_unlock(&note_lock);
	err = 0;

	
	state = STOPPED;
	up(&sysctlsem);
	return err;
}

static struct file_operations oprof_fops = {
#ifdef HAVE_FILE_OPERATIONS_OWNER
	owner: THIS_MODULE,
#endif
	open: oprof_open,
	release: oprof_release,
	read: oprof_read,
	mmap: oprof_mmap,
};



static void lock_sysctl(void)
{
	MOD_INC_USE_COUNT;
	down(&sysctlsem);
}

static void unlock_sysctl(void)
{
	up(&sysctlsem);
	MOD_DEC_USE_COUNT;
}

static int get_nr_interrupts(ctl_table * table, int write, struct file * filp, void * buffer, size_t * lenp)
{
	uint cpu;
	int ret = -EINVAL;

	lock_sysctl();

	if (write)
		goto out;

	sysctl.nr_interrupts = 0;

	for (cpu = 0 ; cpu < smp_num_cpus; cpu++) {
		sysctl.nr_interrupts += oprof_data[cpu].nr_irq;
		oprof_data[cpu].nr_irq = 0;
	}

	ret =  proc_dointvec(table, write, filp, buffer, lenp);
out:
	unlock_sysctl();
	return ret;
}

static int get_nr_buffer_overflow(ctl_table * table, int write, struct file * filp, void * buffer, size_t * lenp)
{
	uint cpu;
	int ret = -EINVAL;

	lock_sysctl();

	if (write)
		goto out;

	for (cpu = 0 ; cpu < smp_num_cpus; cpu++) {
		sysctl.nr_buffer_overflow += oprof_data[cpu].nr_buffer_overflow;
		oprof_data[cpu].nr_buffer_overflow = 0;
	}

	ret =  proc_dointvec(table, write, filp, buffer, lenp);
out:
	unlock_sysctl();
	return ret;
}

int lproc_dointvec(ctl_table * table, int write, struct file * filp, void * buffer, size_t * lenp)
{
	int err;

	lock_sysctl();
	err = proc_dointvec(table, write, filp, buffer, lenp);
	unlock_sysctl();

	return err;
}

static void do_actual_dump(void)
{
	uint cpu;

	for (cpu = 0 ; cpu < smp_num_cpus; cpu++)
		oprof_ready[cpu] = 2;
	oprof_wake_up(&oprof_wait);
}

static int sysctl_do_dump(ctl_table * table, int write, struct file * filp, void * buffer, size_t * lenp)
{
	int err = -EINVAL;

	lock_sysctl();

	if (state != RUNNING)
		goto out;
 
	if (!write) {
		err = proc_dointvec(table, write, filp, buffer, lenp);
		goto out;
	}

	do_actual_dump();

	err = 0;
out:
	unlock_sysctl();
	return err;
}

static int sysctl_do_dump_stop(ctl_table * table, int write, struct file * filp, void * buffer, size_t * lenp)
{
	int err = -EINVAL;

	lock_sysctl();

	if (state != RUNNING)
		goto out;
 
	if (!write) {
		err = proc_dointvec(table, write, filp, buffer, lenp);
		goto out;
	}

	oprof_partial_stop();

	
	do_actual_dump();

	err = 0;
out:
	unlock_sysctl();
	return err;
}

static int const nr_oprof_static = 8;

static ctl_table oprof_table[] = {
	{ 1, "bufsize", &sysctl_parms.buf_size, sizeof(int), 0644, NULL, &lproc_dointvec, NULL, },
	{ 1, "dump", &sysctl_parms.dump, sizeof(int), 0666, NULL, &sysctl_do_dump, NULL, },
	{ 1, "dump_stop", &sysctl_parms.dump_stop, sizeof(int), 0644, NULL, &sysctl_do_dump_stop, NULL, },
	{ 1, "nr_interrupts", &sysctl.nr_interrupts, sizeof(int), 0444, NULL, &get_nr_interrupts, NULL, },
	{ 1, "notesize", &sysctl_parms.note_size, sizeof(int), 0644, NULL, &lproc_dointvec, NULL, },
	{ 1, "cpu_type", &sysctl.cpu_type, sizeof(int), 0444, NULL, &lproc_dointvec, NULL, },
	{ 1, "note_buffer_overflow", &sysctl.nr_note_buffer_overflow, sizeof(int), 0444, NULL, &lproc_dointvec, NULL, },
	{ 1, "buffer_overflow", &sysctl.nr_buffer_overflow, sizeof(int), 0444, NULL, &get_nr_buffer_overflow, NULL, },
	{ 0, }, { 0, }, { 0, }, { 0, }, { 0, }, { 0, }, { 0, }, { 0, },
	{ 0, },
};

static ctl_table oprof_root[] = {
	{1, "oprofile", NULL, 0, 0755, oprof_table},
 	{0, },
};

static ctl_table dev_root[] = {
	{CTL_DEV, "dev", NULL, 0, 0555, oprof_root},
	{0, },
};

static struct ctl_table_header * sysctl_header;


static int __init init_sysctl(void)
{
	int err = 0;
	ctl_table * next = &oprof_table[nr_oprof_static];

	
	sysctl_parms.buf_size = OP_DEFAULT_BUF_SIZE;
	sysctl_parms.note_size = OP_DEFAULT_NOTE_SIZE;

	if ((err = int_ops->add_sysctls(next)))
		return err;

	sysctl_header = register_sysctl_table(dev_root, 0);
	return err;
}

static void cleanup_sysctl(void)
{
	ctl_table * next = &oprof_table[nr_oprof_static];
	unregister_sysctl_table(sysctl_header);

	int_ops->remove_sysctls(next);

	return;
}

static int can_unload(void)
{
	int can = -EBUSY;
	down(&sysctlsem);

	if (allow_unload && state == STOPPED && !GET_USE_COUNT(THIS_MODULE))
		can = 0;
	up(&sysctlsem);
	return can;
}

int __init oprof_init(void)
{
	int err = 0;

	if (sysctl.cpu_type != CPU_RTC) {
		int_ops = op_int_interface();

		
		if ((err = int_ops->init())) {
			int_ops = &op_rtc_ops;
			if ((err = int_ops->init()))
				return err;
			sysctl.cpu_type = CPU_RTC;
		}
	} else {
		int_ops = &op_rtc_ops;
		if ((err = int_ops->init()))
			return err;
	}

	if ((err = init_sysctl()))
		goto out_err;

	err = op_major = register_chrdev(0, "oprof", &oprof_fops);
	if (err < 0)
		goto out_err2;

	err = oprof_init_hashmap();
	if (err < 0) {
		printk(KERN_ERR "oprofile: couldn't allocate hash map !\n");
		unregister_chrdev(op_major, "oprof");
		goto out_err2;
	}

	
	THIS_MODULE->can_unload = can_unload;

	
	op_save_syscalls();

	printk(KERN_INFO "%s loaded, major %u\n", op_version, op_major);
	return 0;

out_err2:
	cleanup_sysctl();
out_err:
	int_ops->deinit();
	return err;
}

void __exit oprof_exit(void)
{
	oprof_free_hashmap();

	unregister_chrdev(op_major, "oprof");

	cleanup_sysctl();

	int_ops->deinit();
}

