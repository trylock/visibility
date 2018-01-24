# Visibility Polygon
[![Build Status](https://travis-ci.org/trylock/visibility.svg?branch=master)](https://travis-ci.org/trylock/visibility)

Simple sweep line [Visibility polygon](https://en.wikipedia.org/wiki/Visibility_polygon) algorithm implementation. My inspiration for the project was [this 2d Visibility](http://www.redblobgames.com/articles/visibility/) article on the Red Blob Games website. This was my semestral work for lecture Algorithms and Data Structures II at [MFF UK](https://www.mff.cuni.cz/to.en/).

The project is separated into 2 subprojects: *visibility* (header-only library) and *tests*.

## Main idea of the algorithm

- Input: set of line segments (obstacles) and a position of an observer. 
- Output: visibility polygon (i.e. largest polygon s.t. a line segment from the observer's position to any point in the polygon does not intersect any obstacle)

First, we sort endpoints of the line segments (obstacles) in CW order where the observer's position is the center point. Line segment endpoint whose other endpoint is greater in this CW order is called a *start vertex*. Otherwise it's an *end vertex*.

Visibility polygon is found using a sweep line algorithm. The sweep line here is a ray. Its origin is at the observer's position and it rotates around this point. The algorithm keeps a state which is a set of line segments that are currently intersected by the ray. In addition, these line segments are sorted by the distance to the origin. When the sweep line passes through an enpoint of a line segment, it either adds this line segment to the state (in case of a *start vertex*) or removes it from the state (in case of an *end vertex*). Whenever the nearest line segment changes, we update vertices of the visibility polygon accordingly.

## API

All functions and classes are in the `geometry` namespace.

### Visibility Polygon

Main function of the library is the `visibility_polygon` function. It takes observer's position and begin and end iterator of the line segment list (obstacle list) as its arguments. It computes vertices of a visibility polygon in CW order and returns them.

**Preconditions:**
- The line segments must not intersect except at their endpoints 
- The visiblity polygon has to be closed. 

**Behaviour of the library is undefined if the preconditions aren't met**. The first condition can be met by finding all intersection points of line segments and splitting them up. The second condition can be met by adding line segments of the bounding box of all obstacles. Note: checking these conditions is entirely up you. This library does not check them as that would introduce additional overhead.

### Vector

The program implements a 2D vector template in the `vector2.hpp` header. You can use immutable operators `+`, `-`, `*`, `/` as well as their mutable variants. Apart from that you can use global functions `dot(a, b)` (calculates a dot product of 2 vectors), `length_squared(vector)`, `distance_squared(a, b)`, `normal(a)` (calculates a 2D orthogonal vector), `cross(a, b)` (determinat of the `[[a_x, b_x], [a_y, b_y]]` matrix). Floating point vectors can be normalized to have an unit length using the `normalize(vector)` function (it returns 0 vector in case of a 0 vector). 

### Primitives

All primitive objects that are used, are defined in the `primitives.hpp` header. It defines `line_segment` and `ray` types as well as a healper function `compute_orientation` that returns orientation of 3 points in a plane (left_turn, right_turn or collinear).
