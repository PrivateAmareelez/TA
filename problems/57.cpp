#include <vector>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("input.txt");
ofstream cout("output.txt");
#else
#include <iostream>
#endif

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int m, c, n;
    cin >> m >> c >> n;

    vector<int> table(m, -1);
    for (int i = 0; i < n; i++) {
        int num;
        cin >> num;

        int attempt = 0, index = (num % m + c * attempt) % m;
        while (table[index] != -1) {
            if (table[index] == num)
                break;
            attempt++;
            index = (num % m + c * attempt) % m;
        }

        if (table[index] == -1) {
            table[index] = num;
        }
    }

    for (int i = 0; i < m; i++) {
        cout << table[i] << " ";
    }
    cout << "\n";
    return 0;
}