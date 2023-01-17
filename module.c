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

static bool dump_kernel_stack = false;
static int match_sig = -1;
static int match_from_uid = -1;
static int match_target_uid = -1;

module_param(dump_kernel_stack, bool, S_IRUSR | S_IWUSR);
module_param(match_sig, int, S_IRUSR | S_IWUSR);
module_param(match_from_uid, int, S_IRUSR | S_IWUSR);
module_param(match_target_uid, int, S_IRUSR | S_IWUSR);

static void dump_stack_if_match(int sig, int from_uid, int to_uid){
	if (!dump_kernel_stack) {
		return;
	}
	if (match_sig != -1 && match_sig != sig) {
		return;
	}
	if (match_from_uid != -1 && match_from_uid != from_uid) {
		return;
	}
	if (match_target_uid != -1 && match_target_uid != to_uid) {
		return;
	}
	pr_info("-----Stackdump Begin-----");
	dump_stack();
	pr_info("-----Stackdump End-----");
}

static int handler_pre_send_signal(struct kprobe *p, struct pt_regs *regs){
	int sig = regs->regs[0];
	struct task_struct *t = (struct task_struct *)regs->regs[2];
	pr_info("send_signal: sig=%d from_pid=%d from_uid=%d to_pid=%d to_uid=%d", 
		sig, current->pid, current->cred->uid, t->pid, t->cred->uid);
	dump_stack_if_match(sig, current->cred->uid.val, t->cred->uid.val);
	return 0;
}


static struct kprobe kp_send_signal = {
    .symbol_name = "__send_signal",
    .pre_handler = handler_pre_send_signal,
};


int signal_tracer_init(void){
	pr_info("SignalTracer init");
	int ret = register_kprobe(&kp_send_signal);
	return ret;
}

void signal_tracer_exit(void){
	pr_info("SignalTracer exit");
	unregister_kprobe(&kp_send_signal);
}

module_init(signal_tracer_init);
module_exit(signal_tracer_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ylarod");
MODULE_DESCRIPTION("A kernel module for tracing signal");