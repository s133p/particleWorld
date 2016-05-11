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
    
    std::vector<vec4> positions;
    
    //Set up Initial state
    for (int i=0; i < MAX_PARTICLES; i++)
    {
        particleArray[i].position = vec3();
        particleArray[i].prevPosition = vec3();
        particleArray[i].moving = false;
        particleArray[i].drawing = false;
        
        inactiveParticles.push_back(&particleArray[i]);
        vec4 p = vec4(particleArray[i].position, particleArray[i].radius);
        positions.push_back( p );
    }

    //Move single particle from inactive to active
    for (int i = 0; i < 10; i++)
    {
        activeParticles.push_front(inactiveParticles.front());
        activeParticles.front()->radius = 32.0f;
        activeParticles.front()->position = vec3((i*98.0f)-(98.0f*5.0f), 0.0f, -170.0f);
        transitionParticles.push_front(activeParticles.front());
        inactiveParticles.pop_front();
    }
    
    for (auto it : activeParticles)
    {
        //it->position = vec3(0,0, -200);
        it->prevPosition = it->position+ randVec3()*vec3(4.5f);
        it->drawing = true;
        it->radius = 48.0f;
        it->moving = true;
        
        timeline().applyPtr( &(it->radius), 2.0f, 2.5f, EaseInAtan() );
    }
    
    spTest = new springMotion();
    
    //INSTANCTED DRAWING setup
	shader = gl::GlslProg::create(loadAsset("shader.vert"), loadAsset("shader.frag"));
    gl::VboMeshRef mesh = gl::VboMesh::create( geom::Cube() >> geom::Scale(2.0) ) ;
    
    mInstanceDataVbo = gl::Vbo::create( GL_ARRAY_BUFFER, positions.size() * sizeof(vec4), positions.data(), GL_STREAM_DRAW );
    geom::BufferLayout instanceDataLayout;
    instanceDataLayout.append( geom::Attrib::CUSTOM_0, 4, sizeof(vec4), 0, 1 /* per instance */ );
    mesh->appendVbo( instanceDataLayout, mInstanceDataVbo );
    mBox = gl::Batch::create( mesh, shader, { { geom::Attrib::CUSTOM_0, "vInstancePosition" } } );
}

void particleController::update()
{
    if(getElapsedFrames() > 150-15 && getElapsedFrames()%16 == 0 && inactiveParticles.size() > 0 && getElapsedFrames() < 32*12)
    {
        list<particle*> tempParticles;
        while (transitionParticles.size() > 0 )
        {
            particle * p = transitionParticles.front();
            for (int i = 0; i < 2 && inactiveParticles.size() > 0; i++)
            {
                activeParticles.push_front( inactiveParticles.front() );
                activeParticles.front()->radius = 2.0f;
                activeParticles.front()->position = p->position + p->velocity*randFloat(.9, 1.9);
                activeParticles.front()->prevPosition = p->position+ randVec3();
                activeParticles.front()->drawing = true;
                activeParticles.front()->moving= true;
                spTest->addSpring( p, activeParticles.front() );
                tempParticles.push_front(activeParticles.front());
                inactiveParticles.pop_front();
            }
            transitionParticles.pop_front();
        }
        for (auto it : tempParticles)
        {
            transitionParticles.push_front(it);
        }
    }
    
    //if (getElapsedFrames() < 32*40)
    //{
        spTest->update(.95);
    //}
    
    if ( getElapsedFrames() == 32*12 )
    {
        test = new flockingMotion(activeParticles);
    }
    
    if (getElapsedFrames() > 32*12 && getElapsedFrames() < 32*40)
    {
        test->update(0.25f);
    }
    
    
    if (getElapsedFrames() > 32*40 && activeParticles.size() > 1)
    {
        inactiveParticles.push_front( activeParticles.front() );
        activeParticles.pop_front();
        
        if (activeParticles.size() > 238)
        {
            inactiveParticles.push_front( activeParticles.front() );
            activeParticles.pop_front();
            inactiveParticles.push_front( activeParticles.front() );
            activeParticles.pop_front();
            inactiveParticles.push_front( activeParticles.front() );
            activeParticles.pop_front();
        }
    }
    if (activeParticles.size() == 228)
    {
        timeline().applyPtr( &(activeParticles.back()->position), vec3(), 1.75f, EaseInAtan() );
        timeline().applyPtr( &(activeParticles.back()->radius), 82.0f, 2.0f, EaseInAtan() );
        timeline().appendToPtr( &(activeParticles.back()->radius), 0.1f, .85f, EaseInAtan() );
    }
    
    vec4 *positions = (vec4*)mInstanceDataVbo->map(GL_WRITE_ONLY);
    
    for (auto it : activeParticles)
    {
        if (it->moving)
        {
            it->addForce( noise.dfBm(it->position*0.06f) * 0.045f );
            it->update();
            *positions = vec4(it->position, it->radius);
            
            positions++;
        }
    }
    mInstanceDataVbo->unmap();
}

void particleController::draw()
{
    gl::color(.4, .4, .4);
    spTest->draw();
    
    mBox->drawInstanced( activeParticles.size() );
}






