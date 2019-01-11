#include<stdio.h>
#include<stdlib.h>
#include "stats.h"

#define MAX_FACTORY 50

struct Factstats{
	int factnumber;
	int made;
	int eaten;
	double total;
	double min;
	double max;
};

struct Factstats F[MAX_FACTORY];
int num;

void stats_init(int num_producers)
{
	int i;
	//F = (struct Factstats*)malloc(sizeof(struct Factstats) * num_producers);
	num = num_producers;
	for(i=0;i<num_producers;i++)
	{
		F[i].factnumber = i;
		F[i].made = 0;
		F[i].eaten = 0;
		F[i].max = 0;
		F[i].min = 10000;
		F[i].total = 0;
	}
}

void stats_cleanup(void)
{
	//free(F);
	return;
}

void stats_record_produced(int factory_number)
{
	F[factory_number].made++;
}

void stats_record_consumed(int factory_number, double delay_in_ms)
{
	F[factory_number].total += delay_in_ms;
	F[factory_number].eaten++;
	
	if(F[factory_number].max < delay_in_ms)
		F[factory_number].max = delay_in_ms;
		
	if(F[factory_number].min > delay_in_ms)
		F[factory_number].min = delay_in_ms;
}

void stats_display(void)
{
	int i;
	printf("%8s%8s%8s%12s%12s%12s\n", "Factory#", "#Made", "#Eaten","Min Delay","Avg Delay","Max Delay");
	for(i=0;i<num;i++)
	{
		printf("%8d%8d%8d%12.5f%12.5f%12.5f\n",F[i].factnumber, F[i].made, F[i].eaten, F[i].min, F[i].total/F[i].eaten, F[i].max);
	}
}

