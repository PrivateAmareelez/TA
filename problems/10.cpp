#include <vector>
#include <stack>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("input.txt");
ofstream cout("output.txt");
#else
#include <iostream>
#endif

enum Direction {
    UP, DOWN, LEFT, RIGHT
};

bool dfs(int v, vector<vector<char> > &maze) {

    stack<pair<pair<int, int>, Direction> > s;
    s.push(make_pair(make_pair(0, v), UP));

    while (!s.empty()) {

    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;
    vector<vector<char> > maze(n, vector<char>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            cin >> maze[i][j];


    return 0;
}