//
//  motion.cpp
//  particleWorld
//
//  Created by Luke Purcell on 5/3/16.
//
//

#include "motion.hpp"

motion::motion()
{
    
}

motion::motion(list<particle*> & availableParticles)
{
    running = true;
    drawing = true;
    for (auto it = availableParticles.begin(); it != availableParticles.end();)
    {
        particles.push_back(*it);
        //it = availableParticles.erase(it); //Erase when removing from container
        it++;
    }
}

void motion::saveForce()
{
    if (!running) return;
    for (auto p : particles)
    {
        p->sForce = p->force;
        p->force = vec3();
    }
}

void motion::avgForce()
{
    if (!running) return;
    for (auto p : particles)
    {
        p->force += p->sForce;
    }
}

void motion::update(float forceScale)
{
    if (!running) return;
    for (auto p : particles)
    {
        p->force *= forceScale;
    }
}

void motion::draw()
{
    if (!drawing) return;
    
    
    //Draws "shadows"
    gl::color(.3,.3,.3);
    for (auto p : particles)
    {
        gl::pushMatrices();
        gl::translate( (p->position*vec3(1,0,1)) + vec3(0,-390,0) );
        gl::rotate( toRadians(90.0f), vec3(1,0,0) );
        gl::drawSolidCircle(vec2(), p->radius);
        gl::popMatrices();
    }
}