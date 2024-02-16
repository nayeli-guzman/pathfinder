#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace sf;

class Menu {
private:
	int height, width, bottom;
	RenderWindow& window;
	RectangleShape b_menu, b_algorithms, b_selector, b_help;
	Font font_1, font_2, font_3;
	Text text_1, text_2, text_3, text_4, text_5, text_6, text_7, text_8;
	string t1 = "Bienvenido"
		, t2 = "Presione 'M' para abrir el Menu"
		, t3 = "   Menu"
		, t4 = "Presione las siguientes teclas de acuerdo al \n\n          algoritmo que quiera ejecutar"
		, t5 = ". . .  seleccione un algoritmo  . . ." // dynamic text
		, t6 = "Para ejecutar el algoritmo seleccionado, usted"
		"\n\n          debe presionar 'ESPACIO'."
		"\n\n\n\nAntes de esto el inicio y fin deben definirse"
		, t7 = "[click] \t Inicio"
		"\n\n[anticlick]\t Fin"
		, t8 = "Para reiniciar el mapa y ejecutar otro algoritmo, \n\n	debe presionar 'C' y repetir el proceso\n\n				 de seleccion"
		;
	vector<string> help_alg{ "[1]\t Dijkstra", "[2]\t BFS", "[3]\t DFS" };
	vector<Text> help_txt;

	vector<Text> alg_txt;
	vector<string> algorithms{ "Dijkstra", "DFS", "BFS" };
	bool help_w = false;



public:

	Menu(int h, int w, int b, RenderWindow& wd)
		: height(h), width(w), bottom(b), window(wd) {

		// setting fonts for every str
		font_1.loadFromFile("sprites/font_headers.TTF");
		font_2.loadFromFile("sprites/font_body.ttf");
		font_3.loadFromFile("sprites/font_normal.ttf");

		setBackgrounds();
		setTexts();
		setTextHelpWindow();

	}

	void setBackgrounds() {

		float radius = 0.2;

		// background of the header

		b_menu = RectangleShape(Vector2f(width, height));
		b_menu.setFillColor(Color(60, 100, 250, 100));
		b_menu.setPosition(0, 0);

		// background of algorithms
		b_algorithms = RectangleShape(Vector2f((width - 2 * NODE_SIZE), 50));
		b_algorithms.setFillColor(Color(220, 200, 40, 200));
		b_algorithms.setPosition(NODE_SIZE, 140);

		// background - selector of algorithm it'll be dynamic
		b_selector = RectangleShape(Vector2f(NODE_SIZE * 6, 40));
		b_selector.setFillColor(Color(175, 80, 0, 255));
		b_selector.setPosition(width / 2 - 3 * NODE_SIZE, 145);

		// background of help window
		b_help = RectangleShape(Vector2f(NODE_SIZE * 21, NODE_SIZE * 15));
		b_help.setFillColor(Color(55, 55, 55, 255));
		b_help.setPosition(width / 2 - 10.5 * NODE_SIZE, 150);


	}

	void setTexts() {


		// main text 
		text_1.setFont(font_1);
		text_1.setString(t1);
		text_1.setCharacterSize(45);
		text_1.setPosition(2 * NODE_SIZE, 30);

		// setting position for each algorithm - txt
		alg_txt.resize(3);

		for (int i = 0; i < alg_txt.size(); i++) {
			alg_txt[i].setFont(font_1);
			alg_txt[i].setString(algorithms[i]);
			alg_txt[i].setCharacterSize(20);
		}
		alg_txt[0].setPosition(width / 2 - 12.5 * NODE_SIZE, 150);
		alg_txt[1].setPosition(width / 2 - 1.5 / 2 * NODE_SIZE, 150);
		alg_txt[2].setPosition(width / 2 + 10.25 * NODE_SIZE, 150);

		// for help
		text_2.setFont(font_2);
		text_2.setString(t2);
		text_2.setCharacterSize(13);
		text_2.setPosition(2 * NODE_SIZE, 90);

		// dynamic text / 

		text_5.setFont(font_3);
		text_5.setString(t5);
		text_5.setCharacterSize(23);
		text_5.setPosition(width / 2 + 2 * NODE_SIZE, NODE_SIZE * 1.5);



	}

	void setTextHelpWindow() {

		// content of help window
		text_3.setFont(font_1);
		text_3.setString(t3);
		text_3.setCharacterSize(30);
		text_3.setPosition(width / 2 - 3.25 * NODE_SIZE, 150 + 1 * NODE_SIZE);

		text_4.setFont(font_2);
		text_4.setString(t4);
		text_4.setCharacterSize(15);
		text_4.setPosition(width / 2 - 8.5 * NODE_SIZE, 150 + 2.5 * NODE_SIZE);

		help_txt.resize(3);

		for (int i = 0, j = 0; i < alg_txt.size(); i++, j = j + 1.5) {
			help_txt[i].setFont(font_2);
			help_txt[i].setString(help_alg[i]);
			help_txt[i].setCharacterSize(15);
			help_txt[i].setPosition(width / 2 - 3 * NODE_SIZE, 150 + (4.5 + j) * NODE_SIZE);
		}

		text_6.setFont(font_2);
		text_6.setString(t6);
		text_6.setCharacterSize(15);
		text_6.setPosition(width / 2 - 8.75 * NODE_SIZE, 150 + 7.8 * NODE_SIZE);

		text_7.setFont(font_2);
		text_7.setString(t7);
		text_7.setCharacterSize(15);
		text_7.setPosition(width / 2 - 4.5 * NODE_SIZE, 150 + 10.9 * NODE_SIZE);

		text_8.setFont(font_2);
		text_8.setString(t8);
		text_8.setCharacterSize(15);
		text_8.setPosition(width / 2 - 8.75 * NODE_SIZE, 150 + 12.7 * NODE_SIZE);
	}

	void changeDynamicText(int index) {

		switch (index) {
		case 1:
			t5 = "seleccione un inicio y/o fin";
			break;
		case 2:
			t5 = "ejecutando Dijkstra";
			break;
		case 3:
			t5 = "ejecutando DFS";
			break;
		case 4:
			t5 = "ejecutando BFS";
			break;
		default:
			break;
		}

		text_5.setString(t5);

	}

	void updateSelector(int i) {
		switch (i) {
		case 1:
			b_selector.setPosition(width / 2 - 14 * NODE_SIZE, 145);
			break;
		case 2:
			b_selector.setPosition(width / 2 - 3 * NODE_SIZE, 145);
			break;
		case 3:
			b_selector.setPosition(width / 2 + 8 * NODE_SIZE, 145);
			break;
		default:
			break;
		}
	}

	void activate_help_window(bool c) {
		help_w = c;
	}

	void draw() {

		window.draw(b_menu);
		window.draw(b_algorithms);
		window.draw(b_selector);

		window.draw(text_1);
		window.draw(text_2);
		window.draw(text_5);

		for_each(alg_txt.begin(), alg_txt.end(), [=](Text x) {window.draw(x); });

		if (help_w) {
			window.draw(b_help);
			window.draw(text_3);
			window.draw(text_4);
			for_each(help_txt.begin(), help_txt.end(), [=](Text x) {window.draw(x); });
			window.draw(text_6);
			window.draw(text_7);
			window.draw(text_8);
		}

		window.display();

	}
};

