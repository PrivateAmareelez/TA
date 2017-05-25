#include <vector>
#include <set>
#include <iomanip>
#include <algorithm>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("lazy.in");
ofstream cout("lazy.out");
#else
#include <iostream>
#endif

class Order {
public:
    long long boost, time, critical_time;

    Order() {
        boost = time = critical_time = 0;
    }

    Order(long long a, long long b, long long c) {
        boost = a;
        time = b;
        critical_time = c;
    }
};

bool comp(const Order &x, const Order &y) {
    return x.critical_time < y.critical_time;
}

class Comparator {
public:
    bool operator()(const Order &x, const Order &y) {
        return x.boost > y.boost;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n;
    cin >> n;

    vector<Order> v(n);
    for (int i = 0; i < n; i++) {
        long long a, b, c;
        cin >> a >> b >> c;
        v[i] = Order(a, b, c);
    }

    sort(v.begin(), v.end(), comp);

    multiset<Order, Comparator> s;
    double time = 0, ans = 0;
    for (int i = 0; i < n; i++) {
        ]
        s.insert(v[i]);
        time += v[i].time;

        while (time - v[i].critical_time > 0) {
            Order curr = *s.begin();
            s.erase(s.begin());

            double compensation = min(1. * curr.time, time - v[i].critical_time);
            curr.time -= compensation;
            time -= compensation;
            ans += compensation / curr.boost;

            if (curr.time > 0)
                s.insert(curr);
        }
    }

    cout << fixed << setprecision(4) << ans << "\n";
    return 0;
}