#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <omp.h>
#include <vector>
#pragma GCC target("sse4,avx")
#pragma GCC optimize("unroll-loops")
#define getn S[df++]
char *S;
int *tempE,*rE,*c;

int df=0;
int *Li,*Ri,*dsu,*nex,*id,*linked_list,fill=0,n,m,*fin_table;
int db=-1, cpu_threads=4, gpu_threads=3;
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
void countingSort(int *A) 
{ 
	int i, sum, maxi = max(n,m);
	memset(c, 0, (max(n,m))*sizeof(int)); 
	for (i = 0; i < m; i++) 
		c[A[i]]++;
	for (i = sum = 0; i < maxi; i++)
	{
		int t = c[i]; c[i] = sum; sum += t; 
	}
	for (i = 0; i < m; i++) 
		tempE[c[A[rE[i]]]++] = rE[i];
	for (i = 0; i < m; i++)
		rE[i] = tempE[i];
}
void sort_linear()
{
	rE=(int*)malloc(sizeof(int)*(max(m,n)));
	tempE=(int*)malloc(sizeof(int)*(max(m,n)));
	int z=n;
	if(m>z)
	z=m;
	c=(int*)malloc(sizeof(int)*(max(n,m)));
	for(int i=0;i<m;i++)
	rE[i]=i;
	countingSort(Ri);
	countingSort(Li);
	

	for(int i=0;i<m;i++)
	{
		c[i]=Li[rE[i]];
		tempE[i]=Ri[rE[i]];
	}
	for(int i=0;i<m;i++)
		Li[i]=c[i],Ri[i]=tempE[i];
}
int find_set(int L)
{
	if(dsu[L]==L)
		return L;
	 dsu[L]=find_set(dsu[L]);
	return dsu[L];
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
void read_input(char *file_name)
{
	df=0;
	FILE *f = fopen(file_name, "r");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET); 
	S =(char*) malloc(fsize + 1);
	fread(S, fsize, 1, f);
	fclose(f);
	S[fsize] = 0;
	
}
 int find_set_serial(int L,int offset, int *dsu)
{
	int L0=L;
	while(dsu[L-offset]!=L)
	{
		L=dsu[L-offset];
	}
	while(dsu[L0-offset]!=L0)
	{
		int c=dsu[L0-offset];
		dsu[L0-offset]=L;
		L0=c;
	}
	return L0;
}
 void unite_serial(int p,int q,int offset, int *dsu)
{	

	int a=find_set_serial(p,offset,dsu),b=find_set_serial(q,offset,dsu);
	if(a!=b)
		dsu[a-offset]=b;

}
__device__ int find_set(int L,int offset, int *dsu)
{
	int L0=L;
	while(dsu[L-offset]!=L)
	{
		L=dsu[L-offset];
	}
	while(dsu[L0-offset]!=L0)
	{
		int c=dsu[L0-offset];
		dsu[L0-offset]=L;
		L0=c;
	}
	return L0;
}
 __device__ void unite(int p,int q,int offset, int *dsu)
{	

	int a=find_set(p,offset,dsu),b=find_set(q,offset,dsu);
	if(a!=b)
		dsu[a-offset]=b;

}
__global__ void connected_components( int *dsu, int *EL, int *ER,
		 int n, int m, int Li, int Ri) 
{
	 int ID = blockIdx.x;
	 int sz = gridDim.x;
	 int eli=((Ri-Li+1)/sz+1)*ID+Li;
	 if(eli>Ri)
		return;
	 int endi=eli+(Ri-Li+1)/sz;
	 if(endi>Ri)
		endi=Ri;
	int a=0,b=m-1;
	/*while(b-a>1)
	{
		int mid=(a+b)/2;
		if(EL[mid]<eli)
		{
			a=mid;
		}
		else
			b=mid;
	}
*/
	 for(int i=a;i<m;i++)
	{
		int li=EL[i],ri=ER[i];
		if(eli<=li && ri<=endi && li!=ri)
		{
			unite(li,ri,Li,dsu);
		}
	/*	if(li>endi)
			break;*/
	}
}
int *device_dsu,*dev_EL,*dev_ER;
void mem_init_cuda(int start_idx,int end_idx)
{
	cudaMalloc((void **) &device_dsu, (end_idx-start_idx+1)*sizeof(int));
	cudaMalloc((void **) &dev_EL, m*sizeof(int));
	cudaMalloc((void **) &dev_ER, m*sizeof(int));
	
	cudaMemcpy(device_dsu, dsu+start_idx, (end_idx-start_idx+1)*sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_EL, Li, m*sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(dev_ER, Ri, m*sizeof(int), cudaMemcpyHostToDevice);
}
void mem_release_cuda(int start_idx,int end_idx)
{
	cudaMemcpy(dsu+start_idx, device_dsu, (end_idx-start_idx+1)*sizeof(int), cudaMemcpyDeviceToHost);
	cudaFree(device_dsu);
	cudaFree(dev_EL);
	cudaFree(dev_ER);
}
void execute_on_gpu(int start_idx,int end_idx,int number_of_threads)
{
	connected_components<<<number_of_threads,1>>>(device_dsu,dev_EL,dev_ER,n,m,start_idx,end_idx);
	return ;
}
void execute_on_cpu(int start_idx,int end_idx,int number_of_threads)
{
int tid;
#pragma omp parallel for
 	for( tid=0;tid<number_of_threads;tid++)
	{
		int ID = tid;
		int sz = number_of_threads;
		int eli=((end_idx-start_idx+1)/sz+1)*ID+start_idx;
		 
		if(eli>end_idx)
			continue;
		int endi=eli+(end_idx-start_idx+1)/sz;
		
		if(endi>end_idx)
			endi=end_idx;
		int a=0,b=m-1;
		/* uncomment to turn on optimization
		while(b-a>1)
		{
			int mid=(a+b)/2;
			if(Li[mid]<eli)
			{
				a=mid;
			}
			else
				b=mid;
		}
		   */ 
		for(int i=a;i<m ;i++)
		{
			int li=Li[i],ri=Ri[i];
			if(eli<=li && ri<=endi && li!=ri)
			{
				unite_serial(li,ri,start_idx,dsu);
			}
		}
	}
}
void finish(int l,int z1,int z2);
int device_info();
void make_table(int,int,int);
int main(int argc,char** argv)
{
	/*
	argv[1]=gpu_threads
	argv[2]=cpu_threads
	argv[3]= input file 
	argv[4]= output file
	argv[5]= logfile
	input file format
	A single line of input first integer 'n'
	second integer 'm' then m pairs of integers denoting edges
	output : Connected components in each line
	*/
	gpu_threads=4096;
	cpu_threads=32;
		if(argc>1)	
	gpu_threads=atoi(argv[1]);
	if(argc>2)
		cpu_threads=atoi(argv[2]);
	char file[] = "input.txt";
	char file2[] = "Pdsuout.txt";
	char file3[] = "Pdsulog.txt";
	if (argc>3)
		strcpy(file,argv[3]);
	if(argc>4)
		strcpy(file2,argv[4]);
	if(argc>5)
		strcpy(file3, argv[5]);
	clock_t Input_time, Comp_time, Tot_exec_time, Output_time;
	Input_time = clock();
	read_input(file);
	Tot_exec_time = Input_time;
	Comp_time=clock();
	Input_time=Comp_time - Input_time;
	 n=readInt(), m=readInt();
	dsu=(int*)malloc(sizeof(int)*n);
	nex=(int*)malloc(sizeof(int)*n);
	id=(int*)malloc(sizeof(int)*n);
	fin_table=(int*)malloc(sizeof(int)*n);
	Li=(int*)malloc(sizeof(int)*m);
	Ri=(int*)malloc(sizeof(int)*m);
	linked_list=(int*)malloc(sizeof(int)*n);
	memset((void*)linked_list,255,n*sizeof(int));
	memset((void*)nex,255,n*sizeof(int));
	for(int i=0;i<n;i++)
		dsu[i]=i;
	for(int i=0;i<m;i++)
	{
		Li[i]=readInt()-1;
		Ri[i]=readInt()-1;
		if(Li[i]>Ri[i])
		{
		// swapping values
			Li[i]^=Ri[i];
			Ri[i]^=Li[i];
			Li[i]^=Ri[i];
		}
	}
		
//	sort_linear();
fprintf(stderr,"ok %d %d",n,m);
	db=n/3;
	mem_init_cuda(db+1,n-1);
	make_table(db,cpu_threads,gpu_threads);
	Comp_time=clock();
	execute_on_gpu(db+1, n - 1, gpu_threads);
	execute_on_cpu(0,db, cpu_threads);
	cudaDeviceSynchronize();
	mem_release_cuda(db+1,n-1);
	finish(db,cpu_threads,gpu_threads);
	Output_time=clock();
	mem_release_cuda(db+1,n-1);
	for(int i=0;i<n;i++)
	{
		add(i,find_set(i));
	}
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
	fprintf(fst, "Nodes : %d, Edges : %d\n",n,m);
	fprintf(fst,"Input time = %.3fms\n",(Input_time*1.0/CLOCKS_PER_SEC)*1000.0);
	fprintf(fst,"Computation time = %.3fms\n",(Comp_time*1.0/CLOCKS_PER_SEC)*1000.0);
	fprintf(fst,"Output time = %.3fms\n",(Output_time*1.0/CLOCKS_PER_SEC)*1000.0);
	fprintf(fst,"Total execution time = %.3fms\n",(Tot_exec_time*1.0/CLOCKS_PER_SEC)*1000.0);
	fclose(fst);	
	return 0;
}
void make_table(int l,int z1,int z2)
{
	int ft = (l + 1) / z1 + 1, ft2 = (n - 1 - l) / z2 + 1;
	int l0=0,j=0,l1=0;
	while(j<=l)
	{
		fin_table[j]=l1;
		++l0;
		if(l0>=ft)
		{
			l0=0;
			l1++;
		}
		++j;
	}
	j=l+1;
	l1=0;
	l0=0;
	while(j<n)
	{
		fin_table[j]=l1;
		++l0;
		if(l0>=ft2)
		{
			l0=0;
			l1++;
		}
		++j;
	}
}
void finish(int l, int z1, int z2)
{
	// first cpu then gpu nodes
	// z1,z2 cpu and gpu threads
	
	for (int i = 0; i < m; i++)
	{
		if (Ri[i] <= l)
		{
			if (fin_table[Li[i]] != fin_table[Ri[i]])
				unite(Li[i], Ri[i]);
		}
		else if(Li[i]>l)
		{
			if (fin_table[Li[i]]!= fin_table[Ri[i]])
				unite(Li[i], Ri[i]);
		}
	//	else
			unite(Li[i], Ri[i]);
	}
}
void printDevProp(cudaDeviceProp devProp)
{
    printf("Major revision number:         %d\n",  devProp.major);
    printf("Minor revision number:         %d\n",  devProp.minor);
    printf("Name:                          %s\n",  devProp.name);
    printf("Total global memory:           %u\n",  devProp.totalGlobalMem);
    printf("Total shared memory per block: %u\n",  devProp.sharedMemPerBlock);
    printf("Total registers per block:     %d\n",  devProp.regsPerBlock);
    printf("Warp size:                     %d\n",  devProp.warpSize);
    printf("Maximum memory pitch:          %u\n",  devProp.memPitch);
    printf("Maximum threads per block:     %d\n",  devProp.maxThreadsPerBlock);
    for (int i = 0; i < 3; ++i)
    printf("Maximum dimension %d of block:  %d\n", i, devProp.maxThreadsDim[i]);
    for (int i = 0; i < 3; ++i)
    printf("Maximum dimension %d of grid:   %d\n", i, devProp.maxGridSize[i]);
    printf("Clock rate:                    %d\n",  devProp.clockRate);
    printf("Total constant memory:         %u\n",  devProp.totalConstMem);
    printf("Texture alignment:             %u\n",  devProp.textureAlignment);
    printf("Concurrent copy and execution: %s\n",  (devProp.deviceOverlap ? "Yes" : "No"));
    printf("Number of multiprocessors:     %d\n",  devProp.multiProcessorCount);
    printf("Kernel execution timeout:      %s\n",  (devProp.kernelExecTimeoutEnabled ? "Yes" : "No"));
    return;
}
 
int device_info()
{
    // Number of CUDA devices
    int devCount;
    cudaGetDeviceCount(&devCount);
    printf("CUDA Device Query...\n");
    printf("There are %d CUDA devices.\n", devCount);
 
    // Iterate through devices
    for (int i = 0; i < devCount; ++i)
    {
        // Get device properties
        printf("\nCUDA Device #%d\n", i);
        cudaDeviceProp devProp;
        cudaGetDeviceProperties(&devProp, i);
        printDevProp(devProp);
    } 
    return 0;
}
