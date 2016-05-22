//
//  particle.cpp
//  particleWorld
//
//  Created by Luke Purcell on 5/1/16.
//
//

#include "particle.hpp"


particle::particle()
{
    position = vec3(0, 0, 0);
    radius = randFloat(1.0,2.5);
    mass = radius*.4;
    
    moving = true;
    drawing = true;
    lerpControl = 0.95f;
    neighbors = 0;
}

void particle::addForce(vec3 f)
{
    force += f;
}

void particle::update()
{
    if (!moving) return;
    
    mass = (radius*radius)/1.5f;
    
    velocity = position - prevPosition;
    prevPosition = position;
    
    force /= mass;
    force += vec3(0, -.1f, 0);
    //if (length2(force) > .2*.2) force = normalize(force)*0.2f; // limit force

    
    velocity += force;
    //velocity = lerp(velocity, targetVelocity, lerpControl);
    velocity = velocity * .99f;
    if (length2(velocity) > 9*9) velocity = normalize(velocity)*9.0f;
    
    position += velocity;
    
    /*position.y = glm::max(position.y, -380.0f); //Floor
    position.y = glm::min(position.y, 380.0f); //Ceil
    
    position.x = glm::max(position.x, -620.0f); //wall left
    position.x = glm::min(position.x, 620.0f); //wall right
    
    position.z = glm::max(position.z, -440.0f); //wall back
    position.z = glm::min(position.z, 240.0f); //wall front
    */
    if(length2(position) > 400.0f*400.0f)
        position = normalize(position)*400.0f;
     
    force = vec3(0);
}

void particle::draw()
{
    if (!drawing) return;
    
    gl::drawCube(position, vec3(30));
}