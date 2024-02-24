#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include <map>
#include <iostream>

using namespace std;
using namespace sf;

class MenuAbstract {
protected:
	Font font_1, font_2, font_3;
public:

	MenuAbstract() {
		font_1.loadFromFile("sprites/font_headers.TTF");
		font_2.loadFromFile("sprites/font_body.ttf");
		font_3.loadFromFile("sprites/font_normal.ttf");
	}

	virtual void changeDynamicText(int, double) = 0;
	virtual void updateSelector(int) = 0;
	virtual void activate_help_window(bool) = 0;
	virtual void activate_comparison_table(bool) = 0;
	virtual void draw() = 0;
	virtual void append(string, double) = 0;
	virtual bool get_activate_help_window() = 0;
	virtual bool get_activate_comparison() = 0;

};

class Menu : public MenuAbstract {
private:

	int height, width, bottom;
	RenderWindow& window;
	RectangleShape b_menu, b_algorithms, b_selector;
	Text text_1, text_2, text_3, text_9;
	string t1 = "Bienvenido"
		, t2 = "Presione 'M' para abrir el menu"
		, t3 = ". . .  seleccione un algoritmo  . . ." // dynamic text
		;
	vector<string> algorithms{ "Dijkstra", "DFS", "BFS" };
	vector<Text> alg_txt;
	bool help_w = false, comparison_t = false;

public:

	Menu(int h, int w, int b, RenderWindow& wd)
		: height(h), width(w), bottom(b), window(wd) {

		setBackgrounds();
		setTexts();
		//setTextComparison();

	}

	~Menu() {}

	void setBackgrounds() {

		// background of the header

		b_menu = RectangleShape(Vector2f(width, height));
		b_menu.setFillColor(Color(150, 160, 210, 255));
		b_menu.setPosition(0, 0);

		// background of algorithms
		b_algorithms = RectangleShape(Vector2f((width - 2 * NODE_SIZE), 50));
		b_algorithms.setFillColor(Color(220, 200, 40, 200));
		b_algorithms.setPosition(NODE_SIZE, 140);

		// background - selector of algorithm it'll be dynamic
		b_selector = RectangleShape(Vector2f(NODE_SIZE * 6, 40));
		b_selector.setFillColor(Color(175, 80, 0, 255));
		b_selector.setPosition(width / 2 - 3 * NODE_SIZE, 145);

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

		text_3.setFont(font_3);
		text_3.setString(t3);
		text_3.setCharacterSize(23);
		text_3.setPosition(width / 2 + 2 * NODE_SIZE, NODE_SIZE * 1.5);

	}

	void changeDynamicText(int index, double t) override {

		switch (index) {
		case 1:
			t3 = "seleccione un inicio y/o fin";
			break;
		case 2:
			t3 = "ejecutando Dijkstra";
			break;
		case 3:
			t3 = "ejecutando DFS";
			break;
		case 4:
			t3 = "ejecutando BFS";
			break;
		case 5:
			t3 = "Dijkstra tomó " + to_string(t) + " ms";
			break;
		case 6:
			t3 = "DFS tomó " + to_string(t) + " ms";
			break;
		case 7:
			t3 = "BFS tomó " + to_string(t) + " ms";
			break;
		default:
			break;
		}

		text_3.setString(t3);

	}

	void updateSelector(int i) override {
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

	void activate_help_window(bool c) override {
		help_w = c;
	}

	void activate_comparison_table(bool c) override {
		comparison_t = c;
	}

	void draw() override {

		window.draw(b_menu);
		window.draw(b_algorithms);
		window.draw(b_selector);

		window.draw(text_1);
		window.draw(text_2);
		window.draw(text_3);

		for_each(alg_txt.begin(), alg_txt.end(), [=](Text x) {window.draw(x); });

		window.display();

	}

	void append(string algoritmo, double time) override {}

	bool get_activate_help_window() override {
		return help_w;
	}

	bool get_activate_comparison() override {
		return comparison_t;
	}

};

class Decorator : public MenuAbstract {
protected:
	MenuAbstract* menu;
public:

	Decorator(MenuAbstract* menu) : menu(menu) {}

	~Decorator() {
		menu = nullptr;
		delete menu;
	}

	void changeDynamicText(int, double) override {};
	void updateSelector(int) override {};
	void activate_help_window(bool c) override {};
	void activate_comparison_table(bool c) override {};
	void draw() override {
		menu->draw();
	};
	void append(string, double) override {};
};

class HelpWindowMenu : public Decorator {

	int height = HEADER_HEIGHT, width = NODE_SIZE * COLS, bottom;
	RenderWindow& window;
	RectangleShape b_help;

	Text text_1, text_2, text_3, text_4, text_5, text_6;
	string t1 = "Informacion"
		, t2 = "Presione las siguientes teclas de acuerdo al \n\n          algoritmo que quiera ejecutar"
		, t3 = "  Debe presionar 'ESPACIO' para ejecutarlos"
		"\n\n\n\nAntes de esto el inicio y fin deben definirse"
		, t4 = "[click] \t Inicio \t\t[anticlick]\t Fin"
		, t5 = "Para limpiar el mapa, presione 'C'"
		, t6 = "[up, down]\t Cambiar tamaño"
		"\n\n\n\n[right, left]\t Cambiar mapa"
		;
	vector<string> help_alg{ "[1]\t Dijkstra \t\t\t [2]\t BFS", "[3]\t DFS  \t\t\t\t [4]\t Todos" };
	vector<Text> help_txt;
public:

	HelpWindowMenu(MenuAbstract* menu, RenderWindow& wd) : Decorator(menu), window(wd) {

		setBackgrounds();
		setTexts();
	}

	~HelpWindowMenu() {}

	void setBackgrounds() {
		b_help = RectangleShape(Vector2f(NODE_SIZE * 21, NODE_SIZE * 15));
		b_help.setFillColor(Color(55, 55, 55, 255));
		b_help.setPosition(width / 2 - 10.5 * NODE_SIZE, 150);
	};

	void setTexts() {

		text_1.setFont(font_1);
		text_1.setString(t1);
		text_1.setCharacterSize(30);
		text_1.setPosition(width / 2 - 3.25 * NODE_SIZE, 150 + 1 * NODE_SIZE);

		text_2.setFont(font_2);
		text_2.setString(t2);
		text_2.setCharacterSize(13);
		text_2.setPosition(width / 2 - 7 * NODE_SIZE, 150 + 2.5 * NODE_SIZE);

		help_txt.resize(2);

		for (int i = 0, j = 0; i < help_alg.size(); i++, j = j + 1.5) {
			help_txt[i].setFont(font_2);
			help_txt[i].setString(help_alg[i]);
			help_txt[i].setCharacterSize(13);
			help_txt[i].setPosition(width / 2 - 7 * NODE_SIZE, 150 + (4 + j) * NODE_SIZE);
		}

		text_3.setFont(font_2);
		text_3.setString(t3);
		text_3.setCharacterSize(13);
		text_3.setPosition(width / 2 - 7.5 * NODE_SIZE, 150 + 6.5 * NODE_SIZE);

		text_4.setFont(font_2);
		text_4.setString(t4);
		text_4.setCharacterSize(13);
		text_4.setPosition(width / 2 - 8 * NODE_SIZE, 150 + 8.7 * NODE_SIZE);

		text_5.setFont(font_2);
		text_5.setString(t5);
		text_5.setCharacterSize(13);
		text_5.setPosition(width / 2 - 5.5 * NODE_SIZE, 150 + 10 * NODE_SIZE);

		text_6.setFont(font_2);
		text_6.setString(t6);
		text_6.setCharacterSize(13);
		text_6.setPosition(width / 2 - 5 * NODE_SIZE, 150 + 11.5 * NODE_SIZE);

	}

	void draw() override {

		Decorator::draw();

		window.draw(b_help);
		window.draw(text_1);
		window.draw(text_2);
		for_each(help_txt.begin(), help_txt.end(), [=](Text x) {window.draw(x); });
		window.draw(text_3);
		window.draw(text_4);
		window.draw(text_5);
		window.draw(text_6);

		window.display();

	};

	bool get_activate_help_window() override { return 3; }
	bool get_activate_comparison() override { return 3; }

};

class ComparisonTableMenu : public Decorator {

private:

	int height = HEADER_HEIGHT, width = NODE_SIZE * COLS;
	RenderWindow& window;
	RectangleShape b_table;
	Text text_9;
	string t9 = "Comparacion";

	map<string, double> times;

	vector<string> times_str;

	vector<Text> times_txt;
	vector<string> algorithms{ "Dijkstra", "DFS", "BFS" };
	bool comparison_t = false;

public:

	ComparisonTableMenu(MenuAbstract* menu, RenderWindow& wd) : Decorator(menu), window(wd) {
		setBackgrounds();
	}

	~ComparisonTableMenu() {}

	void setBackgrounds() {
		b_table = RectangleShape(Vector2f(NODE_SIZE * 21, NODE_SIZE * 15));
		b_table.setFillColor(Color(55, 55, 55, 255));
		b_table.setPosition(width / 2 - 10.5 * NODE_SIZE, 150);
	};

	void setTexts() {

		text_9.setFont(font_1);
		text_9.setString(t9);
		text_9.setCharacterSize(30);
		text_9.setPosition(width / 2 - 3.25 * NODE_SIZE, 150 + 1.5 * NODE_SIZE);

		times_str.resize(4);
		times_txt.resize(4);
		times_str[0] = "Algoritmo\t\t\tTiempo";

		times_str[1] = "DFS\t\t\t " + to_string(times["DFS"]);
		times_str[2] = "BFS\t\t\t " + to_string(times["BFS"]);
		times_str[3] = "Dijkstra\t\t" + to_string(times["Dijkstra"]);

		for (int i = 0, j = 0; i < times_str.size(); i++, j = j + 1.5) {
			times_txt[i].setFont(font_2);
			times_txt[i].setString(times_str[i]);
			times_txt[i].setCharacterSize(15);
			times_txt[i].setPosition(width / 2 - 5 * NODE_SIZE, 150 + (5 + j) * NODE_SIZE);
		}

	}

	void append(string algoritmo, double time) override {
		times[algoritmo] = time;
	}

	void draw() override {

		Decorator::draw();

		setTexts();
		window.draw(b_table);
		window.draw(text_9);
		for_each(times_txt.begin(), times_txt.end(), [=](Text x) {window.draw(x); });


		window.display();


	};

	bool get_activate_help_window() override { return 3; }
	bool get_activate_comparison() override { return 3; }

};
