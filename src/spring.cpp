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
    k = .18;
    d = 50.0f;// * length(a->position - b->position);
    drawing = true;
}

void spring::update(float iterations)
{
    
    vec3 force = (b->position - a->position);
    float l = length(force);
    float x = d-l;
    force = normalize(force);
    float f = -1 * k * x;
    force *= f;
    force -= (0.02f * (a->velocity - b->velocity));
    
    if (x < 16 && d == 0) { //"ARRIVE" spring when spring rest length is 0
        force = force - a->velocity;
    }
    
    if (a->moving) a->addForce(force);
    if (b->moving) b->addForce(-force);
}

void spring::draw()
{
    if (!drawing) return;
    if (a->drawing && b->drawing)
        gl::drawLine(a->position, b->position);
}