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

#include "../includes/concave_hull_k_nearest_neighbours_optimized.hpp"

namespace bg = boost::geometry;
using bg::dsv;

typedef bg::model::point<double, 2, bg::cs::cartesian> point_t;
typedef bg::model::multi_point<point_t> mpoint_t;
typedef bg::model::polygon<point_t> polygon_t;
typedef bg::model::segment<point_t> segment_t;

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

bool test(mpoint_t &input, mpoint_t &output)
{
    // First test is to check if all the points must lie inside or on the concave hull.
    polygon_t p;
    for (int i = 0; i < boost::size(output); ++i)
    {
        bg::append(p, output[i]);
    }
    for (int i = 0; i < boost::size(input); ++i)
    {
        if (!(bg::covered_by(input[i], p)))
        {
            std::cout << dsv(input[i]) << "\n";
            return false;
        }
    }

    // Second test is to check if no two edges of the concave hull intersects each other.
    for (int i = 0; i < int(boost::size(output)) - 1; ++i)
    {
        segment_t cur(output[i], output[i + 1]);
        for (int j = 0; j < int(boost::size(output)) - 1; ++j)
        {
            if (bg::equals(output[i], output[j]) or bg::equals(output[i], output[j + 1]))
            {
                continue;
            }
            if (bg::equals(output[i + 1], output[j]) or bg::equals(output[i + 1], output[j + 1]))
            {
                continue;
            }
            segment_t check(output[j], output[j + 1]);
            if (bg::intersects(check, cur))
            {
                std::cout << dsv(check) << "\n";
                std::cout << dsv(cur) << "\n";
                return false;
            }
        }
    }
    return true;
}

int main()
{
    std::cout << std::fixed << std::setprecision(0);
    #ifdef HOME
        freopen("input.txt", "r", stdin);
        freopen("output.txt", "w", stdout);
    #endif

    for (int tt = 1; tt <= 50; ++tt) // 50 random test cases
    {
        mpoint_t mpt1, hull1, hull2;

        int points = 30, k = 3;
        for (int i = 0; i < points; ++i)
        {
            int x = my_rand(0, 100);
            int y = my_rand(0, 100);
            bg::append(mpt1, point_t(x, y));
        }

        algo3::ConcaveHullKNN(mpt1, hull2, k);
        assert(test(mpt1, hull2) == true);
    }

    return 0;
}
