#include <vector>
#include <list>
#include <set>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("input.txt");
ofstream cout("output.txt");
#else
#include <iostream>
#endif

vector<int> topSort(vector<set<int> > v, vector<set<int> > vT) {
    vector<int> ans;
    set<int> s;

    for (int i = 0; i < v.size(); i++)
        if (vT[i].empty())
            s.insert(i);

    while (!s.empty()) {
        int curr = *s.begin();
        s.erase(s.begin());
        ans.push_back(curr);

        for (set<int>::iterator it = v[curr].begin(); it != v[curr].end(); it++) {
            vT[*it].erase(curr);
            if (vT[*it].empty())
                s.insert(*it);
        }

        v[curr].clear();
    }

    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;
    vector<set<int> > v(n), vT(n);
    for (int i = 0; i < m; i++) {
        int a, b;
        cin >> a >> b;
        a--;
        b--;
        v[a].insert(b);
        vT[b].insert(a);
    }
    int start, finish, k;
    cin >> start >> finish >> k;
    start--;
    finish--;
    set<int> places;
    for (int i = 0; i < k; i++) {
        int place;
        cin >> place;
        place--;
        places.insert(place);
    }
    places.insert(start);
    places.insert(finish);

    vector<int> order = topSort(v, vT);

    vector<int> dp(n, 0);
    vector<int> lastVisited(n, -1);
    vector<int> indexInOrder(n);
    for (int i = 0; i < n; i++)
        indexInOrder[order[i]] = i;

    for (set<int>::iterator it = places.begin(); it != places.end(); it++)
        if (indexInOrder[*it] < indexInOrder[start]) {
            cout << 0 << "\n";
            return 0;
        }

    int currVisited = -1;

    bool skip = true;
    for (int i = 0; i < n; i++) {
        int vertex = order[i];
        if (vertex == start) {
            skip = false;
        }

        if (skip)
            continue;

        if (vertex == start || (lastVisited[vertex] == currVisited && vT[vertex].empty())) {
            dp[vertex] = 1;
        } else
            for (set<int>::iterator it = vT[vertex].begin(); it != vT[vertex].end(); it++)
                if (lastVisited[*it] == currVisited) {
                    dp[vertex] += dp[*it];
                    lastVisited[vertex] = max(lastVisited[vertex], lastVisited[*it]);
                }

        if (places.find(vertex) != places.end()) {
            currVisited = indexInOrder[vertex];
            lastVisited[vertex] = indexInOrder[vertex];
        }
    }

    cout << dp[finish] << "\n";

    return 0;
}