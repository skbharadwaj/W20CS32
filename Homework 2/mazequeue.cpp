#include <queue>
#include <iostream>
#include <cstdlib>
using namespace std;
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

class Coord     {
        public:
	        Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    	    int r() const { return m_r; }
        	int c() const { return m_c; }
        private:
        	int m_r;
        	int m_c;
};

        int main()
        {
            string maze[10] = {
                "XXXXXXXXXX",
                "X...X..X.X",
                "X.XXX....X",
                "X.X.XXXX.X",
                "XXX......X",
                "X...X.XX.X",
                "X.X.X..X.X",
                "X.XXXX.X.X",
                "X..X...X.X",
                "XXXXXXXXXX"
            };
        
            if (pathExists(maze, 10,10, 4,3, 1,8))
                cout << "Solvable!" << endl;
            else
                cout << "Out of luck!" << endl;
        }

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	queue<Coord> coords;
	coords.push(Coord(sr, sc));
	maze[sr][sc] = 'E';	
	while(!coords.empty()) {
		Coord curr = coords.front();
        coords.pop();
        if(curr.r() == er && curr.c() == ec) return true;
        if(curr.r() < nRows && curr.c() < nCols) {
            if(maze[curr.r()][curr.c()+1] == '.') {
                coords.push(Coord(curr.r(),curr.c()+1));
                maze[curr.r()][curr.c()+1] = 'E';
            }
            if(maze[curr.r()+1][curr.c()] == '.') {
                coords.push(Coord(curr.r()+1,curr.c()));
                maze[curr.r()+1][curr.c()] = 'E';
            }
            if(maze[curr.r()][curr.c() - 1] == '.') {
                coords.push(Coord(curr.r(),curr.c() - 1));
                maze[curr.r()][curr.c()-1] = 'E';
            }
            if(maze[curr.r()-1][curr.c()] == '.') {
                coords.push(Coord(curr.r()-1,curr.c()));
                maze[curr.r()-1][curr.c()] = 'E';
            }
        }
	}
	return false;
}

