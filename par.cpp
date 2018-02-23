#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#pragma warning(disable:4996)
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif
#define NUM_DATA 100

#define CL_CHECK(_expr)                                                         \
   do {                                                                         \
     cl_int _err = _expr;                                                       \
     if (_err == CL_SUCCESS)                                                    \
       break;                                                                   \
     fprintf(stderr, "OpenCL Error: '%s' returned %d!\n", #_expr, (int)_err);   \
     abort();                                                                   \
   } while (0)

#define CL_CHECK_ERR(_expr)                                                     \
   ({                                                                           \
     cl_int _err = CL_INVALID_VALUE;                                            \
     typeof(_expr) _ret = _expr;                                                \
     if (_err != CL_SUCCESS) {                                                  \
       fprintf(stderr, "OpenCL Error: '%s' returned %d!\n", #_expr, (int)_err); \
       abort();                                                                 \
     }                                                                          \
     _ret;                                                                      \
   })

#define MAX_SOURCE_SIZE (0x100000)
#define getn S[df++]
#define gets(S) fgets(S,200000000-1,stdin)
char S[200000000];
int df = 0;
#define min(a,b) a<b?a:b
#define max(a,b) a>b?a:b
int *dsu, *nex, *id, *linked_list, fill = 0, *EL, *ER;
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
	int L0 = L;
	while (dsu[L] != L)
	{
		L = dsu[L];
	}
	while (dsu[L0] != L0)
	{
		int c = dsu[L0];
		dsu[L0] = L;
		L0 = c;
	}
	return L0;
}
void unite(int p, int q)
{
	int a = find_set(p), b = find_set(q);
	if (a != b)
		dsu[a] = b;
}
int is_connected(int a, int b)
{
	return find_set(a) == find_set(b);
}
int get_new()
{
	return fill++;
}
void set_val(int idx, int val)
{
	id[idx] = val;
}
void add(int node, int list_no)
{
	if (linked_list[list_no] == -1)
	{
		linked_list[list_no] = get_new();
		set_val(linked_list[list_no], node);
	}
	else
	{
		int N = get_new();
		nex[N] = linked_list[list_no];
		linked_list[list_no] = N;
		set_val(N, node);
	}
}
int main(int argc, char** argv) {
	char file[]  = "C:\\Users\\Xenor\\Documents\\sample\\parallel\\sample.txt";
	char file2[] = "C:\\Users\\Xenor\\Documents\\sample\\parallel\\parout.txt";
	char file3[] = "C:\\Users\\Xenor\\Documents\\sample\\parallel\\parlog.txt";
	if (argc>1)
		strcpy(file, argv[1]);
	if (argc>2)
		strcpy(file2, argv[2]);
	if (argc>3)
		strcpy(file3, argv[3]);
	clock_t Input_time, Comp_time, Tot_exec_time, Output_time;
	FILE* fs = freopen(file, "r", stdin);
	Input_time = clock();
	Tot_exec_time = Input_time;
	gets(S);
	
	df = 0;
	int n = readInt(), m = readInt();

	dsu = (int*)malloc(sizeof(int)*n);
	nex = (int*)malloc(sizeof(int)*n);
	id = (int*)malloc(sizeof(int)*n);
	linked_list = (int*)malloc(sizeof(int)*n);
	memset((void*)linked_list, 255, n * sizeof(int));
	memset((void*)nex, 255, n * sizeof(int));
	for (int i = 0; i<n; i++)
		dsu[i] = i;
	EL = (int*)malloc(sizeof(int)*m);
	ER = (int*)malloc(sizeof(int)*m);
	for (int i = 0; i < m; i++)
	{
		int u = readInt() - 1, v = readInt() - 1;
		EL[i] = min(u, v);
		ER[i] = max(u, v);
	}
	Comp_time = clock();
	Input_time = Comp_time - Input_time;
	cl_platform_id platforms2[100];
	cl_uint platforms_n = 0;
	CL_CHECK(clGetPlatformIDs(100, platforms2, &platforms_n));
	/*
	printf("=== %d OpenCL platform(s) found: ===\n", platforms_n);
	for (int i = 0; i<platforms_n; i++)
	{
		char buffer[10240];
		printf("  -- %d --\n", i);
		CL_CHECK(clGetPlatformInfo(platforms2[i], CL_PLATFORM_PROFILE, 10240, buffer, NULL));
		printf("  PROFILE = %s\n", buffer);
		CL_CHECK(clGetPlatformInfo(platforms2[i], CL_PLATFORM_VERSION, 10240, buffer, NULL));
		printf("  VERSION = %s\n", buffer);
		CL_CHECK(clGetPlatformInfo(platforms2[i], CL_PLATFORM_NAME, 10240, buffer, NULL));
		printf("  NAME = %s\n", buffer);
		CL_CHECK(clGetPlatformInfo(platforms2[i], CL_PLATFORM_VENDOR, 10240, buffer, NULL));
		printf("  VENDOR = %s\n", buffer);
		CL_CHECK(clGetPlatformInfo(platforms2[i], CL_PLATFORM_EXTENSIONS, 10240, buffer, NULL));
		printf("  EXTENSIONS = %s\n", buffer);
	}
	*/
	if (platforms_n == 0)
		return 1;

	cl_device_id devices[100];
	cl_uint devices_n = 0;
	// CL_CHECK(clGetDeviceIDs(NULL, CL_DEVICE_TYPE_ALL, 100, devices, &devices_n));
	CL_CHECK(clGetDeviceIDs(platforms2[0], CL_DEVICE_TYPE_GPU, 100, devices, &devices_n));
	/*
	printf("=== %d OpenCL device(s) found on platform:\n", platforms_n);
	
	for (int i = 0; i<devices_n; i++)
	{
		char buffer[10240];
		cl_uint buf_uint;
		cl_ulong buf_ulong;
		printf("  -- %d --\n", i);
		CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(buffer), buffer, NULL));
		printf("  DEVICE_NAME = %s\n", buffer);
		CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_VENDOR, sizeof(buffer), buffer, NULL));
		printf("  DEVICE_VENDOR = %s\n", buffer);
		CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, sizeof(buffer), buffer, NULL));
		printf("  DEVICE_VERSION = %s\n", buffer);
		CL_CHECK(clGetDeviceInfo(devices[i], CL_DRIVER_VERSION, sizeof(buffer), buffer, NULL));
		printf("  DRIVER_VERSION = %s\n", buffer);
		CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(buf_uint), &buf_uint, NULL));
		printf("  DEVICE_MAX_COMPUTE_UNITS = %u\n", (unsigned int)buf_uint);
		CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(buf_uint), &buf_uint, NULL));
		printf("  DEVICE_MAX_CLOCK_FREQUENCY = %u\n", (unsigned int)buf_uint);
		CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(buf_ulong), &buf_ulong, NULL));
		printf("  DEVICE_GLOBAL_MEM_SIZE = %llu\n", (unsigned long long)buf_ulong);
		CL_CHECK(clGetDeviceInfo(devices[i], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(buf_ulong), &buf_ulong, NULL));
		printf("  DEVICE_MAX_WORK_GROUP_SIZE = %llu\n", (unsigned long long)buf_ulong);
	}
	*/
	printf("started running\n");


	// Load the kernel source code into the array source_str
	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen("device.cl", "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		getchar();
		exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);
	printf("kernel loading done\n");
	// Get platform and device information
	cl_device_id device_id = NULL;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;


	cl_int ret = clGetPlatformIDs(0, NULL, &ret_num_platforms);
	cl_platform_id *platforms = NULL;
	platforms = (cl_platform_id*)malloc(ret_num_platforms * sizeof(cl_platform_id));

	ret = clGetPlatformIDs(ret_num_platforms, platforms, NULL);
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clGetDeviceIDs(platforms[1], CL_DEVICE_TYPE_CPU, 1,
		&device_id, &ret_num_devices);
	printf("ret at %d is %d\n", __LINE__, ret);
	// Create an OpenCL context
	cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	printf("ret at %d is %d\n", __LINE__, ret);

	// Create a command queue
	cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);
	printf("ret at %d is %d\n", __LINE__, ret);

	// Create memory buffers on the device for each vector 
	cl_mem dsu_mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE,
		n * sizeof(int), NULL, &ret);
	
	cl_mem L_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
		m * sizeof(int), NULL, &ret);
	
	cl_mem R_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
		m * sizeof(int), NULL, &ret);

	cl_mem n_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
	 sizeof(int), NULL, &ret);

	cl_mem m_mem_obj = clCreateBuffer(context, CL_MEM_READ_ONLY,
		sizeof(int), NULL, &ret);
	// Copy the lists A and B to their respective memory buffers
	ret = clEnqueueWriteBuffer(command_queue, dsu_mem_obj, CL_TRUE, 0,
		n * sizeof(int), dsu, 0, NULL, NULL);
	printf("ret at %d is %d\n", __LINE__, ret);


	ret = clEnqueueWriteBuffer(command_queue, L_mem_obj, CL_TRUE, 0,
		m * sizeof(int), EL, 0, NULL, NULL);
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clEnqueueWriteBuffer(command_queue, R_mem_obj, CL_TRUE, 0,
		m * sizeof(int), ER, 0, NULL, NULL);
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clEnqueueWriteBuffer(command_queue, n_mem_obj, CL_TRUE, 0,
		 sizeof(int), &n, 0, NULL, NULL);
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clEnqueueWriteBuffer(command_queue, m_mem_obj, CL_TRUE, 0,
		sizeof(int), &m, 0, NULL, NULL);
	printf("ret at %d is %d\n", __LINE__, ret);

	printf("before building\n");
	// Create a program from the kernel source
	cl_program program = clCreateProgramWithSource(context, 1,
		(const char **)&source_str, (const size_t *)&source_size, &ret);
	printf("ret at %d is %d\n", __LINE__, ret);

	// Build the program
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	printf("ret at %d is %d\n", __LINE__, ret);

	printf("after building\n");
	// Create the OpenCL kernel
	cl_kernel kernel = clCreateKernel(program, "connected_components", &ret);
	printf("ret at %d is %d\n", __LINE__, ret);

	// Set the arguments of the kernel

	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&dsu_mem_obj);
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&L_mem_obj);
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&R_mem_obj);
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clSetKernelArg(kernel, 3, sizeof(cl_int),(void*) &n_mem_obj);
	printf("ret at %d is %d\n", __LINE__, ret);

	ret = clSetKernelArg(kernel, 4, sizeof(cl_int),(void*) &m_mem_obj);
	printf("ret at %d is %d\n", __LINE__, ret);
	//added this to fix garbage output problem
	//ret = clSetKernelArg(kernel, 2, sizeof(int), &LIST_SIZE);

	printf("before execution\n");
	
	clock_t Ct = clock();
	
	// Execute the OpenCL kernel on the list
	size_t global_item_size = 4; // Process the entire lists
	size_t local_item_size = 1; // Divide work items into groups of 16
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);
	printf("after execution\n");
	// Read the memory buffer C on the device to the local variable C
	ret = clEnqueueReadBuffer(command_queue, dsu_mem_obj, CL_TRUE, 0,
		n* sizeof(int), dsu, 0, NULL, NULL);
	printf("after copying\n");
	FILE* fle = fopen(file2, "w");
	FILE* fst = fopen(file3, "a");
	fprintf(fst, "-------------------------------------------------------------\n");
	
	int ft = n / global_item_size+1;
	for (int i = 0; i < m; i++)
	{
		if((EL[i]/ft)!=(ER[i]/ft))
			unite(EL[i], ER[i]);
	}
	for (int i = 0; i<n; i++)
		add(i, find_set(i));
	fprintf(fst, "Parallel Execution time : %d ms\n", clock() - Ct);
	Ct = clock();
	Output_time = clock();
	Comp_time = Output_time - Comp_time;
	
	for (int i = 0; i<n; i++)
	{
		int f = linked_list[i];
		if (f != -1)
		{
			while (f != -1)
			{
				fprintf(fle,"%d ", id[f] + 1);
				f = nex[f];
			}
			fprintf(fle,"\n");
		}
	}
	clock_t temp = clock();
	Output_time = temp - Output_time;
	Tot_exec_time = temp - Tot_exec_time;
	
	
	time_t end_time;
	time(&end_time);
	fprintf(fst, "Finished computation at : %s", ctime(&end_time));
	fprintf(fst, "Input time = %.3fms\n", (Input_time*1.0 / CLOCKS_PER_SEC)*1000.0);
	fprintf(fst, "Computation time = %.3fms\n", (Comp_time*1.0 / CLOCKS_PER_SEC)*1000.0);
	fprintf(fst, "Output time = %.3fms\n", (Output_time*1.0 / CLOCKS_PER_SEC)*1000.0);
	fprintf(fst, "Total execution time = %.3fms\n", (Tot_exec_time*1.0 / CLOCKS_PER_SEC)*1000.0);
	fclose(fst);
	// Display the result to the screen
	//getchar();
	// Clean up
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(dsu_mem_obj);
	ret = clReleaseMemObject(L_mem_obj);
	ret = clReleaseMemObject(R_mem_obj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
	free(dsu);
	free(EL);
	free(ER);
	free(nex);
	free(id);
	free(linked_list);
	return 0;
}