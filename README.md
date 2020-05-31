# Sudoku-Solver
Created a Sudoku Solver using OpenGl in C++. The window is created using GLFW library and modern OpenGl function were extracted using GLEW. Used spdlog library for logging messages to the console.

**Usage:**
User can either use the mouse or the arrow keys to select a tile (for entering the numbers.) The 'S' key, solves the sudoku. While it is solving, the user cannot enter any numbers or select a tile. The 'Q' key manually steps through the solving algorithm (only if the 'S' key was not pressed). The 'C' key clears the board and all its numbers.

**Images:**
<img src = "Images\img1.PNG" width="600">  
<img src = "Images\img2.PNG" width="600">
<img src = "Images\img3.PNG" width="600">
<img src = "Images\img4.PNG" width="600">

**Algorithm:**
The algortihm used to solve the sudoku is very similar to how a human would approach it. The algorithm calculates and stores all the possibilities of a given cell. If a cell has only one possibility then it assigns that value to the cell. If a row/column/3X3 block has only one cell where a number could exist, then it assigns that value to the cell (Eg: if in a row, if the number one can exist in the first cell but not in any of the other cells, then the first cell must contain a one). If at any time the algorithm is not discovering new cells by this method then it makes an assumption. It creates a copy of the board and arbitrarily makes an assumption for the value of a cell (based on the possible values). This can happen several times. If a contradiction occured in the board then the board reevrts back to the original version and removes the assumption from one of cell's possibilities. If too many assumptions were made, then the algorithm stops and gives up.
