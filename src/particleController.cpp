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
    for (int i = 0; i < 1; i++)
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
        it->radius = 5.0f;
        it->moving = true;
        
        timeline().applyPtr( &(it->radius), 60.0f, 2.5f, EaseInAtan() );
                            //&(it->radius), 20.0f, 2.5f, EaseInAtan() );
    }
    
	test = nullptr;
    spTest = new springMotion();
    
    //INSTANCTED DRAWING setup
	shader = gl::GlslProg::create(loadAsset("shader.vert"), loadAsset("shader.frag"));
    gl::VboMeshRef mesh = gl::VboMesh::create( geom::Sphere().subdivisions(32) >> geom::Scale(2.0) ) ;
    
    mInstanceDataVbo = gl::Vbo::create( GL_ARRAY_BUFFER, positions.size() * sizeof(vec4), positions.data(), GL_DYNAMIC_DRAW );
    geom::BufferLayout instanceDataLayout;
    instanceDataLayout.append( geom::Attrib::CUSTOM_0, 4, sizeof(vec4), 0, 1 /* per instance */ );
    mesh->appendVbo( instanceDataLayout, mInstanceDataVbo );
    mBox = gl::Batch::create( mesh, shader, { { geom::Attrib::CUSTOM_0, "vInstancePosition" } } );
}

void particleController::update()
{
    if(getElapsedFrames() > 150-15 && getElapsedFrames()%8 == 0 && inactiveParticles.size() > 0 && getElapsedFrames() < 32*14)
    {
        list<particle*> tempParticles;
        int count = 0;
        while (transitionParticles.size() > 0 && ++count < 80)
        {
            particle * p = transitionParticles.front();
            int min = 2 >= activeParticles.size()/4? activeParticles.size()/4 : 2;
            int numSplits = 1 + min;//ci::randInt(1,4);
            for (int i = 0; i < numSplits && inactiveParticles.size() > 0; ++i)
            {
                activeParticles.push_front( inactiveParticles.front() );
                if (p->radius * 0.64f > 1.1)
                    activeParticles.front()->radius = p->radius * 0.64f;
                else
                    activeParticles.front()->radius = p->radius;
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
            transitionParticles.push_back(it);
        }
    }
    
    //if (getElapsedFrames() < 32*40)
    //{
        spTest->update(.95);
    //}
    
    if ( getElapsedFrames() == 32*14 && !test )
    {
        test = new flockingMotion(activeParticles);
    }
    
    if (test && getElapsedFrames() < 32*80)
    {
        test->update(0.8f);
    }
    
    if (getElapsedFrames() == (32*80)){
        spTest->springs.clear();
    }
    
    if (getElapsedFrames() == 32*30){
        spTest->springs.clear();
    }
    if (getElapsedFrames() > 32*38 && getElapsedFrames() < 32*70 && getElapsedFrames()%2 == 0){
        for (int i = 0; i < 2 + (getElapsedFrames()/(32*38)); ++i){
            uint32_t a = randInt(activeParticles.size());
            uint32_t b = randInt(activeParticles.size());
            auto it = find(begin(spTest->particles), end(spTest->particles), &particleArray[a]);
            auto it2 = find(begin(spTest->particles), end(spTest->particles), &particleArray[b]);
            if (it == end(spTest->particles) || it2 == end(spTest->particles)){
                if (a != b)
                    spTest->addSpring(&particleArray[a], &particleArray[b]);
            }
        }
    }
    
    vec4 *positions = (vec4*)mInstanceDataVbo->map(GL_WRITE_ONLY);
    
    for (auto it : activeParticles)
    {
        if (it->moving)
        {
            if (app::getElapsedFrames() <  32*84) it->addForce( noise.dfBm(it->position*0.06f) * 0.145f );
            it->update();
            *positions = vec4(it->position, it->radius);
            
            positions++;
        }
    }
    
    if (app::getElapsedFrames() ==  32*116){
        activeParticles.clear();
        activeParticles.push_back(&particleArray[0]);
        timeline().applyPtr(&(activeParticles.front()->radius), 0.1f, 1.0f, EaseInQuad());
    }
        //activeParticles.clear();
    
    mInstanceDataVbo->unmap();
}

void particleController::draw()
{
    gl::translate(vec3(0,50,0));
    gl::color(.2, .2, .2);
    spTest->draw();
    
    mBox->drawInstanced( activeParticles.size() );
    if (test)
        test->motion::draw();
}






