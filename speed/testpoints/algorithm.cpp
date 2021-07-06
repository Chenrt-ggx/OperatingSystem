#include <set>
#include <cstdio>
#include <vector>
#include <random>
#define MAXN 20000

int fpr; FILE* out;
std::vector<int> visit;
std::mt19937 generator(19260817);

inline int rand(long long l,long long r)
{
	if (l==r) return l;
	long long num=generator();
	return (int)((num%(r-l+1)+r-l+1)%(r-l+1)+l);
}

inline void nextFile(bool isEnd)
{
	if (out)
	{
		std::set<int> pos;
		for (int i=0;i<100;i++)
		{
			int add=rand(0,visit.size()-1);
			while (pos.find(add)!=pos.end())
			add=rand(0,visit.size()-1);
			pos.insert(add);
		}
		fprintf(out,"%d\n",(1<<30)+(rand(0,5)<<12));
		for (int i=0;i<(int)visit.size();i++)
		{
			if (pos.find(i)!=pos.end())
			{
				int tot=rand(20,100);
				while (tot--) fprintf(out,"%d\n",(1<<30)+(rand(0,5)<<12));
			}
			fprintf(out,"%d\n",visit[i]);
		}
		visit.clear();
		fputs("-1\n",out);
		fclose(out);
	}
	if (isEnd) return;
	std::string name="testpoint"+std::to_string(fpr++)+".stdin";
	out=fopen(name.c_str(),"w"); return;
}

template<class T> class VisiableArray
{
private:
	int base,val;
	std::vector<T> data;
public:
	inline VisiableArray() {base=4096+rand(1,512);}
	inline void add(T obj) {data.push_back(obj); return;}

	inline T& operator[](int i)
	{
		visit.push_back(i*val+base);
		return data[i];
	}

	inline void reset(int size)
	{
		data.clear();
		val=32*MAXN/size;
		base=4096+rand(1,512);
	}

	inline void reset(int size,int n)
	{
		data.clear();
		data.resize(n);
		val=256*MAXN/size;
		base=4096+rand(1,512);
	}
};

class RealSingle
{
private:
	VisiableArray<int> a;
	inline int ls(int x) {return x<<1;}
	inline int rs(int x) {return x<<1|1;}
	inline void sum(int x) {for (int ans=0;x;ans+=a[x],x-=x&-x); return;}
	inline void add(int x,int k,int n) {for (;x<=n;a[x]+=k,x+=x&-x); return;}

	inline void bubblesort(int n,int f)
	{
		for (int i=0;i<n;i++)
		for (int j=0;j<n-i-1;j++)
		if (a[j]>a[j+1]) std::swap(a[j],a[j+1]);
		return;
	}

	inline void selectsort(int n,int f)
	{
		for (int i=n,pos;i;i--)
		{
			f=0x80000000;
			for (int j=0;j<i;j++)
			if (a[j]>f) f=a[j],pos=j;
			if (pos==i-1) continue;
			std::swap(a[pos],a[i-1]);
		}
		return;
	}

	inline void insertsort(int n,int f)
	{
		for (int i=1;i<n;i++)
		{
			for (int j=i-1;j>=0 && a[j]>a[j+1];j--)
			std::swap(a[j],a[j+1]);
		}
		return;
	}

	void quicksort(int l,int r)
	{
		if (l>r) return;
		int mid=(l+r)>>1;
		if (a[l]>a[r]) std::swap(a[l],a[r]);
		if (a[r]<a[mid]) std::swap(a[r],a[mid]);
		if (a[l]<a[mid]) std::swap(a[l],a[mid]);
		int i=l,j=r,tmp=a[l];
		while (i!=j)
		{
			while (a[j]>=tmp && i<j) j--;
			while (a[i]<=tmp && i<j) i++;
			if (i<j) std::swap(a[i],a[j]);
		}
		a[l]=a[i],a[i]=tmp,mid=j=i;
		while (a[i]==a[mid]) i--;
		while (a[j]==a[mid]) j++;
		quicksort(l,i),quicksort(j,r);
		return;
	}

	inline void siftdown(int idx,int size)
	{
		for (int t;ls(idx)<=size;)
		{
			if (a[idx]<a[ls(idx)]) t=ls(idx); else t=idx;
			if (rs(idx)<=size && a[t]<a[rs(idx)]) t=rs(idx);
			if (t-idx) std::swap(a[idx],a[t]),idx=t;
			else return;
		}
		return;
	}

	inline void heapsort(int n,int f)
	{
		for (int i=n>>1;i;i--)
			siftdown(i,n);
		while (f-1)
		{
			std::swap(a[1],a[f--]);
			siftdown(1,f);
		}
		return;
	}

	inline void shellsort(int n,int f)
	{
		for (int gap=n>>1;gap;gap>>=1)
		for (int i=gap;i<n;i++)
		for (int j=i-gap;j>=0 && a[j]>a[j+gap];j-=gap)
		std::swap(a[j],a[j+gap]);
		return;
	}

	inline void merge(int l,int mid,int r,int n)
	{
		for (int i=l;i<=r;i++)
			a[n+i]=a[i];
		int i=l,j=mid+1,k=l;
		while (i<=mid || j<=r)
		{
			if (i>mid) a[k++]=a[n+j++];
			else if (j>r) a[k++]=a[n+i++];
			else if (a[n+i]>a[n+j]) a[k++]=a[n+j++];
			else a[k++]=a[n+i++];
		}
		return;
	}

	void mergesort(int l,int r,int n)
	{
		if (l>=r) return;
		int mid=(l+r)>>1;
		mergesort(l,mid,n);
		mergesort(mid+1,r,n);
		merge(l,mid,r,n);
		return;
	}
public:
	inline void bubblesort(int n)
	{
		nextFile(false); a.reset(n);
		for (int i=0;i<n;i++) a.add(generator());
		bubblesort(n,-1); return;
	}

	inline void selectsort(int n)
	{
		nextFile(false); a.reset(n);
		for (int i=0;i<n;i++) a.add(generator());
		selectsort(n,-1); return;
	}

	inline void insertsort(int n)
	{
		nextFile(false); a.reset(n);
		for (int i=0;i<n;i++) a.add(generator());
		insertsort(n,-1); return;
	}

	inline void quicksort(int n)
	{
		nextFile(false); a.reset(n);
		for (int i=0;i<n;i++) a.add(generator());
		quicksort(0,n-1); return;
	}

	inline void heapsort(int n)
	{
		nextFile(false); a.reset(n); a.add(0);
		for (int i=0;i<n;i++) a.add(generator());
		heapsort(n,n); return;
	}

	inline void shellsort(int n)
	{
		nextFile(false); a.reset(n);
		for (int i=0;i<n;i++) a.add(generator());
		shellsort(n,-1); return;
	}

	inline void mergesort(int n)
	{
		nextFile(false); a.reset(n);
		for (int i=0;i<n;i++) a.add(generator());
		for (int i=-5;i<n;i++) a.add(0);
		mergesort(0,n-1,n); return;
	}

	inline void treearray(int n)
	{
		nextFile(false); a.reset(n);
		for (int i=-5;i<n;i++) a.add(0);
		for (int i=1;i<=n;i++) add(i,generator(),n);
		for (int i=1,x,y;i<=n;i++)
		{
			x=rand(1,n),y=rand(1,n);
			if (x>y) std::swap(x,y);
			if (generator()&1) add(x,y,n);
			else sum(y),sum(x-1);
		}
		return;
	}
};

class TreapTest
{
private:
	struct Treap
	{
		int data,value;
		int left,right,siz;
	};

	int cnt=0,root=0;
	VisiableArray<Treap> t;

	inline void pushup(int x)
	{
		t[x].siz=t[t[x].left].siz+t[t[x].right].siz+1;
		return;
	}

	inline void right_rorate(int &now)
	{
		int tmp=t[now].left;
		t[now].left=t[tmp].right;
		t[tmp].right=now;
		t[tmp].siz=t[now].siz;
		pushup(now),now=tmp;
		return;
	}

	inline void left_rorate(int &now)
	{
		int tmp=t[now].right;
		t[now].right=t[tmp].left;
		t[tmp].left=now;
		t[tmp].siz=t[now].siz;
		pushup(now),now=tmp;
		return;
	}

	void ins(int &now,int data)
	{
		if (now==0)
		{
			now=++cnt,t[now].siz=1,t[now].data=data;
			t[now].value=generator(); return; 
		}
		t[now].siz++;
		if (data>=t[now].data) ins(t[now].right,data);
		else ins(t[now].left,data);
		if (t[now].left && t[now].value>t[t[now].left].value) right_rorate(now);
		if (t[now].right && t[now].value>t[t[now].right].value) left_rorate(now);
		pushup(now); return;
	}

	void del(int &now,int data)
	{
		if (now==0) return;
		else t[now].siz--;
		if (t[now].data==data)
		{
			if (t[now].left==0 && t[now].right==0) {now=0; return;}
			if (t[now].left==0 || t[now].right==0) {now=t[now].left+t[now].right; return;}
			if (t[t[now].left].value<t[t[now].right].value)
				{right_rorate(now); del(t[now].right,data); return;}
			else {left_rorate(now); del(t[now].left,data); return;}
		}
		if (t[now].data>=data) del(t[now].left,data);
		else del(t[now].right,data);
		pushup(now); return;
	}

	int rank(int now,int data)
	{
		if (now==0) return 0;
		if (data>t[now].data)
			return t[t[now].left].siz+1+rank(t[now].right,data);
		return rank(t[now].left,data);
	}

	int find(int now,int rank)
	{
		if (now==0) return 0;
		if (rank==t[t[now].left].siz+1) return t[now].data;
		if (rank>t[t[now].left].siz+1)
			return find(t[now].right,rank-t[t[now].left].siz-1);
		return find(t[now].left,rank);
	}

	int query_pre(int now,int data)
	{
		if (now==0) return 0;
		if (t[now].data>=data) return query_pre(t[now].left,data);
		int tmp=query_pre(t[now].right,data);
		if (tmp==0) return t[now].data;
		return tmp;
	}

	int query_suf(int now,int data)
	{
		if (now==0) return 0;
		if (t[now].data<=data) return query_suf(t[now].right,data);
		int tmp=query_suf(t[now].left,data);
		if (tmp==0) return t[now].data;
		return tmp;
	}
public:
	inline void test(int n)
	{
		nextFile(false); t.reset(n,n+5);
		for (int i=0,k;i<n;i++)
		{
			k=generator()%6;
			if (k==0) ins(root,rand(1,1000));
			else if (k==1) del(root,rand(1,1000));
			else if (k==2) rank(root,rand(1,1000));
			else if (k==3) find(root,rand(1,n));
			else if (k==4) query_pre(root,rand(1,1000));
			else if (k==5) query_suf(root,rand(1,1000));
		}
	}
};

class SplayTest
{
private:
	struct Splay
	{
		int ff,ch[2];
		int cnt,val,siz;
	};

	int tot=0,root=0;
	VisiableArray<Splay> t;

	inline int rand(long long l,long long r)
	{
		if (l==r) return l;
		long long num=generator();
		return (int)((num%(r-l+1)+r-l+1)%(r-l+1)+l);
	}

	inline void pushup(int u)
	{
		t[u].siz=t[t[u].ch[0]].siz+t[t[u].ch[1]].siz+t[u].cnt;
		return;
	}

	inline void rotate(int x)
	{
		int y=t[x].ff;
		int z=t[y].ff;
		int k=t[y].ch[1]==x;
		t[z].ch[t[z].ch[1]==y]=x;
		t[x].ff=z;
		t[y].ch[k]=t[x].ch[k^1];
		t[t[x].ch[k^1]].ff=y;
		t[x].ch[k^1]=y,t[y].ff=x;
		pushup(y),pushup(x);
		return;
	}

	inline void splay(int x,int goal)
	{
		while (t[x].ff!=goal)
		{
			int y=t[x].ff,z=t[y].ff;
			if (z!=goal)
				(t[y].ch[0]==x)^(t[z].ch[0]==y)?
				rotate(x):rotate(y);
			rotate(x);
		}
		if (goal==0) root=x;
		return;
	}

	inline void ins(int x)
	{
		int u=root,ff=0;
		while (u && t[u].val!=x)
			ff=u,u=t[u].ch[x>t[u].val];
		if (u) t[u].cnt++;
		else
		{
			u=++tot;
			if (ff) t[ff].ch[x>t[ff].val]=u;
			t[tot].ch[0]=0; t[tot].ch[1]=0;
			t[tot].ff=ff; t[tot].val=x;
			t[tot].cnt=1; t[tot].siz=1;
		}
		splay(u,0); return;
	}

	inline void find(int x)
	{
		int u=root; if (!u) return;
		while (t[u].ch[x>t[u].val] && x!=t[u].val)
			u=t[u].ch[x>t[u].val];
		splay(u,0); return;
	}

	inline int next(int x,int f)
	{
		find(x); int u=root;
		if ((t[u].val>x && f) || (t[u].val<x && !f)) return u;
		u=t[u].ch[f]; while (t[u].ch[f^1]) u=t[u].ch[f^1];
		return u;
	}

	inline void del(int x)
	{
		int lst=next(x,0),nxt=next(x,1);
		splay(lst,0),splay(nxt,lst);
		int del=t[nxt].ch[0];
		if (t[del].cnt>1) t[del].cnt--,splay(del,0);
		else t[nxt].ch[0]=0;
		return;
	}

	inline int rank(int x)
	{
		int u=root;
		if (t[u].siz<x) return -1;
		while (true)
		{
			int y=t[u].ch[0];
			if (x>t[y].siz+t[u].cnt)
			{
				x-=t[y].siz+t[u].cnt;
				u=t[u].ch[1];
			}
			else if (t[y].siz>=x) u=y;
			else return t[u].val;
		}
		return -1;
	}
public:
	inline void test(int n)
	{
		nextFile(false); t.reset(n,n+5);
		ins(0x80000000),ins(0x7fffffff);
		for (int i=0,k;i<n;i++)
		{
			k=generator()%6;
			if (k==0) ins(rand(1,1000));
			else if (k==1) del(rand(1,1000));
			else if (k==2) rank(rand(1,1000));
			else if (k==3) find(rand(1,n));
			else if (k==4) next(rand(1,1000),0);
			else if (k==5) next(rand(1,1000),1);
		}
	}
};

int main(int argc,char const *argv[])
{
	RealSingle a;
	if (argc>1) sscanf(argv[1],"%d",&fpr);
	a.bubblesort(MAXN*0.04); puts("bubblesort done");
	a.selectsort(MAXN*0.07); puts("selectsort done");
	a.insertsort(MAXN*0.05); puts("insertsort done");
	a.quicksort(MAXN*1.05); puts("quicksort done");
	a.heapsort(MAXN*0.67); puts("heapsort done");
	a.shellsort(MAXN*0.60); puts("shellsort done");
	a.mergesort(MAXN*0.62); puts("mergesort done");
	a.treearray(MAXN*2.45); puts("treearray done");
	//--------------------------------
	TreapTest b; b.test(MAXN*0.63); puts("treap done");
	SplayTest c; c.test(MAXN*0.20); puts("splay done");
	nextFile(true); return 0;
}