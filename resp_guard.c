#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/sched.h>

#define CPU_THRESHOLD 300000000ULL   // 0.3 sec CPU time
#define CHECK_INTERVAL (10 * HZ)

static struct timer_list hog_timer;

static void detect_cpu_hog(struct timer_list *t)
{
    struct task_struct *task;

    for_each_process(task) {

        /* Skip kernel threads */
        if (task->flags & PF_KTHREAD)
            continue;

        /* If process already penalized, skip */
        if (task_nice(task) >= 10)
            continue;

        if (task->se.sum_exec_runtime > CPU_THRESHOLD) {

            printk(KERN_INFO "RESP_GUARD: CPU hog detected! PID=%d NAME=%s\n",
                   task->pid, task->comm);

            /* Reduce priority only once */
            set_user_nice(task, 10);

            printk(KERN_INFO "RESP_GUARD: Priority reduced for PID=%d new nice=%d\n",
                   task->pid, task_nice(task));
        }
    }

    mod_timer(&hog_timer, jiffies + CHECK_INTERVAL);
}

static int __init resp_guard_init(void)
{
    printk(KERN_INFO "RESP_GUARD: Module loaded\n");

    timer_setup(&hog_timer, detect_cpu_hog, 0);
    mod_timer(&hog_timer, jiffies + CHECK_INTERVAL);

    return 0;
}

static void __exit resp_guard_exit(void)
{
    del_timer(&hog_timer);
    printk(KERN_INFO "RESP_GUARD: Module unloaded\n");
}

module_init(resp_guard_init);
module_exit(resp_guard_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Team Illumina");
MODULE_DESCRIPTION("System Responsiveness Guard - CPU Hog Detection & Control");


