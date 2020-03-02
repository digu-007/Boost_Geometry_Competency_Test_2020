# Gift Wrapping Convex Hull and Concave Hull using k-Nearest Neighbours

## How to use?
1. Clone / Download the repository.
2. Add the header in your cpp file, by specifying the file address this way: 
```cpp
#include <file_address/concave_hull_k_nearest_neighbours_optimized.hpp>
```
You can check the file address using command `pwd` in linux or`cd` in windows (You can use relative file address as well).

## Gift Wrapping Convex Hull
This is the brute force method to find the convex hull of a set of points, in complexity O(n<sup>2</sup>). This header supports MultiPoint concept of boost geometry as specified in test.

### Output
For random points, execution time was about seconds.

**Ouput file**

## Concave Hull using k-Nearest Neighbours
Since my project is related to concave hull so I implemented an easy concave hull algorithm using template metaprogramming supporting MultiPoint concept for this test. k-Nearest Neighbours approach for finding concave hull uses k nearest points from the current point to find the next vertex of the concave hull. This method is kind of similar to gift wrapping, but there we have value of k = total number of points in the current dataset and there exists no special cases in that. Here we tackle two special cases.

1. 

2. 

Naive implementation of this algorithm is of complexity O(n<sup>3</sup>), but after using boost geometry's R-Tree for k-NN and most importantly intersection queries, I achieved the final complexity of O(n<sup>2</sup> log n).

### Pseudo code
Following is the pseudo code for the optimized version of the algorithm:

Here is an example showing convex hull for 42 points in the shape of letter 'L', followed by concave hull of the same dataset for different values of k.

### Output
For points, execution time was about seconds.

**Ouput file**

For points, execution time was about seconds.

**Ouput file**
