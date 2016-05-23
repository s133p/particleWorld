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
    return;
    if (!drawing) return;
    
    gl::disableDepthWrite();
    //Draws "shadows"
    gl::color(.35,.35,.35);
    for (auto p : particles)
    {
        gl::pushMatrices();
        gl::translate( (p->position*vec3(1,0,1)) + vec3(0,-600,0) );
        gl::rotate( toRadians(90.0f), vec3(1,0,0) );
        gl::drawSolidCircle(vec2(), p->radius*1.5f);
        gl::popMatrices();
    }
    gl::enableDepthWrite();
}