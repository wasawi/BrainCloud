#include "vizManager.h"


//----------------------------------------------
vizManager::vizManager(){
	
}

//----------------------------------------------
vizManager::~vizManager(){
	delete guiVolume;
	delete guiSliders;
}

//--------------------------------------------------------------
void vizManager::setup(){

	// TODO: Remove this from here
//	ofEnableSmoothing();
	initX=50;
	initY=50;
	sliderW = 20;
	dist = 20;
	length = 251;
	boxW = boxH = 200;
	
	fsliceY = volHeight /2;
	fsliceX = volWidth /2;
	fsliceZ = volDepth /2;
	
	//camera
	loadCameraPosition();
	bcameraMode = true;
	cam.disableMouseInput();
//	cam.setFov(90.);
	//	cam.disableMouseInput();

	//Volume
	initVolume();
	
	//GUI
/*	guiVolume->setFont("Arial Unicode.ttf");
    guiVolume->setFontSize(OFX_UI_FONT_LARGE, 14);
    guiVolume->setFontSize(OFX_UI_FONT_MEDIUM, 10);
    guiVolume->setFontSize(OFX_UI_FONT_SMALL, 8);
*/
	setup_guiVolume();
	guiVolume->loadSettings("GUI/viz_settings.xml");
	guiVolume->setDrawBack(true);
	guiVolume->setVisible(false);
	
	//3d Views
	bDraw = true;
	setup_guiSliders();
	guiSliders->loadSettings("GUI/viz_settings_2.xml");
	guiSliders->setDrawBack(false);
	//	guiSliders->setAutoDraw(true);
}

//--------------------------------------------------------------
void vizManager::initVolume(){
	
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
	volume2D.setup		(volumeData, volWidth, volHeight, volDepth, boxW, boxH);
}

//--------------------------------------------------------------
void vizManager::update(){

}


//--------------------------------------------------------------
void vizManager::draw(){
if (bDraw){

	//Draw box below
	ofPushView();
	ofTranslate(initX, initY);
	ofSetColor(0,0,0, 100 );
	ofRect(0, 0, (boxW+dist)*2+dist*3, (boxW+dist)*2+dist);
	
	//Draw Slices
	ofPushView();
		ofTranslate(dist, dist);
		volume2D.draw							(isliceY, CORONAL);
	
		ofPushView();
			ofTranslate( boxH+ (dist+sliderW), 0);
			volume2D.draw						(isliceX, SAGITTAL);
		ofPopView();
		
		ofPushView();
			ofTranslate( 0, boxH+ dist);
			volume2D.draw						(isliceZ, AXIAL);
		ofPopView();
	
	// Draw Volume
	ofSetColor(0);
	ofRect(boxH+ (dist+sliderW), boxH+ dist, boxW, boxH);
	ofSetColor(255);
	cam.begin();
	ofRotateZ(rot);
	rot++;
	ofPushMatrix();										//	save the old coordinate system
		ofScale(1.0f, -1.0f);							//	flip the y axis vertically, so that it points upwards
		myVolume.update(0,0,0, ofGetHeight(), 0);	//	draw Volume
	ofPopMatrix();										//	restore the previous coordinate system
	cam.end();
	myVolume.draw(boxH + (dist+sliderW), boxH + dist, boxW, boxH);
	
	ofPopView();
	ofPopView();
}
}

//--------------------------------------------------------------
void vizManager::setup_guiVolume()
{
	float sliderW = 10;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING + OFX_UI_GLOBAL_PADDING;
    float length = boxW*2+dist*3;
	
	guiVolume = new ofxUICanvas(initX, initY+ (dist*2) +dist + (boxH*2)+2, boxW*2+(dist*3)+(sliderW*2), 200);
//	guiVolume->addWidgetDown(new ofxUILabel("Volume Settings", OFX_UI_FONT_MEDIUM));
	//	guiVolume->addSpacer( length-xInit, 2 );
	guiVolume->addSlider("FBO quality", 0.0, 1.0, FBOq, length-xInit, sliderW)->setDrawBack(true);
	guiVolume->addSlider("Z quality", 0.0, 2.0, Zq, length-xInit, sliderW)->setDrawBack(true);
	guiVolume->addSlider("Threshold", 0.0, 1.0, thresh, length-xInit, sliderW)->setDrawBack(true);
	guiVolume->addSlider("Density", 0.0, 1.0, density, length-xInit, sliderW)->setDrawBack(true);
	guiVolume->addSlider("Dithering", 0.0, 1.0, dithering, length-xInit, sliderW)->setDrawBack(true);
	guiVolume->addSlider("Clip depth", -1.0, 1.0, clipPlaneDepth, length-xInit, sliderW)->setDrawBack(true);
	guiVolume->addSlider("Elevation clip angle", -1.0, 1.0, elevation, length-xInit, sliderW)->setDrawBack(true);
	guiVolume->addSlider("Azimuth clip angle", -1.0, 1.0, azimuth, length-xInit, sliderW)->setDrawBack(true);
	//	guiVolume->addWidgetDown(new ofxUIToggle( sliderW, sliderW, false, "linearFilter"));
	guiVolume -> autoSizeToFitWidgets();
	ofAddListener(guiVolume->newGUIEvent,this,&vizManager::guiEvent);
}

//--------------------------------------------------------------
void vizManager::setup_guiSliders()
{
	guiSliders = new ofxUICanvas(initX, initY, boxW*2+(dist*3)+(sliderW*2), boxW*2+dist*3);
	//(string _name, T _min, T _max, T _value, float w, float h, float x = 0, float y = 0);

	//--------------------------- Coronal ---------------------------//
	guiSliders->addWidget(new ofxUISlider("fsliceY", -boxH/2, boxH/2, fsliceY , sliderW,	boxH, boxW+dist, dist));
	ofxUISlider *slider = (ofxUISlider *) guiSliders->getWidget("fsliceY");
	slider->setLabelVisible(false);

	guiSliders->addWidget(new ofxUI2DPad("coronalPad",
										 ofVec3f(-boxW/2, boxW/2, 0),		//_rangeX
										 ofVec3f(boxH/2, -boxH/2, 0),		//_rangeY
										 ofVec3f(fsliceX,fsliceZ, 0),		//_value
										 boxW, boxH, dist, dist));			// float w, float h, float x = 0, float y = 0);
	ofxUI2DPad *pad = (ofxUI2DPad *) guiSliders->getWidget("coronalPad");
	pad->setLabelVisible(false);
	ofColor transparent;
	transparent.set(0,0,0,0);
	pad->setColorBack(transparent);

	//--------------------------- Sagittal ---------------------------//
	guiSliders->addWidget(new ofxUISlider("fsliceX",-boxH/2, boxH/2, fsliceX, sliderW,	boxH, (boxW*2)+(dist*2)+sliderW, dist));
	slider = (ofxUISlider *) guiSliders->getWidget("fsliceX");
	slider->setLabelVisible(false);
	
	guiSliders->addWidget(new ofxUI2DPad("sagittalPad",
										 ofVec3f(-boxW/2, boxW/2, 0),		//_rangeX
										 ofVec3f(boxH/2, -boxH/2, 0),		//_rangeY
										 ofVec3f(fsliceY,fsliceZ, 0),		//_value
										 boxW, boxH, boxW+(dist*2)+sliderW, dist));			// float w, float h, float x = 0, float y = 0);
	pad = (ofxUI2DPad *) guiSliders->getWidget("sagittalPad");
	pad->setLabelVisible(false);
	pad->setColorBack(transparent);
		
	//--------------------------- Axial ---------------------------//
	guiSliders->addWidget(new ofxUISlider("fsliceZ", -boxH/2, boxH/2, fsliceZ, sliderW,	boxH, boxW+dist, boxW+dist+sliderW));
	slider = (ofxUISlider *) guiSliders->getWidget("fsliceZ");
	slider->setLabelVisible(false);
	
	guiSliders->addWidget(new ofxUI2DPad("axialPad",
										 ofVec3f(-boxW/2, boxW/2, 0),		//_rangeX
										 ofVec3f(-boxH/2, boxH/2, 0),		//_rangeY
										 ofVec3f(fsliceX,fsliceY, 0),		//_value
										 boxW, boxH, dist, boxW+dist+sliderW));			// float w, float h, float x = 0, float y = 0);
	pad = (ofxUI2DPad *) guiSliders->getWidget("axialPad");
	pad->setLabelVisible(false);
	pad->setColorBack(transparent);

	//--------------------------- Volume ---------------------------//
	guiSliders->addWidget(new ofxUISlider( "latitude", -18.0, 18.0, latitude, sliderW,	boxH, (boxW*2)+(dist*2)+sliderW, boxW+dist+sliderW));
	slider = (ofxUISlider *) guiSliders->getWidget("latitude");
	slider->setLabelVisible(false);

	ofAddListener(guiSliders->newGUIEvent,this,&vizManager::guiEvent);
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
	else if(name == "fsliceY"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		fsliceY = floor(slider->getScaledValue());
		cout <<	"fsliceY " << fsliceY<< endl;
		updateCoordinates();
		updateSliders();
		updatePads();
		
	}
	else if(name == "fsliceX"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		fsliceX = floor(slider->getScaledValue());
		cout <<	"fsliceX " << fsliceX<< endl;
		updateCoordinates();
		updateSliders();
		updatePads();
	}
	else if(name == "fsliceZ"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		fsliceZ = floor(slider->getScaledValue());
		cout <<	"fsliceZ " << fsliceZ<< endl;
		updateCoordinates();
		updateSliders();
		updatePads();
	}
	
	else if (name== "coronalPad"){
		ofxUI2DPad *pad = (ofxUI2DPad *) guiSliders->getWidget("coronalPad");

		// get values
		fsliceX = pad->getScaledValue().x;
		fsliceZ = pad->getScaledValue().y;
		cout <<	"coronalPad.x =  " << fsliceX<< endl;
		cout <<	"coronalPad.y =  " << fsliceZ<< endl;
		updateCoordinates();
		updateSliders();
		updatePads();
		}

	else if (name== "sagittalPad"){
		ofxUI2DPad *pad = (ofxUI2DPad *) guiSliders->getWidget("sagittalPad");

		// get values
		fsliceY = pad->getScaledValue().x;
		fsliceZ = pad->getScaledValue().y;
		cout <<	"SagittalPad.x = " << fsliceY<< endl;
		cout <<	"SagittalPad.y = " << fsliceZ<< endl;
		updateCoordinates();
		updateSliders();
		updatePads();
	}

	else if (name== "axialPad"){
		ofxUI2DPad *pad = (ofxUI2DPad *) guiSliders->getWidget("axialPad");
		
		// get values
		fsliceX = pad->getScaledValue().x;
		fsliceY = pad->getScaledValue().y;
		cout <<	"axialPad.x = " << fsliceX<< endl;
		cout <<	"axialPad.y = " << fsliceY<< endl;
		updateCoordinates();
		updateSliders();
		updatePads();
	}
	
	else if(name == "latitude"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		latitude = floor(slider->getScaledValue());
		cout <<	"latitude " << latitude<< endl;
	}
}

//--------------------------------------------------------------
void vizManager::keyPressed(int key ){
    switch(key)
    {
		case 's':
			guiVolume->saveSettings("GUI/viz_settings.xml");
			guiSliders->saveSettings("GUI/viz_settings_2.xml");
			saveCameraPosition();
			break;
		case 'l':
			loadCameraPosition();
			guiVolume->loadSettings("GUI/viz_settings.xml");
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
            guiVolume->toggleVisible();
//			guiSliders->toggleVisible();
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

//--------------------------------------------------------------
void vizManager::updateCoordinates(){

	int halfW = (boxW - volWidth) /2;
	int	halfH = (boxW - volHeight) /2;
	int halfD = (boxW - volDepth) /2;
	
	isliceX		= floor (ofMap(fsliceX,		-boxW/2, boxW/2, -halfW, volWidth-1 + halfW));
	isliceX		= ofClamp(isliceX,0, volWidth-1);
	
	isliceY		= floor (ofMap(fsliceY,		-boxW/2, boxW/2, -halfH, volHeight-1 + halfH));
	isliceY		= ofClamp(isliceY,0, volHeight-1);

	isliceZ		= floor (ofMap(fsliceZ,		-boxW/2, boxW/2, -halfD, volDepth-1 + halfD));
	isliceZ		= ofClamp(isliceZ,0, volDepth-1);
	
	cout <<	"isliceX " << isliceX<< endl;
	cout <<	"isliceY " << isliceY<< endl;
	cout <<	"isliceZ " << isliceZ<< endl;
}

//--------------------------------------------------------------
void vizManager::updateSliders(){
	
	ofxUISlider *slider;
	//update sliders
	slider = (ofxUISlider *) guiSliders->getWidget("fsliceX");
	slider -> setValue(fsliceX);
	slider = (ofxUISlider *) guiSliders->getWidget("fsliceY");
	slider -> setValue(fsliceY);
	slider = (ofxUISlider *) guiSliders->getWidget("fsliceZ");
	slider -> setValue(fsliceZ);
}

//--------------------------------------------------------------
void vizManager::updatePads(){
	
	ofxUI2DPad *pad;
	//update pads
	pad = (ofxUI2DPad *) guiSliders->getWidget("coronalPad");
	pad -> setValue(ofVec3f(fsliceX, fsliceZ, 0));
	pad = (ofxUI2DPad *) guiSliders->getWidget("sagittalPad");
	pad -> setValue(ofVec3f(fsliceY,fsliceZ,0));
	pad = (ofxUI2DPad *) guiSliders->getWidget("axialPad");
	pad -> setValue(ofVec3f(fsliceX,fsliceY,0));
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


































