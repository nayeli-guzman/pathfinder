#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <queue>
#include <stack>
#include <chrono>
#include <algorithm>
#include <vector>
#include "node.h"
#include "menu.h"

using namespace std;
using namespace chrono;
using namespace sf;

class Grid {

private:
	RenderWindow& window;
	Node** grid;
	Tuple<> start_tuple, end_tuple ;
	bool start = false, end = false;
	int rows, cols
		, number_grid = 2
		, number_of_algorithm = 0;
	double time = 0;


	// dijkstra
	
	bool q = false
		, v = false
		, is_dijkstra = false
		, target_node = false
		, is_clean = true; // no cuenta inicio ni fin
	;



public:


	Grid(int r, int c, RenderWindow& w, int n) : rows(r), cols(c), window(w), number_grid(n) { // 17, 40

		grid = new Node * [cols];

		for (int x = 0; x < cols; x++) {
			grid[x] = new Node[rows];
			for (int y = 0; y < rows; y++) {
				grid[x][y] = Node(Tuple<>(x, y), 'W', number_grid); // E empty
				// WALL para prims algorithm, CAMBIAR A EMPTY SI NO SE VA A 
				// EJECUTAR PRIMS_ALGORITHM
				grid[x][y].setCols(cols);
				grid[x][y].setRows(rows);
			}

		}

		prims_algorithm();

		//generateMazeDFS();

		//maze();
		
		
	}

	Grid(const Grid& otro) : rows(otro.rows), cols(otro.cols), window(otro.window)
							, start_tuple(otro.start_tuple), end_tuple(otro.end_tuple){

		grid = new Node * [cols];

		for (int x = 0; x < cols; x++) {
			grid[x] = new Node[rows];
			for (int y = 0; y < rows; y++)
				grid[x][y] = otro.grid[x][y];
		
		}

	}

	// REVISAR

	void maze() {
		recursive_division(0, 0, rows, cols);
	}
	void recursive_division(int x, int y, int height, int width) { //16, 40

		if (width < 4 || height < 4)
			return;

		int new_wall_x = 0
			, new_wall_y = 0
			, new_hole_x = 0
			, new_hole_y = 0
			, new_height = 0
			, new_width = 0
			, y_pair = 0
			, x_pair = 0
			, new_height_pair = 0
			, new_width_pair = 0
			;

		int orientation, horizontal = 1, vertical = 0;

		if (width < height)
			orientation = horizontal;
		else
			orientation = vertical;

		cout << endl << orientation << endl;

		new_wall_x = x + (orientation ? 0 : rand() % (width - 2));
		new_wall_y = y + (orientation ? rand() % (height - 2) : 0);

		new_hole_x = new_wall_x + (orientation ? rand() % (width) : 0);
		new_hole_y = new_wall_y + (orientation ? 0 : rand() % (height));

		//cout << "nw" << new_wall_x << ", " << new_wall_y << endl;
		//cout << "nh" << new_hole_x << ", " << new_hole_y << endl;

		int lenght = (orientation) ? width : height;

		int dx = (orientation) ? 1 : 0;
		int dy = (orientation) ? 0 : 1;

		int nwx = new_wall_x;

		for (int i = 0; i < lenght; i++) {
			grid[new_wall_x][new_wall_y].setType('W');
			new_wall_x += dx;
			new_wall_y += dy;
		}

		grid[new_hole_x][new_hole_y].setType('E');
		

		int nx = x, ny = y;
		int w = orientation ? width : new_wall_x - x + 1
			, h = orientation ? (new_wall_y - y + 1) : height;

		cout << "1. " << h << " " << w << endl;

		//recursive_division(nx, ny, w, h);

		nx = horizontal ? x : (new_wall_x + 1);
		ny = horizontal ? (new_wall_y + 1) : y;

		w = horizontal ? width : (x + width - new_wall_x - 1);
		h = horizontal ? (y + height - new_wall_y - 1) : height;

		cout << "2. " << h << " " << w << endl;
		//recursive_division(nx, ny, w, h);

		
		/*
		SEGUNDA OPCIÓN ):
		if (horizontal == orientation) {

			if (height <= 5) 
				return;
			
			
			// Random place for the wall and for the hole. 

			int random_wall = rand() % (height - 5) + 2;
			int random_hole = rand() % (width - 3) + 1;

			// Make sure, that the wall is on an even coordinate and the hole is
			// on an odd coordinate. 
			new_wall = y + random_wall / 2 * 2;
			new_hole = x + random_hole / 2 * 2 + 1;

			cout << "rw1: " << random_wall << endl;
			cout << "rh1: " << random_hole << endl;

			cout << "nw1: " << new_wall << endl;
			cout << "nh1: " << new_hole << endl << endl;

			// Place the wall. 
			for (int i = x; i < (x + width - 1); i++) 
				grid[i][new_wall].setType('W');
			
			// Place the hole. 
			grid[new_hole][new_wall].setType('E');

			// Calculate the new values for the next run. 
			new_height = new_wall - y + 1;
			new_width = width;

			// Complementary pairs. 'The other side of the wall.' 
			y_pair = new_wall;
			x_pair = x;
			new_height_pair = y + height - new_wall;
			new_width_pair = width;

			cout << "yp: " << y_pair<< endl;
			cout << "xp: " << x_pair << endl;

			cout << "nhp: " << new_height_pair << endl;
			cout << "nwp: " << new_width_pair<< endl << endl;

		
		}
		else if (vertical == orientation) {

			if (width <= 5) {
				cout << "PENE";
				return;
			}

			int random_wall = rand() % (width - 5) + 2;
			int random_hole = rand() % (height - 3) + 1;


			// Make sure, that the wall is on an even coordinate and the hole is on
			an odd coordinate. 
			new_wall = y + random_wall / 2 * 2;
			new_hole = x + random_hole / 2 * 2 + 1;

			cout << "rw2: " << random_wall << endl;
			cout << "rh2: " << random_hole << endl;

			cout << "nw2: " << new_wall << endl;
			cout << "nh2: " << new_hole << endl << endl;

			// Place the wall. 
			for (int i = y; i < (y + height - 1); i++) 
				grid[new_wall][i].setType('W');
			
			// Place the hole. 
			grid[new_wall][new_hole].setType('E');

			// Calculate the new values for the next run. 
			new_height = height;
			new_width = new_wall - x + 1;

			// Complementary pairs. 'The other side of the wall.' 
			y_pair = y;
			x_pair = new_wall;
			new_height_pair = height;
			new_width_pair = x + width - new_wall;


			cout << "yp: " << y_pair << endl;
			cout << "xp: " << x_pair << endl;

			cout << "nhp: " << new_height_pair << endl;
			cout << "nwp: " << new_width_pair << endl << endl;
		}

		recursive_division(y, x, new_height, new_width);
		// When there are no more places left, then go to the 'other side'. 
		recursive_division(y_pair, x_pair, new_height_pair, new_width_pair);
		*/

	}

	//

	void prims_algorithm() {


		vector<Node> vecinos_bloqueados;
		vector<Node> vecinos_desbloqueados;

		int x_r = rand() % cols;
		int y_r = rand() % rows;

		grid[x_r][y_r].setType('E');
		grid[x_r][y_r].setLocked(false); // ya no está bloqueada
		

		get_vecinos_bloqueados(x_r, y_r, vecinos_bloqueados); // VECINOS BLOQ DE INICIO RANDOM

		while (!vecinos_bloqueados.empty()) {

			
			int random1 = rand() % vecinos_bloqueados.size();
			int x1 = vecinos_bloqueados[random1].getTuple().x;
			int y1 = vecinos_bloqueados[random1].getTuple().y; // ID VECINO BLOQ RANDOM
			/*
			Node* vecino_bloq = &vecinos_bloqueados.front();
			int x1 = vecino_bloq->getTuple().x;
			int y1 = vecino_bloq->getTuple().y;
			*/
			get_vecinos_desbloqueados(x1, y1, vecinos_desbloqueados);


			
			int random2 = rand() % vecinos_desbloqueados.size();
			int x2 = vecinos_desbloqueados[random2].getTuple().x;
			int y2 = vecinos_desbloqueados[random2].getTuple().y; // ID VECINO DESBLOQ RANDOM
			
			/*
			Node* vecino_desbloq = &vecinos_desbloqueados.top();
			int x2 = vecino_desbloq->getTuple().x;
			int y2 = vecino_desbloq->getTuple().y;
			*/
			//cout << endl << "real:" << x2 << ", " << y2 << endl;
			//for (auto e : vecinos_desbloqueados) cout << e.getTuple().x << ", " << e.getTuple().y << endl;
			
			//cout << x2 << ", " << y2 << endl;

			Node* temp = getMedio(grid[x1][y1].getTuple(), grid[x2][y2].getTuple()); 
			temp->setType('E');

			get_vecinos_bloqueados(x1, y1, vecinos_bloqueados);

			grid[x1][y1].setLocked(false);
			grid[x1][y1].setType('E');

			vecinos_bloqueados.erase(vecinos_bloqueados.begin()+ random1);

			vecinos_desbloqueados.clear();
			


		}

	}

	Node* getMedio(Tuple<> t1, Tuple<> t2) { // original, adyacente

		//cout << t1.x << ", " << t1.y << endl;
		//cout << t2.x << ", " << t2.y << endl;
		

		if (t1.x == t2.x) {
			if (t1.y == t2.y + 2)
				return &grid[t1.x][t1.y - 1];
			else if (t1.y == t2.y - 2)
				return &grid[t1.x][t1.y + 1];
		} 
		
		else if (t1.y == t2.y) {
			
			if (t1.x == t2.x + 2)
				return &grid[t1.x - 1][t1.y];
			else if (t1.x == t2.x - 2) {
				return &grid[t1.x + 1][t1.y];
			}
		}
		
	}

	void get_vecinos_bloqueados(int x, int y, vector<Node> &bloq) {

		if (x > 1 && grid[x - 2][y].getLocked() == true) bloq.push_back(grid[x - 2][y]);
		if (y > 1 && grid[x][y - 2].getLocked() == true) bloq.push_back(grid[x][y - 2]);
		if (x < cols-2 && grid[x + 2][y].getLocked() == true) bloq.push_back(grid[x + 2][y]);
		if (y < rows-2 && grid[x][y + 2].getLocked() == true) bloq.push_back(grid[x][y + 2]);

	}

	void get_vecinos_desbloqueados(int x, int y, vector<Node>& desbloq) {

		if (x > 1 && grid[x - 2][y].getLocked() == false) desbloq.push_back(grid[x - 2][y]);
		if (y > 1 && grid[x][y - 2].getLocked() == false) desbloq.push_back(grid[x][y - 2]);
		if (x < cols - 2 && grid[x + 2][y].getLocked() == false) desbloq.push_back(grid[x + 2][y]);
		if (y < rows - 2 && grid[x][y + 2].getLocked() == false) desbloq.push_back(grid[x][y + 2]);

	}

	void draw() {
		for (int x = 0; x < cols; x++)
			for (int y = 0; y < rows; y++)
				grid[x][y].draw(window);
	}

	// setters

	void setWall(Tuple<> t) {
		grid[t.x][t.y].animation(t, 'W');
	}

	void setBegin(Tuple<> t) {

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

	void setNumberGrid(int t) {
		number_grid = t;
	}

	void setEnd(Tuple<> t) {

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

	void setEmpty(Tuple<> t) {
		grid[t.x][t.y].animation(t, 'E');

	}

	void setAlgorithm(int i) {
		number_of_algorithm = i;
	}

	// getters

	Tuple<> getBegin() {
		return start_tuple;
	}

	Tuple<> getEnd() {
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

	Node** get_Grid() {
		return grid;
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

		

	}

	void partial_clean(int i) { 

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
			is_clean = true;

		}


	}

	// - - - - -  algorithms  - - - - -

	// dijkstra

	void dijkstra(Menu menu) {

		if (!is_clean) partial_clean(3);

		set_neighbors();
		queue<Node> queued_nodes;

		bool searching = true;

		grid[start_tuple.x][start_tuple.y].setIsStart(true);
		grid[start_tuple.x][start_tuple.y].setQueued(true);
		queued_nodes.push(grid[start_tuple.x][start_tuple.y]);

		while (!queued_nodes.empty() && searching) {

			Tuple<> temp = queued_nodes.front().getTuple();

			if (temp != start_tuple && temp != end_tuple) {
				grid[temp.x][temp.y].setType('X');
				draw();
				menu.draw();
				sleep(sf::milliseconds(0.1));
			}

			queued_nodes.front().setVisited(true); 
			queued_nodes.pop();

			if (temp == end_tuple) {

				grid[end_tuple.x][end_tuple.y].setType('T');
				searching = false;
				Tuple<> p = grid[temp.x][temp.y].getPrior();

				while ((p.x != start_tuple.x) || (p.y != start_tuple.y)) {
					
					if (p != start_tuple && p != end_tuple) {
						grid[p.x][p.y].setType('P');
						draw();
						menu.draw();
						sleep(sf::milliseconds(0.1));
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
							sleep(sf::milliseconds(0.1));
						}
						
						
					}
				}
			}
			// falta cuando no hay solución
		}

		is_clean = false;

	}

	// dfs

	void dfs(Menu menu) {

		set_neighbors();

		stack<Tuple<>> stack_nodes;

		grid[start_tuple.x][start_tuple.y].setVisited(true);
		stack_nodes.push(start_tuple);

		while (!stack_nodes.empty()) {
			Tuple<> current_tuple = stack_nodes.top();
			stack_nodes.pop();

			if (current_tuple == end_tuple) {
				Tuple<> p = grid[current_tuple.x][current_tuple.y].getPrior();
				while (p != start_tuple) {

					if (p != start_tuple && p != end_tuple) {
						grid[p.x][p.y].setType('P');
						draw();
						menu.draw();
						sleep(sf::milliseconds(0.1));
					}

					p = grid[p.x][p.y].getPrior();
				}
				break;
			}

			if (current_tuple != start_tuple && current_tuple != end_tuple) {
				grid[current_tuple.x][current_tuple.y].setType('X');
				draw(); // Dibuja el estado actual
				menu.draw();
				sleep(sf::milliseconds(0.1));
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
						sleep(sf::milliseconds(0.1));
					}
				}
			}
		}

		is_clean = false;

	}

	// bfs

	void bfs(Menu menu) {

		// Configurar vecinos
		set_neighbors();

		// Inicializar la cola para BFS
		queue<Tuple<>> queue_nodes;

		// Marcar el nodo inicial como visitado y agregarlo a la cola
		grid[start_tuple.x][start_tuple.y].setVisited(true);
		queue_nodes.push(start_tuple);

		// Bucle principal de BFS
		while (!queue_nodes.empty()) {
			Tuple<> current_tuple = queue_nodes.front();
			queue_nodes.pop();

			if (current_tuple == end_tuple) {
				Tuple<> p = grid[current_tuple.x][current_tuple.y].getPrior();
				while (p != start_tuple) {

					if (p != start_tuple && p != end_tuple) {
						grid[p.x][p.y].setType('P');
						draw();
						menu.draw();
						sleep(sf::milliseconds(0.1));
					}

					p = grid[p.x][p.y].getPrior();
				}
				break;
			}

			if (current_tuple != start_tuple && current_tuple != end_tuple) {
				grid[current_tuple.x][current_tuple.y].setType('X');
				draw(); // Dibuja el estado actual
				menu.draw();
				sleep(sf::milliseconds(3));
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
						sleep(sf::milliseconds(0.1));
					}
				}
			}
		}
		is_clean = false;

	}

	// - - - - - - - - - - - - - - - -

	// tiempos

	double dijkstra_time() {

		if (!is_clean) partial_clean(3);

		auto t_begin = high_resolution_clock::now();

		set_neighbors(); // mod dentro de cada nodo
		queue<Node> queued_nodes;

		bool searching = true;

		grid[start_tuple.x][start_tuple.y].setIsStart(true);
		grid[start_tuple.x][start_tuple.y].setQueued(true); // en cola

		queued_nodes.push(grid[start_tuple.x][start_tuple.y]);

		while (!queued_nodes.empty() && searching) {

			Tuple<> temp = queued_nodes.front().getTuple();

			queued_nodes.front().setVisited(true); 
			queued_nodes.pop();

			if (temp == end_tuple) {

				searching = false;

				Tuple<> p = grid[temp.x][temp.y].getPrior();

				while ((p.x != start_tuple.x) || (p.y != start_tuple.y))
					p = grid[p.x][p.y].getPrior();
				
			}
			else {

				for (auto t_n : grid[temp.x][temp.y].get_n())  
					if (!grid[t_n.x][t_n.y].getQueued()) { 
						grid[t_n.x][t_n.y].setQueued(true);
						grid[t_n.x][t_n.y].setPrior(temp);
						queued_nodes.push(grid[t_n.x][t_n.y]);
					}
		
			}
		}

		auto t_end = high_resolution_clock::now();

		duration<double, milli> t = t_end - t_begin;

		time = t.count();
		partial_clean(3);  //3 -> no borra inicio, ni fin


		return time;

	}


	double dfs_time() {

		if (!is_clean) partial_clean(3);

		auto t_begin = high_resolution_clock::now();

		set_neighbors();

		stack<Tuple<>> stack_nodes;

		grid[start_tuple.x][start_tuple.y].setVisited(true);
		stack_nodes.push(start_tuple);

		while (!stack_nodes.empty()) {
			Tuple<> current_tuple = stack_nodes.top();
			stack_nodes.pop();

			if (current_tuple == end_tuple) {
				Tuple<> p = grid[current_tuple.x][current_tuple.y].getPrior();
				while (p != start_tuple) {

					p = grid[p.x][p.y].getPrior();
				}
				break;
			}

			for (auto neighbor : grid[current_tuple.x][current_tuple.y].get_n()) {
				if (!grid[neighbor.x][neighbor.y].getVisited()) {
					grid[neighbor.x][neighbor.y].setVisited(true);
					grid[neighbor.x][neighbor.y].setPrior(current_tuple);
					stack_nodes.push(neighbor);
				}
			}
		}

		auto t_end = high_resolution_clock::now();

		duration<double, milli> t = t_end - t_begin;

		time = t.count();
		partial_clean(3);

		return time;


	}

	// bfs

	double bfs_time() {

		if (!is_clean) partial_clean(3);

		auto t_begin = high_resolution_clock::now();

		// Configurar vecinos
		set_neighbors();

		// Inicializar la cola para BFS
		queue<Tuple<>> queue_nodes;

		// Marcar el nodo inicial como visitado y agregarlo a la cola
		grid[start_tuple.x][start_tuple.y].setVisited(true);
		queue_nodes.push(start_tuple);

		// Bucle principal de BFS
		while (!queue_nodes.empty()) {
			Tuple<> current_tuple = queue_nodes.front();
			queue_nodes.pop();

			if (current_tuple == end_tuple) {
				Tuple<> p = grid[current_tuple.x][current_tuple.y].getPrior();
				while (p != start_tuple) {
					p = grid[p.x][p.y].getPrior();
				}
				break;
			}

			for (auto neighbor : grid[current_tuple.x][current_tuple.y].get_n()) {
				if (!grid[neighbor.x][neighbor.y].getVisited()) {
					grid[neighbor.x][neighbor.y].setVisited(true);
					grid[neighbor.x][neighbor.y].setPrior(current_tuple);
					queue_nodes.push(neighbor);

				}
			}
		}


		auto t_end = high_resolution_clock::now();

		duration<double, milli> t = t_end - t_begin;

		time = t.count();
		partial_clean(3);
		return time;
	}

	// stuff

	void set_neighbors() {

		for (int x = 0; x < cols; x++)
			for (int y = 0; y < rows; y++)
				grid[x][y].set_n(grid);


	}
	
	void generateMazeDFS() {
		// Restablecer el laberinto

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

	}

	void resetMazeFalseAndPaint() {

		// fijar los vecinos de cada nodo

		for (int x = 0; x < cols; x++)
			for (int y = 0; y < rows; y++)
				if (grid[x][y].getType() != 'W') {
					grid[x][y].setType('E');
					grid[x][y].setVisited(false);

				}
		grid[start_tuple.x][start_tuple.y].setType('B');
		grid[end_tuple.x][end_tuple.y].setType('T');
	}


	vector<Node*> getUnvisitedNeighbors(Node* current) {
		vector<Node*> neighbors;
		Tuple<> pos = current->getTuple();
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
