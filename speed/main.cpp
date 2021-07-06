#include <ctime>
#include <random>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <unordered_map>

//#define _DEBUG
//#define _FILELOG
#define PAGE_BIT 12
#define MAXN_PAGE 64

class Tree
{
private:
	struct node
	{
		struct node* ls;
		struct node* rs;
		int siz,data,height;
	};

	typedef struct node stu;
	typedef struct node* ptr;

	ptr root;

	inline int h(ptr now)
	{
		if (now==NULL) return 0;
		return now->height;
	}

	inline int size(ptr now)
	{
		if (now==NULL) return 0;
		return now->siz;
	}

	inline void pushup(ptr now)
	{
		if (now==NULL) return;
		now->siz=1+size(now->ls)+size(now->rs);
		now->height=1+std::max(h(now->ls),h(now->rs));
		return;
	}

	inline void left(ptr* now)
	{
		ptr tmp=(*now)->rs; (*now)->rs=tmp->ls;
		tmp->ls=*now; tmp->siz=(*now)->siz;
		pushup(*now),pushup(tmp);
		*now=tmp; return;
	}

	inline void right(ptr* now)
	{
		ptr tmp=(*now)->ls; (*now)->ls=tmp->rs;
		tmp->rs=*now; tmp->siz=(*now)->siz;
		pushup(*now),pushup(tmp);
		*now=tmp; return;
	}

	inline void balance(ptr* now)
	{
		if (*now==NULL) return;
		if (h((*now)->ls)-h((*now)->rs)==2)
		{
			if (h((*now)->ls->ls)>h((*now)->ls->rs)) right(now);
			else left(&(*now)->ls),right(now);
			return;
		}
		if (h((*now)->rs)-h((*now)->ls)==2)
		{
			if (h((*now)->rs->rs)>h((*now)->rs->ls)) left(now);
			else right(&(*now)->rs),left(now);
			return;
		}
		return;
	}

	void ins(ptr* now,int num)
	{
		if (*now==NULL)
		{
			*now=(ptr)malloc(sizeof(stu));
			(*now)->ls=(*now)->rs=NULL;
			(*now)->siz=1,(*now)->data=num;
			(*now)->height=0; return;
		}
		else if ((*now)->data>num) ins(&(*now)->ls,num);
		else if ((*now)->data<num) ins(&(*now)->rs,num);
		else return;
		pushup(*now); balance(now); return;
	}

	int nxt(ptr now,int num,int ans)
	{
		if (now->data<=num)
		{
			if (now->rs==NULL) return ans;
			return nxt(now->rs,num,ans);
		}
		if (now->ls==NULL) return now->data;
		return nxt(now->ls,num,now->data);
	}

	void clear(ptr now)
	{
		if (now==NULL) return;
		clear(now->ls);
		clear(now->rs);
		free(now);
		return;
	}
public:
	inline Tree()
	{
		root=NULL;
		return;
	}

	inline void ins(int x)
	{
		ins(&root,x);
		return;
	}

	inline int nxt(int x)
	{
		return nxt(root,x,0x7fffffff);
	}

	inline ~Tree()
	{
		clear(root);
		return;
	}
};

int top,input[1<<23];
std::unordered_map<int,Tree> opt;
extern void pageReplace(long[],long);

FILE* in; FILE* out;
double tot; clock_t start,finish;
long tstmem[MAXN_PAGE],tstbak[MAXN_PAGE]; int tstscore;
long stdmem[MAXN_PAGE],stdbak[MAXN_PAGE]; int stdscore;

inline void dataInit()
{
	char* buffer=(char*)malloc(1<<26);
	int n=fread(buffer,1,1<<26,in);
	for (int i=0,cur=0;i<n;i++)
	{
		if ('0'<=buffer[i] && buffer[i]<='9')
			cur=(cur<<3)+(cur<<1)+(buffer[i]^48);
		else if (cur)
		{
			#ifdef _DEBUG
				n<<=12;
			#endif
			input[top]=cur;
			std::unordered_map<int,Tree>::iterator find=opt.find(cur>>12);
			if (find==opt.end())
			{
				Tree add;
				opt[cur>>12]=add;
			}
			opt[cur>>12].ins(top);
			top++,cur=0;
		}
		if (buffer[i]=='-') break;
	}
	free(buffer); return;
}

inline void pushUp(int n,long mem[],long bak[],int &score,bool vis)
{
	int cnt=0;
	bool flag=false;
	for (int i=0;i<MAXN_PAGE && !flag;i++)
		if (mem[i]>0 && mem[i]==n)
			flag=true;
	#ifdef _DEBUG
		if (vis)
		{
			for (int i=0;i<MAXN_PAGE;i++) printf("%ld ",mem[i]);
			printf(flag?"\ntrue\n":"\nfalse\n");
			printf("old score is %d\n",score);
		}
	#endif
	if (!flag)
	{
		#ifdef _DEBUG
			if (vis) printf("too low for %d\n",n);
		#endif
		#ifdef _FILELOG
			if (vis) fprintf(out,"too low for %d\n",n);
		#endif
		exit(0);
	}
	#ifdef _DEBUG
		if (vis)
		{
			for (int i=0;i<MAXN_PAGE;i++) printf("%ld ",bak[i]); puts("");
			for (int i=0;i<MAXN_PAGE;i++) printf("%ld ",mem[i]); puts("");
		}
	#endif
	for (int i=0;i<MAXN_PAGE;i++)
		if (mem[i]-bak[i])
		{
			#ifdef _DEBUG
				if (vis) printf("at index %d\n",i);
			#endif
			cnt++;
		}
	#ifdef _DEBUG
		if (vis) printf("cnt is %d\n",cnt);
	#endif
	score+=cnt?(cnt<<1|1):0;
	#ifdef _DEBUG
		if (vis) printf(cnt?"%s%d\n":"no %s%d\n","change for ",n);
	#endif
	#ifdef _FILELOG
		if (vis) fprintf(out,cnt?"%s%d\n":"no %s%d\n","change for ",n);
	#endif
	#ifdef _DEBUG
		if (vis)
		{
			printf("new score is %d\n",score);
			puts("\n------------\n");
		}
	#endif
	return;
}

inline void standardReplace(long physic_memery[],long nwAdd,int idx)
{
	for (int i=0;i<MAXN_PAGE;i++)
		if ((nwAdd>>12)==physic_memery[i])
			return;
	for (int i=0;i<MAXN_PAGE;i++)
		if (physic_memery[i]==0)
		{
			physic_memery[i]=nwAdd>>12;
			return;
		}
	int max=-1,replace=-1;
	for (int i=0;i<MAXN_PAGE;i++)
	{
		int temp=opt[physic_memery[i]].nxt(idx);
		if (temp>max) max=temp,replace=i;
	}
	physic_memery[replace]=nwAdd>>12;
	return;
}

int main(int argc,char const *argv[])
{
	if (argc<3) return -1;
	in=fopen(argv[1],"r");
	out=fopen(argv[2],"w");
	if (in==NULL) return -1;
	if (out==NULL) return -1;
	dataInit();
	for (int i=0;i<top;i++)
	{
		memcpy(tstbak,tstmem,sizeof(tstmem));
		start=clock(); pageReplace(tstmem,input[i]); finish=clock();
		memcpy(stdbak,stdmem,sizeof(stdmem));
		standardReplace(stdmem,input[i],i);
		#ifdef _WIN64
			tot+=(((double)finish-(double)start)/1e3);
		#else
			tot+=(((double)finish-(double)start)/1e6);
		#endif
		#ifdef _DEBUG
			printf("loop %d\n",i);
		#endif
		pushUp(input[i]>>PAGE_BIT,tstmem,tstbak,tstscore,true);
		pushUp(input[i]>>PAGE_BIT,stdmem,stdbak,stdscore,false);
	}
	fprintf(out,"\nrun time is:\n%lf\nrun score is:\n%lf\n",
		tot,1.0*tstscore/stdscore);
	printf("%lf %lf\n",tot,1.0*tstscore/stdscore);
	fclose(in); fclose(out);
	return 0;
}