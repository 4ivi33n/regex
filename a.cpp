#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <stack>
#include <utility>
#include <cstdlib>
#include <climits>
using namespace std;

#define REP(i, n) for(typeof(n) i=0;i<(n);++i)
#define FOR(i,a,b) for(typeof(b) i=a;i<=b;++i)
#define FORD(i,a,b) for(typeof(b) i=a;i>=b;--i)
#define SZ(x) ((int)((x).size()))
#define all(c) (c).begin(), (c).end()
#define pb push_back
#define foreach(it, c)for(typeof((c).begin()) it=(c).begin();it!=(c).end();++it)
class Digraph {
	int numOfVertices, numOfEdges;
	vector<vector<int> >  adj;
public:
	Digraph() {}
	Digraph(int V) : numOfVertices(V), numOfEdges(0) {
		adj = vector<vector<int> >(numOfVertices, vector<int>());
	}
	int getNumOfVertices() {
		return numOfVertices;
	}
	int getNumOfEdges() {
		return numOfEdges;
	}
	void addEdge(int v,int w) {
		adj[v].push_back(w);;
		numOfEdges++;
	}
	vector<int> adjacentTo(int v) {
		return adj[v];
	}
};


class DirectedDFS {
	bool *marked;
public:
	DirectedDFS(Digraph g, int s) {
		marked = new bool[g.getNumOfVertices()];
		REP(i,g.getNumOfVertices()) marked[i]=false;
		dfs(g, s);
	}

	DirectedDFS(Digraph g, vector<int> s) {
		marked = new bool[g.getNumOfVertices()];
		REP(i,g.getNumOfVertices()) marked[i]=false;
		REP(i, SZ(s)) {
			if (!marked[s[i]]) {
				dfs(g,s[i]);
			}
		}
	}

	void dfs(Digraph g, int v) {
		marked[v] = true;
		vector<int> adj = g.adjacentTo(v);
		REP(i, SZ(adj)) {
			if (!marked[adj[i]]) {
				dfs(g,adj[i]);
			}
		}
	}
	bool isMarked(int v) {
		return marked[v];
	}
};


class NFA {
	string regex;
	Digraph g;
	int M;
public:
	NFA(string r) : regex(r) {
		stack<int> ops;
		M = regex.size();
		g = Digraph(M+1);
		
		for (int i=0;i<M;i++) {
			int lp = i;
			if (regex[i] == '(' || regex[i] == '|') {
				ops.push(i);
			}
			else if (regex[i] == ')') {
				int OR = ops.top();
				ops.pop();
				if (regex[OR] == '|') {
					lp = ops.top();
					ops.pop();
					g.addEdge(lp,OR+1);
					g.addEdge(OR,i);
				} else if (regex[OR] == '(') {
					lp = OR;
				}
			}
			if (i < M-1 && regex[i+1] == '*') {
				g.addEdge(lp,i+1);
				g.addEdge(i+1,lp);
			}
			if (regex[i] == '(' || regex[i] == '*' || regex[i] == ')') {
				g.addEdge(i,i+1);
			}
			
		}

	}

	bool recognizes(string txt) {
		set<int> pc;
		DirectedDFS dfs1(g,0);
		for (int v = 0; v < g.getNumOfVertices(); v++) {
			if (dfs1.isMarked(v)) {
				pc.insert(v);
			}
		}

		for (int i=0;i<SZ(txt);i++) {
			set<int> match;
			foreach(v,pc) {
				if (*v == M) continue;
				if (regex[*v] == txt[i] || regex[*v] == '.') {
					match.insert(*v+1);
				}
			}
			pc.clear();
			vector<int> pcv(match.begin(), match.end());
			DirectedDFS ndfs(g,pcv);
			pc = set<int>();


			for (int v=0;v<g.getNumOfVertices();v++) {
				if (ndfs.isMarked(v)) {
					pc.insert(v);
				}
			}
			if (pc.size() == 0) return false;
		}
		foreach(v,pc) {
			if (*v == M) {
				return true;
			}
		}
		return false;
			
	}
};

int main() {
	string regex="((a|b)|c)*", txt="acaaab";
	// cout<<"Regular expression : ";
	// cin >> regex;

	// cout<<"Text : ";
	// cin >> txt;
	NFA n(regex);
	if (n.recognizes(txt)) 
		cout<<"yes";
	else 
		cout<<"no";
	return 0;
}


