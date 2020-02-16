/*
    Boost Competency Test - GSoC 2020
    digu_J - Digvijay Janartha
    NIT Hamirpur - INDIA
*/

#include <bits/stdc++.h>
using namespace std;

// Program to find convex hull of n points using jarvis march or gift wrapping algorithm.
// Convex hull of a shape is the smallest convex set that contains it.

struct pnt{
    double x, y;
};

bool cross_product(pnt a, pnt b, pnt c) {
    // return true if points are oriented in counter-clockwise manner
    return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x) < 0);
}

vector<pnt> wrapping_jarvis(vector<pnt> pt) {
    // computes convex hull in O(n * h)
    // where, n is the total input points and h is the total points in convex hull
    vector<pnt> hull;
    int leftmost = 0, n = pt.size();
    for (int i = 1; i < n; ++i) {
        if (pt[i].x < pt[leftmost].x) {
            leftmost = i;
        }
    }
    int p = leftmost, q = 0;
    while (true) {
        hull.push_back(pt[p]);
        q = (p + 1) % n;
        for (int i = 0; i < n; ++i) {
            if (cross_product(pt[p], pt[i], pt[q])) {
                q = i;
            }
        }
        if (q == leftmost) {
            break;
        }
        p = q;
    }
    return hull;
}

int main() {
    cout << fixed << setprecision(2);
    int n;
    cin >> n;
    assert(n > 2); // at least three points are required for convex hull
    vector<pnt> pt;
    for (int i = 0; i < n; ++i) {
        double a, b;
        cin >> a >> b;
        pt.push_back({a, b});
    }
    vector<pnt> hull = wrapping_jarvis(pt);
    for (int i = 0; i < int(hull.size()); ++i) {
        cout << hull[i].x << " " << hull[i].y << "\n";
    }
    return 0;
}
