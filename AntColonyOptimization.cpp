/*
Made by Manvi Agrawal as an assignment for Soft Computing under the guidance of
Dr. Shampa Chakraverty
A program designed to find optimal path for e-learning system
The following are given about the system:
a) Credits of a course
b) Time required to complete a course
c) Difficulty[i][j] representing the difficulty of other course j
given learner has completed course i
The following objectives needs to be fulfilled :
a) The credits along a path must be maximized.
b) The total difficulty of the path must be minimized.
The following are the constraints :
a) Minimum credits that the user wants to acquire
b) The total time limit must not exceed the maximum time limit
The problem is modelled by a graph as follows:
a) The nodes of the graph are course modules. They have 2 properties : credits
and time_required
b) The edges of the graph contain the difficulty of going from a course i to a
course j
c) Pherome matrix : To store pherome values.
d) pherome_increment_prob : A matrix to store pherome Increments of various
ants.
*/
#include <omp.h>
#include <bits/stdc++.h>
#include<random>
#define INF 1000000000
using namespace std;
int N, GEN, num_ants;
vector<int> credits;
vector<int> time_required;
vector< vector<float> > pherome;
vector< vector<float> > difficulty;
vector< vector< vector<float> > > pherome_increment_prob;
float alpha,beta,rho;int max_time, min_credits;
vector<vector<int>> path;
float w1=0.5, w2=0.5;
void input_parameters();/*To take input from the user*/
void explore_paths();/*To explore paths*/
void choose_destination();/*To choose a destination vertex*/
void init_pherome_increment_prob();/*To initialize pherome increment matrix of
each ant to 0 */
void update_pherome();/*To update global pherome matrix by pherome evaporation
and pherome collection*/
void print_pherome_matrix();/*To print pherome matrix*/
void print_path_matrix();/*To print path matrix*/
void remove_path();/*To clear path matrix after a generation is over*/

/*Driver Code*/
int main(int argc, char* argv[])
{
	if(argc!=2)
	{
		cout << "Program requires number of ants as a cmd line argument\n";
	}
	else
	{
		num_ants=stoi(argv[1]);
		input_parameters();
		print_pherome_matrix();
		print_path_matrix();
		for(int gen=1; gen<=GEN; gen++)
		{
			init_pherome_increment_prob();
			explore_paths();
			update_pherome();
			print_pherome_matrix();
			print_path_matrix();
			remove_path();
			cout<< "gen " << gen << " of ants completed\n";
		}
	}
	return 0;
}
void input_parameters()
{
	//Input alpha, beta and rho*/
	cout << "Enter alpha, beta and rho : ";
	cin >> alpha >> beta >> rho;
	//Input number of generations*/
	cout << "Enter the number of generations : ";
	cin >> GEN;
	//Input number of nodes in graph*/
	cout << "Enter the number of nodes in graph : ";
	cin >> N;
	//Input credits of each course*/
	credits = vector<int>(N);
	cout << "Enter the credits of each course : ";
	for(int i=0; i<N; i++)
		cin>>credits[i];
	//Input Time required for each course*/
	cout << "Enter the time required to complete a course : ";
	time_required = vector<int>(N);
	for(int i=0; i<N; i++)
		cin >> time_required[i];
	//Input Diff Matrix*/
	cout << "Enter the difficulty matrix of courses : ";
	difficulty = vector< vector<float> >(N, vector<float>(N));
	for(int i=0; i<N; i++)
		for(int j=0; j<N; j++)
			cin >> difficulty[i][j];
	//Set Pheorome Matrix*/
	pherome = vector< vector<float> >(N, vector<float>(N,1));
	//Set Pherome Increment Matrix*/
	pherome_increment_prob = vector< vector< vector<float> > >(num_ants,
	vector< vector<float> >(N, vector<float>(N,0)) );
	//Set path Matrix*/
	path = vector< vector<int> >(num_ants);
	//Input max_time and min_credits*/
	cout << "Enter the maximum time limit and minimum credits : ";
	cin >> max_time >> min_credits ;
}
void init_pherome_increment_prob()
{
	for(int t=0; t<num_ants; t++)
	{
		for(int i=0; i<N; i++)
		{
			for(int j=0; j<N; j++)
				pherome_increment_prob[t][i][j]=0;
		}
	}
}
int choose_destination(vector<bool> &visited, int src)
{
	random_device rd;
	mt19937 gen(rd());
	vector<float> weights(N);
	for(int j=0;j<N;j++)
	{
		float pheorome_ij = pherome[src][j];
		float yeta_ij = credits[j]/difficulty[src][j]; /*For local
		exploitation*/
		weights[j] = pow(pheorome_ij,alpha)*pow(yeta_ij,beta);/*Division by
		summation is skipped
		since the discrete_distribution generates a random number according
		to proportion of the weights*/
		if(visited[j])
		{
		weights[j]=0;
		}/*Setting to zero to avoid generating a
		vertex that is already visited */
	}
	discrete_distribution<> d(weights.begin(),weights.end());
	return d(gen);
}

void explore_paths()
{
	int ant_id;
	#pragma omp parallel private(ant_id) //Let each ant explore
	{
		int num_a = omp_get_num_threads();
		cout << "number of ants : " << num_a << "\n";
		int ant_id = omp_get_thread_num(); // Getting processor number
		int credits_left = min_credits;
		int time_left = max_time;
		int source = 0;//from starting course
		vector<bool> visited(N,false);

		while(time_left-time_required[source]>=0 && path[ant_id].size()<N)
		{
			path[ant_id].push_back(source);
			time_left -= time_required[source];
			credits_left -= credits[source];
			visited[source] = true;
			int destination = choose_destination(visited,source);
			source = destination;
		}
		if(credits_left<=0)
		{
			/* Increment pherome only if min credits criteria is
			satisfied*/
			for(int i=0; i<path[ant_id].size()-1 ; i++)
			pherome_increment_prob[ant_id][path[ant_id][i]]
			[path[ant_id][i+1]] =
			(w1*credits[path[ant_id][i+1]] -
			w2*difficulty[ path[ant_id][i]][path[ant_id][i+1]]) /
			(w1+w2);
		}
		else
		{
			/*Remove any wrong path from the path matrix*/
			while(path[ant_id].size() > 0)
				path[ant_id].pop_back();
		}
	}
}

void update_pherome()
{
	for(int i=0; i<N; i++)
	{
		for(int j=0; j<N; j++)
		{
			float sum=0;/*For adding pherome collected by current
			generation of ants*/
			for(int t=0; t<num_ants; t++)
			sum += pherome_increment_prob[t][i][j];
			pherome[i][j] = (1-rho)*pherome[i][j] + sum;
			/* (1-rho)*pherome[i][j] calculates pherome left after
			evaporation*/
		}
	}
}
void print_pherome_matrix()
{
	cout<< "\nPherome matrix :\n";
	for(int i=0; i<N; i++)
	{
		for(int j=0; j<N; j++)
		{
			cout << pherome[i][j] << " " ;
		}
		cout<<"\n";
	}
}

void print_path_matrix()
{
	cout<< "\nPath matrix :\n";
	for(int i=0; i<num_ants; i++)
	{
		int credits_acquired=0;
		int time_spent=0;
		float total_difficulty=0;
		cout<<"{";
		for(int j=0; j<path[i].size(); j++)
		{
			cout << path[i][j] << ", " ;
			credits_acquired += credits[ path[i][j] ];
			time_spent += time_required[ path[i][j] ];
			if( j != path[i].size()-1 )
			{
				total_difficulty += difficulty[ path[i][j]][path[i]
				[j+1]];
			}
	}
	cout <<"}, {credits : " << credits_acquired << "}, ";
	cout << "{ time spent : " << time_spent << "}, ";
	cout << "{ difficulty : " << total_difficulty << "} \n";
	}
}

void remove_path()
{
	for(int i=0; i<num_ants; i++)
	{
		while(path[i].size()>0)
		{
			path[i].pop_back();
		}
	}
}
