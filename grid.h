#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include <stack>
#include <chrono>
#include "node.h"
#include <vector>

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

	int number_of_algorithm = 0;
	double time;


	// dijkstra
	queue<Node> queued_nodes;
	//vector<Node> path;
	bool q = false
		, v = false
		, is_dijkstra = false
		, target_node = false
		;

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



	// setters

	void setWall(Tuple t) {
		grid[t.x][t.y].animation(t, 'W');
	}

	void setBegin(Tuple t) {

		if (grid[t.x][t.y].getType() != 'W') {

			if (start) { // si ya hay un start, se borrará el anterior
				setEmpty(start_tuple);
				grid[start_tuple.x][start_tuple.y].setIsStart(false); // el anterior ya no será reconocido como start point
			}
			grid[t.x][t.y].animation(t, 'B');
			grid[t.x][t.y].setIsStart(true); // ese nodo está setteado como true
			start_tuple = t;
			start = true; // ahora sí hay un start
		}

	}

	void setEnd(Tuple t) {

		if (grid[t.x][t.y].getType() != 'W') {
			if (end) {
				setEmpty(end_tuple);
				grid[end_tuple.x][end_tuple.y].setIsEnd(false);
			}

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
		number_of_algorithm = i;
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
		return number_of_algorithm; // 1. Dijkstra   2. DFS   3.BFS
	}

	double getTime() {
		return time;
	}

	// limpieza

	void clearBegin() {
		grid[start_tuple.x][start_tuple.y].animation(start_tuple, 'E');
		grid[start_tuple.x][start_tuple.y].setIsStart(false);
		start_tuple.x = -1;
		start_tuple.y = -1;
		start = false;
	}

	void clearEnd() {
		grid[end_tuple.x][end_tuple.y].animation(end_tuple, 'E');
		grid[end_tuple.x][end_tuple.y].setIsEnd(false);
		end_tuple.x = -1;
		end_tuple.y = -1;
		end = false;
	}

	void total_clean() {

		for (int x = 0; x < cols; x++)
			for (int y = 0; y < rows; y++)
				if (grid[x][y].getType() != 'W')
					grid[x][y].total_clean();

		while (!queued_nodes.empty()) queued_nodes.pop();
	}

	void partial_clean(int i) { // no limpia fin

		for (int x = 0; x < cols; x++)
			for (int y = 0; y < rows; y++)
				if (grid[x][y].getType() != 'W')
					grid[x][y].partial_clean();

		if (i == 1) {

			grid[start_tuple.x][start_tuple.y].setIsStart(false);
			grid[end_tuple.x][end_tuple.y].setType('T');

		}

		else if (i == 2) {

			grid[end_tuple.x][end_tuple.y].setIsEnd(false);
			grid[start_tuple.x][start_tuple.y].setType('B');

		}

		while (!queued_nodes.empty()) queued_nodes.pop();


	}


	// - - - - -  algorithms  - - - - -

	// dijkstra

	void dijkstra() {

		// tiempos

		auto t_begin = high_resolution_clock::now();

		set_neighbors();

		bool searching = true;

		//for (auto e: grid[start_tuple.x][start_tuple.y].get_n()) 
			//cout << e.x << ", " << e.y << endl;

		grid[start_tuple.x][start_tuple.y].setIsStart(true);
		grid[start_tuple.x][start_tuple.y].setQueued(true); // en cola

		queued_nodes.push(grid[start_tuple.x][start_tuple.y]);

		while (!queued_nodes.empty() && searching) {

			Tuple temp = queued_nodes.front().getTuple();
			queued_nodes.front().setQueued(true); // para verificar si se puso en la cola
			queued_nodes.pop();

			if (temp == end_tuple) {

				grid[end_tuple.x][end_tuple.y].setType('T');
				searching = false;
				Tuple p = grid[temp.x][temp.y].getPrior();

				while ((p.x != start_tuple.x) || (p.y != start_tuple.y)) {
					//cout << "PEN" << endl;
					//path.push_back(grid[p.x][p.y]);
					grid[p.x][p.y].setType('P');
					p = grid[p.x][p.y].getPrior();

				}

			}
			else {

				for (auto t_n : grid[temp.x][temp.y].get_n()) { // iterando en los vecinos del nodo
					if (!grid[t_n.x][t_n.y].getVisited()) { // para verificar si ese vecino ya ha sido visitado
						grid[t_n.x][t_n.y].setVisited(true);
						grid[t_n.x][t_n.y].setType('X');
						grid[t_n.x][t_n.y].setPrior(temp);
						queued_nodes.push(grid[t_n.x][t_n.y]);
					}
				}
			}
			// falta cuando no hay solución
		}

		grid[start_tuple.x][start_tuple.y].setType('B');


		auto t_end = high_resolution_clock::now();

		duration<double, milli> t = t_end - t_begin;

		time = t.count();

	}

	void dfs() {
		// Iniciar temporizador
		auto t_begin = high_resolution_clock::now();

		// Configurar vecinos
		set_neighbors();

		// Inicializar la pila para DFS
		stack<Tuple> stack_nodes;

		// Vector para registrar el camino
		vector<Tuple> path;

		// Variable para detener la búsqueda si se encuentra el nodo final
		bool found = false;

		// Marcar el nodo final como visitado y apilarlo
		grid[end_tuple.x][end_tuple.y].setVisited(true);
		grid[start_tuple.x][start_tuple.y].setType('B');
		stack_nodes.push(end_tuple);
		path.push_back(end_tuple); // Agregar el nodo final al camino

		// Bucle principal de DFS
		while (!stack_nodes.empty()) {
			// Extraer la tupla superior de la pila
			Tuple current_tuple = stack_nodes.top();
			stack_nodes.pop();

			// Si es el nodo inicial, marcar como encontrado y detener la búsqueda
			if (current_tuple == start_tuple) {
				grid[start_tuple.x][start_tuple.y].setType('B'); // Marcar el nodo inicial como 'B'
				found = true;
				break;
			}

			// Marcar el nodo actual como explorado ('X')
			if (grid[current_tuple.x][current_tuple.y].getType() != 'B') {
				grid[current_tuple.x][current_tuple.y].setType('X');
			}

			// Explorar los vecinos de la tupla actual
			for (auto neighbor : grid[current_tuple.x][current_tuple.y].get_n()) {
				if (!grid[neighbor.x][neighbor.y].getVisited()) {
					// Marcar el vecino como visitado y apilar su tupla
					grid[neighbor.x][neighbor.y].setVisited(true);
					stack_nodes.push(neighbor);
					path.push_back(neighbor); // Agregar el vecino al camino
				}
			}
		}

		// Si se encuentra el nodo inicial, marcar el camino recorrido
		if (found) {
			//FALTA
		}
		else {
			//Igual que dijkstra falta que hacer si no se encuentra el camino
		}

		// Detener temporizador y calcular el tiempo transcurrido
		auto t_end = high_resolution_clock::now();
		duration<double, milli> t = t_end - t_begin;
		time = t.count();
	}


	void set_neighbors() {

		// fijar los vecinos de cada nodo

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
