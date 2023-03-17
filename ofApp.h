#pragma once

#include "ofMain.h"
#include <vector>
#include "boid.h"

class ofApp : public ofBaseApp{

	public:
    ~ofApp();
    void reset();
    
    
		void setup();
		void update();
		void draw();

		void keyPressed  (int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		void audioOut(ofSoundBuffer & buffer);
		
		
		ofSoundStream soundStream;

		float 	pan;
		int		sampleRate;
		bool 	bNoise;
		float 	volume;

		vector <float> lAudio;
		vector <float> rAudio;
		
		//------------------- for the simple sine wave synthesis
		float 	targetFrequency;
		float 	phase;
		float 	phaseAdder;
		float 	phaseAdderTarget;
    
    
    
    
    bool bFill;
    bool bWireframe;
    bool bDrawNormals;
    bool bDrawAxes;
    bool bDrawLights;
    bool bHelpText;
    bool bMousePressed;
    bool bSplitFaces;
    
    
    
    ofBlendMode blendMode;
    ofImage xingkong;
    ofTrueTypeFont     vagRounded;
    string eventString;
    string timeString;
    
    ofImage texture;
    ofImage img;
    ofVideoGrabber vidGrabber;
    int mode;

    ofSpherePrimitive sphere;
    ofIcoSpherePrimitive icoSphere;
    ofPlanePrimitive plane;
    ofCylinderPrimitive cylinder;
    ofConePrimitive cone;
    ofBoxPrimitive box;

    ofLight pointLight;
    ofLight pointLight2;
    ofLight pointLight3;
    ofMaterial material;

    // place to store the sides of the box //
    ofVboMesh boxSides[ofBoxPrimitive::SIDES_TOTAL];
    ofVboMesh deformPlane;
    ofVboMesh topCap, bottomCap, body;
    vector<ofMeshFace> triangles;

    ofCamera cam;
    
    std::vector<Boid *> boids;
    std::vector<Boid *> myboids;
    
    
    
    int maxSize;
    int generation;
    ofImageQualityType quality;
    int addX, addY, subX, subY;
    float glitchStart;
};
