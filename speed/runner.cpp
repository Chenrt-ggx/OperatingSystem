#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>

double time[1<<7][1<<16],sumt[1<<7];
double score[1<<7][1<<16],sums[1<<7];

const char* fileName[]=
{
	"AGE.cpp",
	"CLK.cpp",
	"GRP.cpp",
	"L2Q.cpp",
	"LRU.cpp",
};

inline void remove(char* buffer)
{
	char* temp=(char*)malloc(1<<10);
	FILE* test=fopen(buffer,"r");
	if (test==NULL) return;
	else fclose(test);
	sprintf(temp,"rm %s",buffer);
	system(temp); free(temp); return;
}

inline int init(char* buffer)
{
	FILE* in; int tot;
	for (tot=0;true;fclose(in),tot++)
	{
		sprintf(buffer,"testpoints/testpoint%d.stdin",tot);
		in=fopen(buffer,"r");
		if (in==NULL) break;
	}
	printf("%d testpoint detected\n\n",tot);
	for (int i=0;i<(int)(sizeof(fileName)/sizeof(*fileName));i++)
	{
		in=fopen(fileName[i],"r");
		if (in==NULL) printf("wrong file name %s\n",fileName[i]),exit(1);
		else fclose(in);
		sprintf(buffer,"g++ main.cpp %s -o main%d -std=c++11",fileName[i],i);
		system(buffer);
		printf("%s compiled\n",fileName[i]);
	}
	putchar('\n');
	return tot;
}

inline int run(int tot,char* buffer)
{
	FILE* p;
	for (int i=0;i<tot;i++)
	{
		printf("testing testpoint%d\n",i);
		for (int j=0;j<(int)(sizeof(fileName)/sizeof(*fileName));j++)
		{
			#ifdef _WIN64
				sprintf(buffer,"main%d.exe testpoints/testpoint%d.stdin \
					testpoints/testpoint%d.stdout",j,i,i);
			#else
				sprintf(buffer,"./main%d testpoints/testpoint%d.stdin \
					testpoints/testpoint%d.stdout",j,i,i);
			#endif
			p=popen(buffer,"r"); fgets(buffer,1<<10,p); fclose(p);
			assert(sscanf(buffer,"%lf %lf",&time[j][i],&score[j][i])==2);
			sumt[j]+=time[j][i],sums[j]+=score[j][i];
			printf("program %d done\n",j);
		}
	}
	puts("\n^^^^^^finished^^^^^^\n");
	return tot;
}

inline int printResult(int tot)
{
	freopen("result.txt","w",stdout);
	for (int i=0;i<tot;i++,puts(""))
	{
		printf("testpoint%d\n",i);
		for (int j=0;j<(int)(sizeof(fileName)/sizeof(*fileName));j++)
			printf("time/score of %s at testpoint%d is %lf / %lf\n",
				fileName[j],i,time[j][i],score[j][i]);
	}
	puts("-------------------------\n");
	for (int j=0;j<(int)(sizeof(fileName)/sizeof(*fileName));j++,puts(""))
	{
		printf("program %s\n",fileName[j]);
		for (int i=0;i<tot;i++)
			printf("time/score of %s at testpoint%d is %lf / %lf\n",
				fileName[j],i,time[j][i],score[j][i]);
	}
	puts("-------------------------\n");
	for (int i=0;i<(int)(sizeof(fileName)/sizeof(*fileName));i++)
	{
		printf("total time of %s is %lf\n",fileName[i],sumt[i]);
		printf("total score of %s is %lf\n",fileName[i],sums[i]);
		putchar('\n');
	}
	return tot;
}

inline void clean(char* buffer)
{
	for (int i=0;i<(int)(sizeof(fileName)/sizeof(*fileName));i++)
	{
		#ifdef _WIN64
			sprintf(buffer,"main%d.exe",i);
		#else
			sprintf(buffer,"main%d",i);
		#endif
		remove(buffer);
	}
	return;
}

int main()
{
	char* buffer=(char*)malloc(1<<10);
	printResult(run(init(buffer),buffer));
	clean(buffer); free(buffer); return 0;
}