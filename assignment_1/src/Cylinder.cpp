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

bool
Cylinder::
intersect(const Ray&  _ray,
          vec3&       _intersection_point,
          vec3&       _intersection_normal,
          double&     _intersection_t) const
{
    // TODO

    /** DEFINITION OF CYLINDER
     * 
     * Cylinder(const vec3 &_center = vec3(0,0,0),
     *          double _radius = 1,
     *          const vec3 &_axis = vec3(1,0,0),
     *          double _height = 1)
     *          :  center(_center), radius(_radius), axis(_axis), height(_height) { }
    **/

    //compute the first valid intersection `_ray` with the cylinder
    //(valid means in front of the viewer: t > 0)

    const vec3 &dir = _ray.direction;
    const vec3 oc = _ray.origin - center;

    std::array<double, 2> t;
    size_t nsol = solveQuadratic(dir[0]*dir[0]+dir[2]*dir[2], //dir.x²+dir.z²
                                 dir[0]*oc[0]+dir[2]*oc[2], // dir.x*oc.x + dir.z*oc.z
                                 oc[0]*oc[0]+oc[2]*oc[2]- radius*radius, t); // oc.x² + oc.z² - radius²

    _intersection_t = NO_INTERSECTION;

    for (size_t i = 0; i < nsol; ++i) { // closest intersection
        if (t[i] > 0) _intersection_t = std::min(_intersection_t, t[i]);
        std::cout << "test" << std::endl;
    }

    if (_intersection_t == NO_INTERSECTION) return false;

    //store intersection point in `_intersection_point`
    _intersection_point  = _ray(_intersection_t);

    //store ray parameter in `_intersection_t`
    
    //store normal at _intersection_point in `_intersection_normal`.
    vec3* point = new vec3(center[0], _intersection_point[1], center[2]);
    //_intersection_normal = norm(_intersection_point - point);
    
    //return whether there is an intersection with t > 0
    return true; // already return false if NO_INTERSECTION
}
