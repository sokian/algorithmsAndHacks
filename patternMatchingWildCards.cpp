#include <bits/stdc++.h>
using namespace std;

inline int nxt() {
    int a;
    scanf("%d", &a);
    return a;
}


struct Comp {
    double x, y;
    Comp(double x = 0, double y = 0) : x(x), y(y) {}
    Comp operator + (const Comp &r) const {
        return Comp(x + r.x, y + r.y);
    }
    Comp operator - (const Comp &r) const {
        return Comp(x - r.x, y - r.y);
    }
    Comp operator * (const Comp &r) const {
        return Comp(x * r.x - y * r.y, x * r.y + y * r.x);
    }
    Comp & operator /= (const double &r) {
        x /= r;
        y /= r;
        return *this;
    }
};

const double PI = acos(-1.0);

void fft(Comp *a, int n, bool inv) {
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        while (j >= bit) {
            j -= bit;
            bit >>= 1;
        }
        j += bit;
        if (i < j) swap(a[i], a[j]);
    }

    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (inv ? -1 : 1);
        Comp wlen(cos(ang), sin(ang));
        int len2 = len >> 1;
        for (int i = 0; i < n; i += len) {
            Comp w(1);
            for (int j = 0; j < len2; ++j) {
                Comp u = a[i + j];
                Comp v = a[i + j + len2] * w;
                a[i + j] = u + v;
                a[i + j + len2] = u - v;
                w = w * wlen;
            }
        }
    }
    if (inv) {
        for (int i = 0; i < n; ++i) {
            a[i] /= n;
        }
    }
}

inline void solve() {
    string s, t;
    cin >> s >> t;
    int n = s.length();
    int m = t.length();

    int sz = 1;
    while (sz < max(n, m)) sz <<= 1;
    sz <<= 1;

    Comp a[sz];
    Comp b[sz];
    long long ans[sz];
    memset(ans, 0, sizeof(ans));

    memset(a, 0, sizeof(a));
    memset(b, 0, sizeof(b));
    for (int i = 0; i < n; ++i) {
        if (s[i] == '?') {
            a[i] = 0;
        } else {
            int x = s[i] - 'a' + 1;
            a[i] = x * x * x;
        }
    }
    for (int i = 0; i < m; ++i) {
        if (t[i] == '?') {
            b[m - 1 - i] = 0;
        } else {
            int x = t[i] - 'a' + 1;
            b[m - 1 - i] = x;
        }
    }

    fft(a, sz, false);
    fft(b, sz, false);
    for (int i = 0; i < sz; ++i) {
        a[i] = a[i] * b[i];
    }
    fft(a, sz, true);

    for (int i = 0; i < sz; ++i) {
        ans[i] += (long long)(a[i].x + 0.5);
    }

    memset(a, 0, sizeof(a));
    memset(b, 0, sizeof(b));
    for (int i = 0; i < n; ++i) {
        if (s[i] == '?') {
            a[i] = 0;
        } else {
            int x = s[i] - 'a' + 1;
            a[i] = x;
        }
    }
    for (int i = 0; i < m; ++i) {
        if (t[i] == '?') {
            b[m - 1 - i] = 0;
        } else {
            int x = t[i] - 'a' + 1;
            b[m - 1 - i] = x * x * x;
        }
    }

    fft(a, sz, false);
    fft(b, sz, false);
    for (int i = 0; i < sz; ++i) {
        a[i] = a[i] * b[i];
    }
    fft(a, sz, true);

    for (int i = 0; i < sz; ++i) {
        ans[i] += (long long)(a[i].x + 0.5);
    }


    memset(a, 0, sizeof(a));
    memset(b, 0, sizeof(b));
    for (int i = 0; i < n; ++i) {
        if (s[i] == '?') {
            a[i] = 0;
        } else {
            int x = s[i] - 'a' + 1;
            a[i] = x * x;
        }
    }
    for (int i = 0; i < m; ++i) {
        if (t[i] == '?') {
            b[m - 1 - i] = 0;
        } else {
            int x = t[i] - 'a' + 1;
            b[m - 1 - i] = x * x;
        }
    }

    fft(a, sz, false);
    fft(b, sz, false);
    for (int i = 0; i < sz; ++i) {
        a[i] = a[i] * b[i];
    }
    fft(a, sz, true);

    for (int i = 0; i < sz; ++i) {
        long long val = (long long)(a[i].x + 0.5);
        ans[i] -= 2 * val;
    }

    vector<int> pos;
    for (int i = 0; i < n - m + 1; ++i) {
        if (ans[i + m - 1] == 0) {
            pos.push_back(i);
        }
    }

    printf("%d\n", (int)pos.size());
    if (!pos.empty()) {
        for (int i = 0; i < (int)pos.size(); ++i) {
            if (i) printf(" ");
            printf("%d", pos[i] + 1);
        }
        printf("\n");
    }
}

int main() {
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
#endif
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    solve();

    return 0;
}

