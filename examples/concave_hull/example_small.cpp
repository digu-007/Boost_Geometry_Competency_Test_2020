/*
    Boost Competency Test - GSoC 2020
    digu_J - Digvijay Janartha
    NIT Hamirpur - INDIA
*/

#include <algorithm>
#include <iostream>
#include <utility>

#include "../../includes/concave_hull_k_nearest_neighbours_optimized.hpp"

#include <boost/geometry/geometry.hpp>

namespace bg = boost::geometry;
using bg::dsv;

int main()
{
    typedef bg::model::point<double, 2, bg::cs::cartesian> point_t;
    typedef bg::model::multi_point<point_t> mpoint_t;

    mpoint_t mpt1, hull;
    bg::read_wkt("MULTIPOINT(0 0,6 0,3 1,3 2,4 2,5 3,5 5,4 4,-2 2)", mpt1);

    int k = 3;
    algo3::ConcaveHullKNN(mpt1, hull, k);

    std::cout << "Dataset: " << dsv(mpt1) << std::endl;
    std::cout << "Concave hull for k = " << k << ": " << dsv(hull) << std::endl;

    return 0;
}

/*
Output:
Dataset: ((0, 0), (6, 0), (3, 1), (3, 2), (4, 2), (5, 3), (5, 5), (4, 4), (-2, 2))
Concave hull for k = 3: ((-2, 2), (0, 0), (3, 1), (6, 0), (5, 5), (-2, 2))
*/