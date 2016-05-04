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
    k = .01;
    d = length(a->position - b->position)*randFloat(.2,0.45);
    running = true;
}

void spring::update(float iterations)
{
    
    
    vec3 force = (b->position - a->position);
    float l = length(force);
    float x = d-l;
    
    /*if (x > 15) running = false;
     if (x <= 15) running = true;
     if (!running) return;*/
    
    force = normalize(force);
    force *= -1 * k * x * (1.0f/iterations);
    
    a->addForce(force);
    b->addForce(-force);
}

void spring::draw()
{
    if (!running) return;
    gl::drawLine(a->position, b->position);
}