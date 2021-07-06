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

#define MOD 7
#define SIZE 8
#define GROUP 8
#define _CLOCK_REMOVE

struct stu
{
	struct stu* pre;
	struct stu* nxt;
	bool tag; unsigned int val;
};

int poolCount;
stu pool[MAX_PHY_PAGE];
typedef struct stu* ptr;

class List
{
private:
	int curSize;
	ptr head,tail;
public:
	inline List()
	{
		curSize=0;
		head=tail=0;
	}

	inline ptr insHead(unsigned int page)
	{
		if (curSize==SIZE) return 0;
		ptr node=&pool[poolCount++];
		node->val=page,node->tag=false,node->nxt=head;
		if (head) head->pre=node;
		head=node,curSize++;
		if (!tail) tail=node;
		return node;
	}

	inline void insHead(ptr node,bool tag)
	{
		node->tag=tag,node->nxt=head,node->pre=0;
		if (!tail) tail=node;
		if (head) head->pre=node;
		head=node; return;
	}

	inline ptr remove(unsigned int page)
	{
		for (ptr i=head;i;i=i->nxt)
			if (i->val==page)
			{
				if (i==head) head=head->nxt;
				if (i==tail) tail=tail->pre;
				if (i->nxt) i->nxt->pre=i->pre;
				if (i->pre) i->pre->nxt=i->nxt;
				return i;
			}
		return 0;
	}

	inline ptr removeTail()
	{
		ptr out=tail;
		#ifdef _CLOCK_REMOVE
			if (!out) return 0;
			while (out->pre && out->tag)
				out->tag=0,out=out->pre;
			if (out==head) head=head->nxt;
			if (out==tail) tail=tail->pre;
			if (out->nxt) out->nxt->pre=out->pre;
			if (out->pre) out->pre->nxt=out->nxt;
		#else
			if (tail) tail=tail->pre;
			tail->nxt=0;
		#endif
		return out;
	}
};

List list[GROUP];

void pageReplace(long physic_memery[],long nwAdd)
{
	unsigned int page=(unsigned int)(nwAdd>>12);
	ptr node=list[page&MOD].remove(page);
	if (node)
	{
		physic_memery[node-pool]=page;
		list[page&MOD].insHead(node,true);
		return;
	}
	node=list[page&MOD].insHead(page);
	if (!node)
	{
		node=list[page&MOD].removeTail();
		node->tag=false;
		node->val=page;
		list[page&MOD].insHead(node,false);
	}
	physic_memery[node-pool]=page;
	return;
}