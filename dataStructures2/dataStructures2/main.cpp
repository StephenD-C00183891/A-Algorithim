#include "stdafx.h" 
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 

#include "SFML/Graphics.hpp" 
#include "SFML/OpenGL.hpp" 
#include <iostream> 
#define _USE_MATH_DEFINES
#include <math.h>
#include <tuple>
#include <fstream>
#include <string>
#include "Graph.h"

using namespace std;


typedef GraphNode<tuple<string, int, int, int, int>, int> Node;
//typedef GraphArc<tuple<string, int, int, int, int>, int> Arc;


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

	myfile.open ("nodes2.txt");
	while ( myfile >> c >> x >> y) {
		graph.addNode(tuple<string, int, int, int, int>(c, 99999999, 99999999, x, y), i++);
	}
	myfile.close();


	myfile.open("arcs2.txt");
	int from, to, weight;
	while ( myfile >> from >> to >> weight ) {
		graph.addArc(from, to, weight);
	}
    myfile.close();

	std::vector<Node*> path;
	cout << '\n' << "A*: " << endl;
	graph.clearMarks();
	
	path.clear();

	graph.aStar(graph.nodeArray()[0], graph.nodeArray()[6], visit, path, 30);

	for (Node* n : path)
		visit(n);


	system("PAUSE");
}
