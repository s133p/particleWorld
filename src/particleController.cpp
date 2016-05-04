//
//  particleController.cpp
//  particleWorld
//
//  Created by Luke Purcell on 5/1/16.
//
//

#include "particleController.hpp"


particleController::particleController()
{
    noise = Perlin(4, 123123);
    
    //Set center Particle
    particleArray[0].position = vec3();
    particleArray[0].moving = false;
    particleArray[0].radius = 10;
    activeParticles.push_back(&particleArray[0]);
    
    int i = 1;
    for (; i < 1/*MAX_PARTICLES/2*/; i++)
    {
        vec4 circ = vec4(0,1, 0, 0);
        mat4 ex = rotate( toRadians(i*(360/(float)MAX_PARTICLES )), vec3( 0, 0, 1 )  );
        ex = scale(ex, vec3(200,200, 1));
        circ = circ * ex;
        
        particleArray[i].position = vec3(circ.x, circ.y, randInt(0, 20));
        particleArray[i].prevPosition = particleArray[i].position+(randVec3()*vec3(1.0));
        particleArray[i].moving = true;
        activeParticles.push_back(&particleArray[i]);
    }
    for (; i < MAX_PARTICLES; i++)
    {
        vec4 circ = vec4(0,1, 0, 0);
        mat4 ex = rotate( toRadians(i*(360/(float)MAX_PARTICLES )), vec3( 0, 0, 1 )  );
        ex = scale(ex, vec3(200,200, 1));
        circ = circ * ex;
        
        particleArray[i].position = vec3(circ.x, circ.y, randInt(0, 100));
        particleArray[i].prevPosition = particleArray[i].position+(randVec3()*vec3(2.0));
        particleArray[i].moving = true;
        if (randInt(0, 1000) > 990)
            particleArray[i].moving = false;
        
        inactiveParticles.push_back(&particleArray[i]);
    }
    
    test = new flockingMotion(inactiveParticles);
    spTest = NULL;// new springMotion(inactiveParticles);
    
    console() << inactiveParticles.size() << endl;
}

void particleController::update()
{
    for (auto it : activeParticles)
    {
        if (it->moving)
        {
            it->addForce( noise.dfBm(it->position*0.005f) );
            it->force *= .1f;
            //it->update();
        }
    }
    
    //if (test != NULL && getElapsedSeconds() < 2.0)
    //{
        test->update();
    //}else if (test != NULL && test->particles.size() > 0)
    //{
    //    spTest = new springMotion(test->particles);
    //    test = NULL;
    //}else if (spTest != NULL){
    if (getElapsedSeconds() > 4.0 && spTest == NULL)
        spTest = new springMotion(inactiveParticles);
    else if (getElapsedSeconds() > 3.0 && spTest != NULL)
        spTest->update();
    //}
}

void particleController::draw()
{
    gl::color(.8, .8, .8);
    /*for (auto it : activeParticles)
    {
        if (it->drawing)
            it->draw();
    }*/
    if (getElapsedSeconds() < 6)
        test->draw();
   if (spTest != NULL)
        spTest->draw();
    /*else
    {
        for (auto it : inactiveParticles)
        {
            if (it->drawing)
                it->draw();
        }
    }*/
}