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
	vector<sf::CircleShape> aFillCircles;
	vector<sf::VertexArray> lines2;
	vector<sf::CircleShape> selectCircles;
	vector<sf::Text> placeNames;
	vector<sf::Text> weightList;
	
	bool start = false, end = false;
	string c;
	int i = 0;
	int startNode = 0 , endNode = 1;
	int x;
	int y;
	int fillMax = 0;
	int aFillMax = 0;
	int lineMax = 0;
	int circleMax = 0;
	int selectMax = 0;
	int nameCount = 0;
	int weightCount = 0;
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

		sf::Text weights;
		weights.setFont(font);
		weights.setString(std::to_string(weight));
		weights.setColor(sf::Color::Green);
		weights.setPosition(((lines[0].position.x + lines[1].position.x)/2) - 10, ((lines[0].position.y+lines[1].position.y)/2) - 10);
		weights.setCharacterSize(20);
		weightList.push_back(weights);
		weightCount++;
	}
    myfile.close();

	std::vector<Node*> path;
	std::vector<Node*> attemptedPath;
	cout << '\n' << "A*: " << endl;
	graph.clearMarks();
	
	path.clear();

	//graph.aStar(graph.nodeArray()[0], graph.nodeArray()[15], visit, path, 30);

	sf::Texture startButtonTexture;
	startButtonTexture.loadFromFile("startbutton.png");
	sf::Texture resetButtonTexture;
	resetButtonTexture.loadFromFile("resetbutton.png");
	sf::Sprite startButton;
	startButton.setTexture(startButtonTexture);
	startButton.setPosition(sf::Vector2f(10, 450));
	startButton.setTextureRect(sf::IntRect(0, 0, startButtonTexture.getSize().x, startButtonTexture.getSize().y));
	sf::Sprite resetButton;
	resetButton.setTexture(resetButtonTexture);
	resetButton.setPosition(sf::Vector2f(110, 450));
	resetButton.setTextureRect(sf::IntRect(0, 0, resetButtonTexture.getSize().x, resetButtonTexture.getSize().y));

	bool startClick = false;
	bool resetClick = false;
	bool selectNewNodes = true;

	for (int i = 0; i < 30; i++) {
		sf::CircleShape circle(20);
		circle.setPosition(get<3>(graph.nodeArray()[i]->data()), get<4>(graph.nodeArray()[i]->data()));
		circles.push_back(circle);
		circleMax++;

		//create a formatted text string
		sf::Text placeName;
		placeName.setFont(font);
		placeName.setString(get<0>(graph.nodeArray()[i]->data()));
		placeName.setColor(sf::Color::Black);
		placeName.setPosition(get<3>(graph.nodeArray()[i]->data()) + 15, get<4>(graph.nodeArray()[i]->data()) + 6);
		placeName.setCharacterSize(20);
		placeNames.push_back(placeName);
		nameCount++;
		//visit(n);
	}

	while (window.isOpen())
	{

		int mouseX = sf::Mouse::getPosition(window).x - 10;
		int mouseY = sf::Mouse::getPosition(window).y - 10;

		sf::CircleShape mouseCircle(10);
		mouseCircle.setPosition(sf::Vector2f(mouseX, mouseY));


		if (selectNewNodes == true){
			for (int i = 0; i < 30; i++)
			{
				if (mouseCircle.getGlobalBounds().intersects(circles[i].getGlobalBounds()) && start == false && sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					start = true;
					startNode = i;
					sf::CircleShape circle(20);
					circle.setPosition(circles[i].getPosition().x, circles[i].getPosition().y);
					circle.setFillColor(sf::Color::Yellow);
					selectCircles.push_back(circle);
					selectMax++;
				}

				else if (mouseCircle.getGlobalBounds().intersects(circles[i].getGlobalBounds()) && start == true && sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					end = true;
					endNode = i;
					sf::CircleShape circle(20);
					circle.setPosition(circles[i].getPosition().x, circles[i].getPosition().y);
					circle.setFillColor(sf::Color::Yellow);
					selectCircles.push_back(circle);
					selectMax++;
				}
			}
		}
		sf::Event Event;
		while (window.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
				window.close();


			if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
				window.close();
		}
		if (mouseCircle.getGlobalBounds().intersects(startButton.getGlobalBounds()) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			startClick = true;
		}
		if (mouseCircle.getGlobalBounds().intersects(resetButton.getGlobalBounds()) && sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			resetClick = true;
		}
		if (startClick == true && start == true && end == true) {
			graph.aStar(graph.nodeArray()[startNode], graph.nodeArray()[endNode], visit, path, 30, attemptedPath);
			selectNewNodes = false;
			for (Node* n : path) {
				sf::CircleShape circle(20);
				circle.setPosition(get<3>(n->data()), get<4>(n->data()));
				circle.setFillColor(sf::Color::Red);
				fillCircles.push_back(circle);
				fillMax++;
			}

			for (Node* p : attemptedPath) {
				sf::CircleShape circle2(20);
				circle2.setPosition(get<3>(p->data()), get<4>(p->data()));
				circle2.setFillColor(sf::Color::Blue);
				aFillCircles.push_back(circle2);
				aFillMax++;
				//visit(n);
			}
			std::cout << "/////////////////////////////////////////////////////////" << endl << "/////////////////////////////////////////////////////////" << endl;
			startClick = false;
			start = false;
			end = false;
		}
		if (resetClick){
			startClick = false;
			start = false;
			end = false;
			resetClick = false;
			fillCircles.clear();
			aFillCircles.clear();
			selectCircles.clear();
			path.clear();
			attemptedPath.clear();
			endNode = 0;
			startNode = 0;
			for (int i = 0; i < 30; i++){
				graph.nodeArray()[i]->setMarked(false);
				graph.nodeArray()[i]->setPrevious(NULL);
			}
			selectNewNodes = true;
		}


		window.clear();
		for (int i = 0; i < lines2.size(); i++)
		{
			window.draw(lines2[i]);
		}

		for (int i = 0; i < circles.size(); i++) {

			window.draw(circles[i]);
		}

		for (int i = 0; i < aFillCircles.size(); i++) {

			window.draw(aFillCircles[i]);
		}

		for (int i = 0; i < fillCircles.size(); i++) {

			window.draw(fillCircles[i]);
		}
		for (int i = 0; i < selectCircles.size(); i++) {

			window.draw(selectCircles[i]);
		}

		for (int i = 0; i < placeNames.size(); i++) {
			window.draw(placeNames[i]);
		}
		for (int i = 0; i < weightList.size(); i++) {
			window.draw(weightList[i]);
		}

		window.draw(mouseCircle);
		window.draw(startButton);
		window.draw(resetButton);
		window.display();
	}
	return EXIT_SUCCESS;
}

