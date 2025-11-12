
![GoLsim](https://github.com/user-attachments/assets/b284d883-620e-481c-8380-ec55ade799c9)

## Game-of-Life-Simulation
Simulating John Conway's Game of Life using a Sparse Matrix in Raylib (for a data structures mini-project)
> What is the "Game of Life"?

A zero-player game. The user is presented with a grid of cells, which can either live (1) or die (0) - the user can select some cells to be live as a _starting state_, and then based on a few mathematical rules, once "**PLAY**" is pressed, the figure the user has drawn will change:
> RULES -
-  **Rule of Solitude**: If a live cell is alone or has one live neighbor, the cell dies.
-  **Rule of Overpopulation**: If a live cell has four or more live neighbors, the cell dies.
-  **Rule of Moderation**: If the a live cell has two or three neighbors, the cell lives on.
-  **Rule of Creation**: If a dead cell has exactly thee neighbors, the cell becomes live.

Based on these rules, the state of every cell in the next instant of time (next iteration) gets decided. Many figures can be formed of them.
# Features
1. GUI with **_Raylib_**
2. User customizability - Click on empty cells to draw, click on filled cells to erase
3. Speed controls
4. Lexicon (preload figures)
# Applications
Apart from just being a fascinating museum-piece, it has great practical utility:
+ It's **Turing Complete**, which means it can compute any automata which lies under the realm of computability.
+ Being an example of a **Cellular automaton**, it can model and predict complex data like population dynamics, epidemic spread, and ecosystem evolution.
+ It makes for a great benchmark for testing **multi-threading** in CPUs.
+ **Terrain generation** in video games.
# Resources
- https://youtu.be/R9Plq-D1gEk
- https://youtu.be/CgOcEZinQ2I
