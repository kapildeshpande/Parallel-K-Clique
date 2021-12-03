#include <omp.h>
#include <bits/stdc++.h>
using namespace std;

void listKCliqueParallel(int l, unordered_map<int, unordered_set<int> > &sg, int &numCliques, vector <int> &c) {

	if(l==2) {
		for (auto &p : sg) {
			//critical section
			#pragma omp critical
			for (auto &v : p.second) {
				for (int val : c) 
					cout<<val<<" ";
				cout<<p.first<<" "<<v<<"\n";
				numCliques++;
			}
		}
		return;
	}
	
	for (auto it=sg.begin();it!=sg.end();++it) {
		int u = it->first;
		unordered_map<int, unordered_set<int> > sgNew;
		
		for(auto it1=sg.begin();it1!=sg.end();++it1) {
			if (it1->first == u || !(it->second).count(it1->first))
				continue;
			
			for (auto &val : it1->second) {
				if ((it->second).count(val))
					sgNew[it1->first].insert(val);
			}
		}
		
		c.push_back(u);
		listKCliqueParallel(l-1, sgNew, numCliques, c);
		c.pop_back();
			
	}

}

void builtSubgraph(vector<vector<int> > &dag,int u,unordered_map<int, unordered_set<int> > &sg) {
	unordered_set<int> s;
	for (int v : dag[u])
		s.insert(v);
		
	for (int i=0;i<dag.size();i++) {
		if (i == u || !s.count(i))
			continue;
		for (int j=0;j<dag[i].size();j++) {
			if (s.count(dag[i][j]))
				sg[i].insert(dag[i][j]);
		}
	}
}

int listKClique(int k, vector<vector<int> > &dag) {
	int numCliques = 0; //num of k clique
	#pragma omp parallel for 
	for(int u=0; u<dag.size(); u++){
		unordered_map<int, unordered_set<int> > sg;
		builtSubgraph(dag,u,sg);
		vector<int> c;
		c.push_back(u);
		listKCliqueParallel(k-1, sg, numCliques, c);
	}
	return numCliques;
}

vector<vector<int> > builtGraph(const char *input, int &k) {
	vector<vector<int> > adj;
	int numNodes = 0, numEdges = 0, u, v;
	vector<int> deg;

	FILE *file;
	
	file=fopen(input,"r");
	
	while (fscanf(file,"%d %d", &u, &v)==2) {
		numNodes = max(numNodes, max(u, v)+1);
		numEdges++;
		if (deg.size() < numNodes)
			deg.resize(numNodes);
		if (adj.size() < numNodes)
			adj.resize(numNodes);
			
		adj[u].push_back(v);
		adj[v].push_back(u);
		
		deg[u]++;
		deg[v]++;
	}
	k = u;
	fclose(file);
	
	vector<int> visited(numNodes);
	priority_queue<pair<int, int> > pq;//{deg, node}
	
	for (int i=0;i<deg.size();i++) {
		if (deg[i] > 0)
			pq.push({deg[i], i});
	}
	
	vector<vector<int> > dag(numNodes);
	
	while (!pq.empty()) {
		int node = pq.top().second;
		pq.pop();
		
		if (visited[node])
			continue;
		
		visited[node] = 1;
		for (int child : adj[node]) {
			if (!visited[child])
				dag[node].push_back(child);	
		}
	}
	
	return dag;
}

void printDag(vector<vector<int> > &dag) {
	for (int i=0;i<dag.size();i++) {
		cout<<i<<" -> ";
		for (int v : dag[i])
			cout<<v<<" ";
		cout<<"\n";
	}
}

int main (int argc,char** argv) {
	//int k = atoi(argv[2]);
	//cout<<"K value = "<<k<<"\n";
	
	int k = 0;
	const char *input = (argv[1] != NULL) ? argv[1] : "edgelist.txt";
	
	vector<vector<int> > dag = builtGraph(input, k);
	//printDag(dag);
	clock_t begin_time = clock();
	
	int numCliques = listKClique(k, dag);
	cout<<numCliques<<"\n";
	
	clock_t end_time = clock();
	double time_req = (double)(end_time - begin_time) / CLOCKS_PER_SEC;
	cout<<"Execution Req: "<<time_req<<"\n";

	return 0;
}
