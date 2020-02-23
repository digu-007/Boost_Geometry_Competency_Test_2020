/*
    Boost Competency Test - GSoC 2020
    digu_J - Digvijay Janartha
    NIT Hamirpur - INDIA
*/

#include <bits/stdc++.h>
#include <boost/geometry.hpp>

using namespace std;
namespace bg = boost::geometry;

typedef bg::model::point<double, 2, bg::cs::cartesian> point_t;
typedef bg::model::polygon<point_t> poly_t;

const double EPS = 1e-9; // used in various places to solve precision issues

// This program takes input integer n and k, followed by n points and outputs concave hull using KNN algorithm.
// The concave hull of a geometry represents a possibly concave geometry that encloses all geometries within the set.

struct pnt{
    double x, y;

    bool operator<(pnt const& oth) {
        if (x == oth.x) {
            return y < oth.y;
        }
        return x < oth.x;
    }

    bool operator==(const pnt& a) const {return a.x == x && a.y == y;}
};

struct line{
    double a, b, c;
};

double dist(pnt a, pnt b) {
    // returns distance square between two points
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

int cross_product(pnt a, pnt b, pnt c) {
    // returns 1 if points are oriented in counter-clockwise manner
    // returns 0 if points are oriented in clockwise manner
    double ans = (b.y - a.y) * (c.x - b.x) - (b.x - a.x) * (c.y - b.y);
    if (!ans) {
        // returns -1 if collinear
        return -1;
    }
    return (ans < 0 ? 1 : 0);
}

void CleanList(vector<pnt> &pt) {
    // this function removes duplicates from input vector
    // and then sorts the vector with increasing x value
    int n = pt.size();
    set<pair<double,double>> st;
    for (int i = 0; i < n; ++i) {
        st.insert({pt[i].x, pt[i].y});
    }
    for (int i = 0; i < n; ++i) {
        pt.pop_back();
    }
    for (auto &p: st) {
        pt.push_back({p.first, p.second});
    }
    sort(pt.begin(), pt.end());
}

void RemovePoint(vector<pnt> &pt, pnt cur) {
    // removes the cur point from vector pt
    int n = pt.size();
    bool active = false;
    for (int i = 0; i < n; ++i) {
        if (pt[i].x == cur.x and pt[i].y == cur.y) {
            for (int j = i; j < n - 1; ++j) {
                pt[j] = pt[j + 1];
            }
            break;
        }
    }
    pt.pop_back();
}

vector<pnt> NearestPoints(vector<pnt> pt, pnt cur, int cur_k) {
    // this function returns vector with cur_k nearest points to point cur
    sort(pt.begin(), pt.end(), [&](pnt a, pnt b) {
        return (dist(a, cur) < dist(b, cur));
    });
    vector<pnt> ans;
    for (int i = 0; i < min(cur_k, int(pt.size())); ++i) {
        ans.push_back(pt[i]);
    }
    sort(ans.begin(), ans.end());
    return ans;
}

bool IsInside(vector<pnt> pt, pnt cur) {
    // returns true if cur is inside or in the border of the polygon formed by points in vector pt
    poly_t poly;
    for (int i = 0; i < int(pt.size()); ++i) {
        bg::append(poly, point_t(pt[i].x, pt[i].y));
    }
    point_t now = point_t(cur.x, cur.y);
    return bg::covered_by(now, poly);
}

bool PointOnSegment(pnt a, pnt b, pnt p) {
    // returns true if point p lies on the line segment formed by point a and b (exclusive)
    if (p == a or p == b) {
        return false;
    }
    double len = sqrt(dist(a, b));
    double tot = sqrt(dist(a, p)) + sqrt(dist(b, p));
    return (len + EPS > tot);
}

bool Intersect(pnt a, pnt b, pnt c, pnt d) {
    // returns true if line formed by point a and b intersects line formed by point c and d
    // if segments only touches each other than return false
    line l1 = {a.y - b.y, b.x - a.x, a.x * (b.y - a.y) + a.y * (a.x - b.x)};
    line l2 = {c.y - d.y, d.x - c.x, c.x * (d.y - c.y) + c.y * (c.x - d.x)};
    double D = l1.a * l2.b - l1.b * l2.a;
    double Ny = l1.c * l2.a - l1.a * l2.c;
    double Nx = l1.b * l2.c - l1.c * l2.b;
    if (abs(D) < EPS) {
        if (abs(Nx) < EPS and abs(Ny) < EPS) {
            double Sum = sqrt(dist(a, b)) + sqrt(dist(c, d));
            double Max = sqrt(max({dist(a, c), dist(a, d), dist(b, c), dist(b, d)}));
            if (Sum > Max) {
                return true;
            }
            return false;
        }
        return false;
    }
    pnt Mid = {Nx / D, Ny / D};
    if (PointOnSegment(a, b, Mid) and PointOnSegment(c, d, Mid)) {
        return true;
    }
    return false;
}

vector<pnt> KNN(vector<pnt> input, int k) {
    vector<pnt> pt = input;

    // k must be atleast 2
    k = max(k, 2);
    if (pt.size() < 3 or k > int(pt.size())) {
        return {};
    }
    if (pt.size() == 3) {
        return pt;
    }

    int cur_k = min(k, int(pt.size()) - 1);
    vector<pnt> hull = {pt[0]};
    pnt cur_pt = pt[0];
    int step = 0;
    RemovePoint(pt, cur_pt);

    while (pt.size()) {
        if (step == 3) {
            // once a polygon with more than 3 edges is formed add the first point to the pt vector again
            pt.push_back(hull[0]);
        }
        vector<pnt> nearest = NearestPoints(pt, cur_pt, cur_k);
        vector<pnt> good;
        for (int i = 0; i < int(nearest.size()); ++i) {
            bool ok = true;
            for (int j = int(hull.size()) - 1; j > 0; --j) {
                if (Intersect(hull[j], hull[j - 1], nearest[i], cur_pt)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                // non intersecting edge is good
                good.push_back(nearest[i]);
            }
        }
        if (good.empty()) {
            // if no good points remaining, increase k
            return KNN(input, k + 1);
        }

        pnt best = good[0];
        for (int i = 0; i < int(good.size()); ++i) {
            int val = cross_product(cur_pt, good[i], best);
            if (val) {
                // update best if we have counter-clockwise orientation
                if (val == -1) {
                    // in case of collinear points take the nearest one as best
                    if (dist(good[i], cur_pt) < dist(best, cur_pt)) {
                        best = good[i];
                    }
                } else {
                    best = good[i];
                }
            }
        }
        cur_pt = best;
        hull.push_back(cur_pt);
        RemovePoint(pt, cur_pt);

        if (cur_pt == hull[0]) {
            // cycle completed
            break;
        }
        cur_k = min(cur_k, int(pt.size()));
        ++step;
    }
    for (auto &p: pt) {
        if (!IsInside(hull, p)) {
            // if any point is outside the concave hull, increase k
            return KNN(input, k + 1);
        }
    }
    cout << "k => " << k << "\n";
    return hull;
}

int main() {
    cout << fixed << setprecision(0);
    // take user input
    int n, k;
    cin >> n >> k;
    vector<pnt> pt;
    for (int i = 0; i < n; ++i) {
        double a, b;
        cin >> a >> b;
        pt.push_back({a, b});
    }

    CleanList(pt);
    vector<pnt> hull = KNN(pt, k);

    if (hull.empty()) {
        cout << "Invalid input\n";
    } else {
        cout << "Hull size: " << int(hull.size()) << "\n";
        for (int i = 0; i < int(hull.size()); ++i) {
            cout << hull[i].x << " " << hull[i].y << "\n";
        }
    }

    // To calculate the time taken in executing the code
    cout << "Time: " << (int)(clock() * 1000. / CLOCKS_PER_SEC) << "ms";
    return 0;
}
