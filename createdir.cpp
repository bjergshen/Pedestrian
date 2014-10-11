#include <sys/stat.h>
#include "stdlib.h"
#include<stdio.h>
#include <unistd.h>
#include<sys/types.h>
int  main()
{
	pid_t pid; 
	//创建新的进程 
    	pid=fork();
	int processID = getpid();
	char strID[10],filename[100],cmd[200];
	//如果返回0，则表示为子进程
	if(0 == pid)
        {
		sprintf(filename, "tmp/%d", processID);
		sprintf(cmd,"rm -r %s",filename);
		if(access(filename,0)==0)//使用access函数查看文件夹是否存在，存在则删除原文件夹
		{
			//printf("file exists!!!");			
			system(cmd);
		}
		if (mkdir("00000",0777))//用mkdir函数来创建新文件夹
		{
			printf("creat file bag failed!");
		}

		FILE *fp;
		
		sprintf(filename, "tmp/%d/hello.txt", processID);
		if((fp=fopen(filename,"w"))==NULL)//创建文件hello.txt
		{
			printf("creat file failed!\n");
		}

		fprintf(fp,"hello,world!");
		fclose(fp);
	}
	return 0;
}
/*
<pre></pre>
<p>程序说明：</p>
<p>使用fork()新建进程，并在子进程中作如下操作：</p>
<p>         1、查询当前目录下是否有以子进程ID命名的文件夹，如有则删除；</p>
<p>         2、新建以子进程ID为文件名的文件；</p>
<p>         3、在2中文件夹中新建hello.txt文件，并在其中写入一句话。</p>
<p>本程序为测试程序，仅为完成linux下文件夹的删除、新建，以及fork()的基本使用。</p>
<pre></pre>

*/
