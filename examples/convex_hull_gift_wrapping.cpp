/*
    Boost Competency Test - GSoC 2020
    digu_J - Digvijay Janartha
    NIT Hamirpur - INDIA
*/

#include <algorithm>
#include <iostream>
#include <utility>

#include "../include/convex_hull_gift_wrapping.hpp"

#include <boost/geometry/geometry.hpp>

namespace bg = boost::geometry;
using bg::dsv;

int main()
{
    typedef bg::model::point<double, 2, bg::cs::cartesian> point_t;
    typedef bg::model::multi_point<point_t> mpoint_t;

    mpoint_t mpt1, hull;
    bg::read_wkt("MULTIPOINT(0 0,6 0,2 2,4 2,5 3,5 5,-2 2)", mpt1);

    algo1::GiftWrapping(mpt1, hull);

    std::cout << "Dataset: " << dsv(mpt1) << std::endl;
    std::cout << "Convex hull: " << dsv(hull) << std::endl;

    return 0;
}

/*
Output:
Dataset: ((0, 0), (6, 0), (2, 2), (4, 2), (5, 3), (5, 5), (-2, 2))
Convex hull: ((-2, 2), (0, 0), (6, 0), (5, 5), (-2, 2))
*/