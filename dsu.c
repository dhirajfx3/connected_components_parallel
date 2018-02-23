#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#pragma GCC target("sse4,avx")
#pragma GCC optimize("unroll-loops")
#define getn S[df++]
#define gets(S) fgets(S,200000000-1,stdin)
char S[200000000];
int df=0;
int *dsu,*nex,*id,*linked_list,fill=0;
int readInt()
{
	int m = 0;
	char g = getn;
	while (g<'0' || g>'9')
		g = getn;
	while (g <= '9'&& g >= '0')
	{
		m = (m << 1) + (m << 3) + g - 48;
		g = getn;
	}
	return m;
}
int find_set(int L)
{
	int L0=L;
	while(dsu[L]!=L)
	{
		L=dsu[L];
	}
	while(dsu[L0]!=L0)
	{
		int c=dsu[L0];
		dsu[L0]=L;
		L0=c;
	}
	return L0;
}
void unite(int p,int q)
{
	int a=find_set(p),b=find_set(q);
	if(a!=b)
		dsu[a]=b;
}
int is_connected(int a,int b)
{
	return find_set(a)==find_set(b);
}
int get_new()
{
	return fill++;
}
void set_val(int idx,int val)
{
	id[idx]=val;
}
void add(int node,int list_no)
{
	if(linked_list[list_no]==-1)
	{
		linked_list[list_no]=get_new();
		set_val(linked_list[list_no],node);
	}
	else
	{
		int N=get_new();
		nex[N]=linked_list[list_no];
		linked_list[list_no]=N;
		set_val(N,node);
	}
}
int main(int argc,char** argv)
{
	/*
	argv[1]= input file 
	argv[2]= output file
	argv[3]= logfile
	input file format
	A single line of input first integer 'n'
	second integer 'm' then m pairs of integers denoting edges
	output : Connected components in each line
	*/
	char file[] = "C:\\Users\\Xenor\\Documents\\sample\\parallel\\inp.txt";
	char file2[] = "C:\\Users\\Xenor\\Documents\\sample\\parallel\\dsuout.txt";
	char file3[] = "C:\\Users\\Xenor\\Documents\\sample\\parallel\\dsulog.txt";
	if (argc>1)
		strcpy(file,argv[1]);
	if(argc>2)
		strcpy(file2,argv[2]);
	if(argc>3)
		strcpy(file3,argv[3]);
	clock_t Input_time,Comp_time,Tot_exec_time,Output_time;
	FILE* fs=freopen(file,"r",stdin);
	Input_time=clock();
	Tot_exec_time=Input_time;
	gets(S);
	Comp_time=clock();
	Input_time=Comp_time-Input_time;
	df=0;
	int n=readInt(),m=readInt();
	
	dsu=(int*)malloc(sizeof(int)*n);
	nex=(int*)malloc(sizeof(int)*n);
	id=(int*)malloc(sizeof(int)*n);
	linked_list=(int*)malloc(sizeof(int)*n);
	memset((void*)linked_list,255,n*sizeof(int));
	memset((void*)nex,255,n*sizeof(int));
	for(int i=0;i<n;i++)
		dsu[i]=i;
	for(int i=0;i<m;i++)
		unite(readInt()-1,readInt()-1);
	for(int i=0;i<n;i++)
	{
		add(i,find_set(i));
	}
	Output_time=clock();
	FILE* fout=fopen(file2,"w");
	Comp_time=Output_time-Comp_time;
	for(int i=0;i<n;i++)
	{
		int f=linked_list[i];
		if(f!=-1)
		{
			while(f!=-1)
			{
				fprintf(fout,"%d ",id[f]+1);
				f=nex[f];
			}
			fprintf(fout,"\n");
		}
	}
	fclose(fout);
	clock_t temp=clock();
	Output_time=temp-Output_time;
	Tot_exec_time=temp-Tot_exec_time;
	FILE* fst=fopen(file3,"a");
	fprintf(fst,"-------------------------------------------------------------\n");
	time_t end_time;
	time(&end_time);
	fprintf(fst, "Finished computation at : %s", ctime(&end_time));
	fprintf(fst,"Input time = %.3fms\n",(Input_time*1.0/CLOCKS_PER_SEC)*1000.0);
	fprintf(fst,"Computation time = %.3fms\n",(Comp_time*1.0/CLOCKS_PER_SEC)*1000.0);
	fprintf(fst,"Output time = %.3fms\n",(Output_time*1.0/CLOCKS_PER_SEC)*1000.0);
	fprintf(fst,"Total execution time = %.3fms\n",(Tot_exec_time*1.0/CLOCKS_PER_SEC)*1000.0);
	fclose(fst);	
	return 0;
}