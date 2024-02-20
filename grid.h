#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include <stack>
#include <chrono>
#include <vector>
#include "node.h"
#include "menu.h"

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
	double time = 0;


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

		start = false;
		end = false;

		start_tuple.x = -1;
		start_tuple.y = -1;
		end_tuple.x = -1;
		end_tuple.y = -1;

		while (!queued_nodes.empty()) queued_nodes.pop();

	}

	void partial_clean(int i) { // no limpia fin

		for (int x = 0; x < cols; x++)
			for (int y = 0; y < rows; y++)
				if (grid[x][y].getType() != 'W')
					grid[x][y].partial_clean();

		if (i == 1) { // limpia todo menos el fin

			grid[start_tuple.x][start_tuple.y].setIsStart(false); // verificar la tupla que almacenan
			grid[end_tuple.x][end_tuple.y].setType('T');
			start_tuple.x = -1;
			start_tuple.y = -1;
			start = false;

		}

		else if (i == 2) { // limpia todo menos el inicio

			grid[end_tuple.x][end_tuple.y].setIsEnd(false);
			grid[start_tuple.x][start_tuple.y].setType('B');
			end_tuple.x = -1;
			end_tuple.y = -1;
			end = false;

		}

		else if (i == 3) { // limpia todo menos el inicio y fin

			grid[end_tuple.x][end_tuple.y].setType('T');
			grid[start_tuple.x][start_tuple.y].setType('B');

		}

		while (!queued_nodes.empty()) queued_nodes.pop();


	}


	// - - - - -  algorithms  - - - - -

	// dijkstra

	void dijkstra(Menu menu) {

		auto t_begin = high_resolution_clock::now();

		set_neighbors();

		bool searching = true;

		grid[start_tuple.x][start_tuple.y].setIsStart(true);
		grid[start_tuple.x][start_tuple.y].setQueued(true); // en cola

		queued_nodes.push(grid[start_tuple.x][start_tuple.y]);

		while (!queued_nodes.empty() && searching) {
			// cuando entra a este bucle es pq está siendo visitado

			Tuple temp = queued_nodes.front().getTuple();

			if (temp != start_tuple && temp != end_tuple) {
				grid[temp.x][temp.y].setType('X');
				draw();
				menu.draw();
				sleep(sf::milliseconds(0.01));
			}



			queued_nodes.front().setVisited(true); // para verificar si se puso en la cola
			queued_nodes.pop();

			// pintando el camino

			if (temp == end_tuple) {

				grid[end_tuple.x][end_tuple.y].setType('T');
				searching = false;
				Tuple p = grid[temp.x][temp.y].getPrior();

				while ((p.x != start_tuple.x) || (p.y != start_tuple.y)) {
					//grid[p.x][p.y].setType('P');


					if (p != start_tuple && p != end_tuple) {
						grid[p.x][p.y].setType('P');
						draw();
						menu.draw();
						sleep(sf::milliseconds(0.01));
					}

					p = grid[p.x][p.y].getPrior();

				}

			}
			else {

				for (auto t_n : grid[temp.x][temp.y].get_n()) { // iterando en los vecinos del nodo
					if (!grid[t_n.x][t_n.y].getQueued()) { // para verificar si ese vecino ya ha sido visitado
						grid[t_n.x][t_n.y].setQueued(true);

						grid[t_n.x][t_n.y].setPrior(temp);
						queued_nodes.push(grid[t_n.x][t_n.y]);

						if (t_n != start_tuple && t_n != end_tuple) {

							grid[t_n.x][t_n.y].setType('Q');
							draw();
							menu.draw();
							sleep(sf::milliseconds(0.01));
						}


					}
				}
			}
			// falta cuando no hay solución
		}

		auto t_end = high_resolution_clock::now();

		duration<double, milli> t = t_end - t_begin;

		time = t.count();



	}

	// dfs

	void dfs(Menu menu) {

		auto t_begin = high_resolution_clock::now();

		set_neighbors();

		stack<Tuple> stack_nodes;

		grid[start_tuple.x][start_tuple.y].setVisited(true);
		stack_nodes.push(start_tuple);

		while (!stack_nodes.empty()) {
			Tuple current_tuple = stack_nodes.top();
			stack_nodes.pop();

			if (current_tuple == end_tuple) {
				Tuple p = grid[current_tuple.x][current_tuple.y].getPrior();
				while (p != start_tuple) {

					if (p != start_tuple && p != end_tuple) {
						grid[p.x][p.y].setType('P');
						draw();
						menu.draw();
						sleep(sf::milliseconds(0.01));
					}

					p = grid[p.x][p.y].getPrior();
				}
				break;
			}

			if (current_tuple != start_tuple && current_tuple != end_tuple) {
				grid[current_tuple.x][current_tuple.y].setType('X');
				draw(); // Dibuja el estado actual
				menu.draw();
				sleep(sf::milliseconds(0.01));
			}

			for (auto neighbor : grid[current_tuple.x][current_tuple.y].get_n()) {
				if (!grid[neighbor.x][neighbor.y].getVisited()) {
					grid[neighbor.x][neighbor.y].setVisited(true);
					grid[neighbor.x][neighbor.y].setPrior(current_tuple);
					stack_nodes.push(neighbor);

					if (neighbor != start_tuple && neighbor != end_tuple) {
						grid[neighbor.x][neighbor.y].setType('Q'); // Establecer tipo como 'Q'
						draw();
						menu.draw();
						sleep(sf::milliseconds(0.01));
					}
				}
			}
		}

		auto t_end = high_resolution_clock::now();

		duration<double, milli> t = t_end - t_begin;

		time = t.count();


	}

	// bfs

	void bfs(Menu menu) {

		auto t_begin = high_resolution_clock::now();

		// Configurar vecinos
		set_neighbors();

		// Inicializar la cola para BFS
		queue<Tuple> queue_nodes;

		// Marcar el nodo inicial como visitado y agregarlo a la cola
		grid[start_tuple.x][start_tuple.y].setVisited(true);
		queue_nodes.push(start_tuple);

		// Bucle principal de BFS
		while (!queue_nodes.empty()) {
			Tuple current_tuple = queue_nodes.front();
			queue_nodes.pop();

			if (current_tuple == end_tuple) {
				Tuple p = grid[current_tuple.x][current_tuple.y].getPrior();
				while (p != start_tuple) {

					if (p != start_tuple && p != end_tuple) {
						grid[p.x][p.y].setType('P');
						draw();
						menu.draw();
						sleep(sf::milliseconds(0.01));
					}

					p = grid[p.x][p.y].getPrior();
				}
				break;
			}

			if (current_tuple != start_tuple && current_tuple != end_tuple) {
				grid[current_tuple.x][current_tuple.y].setType('X');
				draw(); // Dibuja el estado actual
				menu.draw();
				sleep(sf::milliseconds(0.01));
			}

			for (auto neighbor : grid[current_tuple.x][current_tuple.y].get_n()) {
				if (!grid[neighbor.x][neighbor.y].getVisited()) {
					grid[neighbor.x][neighbor.y].setVisited(true);
					grid[neighbor.x][neighbor.y].setPrior(current_tuple);
					queue_nodes.push(neighbor);

					if (neighbor != start_tuple && neighbor != end_tuple) {
						grid[neighbor.x][neighbor.y].setType('Q'); // Establecer tipo como 'Q'
						draw();
						menu.draw();
						sleep(sf::milliseconds(0.01));
					}
				}
			}
		}


		auto t_end = high_resolution_clock::now();

		duration<double, milli> t = t_end - t_begin;

		time = t.count();
	}

	// - - - - - - - - - - - - - - - -


	void set_neighbors() {

		// fijar los vecinos de cada nodo

		for (int x = 0; x < cols; x++)
			for (int y = 0; y < rows; y++)
				grid[x][y].set_n(grid);


	}
	void generateMazeDFS() {
		// Restablecer el laberinto
		resetMaze();

		// Crear una pila para DFS
		stack<Node*> pila;

		// Elegir una posición de inicio aleatoria
		int startX = rand() % cols;
		int startY = rand() % rows;
		grid[startX][startY].setType('W'); // Marcar el nodo inicial como pared
		pila.push(&grid[startX][startY]); // Empujar el nodo inicial a la pila

		// DFS para generar el laberinto
		while (!pila.empty()) {
			Node* actual = pila.top();
			pila.pop();

			// Obtener vecinos no visitados
			vector<Node*> vecinos = getUnvisitedNeighbors(actual);

			// Elegir aleatoriamente un vecino y conectarlo
			if (!vecinos.empty()) {
				int indiceAleatorio = rand() % vecinos.size();
				Node* vecino = vecinos[indiceAleatorio];
				vecino->setType('W'); // Marcar el vecino como pared
				int x = actual->getTuple().x + (vecino->getTuple().x - actual->getTuple().x) / 2;
				int y = actual->getTuple().y + (vecino->getTuple().y - actual->getTuple().y) / 2;
				grid[x][y].setType('W'); // Marcar el espacio entre el nodo actual y el vecino como pared
				pila.push(vecino); // Empujar el vecino a la pila
				pila.push(actual); // Empujar el nodo actual nuevamente para continuar la exploración
			}
		}

		// Verificar que no haya espacios vacíos aislados
		if (verificarEspaciosAislados()) {
			generateMazeDFS();
		}
	}




	void resetMaze() {
		// Reset los nodos como Empty y false
		for (int x = 0; x < cols; ++x) {
			for (int y = 0; y < rows; ++y) {
				grid[x][y].setType('E');
				grid[x][y].setVisited(false);
			}
		}
	}


	bool verificarEspaciosAislados() {
		// Realizar el algoritmo de inundación para verificar espacios vacíos aislados
		// Comenzar la inundación desde un espacio vacío aleatorio
		int startX, startY;
		do {
			startX = rand() % cols;
			startY = rand() % rows;
		} while (grid[startX][startY].getType() != 'E'); // Encontrar un espacio vacío aleatorio

		// Realizar la inundación desde el espacio vacío aleatorio
		inundarEspacios(startX, startY);

		// Verificar si todos los espacios vacíos han sido visitados
		for (int x = 0; x < cols; ++x) {
			for (int y = 0; y < rows; ++y) {
				if (grid[x][y].getType() == 'E' && !grid[x][y].getVisited()) {
					// Se encontró un espacio vacío aislado
					return true;
				}
			}
		}

		// No se encontraron espacios vacíos aislados
		return false;
	}

	void inundarEspacios(int x, int y) {
		// Verificar si la posición actual está dentro de los límites y es un espacio vacío no visitado
		if (x >= 0 && x < cols && y >= 0 && y < rows && grid[x][y].getType() == 'E' && !grid[x][y].getVisited()) {
			// Marcar el espacio actual como visitado
			grid[x][y].setVisited(true);

			// Realizar la inundación de manera recursiva en las cuatro direcciones
			inundarEspacios(x + 1, y); // Derecha
			inundarEspacios(x - 1, y); // Izquierda
			inundarEspacios(x, y + 1); // Abajo
			inundarEspacios(x, y - 1); // Arriba
		}
	}



	// Función para obtener los vecinos no visitados de un nodo
	vector<Node*> getUnvisitedNeighbors(Node* current) {
		vector<Node*> neighbors;
		Tuple pos = current->getTuple();
		// pos actual 
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
