#ifndef HISTORY_H
#define HISTORY_H

#include "globals.h"
#include <string>
using namespace std;

class History
{
	    public:
	        History(int nRows, int nCols);
            bool record(int r, int c);
            void display();
        private:
            int m_r, m_c;
            int grid[MAXROWS][MAXCOLS];
            void setVal(int r, int c, int val);
            void check(int r, int c, string functionName) ;
            bool inBounds(int r, int c) ;
};

#endif