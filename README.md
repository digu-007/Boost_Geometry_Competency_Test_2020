# Gift Wrapping Convex Hull and Concave Hull using k-Nearest Neighbours

## How to use?
1. Clone / Download the repository.
2. Add the header of the file you want to use in your cpp file, by specifying the file address this way: 
```cpp
#include <file_address/concave_hull_k_nearest_neighbours_optimized.hpp>
```
You can check the file address using command `pwd` in linux or`cd` in windows (You can use relative file address as well).

## Gift Wrapping Convex Hull
This is the brute force method to find the convex hull of a set of points, in complexity O(n<sup>2</sup>). In this algorithm we check all the points in the current dataset and finds the one which makes the biggest counter-clockwise turn with the last edge, so every point added contends with all other non-convex hull points, making it a slower process for finding convex hull.This header supports MultiPoint concept of boost geometry as specified in test.

### Performance
For 100000 random points, execution time was about `752ms`.

For 50000 random points, execution time was about `359ms`.

For 10000 random points, execution time was about `63ms`.

## Concave Hull using k-Nearest Neighbours
Since my project is related to concave hull so I implemented a concave hull algorithm using template metaprogramming supporting Boost Geometry's MultiPoint concept for this test. k-Nearest Neighbours approach for finding concave hull uses k nearest points from the current point to find the next vertex of the concave hull. This method is kind of similar to gift wrapping, but there we have value of k = total number of points in the current dataset and there exists no special cases in that. Here we tackle two special cases.

1. If we form an edge with the previous point to current point and if this edge intersects the polygon, then this point is undesirable for the current addition. So we need to take care of all such `bad` points.

2. If there are clusters of points which are very far away from each other, then small `k` would lead to concave hull with points of only one cluster and others would remain outside the polygon. If this condition occurs we can simply call the same algorithm for `k = k + 1` and carry on this process until we get all the points inside concave hull.

Naive implementation of this algorithm is of complexity O(n<sup>3</sup>) as written in many of the papers, but after using boost geometry's **R-Tree** for k-NN and most importantly **intersection queries**, I achieved the final complexity of **O(n<sup>2</sup> log n)**. Here is the exact code for interesection query:
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

### Performance

For 200 random points and `k = 3`, execution time was about `221ms` for optimized version and `501ms` for basic version. Here k was incremented to `k = 14` for valid concave hull.

For 100 random points and `k = 3`, execution time was about `1122ms` for optimized version and `2474ms` for basic version. Here k was incremented to `k = 44` for valid concave hull.

For 100 random points and `k = 12`, execution time was about `58ms` for optimized version and `145ms` for basic version. Here k was incremented to `k = 15` for valid concave hull.

As we can see this algorithm is simple but not that efficient for real use. For that I will be implementing the `χ-shape` concave hull algorithm in the GSoC period, if my proposal gets accepted.