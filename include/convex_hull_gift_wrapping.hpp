/*
    Boost Competency Test - GSoC 2020
    digu_J - Digvijay Janartha
    NIT Hamirpur - INDIA
*/

#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>

#include <boost/geometry/geometry.hpp>

namespace bg = boost::geometry;

// Solves 2D convex hull in O(n * h)
// Where n is the total input points and h is the total points in convex hull
template <typename MultiPoint, typename Size>
class Hull {

    private:
        MultiPoint hull;
        Size n;

    public:
        Hull(MultiPoint mp, Size sz);
        void print_hull();
};

template <typename MultiPoint, typename Size>
Hull<MultiPoint, Size>::Hull(MultiPoint mp, Size sz)
{
    hull = mp;
    n = sz;
}

// To print the convex hull
template <typename MultiPoint, typename Size>
void Hull<MultiPoint, Size>::print_hull()
{
    std::cout << "Resulting points of Convex Hull are: " << std::endl;
    for (Size i = 0; i < n; ++i)
    {
        std::cout << bg::get<0>(hull[i]) << " " << bg::get<1>(hull[i]) << std::endl;
    }
}

// This method returns true if the points are oriented in counter-clockwise manner
template <typename Point>
static inline bool check(Point a, Point b, Point c)
{   
    typedef typename bg::coordinate_type<Point>::type c_type;

    c_type value = (bg::get<1>(b) - bg::get<1>(a)) * (bg::get<0>(c) - bg::get<0>(b))
                   - (bg::get<0>(b) - bg::get<0>(a)) * (bg::get<1>(c) - bg::get<1>(b));

    return (value < 0);
}

// Driver code
template <typename MultiPoint>
inline MultiPoint GiftWrapping(MultiPoint input)
{
    MultiPoint hull;

    typedef typename bg::point_type<MultiPoint>::type point_type;
    typedef typename boost::range_size<MultiPoint>::type size_type;

    size_type n = boost::size(input);

    // Sorts input in increasing order of x values and in case of ties, increasing y values
    std::sort(boost::begin(input), boost::end(input), [&](point_type a, point_type b)
    {
        if (bg::get<0>(a) == bg::get<0>(b))
        {
            return (bg::get<1>(a) < bg::get<1>(b));
        }
        return (bg::get<0>(a) < bg::get<0>(b));
    });

    // Algorithm starts from leftmost point, which is the first point in input
    size_type p = 0;
    size_type q = 0;
    while (true)
    {
        bg::append(hull, input[p]);
        q = (p + 1) % n;
        for (size_type i = 0; i < n; ++i)
        {
            if (check(input[p], input[i], input[q]))
            {
                q = i;
            }
        }
        if (q == 0)
        {
            bg::append(hull, input[q]);
            break;
            // Ends when counter reaches leftmost point again
        }
        p = q;
    }

    // Uncomment lines below to print the current convex hull
    // size_type h = boost::size(hull);
    // Hull<MultiPoint, size_type> debug(hull, h);
    // debug.print_hull();
    
    return hull;
}
