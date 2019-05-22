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
    Sky(const float _radius) :
        pos_(0,0,0,1),
        radius_(_radius)
    {}

    /// the current position of the planet
    vec4 pos_;

    /// the radius of the planet
    float radius_;

};

//=============================================================================
#endif // SKY_H
//=============================================================================
