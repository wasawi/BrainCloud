#include "vizManager.h"


//----------------------------------------------
vizManager::vizManager(){
	
}

//----------------------------------------------
vizManager::~vizManager(){
	delete gui1;
	delete gui2;
}

//--------------------------------------------------------------
void vizManager::setup(){

	// TODO: Remove this from here
	ofEnableSmoothing();
	
	//GUI
	setup_GUI1();
	gui1->loadSettings("GUI/viz_settings.xml");
	gui1->setDrawBack(true);
//	gui1->setVisible(false);

	//camera
	loadCameraPosition();
	bcameraMode = true;
	cam.disableMouseInput();
	
	//------------ Volumetrics setup ---------------//
	//----------------------------------------------//
	imageSequence.init("volumes/Colin27T1_tight/IM-0001-0",3,".tif", 1);
	
	volWidth	= imageSequence.getWidth();
    volHeight	= imageSequence.getHeight();
    volDepth	= imageSequence.getSequenceLength();
	//add one morw pixel in width and height..
	//i dont know why some tifs are not reading well...
	//maybe is related to th texture which has to be a power of 2..
//	int volWidth	= 151;
//	int volHeight	= 188;

//	int volWidth	= volWidth;
//	int volHeight	= volHeight;
	
    cout << "setting up volume data buffer at " << volWidth << "x" << volHeight << "x" << volDepth <<"\n";
    volumeData = new unsigned char[volWidth*volHeight*volDepth];
	
	//fill out the added pixel in 0
	for (int i=0; i<volWidth*volHeight*volDepth; i++ ){
		volumeData[i]= (unsigned char) 0;
	}
	
	// fill my array with pixels
    for(int z=0; z<volDepth; z++)
    {
        imageSequence.loadFrame(z);
		int gradient = 0;
		for(int y=0; y<volHeight; y++)
        {
			for(int x=0; x<volWidth; x++)
			{
				if (x<volWidth && y<volHeight)
				{																// get values from image
					int i = ((x + volWidth*y) + z*volWidth*volHeight);			// the pointer position at Array
					int sample = imageSequence.getPixels()[x+y*volWidth];		// the pixel on the image
					volumeData[i] = sample;
				}
            }
        }
    }
	
	// Init Volume
    myVolume.setup(volWidth, volHeight, volDepth, ofVec3f(1,1,1));
	myVolume.updateVolumeData(volumeData, volWidth, volHeight, volDepth, 0, 0, 0);
    myVolume.setRenderSettings(FBOq, Zq, density, thresh);
	myVolume.setVolumeTextureFilterMode(GL_LINEAR);
	
	// Init Slices
	sagittal.setup(volumeData, volWidth, volHeight, volDepth, SAGITTAL);
	axial.setup(volumeData, volWidth, volHeight, volDepth, AXIAL);
	coronal.setup(volumeData, volWidth, volHeight, volDepth, CORONAL);
	sagittalS = axialS = coronalS = 20;
	//fbo
	myfboRender = myVolume.getFboReference();
	
	
	//3d Views
	bDraw = true;
	setup_GUI2();
	gui2->loadSettings("GUI/viz_settings_2.xml");
	gui2->setDrawBack(false);
//	gui2->setAutoDraw(true);
//	gui2->setVisible(false);
}


//--------------------------------------------------------------
void vizManager::update(){

}


//--------------------------------------------------------------
void vizManager::draw(){
if (bDraw){
	ofSetColor(0);
	
	float boxW = 200;
	float boxH = 200;
	int dist = 50;
	int slider = 50;
	// needed to align the volume at the center of the box
	int aX = (boxW - volHeight) /2;
	int aY = (boxH - volDepth) /2;
	int aZ = (boxH - volWidth) /2;

	// Draw Boxes
	ofRect(dist, dist, boxW, boxH);
	ofRect(boxH+ (dist*2), dist, boxW, boxH);
	ofRect(dist, boxH + (dist*2), boxW, boxH);
	ofRect(boxH + (dist*2), boxH + (dist*2), boxW, boxH);

	// Draw slices
	ofSetColor(255);
	axial.draw		(dist+ aY, dist+aY, axialS);
	sagittal.draw	(boxH+ (dist*2)+ aX, dist+aY, sagittalS);
	coronal.draw	(dist+ aZ, boxW+ (dist*2)+aX, coronalS);
		
//	cam.xRot = latitude;
//	cam.updateMouse();
//	cam.updateRotation();
//	cam.xRot=0;

//	ofQuaternion curRot = ofQuaternion(latitude, cam.getXAxis(), cam.yRot, cam.getYAxis(), cam.zRot, cam.getZAxis());
//	cam.setPosition((cam.getGlobalPosition()-cam.target.getGlobalPosition())*curRot +cam.target.getGlobalPosition());
//	cam.rotate(curRot);
	
	// Draw Volume
	cam.begin();
	ofRotateZ(rot);
	rot++;
	ofPushMatrix();										//	save the old coordinate system
		ofScale(1.0f, -1.0f);							//	flip the y axis vertically, so that it points upwards
		myVolume.drawVolume(0,0,0, ofGetHeight(), 0);	//	draw Volume
	ofPopMatrix();										//	restore the previous coordinate system
	cam.end();
	myVolume.draw(boxH + (dist*2), boxH + (dist*2), boxW, boxH);

	// Draw lines
	ofSetColor(150,0,0);
	int	invCoronalS;
	invCoronalS = ofMap(coronalS, 0, volWidth-1, volWidth-1, 0);
	ofLine(dist, dist+invCoronalS+ aY, dist+boxW, dist+invCoronalS+ aY);				//axial hor line
	ofLine(boxW+ (dist*2), dist+invCoronalS+ aY, (boxW*2)+ (dist*2), dist+invCoronalS+ aY);	//sagital hor line

	ofLine(boxW+ (dist*2)+axialS+ aX, dist, boxW+ (dist*2)+axialS+aX, boxH+dist);		//sagital vert line
	ofLine(dist, boxW+ (dist*2)+axialS+ aX, boxH+dist,  boxW+ (dist*2)+axialS+aX);		//coronal hor line
	
	ofLine(dist+sagittalS+ aY, dist, dist+sagittalS+ aY, dist+boxW);					//axial vert line
	ofLine(dist+sagittalS+ aY, boxW+ (dist*2), dist+sagittalS+ aY, (boxW*2)+ (dist*2));	//coronal vert
}
}


//--------------------------------------------------------------
void vizManager::saveCameraPosition()
{
	posMat = cam.getGlobalTransformMatrix();
	ofxXmlSettings *XML = new ofxXmlSettings("GUI/camera_settings.xml");
    XML->setValue("posMat_00", posMat(0,0), 0);
	XML->setValue("posMat_01", posMat(0,1), 0);
	XML->setValue("posMat_02", posMat(0,2), 0);
	XML->setValue("posMat_03", posMat(0,3), 0);

    XML->setValue("posMat_10", posMat(1,0), 0);
	XML->setValue("posMat_11", posMat(1,1), 0);
	XML->setValue("posMat_12", posMat(1,2), 0);
	XML->setValue("posMat_13", posMat(1,3), 0);
	
	XML->setValue("posMat_20", posMat(2,0), 0);
	XML->setValue("posMat_21", posMat(2,1), 0);
	XML->setValue("posMat_22", posMat(2,2), 0);
	XML->setValue("posMat_23", posMat(2,3), 0);
    
	XML->setValue("posMat_30", posMat(3,0), 0);
	XML->setValue("posMat_31", posMat(3,1), 0);
	XML->setValue("posMat_32", posMat(3,2), 0);
	XML->setValue("posMat_33", posMat(3,3), 0);
	
	XML->setValue("distance", cam.getDistance(), 0);
	XML->saveFile("GUI/camera_settings.xml");
    delete XML;
	message = "saved camera_settings.xml";
	ofLog(OF_LOG_NOTICE, message);
}

//--------------------------------------------------------------
void vizManager::loadCameraPosition()
{
	if( XML.loadFile("GUI/camera_settings.xml") ){
		message = "camera_settings.xml loaded!";
	}else{
		message = "unable to load camera_settings.xml check data/ folder";
	}
	ofLog(OF_LOG_NOTICE, message);
	
	posMat(0,0)= XML.getValue("posMat_00",0.0, 0);
	posMat(0,1)= XML.getValue("posMat_01",0.0, 0);
	posMat(0,2)= XML.getValue("posMat_02",0.0, 0);
	posMat(0,3)= XML.getValue("posMat_03",0.0, 0);
	
	posMat(1,0)= XML.getValue("posMat_10",0.0, 0);
	posMat(1,1)= XML.getValue("posMat_11",0.0, 0);
	posMat(1,2)= XML.getValue("posMat_12",0.0, 0);
	posMat(1,3)= XML.getValue("posMat_13",0.0, 0);
	
	posMat(2,0)= XML.getValue("posMat_20",0.0, 0);
	posMat(2,1)= XML.getValue("posMat_21",0.0, 0);
	posMat(2,2)= XML.getValue("posMat_22",0.0, 0);
	posMat(2,3)= XML.getValue("posMat_23",0.0, 0);
	
	posMat(3,0)= XML.getValue("posMat_30",0.0, 0);
	posMat(3,1)= XML.getValue("posMat_31",0.0, 0);
	posMat(3,2)= XML.getValue("posMat_32",0.0, 0);
	posMat(3,3)= XML.getValue("posMat_33",0.0, 0);
	
	cam.setDistance(XML.getValue("distance",0.0, 0));
	cam.setTransformMatrix(posMat);
}

//--------------------------------------------------------------
void vizManager::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
//	cout << "GUI:: got event from: " << name << endl;
	
	if(name == "FBO quality"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "FBO quality " << slider->getScaledValue() << endl;
		FBOq = slider->getScaledValue();
		myVolume.setXyQuality(FBOq);
	}
	else if(name == "Z quality"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "Z quality " << slider->getScaledValue() << endl;
		Zq = slider->getScaledValue();
		myVolume.setZQuality(Zq);
	}
	else if(name == "Threshold"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "Threshold " << slider->getScaledValue() << endl;
		thresh = slider->getScaledValue();
		myVolume.setThreshold(thresh);
	}
	else if(name == "Density"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "Density " << slider->getScaledValue() << endl;
		density = slider->getScaledValue();
		myVolume.setDensity(density);
	}
	else if(name == "Dithering"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "Dithering " << slider->getScaledValue() << endl;
		dithering = slider->getScaledValue();
		myVolume.setDithering(dithering);
	}
	else if(name == "Clip depth"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "Cut Plane Depth " << slider->getScaledValue() << endl;
		clipPlaneDepth = slider->getScaledValue();
		myVolume.setClipDepth(clipPlaneDepth);
	}
	else if(name == "Elevation clip angle"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "Elevation " << slider->getScaledValue() << endl;
		elevation = slider->getValue();
		myVolume.setElevation(elevation);
	}
	else if(name == "Azimuth clip angle"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cout << "Azimuth " << slider->getScaledValue() << endl;
		azimuth = slider->getValue();
		myVolume.setAzimuth(azimuth);
	}
	else if(name == "linearFilter"){
		ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
		linearFilter = toggle->getValue();
		if (linearFilter){
            myVolume.setVolumeTextureFilterMode(GL_LINEAR);
        }else {
            myVolume.setVolumeTextureFilterMode(GL_NEAREST);
        };
	}
	else if(name == "sagittalS"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		sagittalS = floor(slider->getScaledValue());
		cout <<	"sagittalS " << sagittalS<< endl;
	}
	else if(name == "axialS"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		axialS = floor(slider->getScaledValue());
		cout <<	"axialS " << axialS<< endl;
	}
	else if(name == "coronalS"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		coronalS = floor(slider->getScaledValue());
		cout <<	"coronalS " << coronalS<< endl;
	}
	else if(name == "latitude"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		latitude = floor(slider->getScaledValue());
		cout <<	"latitude " << latitude<< endl;

	}
}

//-----"VOLUMETRICS GFX"----------------------------------------
void vizManager::setup_GUI1()
{
	float dim = 16+10;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
		
	gui1 = new ofxUICanvas(ofGetWidth()-length - 30, 0, length+xInit, ofGetHeight());
	gui1->addWidgetDown(new ofxUILabel("Volume Settings", OFX_UI_FONT_MEDIUM));
	gui1->addSpacer( length-xInit, 2 );
	
	gui1->addSlider("FBO quality", 0.0, 1.0, FBOq, length-xInit, dim);
	gui1->addSlider("Z quality", 0.0, 2.0, Zq, length-xInit, dim);
	gui1->addSlider("Threshold", 0.0, 1.0, thresh, length-xInit, dim);
	gui1->addSlider("Density", 0.0, 1.0, density, length-xInit, dim);
	gui1->addSlider("Dithering", 0.0, 1.0, dithering, length-xInit, dim);
	gui1->addSlider("Clip depth", -1.0, 1.0, clipPlaneDepth, length-xInit, dim);
	gui1->addSlider("Elevation clip angle", -1.0, 1.0, elevation, length-xInit, dim);
	gui1->addSlider("Azimuth clip angle", -1.0, 1.0, azimuth, length-xInit, dim);
	gui1->addWidgetDown(new ofxUIToggle( dim, dim, false, "linearFilter"));

	ofAddListener(gui1->newGUIEvent,this,&vizManager::guiEvent);
}

//-----"VOLUMETRICS GFX"----------------------------------------
void vizManager::setup_GUI2()
{
	float dim = 20;
    float length = 200;
	float boxW = 200;
	float boxH = 200;
	int dist = 50;
	
	gui2 = new ofxUICanvas(dist-dim, dist-dim, boxW*2+dist+dim*3, boxW*2+dist+dim*2);
//	gui2->addWidgetDown(new ofxUILabel("Tweet Visualizer", OFX_UI_FONT_MEDIUM));
	gui2->addWidget(new ofxUISlider(boxW+dim, dim, dim,				length, 0, volHeight-1, axialS, "axialS"));
	gui2->addWidget(new ofxUISlider((boxW*2)+dist+dim, dim, dim,	length, 0, volWidth-1, sagittalS, "sagittalS"));
	gui2->addWidget(new ofxUISlider(boxW+dim, boxW+dist+dim, dim,	length, 0, volDepth-1, coronalS, "coronalS"));
	gui2->addWidget(new ofxUISlider((boxW*2)+dist+dim, boxW+dist+dim, dim,	length, -18.0, 18.0, latitude, "latitude"));
	ofAddListener(gui2->newGUIEvent,this,&vizManager::guiEvent);
}

//--------------------------------------------------------------
void vizManager::keyPressed(int key ){
    switch(key)
    {
		case 's':
			gui1->saveSettings("GUI/viz_settings.xml");
			gui2->saveSettings("GUI/viz_settings_2.xml");
			saveCameraPosition();
			break;
		case 'l':
			loadCameraPosition();
			gui1->loadSettings("GUI/viz_settings.xml");
			break;
		case 'f':
			ofSetWindowPosition(0, 0);
			ofSetVerticalSync(false);
			//ofSetFullscreen(false);
			break;
		case 'F':
			ofSetVerticalSync(true);
			ofSetFullscreen(true);
			break;
		case 'h':
            gui1->toggleVisible();
			gui2->toggleVisible();
			if (bDraw){
				bDraw=false;
			}else{
				bDraw=true;
			}
			break;
		case OF_KEY_UP:
			if(bcameraMode)cam.getTarget().boom(-5);
			else {
				cam.tilt(1);
			}
			break;
		case OF_KEY_DOWN:
			if(bcameraMode)cam.getTarget().boom(5);
			else {
				cam.tilt(-1);
			}
			break;
		case OF_KEY_LEFT:
			if(bcameraMode)cam.getTarget().truck(-5);
			else {
				cam.pan(1);
			}
			break;
		case OF_KEY_RIGHT:
			if(bcameraMode)cam.getTarget().truck(5);
			else {
				cam.pan(-1);
			}
			break;
		case 'M':
			bcameraMode = !bcameraMode;
			break;
	}
	
}




































