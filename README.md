# Visibility Polygon
[![Build Status](https://travis-ci.org/trylock/visibility.svg?branch=master)](https://travis-ci.org/trylock/visibility)

Simple [Visibility polygon](https://en.wikipedia.org/wiki/Visibility_polygon) algorithm implementation. My inspiration for this project was [2d Visibility](http://www.redblobgames.com/articles/visibility/) article on the Red Blob Games website. This was my semestral work for lecture ADS2 at MFF UK.

The project is separated into 2 subprojects: *visibility* (header-only library) and *tests*.

The program implements a sweep line algorithm. The sweep line moves on a circle that is centered at the observer's position. State of the algorithm is a set of line segments that are currently intersected by the sweep line (which is a ray). The events are: *start of a line segment* and *end of a line segment*.

Input of the algorithm is a point (observer's position) and a list of line segments (obstacles). The line segments **must not intersect** except at their endpoints. The time complexity of the algorithm is linearithmic. Space complexity is linear in the number of line segments.

## API

All functions and classes are in the `geometry` namespace.

### Vector

The program implements a fixed sized vector template in the `Vector.hpp` header. You can use immutable operators `+`, `-`, `*`, `/` as well as their mutable variants. Apart from that you can use global functions `dot(a, b)` (calculates a dot product of 2 vectors), `length_squared(vector)`, `distance_squared(a, b)`, `normal(a)` (calculates a 2D orthogonal vector), `cross(a, b)` (determinat of the `[[a_x, b_x], [a_y, b_y]]` matrix). Floating point vectors can be normalized to have an unit length using the `normalize(vector)` function (returns 0 vector in case of a 0 vector). 

### Primitives

All primitive objects that are used, are defined in the `Primitives.hpp` header. It defines `LineSegment` and `Ray` types as well as a healper function `orientation` that returns orientation of 3 points in a plane (LeftTurn, RightTurn or Collinear).
