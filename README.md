# Visibility Polygon
[![Build Status](https://travis-ci.org/trylock/visibility.svg?branch=master)](https://travis-ci.org/trylock/visibility)

Simple [Visibility polygon](https://en.wikipedia.org/wiki/Visibility_polygon) algorithm implementation. My inspiration for this project was [2d Visibility](http://www.redblobgames.com/articles/visibility/) article on the Red Blob Games website. This was my semestral work for lecture Algorithms and Data Structures II at [MFF UK](https://www.mff.cuni.cz/to.en/).

The project is separated into 2 subprojects: *visibility* (header-only library) and *tests*.

## Main idea of the algorithm

First, we sort the endpoints of the line segments in CW order where the observer's position is the center point. Line segment endpoint whose other endpoint is greater in this CW order is called a *start vertex*. Otherwise it's an *end vertex*.

Next, the program implements a sweep line algorithm. The sweep line here is a ray. Its origin is at the observer's position and it rotates around this point. The algorithm keeps a state which is a set of line segments that are currently intersected by the ray. In addition, these line segments are sorted by the distance to the origin. When the sweep line passes through an enpoint of a line segment, it either adds this line segment to the state (in case of a *start vertex*) or removes it from the state (in case of an *end vertex*). Whenever the nearest line segment changes, we update vertices of the visibility polygon accordingly.

## API

All functions and classes are in the `geometry` namespace.

### Visibility Polygon

Main function of the library is the `visibility_polygon` function. It takes the observer's position and list (`std::vector`) of line segments as its arguments. It computes vertices of a visibility polygon in CW order and returns them. The line segments must not intersect except at their endpoints and the visiblity polygon has to be closed in order for it to produce a correct result (you can, for example, add edges of a bounding rectangle to ensure that the visibility polygon will always be closed). Notice that a closed visibility polygon will always be star-shaped (by definition) so its triangulation is trivial.

### Vector

The program implements a 2D vector template in the `vector2.hpp` header. You can use immutable operators `+`, `-`, `*`, `/` as well as their mutable variants. Apart from that you can use global functions `dot(a, b)` (calculates a dot product of 2 vectors), `length_squared(vector)`, `distance_squared(a, b)`, `normal(a)` (calculates a 2D orthogonal vector), `cross(a, b)` (determinat of the `[[a_x, b_x], [a_y, b_y]]` matrix). Floating point vectors can be normalized to have an unit length using the `normalize(vector)` function (it returns 0 vector in case of a 0 vector). 

### Primitives

All primitive objects that are used, are defined in the `primitives.hpp` header. It defines `line_segment` and `ray` types as well as a healper function `compute_orientation` that returns orientation of 3 points in a plane (left_turn, right_turn or collinear).
