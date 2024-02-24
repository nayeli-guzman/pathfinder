#pragma once

#define ROWS 16
#define COLS 40
#define NODE_SIZE 40
#define HEADER_HEIGHT 200

#include <SFML/Graphics.hpp>
#include <vector>
#include <thread>
#include "grid.h"
#include "menu.h"

using namespace std;
using namespace sf;

class Screen {
private:
	RenderWindow window;
	bool help_window = false, comparison_table = false
		, selected_alg_1 = false, selected_alg_2 = false;
	int number_of_map = 2;
	vector<double> Times = { 0,0,0 };
	static Screen* instancia;

	Screen() {
		window.create({ NODE_SIZE * COLS, NODE_SIZE * ROWS + HEADER_HEIGHT }, "Project");
		window.setFramerateLimit(60);
	}

public:

	static Screen* getInstance() {
		if (instancia == nullptr)
			instancia = new Screen;
		return instancia;
	}

	~Screen() {
		instancia = nullptr;
		delete instancia;
	}

	void execute() {

		Grid grid(ROWS, COLS, window, 2);
		Grid small_grid(20, 50, window, 1);

		MenuAbstract* menu = new Menu(HEADER_HEIGHT, NODE_SIZE * COLS, NODE_SIZE * ROWS, window);
		MenuAbstract* menu_help_window = new HelpWindowMenu(menu, window);
		MenuAbstract* menu_comparison = new ComparisonTableMenu(menu, window);

		while (window.isOpen()) {
			window.clear(Color::White);

			for (auto event = Event{}; window.pollEvent(event); ) {

				if (event.type == Event::Closed) window.close();

				if (help_window && event.type == Event::KeyPressed) {
					help_window = false;
					menu->activate_help_window(help_window);
				}

				else if (comparison_table && event.type == Event::KeyPressed) {
					comparison_table = false;
					menu->activate_comparison_table(comparison_table);
				}

				else {
					if (event.type == Event::MouseButtonPressed) {

						Tuple<> current_pos = getTuple();

						if (event.mouseButton.button == Mouse::Left) {
							if (number_of_map == 1)
								setBegin(&small_grid, current_pos, selected_alg_1);
							else if (number_of_map == 2)
								setBegin(&grid, current_pos, selected_alg_2);
						}

						else if (event.mouseButton.button == Mouse::Right) {
							if (number_of_map == 1)
								setEnd(&small_grid, current_pos, selected_alg_1);
							else if (number_of_map == 2)
								setEnd(&grid, current_pos, selected_alg_2);
						}

					}

					else if (event.type == Event::KeyPressed) {

						if (event.key.code == Keyboard::M) {
							help_window = true;
							menu->activate_help_window(help_window);
						}

						else if (event.key.code == Keyboard::C) {
							if (number_of_map == 1)
								totalClean(&small_grid, selected_alg_1);
							else if (number_of_map == 2)
								totalClean(&grid, selected_alg_2);
						}

						else if (event.key.code >= Keyboard::Num1 && event.key.code <= Keyboard::Num4) {

							if (number_of_map == 1) {

								if (small_grid.isBegin() == false || small_grid.isEnd() == false)
									menu->changeDynamicText(1, 0); // 1: No begin or end
								else {
									if (event.key.code == Keyboard::Num1)
										updateAlgorithm(&small_grid, menu, selected_alg_1, 1);

									else if (event.key.code == Keyboard::Num2)
										updateAlgorithm(&small_grid, menu, selected_alg_1, 2);

									else if (event.key.code == Keyboard::Num3)
										updateAlgorithm(&small_grid, menu, selected_alg_1, 3);

									else if (event.key.code == Keyboard::Num4) {

										if (selected_alg_1) small_grid.partial_clean(3);
										selected_alg_1 = false;

										Grid temp = small_grid; // se copia cuando solo hay inicio y fin

										thread dijkstra_thread([&]() {Times[0] = temp.dijkstra_time(); });
										execute_4(&small_grid, menu, menu_comparison, 1, "Dijkstra");
										dijkstra_thread.join();
										small_grid.partial_clean(3);

										thread dfs_thread([&]() {Times[1] = temp.dfs_time(); });
										execute_4(&small_grid, menu, menu_comparison, 2, "DFS");
										dfs_thread.join();
										small_grid.partial_clean(3);

										thread bfs_thread([&]() {Times[2] = temp.bfs_time(); });
										execute_4(&small_grid, menu, menu_comparison, 3, "BFS");
										bfs_thread.join();

										comparison_table = true;
										menu->activate_comparison_table(comparison_table);
										selected_alg_1 = true;

									}

								}
							}

							else if (number_of_map == 2) {

								if (grid.isBegin() == false || grid.isEnd() == false)
									menu->changeDynamicText(1, 0); // 1: No begin or end

								else { // si el inicio y fin están definidos

									if (event.key.code == Keyboard::Num1)
										updateAlgorithm(&grid, menu, selected_alg_2, 1);

									else if (event.key.code == Keyboard::Num2)
										updateAlgorithm(&grid, menu, selected_alg_2, 2);

									else if (event.key.code == Keyboard::Num3)
										updateAlgorithm(&grid, menu, selected_alg_2, 3);

									else if (event.key.code == Keyboard::Num4) {

										if (selected_alg_2) grid.partial_clean(3);
										selected_alg_2 = false;

										Grid temp = grid; // se copia cuando solo hay inicio y fin

										thread dijkstra_thread([&]() {Times[0] = temp.dijkstra_time(); });
										execute_4(&grid, menu, menu_comparison, 1, "Dijkstra");
										dijkstra_thread.join();
										grid.partial_clean(3);

										thread dfs_thread([&]() {Times[1] = temp.dfs_time(); });
										execute_4(&grid, menu, menu_comparison, 2, "DFS");
										dfs_thread.join();
										grid.partial_clean(3);

										thread bfs_thread([&]() {Times[2] = temp.bfs_time(); });
										execute_4(&grid, menu, menu_comparison, 3, "BFS");
										bfs_thread.join();

										comparison_table = true;
										menu->activate_comparison_table(comparison_table);
										selected_alg_2 = true;

									}

								}

							}

						}

						else if (event.key.code == Keyboard::Space) {

							if (number_of_map == 1) {

								if (small_grid.getAlgorithm() != 0 && (small_grid.isBegin() == true && small_grid.isEnd() == true)) {
									if (!selected_alg_1) {

										Grid temp = small_grid;

										if (small_grid.getAlgorithm() == 1) {
											thread dijkstra_thread([&]() {Times[0] = temp.dijkstra_time(); });
											small_grid.dijkstra(menu);
											dijkstra_thread.join();
											execute_1_2_3(&small_grid, menu, 1, selected_alg_1);
										}
										else if (small_grid.getAlgorithm() == 2) {
											thread dfs_thread([&]() {Times[1] = temp.dfs_time(); });
											small_grid.dfs(menu);
											dfs_thread.join();
											execute_1_2_3(&small_grid, menu, 2, selected_alg_1);
										}
										else if (small_grid.getAlgorithm() == 3) {
											thread bfs_thread([&]() {Times[2] = temp.bfs_time(); });
											small_grid.bfs(menu);
											bfs_thread.join();
											execute_1_2_3(&small_grid, menu, 3, selected_alg_1);
										}
									}
								}
								else menu->changeDynamicText(1, 0);

							}
							else if (number_of_map == 2) {

								if (grid.getAlgorithm() != 0 && (grid.isBegin() == true && grid.isEnd() == true)) {
									if (!selected_alg_2) {

										Grid temp = grid;

										if (grid.getAlgorithm() == 1) {
											thread dijkstra_thread([&]() {Times[0] = temp.dijkstra_time(); });
											grid.dijkstra(menu);
											dijkstra_thread.join();
											execute_1_2_3(&grid, menu, 1, selected_alg_2);
										}
										else if (grid.getAlgorithm() == 2) {
											thread dfs_thread([&]() {Times[1] = temp.dfs_time(); });
											grid.dfs(menu);
											dfs_thread.join();
											execute_1_2_3(&grid, menu, 2, selected_alg_2);
										}
										else if (grid.getAlgorithm() == 3) {
											thread bfs_thread([&]() {Times[2] = temp.bfs_time(); });
											grid.bfs(menu);
											bfs_thread.join();
											execute_1_2_3(&grid, menu, 3, selected_alg_2);
										}
									}

								}
								else menu->changeDynamicText(1, 0);

							}

						}

						else if (event.key.code == Keyboard::Up || event.key.code == Keyboard::Down) {

							if (event.key.code == Keyboard::Up) number_of_map = 1;
							else number_of_map = 2;

						}

						else if (event.key.code == Keyboard::Left || event.key.code == Keyboard::Right) {
							grid.changeSelectedGrid();
							small_grid.changeSelectedGrid();
						}

					}

				}

			}

			if (number_of_map == 1) small_grid.draw();
			else if (number_of_map == 2) grid.draw();

			if (menu->get_activate_help_window())
				menu_help_window->draw();
			else if (menu->get_activate_comparison())
				menu_comparison->draw();
			else menu->draw();

		}

	}

	Tuple<> getTuple() {

		int node_size = 40;

		if (number_of_map == 1) node_size = 32;
		else if (number_of_map == 2) node_size = 40;

		Vector2i mouse = Mouse::getPosition(window);

		int x = mouse.x / node_size;
		int y = (mouse.y - HEADER_HEIGHT) / node_size;

		return Tuple<>(x, y);

	}

	void setBegin(Grid* grid, Tuple<> tuple, bool& selected_algorithm) {
		if (selected_algorithm) grid->partial_clean(1);
		selected_algorithm = false; // revisar el orden de estos dos if
		if (grid->getEnd() == tuple) grid->clearEnd();
		grid->setBegin(tuple);
	}

	void setEnd(Grid* grid, Tuple<> tuple, bool& selected_algorithm) {
		if (selected_algorithm) grid->partial_clean(2);
		selected_algorithm = false; // revisar el orden de estos dos if
		if (grid->getBegin() == tuple) grid->clearBegin();
		grid->setEnd(tuple);
	}

	void totalClean(Grid* grid, bool& selected_algorithm) {
		grid->total_clean();
		selected_algorithm = false;
	}

	void updateAlgorithm(Grid* grid, MenuAbstract* menu, bool& selected_algorithm, int i) {
		if (selected_algorithm) grid->partial_clean(3); // no borra inicio ni fin
		selected_algorithm = false;
		menu->updateSelector(i);
		menu->changeDynamicText(i + 1, 0);
		grid->setAlgorithm(i);
	}

	void execute_4(Grid* grid, MenuAbstract* menu, MenuAbstract* mc, int i, string algorithm) {
		menu->updateSelector(i);
		menu->changeDynamicText(i + 1, 0);
		grid->setAlgorithm(i);
		if (i == 1) grid->dijkstra(menu);
		else if (i == 2) grid->dfs(menu);
		else if (i == 3) grid->bfs(menu);
		menu->changeDynamicText(i + 4, Times[i - 1]);
		mc->append(algorithm, Times[i - 1]);
	}

	void execute_1_2_3(Grid* grid, MenuAbstract* menu, int i, bool& selected_algorithm) {
		menu->changeDynamicText(i + 4, Times[i - 1]);
		selected_algorithm = true;
	}

};
