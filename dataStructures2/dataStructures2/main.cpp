#include "stdafx.h"
#include <tuple>
#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include "Graph.h"

using namespace std;



//typedef GraphArc<pair<string, int>, int > Arc;
//typedef GraphNode<pair<string, int>, int > Node;
//const int size = 3;
//typedef GraphArc<array<char, 3>, int > Arc;
//typedef GraphNode<array<char, 3>, int > Node;
typedef GraphNode<tuple<string, int, int, int, int>, int> Node;
typedef GraphArc<tuple<string, int, int, int, int>, int> Arc;


void visit(Node * pNode) {

	cout << "Visiting: Data= " << get<0>(pNode->data()) << " gN= " << get<1>(pNode->data()) << " hN = " << get<2>(pNode->data()) <<endl;
}

int main(int argc, char *argv[]) {
   
    Graph<tuple<string, int, int, int, int>,int>graph( 30 );
	

	string c;

	int i = 0;
	int x;
	int y;
	ifstream myfile;
	myfile.open ("nodes.txt");

	//For each node v in graph G
	//Calculate h[v] // Compute estimated distance to goal for each node.
	//Initialise g[v] to infinity // Don’t yet know the distances to these nodes 
	while ( myfile >> c >> x >> y) {
		graph.addNode(tuple<string, int, int, int, int>(c, 99999999, 99999999, x, y), i++);
	}

	myfile.close();
	myfile.open("arcs.txt");

	int from, to, weight;
	while ( myfile >> from >> to >> weight ) {
		graph.addArc(from, to, weight);
	}
    myfile.close();

	std::vector<Node*> path;
	cout << '\n' << "A*: " << endl;
	graph.clearMarks();
	
	path.clear();
	graph.aStar(graph.nodeArray()[0], graph.nodeArray()[1], visit, path, 30);

	for (Node* n : path)
		visit(n);


	system("PAUSE");
}
