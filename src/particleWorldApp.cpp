#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class particleWorldApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void particleWorldApp::setup()
{
}

void particleWorldApp::mouseDown( MouseEvent event )
{
}

void particleWorldApp::update()
{
}

void particleWorldApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( particleWorldApp, RendererGl )
