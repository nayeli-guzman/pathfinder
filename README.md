# pathfinder

**screen.h:** 


**menu.h:**


**grid.h**

**Observaciones/Falta:**

- Falta poner lo del 4 en Menu

- Modificar el laberinto para que todo este conectado. (Modificar generateMazeDFS o todo de nuevo).

- Al momento de ejecutar 4, la tabla de posiciones debería ser de menor a mayor tiempo 

- **Implementar varios patrones de diseño**

- Video

- Demo

- Readme
----------------------------------------
Informe:

# Proyecto Final de Programación III: Laberintos con DFS, BFS y Dijsktra

## Introducción:
Este proyecto tiene como objetivo la comparación de la eficiencia de los algoritmos al momento de realizar la tarea planteada, la cual, es encontrar el camino más corto desde un punto elegido por el usuario a otro. Estos algoritmos serán evaluados en diferentes tamaños y rutas posibles del laberinto. Para finalmente, registrar los tiempos obtenidos, promediar y comparar y determinar cuál es el más óptimo.

Este proyecto aplicaremos en la práctica lo aprendido en el ciclo como contenedores, clases concurrencia, complejidad algorítmica, patrones de diseño, etc. Implementar los algoritmos DFS (Depth-First Search), BFS (Breadth-first search) y Dijsktra nos ayudará a comprender cómo funcionan y el cómo exploran de forma distinta y encuentran el camino. Nos ayudará a evaluar la optimización y el rendimiento de las aplicaciones que vayamos a desarrollar en el futuro.

# Requerimientos y guia de instalación:
---Indicar los requerimientos: núcleos, procesador, etc.---
El programa utiliza la librería externa SFML para el desarrollo de la interfaz al momento de desplegar el programa, descargar la librería a través del link https://www.sfml-dev.org/download.php  

# Instrucciones
---Comandos para ejecutar el programa---

# Descripción del proyecto y características principales:

Las librerías utilizadas en la creación del proyecto fueron:
  - SFML/Graphics.hpp
  - iostream
  - queue
  - stack
  - chrono
  - vector
  - algorithm
  - map
  - ctime
El proyecto se compone de 4 archivos (grid.h, menu.h, node.h, screen.h )  en donde se desarrolló toda el funcionamiento del algoritmo y un archivo (proyecto_x64.cpp) el cual ejecuta el programa:
  - Archivo grid.h: …
  - Archivo menu.h: Es este archivo se desarrolla la clase Menu, la cual, consta de 11 métodos. La clase Menu tiene 
    - Menu: Asigna los valores a las variables. Además Inicializa 
    - setBackgrounds: Inicia los fondos de todas las secciones del menú.  
    - setTexts: Inicia las fuentes, tamaño y posición de los textos en el menú principal..
    - setTextHelpWindow: Inicia las fuentes, tamaño y posición de los textos en el menú de ayuda.
    - setTextComparison: Inicia y configura los textos en la tabla de comparación de tiempos.
    - changeDynamicText: Imprime un mensaje distinto según el índice.
    - updateSelector: Cambia la posición del selector de algoritmos.
    - activate_help_window: Despliega el menú de ayuda al usuario.
    - activate_comparison_table: Despliega la tabla de comparación de tiempos.
    - draw: Ejecuta todos los elementos del menú en la ventana de renderizado.
    - append: Agrega el tiempo en una lista dependiendo del algoritmo.
  - Archivo node.h: Se encuentran las clases Tuple y Node y cuentan con 5 y 25 métodos respectivamente. La clase Tuple cuenta con 2 variables (x,y) de tipo genérico.
    - Tuple(): El constructor por defecto asigna 0 como valor por defecto a las variables de su interior.
    - Tuple(T, T): Asigna el valor de las variables a través de los parámetros que recibe.
    - Tuple(Tuple): Copia las variables de otra Tuple
    - get_x: Retorna el valor de x.
    - get_y: Retorna el valor de y.
    En la clase Tuple están sobrecargados los operadores == y != para ayudar a compararse con otros. En la clase Node se encuentra las variables (square, image, grid, type, tuple, x, y, numbrer_grid, v_n, visited, queued, is_start, is_end,         prior, dijkstra, rows, cols, node_size, locked).
    - Node(): Constructor por defecto.
    - Node(Tuple. char, int): Asigna las coordenadas, el tipo y el número de cuadrícula del nodo.
    - animation: Cambia la animación del nodo.
    - filterImages_x32 y filterImages_x40: Aplica diferentes texturas al nodo según el tipo y el tamaño.
    - draw: Dibuja el nodo en la ventana de renderizado.
    - set_n: Agrega a los vecinos de cada nodo.
    - total_clean y partial_clean: Regresa parcial o totalmente un nodo a sus valores por defecto. 
    - setLocked, setQueued, setVisited, setType, setPrior, setIsStart, setIsEnd, setRows, setCols: Modifica los valores de sus respectivas variables
    - getLocked, getQueued, getVisited, getType, getPrior, getTuple, get_n: Retorna el valor actual de sus respectivas variables.
  - Archivo screen.h: Dentro se ubica la clase Screen y cuenta con ...
  - Archivo proyecto_x64.cpp: …

##Conclusiones:
...

##Bibliografía:
  - Documentación de SFML: https://www.sfml-dev.org/documentation/2.5.1/index.php





