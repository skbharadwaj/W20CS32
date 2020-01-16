#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;

// History Implementation

History::History(int nRows, int nCols)
{
    m_r = nRows;
    m_c = nCols;
    for (int r = 0; r < m_r; r++)
        for (int c = 0; c < m_c; c++)
            grid[r][c] = 0;
}

bool History::record(int r, int c)
{
    if (r <= 0 || c <= 0 || r > m_r || c > m_c)
        return false;
    grid[r-1][c-1]++;
    return true;
}

void History::display() 
{
    char displayGrid[MAXROWS][MAXCOLS];
    for (int r = 0; r < m_r; r++)
        for (int c = 0; c < m_c; c++) {
                if(grid[r][c] == 0) displayGrid[r][c] = '.';
                if(grid[r][c] == 1) displayGrid[r][c] = 'A';
                if(grid[r][c] == 2) displayGrid[r][c] = 'B';
                if(grid[r][c] == 3) displayGrid[r][c] = 'C';
                if(grid[r][c] == 4) displayGrid[r][c] = 'D';
                if(grid[r][c] == 5) displayGrid[r][c] = 'E';
                if(grid[r][c] == 6) displayGrid[r][c] = 'F';
                if(grid[r][c] == 7) displayGrid[r][c] = 'G';
                if(grid[r][c] == 8) displayGrid[r][c] = 'H';
                if(grid[r][c] == 9) displayGrid[r][c] = 'I';
                if(grid[r][c] == 10) displayGrid[r][c] = 'J';
                if(grid[r][c] == 11) displayGrid[r][c] = 'K';
                if(grid[r][c] == 12) displayGrid[r][c] = 'L';
                if(grid[r][c] == 13) displayGrid[r][c] = 'M';
                if(grid[r][c] == 14) displayGrid[r][c] = 'N';
                if(grid[r][c] == 15) displayGrid[r][c] = 'O';
                if(grid[r][c] == 16) displayGrid[r][c] = 'P';
                if(grid[r][c] == 17) displayGrid[r][c] = 'Q';
                if(grid[r][c] == 18) displayGrid[r][c] = 'R';
                if(grid[r][c] == 19) displayGrid[r][c] = 'S';
                if(grid[r][c] == 20) displayGrid[r][c] = 'T';
                if(grid[r][c] == 21) displayGrid[r][c] = 'U';
                if(grid[r][c] == 22) displayGrid[r][c] = 'V';
                if(grid[r][c] == 23) displayGrid[r][c] = 'W';
                if(grid[r][c] == 24) displayGrid[r][c] = 'X';
                if(grid[r][c] == 25) displayGrid[r][c] = 'Y';
                if(grid[r][c] >= 26) displayGrid[r][c] = 'Z';
            }
    
    clearScreen();
    for (int i = 0; i < m_r; i++)
    {
        for (int j = 0; j < m_c; j++)
            cout << displayGrid[i][j];
        cout << endl;
    }

    cout << endl;
}