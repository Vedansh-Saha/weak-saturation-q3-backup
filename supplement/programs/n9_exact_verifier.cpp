#include <bits/stdc++.h>
using namespace std;
using U = uint64_t;

static inline int popc(U x) { return __builtin_popcountll(x); }

int main() {
    const int n = 9;
    int eid[9][9]; memset(eid, -1, sizeof(eid));
    vector<pair<int,int>> edges;
    for (int i = 0, k = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j) {
            eid[i][j] = k++;
            edges.push_back({i,j});
        }

    vector<pair<int,int>> q3;
    for (int v = 0; v < 8; ++v)
        for (int b : {1,2,4}) {
            int w = v ^ b;
            if (v < w) q3.push_back({v,w});
        }

    U core = 0;
    for (auto [a,b] : q3) core |= 1ULL << eid[a][b];

    vector<pair<int,int>> outside;
    int oi[36]; fill(oi, oi + 36, -1);
    for (int e = 0; e < 36; ++e)
        if (!(core >> e & 1ULL)) {
            oi[e] = (int)outside.size();
            outside.push_back(edges[e]);
        }

    vector<pair<int,int>> local;
    int leid[8][8]; memset(leid, -1, sizeof(leid));
    for (int i = 0, k = 0; i < 8; ++i)
        for (int j = i + 1; j < 8; ++j) {
            leid[i][j] = k++;
            local.push_back({i,j});
        }


    unordered_set<U> base;
    base.reserve(1000);
    vector<int> p(8); iota(p.begin(), p.end(), 0);
    do {
        U m = 0;
        for (auto [a,b] : q3) {
            int x = min(p[a], p[b]), y = max(p[a], p[b]);
            m |= 1ULL << leid[x][y];
        }
        base.insert(m);
    } while (next_permutation(p.begin(), p.end()));


    unordered_set<U> cubes;
    cubes.reserve(10000);
    for (int sm = 0; sm < (1 << n); ++sm) if (popc((U)sm) == 8) {
        vector<int> V;
        for (int i = 0; i < n; ++i) if (sm >> i & 1) V.push_back(i);
        for (U bm : base) {
            U gm = 0;
            for (auto [a,b] : local) if (bm >> leid[a][b] & 1ULL) {
                int x = V[a], y = V[b]; if (x > y) swap(x,y);
                gm |= 1ULL << eid[x][y];
            }
            cubes.insert(gm);
        }
    }

    unordered_set<U> outside_masks_set;
    outside_masks_set.reserve(10000);
    for (U cube : cubes) {
        U om = 0;
        for (int i = 0; i < (int)outside.size(); ++i) {
            auto [a,b] = outside[i];
            if (cube >> eid[a][b] & 1ULL) om |= 1ULL << i;
        }
        outside_masks_set.insert(om);
    }
    vector<U> outside_masks(outside_masks_set.begin(), outside_masks_set.end());

    vector<vector<U>> rules(outside.size());
    for (U om : outside_masks) {
        U x = om;
        while (x) {
            U b = x & -x;
            int i = __builtin_ctzll(x);
            rules[i].push_back(om ^ b);
            x ^= b;
        }
    }
    for (auto &r : rules) {
        sort(r.begin(), r.end());
        r.erase(unique(r.begin(), r.end()), r.end());
    }

    const U full = (1ULL << outside.size()) - 1;
    auto closure = [&](U seed) {
        U state = seed;
        while (true) {
            U add = 0;
            for (int e = 0; e < (int)outside.size(); ++e) if (!(state >> e & 1ULL)) {
                for (U pre : rules[e]) {
                    if ((pre & ~state) == 0) {
                        add |= 1ULL << e;
                        break;
                    }
                }
            }
            add &= ~state;
            if (!add) return state;
            state |= add;
        }
    };

    auto mask_seed = [&](initializer_list<pair<int,int>> es) {
        U s = 0;
        for (auto [a,b] : es) {
            if (a > b) swap(a,b);
            int oe = oi[eid[a][b]];
            if (oe < 0) throw runtime_error("seed edge is in the canonical cube");
            s |= 1ULL << oe;
        }
        return s;
    };


    vector<pair<int,int>> initial = {
        {0,2},{0,3},{0,4},{0,5},{0,8},{1,2},{1,3},{1,5},
        {2,3},{2,6},{2,8},{3,7},{4,5},{4,6},{5,7},{6,7}
    };
    vector<pair<int,int>> order = {
        {0,1},{1,4},{4,8},{7,8},{0,6},{1,6},{1,7},{1,8},{2,4},{2,5},
        {2,7},{3,4},{3,5},{3,6},{3,8},{4,7},{5,6},{5,8},{6,8},{0,7}
    };

    U graph = 0;
    for (auto [a,b] : initial) {
        if (a > b) swap(a,b);
        graph |= 1ULL << eid[a][b];
    }

    for (U cube : cubes) if ((cube & ~graph) == 0) {
        cerr << "FAIL: initial graph contains Q3\n";
        return 2;
    }

    for (size_t step = 0; step < order.size(); ++step) {
        auto [a,b] = order[step];
        if (a > b) swap(a,b);
        U ebit = 1ULL << eid[a][b];
        if (graph & ebit) {
            cerr << "FAIL: edge already present at step " << step + 1 << "\n";
            return 2;
        }
        bool legal = false;
        for (U cube : cubes) {
            if (!(cube & ebit)) continue;
            if ((cube & ~(graph | ebit)) == 0) {
                legal = true;
                break;
            }
        }
        if (!legal) {
            cerr << "FAIL: illegal step " << step + 1 << " = " << a << b << "\n";
            return 2;
        }
        graph |= ebit;
    }

    if (popc(graph) != 36) {
        cerr << "FAIL: certificate does not reach K9\n";
        return 2;
    }

    long long count4 = 0;
    int max4 = 0;
    for (int a = 0; a < 24 - 3; ++a)
        for (int b = a + 1; b < 24 - 2; ++b)
            for (int c = b + 1; c < 24 - 1; ++c)
                for (int d = c + 1; d < 24; ++d) {
                    U s = (1ULL<<a)|(1ULL<<b)|(1ULL<<c)|(1ULL<<d);
                    U cstate = closure(s);
                    max4 = max(max4, popc(cstate));
                    if (cstate == full) ++count4;
                }

    long long count5 = 0;
    int max5 = 0;
    for (int a = 0; a < 24 - 4; ++a)
        for (int b = a + 1; b < 24 - 3; ++b)
            for (int c = b + 1; c < 24 - 2; ++c)
                for (int d = c + 1; d < 24 - 1; ++d)
                    for (int e = d + 1; e < 24; ++e) {
                        U s = (1ULL<<a)|(1ULL<<b)|(1ULL<<c)|(1ULL<<d)|(1ULL<<e);
                        U cstate = closure(s);
                        max5 = max(max5, popc(cstate));
                        if (cstate == full) ++count5;
                    }

    cerr << "distinct_Q3_copies=" << cubes.size() << "\n";
    cerr << "distinct_outside_edge_masks=" << outside_masks.size() << "\n";
    cerr << "four_edge_seeds=" << 10626 << " percolating=" << count4
         << " max_closure=" << max4 << "\n";
    cerr << "five_edge_seeds=" << 42504 << " percolating=" << count5
         << " max_closure=" << max5 << "\n";

    if (count4 != 0 || count5 == 0) {
        cerr << "FAIL: expected no percolating 4-seed and at least one percolating 5-seed\n";
        return 2;
    }
    cout << "PASS: wsat(K9,Q3)=16 under the canonical first-edge reduction.\n";
    return 0;
}
