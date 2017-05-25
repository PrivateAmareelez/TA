#include <vector>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("input.txt");
ofstream cout("output.txt");
#else
#include <iostream>
#endif

template<class T>
class BinaryHeap {
private:
    vector<T> data;
    int size;

    void siftUp(int i) {
        while (i >= 0 && data[i] < data[(i - 1) / 2]) {
            swap(data[i], data[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    void siftDown(int i) {
        while (2 * i + 1 < size) {
            int left = 2 * i + 1, right = 2 * i + 2;
            int j = left;
            if (right < size && data[right] < data[left])
                j = right;

            if (data[i] <= data[j])
                break;
            swap(data[i], data[j]);
            i = j;
        }
    }

    void makeHeap() {
        for (int i = size / 2; i >= 0; i--)
            siftDown(i);
    }

public:

    BinaryHeap(vector<T> &v) {
        data.assign(v.begin(), v.end());
        size = v.size();
        makeHeap();
    }

    const T getMin() const {
        return data[0];
    }

    void deleteMin() {
        data[0] = data[size - 1];
        size--;
        siftDown(0);
    }

    void insert(T x) {
        size++;
        if (data.size() < size) {
            data.resize(data.size() * 2);
        }

        data[size - 1] = x;
        siftUp(size - 1);
    }

    void resize(int n) {
        data.resize(n + 1);
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    int n, m;
    cin >> n >> m;
    vector<vector<int> > v(n, vector<int>(m));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++)
            cin >> v[i][j];
    }

    vector<pair<int, int> > col(n);
    for (int i = 0; i < n; i++)
        col[i] = make_pair(v[i][0], i);
    BinaryHeap<pair<int, int> > heap(col);

    vector<int> left(n, 1);
    for (int i = 0; i < n * m - 1; i++) {
        pair<int, int> curr = heap.getMin();
        heap.deleteMin();

        cout << curr.first << " ";
        if (left[curr.second] < m)
            heap.insert(make_pair(v[curr.second][left[curr.second]++], curr.second));
    }

    cout << heap.getMin().first << "\n";

    return 0;
}