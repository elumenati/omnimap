#include "testApp.h"
#include "IPCDomeView.h"

HINSTANCE instance = NULL;
IPCDomeView *domeViewIPC = NULL;


//--------------------------------------------------------------
void testApp::setup(){	 

	ofBackground(0,0,0);
	ofSetFrameRate(30);
	g_speed = 0.1;
  for(int i = 0; i < MAX_STARS; i++)	{
    stars[i].g_speed = &g_speed;
  }


///////////// DOMEVIEW SDK INIT //////////////////////
  instance = LoadLibrary(TEXT("IPCDomeView.dll")); 

  if (instance != NULL) { 
    IPCDomeViewBuilderFUNC IPCDomeViewBuilder = (IPCDomeViewBuilderFUNC) GetProcAddress(instance, "IPCDomeViewBuilder");
    domeViewIPC = IPCDomeViewBuilder(IPC_DomeViewPlugin_Dummy);
// SEE MORE DEFINES ON IPCPlugin.h
  } else {
    ExitProcess(0);
  }
///////////// DOMEVIEW SDK INIT //////////////////////
}



//--------------------------------------------------------------
void testApp::clean() {
///////////// DOMEVIEW SDK CLEAN //////////////////////
  domeViewIPC->release(); 
  domeViewIPC = NULL; 
  FreeLibrary(instance);
///////////// DOMEVIEW SDK CLEAN //////////////////////
}



//--------------------------------------------------------------
void testApp::update(){
	for(int i=0; i < MAX_STARS; i++){
		stars[i].update();	
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){ 
  camera(0.0, 0.0, 0.0, ofGetWidth(), ofGetHeight());

///////////// DOMEVIEW SDK SEND FRAME //////////////////////
  if (domeViewIPC->isFree(ofGetWidth(), ofGetHeight(), Format_RGBA)) {
    domeViewIPC->getIPCPlugin()->setParamsFilled(PARAMS_FILL_DISCARD);
    domeViewIPC->getIPCPlugin()->setProjection(PROJECTION_AzimuthalEquidistant);
    domeViewIPC->getIPCPlugin()->setMirror(MIRROR_X);
    domeViewIPC->getIPCPlugin()->setAlt(90.0);
    domeViewIPC->getIPCPlugin()->setAzi(0.0);
    domeViewIPC->getIPCPlugin()->setRotation(0);
    domeViewIPC->getIPCPlugin()->setAngularWidth(180.0);
    domeViewIPC->getIPCPlugin()->setAngularHeight(180.0);
    void *pixel = domeViewIPC->getBuffer();
    glReadPixels(0, 0, ofGetWidth(), ofGetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    domeViewIPC->done();
  }
///////////// DOMEVIEW SDK SEND FRAME //////////////////////
}



void testApp::camera(float rotx, float roty, float rotz, int vx, int vy){

	float fov = 90.0;//150.f;//120.f;//90.f;
	float ratio = vx/vy;
	float z = 1.0f;
	float Z = 20000.0f;

  glViewport(0, 0, vx, vy);
  glClear(GL_COLOR_BUFFER_BIT);
  ofBackground(0,0,0);
  glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ratio, z, Z);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

  glRotatef(rotz, 0.0, 0.0, 1.0);
  glRotatef(roty, 0.0, 1.0, 0.0);
  glRotatef(rotx, 1.0, 0.0, 0.0);

//	gluLookAt(w/2.f, h/2.f, 100.f,		w/2.f, h/2.f,0.f,		0.,1.,0.);
//	gluLookAt(0.f, 0.f, 100.f,		0.f, 0.f ,0.f,		0.,1.,0.);
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
    
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){ 
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

//  g_speed = map(x,0,ofGetWidth(), 0.1 , 250. );

  if (x == 0) g_speed = 0.1;
  else if (x == ofGetWidth()) g_speed = 250.0;
  else g_speed = 0.1 + ((double)x / ofGetWidth()) * (250.0-0.1);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
}
