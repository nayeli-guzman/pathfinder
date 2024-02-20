#pragma once
#define ROWS 17
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
	bool selected_alg = false;
public:

	Screen(){}

	void execute() {

		window.create({ NODE_SIZE * COLS
						, NODE_SIZE * ROWS + HEADER_HEIGHT }
					, "Project");

		window.setFramerateLimit(60);

		Grid grid(ROWS, COLS, window);

		Menu menu(HEADER_HEIGHT
					, NODE_SIZE * COLS
					, NODE_SIZE * ROWS
					, window
			);


		while (window.isOpen()) {

			window.clear(Color::White);
			for (auto event = Event{}; window.pollEvent(event); ) {
				if (event.type == Event::Closed)
					window.close();

				if (help_window) {
					if (event.type == Event::KeyPressed) {
						help_window = false;
						menu.activate_help_window(help_window);
					}
				}

				else if (comparison_table) {
					if (event.type == Event::KeyPressed) {
						comparison_table = false;
						menu.activate_comparison_table(comparison_table);
					}
				}

				else {

					if (event.type == Event::MouseButtonPressed) {

						Tuple current_pos = getTuple(); // coord del nodo donde el user dio click

						if (event.mouseButton.button == Mouse::Left) {

							// selected_alg : significa si es que el algoritmo ya ha sido mostrado en pantalla
							// este lo cree para saber si el alg se mostraba en 
							// pantalla, si era así limpiaba todo, menos inicio o fin
							if (selected_alg) grid.partial_clean(1); // no se limpia fin, en la primera 
														// iteración es false;
							selected_alg = false;  // revisar el orden de estos dos if 
							
							if (grid.getEnd() == current_pos) // cuando inicio y fin se superponen, cuidado
															//, en la primera iiteración getend = 0,0
								grid.clearEnd(); // sus valores se ponen a -1,-1
							
							grid.setBegin(current_pos);
							

						} else if (event.mouseButton.button == Mouse::Right) {
							
							if (selected_alg) grid.partial_clean(2);
							
							selected_alg = false;
							
							if (grid.getBegin() == current_pos)
								grid.clearBegin();

							grid.setEnd(current_pos);
						}

					}

					if (event.type == Event::KeyPressed) {

						if (event.key.code == Keyboard::M) {
							help_window = true;
							menu.activate_help_window(help_window);
						}

						if (event.key.code == Keyboard::C) {
							grid.total_clean(); // comienzo y fin se borran
							selected_alg = false; // está todo limpio y no se muestra nada en pantalla
						}

						if (event.key.code >= Keyboard::Num1 && event.key.code <= Keyboard::Num4) {
							
							if (grid.isBegin() == false || grid.isEnd() == false) {
								menu.changeDynamicText(1, 0); // 1: No begin or end
							}
							
							else { // si el inicio y fin están definidos

								if (event.key.code == Keyboard::Num1) {
									if (selected_alg) grid.partial_clean(3);
									selected_alg = false;
									menu.updateSelector(1);
									menu.changeDynamicText(2, 0); // 2. executing dijkstra
									grid.setAlgorithm(1);
								}

								else if (event.key.code == Keyboard::Num2) {
									if (selected_alg) grid.partial_clean(3);
									selected_alg = false;
									menu.updateSelector(2);
									menu.changeDynamicText(3, 0); // 3. executing dfs
									grid.setAlgorithm(2);
								}

								else if (event.key.code == Keyboard::Num3) {
									if (selected_alg) grid.partial_clean(3);
									selected_alg = false;
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

									selected_alg = true;

								}
							
							}

						}
						
						if (event.key.code == Keyboard::Space) {
							
							if (grid.getAlgorithm() != 0 && (grid.isBegin() == true && grid.isEnd() == true)) {
								
								if (!selected_alg) 
									switch (grid.getAlgorithm()) {
										case 1: // DIJKSTRA
											// cuando ya se ejecutó y se muestra en pantalla
											grid.dijkstra(menu);
											//grid.dijkstra_time();
											menu.changeDynamicText(5, grid.getTime());
											selected_alg = true;
											break;
										case 2: // DFS
											grid.dfs(menu);
											//grid.dfs_time();
											menu.changeDynamicText(6, grid.getTime());
											selected_alg = true;
											break;
										case 3: // BFS
											grid.bfs(menu);
											//grid.bfs_time();
											menu.changeDynamicText(7, grid.getTime());
											selected_alg = true;
											break;
										default:
											break;
									}
							
							} else menu.changeDynamicText(1,0);
							
						}

					}

				}

			}

			grid.draw();
			menu.draw();

		}	

	}


	Tuple getTuple() {

		Vector2i mouse = Mouse::getPosition(window);
		
		int x = mouse.x / NODE_SIZE;
		int y = mouse.y / NODE_SIZE - (HEADER_HEIGHT / NODE_SIZE);

		return Tuple(x, y);

	}

};

