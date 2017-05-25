#include <vector>
#include <queue>
#include <deque>
#include <list>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("in.txt");
ofstream cout("out.txt");
#else
#include <iostream>
#endif

const int k = 2;
int n, m;

vector<vector<int> > v;
vector<int> d;
deque<queue<int> > q;
vector<list<pair<int, int> > > g;

bool isOnBoard(int i, int j) { // x = i * m + j
    if (i < 0 || i >= n || j < 0 || j >= m)
        return false;
    return true;
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(0);
    //while (true) {
    /*v.clear();
    d.clear();
    q.clear();
    g.clear();*/
    cin >> n >> m;
    //n = rand() % 1000 + 1;
    //m = rand() % 1000 + 1;

    v.resize(n, vector<int>(m));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            cin >> v[i][j];
            //v[i][j] = -1 + rand() % 3;
        }

    int x1, y1, x2, y2;
    cin >> x1 >> y1 >> x2 >> y2;
    /*x1 = 1 + rand() % n;
    y1 = 1 + rand() % m;
    x2 = 1 + rand() % n;
    y2 = 1 + rand() % m;*/
    x1--;
    y1--;
    x2--;
    y2--;

    if (!isOnBoard(x1, y1) || !isOnBoard(x2, y2) || v[x2][y2] == -1) {
        cout << "No\n";
        //continue;
        return 0;
    }

    g.resize(n * m);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            for (int di = -2; di <= 2; di += 4)
                for (int dj = -1; dj <= 1; dj += 2)
                    if (isOnBoard(i + di, j + dj)) {
                        if (v[i + di][j + dj] == 0)
                            g[i * m + j].push_back(make_pair((i + di) * m + (j + dj), 1));
                        else if (v[i + di][j + dj] == 1)
                            g[i * m + j].push_back(make_pair((i + di) * m + (j + dj), 2));
                    }

            for (int dj = -2; dj <= 2; dj += 4)
                for (int di = -1; di <= 1; di += 2)
                    if (isOnBoard(i + di, j + dj)) {
                        if (v[i + di][j + dj] == 0)
                            g[i * m + j].push_back(make_pair((i + di) * m + (j + dj), 1));
                        else if (v[i + di][j + dj] == 1)
                            g[i * m + j].push_back(make_pair((i + di) * m + (j + dj), 2));
                    }

        }

    int maxLen = (n * m - 1) * k + 1;
    d.resize(n * m, INT32_MAX);
    q.resize(k + 1);

    queue<int> temp;
    temp.push(x1 * m + y1);
    q.pop_front();
    q.push_front(temp);

    d[x1 * m + y1] = 0;

    bool found = false;
    int deltaLen = 0;

    while (!found) {
        const int len = 0;

        if (q[len].empty())
            break;

        while (!q[len].empty()) {
            int curr = q[len].front();
            q[len].pop();

            if (curr == x2 * m + y2) {
                found = true;
                break;
            }

            if (d[curr] != len + deltaLen)
                continue;

            for (list<pair<int, int> >::iterator it = g[curr].begin(); it != g[curr].end(); it++) {
                if (d[it->first] > len + deltaLen + it->second) {
                    d[it->first] = len + deltaLen + it->second;
                    q[len + it->second].push(it->first);
                }
            }
        }

        while (!q.empty() && q[0].empty())
            q.pop_front();
        while (q.size() < k + 1) {
            queue<int> temp2;
            q.push_back(temp2);
            deltaLen++;
        }
    }

    if (d[x2 * m + y2] == INT32_MAX)
        cout << "No\n";
    else
        cout << d[x2 * m + y2] << "\n";
    //}
    return 0;
}