//
//  springMotion.cpp
//  particleWorld
//
//  Created by Luke Purcell on 5/3/16.
//
//

#include "springMotion.hpp"


springMotion::springMotion()
{
    
}

springMotion::springMotion(list<particle*> & availableParticles) : motion(availableParticles)
{
    drawing = true;
    
    int count = 0;
    particle * center = new particle;
    center->position = vec3()-vec3(0,0,0);
    center->moving = false;
    
    for (auto it = particles.begin(); it != particles.end(); it++)
    {
        /*count++;
        if (count > particles.size()/2)
        {
            center = new particle;
            center->position = vec3()+vec3(650,0,0);
            center->moving = false;
            count = -particles.size();
        }*/
        particle * p = *it;
        spring s( (center), (p) );
        springs.push_back(s);
        /*for (int i = 0; i < 20; i++)
        {
            count++;
            it++;
            if (it == particles.end()) return;
            spring s( p, (*it) );
            p = *it;
            springs.push_back(s);
        }*/
    }
    //console() << springs.size() << endl;
}

void springMotion::update(float forceScale)
{
    if (!running) return;
    
    motion::saveForce();
    
    for (auto & s : springs)
    {
        s.update(1.0);
    }
    
    motion::update(forceScale);
    
    motion::avgForce();
}

void springMotion::draw()
{
    
    if (!drawing) return;
    /*gl::color( 1, .514, .184);
    int count = 0;
    for (auto & s : springs)
    {
        //if (count > springs.size()/2) gl::color( .184, .671, 1.0);
        s.draw();
        //count++;
    }*/
    motion::draw();
    
}
