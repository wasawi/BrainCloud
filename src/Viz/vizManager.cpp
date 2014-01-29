#include "vizManager.h"

/*
//--------------------------------------------------------------
void vizManager::setup(){

	// TODO: Remove this from here
	ofEnableSmoothing();
	
	setup_GUI1();
	gui1->loadSettings("GUI/viz_settings.xml");
	gui1->setDrawBack(true);
	
	//------------ Volumetrics setup ---------------//
	//----------------------------------------------//
	imageSequence.init("volumes/Colin27T1_tight/IM-0001-0",3,".tif", 1);
	
	volWidth	= imageSequence.getWidth();
    volHeight	= imageSequence.getHeight();
    volDepth	= imageSequence.getSequenceLength();
	
	//add one morw pixel in width and height..
	//i dont know why some tifs are not reading well...
	//maybe is related to th texture which has to be a power of 2..
	int volWidth_	= 152;
	int volHeight_	= 189;
	
    cout << "setting up volume data buffer at " << volWidth << "x" << volHeight << "x" << volDepth <<"\n";
    volumeData = new unsigned char[volWidth_*volHeight_*volDepth];
	
	//fill out the added pixel in 0
	for (int i=0; i<volWidth_*volHeight_*volDepth; i++ ){
		volumeData[i]= (unsigned char) 0;
	}
	
    for(int z=0; z<volDepth; z++)
    {
        imageSequence.loadFrame(z);
		int gradient = 0;
		for(int y=0; y<volHeight_; y++)
        {
			for(int x=0; x<volWidth_; x++)
			{
				if (x<volWidth && y<volHeight)
				{																	// get values from image
					int i = ((x + volWidth_*y) + z*volWidth_*volHeight_);			// the pointer position at Array
					int sample = imageSequence.getPixels()[x+y*volWidth];			// the pixel on the image
					volumeData[i] = sample;
				}
            }
        }
    }
	
	// Init Volume
    myVolume.setup(volWidth_, volHeight_, volDepth, ofVec3f(1,1,1));
	myVolume.updateVolumeData(volumeData,volWidth_,volHeight_,volDepth, 0, 0, 0);
    myVolume.setRenderSettings(FBOq, Zq, density, thresh);
	myVolume.setVolumeTextureFilterMode(GL_LINEAR);
}


//--------------------------------------------------------------
void vizManager::update(){
	
}


//--------------------------------------------------------------
void vizManager::draw(){
	
}


//--------------------------------------------------------------
void vizManager::saveCameraPosition()
{
	posMat = cam.getTarget().getGlobalTransformMatrix();
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
}

//--------------------------------------------------------------
void vizManager::loadCameraPosition()
{
	message = "loading camera_settings.xml";
	if( XML.loadFile("GUI/camera_settings.xml") ){
		message = "camera_settings.xml loaded!";
	}else{
		message = "unable to load camera_settings.xml check data/ folder";
	}
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
	cam.getTarget().setTransformMatrix(posMat);
}

//--------------------------------------------------------------
void vizManager::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	cout << "GUI:: got event from: " << name << endl;
	
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
}

//-----"VOLUMETRICS GFX"----------------------------------------
void vizManager::setup_GUI1()
{
	float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
		
	gui1 = new ofxUICanvas(ofGetWidth()-length - 30, 0, length+xInit, ofGetHeight());
	gui1->addWidgetDown(new ofxUILabel("Volume Settings", OFX_UI_FONT_MEDIUM));
	
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

*/

