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
    k = .07;
    d = length(a->position - b->position)*randFloat( .6, .9 );
}

void spring::update(float iterations)
{
    
    vec3 force = (b->position - a->position);
    float l = length(force);
    float x = d-l;
    if (x < 2 && d == 0) { //"ARRIVE" spring
        force = normalize(force);
        force *= -1 * k * x * (1.0f/iterations);
        force = force-a->velocity;
    }else{ //NORMAL SPRING
        force = normalize(force);
        force *= -1 * k * x * (1.0f/iterations);
    }
    
    a->addForce(force);
    b->addForce(-force);
}

void spring::draw()
{
    gl::drawLine(a->position, b->position);
}