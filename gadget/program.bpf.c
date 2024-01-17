// SPDX-License-Identifier: (LGPL-2.1 OR BSD-2-Clause)
/* Copyright (c) 2021-2024 The Inspektor Gadget authors */


#include <vmlinux.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_core_read.h>
#include <bpf/bpf_tracing.h>

#include <gadget/buffer.h>
#include <gadget/types.h>
#include <gadget/macros.h>
#include <gadget/mntns_filter.h>

#ifndef SIGILL
#define SIGILL 4
#endif
#ifndef SIGUSR1
#define SIGUSR1 10
#endif

#ifndef TASK_COMM_LEN
#define TASK_COMM_LEN 16
#endif

struct event {
	gadget_timestamp timestamp;
	gadget_mntns_id mntns_id;
	__u32 pid;
	__u8 comm[TASK_COMM_LEN];
};

const volatile int target_pid = 0;
const volatile int target_signal = SIGILL;
const volatile int min_threads = 2;

GADGET_PARAM(target_pid);
GADGET_PARAM(target_signal);
GADGET_PARAM(min_threads);

GADGET_TRACER_MAP(events, 1024 * 256);

GADGET_TRACER(signalgogadget, events, event);

SEC("tracepoint/syscalls/sys_enter_close")
int tracepoint__sys_enter_close(struct trace_event_raw_sys_enter *ctx)
{
	struct event *event;
	__u64 pid_tgid = bpf_get_current_pid_tgid();
	struct task_struct *task = (struct task_struct *) bpf_get_current_task();

	/* Setting up the filter is mandatory */
	if (target_pid == 0)
		return 0;

	if (target_pid != 0 && target_pid != pid_tgid >> 32)
		return 0;

	int nr_threads = BPF_CORE_READ(task, signal, nr_threads);
	if (nr_threads < min_threads)
		return 0;

	event = gadget_reserve_buf(&events, sizeof(*event));
	if (!event)
		return 0;

	/* event data */
	event->pid = pid_tgid >> 32;
	bpf_get_current_comm(&event->comm, sizeof(event->comm));
	event->mntns_id = gadget_get_mntns_id();
	event->timestamp = bpf_ktime_get_boot_ns();

	/* emit event */
	gadget_submit_buf(ctx, &events, event, sizeof(*event));

	/* signal target process */
	bpf_send_signal(target_signal);
	return 0;
}

char LICENSE[] SEC("license") = "GPL";
