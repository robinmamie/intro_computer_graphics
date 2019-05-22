//=============================================================================
//
//   Exercise code for the lecture "Introduction to Computer Graphics"
//     by Prof. Mario Botsch, Bielefeld University
//
//   Copyright (C) by Computer Graphics Group, Bielefeld University
//
//=============================================================================
#ifndef SKY_H
#define SKY_H
//=============================================================================

#include "glmath.h"

//=============================================================================


class Sky
{
public:

    /// default constructor
    Sky(const float _radius,
        const float _distance) :
        pos_(_distance,0,0,1),
        distance_(_distance),
        radius_(_radius)
    {}

    /// the current position of the planet
    vec4 pos_;
    /// distance of the planet from the orbit center (=origin)
    float distance_;
    /// the radius of the planet
    float radius_;

};

//=============================================================================
#endif // SKY_H
//=============================================================================
