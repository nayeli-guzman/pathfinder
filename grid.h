#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include <stack>
#include <chrono>
#include "node.h"

using namespace std;
using namespace chrono;
using namespace sf;

class Grid {

private:

	int rows, cols;
	RenderWindow& window;
	Node** grid;
	Tuple start_tuple, end_tuple;
	bool start = false, end = false;

	int a = 0;
	double time;


	// dijkstra
	queue<Node> queued_nodes;
	vector<Node> path;
	bool q = false, v = false, is_dijkstra = false, target_node = false;

	// dfs

	// bfs

public:

	Grid(int r, int c, RenderWindow& w) : rows(r), cols(c), window(w) { // 17, 40

		grid = new Node * [cols];

		for (int x = 0; x < cols; x++) {
			grid[x] = new Node[rows];
			for (int y = 0; y < rows; y++)
				grid[x][y] = Node(Tuple(x, y), 'E'); // E empty

		}

		generateMazeDFS();

	}

	void draw() {
		for (int x = 0; x < cols; x++)
			for (int y = 0; y < rows; y++)
				grid[x][y].draw(window);
	}

	void clearBegin() {
		grid[start_tuple.x][start_tuple.y].animation(start_tuple, 'E');
		start_tuple.x = -1;
		start_tuple.y = -1;
		start = false;
	}

	void clearEnd() {
		grid[end_tuple.x][end_tuple.y].animation(end_tuple, 'E');
		end_tuple.x = -1;
		end_tuple.y = -1;
		end = false;
	}

	// setters

	void setWall(Tuple t) {
		grid[t.x][t.y].animation(t, 'W');
	}

	void setBegin(Tuple t) {
		if (grid[t.x][t.y].getType() != 'W') {
			if (start) setEmpty(start_tuple);
			grid[t.x][t.y].animation(t, 'B');
			grid[t.x][t.y].setIsStart(true);
			start_tuple = t;
			start = true;
		}
	}

	void setEnd(Tuple t) {
		if (grid[t.x][t.y].getType() != 'W') {
			if (end) setEmpty(end_tuple);
			grid[t.x][t.y].animation(t, 'T');
			grid[t.x][t.y].setIsEnd(true);
			end_tuple = t;
			end = true;
		}
	}

	void setEmpty(Tuple t) {
		grid[t.x][t.y].animation(t, 'E');
	}

	void setAlgorithm(int i) {
		a = i;
	}

	// getters

	Tuple getBegin() {
		return start_tuple;
	}

	Tuple getEnd() {
		return end_tuple;
	}

	bool isBegin() {
		return start;
	}

	bool isEnd() {
		return end;
	}

	int getAlgorithm() {
		return a; // 1. Dijkstra   2. DFS   3.BFS
	}

	double getTime() {
		return time;
	}

	// limpieza

	void total_clean() {

		for (int x = 0; x < cols; x++)
			for (int y = 0; y < rows; y++)
				if (grid[x][y].getType() != 'W')
					grid[x][y].total_clean();

		while (!queued_nodes.empty()) queued_nodes.pop();
	}

	void partial_clean() {

		for (int x = 0; x < cols; x++)
			for (int y = 0; y < rows; y++)
				if (grid[x][y].getType() != 'W')
					grid[x][y].partial_clean();

		while (!queued_nodes.empty()) queued_nodes.pop();
	}

	// - - - - -  algorithms  - - - - -

	// dijkstra

	void dijkstra() {

		// tiempos
//		chrono::time_point<chrono::system_clock> t_inicio, t_fin;

		auto t_begin = high_resolution_clock::now();


		set_neighbors();

		bool searching = true;

		grid[start_tuple.x][start_tuple.y].setStart(true);
		grid[start_tuple.x][start_tuple.y].setQueued(true);

		queued_nodes.push(grid[start_tuple.x][start_tuple.y]);

		while (!queued_nodes.empty() && searching) {

			Tuple temp = queued_nodes.front().getTuple();
			queued_nodes.front().setVisited(true);
			queued_nodes.pop();

			if (temp == end_tuple) {

				grid[end_tuple.x][end_tuple.y].setType('T');
				searching = false;
				Tuple p = grid[temp.x][temp.y].getPrior();

				while ((p.x != start_tuple.x) || (p.y != start_tuple.y)) {
					path.push_back(grid[p.x][p.y]);
					grid[p.x][p.y].setType('P');
					p = grid[p.x][p.y].getPrior();

				}

			}
			else {

				for (auto t_n : grid[temp.x][temp.y].getNeighbours()) {
					if (!grid[t_n.x][t_n.y].getQueued()) {
						grid[t_n.x][t_n.y].setQueued(true);
						grid[t_n.x][t_n.y].setType('X');
						grid[t_n.x][t_n.y].setPrior(temp);
						queued_nodes.push(grid[t_n.x][t_n.y]);
					}
				}
			}
			// falta cuando no hay solución
		}


		auto t_end = high_resolution_clock::now();

		duration<double, milli> t = t_end - t_begin;

		time = t.count();

	}


	void set_neighbors() {

		for (int x = 0; x < cols; x++)
			for (int y = 0; y < rows; y++)
				grid[x][y].set_n(grid);


	}


	void generateMazeDFS() {
		// Elegir una posición de inicio aleatoria
		int startX = rand() % cols;
		int startY = rand() % rows;

		// Crear una pila para realizar DFS
		stack<Node*> stack;
		grid[startX][startY].setType('W'); // Marcar el nodo inicial como Wall
		stack.push(&grid[startX][startY]); // Agregar el nodo inicial a la pila

		while (!stack.empty()) {
			Node* current = stack.top();
			stack.pop();

			// Obtener los vecinos no visitados del nodo actual
			vector<Node*> neighbors = getUnvisitedNeighbors(current);

			// Si hay vecinos no visitados
			if (!neighbors.empty()) {
				// Escoger un vecino aleatorio
				int randIndex = rand() % neighbors.size();
				Node* neighbor = neighbors[randIndex];

				// Marcar el vecino como Wall
				neighbor->setType('W');

				// Marcar el espacio entre el nodo actual y el vecino como Empty
				int x = current->getTuple().x + (neighbor->getTuple().x - current->getTuple().x) / 2;
				int y = current->getTuple().y + (neighbor->getTuple().y - current->getTuple().y) / 2;
				grid[x][y].setType('W');

				// Agregar el vecino a la pila
				stack.push(neighbor);
				stack.push(current); // Agregar el nodo actual nuevamente a la pila para continuar con su exploración
			}
		}
	}

	// Función para obtener los vecinos no visitados de un nodo
	vector<Node*> getUnvisitedNeighbors(Node* current) {
		vector<Node*> neighbors;
		Tuple pos = current->getTuple();
		int x = pos.x;
		int y = pos.y;

		// Verificar vecinos arriba, abajo, izquierda y derecha
		if (x > 1 && grid[x - 2][y].getType() == 'E') neighbors.push_back(&grid[x - 2][y]);
		if (x < cols - 2 && grid[x + 2][y].getType() == 'E') neighbors.push_back(&grid[x + 2][y]);
		if (y > 1 && grid[x][y - 2].getType() == 'E') neighbors.push_back(&grid[x][y - 2]);
		if (y < rows - 2 && grid[x][y + 2].getType() == 'E') neighbors.push_back(&grid[x][y + 2]);

		return neighbors;
	}


};
