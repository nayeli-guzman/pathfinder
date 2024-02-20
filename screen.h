#pragma once
#define ROWS 16
#define COLS 40
#define NODE_SIZE 40
#define HEADER_HEIGHT 200


#include <SFML/Graphics.hpp>
#include "grid.h"
#include "menu.h"

// DONT forget to set destroyers

using namespace std;
using namespace sf;

class Screen {
private:
	RenderWindow window;
	bool help_window = false;
	bool comparison_table = false;
	bool selected_alg_1 = false
		, selected_alg_2 = false;

	int number_of_map = 2; // normal;  1. pequeño (sprites 32x32) 20x50 UP
								    // 2. normal (sprites 40x40)  16x40 
									// 3. grande (sprites -x-) -x- DOWN

	static Screen* instancia;
	Screen() {}

public:

	

	static Screen* getInstance() {
		if (instancia == nullptr)
			instancia = new Screen;
		return instancia;
	}


	void execute() {

		window.create({ NODE_SIZE * COLS
						, NODE_SIZE * ROWS + HEADER_HEIGHT }
					, "Project");

		window.setFramerateLimit(60);

		Grid grid(ROWS, COLS, window, 2);

		Grid small_grid(20, 50, window, 1);

		Menu menu(HEADER_HEIGHT
					, NODE_SIZE * COLS
					, NODE_SIZE * ROWS
					, window
			);


		while (window.isOpen()) {

			window.clear(Color::White);

			for (auto event = Event{}; window.pollEvent(event); ) {

				// cuando se cierra;
				if (event.type == Event::Closed)
					window.close();

				// ventana de ayuda / menu
				if (help_window) {
					if (event.type == Event::KeyPressed) {
						help_window = false;
						menu.activate_help_window(help_window);
					}
				}

				// tabla de comparaciones al presionar 4
				else if (comparison_table) {
					if (event.type == Event::KeyPressed) {
						comparison_table = false;
						menu.activate_comparison_table(comparison_table);
					}
				}

				else {

					if (event.type == Event::MouseButtonPressed) {

						Tuple current_pos = getTuple(); // coord del nodo donde el user dio click

						// cuando da click
						if (event.mouseButton.button == Mouse::Left) {

							if (number_of_map == 1) {

								if (selected_alg_1) 
									small_grid.partial_clean(1);

								selected_alg_1 = false; 

								if (small_grid.getEnd() == current_pos) 
									small_grid.clearEnd();

								small_grid.setBegin(current_pos);

							}

							else if (number_of_map == 2) {

								if (selected_alg_2) // si el recorrido del alg se muestra en pantalla
									grid.partial_clean(1);

								selected_alg_2 = false;  // revisar el orden de estos dos if 

								if (grid.getEnd() == current_pos) // cuando inicio y fin se superponen, cuidado primera it getend == 0, 0
									grid.clearEnd();

								grid.setBegin(current_pos);

							}

						}

						else if (event.mouseButton.button == Mouse::Right) {

							if (number_of_map == 1) {

								if (selected_alg_1)
									small_grid.partial_clean(2);

								selected_alg_1 = false;

								if (small_grid.getBegin() == current_pos)
									small_grid.clearBegin();

								small_grid.setEnd(current_pos);

							}

							else if (number_of_map == 2) {

								if (selected_alg_2)
									grid.partial_clean(2);

								selected_alg_2 = false;

								if (grid.getBegin() == current_pos)
									grid.clearBegin();

								grid.setEnd(current_pos);

							}

						}

					}


					if (event.type == Event::KeyPressed) {

						if (event.key.code == Keyboard::M) {
							help_window = true;
							menu.activate_help_window(help_window);
						}

						else if (event.key.code == Keyboard::C) {

							if (number_of_map == 1) {

								small_grid.total_clean(); 
								selected_alg_1 = false;

							}
							else if (number_of_map == 2) {

								grid.total_clean(); // comienzo y fin se borran
								selected_alg_2 = false; // está todo limpio y no se muestra nada en pantalla
							
							}
						}

						else if (event.key.code >= Keyboard::Num1 && event.key.code <= Keyboard::Num4) {

							if (number_of_map == 1) {
							
								if (small_grid.isBegin() == false || small_grid.isEnd() == false) {
									menu.changeDynamicText(1, 0); // 1: No begin or end
								}

								else { 

									if (event.key.code == Keyboard::Num1) {
										if (selected_alg_1) small_grid.partial_clean(3);
										selected_alg_1 = false;
										menu.updateSelector(1);
										menu.changeDynamicText(2, 0); // 2. executing dijkstra
										small_grid.setAlgorithm(1);
									}

									else if (event.key.code == Keyboard::Num2) {
										if (selected_alg_1) small_grid.partial_clean(3);
										selected_alg_1 = false;
										menu.updateSelector(2);
										menu.changeDynamicText(3, 0); // 3. executing dfs
										small_grid.setAlgorithm(2);
									}

									else if (event.key.code == Keyboard::Num3) {
										if (selected_alg_1) small_grid.partial_clean(3);
										selected_alg_1 = false;
										menu.updateSelector(3);
										menu.changeDynamicText(4, 0); // 4. executing bfs
										small_grid.setAlgorithm(3);
									}

									else if (event.key.code == Keyboard::Num4) {

										// dijkstra
										menu.updateSelector(1);
										menu.changeDynamicText(2, 0);
										small_grid.setAlgorithm(1);
										small_grid.dijkstra(menu);
										menu.changeDynamicText(5, small_grid.getTime());

										menu.append("Dijkstra", small_grid.getTime());
										small_grid.partial_clean(3);

										// dfs

										menu.updateSelector(2);
										menu.changeDynamicText(3, 0); // 3. executing dfs
										small_grid.setAlgorithm(2);
										small_grid.dfs(menu);
										menu.changeDynamicText(6, small_grid.getTime());

										menu.append("DFS", small_grid.getTime());
										small_grid.partial_clean(3);

										// bfs

										menu.updateSelector(3);
										menu.changeDynamicText(4, 0); // 4. executing bfs
										small_grid.setAlgorithm(3);
										small_grid.bfs(menu);
										menu.changeDynamicText(7, small_grid.getTime());

										menu.append("BFS", small_grid.getTime());

										// pop-up

										comparison_table = true;
										menu.activate_comparison_table(comparison_table);

										selected_alg_1 = true;

									}

								}
							}

							else if (number_of_map == 2) {

								if (grid.isBegin() == false || grid.isEnd() == false) {
									menu.changeDynamicText(1, 0); // 1: No begin or end
								}

								else { // si el inicio y fin están definidos

									if (event.key.code == Keyboard::Num1) {
										if (selected_alg_2) grid.partial_clean(3);
										selected_alg_2 = false;
										menu.updateSelector(1);
										menu.changeDynamicText(2, 0); // 2. executing dijkstra
										grid.setAlgorithm(1);
									}

									else if (event.key.code == Keyboard::Num2) {
										if (selected_alg_2) grid.partial_clean(3);
										selected_alg_2 = false;
										menu.updateSelector(2);
										menu.changeDynamicText(3, 0); // 3. executing dfs
										grid.setAlgorithm(2);
									}

									else if (event.key.code == Keyboard::Num3) {
										if (selected_alg_2) grid.partial_clean(3);
										selected_alg_2 = false;
										menu.updateSelector(3);
										menu.changeDynamicText(4, 0); // 4. executing bfs
										grid.setAlgorithm(3);
									}

									else if (event.key.code == Keyboard::Num4) {

										// dijkstra
										menu.updateSelector(1);
										menu.changeDynamicText(2, 0);
										grid.setAlgorithm(1);
										grid.dijkstra(menu);
										menu.changeDynamicText(5, grid.getTime());

										menu.append("Dijkstra", grid.getTime());
										grid.partial_clean(3);

										// dfs

										menu.updateSelector(2);
										menu.changeDynamicText(3, 0); // 3. executing dfs
										grid.setAlgorithm(2);
										grid.dfs(menu);
										menu.changeDynamicText(6, grid.getTime());

										menu.append("DFS", grid.getTime());
										grid.partial_clean(3);

										// bfs

										menu.updateSelector(3);
										menu.changeDynamicText(4, 0); // 4. executing bfs
										grid.setAlgorithm(3);
										grid.bfs(menu);
										menu.changeDynamicText(7, grid.getTime());

										menu.append("BFS", grid.getTime());

										// pop-up

										comparison_table = true;
										menu.activate_comparison_table(comparison_table);

										selected_alg_2 = true;

									}

								}

							}

						}

						else if (event.key.code == Keyboard::Space) {

							if (number_of_map == 1){
							
								if (small_grid.getAlgorithm() != 0 && (small_grid.isBegin() == true && small_grid.isEnd() == true)) {

									if (!selected_alg_1)
										switch (small_grid.getAlgorithm()) {
										case 1: // DIJKSTRA
											// cuando ya se ejecutó y se muestra en pantalla
											small_grid.dijkstra(menu);
											//grid.dijkstra_time();
											menu.changeDynamicText(5, small_grid.getTime());
											selected_alg_1 = true;
											break;
										case 2: // DFS
											small_grid.dfs(menu);
											//grid.dfs_time();
											menu.changeDynamicText(6, small_grid.getTime());
											selected_alg_1 = true;
											break;
										case 3: // BFS
											small_grid.bfs(menu);
											//grid.bfs_time();
											menu.changeDynamicText(7, small_grid.getTime());
											selected_alg_1 = true;
											break;
										default:
											break;
										}

								}

								else menu.changeDynamicText(1, 0);

							}
							else if (number_of_map == 2) {

								if (grid.getAlgorithm() != 0 && (grid.isBegin() == true && grid.isEnd() == true)) {

									if (!selected_alg_2)
										switch (grid.getAlgorithm()) {
										case 1: // DIJKSTRA
											// cuando ya se ejecutó y se muestra en pantalla
											grid.dijkstra(menu);
											//grid.dijkstra_time();
											menu.changeDynamicText(5, grid.getTime());
											selected_alg_2 = true;
											break;
										case 2: // DFS
											grid.dfs(menu);
											//grid.dfs_time();
											menu.changeDynamicText(6, grid.getTime());
											selected_alg_2 = true;
											break;
										case 3: // BFS
											grid.bfs(menu);
											//grid.bfs_time();
											menu.changeDynamicText(7, grid.getTime());
											selected_alg_2 = true;
											break;
										default:
											break;
										}

								}
							
								else menu.changeDynamicText(1, 0);
							
							}
						}

						else if (event.key.code == Keyboard::Up || event.key.code == Keyboard::Down) {

							if (event.key.code == Keyboard::Up)
								number_of_map = 1;
							else
								number_of_map = 2;

						}

					}

				}

			}

				

			
			if (number_of_map == 1) small_grid.draw();
			else if (number_of_map == 2) grid.draw();
			
			menu.draw();

		}	

	}


	Tuple getTuple() {

		int node_size = 40;

		if (number_of_map == 1) node_size = 32;
		else if (number_of_map == 2) node_size = 40;

		Vector2i mouse = Mouse::getPosition(window);
		
		int x = mouse.x / node_size;
		int y = (mouse.y - HEADER_HEIGHT) / node_size;

		return Tuple(x, y);

	}

};

