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

// Solves 2D convex hull in O(n * h) complexity for Multipoint concept
// Where n is the total input points and h is the total points in convex hull
template <typename MultiPoint, typename Size>
class ConvexHull {

    private:
        MultiPoint hull;
        Size n;

    public:
        ConvexHull(MultiPoint mp, Size sz);
        void print_hull();
};

template <typename MultiPoint, typename Size>
ConvexHull<MultiPoint, Size>::ConvexHull(MultiPoint mp, Size sz)
{
    hull = mp;
    n = sz;
}

// To print the convex hull
template <typename MultiPoint, typename Size>
void ConvexHull<MultiPoint, Size>::print_hull()
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

// Sorts input in increasing order of x values and in case of ties, increasing y values
template <typename Range>
static inline void sort(Range& range)
{
    typedef typename boost::range_value<Range>::type point_type;
    typedef boost::geometry::less<point_type> comparator;

    std::sort(boost::begin(range), boost::end(range), comparator());
}

// Driver code
template <typename MultiPoint>
inline void GiftWrapping(MultiPoint input, MultiPoint& hull)
{
    typedef typename boost::range_size<MultiPoint>::type size_type;

    size_type n = boost::size(input);

    sort(input);

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
            // Ends when counter reaches leftmost point again
            break;
        }
        p = q;
    }

    // Uncomment lines below to print the current convex hull
    // size_type h = boost::size(hull);
    // ConvexHull<MultiPoint, size_type> debug(hull, h);
    // debug.print_hull();
}
