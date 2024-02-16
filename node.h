#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using namespace std;
using namespace sf;

struct Tuple {
	int x, y;

	Tuple() : x(0), y(0){}
	Tuple(int x, int y) : x(x), y(y) {}
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
	Tuple tuple;
	int x, y;

	vector<Tuple> v_n;
	bool visited = false, queued = false, start = false;
	Tuple prior;

public:
	Node(){}
	Node(Tuple t, char _t) {

		tuple = t;
		x = t.x;
		y = t.y;

		square = new Sprite();
		square->setPosition(t.x * NODE_SIZE
							, t.y * NODE_SIZE + HEADER_HEIGHT
							);
		image = new Texture();
		type = _t;	
		
	}

	void animation(Tuple t, char _t) {

		square = new Sprite();
		square->setPosition(t.x * NODE_SIZE
							, t.y * NODE_SIZE + HEADER_HEIGHT
						);
		image = new Texture();
		type = _t;

		filterImages();
	}

	void filterImages() {

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

	void set_n(Node** grid) {

		v_n.resize(0);

		if (type != 'W') {

			if (x > 0 && grid[x-1][y].type != 'W') v_n.push_back(Tuple(x - 1, y));
			if (x < (COLS - 1) && grid[x + 1][y].type != 'W') v_n.push_back(Tuple(x + 1, y));

			if (y > 0 && grid[x][y - 1].type != 'W') v_n.push_back(Tuple(x, y - 1));
			if (y < (ROWS - 1) && grid[x][y + 1].type != 'W') v_n.push_back(Tuple(x, y + 1));
		
		}

	}

	Tuple getTuple() {
		return tuple;
	}

	vector<Tuple> getNeighbours() {
		return v_n;
	}

	void setStart(bool c) {
		start = c;
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

	void setPrior(Tuple c) {
		prior = c;
	}

	bool getQueued() {
		return queued;
	}

	char getType() const {
		return type;
	}

	Tuple getPrior() {
		return prior;
	}

	void draw(RenderWindow &w) {
		filterImages();
		w.draw(*square);
	}

};

