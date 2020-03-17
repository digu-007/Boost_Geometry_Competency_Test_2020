# Gift Wrapping Convex Hull and Concave Hull using k-Nearest Neighbours

## Note
This programming competency test is for Project 1, implement algorithms for the concave hull problem. My solution work for competency test is in the `/includes` directory, where `convex_hull_gift_wrapping.hpp` is the solution to what is specified in [wiki page](https://github.com/boostorg/wiki/wiki/Google-Summer-of-Code%3A-2020) and `concave_hull_k_nearest_neighbours_optimized.hpp` is the solution that matches my project idea (as discussed with the mentors earlier).

## How to use?
1. Clone / Download the repository.
2. Add the header of the file you want to use in your cpp file, by specifying the file address this way: 
```cpp
#include <file_address/concave_hull_k_nearest_neighbours_optimized.hpp>
```
You can check the file address using command `pwd` in linux or`cd` in windows (You can use relative file address as well).

## Gift Wrapping Convex Hull
This is the brute force method to find the convex hull of a set of points, in complexity O(n * h), where n is the number of points in the intial dataset and h is the number of points in the convex hull. In this algorithm we check all the points in the current dataset and finds the one which makes the biggest counter-clockwise turn with the last edge, so every point added contends with all other non-convex hull points, making it a slower process for finding convex hull. This header supports MultiPoint concept of boost geometry as specified in test.

### Performance
For 100,000 random points, execution time was about `752ms`.

For 50,000 random points, execution time was about `359ms`.

For 10,000 random points, execution time was about `63ms`.

When I compared this algorithm with the one implemented in boost geometry I found that gift wrapping is on average **25% slower**. But for cases where all the points in the initial dataset are a part of convex hull, in such cases this algorithm is very slow. On the other hand when the concave hull had only 4-5 points, then the performance of gift wrapping was atleast twice better than the boost geometry one. But in general, I don't think so if there are benefits in implementing this algorithm for boost geometry.

The main problem of the code is that we need to iterate over all the points to find the current vertex of convex hull using cross product. Here is bottleneck code:
```cpp
for (size_type i = 0; i < n; ++i)
{
    if (check(input[p], input[i], input[q])) // this function checks orientation of the three points.
    {
        q = i;
    }
}
```

Tests were carried out in Ubuntu 18.04.1, with gcc version 7.4.0 and optimization level `O2`.

## Concave Hull using k-Nearest Neighbours
Since my project is related to concave hull so I implemented a concave hull algorithm using template metaprogramming supporting Boost Geometry's MultiPoint concept for this test. k-Nearest Neighbours approach for finding concave hull uses k nearest points from the current point to find the next vertex of the concave hull. This method is kind of similar to gift wrapping, but there we have value of k = total number of points in the current dataset and there exists no special cases in that. Here we tackle two special cases.

1. If we form an edge with the previous point to current point and if this edge intersects the polygon, then this point is undesirable for the current addition. So we need to take care of all such `bad` points.

2. If there are clusters of points which are very far away from each other, then small `k` would lead to concave hull with points of only one cluster and others would remain outside the polygon. If this condition occurs we can simply call the same algorithm for `k = k + 1` and carry on this process until we get all the points inside concave hull.

Naive implementation of this algorithm is of complexity O(d * n<sup>3</sup>) where n is the total number of points in the initial dataset and d is the change in the value of k. After using boost geometry's **R-Tree** for k-NN and most importantly **intersection queries**, I achieved the final complexity of **O(d * n<sup>2</sup> log n)**. Here is the exact code for intersection query:
```cpp
std::vector<segment_type> bad;
segment_type seg(k_nearest[i], cur_point);
rt2.query(bgi::intersects(seg), std::back_inserter(bad));
```

### Output
Here is an example showing convex hull for `42` points in the shape of letter `L`, followed by concave hull of the same dataset for `k = 20`, `k = 11` and `k = 3`.

![k = 42](https://github.com/digu-007/Boost_Geometry_Competency_Test_2020/blob/develop/images/L-42.png)
![k = 20](https://github.com/digu-007/Boost_Geometry_Competency_Test_2020/blob/develop/images/L-20.png)

![k = 11](https://github.com/digu-007/Boost_Geometry_Competency_Test_2020/blob/develop/images/L-11.png)
![k = 3](https://github.com/digu-007/Boost_Geometry_Competency_Test_2020/blob/develop/images/L-3.png)

Here is another example showing convex hull for `154` points in the shape of letter `S`, followed by concave hull of the same dataset for `k = 20`, `k = 11` and `k = 2`.

![k = 154](https://github.com/digu-007/Boost_Geometry_Competency_Test_2020/blob/develop/images/S-154.png)
![k = 20](https://github.com/digu-007/Boost_Geometry_Competency_Test_2020/blob/develop/images/S-20.png)

![k = 11](https://github.com/digu-007/Boost_Geometry_Competency_Test_2020/blob/develop/images/S-11.png)
![k = 2](https://github.com/digu-007/Boost_Geometry_Competency_Test_2020/blob/develop/images/S-2.png)

As we can see if the k decreases concave hull becomes more 'sharper'. When k is equal to total points, then we get convex hull as the concave hull solution.

### Performance
For 500 random points and `k = 50`, execution time was about `3123ms` for optimized version and `9890ms` for basic version. Here k was incremented to `k = 82` for valid concave hull.

For 500 random points and `k = 30`, execution time was about `873ms` for optimized version and `2082ms` for basic version. Here k was incremented to `k = 39` for valid concave hull.

For 200 random points and `k = 10`, execution time was about `87ms` for optimized version and `368ms` for basic version. Here k was incremented to `k = 14` for valid concave hull.

For 100 random points and `k = 3`, execution time was about `94ms` for optimized version and `389ms` for basic version. Here k was incremented to `k = 16` for valid concave hull.

As we can see this algorithm is simple but not that efficient for real use.
