#include <iostream>
#include <stack>
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;
bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec);

class Coord	{
	public:
		Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
        int r() const { return m_r; }
        int c() const { return m_c; } 
	private:
        int m_r;
        int m_c;
};


bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
	stack<Coord> coords;
	coords.push(Coord(sr,sc));
	maze[sr][sc] = 'E';
	while(!coords.empty()) {
		Coord curr = coords.top();
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


















