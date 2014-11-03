#include <bits/stdc++.h>

using namespace std;

const int LEN = 1 << 21;
string s[6];
long long initialData[6][LEN];
long long zetaTransform[6][LEN];
long long ret[LEN];

void tf(long long X[], int len, int sign)
{
    for (int bit = 1; bit < len; bit <<= 1) {
        int mask = len - bit - 1;
        for (int i = mask; ; i = (i - 1) & mask) {
            X[i | bit] += sign * X[i];
            if (i == 0) {
                break;
            }
        }
    }
}

int main() {
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
#else
    //string filename = "border";
    //freopen((filename + ".in").c_str(), "r", stdin);
    //freopen((filename + ".out").c_str(), "w", stdout);
    //freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
#endif

    int n, m, k;
    cin >> n >> m >> k;
    for (int i = 0; i < m; ++i) {
        cin >> s[i];
    }
    int len = s[0].length();

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < len; ++j) {
            initialData[i][j] = s[i][j] - '0';
            zetaTransform[i][j] = s[i][j] - '0';
        }
    }

    for (int i = 0; i < m; ++i) {
        tf(zetaTransform[i], len, 1);
    }

//    for (int i = 0; i < m; ++i) {
//        for (int j = 0; j < (int)s[i].length(); ++j) {
//            cout << zetaTransform[i][j] << " ";
//        }
//        cout << endl;
//    }
//    cout << endl;

    for (int i = 0; i < k; ++i) {
        int x, y;
        cin >> x >> y;
        for (int j = 0; j < len; ++j) {
            ret[j] = zetaTransform[x][j] * zetaTransform[y][j];
        }
        tf(ret, len, -1);
//        for (int j = 0; j < len; ++j) {
//            if (ret[j] != 0) {
//                cout << "1";
//            } else {
//                cout << "0";
//            }
//        }
//        cout << endl;

        for (int j = 0; j < len; ++j) {
            if (j) cout << " ";
            cout << ret[j];
        }
        cout << endl;

        long long ans[len];
        memset(ans, 0, sizeof(ans));
        for (int j = 0; j < len; ++j) {
            for (int k = 0; k < len; ++k) {
                if (s[x][j] == '1' && s[y][k] == '1') {
                    ans[j | k] += 1;
                }
            }
        }
        for (int j = 0; j < len; ++j) {
            cout << ans[j] << " ";
        }
        cout << endl;
    }
    return 0;
}
