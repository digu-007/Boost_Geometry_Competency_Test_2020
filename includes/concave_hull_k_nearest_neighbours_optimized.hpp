/*
    Boost Competency Test - GSoC 2020
    digu_J - Digvijay Janartha
    NIT Hamirpur - INDIA
*/

#include <algorithm>
#include <iostream>
#include <iterator>
#include <set>
#include <utility>
#include <vector>

#include <boost/geometry/geometry.hpp>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

namespace algo3
{
// Solves 2D concave hull in O((n ^ 2) * log(n)) complexity for Multipoint concept
// Where n is the total input points
template <typename MultiPoint, typename Size, typename Factor>
class ConcaveHull {

    private:
        MultiPoint hull;
        Size n;
        Factor k;
    
    public:
        ConcaveHull(MultiPoint mp, Size sz, Factor cur_k);
        void print_hull();
};

template <typename MultiPoint, typename Size, typename Factor>
ConcaveHull<MultiPoint, Size, Factor>::ConcaveHull(MultiPoint mp, Size sz, Factor cur_k)
{
    hull = mp;
    n = sz;
    k = cur_k;
}

// To print the concave hull
template <typename MultiPoint, typename Size, typename Factor>
void ConcaveHull<MultiPoint, Size, Factor>::print_hull()
{   
    std::cout << "Current value of k is: " << k << std::endl;
    std::cout << "Resulting points of Concave Hull are: " << std::endl;
    for (Size i = 0; i < n; ++i)
    {
        std::cout << bg::get<0>(hull[i]) << " " << bg::get<1>(hull[i]) << std::endl;
    }
}

template <typename DataType>
static inline DataType max(DataType a, DataType b)
{
    return (a > b ? a : b);
}

template <typename DataType>
static inline DataType min(DataType a, DataType b)
{
    return (a > b ? b : a);
}

// Sorts input in increasing order of x values and in case of ties, increasing y values
template <typename Range>
static inline void sort(Range& range)
{
    typedef typename boost::range_value<Range>::type point_type;
    typedef bg::less<point_type> comparator;

    std::sort(boost::begin(range), boost::end(range), comparator());
}

// This function returns 1 if points are oriented in counter-clockwise manner and -1 if collinear
template <typename Point>
static inline int RightHandTurn(Point a, Point b, Point c)
{
    typedef typename bg::coordinate_type<Point>::type c_type;

    c_type value = (bg::get<1>(b) - bg::get<1>(a)) * (bg::get<0>(c) - bg::get<0>(b))
                   - (bg::get<0>(b) - bg::get<0>(a)) * (bg::get<1>(c) - bg::get<1>(b));
    if (value == 0)
    {
        return -1;
    }
    return (value < 0 ? 1 : 0);
}

// Removes duplicates from input in O(n * log(n)) complexity
template <typename MultiPoint>
static inline void CleanList(MultiPoint& mp)
{
    typedef typename boost::range_size<MultiPoint>::type size_type;
    typedef typename bg::coordinate_type<MultiPoint>::type c_type;
    typedef typename bg::point_type<MultiPoint>::type point_type;
    typedef typename std::pair<c_type, c_type> pair_type;
    typedef typename std::set<pair_type>::iterator itr_type;

    size_type n = boost::size(mp);
    std::set<pair_type> st;
    for (size_type i = 0; i < n; ++i)
    {
        st.insert({bg::get<0>(mp[i]), bg::get<1>(mp[i])});
    }
    bg::clear(mp);

    itr_type it;
    for (it = st.begin(); it != st.end(); ++it)
    {
        pair_type p = *it;
        point_type cur = {p.first, p.second};
        bg::append(mp, cur);
    }
}

// Removes a point from multipoint in O(n) complexity by swapping elements and poping out last one
template <typename MultiPoint, typename Point>
static inline void RemovePoint(MultiPoint& mp, Point cur)
{
    typedef typename boost::range_size<MultiPoint>::type size_type;

    size_type n = boost::size(mp);
    for (size_type i = 0; i < n; ++i)
    {
        if (bg::equals(mp[i], cur))
        {
            for (size_type j = i; j < n - 1; ++j)
            {
                mp[j] = mp[j + 1];    
            }
            break;
        }
    }
    mp.pop_back();
}

// Building Rtree for a given container
template <typename MultiPoint, typename RTree>
inline void BuildRTree(MultiPoint& mp, RTree& rt)
{
    typedef typename boost::range_size<MultiPoint>::type size_type;

    size_type n = boost::size(mp);
    for (size_type i = 0; i < n; ++i)
    {
        bgi::insert(rt, mp[i]);
    }
}

// Runs the main algorithm on dataset of points for a particular k value
// Returns empty container when current k gives no solution
template <typename MultiPoint, typename Factor>
static inline MultiPoint Solve(MultiPoint dataset, Factor k)
{
    typedef typename bg::point_type<MultiPoint>::type point_type;
    typedef typename boost::range_size<MultiPoint>::type size_type;
    typedef typename bg::model::linestring<point_type> linestring_type;
    typedef typename bg::model::segment<point_type> segment_type;
    typedef typename bg::model::polygon<point_type> poly_type;

    MultiPoint ans, discard;
    bg::append(ans, dataset[0]);
    point_type cur_point = dataset[0];
    RemovePoint(dataset, cur_point);
    int step = 0;

    bgi::rtree<point_type, bgi::rstar<32>> rt1;
    bgi::rtree<segment_type, bgi::rstar<8>> rt2;
    BuildRTree(dataset, rt1);

    while (boost::size(dataset))
    {
        if (step == 3)
        {
            // once a polygon with more than 3 edges is formed add the first point again
            bg::append(dataset, ans[0]);
            bgi::insert(rt1, ans[0]);
        }
        
        // finding k-NN using RTree
        std::vector<point_type> k_nearest;
        Factor cur_k = min(k, Factor(boost::size(dataset)));
        rt1.query(bgi::nearest(cur_point, cur_k), std::back_inserter(k_nearest));
        
        size_type size1 = boost::size(k_nearest);
        std::vector<point_type> good;
        for (size_type i = 0; i < size1; ++i)
        {   
            bool ok = true;
            linestring_type ls1{k_nearest[i], cur_point};

            // querying RTree to find all the intersecting edges of the hull with the current edge
            std::vector<segment_type> bad;
            segment_type seg(k_nearest[i], cur_point);
            rt2.query(bgi::intersects(seg), std::back_inserter(bad));

            size_type size2 = boost::size(bad);
            if (size2 == 1 and bg::equals(k_nearest[i], ans[0]))
            {   
                // if linestring touches first edge, then its the last edge and is ok
                linestring_type ls2{ans[0], ans[1]};
                if (!bg::touches(ls1, ls2))
                {   
                    // if this linestring does not touch and only intersect then discard this point
                    ok = false;
                }
            }
            else if (size2 > 0)
            {   
                // interesections means point is discarded
                ok = false;
            }

            if (ok)
            {   
                // non intersecting edge is good
                good.push_back(k_nearest[i]);
            }
        }
        if (good.empty())
        {   
            // if no good points are remaining, current k has no solution
            return discard;
        }

        point_type best = good[0];
        size1 = boost::size(good);
        for (size_type i = 1; i < size1; ++i)
        {
            int value = RightHandTurn(cur_point, good[i], best);
            if (value == -1)
            {
                // in case of collinear points take the nearest one as best
                if (bg::distance(good[i], cur_point) < bg::distance(best, cur_point))
                {
                    best = good[i];
                }
            }
            else if (value)
            {
                // update best if we have counter-clockwise orientation
                best = good[i];
            }
        }
        if (step)
        {   
            // adding previously formed edge to RTree
            size_type last = boost::size(ans) - 2;
            segment_type cur_segment(cur_point, ans[last]);
            bgi::insert(rt2, cur_segment);
        }
        cur_point = best;
        bg::append(ans, cur_point);
        RemovePoint(dataset, cur_point);
        bgi::remove(rt1, cur_point);

        if (bg::equals(cur_point, ans[0]))
        {
            // cycle completed
            break;
        }
        ++step;
    }

    // If any point is outside the final polygon, then current k has no solution
    poly_type poly;
    for (size_type i = 0; i < boost::size(ans); ++i)
    {
        bg::append(poly, ans[i]);
    }
    size_type n = boost::size(dataset);
    for (size_type i = 0; i < n; ++i)
    {
        if (!(bg::covered_by(dataset[i], poly)))
        {   
            return discard;
        }
    }
    return ans;
}

// Driver code
template <typename MultiPoint, typename Factor>
inline void ConcaveHullKNN(MultiPoint input, MultiPoint& hull, Factor& k)
{
    typedef typename boost::range_size<MultiPoint>::type size_type;

    CleanList(input);
    size_type n = boost::size(input);
    k = max(k, 2);
    k = min(k, Factor(n));

    // At least three different points are required for concave hull to be formed
    if (n < size_type(3))
    {
        return;
    }
    else if (n == size_type(3))
    {
        hull = input;
        return;
    }

    sort(input);
    while (k <= Factor(n))
    {
        hull = Solve(input, k);
        // If current k given empty solution, then k is increased by one
        if (bg::is_empty(hull))
        {
            ++k;
        }
        else
        {
            break;
        }
    }

    // Uncomment lines below to print information about current concave hull
    // size_type h = boost::size(hull);
    // ConcaveHull<MultiPoint, size_type, Factor> debug(hull, h, k);
    // debug.print_hull();
}

} // namespace algo3
