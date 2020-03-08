/*
    Boost Competency Test - GSoC 2020
    digu_J - Digvijay Janartha
    NIT Hamirpur - INDIA
*/

#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>
#include <random>
#include <chrono>

#include <boost/geometry/geometry.hpp>

#include "../../includes/concave_hull_k_nearest_neighbours_optimized.hpp"
#include "../../includes/concave_hull_k_nearest_neighbours.hpp"

namespace bg = boost::geometry;
using bg::dsv;

std::mt19937_64 rang(std::chrono::high_resolution_clock::now().time_since_epoch().count());

int my_rand(int l, int r)
{
    if (l > r)
    {
        std::swap(l, r);
    }
	std::uniform_int_distribution <int> uid(l, r);
	return uid(rang);
}

int main()
{
    std::cout << std::fixed << std::setprecision(0);
    #ifdef HOME
        freopen("input.txt", "r", stdin);
        freopen("output.txt", "w", stdout);
    #endif
    typedef bg::model::point<double, 2, bg::cs::cartesian> point_t;
    typedef bg::model::multi_point<point_t> mpoint_t;

    mpoint_t mpt1, hull1, hull2;

    int points = 200, k = 3;
    for (int i = 0; i < points; ++i)
    {
        int x = my_rand(0, 1000);
        int y = my_rand(0, 1000);
        bg::append(mpt1, point_t(x, y));
    }

    algo2::ConcaveHullKNN(mpt1, hull1, k);
    int t1 = (int)(clock() * 1000. / CLOCKS_PER_SEC);

    algo3::ConcaveHullKNN(mpt1, hull2, k);
    int t2 = (int)(clock() * 1000. / CLOCKS_PER_SEC) - t1;

    std::cout << "Dataset: " << dsv(mpt1) << std::endl;
    std::cout << "Convex hull: " << dsv(hull1) << std::endl;

    std:: cout << t1 << " " << t2 << std::endl;
    return 0;
}

/*
Output is very big, I made this file to check performance (execution time) using clock()
*/
