#include <vector>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("input.txt");
ofstream cout("output.txt");
#else
#include <iostream>
#endif

int n;
vector<vector<int> > tree;

void buildY(int x, int y, int xLeft, int xRight, int yLeft, int yRight) {
    if (yLeft == yRight)
        return;

    if (yRight - yLeft == 1) {
        if (xRight - xLeft == 1)
            tree[x][y] = 0; //data[xLeft][yLeft]
        else
            tree[x][y] = tree[x * 2 + 1][y] + tree[x * 2 + 2][y];
    } else {
        int yMiddle = yLeft + ((yRight - yLeft) >> 1);
        buildY(x, y * 2 + 1, xLeft, xRight, yLeft, yMiddle);
        buildY(x, y * 2 + 2, xLeft, xRight, yMiddle, yRight);
        tree[x][y] = tree[x][y * 2 + 1] + tree[x][y * 2 + 2];
    }
}

void buildX(int x, int xLeft, int xRight) {
    if (xLeft == xRight)
        return;

    if (xRight - xLeft == 1)
        buildY(x, 0, xLeft, xRight, 0, n);
    else {
        int xMiddle = xLeft + ((xRight - xLeft) >> 1);
        buildX(2 * x + 1, xLeft, xMiddle);
        buildX(2 * x + 2, xMiddle, xRight);
        buildY(x, 0, xLeft, xRight, 0, n);
    }
}

long long sumY(int x, int y, int yLeft, int yRight, int yTargetLeft, int yTargetRight) {
    if (yTargetLeft >= yTargetRight || yLeft == yRight)
        return 0;

    if (yLeft == yTargetLeft && yRight == yTargetRight)
        return tree[x][y];

    int yMiddle = yLeft + ((yRight - yLeft) >> 1);
    return sumY(x, 2 * y + 1, yLeft, yMiddle, yTargetLeft, min(yMiddle, yTargetRight))
           + sumY(x, 2 * y + 2, yMiddle, yRight, max(yMiddle, yTargetLeft), yTargetRight);
}

long long sumX(int x, int xLeft, int xRight, int xTargetLeft, int xTargetRight, int yTargetLeft, int yTargetRight) {
    if (xTargetLeft >= xTargetRight || xLeft == xRight)
        return 0;

    if (xLeft == xTargetLeft && xRight == xTargetRight)
        return sumY(x, 0, 0, n, yTargetLeft, yTargetRight);

    int xMiddle = xLeft + ((xRight - xLeft) >> 1);
    return sumX(2 * x + 1, xLeft, xMiddle, xTargetLeft, min(xMiddle, xTargetRight), yTargetLeft, yTargetRight)
           + sumX(2 * x + 2, xMiddle, xRight, max(xMiddle, xTargetLeft), xTargetRight, yTargetLeft, yTargetRight);

}

void addY(int x, int y, int xLeft, int xRight, int yLeft, int yRight, int i, int j, int addition) {
    if (yLeft == yRight)
        return;

    if (yRight - yLeft == 1) {
        if (xRight - xLeft == 1)
            tree[x][y] += addition;
        else
            tree[x][y] = tree[x * 2 + 1][y] + tree[x * 2 + 2][y];
    } else {
        int yMiddle = yLeft + ((yRight - yLeft) >> 1);
        if (j < yMiddle)
            addY(x, 2 * y + 1, xLeft, xRight, yLeft, yMiddle, i, j, addition);
        else
            addY(x, 2 * y + 2, xLeft, xRight, yMiddle, yRight, i, j, addition);
        tree[x][y] = tree[x][2 * y + 1] + tree[x][2 * y + 2];
    }
}

void addX(int x, int xLeft, int xRight, int i, int j, int addition) {
    if (xLeft == xRight)
        return;

    if (xRight - xLeft == 1)
        addY(x, 0, xLeft, xRight, 0, n, i, j, addition);
    else {
        int xMiddle = xLeft + ((xRight - xLeft) >> 1);
        if (i < xMiddle)
            addX(2 * x + 1, xLeft, xMiddle, i, j, addition);
        else
            addX(2 * x + 2, xMiddle, xRight, i, j, addition);
        addY(x, 0, xLeft, xRight, 0, n, i, j, addition);
    }

}

int main() {
    ios_base::sync_with_stdio(false);

    bool end = false;
    while (!end) {
        int commandType;
        cin >> commandType;

        switch (commandType) {
            case 0: {
                cin >> n;
                tree.resize(4 * n, vector<int>(4 * n, 0));
                buildX(0, 0, n);
                break;
            }
            case 1: {
                int x, y, a;
                cin >> x >> y >> a;
                addX(0, 0, n, x, y, a);
                break;
            }
            case 2: {
                int xLeftBorder, xRightBorder, yLeftBorder, yRightBorder;
                cin >> xLeftBorder >> yLeftBorder >> xRightBorder >> yRightBorder;
                xRightBorder++;
                yRightBorder++;

                long long ans = sumX(0, 0, n, xLeftBorder, xRightBorder, yLeftBorder, yRightBorder);
                cout << ans << "\n";
                cout.flush();
                break;
            }
            case 3: {
                end = true;
                break;
            }
            default:
                break;
        }
    }
    return 0;
}