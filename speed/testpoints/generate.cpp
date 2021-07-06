#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

#define NDEBUG
#include <cassert>

#define RAN_TIME 6
#define MAXN 50000

#define TOTAL 6
#define RANDOM 0
#define LINEAR 1
#define TRIANGEL_SIN 2
#define TRIANGEL_COS 3
#define TRIANGEL_LEFT 4
#define TRIANGEL_RIGHT 5

std::mt19937 generator(0x19260817);
const int pool[]={36,72,108,180,432};

int fpr; FILE* out;

inline int rand(long long l,long long r)
{
	if (l==r) return l;
	long long num=generator();
	return (int)((num%(r-l+1)+r-l+1)%(r-l+1)+l);
}

inline void nextFile()
{
	if (out) fputs("-1\n",out),fclose(out);
	std::string name="testpoint"+std::to_string(fpr++)+".stdin";
	out=fopen(name.c_str(),"w"); return;
}

class Block
{
private:
	int size;
	std::vector<int> addr;

	inline void getRandom()
	{
		for (int i=0,mode=generator()&1?128:generator()&1?256:512;i<size;i++)
		{
			int toPush=(rand(1,mode)<<10)+4096;
			assert(toPush>4095); addr.push_back(toPush);
		}
		return;
	}

	inline void getLinear()
	{
		for (int i=0,mode=rand(1,128),val=rand(1,5);i<size;i++)
		{
			int toPush=((i*val+mode)<<9)+rand(0,512)+4096;
			assert(toPush>4095); addr.push_back(toPush);
		}
		return;
	}

	inline void getSin()
	{
		for (int i=0,mode=rand(1,128),val=rand(512,1024);i<size;i++)
		{
			int src=(int)((sin((1.0*i/size-0.5)*acos(-1))+1)*val)+mode;
			int toPush=(src<<9)+rand(0,512)+4096;
			assert(toPush>4095); addr.push_back(toPush);
		}
		return;
	}

	inline void getCos()
	{
		for (int i=0,mode=rand(1,128),val=rand(512,1024);i<size;i++)
		{
			int src=(int)((cos((1.0*i/size-0.5)*acos(-1))+1)*val)+mode;
			int toPush=(src<<9)+rand(0,512)+4096;
			assert(toPush>4095); addr.push_back(toPush);
		}
		return;
	}

	inline void getLeft()
	{
		for (int i=0,mode=rand(1,128),val=rand(512,1024);i<size;i++)
		{
			int src=(int)(sin((1.0*i/size)/2*acos(-1))*val)+mode;
			int toPush=(src<<9)+rand(0,512)+4096;
			assert(toPush>4095); addr.push_back(toPush);
		}
		return;
	}

	inline void getRight()
	{
		for (int i=0,mode=rand(1,128),val=rand(512,1024);i<size;i++)
		{
			int src=(int)((1-cos((1.0*i/size)/2*acos(-1)))*val)+mode;
			int toPush=(src<<9)+rand(0,512)+4096;
			assert(toPush>4095); addr.push_back(toPush);
		}
		return;
	}
public:
	inline int getSize() {return size;}
	inline ~Block() {addr.clear(); return;}
	inline int& operator[](int i) {return addr[i];}
	inline void reverse() {std::reverse(addr.begin(),addr.end());}

	inline Block(int size,int type)
	{
		this->size=size;
		if (type==RANDOM) getRandom();
		else if (type==LINEAR) getLinear();
		else if (type==TRIANGEL_SIN) getSin();
		else if (type==TRIANGEL_COS) getCos();
		else if (type==TRIANGEL_LEFT) getLeft();
		else if (type==TRIANGEL_RIGHT) getRight();
		else assert(0);
		return;
	}

	inline Block(Block &in1,Block &in2)
	{
		int i=0,j=0;
		while (i<in1.getSize() && j<in2.getSize())
		{
			if (rand(1,in1.getSize()+in2.getSize())<in1.getSize())
				addr.push_back(in1[i++]);
			else addr.push_back(in2[j++]);
		}
		for (;i<in1.size;addr.push_back(in1[i++]));
		for (;j<in2.size;addr.push_back(in2[j++]));
		size=in1.getSize()+in2.getSize();
		return;
	}

	inline void print()
	{
		for (int i=0;i<size;i++)
		printf("%d -> %d\n",addr[i],addr[i]>>12);
		puts("\n---------\n");
		return;
	}

	inline void print(FILE* file)
	{
		for (int i=0;i<size;i++)
		fprintf(file,"%d\n",addr[i]);
		return;
	}
};

inline void oneNormal(int type,int len)
{
	nextFile();
	for (int i=0;i<MAXN;i+=len)
	{
		Block a(len,type);
		a.print(out);
	}
	return;
}

inline void inDouble(int type,int len)
{
	nextFile();
	for (int i=0;i<MAXN;i+=len<<1)
	{
		Block a(len,type);
		a.print(out);
		a.reverse();
		a.print(out);
	}
	return;
}

inline void outDouble(int type,int len)
{
	nextFile();
	for (int i=0;i<MAXN;i+=len<<1)
	{
		Block a(len,type);
		Block b=a;
		b.reverse();
		Block c(a,b);
		c.print(out);
	}
	return;
}

inline void normalRandom(int len)
{
	nextFile();
	for (int i=0;i<MAXN;i+=len)
	{
		Block a(len,generator()%TOTAL);
		if (generator()&1) a.reverse();
		a.print(out);
	}
	return;
}

inline void mixRandom(int len)
{
	nextFile();
	for (int i=0;i<MAXN;i+=len<<1)
	{
		Block a(len,generator()%TOTAL);
		if (generator()&1) a.reverse();
		Block b(len,generator()%TOTAL);
		if (generator()&1) b.reverse();
		Block c(a,b); c.print(out);
	}
	return;
}

inline void test(int len)
{
	for (int i=0;i<TOTAL;i++) oneNormal(i,len);
	//for (int i=0;i<TOTAL;i++) inDouble(i,len);
	//for (int i=0;i<TOTAL;i++) outDouble(i,len);
	//for (int i=0;i<RAN_TIME;i++) normalRandom(len);
	//for (int i=0;i<RAN_TIME;i++) mixRandom(len);
	puts("done"); return;
}

int main(int argc,char const *argv[])
{
	if (argc>1) sscanf(argv[1],"%d",&fpr);
	for (unsigned int i=0;i<(sizeof(pool)>>2);test(pool[i++]))
	printf("%d/%ld started\n",i+1,sizeof(pool)>>2);
	if (out) fputs("-1\n",out),fclose(out);
	puts("\n^^^finished^^^\n"); return 0;
}