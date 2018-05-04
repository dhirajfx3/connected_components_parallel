#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#pragma warning(disable :4996)
#define ve vector
#define getn S[df++]
char *S;
int df = 0;
using namespace std;
inline int readInt()
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
void input_read(char *file)
{
	FILE *f = fopen(file, "r");
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);  //same as rewind(f);
	S=(char*)malloc(fsize+1);
	fread(S, fsize, 1, f);
	fclose(f);
	S[fsize] = 0;
}
ve<bool> vis;
ve<ve<int>> adj;
ve<ve<int>> compo;
ve<int> cs;
int csfill = 0;
void dfs(int c, int id)
{

	cs[csfill++]=c;
	while (csfill>0)
	{
		--csfill;
		c = cs[csfill];
		vis[c] = 1;
		compo[id].push_back(c);
		for (auto &x : adj[c])
		{
			if (!vis[x])
			{
				vis[x] = 1;
				cs[csfill++]=x;
			}
		}
	}
}

using namespace chrono;
int main(int argc, char **argv)
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	/*
		 input file = input.txt
		 output file = dfsout.txt
		 log file = dfslog.txt
	input file format
	A single line of input first integer 'n'
	second integer 'm' then m pairs of integers denoting edges
	output : Connected components in each line
	*/
	
	char file[] = "input.txt";
	char file2[] = "dfsout.txt";
	char file3[] = "dfslog.txt";
	clock_t Input_time, Comp_time, Tot_exec_time, Output_time;
	FILE* fd = freopen(file2, "w", stdout);
	Input_time = clock();
	input_read(file);
	Tot_exec_time = Input_time;
	Comp_time = clock();
	Input_time = Comp_time - Input_time;
	df = 0;
	int n = readInt(), m = readInt();
	vis.resize(n);
	cs.resize(n);
	adj.resize(n);
	for (int i = 0; i<m; i++)
	{
		int x = readInt() - 1, y = readInt() - 1;
		adj[x].push_back(y);
		adj[y].push_back(x);
	}
	int y = 0;
	for (int i = 0; i<n; i++)
		if (!vis[i])
		{
			compo.push_back(ve<int>());
			dfs(i, y++);
		}
	Output_time = clock();
	Comp_time = Output_time - Comp_time;
	for (int i = 0; i<y; i++)
	{
		for (auto &x : compo[i])
		{
			printf("%d ", x + 1);
		}
		printf("\n");
	}
	fclose(fs);
	clock_t temp = clock();
	Output_time = temp - Output_time;
	Tot_exec_time = temp - Tot_exec_time;
	FILE* fst = fopen(file3, "a");
	fprintf(fst, "-------------------------------------------------------------\n");
	auto end = system_clock::now();
	time_t end_time = system_clock::to_time_t(end);
	fprintf(fst, "Finished computation at : %s", std::ctime(&end_time));
	fprintf(fst,"Nodes : %d, Edges : %d\n",n,m);
	fprintf(fst, "Input time = %.3fms\n", (Input_time*1.0 / CLOCKS_PER_SEC)*1000.0);
	fprintf(fst, "Computation time = %.3fms\n", (Comp_time*1.0 / CLOCKS_PER_SEC)*1000.0);
	fprintf(fst, "Output time = %.3fms\n", (Output_time*1.0 / CLOCKS_PER_SEC)*1000.0);
	fprintf(fst, "Total execution time = %.3fms\n", (Tot_exec_time*1.0 / CLOCKS_PER_SEC)*1000.0);
	fclose(fst);
	return 0;
}
