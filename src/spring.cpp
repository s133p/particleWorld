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
    k = .05;
    d = 250.2;//length(a->position - b->position)*randFloat( .6, .9 );
    drawing = false;
}

void spring::update(float iterations)
{
    
    vec3 force = (b->position - a->position);
    float l = length(force);
    float x = d-l;
    force = normalize(force);
    float f = -1 * k * x;
    force *= f;
    force -= (0.07f * (a->velocity - b->velocity));
    
    if (x < 16 && d == 0) { //"ARRIVE" spring
        force = force - a->velocity;
    }
    
    //if (abs(x) < 15) {drawing = false; return;}
    //else {drawing = false;}
    
    a->addForce(force);
    b->addForce(-force);
}

void spring::draw()
{
    if (!drawing) return;
    if (a->drawing && b->drawing)
        gl::drawLine(a->position, b->position);
}