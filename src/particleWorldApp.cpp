#include <list>
#include <vector>

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/Rand.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"

#include "particleController.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;

class particleWorldApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
    
    particleController particles;
    CameraPersp cam;
    Surface voidImg, instinctImg;
    gl::Texture2dRef voidTex;
    
    vector<vec2> instinctPoints;
    vector<vec2> voidPoints;
    gl::GlslProgRef shader;
    bool running;
};

void particleWorldApp::setup()
{
    running = false;
    
    shader = gl::GlslProg::create(  loadResource( "shader.vert" ), loadResource( "shader.frag" ) );
    
    cam = CameraPersp(1280, 800, 35);
    cam.setFarClip(20000);
    
    voidImg = loadImage( loadResource("void.png") );
    instinctImg = loadImage( loadResource("instinct.png") );
    
    Surface::Iter iter = voidImg.getIter();
    while( iter.line() ) {
        while( iter.pixel() ) {
            if (iter.a() > 128)
            {
                vec2 p = iter.getPos();
                p -= vec2((int)(1280/2), (int)(800/2));
                voidPoints.push_back(p);
            }
        }
    }
    
    iter = instinctImg.getIter();
    while( iter.line() ) {
        while( iter.pixel() ) {
            if (iter.a() > 128)
            {
                vec2 p = iter.getPos();
                p -= vec2((int)(1280/2), (int)(800/2));
                instinctPoints.push_back(p);
            }
        }
    }
    
    
    for (int i = 1; i < MAX_PARTICLES-100; i++)
    {
        //int randPoint = randInt(0, voidPoints.size());
        //vec2 rp = voidPoints[randPoint];
        //particles.particleArray[i].position = vec3(rp.x, -rp.y, randInt(-50, 50));
        //particles.particleArray[i].prevPosition = particles.particleArray[i].position;
        particles.particleArray[i].drawing = true;
        particles.particleArray[i].moving = true;
    }
    gl::enableDepthWrite();
    gl::enableDepthRead();
}

void particleWorldApp::mouseDown( MouseEvent event )
{
    
    if (getElapsedSeconds() > 1)
    {
        running = !running;
        //particles.test->running = !particles.test->running;
    }
}

void particleWorldApp::update()
{
    /*if (getElapsedFrames() % 550 == 0)
    {
        for(auto & sp : particles.spTest->springs)
        {
            int randPoint = randInt(0, instinctPoints.size());
            vec2 rp = instinctPoints[randPoint];
            sp.b->position = vec3(rp.x, -rp.y, randInt(-10, 10));
            sp.b->moving = false;
            sp.d = 0;
        }
    }
    
    if (getElapsedFrames() % 1200 == 0)
    {
        for(auto & sp : particles.spTest->springs)
        {
            int randPoint = randInt(0, voidPoints.size());
            vec2 rp = voidPoints[randPoint];
            sp.b->position = vec3(rp.x, -rp.y, randInt(-20, 20));
            sp.b->moving = false;
            sp.d = 0;
        }
    }*/
    if (running)
    {
        particles.update();
    }
}

void particleWorldApp::draw()
{
    gl::setMatrices(cam);
    gl::clear( Color( .98, .98, .98 ) );
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
    gl::translate(getWindowWidth()/2, getWindowHeight()/2, -400);
    //gl::rotate(toRadians(app::getElapsedSeconds()*5.0f), vec3(0,1,0));
    
    gl::pushMatrices();
    shader->bind();
    gl::translate(0,-getWindowHeight()/2, 0);
    gl::color(.75, .75, .75);
    gl::disableDepthWrite();
    gl::drawCube(vec3(), vec3(20000, 5,20000));
    gl::enableDepthWrite();
    gl::popMatrices();
    //shader->
    
    particles.draw();
}

CINDER_APP( particleWorldApp, RendererGl( RendererGl::Options().msaa(4) ), [&]( App::Settings *settings ) {
    
    settings->setWindowSize(1280, 800);
    settings->setHighDensityDisplayEnabled();
    settings->setFrameRate(60.0);
    
} )