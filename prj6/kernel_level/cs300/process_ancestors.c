#include "process_ancestors.h"
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/cred.h>

asmlinkage long sys_process_ancestors(struct process_info info_array[], long size, long *num_filled)
{
    struct task_struct* currentptr;
    struct process_info currentpro;
    struct list_head *headnode, *tempnode;
    long filled=0, index=0, count;

    currentptr = current;
    
    while(1)
    {
        currentpro.pid = currentptr->pid;
        strcpy(currentpro.name,currentptr->comm);
        currentpro.state = currentptr->state;
        currentpro.uid = currentptr->cred->uid.val;
        currentpro.nvcsw = currentptr->nvcsw;
        currentpro.nivcsw = currentptr->nivcsw;

        headnode = tempnode = &(currentptr->children);
        tempnode = tempnode->next;
        count = 1;
        while(headnode != tempnode)
        {
            tempnode = tempnode->next;
            count++;
        }
        currentpro.num_children = count;

        headnode = tempnode = &(currentptr->sibling);
        tempnode = tempnode->next;
        count = 1;
        while(headnode != tempnode)
        {
            tempnode = tempnode->next;
            count++;
        }
        currentpro.num_siblings = count;

        if(copy_to_user(info_array+index, &currentpro, sizeof(struct process_info)))
            return -EFAULT;

        currentptr = currentptr->parent;
        filled++;
        index++;
        if(currentptr->parent == currentptr)    /*judge end cndition */
            break;
    }

    if(copy_to_user(num_filled, &filled, sizeof(long)))
        return -EFAULT;
    
    return 0;
}
