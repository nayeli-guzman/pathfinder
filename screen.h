#pragma once

#define ROWS 16
#define COLS 40
#define NODE_SIZE 40
#define HEADER_HEIGHT 200

#include <SFML/Graphics.hpp>
#include "grid.h"
#include "menu.h"
#include <vector>
#include <thread>


using namespace std;
using namespace sf;

class Screen {
private:
    RenderWindow window;
    bool help_window = false;
    bool comparison_table = false;
    bool selected_alg_1 = false, selected_alg_2 = false;
    int number_of_map = 2;
    static Screen* instancia;
    vector<double> Times = { 0,0,0 };
    Screen() {}

public:
    static Screen* getInstance() {
        if (instancia == nullptr)
            instancia = new Screen;
        return instancia;
    }

    void execute() {
        window.create({ NODE_SIZE * COLS, NODE_SIZE * ROWS + HEADER_HEIGHT }, "Project");
        window.setFramerateLimit(60);

        Grid grid(ROWS, COLS, window, 2);
        Grid small_grid(20, 50, window, 1);
        Menu menu(HEADER_HEIGHT, NODE_SIZE * COLS, NODE_SIZE * ROWS, window);

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
                        Tuple current_pos = getTuple();
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
                                if (selected_alg_2)
                                    grid.partial_clean(1);
                                selected_alg_2 = false;
                                if (grid.getEnd() == current_pos)
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
                                grid.total_clean();
                                selected_alg_2 = false;
                            }
                        }
                        else if (event.key.code >= Keyboard::Num1 && event.key.code <= Keyboard::Num4) {
                            if (number_of_map == 1) {
                                if (small_grid.isBegin() == false || small_grid.isEnd() == false) {
                                    menu.changeDynamicText(1, 0);
                                }
                                else {
                                    if (event.key.code == Keyboard::Num1) {
                                        if (selected_alg_1)
                                            small_grid.partial_clean(3);
                                        selected_alg_1 = false;
                                        menu.updateSelector(1);
                                        menu.changeDynamicText(2, 0);
                                        small_grid.setAlgorithm(1);
                                    }
                                    else if (event.key.code == Keyboard::Num2) {
                                        if (selected_alg_1)
                                            small_grid.partial_clean(3);
                                        selected_alg_1 = false;
                                        menu.updateSelector(2);
                                        menu.changeDynamicText(3, 0);
                                        small_grid.setAlgorithm(2);
                                    }
                                    else if (event.key.code == Keyboard::Num3) {
                                        if (selected_alg_1)
                                            small_grid.partial_clean(3);
                                        selected_alg_1 = false;
                                        menu.updateSelector(3);
                                        menu.changeDynamicText(4, 0);
                                        small_grid.setAlgorithm(3);
                                    }
                                    else if (event.key.code == Keyboard::Num4) {
                                        menu.updateSelector(1);
                                        menu.changeDynamicText(2, 0);
                                        small_grid.setAlgorithm(1);
                                        small_grid.dijkstra(menu);
                                        Node** temp = small_grid.get_Grid();
                                        thread dijkstra_thread([&]() {
                                            Times[0] = small_grid.dijkstra_time(temp);
                                            });

                                        thread dfs_thread([&]() {
                                            Times[1] = small_grid.dfs_time(temp);
                                            });

                                        thread bfs_thread([&]() {
                                            Times[2] = small_grid.bfs_time(temp);
                                            });

                                        // Esperar a que todos los hilos terminen antes de continuar
                                        dijkstra_thread.join();
                                        dfs_thread.join();
                                        bfs_thread.join();

                                        menu.changeDynamicText(5, Times[0]);
                                        menu.append("Dijkstra", Times[0]);
                                        small_grid.partial_clean(3);
                                        menu.updateSelector(2);
                                        menu.changeDynamicText(3, 0);
                                        small_grid.setAlgorithm(2);
                                        small_grid.dfs(menu);
                                        menu.changeDynamicText(6, Times[1]);
                                        menu.append("DFS", Times[1]);
                                        small_grid.partial_clean(3);
                                        menu.updateSelector(3);
                                        menu.changeDynamicText(4, 0);
                                        small_grid.setAlgorithm(3);
                                        small_grid.bfs(menu);
                                        menu.changeDynamicText(7, Times[2]);
                                        menu.append("BFS", Times[2]);
                                        comparison_table = true;
                                        menu.activate_comparison_table(comparison_table);
                                        selected_alg_1 = true;
                                    }
                                }
                            }
                            else if (number_of_map == 2) {
                                if (grid.isBegin() == false || grid.isEnd() == false) {
                                    menu.changeDynamicText(1, 0);
                                }
                                else {
                                    if (event.key.code == Keyboard::Num1) {
                                        if (selected_alg_2)
                                            grid.partial_clean(3);
                                        selected_alg_2 = false;
                                        menu.updateSelector(1);
                                        menu.changeDynamicText(2, 0);
                                        grid.setAlgorithm(1);
                                    }
                                    else if (event.key.code == Keyboard::Num2) {
                                        if (selected_alg_2)
                                            grid.partial_clean(3);
                                        selected_alg_2 = false;
                                        menu.updateSelector(2);
                                        menu.changeDynamicText(3, 0);
                                        grid.setAlgorithm(2);
                                    }
                                    else if (event.key.code == Keyboard::Num3) {
                                        if (selected_alg_2)
                                            grid.partial_clean(3);
                                        selected_alg_2 = false;
                                        menu.updateSelector(3);
                                        menu.changeDynamicText(4, 0);
                                        grid.setAlgorithm(3);
                                    }
                                    else if (event.key.code == Keyboard::Num4) {
                                        menu.updateSelector(1);
                                        menu.changeDynamicText(2, 0);
                                        grid.setAlgorithm(1);
                                        grid.dijkstra(menu);
                                        Node** temp = grid.get_Grid();
                                        thread dijkstra_thread([&]() {
                                            Times[0] = grid.dijkstra_time(temp);
                                            });

                                        thread dfs_thread([&]() {
                                            Times[1] = grid.dfs_time(temp);
                                            });

                                        thread bfs_thread([&]() {
                                            Times[2] = grid.bfs_time(temp);
                                            });

                                        // Esperar a que todos los hilos terminen antes de continuar
                                        dijkstra_thread.join();
                                        dfs_thread.join();
                                        bfs_thread.join();
                                        menu.changeDynamicText(5, Times[0]);
                                        menu.append("Dijkstra", Times[0]);
                                        grid.partial_clean(3);
                                        menu.updateSelector(2);
                                        menu.changeDynamicText(3, 0);
                                        grid.setAlgorithm(2);
                                        grid.dfs(menu);
                                        menu.changeDynamicText(6, Times[1]);
                                        menu.append("DFS", Times[1]);
                                        grid.partial_clean(3);
                                        menu.updateSelector(3);
                                        menu.changeDynamicText(4, 0);
                                        grid.setAlgorithm(3);
                                        grid.bfs(menu);
                                        menu.changeDynamicText(7, Times[2]);
                                        menu.append("BFS", Times[2]);
                                        comparison_table = true;
                                        menu.activate_comparison_table(comparison_table);
                                        selected_alg_2 = true;
                                    }
                                }
                            }
                        }
                        else if (event.key.code == Keyboard::Space) {
                            if (number_of_map == 1) {
                                if (small_grid.getAlgorithm() != 0 && (small_grid.isBegin() == true && small_grid.isEnd() == true)) {
                                    if (!selected_alg_1) {
                                        if (small_grid.getAlgorithm() == 1) {
                                            small_grid.dijkstra(menu);
                                            Node** temp = small_grid.get_Grid();
                                            Times[0] = small_grid.dijkstra_time(temp);
                                            menu.changeDynamicText(5, Times[0]);
                                            selected_alg_1 = true;
                                        }
                                        else if (small_grid.getAlgorithm() == 2) {
                                            small_grid.dfs(menu);
                                            Node** temp = small_grid.get_Grid();
                                            Times[1] = small_grid.dfs_time(temp);
                                            menu.changeDynamicText(6, Times[1]);
                                            selected_alg_1 = true;
                                        }
                                        else if (small_grid.getAlgorithm() == 3) {
                                            small_grid.bfs(menu);
                                            Node** temp = small_grid.get_Grid();
                                            Times[2] = small_grid.bfs_time(temp);
                                            menu.changeDynamicText(7, Times[2]);
                                            selected_alg_1 = true;
                                        }
                                    }
                                }
                                else menu.changeDynamicText(1, 0);
                            }
                            else if (number_of_map == 2) {
                                if (grid.getAlgorithm() != 0 && (grid.isBegin() == true && grid.isEnd() == true)) {
                                    if (!selected_alg_2) {
                                        if (grid.getAlgorithm() == 1) {
                                            Node** temp = grid.get_Grid();
                                            thread dijkstra_thread([&]() {
                                                Times[2] = grid.dijkstra_time(temp);
                                                });
                                            grid.dijkstra(menu);
                                            dijkstra_thread.join();
                                            menu.changeDynamicText(5, Times[0]);
                                            selected_alg_2 = true;
                                        }
                                        else if (grid.getAlgorithm() == 2) {
                                            Node** temp = grid.get_Grid();
                                            thread dfs_thread([&]() {
                                                Times[1] = grid.dfs_time(temp);
                                                });
                                            grid.dfs(menu);
                                            dfs_thread.join();

                                            menu.changeDynamicText(6, Times[1]);
                                            selected_alg_2 = true;
                                        }
                                        else if (grid.getAlgorithm() == 3) {
                                            Node** temp = grid.get_Grid();
                                            thread bfs_thread([&]() {
                                                Times[2] = grid.bfs_time(temp);
                                                });
                                            grid.bfs(menu);
                                            bfs_thread.join();

                                            menu.changeDynamicText(7, Times[2]);
                                            selected_alg_2 = true;
                                        }
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
            if (number_of_map == 1)
                small_grid.draw();
            else if (number_of_map == 2)
                grid.draw();
            menu.draw();
        }
    }

    Tuple getTuple() {
        int node_size = 40;
        if (number_of_map == 1)
            node_size = 32;
        else if (number_of_map == 2)
            node_size = 40;
        Vector2i mouse = Mouse::getPosition(window);
        int x = mouse.x / node_size;
        int y = (mouse.y - HEADER_HEIGHT) / node_size;
        return Tuple(x, y);
    }
};
