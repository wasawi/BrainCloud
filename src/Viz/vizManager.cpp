#include "vizManager.h"


//----------------------------------------------
vizManager::vizManager(){
}

//----------------------------------------------
vizManager::~vizManager()
{
	delete guiVolume;
	delete guiSliders;
}

//--------------------------------------------------------------
void vizManager::setup()
{
	// Talairach Atlas
	talairachAtlas.setup("brainData/TalairachAtlas.txt");
//	talClient.setup("../../../data/brainData/talairach.jar");
	outputLabels.resize(10);
	
	// 2Dpad Canvas
	initX	=0;
	initY	=50;
	sliderW = 20;
	dist	= 20;
	boxW	= boxH = 200;
	
	visCoord.y = volHeight /2;
	visCoord.x = volWidth /2;
	visCoord.z = volDepth /2;
	
	talOffset= ofVec3f(-70,-102,-42);
	TalDrawX	= boxW+dist*3+sliderW;
	TalDrawy	=initY+dist;
	
	
	
	//camera
	loadCameraPosition();
	bcameraMode = true;
//	cam.disableMouseInput();
	
	//Volume
	initVolume();

	//All GUIs here
	setup_guis();
	
}

//--------------------------------------------------------------
void vizManager::initVolume()
{
	imageSequence.init("volumes/Colin27T1_tight/IM-0001-0",3,".tif", 0);
//	imageSequence.init("volumes/talairach_nii/IM-0001-0",3,".tif", 0);
	
	volWidth	= imageSequence.getWidth();
    volHeight	= imageSequence.getHeight();
    volDepth	= imageSequence.getSequenceLength();
	
	ofLogNotice("vizManager") << "setting up volume data buffer at " << volWidth << "x" << volHeight << "x" << volDepth;
    volumeData = new unsigned char[volWidth*volHeight*volDepth];
	
	//fill out the array pixel in white
	for (int i=0; i<volWidth*volHeight*volDepth; i++ ){
		volumeData[i]= (unsigned char) 255;
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
//					ofLogVerbose("vizManager") << sample << " ";
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
void vizManager::update()
{
	updateCoordinates();
	updateSlices();
	updateSliders();
	updatePads();
	updateTalCoords();
	updateTalAtlasLabel();
//	updateTalLabel();
}

//--------------------------------------------------------------
void vizManager::updateTalAtlasLabel()
{
	voxelValue = volume2D.getVoxelValue();
	//mapping from pixel value to index value on the Talairach Atlas
	int mapValue= ofMap(voxelValue, 0, 255, 0, 1105);
	outputLabels [2] = talairachAtlas.getHemisphere(mapValue);
	outputLabels [3] = talairachAtlas.getLobe(mapValue);
	outputLabels [4] = talairachAtlas.getGyrus(mapValue);
	outputLabels [5] = talairachAtlas.getTissueType(mapValue);
	outputLabels [6] = talairachAtlas.getCellType(mapValue);
}

//--------------------------------------------------------------
void vizManager::updateTalCoords()
{
	// transform my coordinates to Talairach coordinates using the offstet provided in nifti headers
	talCoord.x = (volCoord.x + talOffset.x)*-1;
	talCoord.y = volCoord.y + talOffset.y;
	talCoord.z = volCoord.z + talOffset.z;
	
	ofLogVerbose("vizManager") <<	"tal.x " << ofToString(talCoord.x,2);
	ofLogVerbose("vizManager") <<	"tal.y " << ofToString(talCoord.y,2);
	ofLogVerbose("vizManager") <<	"tal.z " << ofToString(talCoord.z,2);
}

//--------------------------------------------------------------
void vizManager::updateTalLabel()
{
	outputLabels = talClient.get(talCoord);
	for (int i=2; i<outputLabels.size(); i++) {
		ofLogVerbose("vizManager") << outputLabels[i];
	}
}

//--------------------------------------------------------------
void vizManager::updateCoordinates()
{
	// get the distance between the box and the volume
	float halfW = (boxW - volWidth) /2;
	float halfH = (boxW - volHeight) /2;
	float halfD = (boxW - volDepth) /2;

	ofLogVerbose("vizManager") <<	"visCoord.x " << ofToString(visCoord.x,2);
	ofLogVerbose("vizManager") <<	"visCoord.y " << ofToString(visCoord.y,2);
	ofLogVerbose("vizManager") <<	"visCoord.z " << ofToString(visCoord.z,2);

	// this must be debugged!!! probably is not exactly correct !!
	// we need to clamp the output of the map because there are no boxels between the volume and the box.
	// therefore clamp from 0 to volW/H/D -1! because the for runs while i < volW
	volCoord.x		= floor (ofMap(visCoord.x,		-boxW/2, boxW/2, -halfW, volWidth-1 + halfW));
	volCoordClamp.x		= ofClamp(volCoord.x,0, volWidth-1);
	
	volCoord.y		= floor (ofMap(visCoord.y,		-boxW/2, boxW/2, -halfH, volHeight-1 + halfH));
	volCoordClamp.y		= ofClamp(volCoord.y,0, volHeight-1);

	volCoord.z		= floor (ofMap(visCoord.z,		-boxW/2, boxW/2, -halfD, volDepth-1 + halfD));
	volCoordClamp.z		= ofClamp(volCoord.z,0, volDepth-1);
	
	ofLogVerbose("vizManager") <<	"volCoord.x " << volCoord.x;
	ofLogVerbose("vizManager") <<	"volCoord.y " << volCoord.y;
	ofLogVerbose("vizManager") <<	"volCoord.z " << volCoord.z;
}

//--------------------------------------------------------------
void vizManager::updateSlices()
{
	volume2D.redraw(volCoordClamp.y, CORONAL);
	volume2D.redraw(volCoordClamp.x, SAGITTAL);
	volume2D.redraw(volCoordClamp.z, AXIAL);
}

//--------------------------------------------------------------
void vizManager::updateSliders()
{
	//update sliders
	ofxUISlider *slider;
	slider = (ofxUISlider *) guiSliders->getWidget("visCoord.x");
	slider -> setValue(visCoord.x);
	slider = (ofxUISlider *) guiSliders->getWidget("visCoord.y");
	slider -> setValue(visCoord.y);
	slider = (ofxUISlider *) guiSliders->getWidget("visCoord.z");
	slider -> setValue(visCoord.z);
}

//--------------------------------------------------------------
void vizManager::updatePads()
{
	//update pads
	ofxUI2DPad *pad;
	pad = (ofxUI2DPad *) guiSliders->getWidget("coronalPad");
	pad -> setValue(ofVec3f(visCoord.x, visCoord.z, 0));
	pad = (ofxUI2DPad *) guiSliders->getWidget("sagittalPad");
	pad -> setValue(ofVec3f(visCoord.y,visCoord.z,0));
	pad = (ofxUI2DPad *) guiSliders->getWidget("axialPad");
	pad -> setValue(ofVec3f(visCoord.x,visCoord.y,0));
}


//--------------------------------------------------------------
void vizManager::draw()
{
	if (bDraw){
		
		// Draw Volume
		ofSetColor(255);
		cam.begin();
		ofPushMatrix();									//	save the old coordinate system
			//myVolume.drawSlices();
			ofScale(1.0f, -1.0f);						//	flip the y axis vertically, so that it points upwards
			myVolume.update(0,0,0, ofGetHeight(), 0);	//	draw Volume
		ofPopMatrix();									//	restore the previous coordinate system
		cam.end();
		myVolume.draw(0, 0, ofGetWidth(), ofGetHeight());
		cam.drawArcBall();
		
		
		//Draw Slices "canvas"
		ofPushView();
		ofTranslate(initX, initY);
		ofSetColor(0,0,0, 100 );
		ofRect(0, 0, boxW+dist*3, (boxW+dist)*3+dist);
		
		//Draw Slices
		ofPushView();
		ofTranslate(dist, dist);
		volume2D.drawCoronal(0, 0, volCoord.y);
		
		ofPushView();
		ofTranslate( 0, boxH+ dist);
		volume2D.drawSagittal(0, 0, volCoord.x);

		ofPushView();
		ofTranslate( 0, boxH+ dist);
		volume2D.drawAxial(0, 0, volCoord.z);
		ofPopView();
		ofPopView();
		ofPopView();
		ofPopView();
		
		//Draw talairach pixel value and labels
		ofPushStyle();
		ofSetColor(voxelValue, 255);
//		ofRect(initX,tempY,boxH*2+dist*5,dist);
		
		ofSetColor(0, 255, 255);
		ofDrawBitmapString("Talairarch coordinate :", TalDrawX, TalDrawy);
		string str= "x= "+ ofToString(talCoord.x)+" y= "+ ofToString(talCoord.y)+" z= "+ ofToString(talCoord.z);
		ofDrawBitmapString(str, TalDrawX, TalDrawy+dist);
		for (int i=0; i<outputLabels.size()-2; i++) {
			vector<string> items = ofSplitString(outputLabels[i+2], ",");
				for (int j=0; j<items.size(); j++) {
					ofDrawBitmapString(items[j], TalDrawX, TalDrawy+i*dist+j*dist+(dist*2));
				}
		}
		ofPopStyle();
	}
}

//--------------------------------------------------------------
void vizManager::setup_guis()
{
	
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
void vizManager::setup_guiVolume()
{
	// Volume Canvas vars from 2Dpad
	guiVolume = new ofxUICanvas(initX+ (dist*2)+ sliderW + boxH,
									initY+dist + (boxH),
									(dist*2)+ boxH,
									280);
	//new vars for widgets
	float canvasW = boxW+dist*2;
	float sliderH = 10;
	float initX = OFX_UI_GLOBAL_WIDGET_SPACING;
	float sliderW = boxW+dist*2-OFX_UI_GLOBAL_WIDGET_SPACING*2;
	
	guiVolume->addWidgetDown(new ofxUILabel("Volume Settings", OFX_UI_FONT_MEDIUM));
	guiVolume->addSlider("FBO quality", 0.0, 1.0, FBOq, sliderW, sliderH)->setDrawBack(true);
	guiVolume->addSlider("Z quality", 0.0, 2.0, Zq, sliderW, sliderH)->setDrawBack(true);
	guiVolume->addSlider("Threshold", 0.0, 1.0, thresh, sliderW, sliderH)->setDrawBack(true);
	guiVolume->addSlider("Density", 0.0, 1.0, density, sliderW, sliderH)->setDrawBack(true);
	guiVolume->addSlider("Clip depth", -1.0, 1.0, clipPlaneDepth, sliderW, sliderH)->setDrawBack(true);
	guiVolume->addSlider("Elevation clip angle", -1.0, 1.0, elevation, sliderW, sliderH)->setDrawBack(true);
	guiVolume->addSlider("Azimuth clip angle", -1.0, 1.0, azimuth, sliderW, sliderH)->setDrawBack(true);
//	guiVolume->addSlider("Dithering", 0.0, 1.0, dithering, sliderW, sliderH)->setDrawBack(true);
//	guiVolume->addWidgetDown(new ofxUIToggle( sliderH, sliderH, false, "linearFilter"));
//	guiVolume -> autoSizeToFitWidgets();
	ofAddListener(guiVolume->newGUIEvent,this,&vizManager::guiEvent);
}

//--------------------------------------------------------------
void vizManager::setup_guiSliders()
{
	guiSliders = new ofxUICanvas(initX, initY, boxW*2+(dist*3)+(sliderW*2), boxW*3+dist*4);

	
	//--------------------------- Coronal ---------------------------//
	//(string _name, T _min, T _max, T _value, float w, float h, float x = 0, float y = 0);
	guiSliders->addWidget(new ofxUISlider("visCoord.y", -boxH/2, boxH/2, visCoord.y , sliderW,	boxH, boxW+dist, dist));
	ofxUISlider *slider = (ofxUISlider *) guiSliders->getWidget("visCoord.y");
	slider->setLabelVisible(false);
	
	guiSliders->addWidget(new ofxUI2DPad("coronalPad",
										 ofVec3f(-boxW/2, boxW/2, 0),		//_rangeX
										 ofVec3f(boxH/2, -boxH/2, 0),		//_rangeY
										 ofVec3f(visCoord.x,visCoord.z, 0),		//_value
										 boxW, boxH, dist, dist));			// float w, float h, float x = 0, float y = 0);
	ofxUI2DPad *pad = (ofxUI2DPad *) guiSliders->getWidget("coronalPad");
	pad->setLabelVisible(false);
	ofColor transparent;
	transparent.set(0,0,0,0);
	pad->setColorBack(transparent);
	
	//--------------------------- Sagittal ---------------------------//
	guiSliders->addWidget(new ofxUISlider("visCoord.x",-boxH/2, boxH/2, visCoord.x, sliderW,	boxH, boxW+dist, boxW+dist+sliderW));
	slider = (ofxUISlider *) guiSliders->getWidget("visCoord.x");
	slider->setLabelVisible(false);
	
	guiSliders->addWidget(new ofxUI2DPad("sagittalPad",
										 ofVec3f(-boxW/2, boxW/2, 0),		//_rangeX
										 ofVec3f(boxH/2, -boxH/2, 0),		//_rangeY
										 ofVec3f(visCoord.y,visCoord.z, 0),		//_value
										 boxW, boxH, dist, boxW+dist+sliderW));			// float w, float h, float x = 0, float y = 0);
//										 boxW, boxH, boxW+(dist*2)+sliderW, dist));			// float w, float h, float x = 0, float y = 0);
	pad = (ofxUI2DPad *) guiSliders->getWidget("sagittalPad");
	pad->setLabelVisible(false);
	pad->setColorBack(transparent);
	
	//--------------------------- Axial ---------------------------//
	guiSliders->addWidget(new ofxUISlider("visCoord.z", -boxH/2, boxH/2, visCoord.z, sliderW,	boxH, boxW+dist, boxW*2+dist*2+sliderW));
	slider = (ofxUISlider *) guiSliders->getWidget("visCoord.z");
	slider->setLabelVisible(false);
	
	guiSliders->addWidget(new ofxUI2DPad("axialPad",
										 ofVec3f(-boxW/2, boxW/2, 0),		//_rangeX
										 ofVec3f(-boxH/2, boxH/2, 0),		//_rangeY
										 ofVec3f(visCoord.x,visCoord.y, 0),		//_value
										 boxW, boxH, dist, boxW*2+dist*2+sliderW));			// float w, float h, float x = 0, float y = 0);
//										 boxW, boxH, dist, boxW+dist+sliderW));			// float w, float h, float x = 0, float y = 0);
	pad = (ofxUI2DPad *) guiSliders->getWidget("axialPad");
	pad->setLabelVisible(false);
	pad->setColorBack(transparent);
	
	//--------------------------- Volume ---------------------------//
//	guiSliders->addWidget(new ofxUISlider( "latitude", -18.0, 18.0, latitude, sliderW,	boxH, (boxW*2)+(dist*2)+sliderW, boxW+dist+sliderW));
//	slider = (ofxUISlider *) guiSliders->getWidget("latitude");
//	slider->setLabelVisible(false);
	
	ofAddListener(guiSliders->newGUIEvent,this,&vizManager::guiEvent);
}

//--------------------------------------------------------------
void vizManager::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	//	ofLogVerbose("vizManager") << "GUI:: got event from: " << name;
	
	if(name == "FBO quality"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		ofLogVerbose() << "FBO quality " << slider->getScaledValue();
		FBOq = slider->getScaledValue();
		myVolume.setXyQuality(FBOq);
	}
	else if(name == "Z quality"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		ofLogVerbose() << "Z quality " << slider->getScaledValue();
		Zq = slider->getScaledValue();
		myVolume.setZQuality(Zq);
	}
	else if(name == "Threshold"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		ofLogVerbose() << "Threshold " << slider->getScaledValue();
		thresh = slider->getScaledValue();
		myVolume.setThreshold(thresh);
	}
	else if(name == "Density"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		ofLogVerbose() << "Density " << slider->getScaledValue();
		density = slider->getScaledValue();
		myVolume.setDensity(density);
	}
	else if(name == "Dithering"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		ofLogVerbose() << "Dithering " << slider->getScaledValue();
		dithering = slider->getScaledValue();
		myVolume.setDithering(dithering);
	}
	else if(name == "Clip depth"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		ofLogVerbose() << "Cut Plane Depth " << slider->getScaledValue();
		clipPlaneDepth = slider->getScaledValue();
		myVolume.setClipDepth(clipPlaneDepth);
	}
	else if(name == "Elevation clip angle"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		ofLogVerbose() << "Elevation " << slider->getScaledValue();
		elevation = slider->getValue();
		myVolume.setElevation(elevation);
	}
	else if(name == "Azimuth clip angle"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		ofLogVerbose() << "Azimuth " << slider->getScaledValue();
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
	else if(name == "visCoord.y"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		visCoord.y = floor(slider->getScaledValue());
		ofLogVerbose() <<	"visCoord.y " << visCoord.y;
		update();
	}
	else if(name == "visCoord.x"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		visCoord.x = floor(slider->getScaledValue());
		ofLogVerbose() <<	"visCoord.x " << visCoord.x;
		update();	}
	else if(name == "visCoord.z"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		visCoord.z = floor(slider->getScaledValue());
		ofLogVerbose() <<	"visCoord.z " << visCoord.z;
		update();
	}
	
	else if (name== "coronalPad"){
		ofxUI2DPad *pad = (ofxUI2DPad *) guiSliders->getWidget("coronalPad");
		
		// get values
		visCoord.x = pad->getScaledValue().x;
		visCoord.z = pad->getScaledValue().y;
		ofLogVerbose() <<	"coronalPad.x =  " << visCoord.x;
		ofLogVerbose() <<	"coronalPad.y =  " << visCoord.z;
		update();
	}
	
	else if (name== "sagittalPad"){
		ofxUI2DPad *pad = (ofxUI2DPad *) guiSliders->getWidget("sagittalPad");
		
		// get values
		visCoord.y = pad->getScaledValue().x;
		visCoord.z = pad->getScaledValue().y;
		ofLogVerbose() <<	"SagittalPad.x = " << visCoord.y;
		ofLogVerbose() <<	"SagittalPad.y = " << visCoord.z;
		update();
	}
	
	else if (name== "axialPad"){
		ofxUI2DPad *pad = (ofxUI2DPad *) guiSliders->getWidget("axialPad");
		
		// get values
		visCoord.x = pad->getScaledValue().x;
		visCoord.y = pad->getScaledValue().y;
		ofLogVerbose() <<	"axialPad.x = " << visCoord.x;
		ofLogVerbose() <<	"axialPad.y = " << visCoord.y;
		update();
	}
	
	else if(name == "latitude"){
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		latitude = floor(slider->getScaledValue());
		ofLogVerbose() <<	"latitude " << latitude;
	}
}

//--------------------------------------------------------------
void vizManager::keyPressed(int key ){
    switch(key)
    {
		case ' ':
			cam.bRotate = !cam.bRotate;
			break;
		case 'h':
            guiVolume->toggleVisible();
			break;
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
		case 't':
			int voxelValue = volume2D.getVoxelValue();
			//mapping from pixel value to index value on the Talairach Atlas
			int currentValue= ofMap(voxelValue, 0, 255, 0, 1105);
			
			string lobe = talairachAtlas.getLobe(currentValue);
			ofLogNotice("Talairach")<< lobe;
			break;
		
/*		case OF_KEY_UP:
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
*/
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


































