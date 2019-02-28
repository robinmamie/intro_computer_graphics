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

#include "Plane.h"
#include <limits>


//== CLASS DEFINITION =========================================================



Plane::Plane(const vec3& _center, const vec3& _normal)
: center(_center), normal(_normal)
{
}


//-----------------------------------------------------------------------------


bool
Plane::
intersect(const Ray& _ray,
          vec3&      _intersection_point,
          vec3&      _intersection_normal,
          double&    _intersection_t ) const
{
	const vec3 &rayDir = _ray.direction;
    const vec3 &rayOrigin = _ray.origin;
    double dotRayDirNormal = dot(rayDir, normal);

    _intersection_t = NO_INTERSECTION;

    //check if parallel
    if(dotRayDirNormal == 0){
		return false;		
	}
	
	//if not, compute intersection
	const vec3   oc = _ray.origin - center;
	double t = (norm(oc) - dot(rayOrigin, normal)) / dotRayDirNormal;
	
	//check if solution is in front of the viewer
	if(t > 0){
		return  false;
	}
	
	_intersection_t = t;
	_intersection_point  = _ray(_intersection_t);
	_intersection_normal = normal;

    return true;
}


//=============================================================================
