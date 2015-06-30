#include <bits/stdc++.h>
 
#define clr(x) memset((x), 0, sizeof((x)))
#define all(x) (x).begin(), (x).end()
#define pb push_back
#define mp make_pair
#define x first
#define y second
 
using namespace std;
typedef long long ll;
typedef long double ld;
typedef pair<int, int> PII;
typedef pair<ll, ll> PLL;
 
inline int nxt() {
    int a;
    scanf("%d", &a);
    return a;
}
 
const int N = 300000;
int n, m;
 
namespace Planarity {
 
    int edge[N][2];
    bool oriented[N];
    vector<int> G[N];
    int height[N];
    int lowpt[N];
    int lowpt2[N];
    int nesting_depth[N];
    int parent_edge[N];
    const int INF = INT_MAX >> 2;
 
 
    void DFS1(int v) {
        int e = parent_edge[v];
        for (int edgeId : G[v]) {
            if (oriented[edgeId]) continue;
            if (edge[edgeId][1] == v) {
                swap(edge[edgeId][0], edge[edgeId][1]);
            }
            oriented[edgeId] = true;
            lowpt[edgeId] = lowpt2[edgeId] = height[v];
 
            int to = edge[edgeId][1];
            if (height[to] == INF) {
                parent_edge[to] = edgeId;
                height[to] = height[v] + 1;
                DFS1(to);
            } else {
                lowpt[edgeId] = height[to];
            }
 
            /**
             * determine nesting depth
             */
            nesting_depth[edgeId] = 2 * lowpt[edgeId];
            if (lowpt2[edgeId] < height[v]) {
                /* chordal */
                nesting_depth[edgeId]++;
            }
 
            /**
             * update lowpoints of parent edge e
             */
            if (e != -1) {
                if (lowpt[edgeId] < lowpt[e]) {
                    lowpt2[e] = min(lowpt[e], lowpt2[edgeId]);
                    lowpt[e]  = lowpt[edgeId];
                }
                else if (lowpt[edgeId] > lowpt[e]) {
                    lowpt2[e] = min(lowpt2[e], lowpt[edgeId]);
                }
                else {
                    lowpt2[e] = min(lowpt2[e], lowpt2[edgeId]);
                }
            }
        }
    }
 
 
    int ref[N];
    int side[N];
    class I {
    public:
        int low, high;
        I(int low = -1, int high = -1) : low(low), high(high) {}
        I(const I &i) : low(i.low), high(i.high) {}
        bool empty() const {
            return low == -1;
        }
        bool operator == (const I &i) const {
            return low == i.low && high == i.high;
        }
    };
    class P {
    public:
        I L;
        I R;
        P(){}
        P(const I &l, const I &r) : L(l), R(r) {}
        bool operator == (const P &p) const {
            return L == p.L && R == p.R;
        }
    };
    int lowpt_edge[N];
    P stack_bottom[N];
    vector<P> S;
 
 
    inline bool conflicting(I i, int edgeId) {
        return (!i.empty() && lowpt[i.high] > lowpt[edgeId]);
    }
 
    int addConstraints(int edgeId, int e) {
        P p = P(I(), I());
        /**
         * merge return edges of edgeId into p.R
         */
        do {
            P q = (S.empty() ? P(I(), I()) : S.back());
            if (!S.empty()) S.pop_back();
            if (!q.L.empty()) swap(q.L, q.R);
            if (!q.L.empty()) {
                /* not planar */
                return 0;
            }
            if (lowpt[q.R.low] > lowpt[e]) {
                /* merge intervals */
                if (p.R.empty()) p.R.high     = q.R.high;
                else             ref[p.R.low] = q.R.high;
 
                p.R.low = q.R.low;
            } else {
                /* make consistent */
                ref[q.R.low] = lowpt_edge[e];
            }
        } while (!((S.empty() ? P(I(), I()) : S.back()) == stack_bottom[edgeId]));
        /**
         * merge conflicting return edges of e1, . . . , eiâ��1 into p.L
         */
        P qw = (S.empty() ? P(I(), I()) : S.back());
        while (conflicting((S.empty() ? P(I(), I()) : S.back()).L, edgeId) ||
               conflicting((S.empty() ? P(I(), I()) : S.back()).R, edgeId)) {
            P q = (S.empty() ? P(I(), I()) : S.back());
            if (!S.empty()) S.pop_back();
            if (conflicting(q.R, edgeId)) swap(q.L, q.R);
            if (conflicting(q.R, edgeId)) {
                /* not planar */
                return 0;
            }
            /* merge interval below lowpt(edgeId) into p.R */
            ref[p.R.low] = q.R.high;
            if (q.R.low >= 0) p.R.low = q.R.low;
 
            if (p.L.empty()) {
                p.L.high = q.L.high;
            } else {
                ref[p.L.low] = q.L.high;
            }
            p.L.low = q.L.low;
        }
        if (!p.L.empty() || !p.R.empty()) {
            S.push_back(p);
        }
        return 1;
    }
 
    inline int lowest(P p) {
        if (p.L.empty()) return lowpt[p.R.low];
        if (p.R.empty()) return lowpt[p.L.low];
        return std::min(lowpt[p.L.low], lowpt[p.R.low]);
    }
 
    void trimBackEdges(int u) {
        while (!S.empty() && lowest(S.back()) == height[u]) {
            P p = S.back();
            S.pop_back();
            if (p.L.low >= 0) side[p.L.low] = -1;
        }
        if (!S.empty()) {
            P p = S.back();
            S.pop_back();
            /*
             * trim left interval
             */
            while (p.L.high != -1 && edge[p.L.high][1] == u) {
                p.L.high = ref[p.L.high];
            }
            if (p.L.high == -1 && p.L.low != -1) {
                ref[p.L.low] = p.R.low;
                side[p.L.low] = -1;
                p.L.low = -1;
            }
 
            /*
             * trim right interval
             */
            while (p.R.high != -1 && edge[p.R.high][1] == u) {
                p.R.high = ref[p.R.high];
            }
            if (p.R.high == -1 && p.R.low != -1) {
                ref[p.R.low] = p.L.low;
                side[p.R.low] = -1;
                p.R.low = -1;
            }
            S.push_back(p);
        }
    }
 
    int DFS2(int v) {
        int e = parent_edge[v];
        int E1 = -1;
        for (int edgeId : G[v]) {
            if (edge[edgeId][0] != v) continue;
            if (E1 == -1) {E1 = edgeId;}
            stack_bottom[edgeId] = (S.empty() ? P(I(), I()) : S.back());
            int to = edge[edgeId][1];
            if (edgeId == parent_edge[to]) {
                int res = DFS2(to);
                if (res == 0) {
                    return 0;
                }
            } else {
                lowpt_edge[edgeId] = edgeId;
                S.push_back(P(I(), I(edgeId, edgeId)));
            }
            if (lowpt[edgeId] < height[v]) {
                if (edgeId == E1) {
                    lowpt_edge[e] = lowpt_edge[edgeId];
                } else {
                    int res = addConstraints(edgeId, e);
                    if (res == 0) {
                        return 0;
                    }
                }
            }
        }
 
        /* v is not root */
        if (e != -1) {
            int u = edge[e][0];
            trimBackEdges(u);
            /**
             * side of e is side of a highest return edge
             */
            /* e has return edge */
            if (lowpt[e] < height[u]) {
                P back = (S.empty() ? P(I(), I()) : S.back());
                int hL = back.L.high;
                int hR = back.R.high;
                if (hL != -1 && (hR == -1 || lowpt[hL] > lowpt[hR])) {
                    ref[e] = hL;
                } else {
                    ref[e] = hR;
                }
            }
        }
        return 1;
    }
 
    int checkPlanarity() {
        /**
        * orientation
        */
        vector<int> roots;
        for (int i = 0; i < n; ++i) {
            height[i] = INF;
        }
        for (int i = 0; i < m; ++i) {
            oriented[i] = false;
        }
        for (int i = 0; i < n; ++i) {
            if (height[i] == INF) {
                height[i] = 0;
                parent_edge[i] = -1;
                roots.push_back(i);
                DFS1(i);
            }
        }
 
        /**
        * testing
        */
        // sort adjacency lists according to non-decreasing nesting depth
        for (int i = 0; i < n; ++i) {
            stable_sort(G[i].begin(), G[i].end(), [&](int edgeLeft, int edgeRight) {
                return nesting_depth[edgeLeft] < nesting_depth[edgeRight];
            });
        }
        for (int i = 0; i < m; ++i) {
            ref[i] = -1;
            side[i] = 1;
        }
        S.clear();
        for (int root : roots) {
            int res = DFS2(root);
            if (res == 0) {
                return 0;
            }
        }
        return 1;
    }
};
 
int solve() {
    n = nxt();
    m = nxt();
    for (int i = 0; i < n; ++i) {
        Planarity::G[i].clear();
    }
    for (int i = 0; i < m; ++i) {
        int a = nxt() - 1;
        int b = nxt() - 1;
        Planarity::edge[i][0] = a;
        Planarity::edge[i][1] = b;
        Planarity::G[a].push_back(i);
        Planarity::G[b].push_back(i);
    }
 
    if (n == 1) {
        return 1;
    }
    if (n == 2) {
        return m == 1;
    }
 
    if (m != 3 * (n - 2)) {
        return 0;
    }
    return Planarity::checkPlanarity();
}
 
 
int main() {
#ifdef LOCAL
    freopen("input.txt", "r", stdin);
    //freopen("output.txt", "w", stdout);
//#else
//    #define file "i"
//    freopen(file".in", "r", stdin);
//    freopen(file".out", "w", stdout);
#endif
    ios_base::sync_with_stdio(false);
    cin.tie(0);
 
 
    int t = nxt();
    for (int i = 0; i < t; ++i) {
        int res = solve();
        cout << res << "\n";
    }
    return 0;
} 
