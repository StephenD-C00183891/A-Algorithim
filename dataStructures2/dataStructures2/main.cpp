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
	int lineMax = 0;
	int circleMax = 0;
	ifstream myfile;

	

	

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
		lineMax++;
		lines2.push_back(lines);
	}
    myfile.close();

	std::vector<Node*> path;
	cout << '\n' << "A*: " << endl;
	graph.clearMarks();
	
	path.clear();

	//graph.aStar(graph.nodeArray()[0], graph.nodeArray()[15], visit, path, 30);

	for (Node* n : path) {
		sf::CircleShape circle(20);
		circle.setPosition(get<3>(n->data()), get<4>(n->data()));
		circle.setFillColor(sf::Color::Red);
		fillCircles.push_back(circle);
		fillMax++;
		//visit(n);
	}

	for (int i = 0; i < 30; i++) {
		sf::CircleShape circle(20);
		circle.setPosition(get<3>(graph.nodeArray()[i]->data()), get<4>(graph.nodeArray()[i]->data()));
		circles.push_back(circle);
		circleMax++;
	}

	while (window.isOpen())
	{
		int mouseX = sf::Mouse::getPosition().x;
		int mouseY = sf::Mouse::getPosition().y;

		sf::CircleShape mouseCircle;
		mouseCircle.setPosition(sf::Vector2f(mouseX, mouseY));

		for (int i = 0; i < 30; i++)
		{
			if (mouseCircle.getGlobalBounds().intersects(circles[i].getGlobalBounds()))
			{
				//start = true;
				startNode = i;
				graph.aStar(graph.nodeArray()[startNode], graph.nodeArray()[15], visit, path, 30);
			}
		}

		sf::Event Event;
		while (window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
				window.close();


			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				window.close();

			
			/*}
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

		for (int i = 0; i < lineMax; i++)
		{
			window.draw(lines2[i]);
		}

		for (int i = 0; i < circleMax; i++) {

			window.draw(circles[i]);
		}

		for (int i = 0; i < fillMax; i++) {

			window.draw(fillCircles[i]);
		}

		window.display();
	}
	return EXIT_SUCCESS;
}

