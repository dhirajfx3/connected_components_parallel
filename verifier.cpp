#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#define hash hash1
char S[100000000];
int hash(long long val,int h,int k,int pr)
{
	val+=h;
	long long ans=1;
	ans%=pr;
	val%=pr;
	while(k)
	{
		if(k&1)
		{
			ans*=val;
			ans%=pr;
		}
		val*=val;
		val%=pr;
		k>>=1;
	}
	return ans;
}
int main(int argc,char **argv)
{
	/*
		argv[1]= output components file 1
		argv[2]= output connected components file 2
		output:
		Results match ! if
			Whether the set of connected components of both graphs are same
		otherwise
			Results do not match
	*/
	if(argc!=3)
	{
		printf("Invalid arguments!");
		exit(-1);
	}
	long long p1=60000607,p2= 333319999;
	fstream fs(argv[1],ios::in);
	long long h1=0,h2=0;
	while(true)
	{
		string S;
		if(!getline(fs,S))
			break;
		long long ch=0;
		S.push_back('p');
		int n=0;
		while(n<S.length())
		{
			int m = 0;
			char g = S[n++];
			while (n<S.length() && g<'0' || g>'9')
			g = S[n++];
			while (n<S.length() && g <= '9'&& g >= '0')
			{
				m = (m << 1) + (m << 3) + g - 48;
				g = S[n++];
			}
			if(m)
			{
				ch+=hash(m,4,20,p1);
			}
			ch%=p1;
		}
		if(ch)
			h1+=hash(ch,4,20,p1);
		h1%=p1;
	}
	fs.close();
	fs.open(argv[2],ios::in);
	while(true)
	{
		string S;
		if(!getline(fs,S))
			break;
		
		long long ch=0;
		S.push_back('p');
		int n=0;
		while(n<S.length())
		{
			int m = 0;
			char g = S[n++];
			while (n<S.length() && g<'0' || g>'9')
			g = S[n++];
			while (n<S.length() && g <= '9'&& g >= '0')
			{
				m = (m << 1) + (m << 3) + g - 48;
				g = S[n++];
			}
			if(m)
			{
				ch+=hash(m,4,20,p1);
			}
			ch%=p1;
		}
		if(ch)
			h2+=hash(ch,4,20,p1);
		h2%=p1;
	}
	if(h1==h2)
	{
		printf("Results match!\n");
		printf("Hash value %d\n",h1);
	}
	else
	{
		printf("Results do not match\n");
	}
	fs.close();
	return 0;
}