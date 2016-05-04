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
    for (auto it = availableParticles.begin(); it != availableParticles.end();)
    {
        particles.push_back(*it);
        //it = availableParticles.erase(it);
        it++;
    }
}

void motion::update()
{
    for (auto p : particles)
    {
        p->update();
    }
}

void motion::draw()
{
    for (auto p : particles)
    {
        p->draw();
    }
}