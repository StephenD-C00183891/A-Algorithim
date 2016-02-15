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
	// Create the main window 
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "SFML First Program");

	//load a font
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\GARA.TTF");

	window.clear();

    Graph<tuple<string, int, int, int, int>,int>graph( 30 );
	vector<sf::CircleShape> circles;
	vector<sf::CircleShape> fillCircles;
	vector<sf::VertexArray> lines2;
	vector<sf::CircleShape> selectCircles;
	
	bool start = false, end = false;
	string c;
	int i = 0;
	int startNode = 0 , endNode = 1;
	int x;
	int y;
	int fillMax = 0;
	ifstream myfile;

	sf::CircleShape endCircle(20);
	sf::CircleShape startCircle(20);

	int mouseX = sf::Mouse::getPosition().x;
	int mouseY = sf::Mouse::getPosition().y;

	myfile.open ("nodes2.txt");
	while ( myfile >> c >> x >> y) {
		graph.addNode(tuple<string, int, int, int, int>(c, 99999999, 99999999, x, y), i++);
	}
	myfile.close();


	myfile.open("arcs2.txt");
	int from, to, weight;
	while (myfile >> from >> to >> weight) {
		graph.addArc(from, to, weight);

		sf::VertexArray lines(sf::LinesStrip, 2);
		lines[0].position = sf::Vector2f(get<3>(graph.nodeArray()[from]->data()) + 20, get<4>(graph.nodeArray()[from]->data()) + 20);
		lines[0].color = sf::Color::Red;
		lines[1].position = sf::Vector2f(get<3>(graph.nodeArray()[to]->data()) + 20, get<4>(graph.nodeArray()[to]->data()) + 20);
		lines[1].color = sf::Color::Red;

		lines2.push_back(lines);
		//window.draw(lines);
	}
    myfile.close();

	std::vector<Node*> path;
	cout << '\n' << "A*: " << endl;
	graph.clearMarks();
	
	path.clear();

	graph.aStar(graph.nodeArray()[0], graph.nodeArray()[1], visit, path, 30);
	//graph.aStar(graph.nodeArray()[0], graph.nodeArray()[4], visit, path, 30);

	for (Node* n : path) {
		sf::CircleShape circle(20);
		circle.setPosition(get<3>(n->data()), get<4>(n->data()));
		circle.setFillColor(sf::Color::Red);
		fillCircles.push_back(circle);
		fillMax++;
		visit(n);
	}


	for (int i = 0; i < 30; i++) {
		sf::CircleShape circle(20);
		circle.setPosition(get<3>(graph.nodeArray()[i]->data()), get<4>(graph.nodeArray()[i]->data()));
		circles.push_back(circle);

	}

	// Start game loop 
	while (window.isOpen())
	{

		if (start == true && end == true)
		{
			
		}
		// Process events 
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			// Close window : exit 
			if (Event.type == sf::Event::Closed)
				window.close();

			// Escape key : exit 
			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				window.close();

			/*for (int i = 0; i < 30; i++)
			{
				if (mouseX > circles[i].getPosition().x && mouseX < circles[i].getPosition().x + 20
					&& mouseY > circles[i].getPosition().y && mouseY < circles[i].getPosition().y + 20
					&& start == false && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					start = true;
					startNode = i;
					
					startCircle.setPosition(get<3>(graph.nodeArray()[i]->data()), get<4>(graph.nodeArray()[i]->data()));
					startCircle.setFillColor(sf::Color::Blue);


				}
				else if (mouseX > circles[i].getPosition().x && mouseX < circles[i].getPosition().x + 20
					&& mouseY > circles[i].getPosition().y && mouseY < circles[i].getPosition().y + 20
					&& start == true && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					end = true;
					endNode = i;
					
					endCircle.setPosition(get<3>(graph.nodeArray()[i]->data()), get<4>(graph.nodeArray()[i]->data()));
					endCircle.setFillColor(sf::Color::Blue);

				}
			}*/
		}
		//window.clear();

		for (int i = 0; i < 74; i++)
		{
			window.draw(lines2[i]);
		}

		for (int i = 0; i < 30; i++) {

			window.draw(circles[i]);
		}

		for (int i = 0; i < fillMax; i++) {

			window.draw(fillCircles[i]);
		}

		//window.draw(startCircle);
		//window.draw(endCircle);
		

		window.display();
	}

	return EXIT_SUCCESS;
}

