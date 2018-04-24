#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <sstream>

const double PI = 3.14159265359;
const double FULL = 2 * PI;
const double RADIUS = 300;
const double CENTER = 400;

inline sf::Vector2f coords(const int &vertex, const double &angle)
{
	double curr_angle = vertex *angle*PI/180;
	return sf::Vector2f(400+RADIUS*cos(curr_angle),400+RADIUS*sin(curr_angle));
}


int main()
{

	std::vector<std::pair<int, int>> paths;
	std::vector<int> hamiltonVertexes;
	std::vector<std::pair<int, int>> eulerEdges;

	std::fstream file("saveToDraw.txt", std::ios::in);
	if (!file.good())
		return 0;
	bool euler_mode = false;
	bool hamilton_mode = false;
	std::string line;
	std::stringstream ss;
	std::getline(file, line);
	if (line == "type: Euler")
		euler_mode = true;
	else if (line == "type: Hamilton")
		hamilton_mode = true;
	/////////////////////////////////////////////////////////////
	//parsing
	ss.str(line);
	int numVertex;
	int numPaths;

	//different modes
	if (euler_mode || hamilton_mode) {

		std::getline(file, line);
		ss.str(line);
		if (euler_mode) {
			int v1, v2;
			ss >> v1;
			while (ss >> v2) {
				eulerEdges.push_back(std::make_pair(v1-1, v2-1));
				v1 = v2;
			}

			///
			for (auto &p : eulerEdges) {
				std::cout << p.first << " " << p.second << std::endl;
			}
			system("pause");

			///
		}
		else {
			int v;
			while (ss >> v) {
				hamiltonVertexes.push_back(v-1);
			}
			///
			for (auto &p : hamiltonVertexes) {
				std::cout << p << " ";
			}
			system("pause");

			///
		}
		file >> numVertex;
		file >> numPaths;
	}

	//default mode
	else {
		ss >> numVertex;
		ss >> numPaths;
	}


	int temp, temp2;
	for (int i = 0; i < numPaths; ++i) {
		file >> temp;
		file >> temp2;
		paths.push_back(std::make_pair(temp, temp2));
	}
	
	file.close();
	////////////////////////////////////////////////////////////////////

	const double angle = 360 / numVertex;
	sf::RenderWindow window(sf::VideoMode(800, 800), "Ilustracja grafu", sf::Style::Titlebar | sf::Style::Close);


	sf::CircleShape kolo;
	kolo.setRadius(30); kolo.setFillColor(sf::Color::Cyan); kolo.setOutlineThickness(5);
	kolo.setOutlineColor(sf::Color::Black);

	sf::VertexArray linia;
	linia = sf::VertexArray(sf::LinesStrip, 2);
	linia[0].color = sf::Color::Black; 
	linia[1].color = sf::Color::Black; 

	std::vector<sf::VertexArray> lines;
	std::vector<sf::CircleShape> circles;
	std::vector<sf::Text> texts;

	sf::Vector2f vertex1;
	sf::Vector2f vertex2;

	sf::Font font;
	font.loadFromFile("tahoma.ttf");
	sf::Text text; text.setFont(font); text.setOutlineColor(sf::Color::Black); text.setOutlineThickness(1);

	sf::Text text2; text2.setFont(font); text2.setFillColor(sf::Color::Red);

	for (unsigned i = 0; i < numVertex; ++i) {
		circles.push_back(kolo);
		texts.push_back(text);
		vertex1 = coords(i, angle);
		circles[i].setPosition(vertex1 - sf::Vector2f(30, 30));
		texts[i].setPosition(vertex1 - sf::Vector2f(10, 18));
		texts[i].setString(std::to_string(i+1));
	}

	if (hamilton_mode) {
		for (unsigned i = 0; i < numVertex; ++i) {

			text2.setString(std::to_string(i+1));
			vertex1 = coords(hamiltonVertexes[i], angle);
			text2.setPosition(vertex1 + sf::Vector2f(40, -30));
			texts.push_back(text2);
		}
	}
	




	std::cout << paths.size();
	int euler_counter = 1;

	for (unsigned i = 0; i < numPaths ; ++i) {

		lines.push_back(linia);
		vertex1 = coords(paths[i].first-1,angle);
		std::cout << paths[i].first << " "<<vertex1.x<<"   "<<vertex1.y << std::endl;
		vertex2 = coords(paths[i].second-1, angle);
		std::cout << paths[i].second << " " << vertex2.x << "   " << vertex2.y << std::endl;
		lines[i][0].position = vertex1;
		lines[i][1].position = vertex2;
	}

	//nadpisanie lini bo chyba to najszybsza metoda
	if (euler_mode) {
		for (unsigned i = 0; i < eulerEdges.size(); ++i) {
			vertex1 = coords(eulerEdges[i].first, angle);
			vertex2 = coords(eulerEdges[i].second, angle);
			text2.setString(std::to_string(i + 1));
			text2.setPosition(sf::Vector2f((vertex1.x + vertex2.x) / 2.0, (vertex1.y + vertex2.y) / 2.0));
			texts.push_back(text2);
		}
	}



	//bool completed = false;

	sf::Vector2u size = window.getSize();
	sf::Texture texture; texture.create(size.x,size.y);
	sf::Sprite sprite;
	bool saving_mode = false;

	//debug mode
	window.setVerticalSyncEnabled(true);
	while (window.isOpen()) {
		sf::Event event;
		window.clear(sf::Color::White);
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
				std::cout << "s pressed" << std::endl;
				saving_mode = true;

			}
		}

		//drawing
		for (const sf::VertexArray& line : lines)
			window.draw(line);
		for (const sf::CircleShape& circ : circles)
			window.draw(circ);
		for (const sf::Text &tex : texts)
			window.draw(tex);
		
		if (saving_mode) {
			texture.update(window);
			sf::Image img = texture.copyToImage();
			img.saveToFile("result.png");
			saving_mode = false;
		}

		window.display();
	}


	//debug mode
	return 0;
}