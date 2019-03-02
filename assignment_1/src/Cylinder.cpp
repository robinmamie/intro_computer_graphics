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


Cylinder::Cylinder(const vec3 &_center,
                   double _radius,
                   const vec3 &_axis,
                   double _height)
: center(_center), radius(_radius), axis(_axis), height(_height)
{
}

//-----------------------------------------------------------------------------


bool
Cylinder::
intersect(const Ray&  _ray,
          vec3&       _intersection_point,
          vec3&       _intersection_normal,
          double&     _intersection_t) const
{

    //compute the first valid intersection `_ray` with the cylinder
    //(valid means in front of the viewer: t > 0)

    const vec3 &dir = _ray.direction;
    const vec3 oc = _ray.origin - center;
    const vec3 &product = dot(_ray.direction, axis)*axis;
    const vec3 &deltaP = _ray.origin - center;
    const vec3 &preA = dir - product;
    const vec3 &preB = deltaP - dot(deltaP, axis)*axis;

    std::array<double, 2> t;
    size_t nsol = solveQuadratic(dot(preA, preA),
                                 2*dot(dir - product, deltaP - dot(deltaP, axis)*axis),
                                 dot(preB, preB) - radius*radius, t);

    _intersection_t = NO_INTERSECTION;

    for (size_t i = 0; i < nsol; ++i) { // closest intersection
        //vec3 distance_on_axis = reflect(_ray(t[i])- center, axis);
        //if (t[i] > 0 && norm(distance_on_axis)<height/2) _intersection_t = std::min(_intersection_t, t[i]);

        double min_value = std::min(_intersection_t, t[i]);
        if (t[i] > 0 && distance(_ray(min_value), center)< sqrt(radius*radius + (height/2)*(height/2))) _intersection_t = min_value;
    }

    if (_intersection_t == NO_INTERSECTION) return false;

    //store intersection point in `_intersection_point`
    _intersection_point  = _ray(_intersection_t);

    //store ray parameter in `_intersection_t`

    //store normal at _intersection_point in `_intersection_normal`.
    vec3 offset_on_axis = reflect(_intersection_point - center, axis);
    _intersection_normal = normalize(_intersection_point - center + offset_on_axis);
    //_intersection_normal *= -1;

    //return whether there is an intersection with t > 0
    return true; // already return false if NO_INTERSECTION
}
