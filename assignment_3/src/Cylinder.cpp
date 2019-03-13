//=============================================================================
//
//   Exercise code for the lecture
//   "Introduction to Computer Graphics"
//   by Prof. Dr. Mario Botsch, Bielefeld University
//
//   Copyright (C) Computer Graphics Group, Bielefeld University.
//
//=============================================================================

//== INCLUDES =================================================================

#include "Cylinder.h"
#include "SolveQuadratic.h"
#include <array>
#include <cmath>

//== IMPLEMENTATION =========================================================
// def on Cylinder is in Cylinder.h
//-----------------------------------------------------------------------------


bool
Cylinder::
intersect(const Ray&  _ray,
          vec3&       _intersection_point,
          vec3&       _intersection_normal,
          double&     _intersection_t) const
{

    // Compute the first valid (i.e. in front of the viewer, t > 0)
    // intersection `_ray` with the cylinder.

    const vec3    &dir = _ray.direction;
    const vec3      &x = dir - dot(dir, axis) * axis;
    const vec3 &deltaP = _ray.origin - center;
    const vec3      &y = deltaP - dot(deltaP, axis) * axis;

    std::array<double, 2> t;
    size_t nsol = solveQuadratic(dot(x, x),
                                 2 * dot(x, y),
                                 dot(y, y) - radius * radius, t);

    _intersection_t = NO_INTERSECTION;

    // Compute the closest intersection.
    for (size_t i = 0; i < nsol; ++i) {
        double min_value =
            std::min(_intersection_t, t[i]);

        double center_to_border =
            sqrt(radius * radius + height * height / 4);

        if (t[i] > 0 && distance(_ray(min_value), center) < center_to_border) {
            _intersection_t = min_value;
        }
    }

    // Stop if there is no intersection.
    if (_intersection_t == NO_INTERSECTION) {
        return false;
    }

    // Return intersection point in `_intersection_point`.
    _intersection_point  = _ray(_intersection_t);

    // Return normal at _intersection_point in `_intersection_normal`..
    vec3 offset_on_axis = reflect(_intersection_point - center, axis);
    _intersection_normal = normalize(_intersection_point - center + offset_on_axis);
    // NB: 2 vectors form an acute angle if their dot product is positive
    _intersection_normal *= dot(_intersection_normal, dir) > 0 ? -1 : 1;

    return true;
}
