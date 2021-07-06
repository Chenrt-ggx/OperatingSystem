#pragma GCC optimize(2)
#pragma GCC optimize(3)
#pragma GCC optimize("Ofast")
#pragma GCC optimize("inline")
#pragma GCC optimize("-fgcse")
#pragma GCC optimize("-fgcse-lm")
#pragma GCC optimize("-fipa-sra")
#pragma GCC optimize("-ftree-pre")
#pragma GCC optimize("-ftree-vrp")
#pragma GCC optimize("-fpeephole2")
#pragma GCC optimize("-ffast-math")
#pragma GCC optimize("-fsched-spec")
#pragma GCC optimize("unroll-loops")
#pragma GCC optimize("-falign-jumps")
#pragma GCC optimize("-falign-loops")
#pragma GCC optimize("-falign-labels")
#pragma GCC optimize("-fdevirtualize")
#pragma GCC optimize("-fcaller-saves")
#pragma GCC optimize("-fcrossjumping")
#pragma GCC optimize("-fthread-jumps")
#pragma GCC optimize("-funroll-loops")
#pragma GCC optimize("-fwhole-program")
#pragma GCC optimize("-freorder-blocks")
#pragma GCC optimize("-fschedule-insns")
#pragma GCC optimize("inline-functions")
#pragma GCC optimize("-ftree-tail-merge")
#pragma GCC optimize("-fschedule-insns2")
#pragma GCC optimize("-fstrict-aliasing")
#pragma GCC optimize("-fstrict-overflow")
#pragma GCC optimize("-falign-functions")
#pragma GCC optimize("-fcse-skip-blocks")
#pragma GCC optimize("-fcse-follow-jumps")
#pragma GCC optimize("-fsched-interblock")
#pragma GCC optimize("-fpartial-inlining")
#pragma GCC optimize("no-stack-protector")
#pragma GCC optimize("-freorder-functions")
#pragma GCC optimize("-findirect-inlining")
#pragma GCC optimize("-fhoist-adjacent-loads")
#pragma GCC optimize("-frerun-cse-after-loop")
#pragma GCC optimize("inline-small-functions")
#pragma GCC optimize("-finline-small-functions")
#pragma GCC optimize("-ftree-switch-conversion")
#pragma GCC optimize("-foptimize-sibling-calls")
#pragma GCC optimize("-fexpensive-optimizations")
#pragma GCC optimize("-funsafe-loop-optimizations")
#pragma GCC optimize("inline-functions-called-once")
#pragma GCC optimize("-fdelete-null-pointer-checks")

#include "pageReplace.h"
#define MAX_PHY_PAGE 64

#define RANGE 1
#include <random>
std::mt19937 generator(0x19260817);

unsigned int last,count;
unsigned long long vis[MAX_PHY_PAGE];
const unsigned long long ORI=0x8000000000000000;

void pageReplace(long physic_memery[],long nwAdd)
{
	for (register unsigned int i=0;i<MAX_PHY_PAGE;++i) vis[i]>>=1;
	for (register unsigned int i=0;i<MAX_PHY_PAGE;++i)
		if ((nwAdd>>12)==physic_memery[i])
			{vis[i]|=ORI; return;}
	for (register unsigned int i=0;i<MAX_PHY_PAGE;++i)
		if (physic_memery[i]==0)
		{
			physic_memery[i]=nwAdd>>12;
			vis[i]|=ORI; return;
		}
	int index;
	if (last==nwAdd>>12) ++count; else count=0;
	if (count==RANGE) count=0,index=generator()%MAX_PHY_PAGE;
	else
	{
		unsigned long long min=0xffffffffffffffff;
		for (register unsigned int i=0;i<MAX_PHY_PAGE;++i)
		if (vis[i]<min) min=vis[i],index=i;
	}
	last=physic_memery[index];
	physic_memery[index]=nwAdd>>12;
	vis[index]|=ORI; return;
}