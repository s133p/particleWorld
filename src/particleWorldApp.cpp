#include <list>

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
    
    list<vec2> instinctPoints;
    list<vec2> voidPoints;
};

void particleWorldApp::setup()
{
    cam = CameraPersp(1280, 800, 35);
    
    voidImg = loadImage( loadAsset("void.png") );
    instinctImg = loadImage( loadAsset("instinct.png") );
    if (instinctImg.hasAlpha()) console() << "has alpha!" <<endl;
    
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
    
    int i = 0;
    for (auto it = voidPoints.begin(); it != voidPoints.end();)
    {
        if(i % 2 == 0) it = voidPoints.erase(it);
        else it++;
        i++;
    }
    
    //list<vec2> instinctPoints;
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
    console() << voidPoints.size();
    for (int i = 1; i < 2000; i++)
    {
        int z = 0;
        int randPoint = randInt(0, voidPoints.size());
        vec2 rp;
        for (auto it : voidPoints) { z++; if(z==randPoint) {rp=it;break;};}
        particles.particleArray[i].position = vec3(rp.x, -rp.y, randInt(-50, 50));
        particles.particleArray[i].prevPosition = particles.particleArray[i].position;
        particles.particleArray[i].drawing = true;
        particles.particleArray[i].moving = false;
    }
    gl::enableDepthWrite();
    gl::enableDepthRead();
}

void particleWorldApp::mouseDown( MouseEvent event )
{
    for (int i = 1; i < 2000; i++)
    {
        particles.particleArray[i].moving = true;
    }
    
    if (getElapsedSeconds() > 18)
    {
        particles.test->running = !particles.test->running;
    }
}

void particleWorldApp::update()
{
    if (getElapsedFrames() % 550 == 0)
    {
        for(auto & sp : particles.spTest->springs)
        {
            int z = 0;
            int randPoint = randInt(0, instinctPoints.size());
            vec2 rp;
            for (auto it : instinctPoints) { z++; if(z==randPoint) {rp=it;break;};}
            sp.b->position = vec3(rp.x, -rp.y, randInt(-50, 50));
            sp.b->moving = false;
            sp.d = 0;
        }
    }
    
    if (getElapsedFrames() % 1200 == 0)
    {
        for(auto & sp : particles.spTest->springs)
        {
            int z = 0;
            int randPoint = randInt(0, voidPoints.size());
            vec2 rp;
            for (auto it : voidPoints) { z++; if(z==randPoint) {rp=it;break;};}
            sp.b->position = vec3(rp.x, -rp.y, randInt(-50, 50));
            sp.b->moving = false;
            sp.d = 0;
        }
    }
    
    particles.update();
}

void particleWorldApp::draw()
{
    gl::setMatrices(cam);
    gl::clear( Color( .98, .98, .98 ) );
    gl::enableDepthRead();
    gl::enableDepthWrite();
    
    
    //gl::color(.05, .05, .05);
    gl::translate(getWindowWidth()/2, getWindowHeight()/2, 0);
    gl::rotate(toRadians(app::getElapsedSeconds()*20.0f), vec3(0,1,0));
    
    particles.draw();
    
    /*gl::enableAlphaBlending();
    gl::color(1, 1, 1);
    //gl::disableDepthWrite();
    gl::pushMatrices();
    gl::setMatricesWindow( getWindowSize() );
    gl::draw( voidTex );
    gl::disableAlphaBlending();
    gl::popMatrices();*/
}

CINDER_APP( particleWorldApp, RendererGl( RendererGl::Options().msaa(4) ), [&]( App::Settings *settings ) {
    
    settings->setWindowSize(1280, 800);
    //settings->setHighDensityDisplayEnabled();
    settings->setFrameRate(60.0);
    
} )