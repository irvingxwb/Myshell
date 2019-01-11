#include <stdio.h>
#include "process_ancestors.h"
#include <sys/syscall.h>
#include <unistd.h>
#define _process_system_call  342

int main()
{
    long number=0, i;
    struct process_info info_array[10];
    printf("Driving to kernel mode\n");
    long result = syscall(_process_system_call, info_array, 10, &number);
    printf("\nRising to user level, result : %ld\n",result);

    printf("number filled : %ld \n",number);

    for(i=0;i<number;i++)
    {        
        printf("pid = %ld\nuid = %ld\n",info_array[i].pid, info_array[i].uid);
        printf("children = %ld\nnvcsw = %ld\n",info_array[i].num_children, info_array[i].nvcsw);
    }

}