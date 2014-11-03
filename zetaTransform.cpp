/**
  Problem D from Opencup GP Siberia 2014.
  Uses ZetaTransform of function f : 2^n -> R

  http://cryptoragbag.wordpress.com/2011/04/23/yates-algorithm-for-the-zeta-transform/
  http://apps.topcoder.com/wiki/display/tc/TCO+2012+Round+2A   problem EvenPaths
  problem D from Opencup GB Spb 2014

   The Zeta Transform is defined as f~(Y) = sum_{X \subset Y} f(X) for all Y \subset N, where N = {2^n}
   Let g_i(Y) = sum_{Y \ {1..i} \subset X \subset Y} f(Y)
   describing partial sums over all subsets X \subset Y covering all elements in Y \ {1,..,i}.
   g_0(Y) = f(Y)
   Then g_i(Y) = if (i in Y) g_{i-1}(Y) + g_{i-1}(Y / {i}) else g_{i-1}(Y)
   f~(Y) = g_n(Y)

   For inverse transform g_i(Y) = if (i in Y) g_{i-1}(Y) - g_{i-1}(Y / {i}) else g_{i-1}(Y).
*/

#include <bits/stdc++.h>
using namespace std;

string s[6];
vector<int> initialData[6];
vector<int> zetaTransform[6];


vector<int> calculateZetaTransform(const vector<int> &a, bool inverse = false) {
    vector<int> g[2];
    for (int i = 0; i < 2; ++i) {
        g[i].resize(a.size(), 0);
    }
    g[1] = a;
    int N = a.size();
    int n = 0;
    while ((1 << n) < N) ++n;

    for (int i = 0; i < n; ++i) {
        int t = i & 1;
        int p = t ^ 1;
        for (int j = 0; j < N; ++j) {
            if ((j >> i) & 1) {
                if (inverse) {
                    g[t][j] = g[p][j] - g[p][j ^ (1 << i)];
                } else {
                    g[t][j] = g[p][j] + g[p][j ^ (1 << i)];
                }
            } else {
                g[t][j] = g[p][j];
            }
        }
    }
    return g[(n - 1) & 1];
}

vector<int> solve(const int &l, const int &r) {

    vector<int> ret(s[l].length(), 0);
    for (int i = 0; i < s[l].length(); ++i) {
        ret[i] = zetaTransform[l][i] * zetaTransform[r][i];
    }
    return calculateZetaTransform(ret, true);
}

void printVector(const vector<int> &a) {
    for (int i = 0; i < a.size(); ++i) {
        if (i) cout << " ";
        cout << a[i];
    }
    cout << endl;
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

    for (int i = 0; i < m; ++i) {
        initialData[i].resize(s[i].length());
        for (int j = 0; j < s[i].length(); ++j) {
            initialData[i][j] = s[i][j] - '0';
        }
    }

//    for (int i = 0; i < m; ++i) {
//        printVector(initialData[i]);
//    }
//    cout << endl;

    for (int i = 0; i < m; ++i) {
        zetaTransform[i] = calculateZetaTransform(initialData[i]);
        //printVector(zetaTransform[i]);
    }
    //cout << endl;

    for (int i = 0; i < k; ++i) {
        int x, y;
        cin >> x >> y;
        vector<int> ans = solve(x, y);
        for (int j = 0; j < (int)ans.size(); ++j) {
            if (ans[j] != 0) {
                cout << '1';
            } else {
                cout << '0';
            }
        }
        cout << endl;
    }
    return 0;
}
