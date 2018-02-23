#pragma OPENCL EXTENSION cl_khr_int64_base_atomics:
// Add you device OpenCL code
int find_set(int L,__global int *dsu)
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
 void unite(int p,int q, __global int *dsu)
{
	int a=find_set(p,dsu),b=find_set(q,dsu);
	if(a!=b)
		dsu[a]=b;
}
__kernel void connected_components(__global int *dsu,__global int *EL,__global int *ER,__global int *n,__global int *m) 
{
	 int ID = get_global_id(0);
	 int sz = get_global_size(0);
	 int eli=(*n/sz+1)*ID;
	 if(eli>=n)
		return;
	 int endi=eli+*n/sz;
	 if(endi>=*n)
		endi=*n-1;
	//	printf("%d %d\n", get_local_id(0),ID);
//	printf("%d %d %d\n",eli,endi,sz);
	 for(int i=0;i<*m;i++)
	{
		int li=EL[i],ri=ER[i];
		if(eli<=li && ri<=endi)
		{
			if(find_set(li,dsu)!=find_set(ri,dsu))
			{
				unite(li,ri,dsu);
			}
		}
	}
}