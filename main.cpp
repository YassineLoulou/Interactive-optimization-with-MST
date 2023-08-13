#define _CRT_SECURE_NO_DEPRECATE
#include <bits/stdc++.h>
#include <iostream>
#include <fstream>
#include"ilcplex\cplex.h" 
#include"ilcplex\ilocplex.h"
#include <ilcp/cp.h>
#include <Python.h>

#include <C:\Users\yassi\Downloads\pyhelper.hpp>

typedef long long ll;


using namespace std;
const int n = 10, k = 20, itMAX = 20, MX_layers = 4;
int a[n], c[n], mn[n], mx[n], b;
const int MX = 100;
int rnd, in, out;
int sz;
const int RES = 295;

int minKey(int key[], bool mstSet[], int V)
{
	// Initialize min value
	int min = INT_MAX, min_index;

	for (int v = 0; v < V; v++)
		if (mstSet[v] == false && key[v] < min)
			min = key[v], min_index = v;

	return min_index;
}
// A utility function to print the
// constructed MST stored in parent[]
int printMST(int parent[], int graph[MX][MX], int V)
{
	printf("Edge \tWeight\n");
	for (int i = 1; i < V; i++)
		printf("%d - %d \t%d \n", parent[i], i, graph[i][parent[i]]);
}

// Function to construct and print MST for
// a graph represented using adjacency
// matrix representation
int *  MST(int graph[MX][MX], int V)
{
	// Array to store constructed MST
	static int parent[MX];
	// Key values used to pick minimum weight edge in cut
	int key[MX];
	// To represent set of vertices included in MST
	bool mstSet[MX];

	// Initialize all keys as INFINITE
	for (int i = 0; i < V; i++)
		key[i] = INT_MAX, mstSet[i] = false;

	// Always include first 1st vertex in MST.
	// Make key 0 so that this vertex is picked as first
	// vertex.
	key[0] = 0;

	// First node is always root of MST
	parent[0] = -1;

	// The MST will have V vertices
	for (int count = 0; count < V - 1; count++) {

		// Pick the minimum key vertex from the
		// set of vertices not yet included in MST
		int u = minKey(key, mstSet,V);

		// Add the picked vertex to the MST Set
		mstSet[u] = true;

		// Update key value and parent index of
		// the adjacent vertices of the picked vertex.
		// Consider only those vertices which are not
		// yet included in MST
		for (int v = 0; v < V; v++)

			// graph[u][v] is non zero only for adjacent
			// vertices of m mstSet[v] is false for vertices
			// not yet included in MST Update the key only
			// if graph[u][v] is smaller than key[v] 
			if (graph[u][v] && mstSet[v] == false
				&& graph[u][v] < key[v])
				parent[v] = u, key[v] = graph[u][v];
	}

	// print the constructed MST
	//printMST(parent, graph,V);
	return parent;
}


IloModel lin_mdl (IloEnv env,int pref[k],int sol[k][n]) {

	//Model
	IloModel mdl(env);
	IloNumVarArray w(env, n);

	//Linear Constraints
	for (int i = 0; i < k-1; i++) {
		IloNumExpr expr(env);
		for (int j = 0; j < n; j++) {
			expr += sol[pref[i]][j]-sol[pref[i+1]][j] * w[j];
		}
		mdl.add(IloRange(env, 0, expr, IloInfinity));
	}
	return mdl;
}
struct Solution {
	int x[n];
	int eps[n];
};
int ManhattanDis(Solution a, Solution b) {
	int s = 0;
	for (int i = 0; i < n; i++)
		s += abs(a.x[i] - b.x[i]);
	return s;
}
int obj_01(int v[n]) {
	int s = 0;
	for (int i = 0; i < n; i++)
		s += c[i] * v[i];
	return s;
}
int obj_jobshop() {

}
void RIP() {
	cout << "RIP RIP RIP RIP RIP RIP RIP RIP RIP" << endl;
	exit(0);
}
bool check_in(int x[n], Solution s) {
	set<int>indices;
	for (int i = 0; i < n; i++)
		if (abs(x[i] - s.x[i]) > s.eps[i]) 
			return false;
	return true;
}
vector<vector<Solution>> get_pref(int sol[k][n],int MX_cost,vector<vector<Solution>> &groups) {
	int pref[k];
	for (int i = 0; i < k; i++) {
		pref[i] = int( 1.0 * obj_01(sol[i]) * MX_layers / RES );
		if (pref[i] > MX_layers - 1 || pref[i] < 0)
			RIP();
		for (int layer = 0; layer < MX_layers; layer++) {
			for (Solution s : groups[layer]) {
				if (check_in(sol[i], s) && layer != pref[i]) {
					for (int j = 0; j < n; j++) {
						if (s.x[j] != sol[i][j])
							s.eps[i] = abs(s.x[j] - sol[i][j]) - 1 ;
					}
				}
			}
		}
	}
	
	// i* MX_cost / MX_layers <= obj_01 <= (i + 1) * MX_cost / MX_layers;

	vector<vector<Solution>> groups2(MX_layers);
	for (int i = 0; i < k; i++) {
		vector<int>new_point(begin(sol[i]), end(sol[i]));
		Solution s;
		for (int j = 0; j < n; j++){
			s.x[j] = new_point[j];
			s.eps[j] = max(mx[j] - s.x[j], s.x[j] - mn[j]);
		}
		groups2[pref[i]].push_back(s);
	}
	return groups2;
}

void constraint_model_01(IloEnv env,IloModel &mdl, IloIntVarArray x) {

	IloIntExpr exp(env);
	for (int i = 0; i < n; i++)
		exp += a[i] * x[i];
	mdl.add(exp <= b);
}
void constraint_model_jobshop(IloEnv env, IloModel& mdl, IloIntervalVarArray x) {

}
void avoid_prev_solutions(IloEnv env,IloModel& mdl, IloIntVarArray x, vector<Solution>generated ) {
	for (Solution sol : generated) {
		IloOr myor(env);
		for (int i = 0; i < n; i++)
			myor.add(x[i] != sol.x[i]);
		mdl.add(myor);
	}
}
void get_out(IloEnv env, IloModel& mdl, IloIntVarArray x, vector<vector<Solution>>groups) {
	for (int layer = 0; layer < MX_layers - 1; layer++) {
		if (groups[layer].size() <2)
			continue;
		IloOr myor(env);
		for (Solution sol : groups[layer]) {
			IloAnd And(env);
			//cout << "good solution : ";
			for (int i = 0; i < n; i++) {
				//cout << sol.x[i] << " " << sol.eps[i] << endl;
				And.add(IloAbs(x[i] - sol.x[i]) >= sol.eps[i]);
			}
			//cout << endl << endl;
			myor.add(And);
		}
		mdl.add(myor);
	}
}
void get_in(IloEnv env, IloModel& mdl, IloIntVarArray x, vector<vector<Solution>> groups) {
	int layer = MX_layers - 1;
	IloOr myor(env);
	for (Solution sol : groups[layer]) {
		IloAnd And (env);
		for (int i = 0; i < n; i++) {
			And.add(IloAbs(x[i] - sol.x[i]) <= sol.eps[i]);
		}
		myor.add(And);
	}
	mdl.add(myor);
}
void get_mst(IloEnv env, IloModel& mdl, IloIntVarArray x, vector < vector < int* > > MstClust,vector<map<int,vector<Solution>>>Clust, map<int,int> nb_clust) {

	IloOr myor(env);
	for (int layer = 0; layer < MX_layers; layer++) {
		for (int cl = 0; cl < nb_clust[layer]; cl++) {
			int m = Clust[layer][cl].size();
			int* parent = MstClust[layer][cl];
			for (int i = 1; i < m; i++) {
				IloAnd myand(env);
				for (int j = 0; j < n; j++) {
					myand.add(x[j] >= min(Clust[layer][cl][i].x[j], Clust[layer][cl][parent[i]].x[j]));
					myand.add(x[j] <= max(Clust[layer][cl][i].x[j], Clust[layer][cl][parent[i]].x[j]));
				}
				myor.add(myand);
			}
		}
	}
	mdl.add(myor);
}

int main() {

	ofstream plot;
	plot.open("C:/Users/yassi/OneDrive/Bureau/plot.txt");

	


	//Env
	IloEnv env;

	//generate domain cardinal
	ll card = pow(2, n);
	ll remain = card;
	
	
	//generate min max domains 
	for (int i = 0; i < n; i++)
		mn[i] = 0, mx[i] = 1;

	//read weights
	for (int i = 0; i < n; i++)
		cin >> a[i];

	//read costs
	for (int i = 0; i < n; i++)
		cin >> c[i];

	//get MX
	int MX_cost = 0;
	for (auto i : c)
		MX_cost += i;

	//read b
	
	cin >> b;
	

	//Model
	IloModel mdl(env);
	IloIntVarArray x(env, n, 0, 1);
	constraint_model_01(env,mdl,x);

	int it = 0;

	vector<vector<Solution>>groups(MX_layers);


	// First Iteration

	IloCP cp(mdl);
	//cp.setParameter(IloCP::TimeLimit, 1MX_layers); 
	

	//cp.setParameter(IloCP::RandomSeed, 1000);


	cp.setParameter(IloCP::SearchType, IloCP::DepthFirst);
	//cp.setParameter(IloCP::MultiPointNumberOfSearchPoints, IloIntMax);


	vector <Solution> generated;
	Solution ans;
	int obj_ans = -1;

	//Solve|
	//-----|
	cp.startNewSearch();
	//
	int nb = 0;
	int sol[k][n];
	while (cp.next() && nb < k) {
		
		Solution s;
		//cout << "Sol "<<nb+1<<"  :  ";
		for (int i = 0; i < n; i++) {

			//cout << cp.getValue(x[i]) << " ";
			
			sol[nb][i] = cp.getValue(x[i]);
			s.x[i] = cp.getValue(x[i]);
		}
		generated.push_back(s);
		if (obj_ans < obj_01(sol[nb])) {
			for (int i = 0; i < n; i++)
				cout << sol[nb][i] << " ";
			cout << endl;
			obj_ans = obj_01(sol[nb]);
			ans = s;
		}
		nb++;
		//plot << to_string(obj_ans);
	}
	//cout << endl;
	remain -= k;

	it++;
	cout << "HERE IS IT " << obj_ans << endl;
	cp.clear();

	// START LOOP
	while (it < itMAX) {
		cout << " iteration " << it << ":" << endl;


		//get pref|
		//--------|

		vector<vector<Solution>>groups_alt(MX_layers);
		groups_alt = get_pref(sol, MX_cost, groups);
		for (int i = 0; i < MX_layers; i++) {
			for (int j = 0; j < int(groups_alt[i].size()); j++)
				groups[i].push_back(groups_alt[i][j]);
		}


		// START CLUSTERING (AGGLO OR MANIFOLD)//
//cout << "STARTED CLUSTERING" << endl;

		map<int, int>nb_clust;
		for (int i = 0; i < MX_layers; i++)
			nb_clust[i] = 0; 
		vector < map<int, vector < Solution > > > Clust(MX_layers);
		for (int layer = 0; layer < MX_layers; layer++) {

			//cout << " layer  [ " << layer * 20 << " - " << (layer + 1) * 20 << " ] " << endl;
			if (int( groups[layer].size() ) < 2) 
				continue;

			//input solutions into file

			ofstream myfile;
			myfile.open("C:/Users/yassi/OneDrive/Bureau/points_input.txt");
			for (int i = 0; i < int( groups[layer].size() ) ; i++) {
				for (int j = 0; j < n; j++) {
					myfile << to_string(groups[layer][i].x[j]);
					if (j < n-1)
						myfile << " ";
				}
				//cout << endl;
				myfile << '\n';
			}
			myfile.close();

			//cluster points
			system("C:/Users/yassi/anaconda3/envs/Clustering/python.exe temp.py");
				

			//read clustering results and load Clust [0 1 2 3 4] by a map : cluster -> points inside

			string line;
			int cluster;
			ifstream myfile2("C:/Users/yassi/OneDrive/Bureau/points_output.txt");
			int point = 0;
			while (getline(myfile2, line)){
				cluster = stoi(line);
				nb_clust[layer] = max(nb_clust[layer], cluster);
				Clust[layer][cluster].push_back(groups[layer][point]);
				point++;
			}
		}



		
												// GENERATE POTENTIALLY GOOD SOLUTIONS FROM DATA
		
		cout << "STARTED MINIMUM SPANNING TREE" << endl;

		vector < vector < int* > > MstClust(MX_layers);

		//// MST : Minimum Spanning Tree for each cluster

		for (int layer = 0; layer < MX_layers;layer++) {
			vector<int*> v;
			for (int cl = 0; cl < nb_clust[layer]; cl++) {
				int graph[MX][MX];
				int m = Clust[layer][cl].size();
			for (int i = 0; i < m; i++)
					for (int j = 0; j < m; j++) {
						graph[i][j] = ManhattanDis(Clust[layer][cl][i], Clust[layer][cl][j]);
					}
					v.push_back(MST(graph, m));
			}
			MstClust.push_back(v);
		}
		



		int wanted_rnd = ceil ( ( 1.0 * remain / card ) * k ) ;
		//cout << "wanted_rnd = "<< wanted_rnd << endl;
		nb = 0;
		IloInt NbSol;

		//cp.setParameter(IloCP::TimeLimit, 1MX_layers); //cp.setParameter(IloCP::SearchType, IloCP::DepthFirst)

		// RANDOM

		cout << " GENERATING random solutions " << endl;

		IloModel mdl_0(env);
		constraint_model_01(env, mdl_0, x);
		avoid_prev_solutions(env, mdl_0, x, generated);
		IloCP cp_0(mdl_0);
	//	cp_0.setParameter(IloCP::RandomSeed, 1000);

		//cp_0.setParameter(IloCP::SearchType, IloCP::Restart);
		//cp_0.setParameter(IloCP::MultiPointNumberOfSearchPoints, 1000000);

		cp_0.setParameter(IloCP::SearchType, IloCP::DepthFirst);


		int nb0 = 0;

		if (cp_0.solve()) {
			cp_0.startNewSearch();
			while (cp_0.next() && nb < min(k/2, wanted_rnd)) {
				Solution s;
				//cout << "Sol " << nb+1 << "  :  ";
				for (int i = 0; i < n; i++) {
					//cout << cp.getValue(x[i]) << " ";
					sol[nb][i] = cp_0.getValue(x[i]);
					s.x[i] = cp_0.getValue(x[i]);
				}
				//cout << obj_01(sol[nb]) << " ";
				generated.push_back(s);
				if (obj_ans < obj_01(sol[nb])) {
					obj_ans = obj_01(sol[nb]);
					ans = s;
					for (int i = 0; i < n; i++)
						cout << sol[nb][i] << " ";
					cout << endl;
				}
				//cout << endl;
				nb++;
			}
		}
		cp_0.clear();
		nb0 = nb - nb0;
		rnd = nb;
		cout << "best answer so far is =  " << obj_ans << endl;


		// OUT 
		int nb2 = nb;
		cout << " GENERATING  out solutions " << endl;


		IloModel mdl_2(env);
		constraint_model_01(env, mdl_2, x);
		avoid_prev_solutions(env, mdl_2, x, generated);
		get_out(env, mdl_2, x, groups);
		IloCP cp_2(mdl_2);

		cp_2.setParameter(IloCP::SearchType, IloCP::DepthFirst);
			
		//cp_2.setParameter(IloCP::SearchType, IloCP::Restart);
		//cp_2.setParameter(IloCP::MultiPointNumberOfSearchPoints, 1000000);
		//cp_2.setParameter(IloCP::RandomSeed, 1000);


		if (cp_2.solve()) {
			cp_2.startNewSearch();
			while (cp_2.next() && nb < (rnd + (k - rnd) / 2)) {
				Solution s;
				//cout << "Sol " << nb + 1 << "  :  ";
				for (int i = 0; i < n; i++) {
					//cout << cp.getValue(x[i]) << " ";
					sol[nb][i] = cp_2.getValue(x[i]);
					s.x[i] = cp_2.getValue(x[i]);
				}
				//cout << obj_01(sol[nb]) << " ";
				generated.push_back(s);
				if (obj_ans < obj_01(sol[nb])) {
					obj_ans = obj_01(sol[nb]);
					for (int i = 0; i < n; i++)
						cout << sol[nb][i] << " ";
					cout << endl;
					ans = s;
				}
				//cout << endl;
				nb++;
			}
		}

		cp_2.clear();
		nb2 = nb - nb2;

		cout << "best answer so far is =  " << obj_ans << endl;



		// IN
		int nb1 = nb;

		cout << " GENERATING in solutions " << endl;
		
		IloModel mdl_1(env);
		constraint_model_01(env, mdl_1, x);
		avoid_prev_solutions(env, mdl_1, x, generated);
		get_in(env, mdl_1, x, groups);
		//get_mst(env, mdl_1, x, MstClust, Clust,nb_clust);
		IloCP cp_1(mdl_1);

		//cp_1.setParameter(IloCP::SearchType, IloCP::Restart);

		cp_1.setParameter(IloCP::SearchType, IloCP::DepthFirst);
		cp_1.setParameter(IloCP::MultiPointNumberOfSearchPoints, 1000000);

		rnd = nb;
		if (cp_1.solve()) {
			cp_1.startNewSearch();
			while (cp_1.next() && nb < k) {
				Solution s;
				//cout << "Sol " << nb + 1 << "  :  ";
				for (int i = 0; i < n; i++) {
					//cout << cp.getValue(x[i]) << " ";
					sol[nb][i] = cp_1.getValue(x[i]);
					s.x[i] = cp_1.getValue(x[i]);
				}
				//cout << obj_01(sol[nb]) << " ";
				generated.push_back(s);
				if (obj_ans < obj_01(sol[nb])) {
					obj_ans = obj_01(sol[nb]);
					for (int i = 0; i < n; i++)
						cout << sol[nb][i] << " ";
					cout << endl;
					ans = s;
				}
				//cout << endl;
				nb++;
			}
		}
		cp_1.clear();
		nb1 = nb - nb1;

		cout << "best answer so far is =  " << obj_ans << endl;


		// COMPLETE REST WITH RANDOM 
		int nb4 = nb;

		IloModel mdl_4(env);
		constraint_model_01(env, mdl_4, x);
		avoid_prev_solutions(env, mdl_4, x, generated);
		IloCP cp_4(mdl_4);

		cp_4.setParameter(IloCP::SearchType, IloCP::DepthFirst);
		//cp_4.setParameter(IloCP::RandomSeed, 1000);

		//cp_4.setParameter(IloCP::SearchType, IloCP::Restart);
		//cp_4.setParameter(IloCP::RandomSeed, 1000);


		if (cp_4.solve()) {
			cp_4.startNewSearch();
			while (cp_4.next() && nb < k) {
				Solution s;
				//cout << "Sol " << nb+1 << "  :  ";
				for (int i = 0; i < n; i++) {
					//cout << cp.getValue(x[i]) << " ";
					sol[nb][i] = cp_4.getValue(x[i]);
					s.x[i] = cp_4.getValue(x[i]);
				}
				//cout << obj_01(sol[nb]) << " ";
				generated.push_back(s);
				if (obj_ans < obj_01(sol[nb])) {
					obj_ans = obj_01(sol[nb]);
					for (int i = 0; i < n; i++)
						cout << sol[nb][i] << " ";
					cout << endl;
					ans = s;
				}
				//cout << endl;
				nb++;
			}
		}
		nb4 = nb - nb4;
		cout << nb0 << " " << nb1 << " " << nb2 << " " << nb4 << endl;
		cp_4.clear();
		for (int i = 0; i < MX_layers; i++) {
			cout << groups[i].size() << " ";

		}
		cout << endl;

		cout << "best answer so far is =  " << obj_ans << endl;
		if (nb < k)
			break;
		remain -= k;
		it++;
	//	plot << to_string(obj_ans);

		for (int i = 0; i < MX_layers; i++) {
			plot << groups[i].size() << (i < MX_layers - 1 ? " " : "\n");
		}
	}

	

	cout << "best answer so far is =  " << obj_ans << endl;
	for (int i = 0; i < n; i++) {
		cout << ans.x[i] << " ";
	}cout << endl;

	//END LOOP









	/*
	IloSolution sol(env);
	sol.add(y);
	sol.store(cplex);
	cout << sol << endl;
	
	cplex.startNewSearch();
	while (cplex.next()) {
		for (int i = 0; i < n; i++)
			cout << cplex.getValue(x[i]) << " ";
		cout << endl;
	}
	
	IloInt NbSol = cplex.getInfo(IloCP::NumberOfSolutions);
	cout << NbSol << endl;
	*/
	return 0;
}



		//MILP
		//IloModel MILP=lin_mdl(env,pref,sol);
		//IloCP cp_lin(MILP);