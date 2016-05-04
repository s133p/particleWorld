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
    
    particle * p = new particle;
    p->position = vec3();
    p->moving = false;
    
    for (auto it = particles.begin(); it != particles.end(); it++)
    {
        /*auto it2 = it;
        for (++it2; it2 != particles.end(); it2++)
        {
            vec3 d = (*it)->position - (*it2)->position;
            float l = length2(d);
            if (l < 200*200 && randInt(0,100) > 90)
            {*/
                spring s( (*it), (p) );
                springs.push_back(s);
          //  }
        //}
    }
}

void springMotion::update()
{
    
    for (int i = 0; i < 1; i ++)
    {
        //std::random_shuffle ( springs.begin(), springs.end() );
        for (auto s : springs)
        {
            s.update(2.0);
        }
        motion::update();
    }
}

void springMotion::draw()
{
    gl::enableAdditiveBlending();
    gl::color(.1, .1, .1);
    for (auto s : springs)
    {
        s.draw();
    }
    gl::disableBlending();
    gl::color(.9, .9, .9);
    motion::draw();
}
