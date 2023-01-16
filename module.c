#include "module.h"
#include "asm/current.h"
#include "asm/string.h"
#include "linux/file.h"
#include "linux/gfp.h"
#include "linux/printk.h"
#include "linux/slab.h"
#include "linux/fs_struct.h"
#include <linux/namei.h>
#include <linux/dcache.h>

static int handler_pre_send_signal(struct kprobe *p, struct pt_regs *regs){
	int sig = regs->regs[0];
	struct task_struct *t = (struct task_struct *)regs->regs[2];
	pr_info("send_signal: sig=%d from_pid=%d to_pid=%d ", sig, current->pid, t->pid);
	return 0;
}


static struct kprobe kp_send_signal = {
    .symbol_name = "__send_signal",
    .pre_handler = handler_pre_send_signal,
};


int root_guard_init(void){
	pr_info("SignalTracer init");
	int ret = register_kprobe(&kp_send_signal);
	return ret;
}

void root_guard_exit(void){
	pr_info("SignalTracer exit");
	unregister_kprobe(&kp_send_signal);
}

module_init(root_guard_init);
module_exit(root_guard_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ylarod");
MODULE_DESCRIPTION("A kernel module for tracing signal");