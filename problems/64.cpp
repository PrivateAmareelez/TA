#include <vector>
#include <assert.h>

using namespace std;

#ifdef ONLINE_JUDGE

#include <fstream>

ifstream cin("littera.in");
ofstream cout("littera.out");
#else

#include <iostream>
#include <fstream>

#endif

vector<int> zFunction(string &s) {
    vector<int> z(s.length());
    int left = 0, right = 0;
    for (int i = 1; i < s.length(); i++) {
        z[i] = max(0, min(right - i, z[i - left]));

        while (i + z[i] < s.length() && s[z[i]] == s[i + z[i]])
            z[i]++;

        if (i + z[i] >= right) {
            left = i;
            right = i + z[i];
        }
    }

    return z;
}

string lcs(string s, int keyLength) {
    s += s;
    vector<int> f(s.length(), -1);
    int k = 0;
    for (int i = 1; i < s.length(); i++) {
        char elem = s[i];
        int j = f[i - k - 1];
        while (j != -1 && elem != s[k + j + 1]) {
            if (elem < s[k + j + 1])
                k = i - j - 1;
            j = f[j];
        }
        if (elem != s[k + j + 1]) {
            if (elem < s[k])
                k = i;
            f[i - k] = -1;
        } else
            f[i - k] = j + 1;
    }

    return s.substr(k, keyLength);
}

string encrypt(string raw, string key, int start = -1) {
    int index = (start == -1 ? rand() % key.length() : start);
    vector<char> encrypted(raw.length());
    for (int i = 0; i < raw.length(); i++) {
        int code = (int) raw[i] + (int) key[index++] - 2 * (int) 'a' + 2;
        if (code > 26)
            code -= 26;
        encrypted[i] = (char) (code - 1 + (int) 'a');

        if (index == key.length())
            index = 0;
    }

    return string(encrypted.begin(), encrypted.end());
}

vector<string> generateTest(int maxRaw, int maxKey) {
    assert(maxRaw >= maxKey);

    vector<char> alphabet;
    for (char i = 'a'; i <= 'z'; i++)
        alphabet.push_back(i);

    vector<char> key(1 + (maxKey <= 1 ? 0 : rand() % (maxKey - 1)));
    for (char &i : key)
        i = alphabet[rand() % alphabet.size()];
    string keyStr(key.begin(), key.end());
    //keyStr = lcs(keyStr, keyStr.length());

    vector<char> raw(keyStr.length() + (maxRaw == maxKey ? 0 : rand() % (maxRaw - maxKey)));
    for (char &i : raw)
        i = alphabet[rand() % alphabet.size()];
    string rawStr(raw.begin(), raw.end());

    string encrypted = encrypt(rawStr, keyStr);

    return {rawStr, keyStr, encrypted};
}

void writeTest(vector<string> &test) {
    ofstream fout("littera.in");
    fout << test[0].length() << "\n";
    fout << test[0] << "\n";
    fout << test[2] << "\n";
    fout.close();
}

bool check(string key, string raw, string encrypted) {
    for (int i = 0; i < key.length(); i++) {
        string enc = encrypt(raw, key, i);
        if (enc == encrypted)
            return true;
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
//    srand(17);

//    while (true) {

//        vector<string> test = generateTest(10, 10);
//        writeTest(test);
//        ifstream fin("littera.in");

    int n;
//        fin >> n;
    cin >> n;
    vector<char> raw(n), encrypted(n);
    int index = 0;
//        while (!fin.eof()) {
    while (!cin.eof()) {
        string s;
//            getline(fin, s);
        getline(cin, s);
        if (index + s.length() <= n) {
            for (char i : s)
                raw[index++] = (char) tolower(i);
        } else {
            for (char i : s)
                encrypted[index++ - n] = (char) tolower(i);
        }
    }
//        fin.close();

    vector<char> diff(n);
    int len = 0;
    for (int i = 0; i < n; i++) {
        if (isalpha(raw[i]) == 0)
            continue;

        int d = encrypted[i] - raw[i] - 1;
        if (d >= 0)
            diff[len++] = (char) (d + 'a');
        else
            diff[len++] = (char) (d + 26 + 'a');
    }
    diff.resize(len);

    string longKey(diff.begin(), diff.end());
    vector<int> z = zFunction(longKey);
    z[0] = longKey.length();

    int keyLength = longKey.length();
    int prevFound = -1, found = -1;
    for (int i = z.size() - 1; i >= 0; i--)
        if (z[i] != 0 && z[i] == z.size() - i) {
            prevFound = found;
            found = z[i];
        }
    if (found != -1 && prevFound != -1)
        keyLength = found - prevFound;
    else
        keyLength = found;
    string key = longKey.substr(0, keyLength);

    string minimalKey = lcs(key, keyLength);
    cout << minimalKey << "\n";
//
//        string ans = test[1];
//        assert(check(ans, test[0], test[2]));
//        assert(check(minimalKey, test[0], test[2]));
//        assert(minimalKey == ans || (minimalKey != ans && check(minimalKey, test[0], test[2])));
//    }
    return 0;
}