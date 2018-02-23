#include<iostream>
#include <vector>
#include <map>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <fstream>
#include <cstring>
using namespace std;
#define map unordered_map
#define ui uniform_int_distribution<int>
int main(int argc,char **argv)
{
	/*
		argv[1]= nodes count
		argv[2]= edges count
		argv[3]= output file with extension
		output : A file containing a graph in the format :
			A single line with first integer 'n'
			second integer 'm' then m pairs of integers denoting edges
	*/
	char file[50]="input.txt";
	if(argc>3)
	{
		strcpy(file,argv[3]);
	}
	if(argc<3)
	{
		freopen("log.txt","w",stdout);
		printf("3 Arguments expected provided %d...",argc-1);
		exit(-1);
	}
	int n=atoi(argv[1]),m=atoi(argv[2]);
	FILE* fs=freopen(file,"w",stdout);
	map<int,map<int,int> > Mp;
	ui U[3]={ui(1,n/3),ui(n/3+1,2*n/3),ui(2*n/3+1,n)};
	ui Ty(0,2);
	default_random_engine R;
	printf("%d %d ",n,m);
	for(int j=0;j<m;j++)
	{
		int rt=Ty(R);
		int a=U[rt](R),b=U[rt](R);
		if(a==b || Mp[min(a,b)].find(max(a,b))!=Mp[min(a,b)].end())
			--j;
		else
			printf("%d %d ",a,b),Mp[min(a,b)][max(a,b)]=1;
	}
	fclose(fs);
	return 0;
}