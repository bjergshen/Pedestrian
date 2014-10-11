#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include <omp.h>
#define MAX_VALUE 10000000


double _test(int value)
{
	int index;
	double result;

	result = 0.0;
	for(index = value + 1; index < MAX_VALUE; index +=2 )
		result += 1.0 / index;

	return result;
}

void test()
{
	int index;
	//int time1;
	//int time2;
	
	double time1 ;
	double time2 ;
	double value1,value2;
	double result[2];

	time1 = 0;
	time2 = 0;

	value1 = 0.0;
	//time1 =(double) GetTickCount();
	puts("test1\n");
	system("sudo hwclock -r --show");
	for(index = 1; index < MAX_VALUE; index ++)
		value1 += 1.0 / index;

	system("sudo hwclock -r --show");
	//time1 =(double) GetTickCount() - time1;

	value2 = 0.0;
	memset(result , 0, sizeof(double) * 2);
	//time2 =(double) GetTickCount();
	puts("test2\n");
	system("sudo hwclock -r --show");
#pragma omp parallel for
	for(index = 0; index < 2; index++)
		result[index] = _test(index);
	system("sudo hwclock -r --show");
	value2 = result[0] + result[1];
	//time2 = GetTickCount() - time2;

	//printf("time1 = %gms,time2 = %gms\n",time1*1000/(double)GetTickFrequency(),time2*1000/(double)GetTickFrequency());
	return;
}
int main(){
//	_test();
	test();
	return 0;
}
