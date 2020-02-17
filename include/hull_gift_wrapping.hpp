/*
    Boost Competency Test - GSoC 2020
    digu_J - Digvijay Janartha
    NIT Hamirpur - INDIA
*/

#include <iostream>

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
    for (int i = 0; i < n; ++i)
    {
        std::cout << bg::get<0>(hull[i]) << " " << bg::get<1>(hull[i]) << std::endl;
    }
}

// This method returns true if the points are oriented in counter-clockwise manner
template <typename Point>
inline bool check(Point a, Point b, Point c)
{
    double value = (bg::get<0>(b) - bg::get<0>(a)) * (bg::get<1>(c) - bg::get<1>(a))
                   - (bg::get<1>(b) - bg::get<1>(a)) * (bg::get<0>(c) - bg::get<0>(a));

    return (value < 0);
}

// Driver code
template <typename MultiPoint>
inline MultiPoint GiftWrapping(MultiPoint input)
{
    MultiPoint hull;

    int n = boost::size(input);
    int leftmost = 0;
    for (int i = 1; i < n; ++i)
    {
        if (bg::get<0>(input[i]) < bg::get<0>(input[leftmost]))
        {
            leftmost = i;
        }
    }

    // Algorithm starts from leftmost point
    int p = leftmost;
    int q = 0;
    while (true)
    {
        bg::append(hull, input[p]);
        q = (p + 1) % n;
        for (int i = 0; i < n; ++i)
        {
            if (check(input[p], input[i], input[q]))
            {
                q = i;
            }
        }
        if (q == leftmost)
        {
            bg::append(hull, input[q]);
            break;
            // Ends when counter reaches leftmost point again
        }
        p = q;
    }

    // Uncomment lines below to print the current convex hull
    // int h = boost::size(hull);
    // Hull<MultiPoint, int> debug(hull, h);
    // debug.print_hull();
    
    return hull;
}
