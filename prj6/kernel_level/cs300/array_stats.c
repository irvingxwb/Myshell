#include "array_stats.h"
#include <linux/kernel.h>
#include <linux/uaccess.h>

asmlinkage long sys_array_stats(struct array_stats *stats, long data[], long size)
{
    long tempdata, i;
    struct array_stats temp;

    if(size <= 0)
        return -EINVAL;

    for(i=0;i<size;i++)
    {
        if(copy_from_user((long*)&tempdata, data+i, sizeof(long)))
            return -EFAULT;
	
	//printk("input: %ld array index: %ld\n",tempdata, i);

        if(i == 0)
        {
            temp.max = tempdata;
            temp.min = tempdata;
            temp.sum = tempdata;
	    //printk("initial : %ld  %ld  %ld \n",temp.max, temp.min, temp.sum);
        }
        else
        {
            if(temp.max < tempdata)
                temp.max = tempdata;
            else if(temp.min > tempdata)
                temp.min = tempdata;
            
            temp.sum += tempdata;
        }
    }

	//printk("out put: %ld  %ld  %ld \n",temp.max, temp.min, temp.sum);

    if(copy_to_user(stats, &temp, sizeof(struct array_stats)))
        return -EFAULT;
    else
        return 0;

}
