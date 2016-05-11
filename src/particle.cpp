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
    
    mass = radius*.4;
    
    velocity = position - prevPosition;
    prevPosition = position;
    
    force /= mass;
    //force += vec3(0, -.05f, 0);
    if (length2(force) > .1*.1) force = normalize(force)*0.1f; // limit force
    
    vec3 targetVelocity = velocity;
    
    targetVelocity += force;
    velocity = lerp(velocity, targetVelocity, lerpControl);
    velocity *= .998;
    if (length2(velocity) > 3*3) velocity = normalize(velocity)*3.0f;
    
    position += velocity;
    
    position.y = max(position.y, -380.0f); //Floor
    position.y = min(position.y, 380.0f); //Ceil
    
    position.x = max(position.x, -620.0f); //Floor
    position.x = min(position.x, 620.0f); //Ceil
    
    position.z = max(position.z, -440.0f); //Floor
    position.z = min(position.z, 240.0f); //Ceil
    
    force = vec3();
}

void particle::draw()
{
    if (!drawing) return;
    
    gl::drawCube(position, vec3(30));
}