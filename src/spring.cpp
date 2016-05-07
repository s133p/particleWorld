//
//  spring.cpp
//  particleWorld
//
//  Created by Luke Purcell on 5/3/16.
//
//

#include "spring.hpp"

spring::spring(particle* aa, particle* bb)
{
    a = aa;
    b = bb;
    k = .03;
    d = 2.2;//length(a->position - b->position)*randFloat( .6, .9 );
}

void spring::update(float iterations)
{
    
    vec3 force = (b->position - a->position);
    float l = length(force);
    float x = d-l;
    force = normalize(force);
    float f = -1 * k * x * (1.0f/iterations);
    force *= f;
    
    if (x < 1 && d == 0) { //"ARRIVE" spring
        force = force - a->velocity;
    }
    
    a->addForce(force);
    b->addForce(-force);
}

void spring::draw()
{
    if (a->drawing && b->drawing)
        gl::drawLine(a->position, b->position);
}