#include "ofApp.h"
ofApp::~ofApp()
{
    for (int i = 0; i < boids.size(); i++)
    {
        delete boids[i];
    }
}
//--------------------------------------------------------------
void ofApp::setup(){

    
    
    
    
    
    ofSetVerticalSync(true);
    
   eventString = "Alpha";
    vagRounded.load("Batang.ttf",25);
    
    blendMode = OF_BLENDMODE_ALPHA;
    ofDisableArbTex();
    
    texture.getTexture().setTextureWrap( GL_REPEAT, GL_REPEAT );
    vidGrabber.setup(640, 480, true);
    bFill       = true;
    bWireframe  = true;
    bDrawNormals= false;
    bDrawAxes   = false;
    bDrawLights = false;
    bHelpText   = true;
    bMousePressed   = false;
    bSplitFaces = false;
    float width     = ofGetWidth() * .12;
    float height    = ofGetHeight() * .12;


    plane.set( width*0.7, height*0.7 );
    box.set( width*0.5 );
    sphere.setRadius( width*0.4 );
    icoSphere.setRadius( width*0.4 );
    cylinder.set(width*.3, height*0.8);
    cone.set( width*.35, height*0.9 );

    mode = 0;

    ofSetSmoothLighting(true);
    pointLight.setDiffuseColor( ofFloatColor(.85, .85, .55) );
    pointLight.setSpecularColor( ofFloatColor(1.f, 1.f, 1.f));

    pointLight2.setDiffuseColor( ofFloatColor( 238.f/255.f, 57.f/255.f, 135.f/255.f ));
    pointLight2.setSpecularColor(ofFloatColor(.8f, .8f, .9f));

    pointLight3.setDiffuseColor( ofFloatColor(19.f/255.f,94.f/255.f,77.f/255.f) );
    pointLight3.setSpecularColor( ofFloatColor(18.f/255.f,150.f/255.f,135.f/255.f) );

    
    material.setShininess( 120 );
    // the light highlight of the material //
    material.setSpecularColor(ofColor(255, 255, 255, 255));

    ofSetSphereResolution(24);
    
   
    
    ofBackground(4, 34, 34);
	int bufferSize		= 512;
	sampleRate 			= 44100;
	phase 				= 0;
	phaseAdder 			= 0.4f;
	phaseAdderTarget 	= 0.0f;
	volume				= 0.1f;
	bNoise 				= false;
    lAudio.assign(bufferSize, 0.9);
	rAudio.assign(bufferSize, 0.0);
	soundStream.printDeviceList();

	ofSoundStreamSettings settings;

	

#ifdef TARGET_LINUX
	
	auto devices = soundStream.getMatchingDevices("default");
	if(!devices.empty()){
		settings.setOutDevice(devices[0]);
	}
#endif

	settings.setOutListener(this);
	settings.sampleRate = sampleRate;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 0;
	settings.bufferSize = bufferSize;
	soundStream.setup(settings);
    
    
    
    
    blendMode = OF_BLENDMODE_ALPHA;
    
    
    
    int screenW = ofGetScreenWidth();
    int screenH = ofGetScreenHeight();

    ofBackground(0,0,0);
    
    // set up the boids
    for (int i = 0; i < 50; i++)
        boids.push_back(new Boid());
    
    for (int i = 0; i < 50; i++)
        myboids.push_back(new Boid());
    
    quality = OF_IMAGE_QUALITY_WORST;
    maxSize = 1500;
    glitchStart = 0.1;
    reset();
    
    
    
}



void ofApp::reset() {
    generation = 0;
    
    // pick random for bleeding
    addX = ofRandom(20, 20);
    addY = ofRandom(10, 60);
    subX = ofRandom(15, addX);
    subY = ofRandom(0, addY-20);
}

//--------------------------------------------------------------
void ofApp::update(){
    pointLight.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.5)*(ofGetWidth()*.3), ofGetHeight()/2, 500);
    pointLight2.setPosition((ofGetWidth()*.5)+ cos(ofGetElapsedTimef()*.15)*(ofGetWidth()*.3),
                            ofGetHeight()*.5 + sin(ofGetElapsedTimef()*.7)*(ofGetHeight()), -300);
    
    pointLight3.setPosition(
                            cos(ofGetElapsedTimef()*1.5) * ofGetWidth()*.5,
                            sin(ofGetElapsedTimef()*1.5f) * ofGetWidth()*.5,
                            cos(ofGetElapsedTimef()*.2) * ofGetWidth()
                            );
    
    //ofSetWindowTitle("Framerate: "+ofToString(ofGetFrameRate(), 0));
    if(mode == 2 || ofGetElapsedTimef() < 10) {
        vidGrabber.update();
        
    }
    
    
    
    
    ofVec3f min(0, 0);
    ofVec3f max(ofGetWidth(), ofGetHeight());
    for (int i = 0; i < boids.size(); i++)
    {
        boids[i]->update(boids, min, max);
    }
    for (int i = 0; i < myboids.size(); i++)
    {
        myboids[i]->update(myboids, min, max);
    }
    
    
    
    string curFilename = "xingkong2.jpg";

    int size = img.getWidth();

    // keeps the image from getting too big
    if(size < maxSize) {
        img.save(curFilename, quality);

        if(ofGetKeyPressed('g')) {
            // this portion glitches the jpeg file
            // first loading the file
            ofBuffer file = ofBufferFromFile(curFilename, true);
            int fileSize = file.size();
            char * buffer = file.getData();

            // pick a byte offset that is somewhere near the end of the file
            int whichByte = (int) ofRandom(fileSize * glitchStart, fileSize);
            // and pick a bit in that byte to turn on
            int whichBit = ofRandom(8);
            char bitMask = 1 << whichBit;
            // using the OR operator
            buffer[whichByte] |= bitMask;

            // write the file out like nothing happened
            ofBufferToFile(curFilename, file, true);
            img.load(curFilename);
        } else {
            img.load(curFilename);

            // switches every other frame
            // resizing up and down breaks the 8x8 JPEG blocks
            if(ofGetFrameNum() % 2 == 0) {
                // resize a little bigger
                img.resize(size + addX, size + addY);
            } else {
                // then resize a little smaller
                img.resize(size - subX, size - subY);
            }
        }
        generation++;
    }
}
//--------------------------------------------------------------
void ofApp::draw(){
    
 
    
    // draw the original image
    ofSetColor(ofColor::white);
    img.draw(0, 0);
    
    // draw the four rectangles
    ofNoFill();
    ofSetColor(ofColor::red);
    ofDrawCircle(mouseX, mouseY, 30, 30);
    
    ofSetColor(ofColor::green);
    ofDrawRectangle(mouseX, mouseY, 40, 30);
    
    ofSetColor(ofColor::blue);
    ofDrawRectangle(mouseX + 25, mouseY + 25, 20, 20);
    
    ofSetColor(ofColor::magenta);
    ofDrawRectangle(mouseX-7, mouseY-7 , 20, 20);
    
    // draw the four corresponding subsections
 
    ofSetColor(ofColor::white);
    img.drawSubsection(0, 0, 100, 100, mouseX, mouseY);
    ofSetColor(ofColor::red);
    ofDrawRectangle(0, 0, 100, 100);
    
    ofSetColor(ofColor::white);
    img.drawSubsection(0, 100, 100, 100, mouseX, mouseY, 50, 50);
    ofSetColor(ofColor::green);
    ofDrawRectangle(0, 100, 100, 100);
    
    ofSetColor(ofColor::white);
    img.drawSubsection(0, 200, 100, 100, mouseX + 25, mouseY + 25, 50, 50);
    ofSetColor(ofColor::blue);
    ofDrawRectangle(0, 200, 100, 100);
    
    ofSetColor(ofColor::white);
    img.drawSubsection(0, 300, 100, 100, mouseX - 7, mouseY - 7, 50, 50);
    ofSetColor(ofColor::magenta);
    ofDrawRectangle(0, 300, 100, 100);
    
    
    
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetColor(265);
    ofDrawBitmapString("(s): Unpause the audio\n(e): Pause the audio\n(f): Toggle Fullscreen\n(o): Draw Solid Shapes\n(w): Draw Wireframes\n(1/2/3/4): Set Resolutions\n(n): Draw Normals\n(LEFT/RIGHT): Set Mode",30,40);
    ofDrawBitmapString("(z): Split Faces\n(a): Draw Axes\n(l): Render lights\n(h): Toggle help\n(g): Glitch",400,40);
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetHexColor(0xffffff);
    vagRounded.drawString(eventString, 98,198);
    ofSetHexColor(0xffffff);
    vagRounded.drawString(eventString, 98,198);
    ofSetColor(255,122,220);
    vagRounded.drawString(eventString, 100,200);
    ofSetHexColor(0xffffff);
    vagRounded.drawString(timeString, 98,98);
    ofSetColor(255,122,220);
    vagRounded.drawString(timeString, 100,100);
    ofSetColor(255, 255, 255,255);
    
    
    ofEnableBlendMode(blendMode);
    
    
    
    ofDisableBlendMode();
    
    
    
    
    
	
	ofNoFill();
	
	// draw the left channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(60, 150, 0);
			
		ofSetColor(265);
		ofDrawBitmapString("Left Channel", 400, 18);
		
		ofSetLineWidth(0.2);
		ofDrawRectangle(0, 0, 900, 200);

		ofSetColor(25, 218, 225);
		ofSetLineWidth(3);
					
			ofBeginShape();
			for (unsigned int i = 0; i < lAudio.size(); i++){
				float x =  ofMap(i, 0, lAudio.size(), 0, 900, true);
				ofVertex(x, 100 -lAudio[i]*180.0f);
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();

	// draw the right channel:
	ofPushStyle();
		ofPushMatrix();
		ofTranslate(60, 350, 0);
			
		ofSetColor(265);
		ofDrawBitmapString("Right Channel", 400, 18);
		
		ofSetLineWidth(1);	
		ofDrawRectangle(0, 0, 900, 200);

		ofSetColor(245, 18, 225);
		ofSetLineWidth(3);
					
			ofBeginShape();
			for (unsigned int i = 0; i < rAudio.size(); i++){
				float x =  ofMap(i, 0, rAudio.size(), 0, 900, true);
				ofVertex(x, 100 -rAudio[i]*180.0f);
			}
			ofEndShape(false);
			
		ofPopMatrix();
	ofPopStyle();
	
		
	ofSetColor(265);
	string reportString = "（-/+）: ("+ofToString(volume, 2)+") modify volume\n（x）: ("+ofToString(pan, 2)+") modify pan\n（y）: ";
	if( !bNoise ){
		reportString += "modify sine wave (" + ofToString(targetFrequency, 2) + "hz)";
	}else{
		reportString += "noise";	
	}
	ofDrawBitmapString(reportString, 32, 579);
    
    
    
    
    
    
    
    float spinX = sin(ofGetElapsedTimef()*.35f);
    float spinY = cos(ofGetElapsedTimef()*.075f);

    if (bMousePressed) {
        spinX = spinY = 0.0f;
    }

    cam.setGlobalPosition({ 0,0,cam.getImagePlaneDistance(ofGetCurrentViewport()) });
    cam.begin();

    ofEnableDepthTest();

    ofEnableLighting();
    pointLight.enable();
    pointLight2.enable();
    pointLight3.enable();



    

    if (mode == 1 || mode == 3) texture.getTexture().bind();
    if (mode == 2) vidGrabber.getTexture().bind();

    float screenWidth = ofGetWidth();
    float screenHeight = ofGetHeight();

    plane.setPosition(     -screenWidth * .5 + screenWidth *  0.8/9.f, screenHeight *  -2/5.f, 0);
    box.setPosition(       -screenWidth * .5 + screenWidth *  1.7/3.f, screenHeight *  -2/5.f, 0);
    sphere.setPosition(    -screenWidth * .5 + screenWidth *  2.7/3.f, screenHeight *  -2/5.f, 0);
    icoSphere.setPosition( -screenWidth * .5 + screenWidth *  1/4.f, screenHeight * -2/5.f, 0);
    cylinder.setPosition(  -screenWidth * .5 + screenWidth *  1.2/3.f, screenHeight * -2/5.f, 0);
    cone.setPosition(      -screenWidth * .5 + screenWidth *  2.9/4.f, screenHeight * -2/5.f, 0);

    // Plane //

    plane.rotateDeg(spinX, 1.0, 0.0, 0.0);
    plane.rotateDeg(spinY, 0, 1.0, 0.0);



    if (mode == 3) {
        deformPlane = plane.getMesh();
        // x = columns, y = rows //
        glm::vec2 planeDims = plane.getResolution();
        float planeAngleX = ofGetElapsedTimef()*3.6;
        float planeAngleInc = 3.f / (float)planeDims.x;
        glm::vec3 vert;
        for (size_t i = 0; i < deformPlane.getNumIndices(); i++) {
            planeAngleX += planeAngleInc;
            int ii = deformPlane.getIndex(i);
            vert = deformPlane.getVertex(ii);
            vert.z += cos(planeAngleX) * 50;
            deformPlane.setVertex(ii, vert);
        }
    }

    if (!bFill && bWireframe) {
        
        material.begin();
    }

    if (bFill) {
        material.begin();
        ofFill();
        if (mode == 3) {
            plane.transformGL();
            deformPlane.draw();
            plane.restoreTransformGL();
        }
        else {
            plane.draw();
        }
        material.end();
    }

    if (bWireframe) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        plane.setPosition(plane.getPosition().x, plane.getPosition().y, plane.getPosition().z + 1);
        plane.drawWireframe();
        plane.setPosition(plane.getPosition().x, plane.getPosition().y, plane.getPosition().z - 1);

    }

    // Box //

    box.rotateDeg(spinX, 1.0, 0.0, 0.0);
    box.rotateDeg(spinY, 0, 1.0, 0.0);

    if (bFill) {
        material.begin();
        ofFill();
        if (mode == 3) {
            box.transformGL();
            for (int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++) {
                ofPushMatrix();
                ofTranslate(boxSides[i].getNormal(0) * sin(ofGetElapsedTimef()) * 50);
                boxSides[i].draw();
                ofPopMatrix();
            }
            box.restoreTransformGL();
        }
        else {
            box.draw();
        }
        material.end();
    }

    if (bWireframe) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        box.setScale(1.01f);
        box.drawWireframe();
        box.setScale(1.f);
    }


    // Sphere //
    sphere.rotateDeg(spinX, 1.0, 0.0, 0.0);
    sphere.rotateDeg(spinY, 0, 1.0, 0.0);

    if (mode == 3) {
        sphere.setMode(OF_PRIMITIVE_TRIANGLES);
        triangles = sphere.getMesh().getUniqueFaces();
    }

    if (bFill) {
        material.begin();
        ofFill();
        if (mode == 3) {
            float angle = ofGetElapsedTimef()*3.2;
            float strength = (sin(angle + .25)) * .5f * 5.f;
            glm::vec3 faceNormal;
            for (size_t i = 0; i < triangles.size(); i++) {
                
                faceNormal = triangles[i].getFaceNormal();
                for (int j = 0; j < 3; j++) {
                    triangles[i].setVertex(j, triangles[i].getVertex(j) + faceNormal * strength);
                }
            }
            sphere.getMesh().setFromTriangles(triangles);
        }
        sphere.draw();
        material.end();
    }

    if (bWireframe) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        sphere.setScale(1.01f);
        sphere.drawWireframe();
        sphere.setScale(1.f);
    }


    // ICO Sphere //
    
    icoSphere.rotateDeg(spinX, 1.0, 0.0, 0.0);
    icoSphere.rotateDeg(spinY, 0, 1.0, 0.0);

    if (mode == 3) {
        triangles = icoSphere.getMesh().getUniqueFaces();
    }

    if (bFill) {
        material.begin();
        ofFill();

        if (mode == 3) {
            float angle = (ofGetElapsedTimef() * 1.4);
            glm::vec3 faceNormal;
            for (size_t i = 0; i < triangles.size(); i++) {
                float frc = ofSignedNoise(angle* (float)i * .1, angle*.05) * 4;
                faceNormal = triangles[i].getFaceNormal();
                for (int j = 0; j < 3; j++) {
                    triangles[i].setVertex(j, triangles[i].getVertex(j) + faceNormal * frc);
                }
            }
            icoSphere.getMesh().setFromTriangles(triangles);
        }

        icoSphere.draw();
        material.end();
    }

    if (bWireframe) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        icoSphere.setScale(1.01f);
        icoSphere.drawWireframe();
        icoSphere.setScale(1.f);
    }


    // Cylinder //
    if (mode == 3) {
        topCap = cylinder.getTopCapMesh();
        bottomCap = cylinder.getBottomCapMesh();
        body = cylinder.getCylinderMesh();
    }

    cylinder.rotateDeg(spinX, 1.0, 0.0, 0.0);
    cylinder.rotateDeg(spinY, 0, 1.0, 0.0);
    if (bFill) {
        material.begin();
        ofFill();
        if (mode == 3) {
            cylinder.transformGL();
            ofPushMatrix(); {
                if (topCap.getNumNormals() > 0) {
                    ofTranslate(topCap.getNormal(0) * (cos(ofGetElapsedTimef() * 5) + 1)*.5f * 100);
                    topCap.draw();
                }
            } ofPopMatrix();
            ofPushMatrix(); {
                if (bottomCap.getNumNormals() > 0) {
                    ofTranslate(bottomCap.getNormal(0) * (cos(ofGetElapsedTimef() * 4) + 1)*.5f * 100);
                    bottomCap.draw();
                }
            } ofPopMatrix();
            ofPushMatrix(); {
                float scale = (cos(ofGetElapsedTimef() * 3) + 1)*.5f + .2;
                ofScale(scale, scale, scale);
                body.draw();
            } ofPopMatrix();
            cylinder.restoreTransformGL();
        }
        else {
            cylinder.draw();
        }
        material.end();
    }

    if (bWireframe) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        cylinder.setScale(1.01f);
        cylinder.drawWireframe();
        cylinder.setScale(1.0f);
    }


    // Cone //
    cone.rotateDeg(spinX, 1.0, 0.0, 0.0);
    cone.rotateDeg(spinY, 0, 1.0, 0.0);

    if (mode == 3) {
        bottomCap = cone.getCapMesh();
        body = cone.getConeMesh();
    }
    if (bFill) {
        material.begin();
        ofFill();
        if (mode == 3) {
            cone.transformGL();
            ofPushMatrix();
            if (bottomCap.getNumNormals() > 0) {
                ofTranslate(bottomCap.getNormal(0) * cone.getHeight()*.5);
                ofRotateDeg(sin(ofGetElapsedTimef() * 5) * RAD_TO_DEG, 1, 0, 0);
                bottomCap.draw();
            }
            ofPopMatrix();

            ofPushMatrix();
            ofRotateDeg(90, 1, 0, 0);
            ofRotateDeg((cos(ofGetElapsedTimef() * 6) + 1)*.5 * 360, 1, 0, 0);
            body.draw();
            ofPopMatrix();
            cone.restoreTransformGL();
        }
        else {
            cone.draw();
        }
        material.end();
    }

    if (bWireframe) {
        ofNoFill();
        ofSetColor(0, 0, 0);
        cone.setScale(1.01f);
        cone.drawWireframe();
        cone.setScale(1.0f);
    }

    if (!bFill && bWireframe) {
        material.end();
    }

    if (mode == 1 || mode == 3) texture.getTexture().unbind();
    if (mode == 2) vidGrabber.getTexture().unbind();

    material.end();
    ofDisableLighting();

    if (bDrawLights) {
        ofFill();
        ofSetColor(pointLight.getDiffuseColor());
        pointLight.draw();
        ofSetColor(pointLight2.getDiffuseColor());
        pointLight2.draw();
        ofSetColor(pointLight3.getDiffuseColor());
        pointLight3.draw();
    }

    if (bDrawNormals) {
        ofSetColor(225, 0, 255);
        plane.drawNormals(20, bSplitFaces);
        box.drawNormals(20, bSplitFaces);
        sphere.drawNormals(20, bSplitFaces);
        icoSphere.drawNormals(20, bSplitFaces);
        cylinder.drawNormals(20, bSplitFaces);
        cone.drawNormals(20, bSplitFaces);
    }
    if (bDrawAxes) {
        plane.drawAxes(plane.getWidth()*.5 + 30);
        box.drawAxes(box.getWidth() + 30);
        sphere.drawAxes(sphere.getRadius() + 30);
        icoSphere.drawAxes(icoSphere.getRadius() + 30);
        cylinder.drawAxes(cylinder.getHeight() + 30);
        cone.drawAxes(cone.getHeight() + 30);
    }

    ofDisableDepthTest();

    ofFill();

    cam.end();
    
    ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL_BILLBOARD);

    
    if(bHelpText) {
        stringstream ss;
        ss << "FPS: " << ofToString(ofGetFrameRate(),10) << endl << endl;
        
        ofDrawBitmapStringHighlight(ss.str().c_str(), 450, 760);
    }
    
    
    
    
    for (int i = 0; i < boids.size(); i++)
    {
        boids[i]->draw();
    }
    for (int i = 0; i < myboids.size(); i++)
    {
        myboids[i]->drawnew();
    }
    
    
    

    
    

}


//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
    switch (key) {
        case 49:
            blendMode = OF_BLENDMODE_ALPHA;
            eventString = "Alpha";
            break;
        case 50:
            blendMode = OF_BLENDMODE_ADD;
            eventString = "Add";
            break;
        case 51:
            blendMode = OF_BLENDMODE_MULTIPLY;
            eventString = "Multiply";
            break;
        case 52:
            blendMode = OF_BLENDMODE_SUBTRACT;
            eventString = "Subtract";
            break;
        case 53:
            blendMode = OF_BLENDMODE_SCREEN;
            eventString = "Screen";
            break;
        default:
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case 'o':
            bFill = !bFill;
            break;
        case 'w':
            bWireframe = !bWireframe;
            break;
        case '49':
            bSplitFaces=false;
            sphere.setResolution(4);
            // icosahedron //
            icoSphere.setResolution(0); // number of subdivides //
            plane.setResolution(3, 2);
            cylinder.setResolution(4,2,0);
            cone.setResolution(4, 1, 0);
            box.setResolution(1);
            break;
        case '50':
            bSplitFaces=false;
            sphere.setResolution(8);
            icoSphere.setResolution(1);
            plane.setResolution(6, 4);
            cylinder.setResolution(8,4,1);
            cone.setResolution(7, 2, 1);
            box.setResolution(2);
            break;
        case '51':
            bSplitFaces=false;
            sphere.setResolution(16);
            icoSphere.setResolution(2);
            plane.setResolution(8,5);
            cylinder.setResolution(12, 9, 2);
            cone.setResolution(10, 5, 2);
            box.setResolution(6);
            break;
        case '52':
            bSplitFaces=false;
            sphere.setResolution(48);
            icoSphere.setResolution(4);
            plane.setResolution(12, 9);
            cylinder.setResolution(20, 13, 4);
            cone.setResolution(20, 9, 3);
            box.setResolution(10);
            break;
        case 'n':
            bDrawNormals = !bDrawNormals;
            break;
        case OF_KEY_RIGHT:
            mode++;
            if(mode > 3) mode = 0;
            if(mode==3){
                // to get unique triangles, you have to use triangles mode //
                sphere.setMode( OF_PRIMITIVE_TRIANGLES );
            }
            break;
        case OF_KEY_LEFT:
            mode--;
            if(mode < 0) mode = 3;
            if(mode==3){
                // to get unique triangles, you have to use triangles mode //
                sphere.setMode( OF_PRIMITIVE_TRIANGLES );
            }
            break;
        case 'a':
            bDrawAxes = !bDrawAxes;
            break;
        case 'l':
            bDrawLights = !bDrawLights;
            break;
        case 'h':
            bHelpText=!bHelpText;
            break;
        case 'z':
            bSplitFaces = !bSplitFaces;
            break;
    }
	if (key == '-' || key == '_' ){
		volume -= 0.05;
		volume = MAX(volume, 0);
	} else if (key == '+' || key == '=' ){
		volume += 0.05;
		volume = MIN(volume, 1);
	}
	
	if( key == 's' ){
		soundStream.start();
	}
	
	if( key == 'e' ){
		soundStream.stop();
        
        
        
        
        
        if(mode == 3) bSplitFaces = false;

        if(bSplitFaces) {
            sphere.setMode( OF_PRIMITIVE_TRIANGLES );
            vector<ofMeshFace> triangles = sphere.getMesh().getUniqueFaces();
            sphere.getMesh().setFromTriangles( triangles, true );

            icoSphere.setMode( OF_PRIMITIVE_TRIANGLES );
            triangles = icoSphere.getMesh().getUniqueFaces();
            icoSphere.getMesh().setFromTriangles(triangles, true);

            plane.setMode( OF_PRIMITIVE_TRIANGLES );
            triangles = plane.getMesh().getUniqueFaces();
            plane.getMesh().setFromTriangles(triangles, true);

            cylinder.setMode( OF_PRIMITIVE_TRIANGLES );
            triangles = cylinder.getMesh().getUniqueFaces();
            cylinder.getMesh().setFromTriangles(triangles, true);

            cone.setMode( OF_PRIMITIVE_TRIANGLES );
            triangles = cone.getMesh().getUniqueFaces();
            cone.getMesh().setFromTriangles(triangles, true);

            box.setMode( OF_PRIMITIVE_TRIANGLES );
            triangles = box.getMesh().getUniqueFaces();
            box.getMesh().setFromTriangles(triangles, true);

        } else {
            // vertex normals are calculated with creation, set resolution //
            sphere.setResolution( sphere.getResolution() );

            icoSphere.setResolution( icoSphere.getResolution() );
            plane.setResolution( plane.getNumColumns(), plane.getNumRows() );

            cylinder.setResolution( cylinder.getResolutionRadius(), cylinder.getResolutionHeight(), cylinder.getResolutionCap() );
            cone.setResolution( cone.getResolutionRadius(), cone.getResolutionHeight(), cone.getResolutionCap() );
            box.setResolution( box.getResolutionWidth() );
        }
        
}

if(mode == 1) {
    // resize the plane to the size of the texture //
    plane.resizeToTexture( texture.getTexture() );
    // setTexCoordsFromTexture sets normalized or non-normalized tex coords based on an ofTexture passed in.
    box.mapTexCoordsFromTexture( texture.getTexture() );
    sphere.mapTexCoordsFromTexture( texture.getTexture() );
    icoSphere.mapTexCoordsFromTexture( texture.getTexture() );
    cylinder.mapTexCoordsFromTexture( texture.getTexture() );
    cone.mapTexCoordsFromTexture( texture.getTexture() );
}

if(mode == 2) {
    plane.resizeToTexture( vidGrabber.getTexture(), .5 );
    box.mapTexCoordsFromTexture( vidGrabber.getTexture() );
    sphere.mapTexCoordsFromTexture( vidGrabber.getTexture() );
    icoSphere.mapTexCoordsFromTexture( vidGrabber.getTexture() );
    cylinder.mapTexCoordsFromTexture( vidGrabber.getTexture() );
    cone.mapTexCoordsFromTexture( vidGrabber.getTexture() );
}

//
if( mode == 3 ) {

    bSplitFaces = false;

    /
    sphere.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    icoSphere.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    cylinder.setMode( OF_PRIMITIVE_TRIANGLE_STRIP );
    cone.setMode( OF_PRIMITIVE_TRIANGLE_STRIP );
    
    box.setMode( OF_PRIMITIVE_TRIANGLES );

    plane.setMode( OF_PRIMITIVE_TRIANGLE_STRIP );
    plane.mapTexCoords(0, 0, 5, 5);

    // rebuild the box,
    box.mapTexCoords(0, 0, 5, 5);
    sphere.mapTexCoords(0, 0, 5, 5);
    icoSphere.mapTexCoords(0, 0, 5, 5);
    cylinder.mapTexCoords(0, 0, 5, 5);
    cone.mapTexCoords(0, 0, 5, 5);

    // store the box sides  //
    for(int i = 0; i < ofBoxPrimitive::SIDES_TOTAL; i++ ) {
        boxSides[i] = box.getSideMesh( i );
        
        
    }
        
        
    
        
	}
	
}

//--------------------------------------------------------------
void ofApp::keyReleased  (int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
	int width = ofGetWidth();
	pan = (float)x / (float)width;
	float height = (float)ofGetHeight();
	float heightPct = ((height-y) / height);
	targetFrequency = 2000.0f * heightPct;
	phaseAdderTarget = (targetFrequency / (float) sampleRate) * TWO_PI;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	int width = ofGetWidth();
	pan = (float)x / (float)width;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	bNoise = true;
    img.load("xingkong.jpg");
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	bNoise = false;
    img.load("xingkong4.jpeg");
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer & buffer){
	//pan = 0.5f;
	float leftScale = 1 - pan;
	float rightScale = pan;

	
	while (phase > TWO_PI){
		phase -= TWO_PI;
	}

	if ( bNoise == true){
		// ---------------------- noise --------------
		for (size_t i = 0; i < buffer.getNumFrames(); i++){
			lAudio[i] = buffer[i*buffer.getNumChannels()    ] = ofRandom(0, 1) * volume * leftScale;
			rAudio[i] = buffer[i*buffer.getNumChannels() + 1] = ofRandom(0, 1) * volume * rightScale;
		}
	} else {
		phaseAdder = 0.95f * phaseAdder + 0.05f * phaseAdderTarget;
		for (size_t i = 0; i < buffer.getNumFrames(); i++){
			phase += phaseAdder;
			float sample = sin(phase);
			lAudio[i] = buffer[i*buffer.getNumChannels()    ] = sample * volume * leftScale;
			rAudio[i] = buffer[i*buffer.getNumChannels() + 1] = sample * volume * rightScale;
            
            
            
		}
	}

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
