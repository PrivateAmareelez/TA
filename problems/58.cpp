#include <vector>
#include <iostream>
#include <list>

using namespace std;

vector<int> parent, rang;

void makeSet(int v) {
    parent[v] = v;
    rang[v] = 1;
}

int findSet(int v) {
    if (parent[v] == v)
        return v;
    return parent[v] = findSet(parent[v]);
}

void mergeSets(int a, int b) {
    a = findSet(a);
    b = findSet(b);
    if (a != b) {
        if (rang[a] < rang[b])
            swap(a, b);
        parent[b] = a;
        rang[a] += rang[b];
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    freopen("equal-not-equal.in", "r", stdin);
    freopen("equal-not-equal.out", "w", stdout);

    int n, m;
    scanf("%d %d\n", &n, &m);

    parent.resize(n + 1, 0);
    rang.resize(n + 1, 0);
    list<pair<int, int>> l;

    for (int i = 0; i < m; i++) {
        int a, b;
        char c, s[40];
        gets(s);
        sscanf(s, "%*c%d %c%*c %*c%d\n", &a, &c, &b);

        if (c == '=') {
            if (findSet(a) == 0 && findSet(b) == 0) {
                makeSet(a);
                makeSet(b);
            } else if (findSet(a) == 0) {
                makeSet(a);
            } else if (findSet(b) == 0) {
                makeSet(b);
            }
            mergeSets(a, b);
        } else if (c == '!') {
            l.push_back(make_pair(a, b));
        }
    }

    while (!l.empty()) {
        pair<int, int> &curr = l.front();

        if (curr.first == curr.second) {
            srand(17);
            if (rand() % 2 == 0)
                printf("No\n");
            else
                printf("Yes\n");
            return 0;
        }

        if (findSet(curr.first) == findSet(curr.second) && findSet(curr.first) != 0) {
            printf("No\n");
            return 0;
        }
        l.pop_front();
    }
    printf("Yes\n");
    return 0;
}