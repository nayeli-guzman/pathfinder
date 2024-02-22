	#pragma once
	#include <SFML/Graphics.hpp>
	#include <iostream>
	#include <vector>
	using namespace std;
	using namespace sf;

	template <class T = int>
	struct Tuple {
		T x, y;

		Tuple() : x(0), y(0){}
		Tuple(T x, T y) : x(x), y(y) {}
		Tuple(const Tuple& t2) { x = t2.x; y = t2.y; }

		int get_x() { return x; }
		int get_y() { return y; }

		bool operator == (Tuple t2) {
			return (x == t2.x) && (y == t2.y);
		}

		bool operator != (Tuple t2) {
			return (x != t2.x) || (y != t2.y);
		}

	};

	class Node{
	private:
		Sprite* square;
		Texture* image;
		Node** grid;
		char type;
		Tuple<> tuple;
		int x, y, number_grid = 2;


		vector<Tuple<>> v_n; // para almacenar los vecinos del nodo

		bool visited = false // para verificar si el nodo ya ha sido visitado
			, queued = false // para verificar si el nodo está en la cola
			, is_start = false // para verificar si el nodo es el comienzo
			, is_end = false
		; 
		Tuple<> prior;
		bool dijkstra = false;

		int rows, cols;

		int node_size;

		bool locked = true; // al inicio todo es wall

	public:

		Node(){}

		Node(Tuple<> t, char _t, int number) {

			number_grid = number;

			tuple = t;
			x = t.x;
			y = t.y;

			square = new Sprite();

			switch (number_grid) {
				case 1:
					node_size = 32;
					break;
				case 2:
					node_size = 40;
					break;
			}
		
			square->setPosition(t.x * node_size
								, t.y * node_size + HEADER_HEIGHT
			);

			image = new Texture();
			type = _t;	
		
		}


		void animation(Tuple<> t, char _t) {

			type = _t;

			switch (number_grid) {
			case 1:
				filterImages_x32();
				break;
			case 2:
				filterImages_x40();
				break;
			}
		

		}

		void filterImages_x32() {

			switch (type) {
			case 'E': // empty
				image->loadFromFile("sprites/x32/empty_sprite.png");
				square->setTexture(*image);
				break;
			case 'W': // wall
				image->loadFromFile("sprites/x32/wall_sprite.png");
				square->setTexture(*image);
				break;
			case 'P': // path
				image->loadFromFile("sprites/x32/path_sprite.png");
				square->setTexture(*image);
				break;
			case 'X': // explored
				image->loadFromFile("sprites/x32/explored_sprite.png");
				square->setTexture(*image);
				break;
			case 'Q': // en cola
				image->loadFromFile("sprites/x32/queued_sprite.png");
				square->setTexture(*image);
				break;
			case 'B': // begin
				image->loadFromFile("sprites/x32/start_sprite.png");
				square->setTexture(*image);
				break;
			case 'T': // end
				image->loadFromFile("sprites/x32/end_sprite.png");
				square->setTexture(*image);
				break;
		
			default:
				break;
			}

		
		}

		void filterImages_x40() {

			switch (type) {
			case 'E': // empty
				image->loadFromFile("sprites/empty_sprite.png");
				square->setTexture(*image);
				break;
			case 'W': // wall
				image->loadFromFile("sprites/wall_sprite.png");
				square->setTexture(*image);
				break;
			case 'P': // path
				image->loadFromFile("sprites/path_sprite.png");
				square->setTexture(*image);
				break;
			case 'X': // explored
				image->loadFromFile("sprites/explored_sprite.png");
				square->setTexture(*image);
				break;
			case 'Q': // en cola
				image->loadFromFile("sprites/queued_sprite.png");
				square->setTexture(*image);
				break;
			case 'B': // begin
				image->loadFromFile("sprites/start_sprite.png");
				square->setTexture(*image);
				break;
			case 'T': // end
				image->loadFromFile("sprites/end_sprite.png");
				square->setTexture(*image);
				break;

			default:
				break;
			}


		}

		void draw(RenderWindow& w) {
			switch (number_grid) {
			case 1:
				filterImages_x32();
				break;
			case 2:
				filterImages_x40();
				break;
			}

		
			w.draw(*square);
		
		}

		// dijkstra

		void set_n(Node** grid) {

		
			v_n.resize(0);

			if (type != 'W') { // las paredes no pueden tener vecinos

				dijkstra = true; 

				if (x > 0 && grid[x - 1][y].type != 'W') v_n.push_back(Tuple<>(x - 1, y));
				if (x < (cols - 1) && grid[x + 1][y].type != 'W') v_n.push_back(Tuple<>(x + 1, y));

				if (y > 0 && grid[x][y - 1].type != 'W') v_n.push_back(Tuple<>(x, y - 1));
				if (y < (rows - 1) && grid[x][y + 1].type != 'W') v_n.push_back(Tuple<>(x, y + 1));

			}

		}

		void total_clean() {
			if (dijkstra) {
				is_start = false;
				is_end = false;
				dijkstra = false;
				queued = false;
				visited = false;
				Tuple<> prior;
				v_n.clear();
				type = 'E';
			}
		}

		void partial_clean() { // solo entra el inicio

			if (dijkstra) {

				dijkstra = false; // ya no estará ejecutando dijks
				queued = false; 
				visited = false;
				Tuple<> prior;
				//v_n.clear();
				type = 'E';
			
			}
		}

		// setters

		void setLocked(bool c) {
			locked = c;
		}

		void setQueued(bool c) {
			queued = c;
		}

		void setVisited(bool c) {
			visited = c;
		}

		void setType(char c) {
			type = c;
		}

		void setPrior(Tuple<> c) {
			prior = c;
		}

		void setIsStart(bool c) {
			is_start = c;
		}

		void setIsEnd(bool c) {
			is_end = c;
		}

		void setRows(int c) {
			rows = c;
		}
	
		void setCols(int c) {
			cols = c;
		}

		// getters

		bool getLocked() {
			return locked;
		}

		bool getQueued() {
			return queued;
		}

		bool getVisited() {
			return visited;
		}

		char getType() const {
			return type;
		}

		Tuple<> getPrior() {
			return prior;
		}

		Tuple<> getTuple() {
			return tuple;
		}

		vector<Tuple<>> get_n() {
			return v_n;
		}

	};

