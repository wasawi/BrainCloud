#include "vizManager.h"


//----------------------------------------------
vizManager::vizManager(){

//	ofAddListener(ofEvents().mousePressed, this, &vizManager::mousePressed);
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
	
	//setup GUIs
	initGui();

	// Talairach Atlas
	initTalairach();
	
	//intersection plane
	initRayPlane();
	
	// Init Volume
	initVolume();
	
	// Volume rendering
	initVolumeRendering();
		
	// point cloud
	createPointCloud();

	//states
	bSelecting		= false;
	bMovingCursor	= false;
	bActive			= true;
	bDraw			= true;

	update();

	
	// camera
	ofxLoadCamera(cam, "GUI/cameraSettings.txt");
}

//--------------------------------------------------------------
void vizManager::initVolume()
{
	// Init Volume
	//vol.load("volumes/Colin27T1_tight/");
	vol.loadVolume("volumes/talairach_nii/");
	vol.setup(boxW, boxH);

	volSize		= vol.getSize();
	volWidth	= volSize.x;
	volHeight	= volSize.y;
	volDepth	= volSize.z;
}

//--------------------------------------------------------------
void vizManager::initVolumeRendering()
{
	// Init Volume Rendering
    volRender.setup(&vol);

    volRender.setRenderSettings(FBOq, Zq, density, thresh);
	volRender.setVolumeTextureFilterMode(GL_LINEAR);
	vol.setSlices(&uiClamp);
	vol.setRayPlane(&rayPlane);
	
	cubeSize	= volRender.getCubeSize();
}

//--------------------------------------------------------------
void vizManager::initRayPlane()
{
	// Init Ray Plane
	rayPlane.setCenter(ofVec3f(0));
	rayPlane.setScale(ofVec3f(1));
	rayPlane.setNormal(ofVec3f(0.0f, 0.0f, 1.0f));
	rayPlane.setUp(ofVec3f(0.0f, 1.0f, 0.0f));
	rayPlane.setInfinite(false);
}

//--------------------------------------------------------------
void vizManager::initTalairach()
{
	// Talairach Atlas
	outputLabels.resize(10);
	talOffset	= ofVec3f(-70,-102,-42);
	talDrawX	= boxW+dist*3+sliderW;
	talDrawY	= initY+dist;
	talairachAtlas.setup("brainData/TalairachAtlas.txt");
	//	talClient.setup("../../../data/brainData/talairach.jar");
}

//--------------------------------------------------------------
void vizManager::initGui()
{
	// 2Dpad Canvas
	initX	=0;
	initY	=50;
	sliderW = 20;
	dist	= 20;
	boxW	= boxH = 200;
	uiRange	= ofVec3f(-1,1,0);
	uiCoord	= ofVec3f(0);
//	allowEvent	= false;
	
	// Sliders UI
	setup_guiSliders();
	guiSliders->loadSettings("GUI/slices_settings.xml");
	guiSliders->setDrawBack(false);
	//	guiSliders->setAutoDraw(true);	this method does not work
	
	// Volume UI
	setup_guiVolume();
	guiVolume->loadSettings("GUI/volume_settings.xml");
	guiVolume->setDrawBack(true);
	guiVolume->setVisible(false);
		
//	allowEvent	= true;
}

//--------------------------------------------------------------
void vizManager::update()
{
	updateCoordinates();
	updateSlicesImage();
	updateSlices2Volume();
	
	updateTalCoords();
//	updateTalAtlasLabel();
//	updateTalLabel();

}

//--------------------------------------------------------------
void vizManager::updateTalLabel()
{
	outputLabels = talClient.get(talCoord);
	for (int i=2; i<outputLabels.size(); i++)
	{
		ofLogVerbose("vizManager") << outputLabels[i];
	}
}

//--------------------------------------------------------------
void vizManager::updateTalAtlasLabel()
{
//	voxelValue = vol.getVoxelValue();
//	voxelNumber = vol.getVoxelNumber();
	//mapping from pixel value to index value on the Talairach Atlas
	int mapValue= ofMap(vol.getVoxelValue(), 0, 255, 0, 1105);
	outputLabels [2] = talairachAtlas.getHemisphere(mapValue);
	outputLabels [3] = talairachAtlas.getLobe(mapValue);
	outputLabels [4] = talairachAtlas.getGyrus(mapValue);
	outputLabels [5] = talairachAtlas.getTissueType(mapValue);
	outputLabels [6] = talairachAtlas.getCellType(mapValue);
}

//--------------------------------------------------------------
void vizManager::createPointCloud()
{
	mesh.clear();
	
	ofVec3f point = ofVec3f(0.0f,0.0f,0.0f);
	ofVec3f	translation = ofVec3f(-cubeSize.x/2, -cubeSize.y/2, -cubeSize.z/2);
	int row=0;
	int page=0;
	int index=0;
	
	for(int z=0; z<volDepth; z++){
		for(int y=0; y<volHeight; y++){
			for(int x=0; x<volWidth; x++){

				row = y*volWidth;
				page = z*volWidth*volHeight;
				index = x + row + page;
				
				int _val = vol.getVoxels()[index];
				int mapValue= ofMap(_val, 0, 255, 0, 1105);

				if (talairachAtlas.getTissueType(mapValue) == "Gray Matter" && _val<2){
					point = ofVec3f(x, y, z);
					// "normalize" voxel position
					voxelToVector(point);
					// scale points to fix volume render size
					point *= cubeSize;
					// move the points to the center of the world
					point += translation;
					// draw the volume with correct map
					point.x *= -1;
					
					ofColor random (ofRandom(25),ofRandom(255),ofRandom(255),50);
					mesh.addColor(random);
					mesh.addVertex(point);
					meshWhite.addVertex(point);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void vizManager::voxelToVector(ofVec3f& voxel)
{
	// transform current selected voxel coordinates
	// to normalised vector
	
	ofVec3f vector;
	vector.x= ofMap(voxel.x, 0, volWidth,	0, 1.0, false);
	vector.y= ofMap(voxel.y, 0, volHeight,	0, 1.0, false);
	vector.z= ofMap(voxel.z, 0, volDepth,	0, 1.0, false);
	 
	voxel = vector;
}


//--------------------------------------------------------------
void vizManager::updateTalCoords()
{
	// transform my volume coordinates to Talairach coordinates
	// using the offstet provided in nifti headers
	talCoord = volCoord + talOffset;
	talCoord.x *=-1;
//	ofLogVerbose("vizManager") <<	"tal.x " << ofToString(talCoord.x,2);
//	ofLogVerbose("vizManager") <<	"tal.y " << ofToString(talCoord.y,2);
//	ofLogVerbose("vizManager") <<	"tal.z " << ofToString(talCoord.z,2);
}

//--------------------------------------------------------------
void vizManager::updateCoordinates()
{
	// get the distance between the box and the volume
	float halfW = (boxW - volWidth) /2;
	float halfH = (boxW - volHeight) /2;
	float halfD = (boxW - volDepth) /2;
	
	ofLogVerbose("vizManager") <<	"uiCoord.x " << ofToString(uiCoord.x,2);
	ofLogVerbose("vizManager") <<	"uiCoord.y " << ofToString(uiCoord.y,2);
	ofLogVerbose("vizManager") <<	"uiCoord.z " << ofToString(uiCoord.z,2);

	// it would be nice to do it like this:
	// volCoord = floor(ofMap(uiCoord, uiRangeMin, uiRangeMax, 0, volSize));
	// all using vec3f and mapped all at once.
	volCoord.z = floor(ofMap(uiCoord.z, uiRange.x, uiRange.y, 0 -halfH, volHeight + halfH));
	volCoord.x = floor(ofMap(uiCoord.x, uiRange.x, uiRange.y, 0 -halfW, volWidth + halfW));
	volCoord.y = floor(ofMap(uiCoord.y, uiRange.y, uiRange.x, 0 -halfD, volDepth + halfD));

	ofLogVerbose("vizManager") <<	"volCoord.x " << volCoord.x;
	ofLogVerbose("vizManager") <<	"volCoord.y " << volCoord.y;
	ofLogVerbose("vizManager") <<	"volCoord.z " << volCoord.z;
}

//--------------------------------------------------------------
void vizManager::updateSlicesImage()
{
	// update de Depth of the slices drawn by volumeSlice
	// this uptade must only run when there is a gui event
	vol.redraw(CORONAL, volCoord.z);
	vol.redraw(SAGITTAL, volCoord.x);
	vol.redraw(AXIAL, volCoord.y);
}

//--------------------------------------------------------------
void vizManager::updateSlices2Volume()
{
	// transform current selected normalised vector
	// to voxel coordinates

	//ofClamp(volCoord.z,0, volDepth-1);
	ofVec3f uiCoord_;

	// map it to pixel values;
	float uiSize=100;
	uiCoord_ = uiCoord*uiSize;

	//use the pixel positions that are only inside the volume
	//(excluding areas between volume and uiPad limits)

	// coronal
	uiCoord_.z= ofMap(uiCoord_.z, -volHeight/2, volHeight/2, -uiSize, uiSize);
	// sagittal
	uiCoord_.x= ofMap(uiCoord_.x, -volWidth/2, volWidth/2, -uiSize, uiSize);
	// axial
	uiCoord_.y= ofMap(uiCoord_.y, -volDepth/2, volDepth/2, -uiSize, uiSize);
	
	//normalized again
	uiCoord_ = uiCoord_/100;
	
	//clamp the mapped values
	uiCoord_.x = ofClamp(uiCoord_.x, -1, 1);
	uiCoord_.y = ofClamp(uiCoord_.y, -1, 1);
	uiCoord_.z = ofClamp(uiCoord_.z, -1, 1);
	uiClamp = uiCoord_;
}

//--------------------------------------------------------------
void vizManager::updateVolume2Slices()
{
	// transform current selected voxel coordinates
	// to normalised vector

	//ofClamp(volCoord.z,0, volDepth-1);
	ofVec3f uiCoord_;
	
	// map it to pixel values;
	float uiSize=100;
	uiCoord_ = uiClamp*uiSize;
	
	//use the pixel positions that are only inside the volume
	//(excluding areas between volume and uiPad limits)
	// coronal
	uiCoord_.z= ofMap(uiCoord_.z, -uiSize, uiSize, -volHeight/2, volHeight/2);
	// sagittal
	uiCoord_.x= ofMap(uiCoord_.x, -uiSize, uiSize, -volWidth/2, volWidth/2);
	// axial
	uiCoord_.y= ofMap(uiCoord_.y, -uiSize, uiSize, -volDepth/2, volDepth/2);
	
	//normalized again
	uiCoord_ = uiCoord_/100;
	
	//clamp the mapped values
	uiCoord_.x = ofClamp(uiCoord_.x, -1, 1);
	uiCoord_.y = ofClamp(uiCoord_.y, -1, 1);
	uiCoord_.z = ofClamp(uiCoord_.z, -1, 1);
	uiCoord = uiCoord_;
}

//--------------------------------------------------------------
void vizManager::draw()
{
	if (bDraw)
	{
		drawVolume();
		
		// check collision
		if (bMovingCursor) moveCursor();
		// Draw ArcBall
		if (!bMovingCursor && ofGetMousePressed()) cam.drawArcBall();
		
		drawSelection();
		
		drawSlices();

		drawTalairach();

		drawMesh();
		
		// draw nearest point
		if (!bMovingCursor && bActive) drawNearestPoint();
	}
	
/*
	// this is really ugly.. needs a fix
	if(bCamLoaded<3){
		// camera
		ofxLoadCamera(cam, "GUI/cameraSettings.txt");
		cout << "camLoaded"<< endl;
		bCamLoaded++;
	}*/
}

//--------------------------------------------------------------
void vizManager::drawSelection()
{
//	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	cam.begin();
	if(bSelecting) selection.drawSphereAxis();
	selection.draw(cam);
	cam.end();
//	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
}

//--------------------------------------------------------------
void vizManager::drawTalairach()
{
	//Draw talairach pixel value and labels
	ofPushStyle();
	//		ofSetColor(voxelValue, 255);
	//		ofRect(initX,tempY,boxH*2+dist*5,dist);
	
	ofSetColor(0, 255, 255);
	ofDrawBitmapString("Talairarch coordinate :", talDrawX, talDrawY);
	string str= "x= "+ ofToString(talCoord.x)+" y= "+ ofToString(talCoord.y)+" z= "+ ofToString(talCoord.z);
	ofDrawBitmapString(str, talDrawX, talDrawY+dist);
	for (int i=0; i<outputLabels.size()-2; i++) {
		vector<string> items = ofSplitString(outputLabels[i+2], ",");
		for (int j=0; j<items.size(); j++) {
			ofDrawBitmapString(items[j], talDrawX, talDrawY+i*dist+j*dist+(dist*2));
		}
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void vizManager::drawVolume()
{
	// Draw Volume
	ofSetColor(255);
	cam.begin();
	volRender.update();
	cam.end();
	
	volRender.draw(0, ofGetHeight(), ofGetWidth(), -ofGetHeight());
}
//--------------------------------------------------------------
void vizManager::drawSlices()
{
	
	ofPushView();
	ofTranslate(initX, initY);
	
	//Draw Slices "canvas"
	ofPushStyle();
		ofSetColor(OFX_UI_COLOR_BACK);
		ofRect(0, 0, boxW+dist*3, (boxW+dist)*3+dist);
	ofPopStyle();
	
	//Draw Slices
	ofPushView();
		ofTranslate(dist, dist);
		vol.draw(CORONAL);
	
	ofPushView();
		ofTranslate( 0, boxH+ dist);
		vol.draw(SAGITTAL);
	
	ofPushView();
		ofTranslate( 0, boxH+ dist);
		vol.draw(AXIAL);
	
	ofPopView();
	ofPopView();
	ofPopView();
	ofPopView();

}

//--------------------------------------------------------------
void vizManager::drawMesh()
{
	cam.begin();
	ofPushView();
	
	glPointSize(4);
	ofSetColor(200,20);
	mesh.drawVertices();

	glPointSize(1);
	ofSetColor(255);
	meshWhite.drawVertices();

	ofPopView();
	cam.end();
}

//--------------------------------------------------------------
void vizManager::drawNearestPoint()
{
	int n = mesh.getNumVertices();
	float nearestDistance = 0;
	ofVec2f nearestVertex;
	int nearestIndex = 0;
	ofVec2f mouse(ofGetMouseX(), ofGetMouseY());
	for(int i = 0; i < n; i++) {
		ofVec3f cur = cam.worldToScreen(mesh.getVertex(i));
		float distance = cur.distance(mouse);
		if(i == 0 || distance < nearestDistance) {
			nearestDistance = distance;
			nearestVertex = cur;
			nearestIndex = i;
		}
	}
	
	ofPushStyle();
		ofSetColor(ofColor::gray);
		ofLine(nearestVertex, mouse);
		
		ofNoFill();
		ofSetColor(ofColor::yellow);
		ofSetLineWidth(2);
		ofCircle(nearestVertex, 4);
		ofSetLineWidth(1);
	ofPopStyle();
	
	ofVec2f offset(10, -10);
	ofDrawBitmapStringHighlight(ofToString(nearestIndex), mouse + offset);
}

//--------------------------------------------------------------
void vizManager::setup_guiVolume()
{
	// Volume Canvas vars from 2Dpad
	guiVolume = new ofxUICanvas(initX+ (dist*2)+ sliderW + boxH,
									initY+dist + (boxH),
									(dist*2)+ boxH,
									280);
	guiVolume->setPadding(4);
	
	//new vars for widgets
	float canvasW = boxW+dist*2;
	float sliderH = 10;
	float initX = OFX_UI_GLOBAL_WIDGET_SPACING;
	float sliderW = boxW+dist*2-OFX_UI_GLOBAL_WIDGET_SPACING*2;
	
	guiVolume->addWidgetDown(new ofxUILabel("Volume Settings", OFX_UI_FONT_MEDIUM));
	guiVolume->addSlider("FBO quality", 0.0, 1.0, &FBOq, sliderW, sliderH)->setDrawBack(true);
	guiVolume->addSlider("Z quality", 0.0, 2.0, &Zq, sliderW, sliderH)->setDrawBack(true);
	guiVolume->addSlider("Threshold", 0.0, 1.0, &thresh, sliderW, sliderH)->setDrawBack(true);
	guiVolume->addSlider("Density", 0.0, 1.0, &density, sliderW, sliderH)->setDrawBack(true);
	guiVolume->addSlider("Clip depth", -1.0, 1.0, &clipPlaneDepth, sliderW, sliderH)->setDrawBack(true);
	guiVolume->addSlider("Elevation clip angle", -1.0, 1.0, &elevation, sliderW, sliderH)->setDrawBack(true);
	guiVolume->addSlider("Azimuth clip angle", -1.0, 1.0, &azimuth, sliderW, sliderH)->setDrawBack(true);
//	guiVolume->addSlider("Dithering", 0.0, 1.0, dithering, sliderW, sliderH)->setDrawBack(true);
//	guiVolume->addWidgetDown(new ofxUIToggle( sliderH, sliderH, false, "linearFilter"));
//	guiVolume -> autoSizeToFitWidgets();
	ofAddListener(guiVolume->newGUIEvent,this,&vizManager::guiEvent);
}

//--------------------------------------------------------------
void vizManager::setup_guiSliders()
{
	guiSliders = new ofxUICanvas(initX, initY, boxW+sliderW+dist*2, boxW*3+dist*4);
	ofAddListener(guiSliders->newGUIEvent,this,&vizManager::guiEvent);
	
	// ADDING WIDGETS:
	//	It doesn't work like this
	//	ofxUISlider *slider;
	//	slider =(ofxUISlider *) guiSliders->addSlider("uiCoord.z", -1, 1, &uiCoord.z , sliderW,	boxH, boxW+dist, dist);
	//	slider->setLabelVisible(false);
	
	// used vars
	ofxUISlider *slider;
	ofxUI2DPad *pad;
	ofColor transparent;
	transparent.set(0,0,0,0);
	
	//--------------------------- Coronal ---------------------------//
	guiSliders->addWidget(new ofxUISlider("coronalDepth",
										  uiRange.x,
										  uiRange.y,
										  &uiCoord.z,
										  sliderW,
										  boxH,
										  boxW+dist, dist));
	
	slider = (ofxUISlider *) guiSliders->getWidget("coronalDepth");
	slider->setLabelVisible(false);
	
	guiSliders->addWidget(new ofxUI3DPad("coronalPad",
										 uiRange,					//_rangeX
										 uiRange,					//_rangeY
										 uiRange,					//_rangeZ
										 &uiCoord,					//_value
										 boxW, boxH, dist, dist,	// w, h, x, y
										 OFX_UI_FRONT));			// viewPoint

	pad = (ofxUI2DPad *) guiSliders->getWidget("coronalPad");
	pad->setLabelVisible(false);
	pad->setColorBack(transparent);
	
	
	//--------------------------- Sagittal ---------------------------//
	guiSliders->addWidget(new ofxUISlider("sagittalDepth",
										  uiRange.x,
										  uiRange.y,
										  &uiCoord.x,
										  sliderW,
										  boxH,
										  boxW+dist,
										  boxW+dist+sliderW));
	
	slider = (ofxUISlider *) guiSliders->getWidget("sagittalDepth");
	slider->setLabelVisible(false);
	
	guiSliders->addWidget(new ofxUI3DPad("sagittalPad",
										 uiRange,					//_rangeX
										 uiRange,					//_rangeY
										 uiRange,					//_rangeZ
										 &uiCoord,					//_value
										 boxW, boxH, dist, boxW+dist+sliderW,	// w, h, x, y
										 OFX_UI_LEFT));				// viewPoint
	
	pad = (ofxUI2DPad *) guiSliders->getWidget("sagittalPad");
	pad->setLabelVisible(false);
	pad->setColorBack(transparent);
	
	
	//--------------------------- Axial ---------------------------//
	guiSliders->addWidget(new ofxUISlider("axialDepth",
										  uiRange.x,
										  uiRange.y,
										  &uiCoord.y,
										  sliderW,
										  boxH,
										  boxW+dist,
										  boxW*2+dist*2+sliderW));
	
	slider = (ofxUISlider *) guiSliders->getWidget("axialDepth");
	slider->setLabelVisible(false);
	
	guiSliders->addWidget(new ofxUI3DPad("axialPad",
										 uiRange,					//_rangeX
										 uiRange,					//_rangeY
										 uiRange,					//_rangeZ
										 &uiCoord,					//_value
										 boxW, boxH, dist, boxW*2+dist*2+sliderW,	// w, h, x, y
										 OFX_UI_TOP));				// viewPoint
	
	pad = (ofxUI2DPad *) guiSliders->getWidget("axialPad");
	pad->setLabelVisible(false);
	pad->setColorBack(transparent);
}

//--------------------------------------------------------------
void vizManager::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	//	ofLogVerbose("vizManager") << "GUI:: got event from: " << name;
	
	if(name == "FBO quality")
	{
		ofLogVerbose("vizManager") << "FBO quality " << FBOq;
		volRender.setXyQuality(FBOq);
	}
	else if(name == "Z quality")
	{
		ofLogVerbose("vizManager") << "Z quality " << Zq;
		volRender.setZQuality(Zq);
	}
	else if(name == "Threshold")
	{
		ofLogVerbose("vizManager") << "Threshold " << thresh;
		volRender.setThreshold(thresh);
	}
	else if(name == "Density")
	{
		ofLogVerbose("vizManager") << "Density " << density;
		volRender.setDensity(density);
	}
	else if(name == "Dithering")
	{
		ofLogVerbose("vizManager") << "Dithering " << dithering;
		volRender.setDithering(dithering);
	}
	else if(name == "Clip depth")
	{
		ofLogVerbose("vizManager") << "Cut Plane Depth " << clipPlaneDepth;
		volRender.setClipDepth(clipPlaneDepth);
	}
	else if(name == "Elevation clip angle")
	{
		ofLogVerbose("vizManager") << "Elevation " << elevation;
		volRender.setElevation(elevation);
	}
	else if(name == "Azimuth clip angle")
	{
		ofLogVerbose("vizManager") << "Azimuth " << azimuth;
		volRender.setAzimuth(azimuth);
	}
	else if(name == "linearFilter")
	{
		ofLogVerbose("vizManager") << "linearFilter " << linearFilter;
		if (linearFilter){
            volRender.setVolumeTextureFilterMode(GL_LINEAR);
        }else {
            volRender.setVolumeTextureFilterMode(GL_NEAREST);
        };
	}
	else if(name == "coronalDepth")
	{
		ofLogVerbose("vizManager") <<	"coronalDepth " << uiCoord.z;
	}
	else if(name == "sagittalDepth")
	{
		ofLogVerbose("vizManager") <<	"sagittalDepth " << uiCoord.x;
	}
	else if(name == "axialDepth")
	{
		ofLogVerbose("vizManager") <<	"axialDepth " << uiCoord.y;
	}
	else if (name== "coronalPad")
	{
		ofLogVerbose("vizManager") <<	"coronalPad.x =  " << uiCoord.x;
		ofLogVerbose("vizManager") <<	"coronalPad.y =  " << uiCoord.y;
	}
	else if (name== "sagittalPad")
	{
		ofLogVerbose("vizManager") <<	"SagittalPad.x = " << uiCoord.z;
		ofLogVerbose("vizManager") <<	"SagittalPad.y = " << uiCoord.y;
	}
	else if (name== "axialPad")
	{
		ofLogVerbose("vizManager") <<	"axialPad.x = " << uiCoord.x;
		ofLogVerbose("vizManager") <<	"axialPad.y = " << uiCoord.z;
	}
	/*
	else if(name == "latitude")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		latitude = floor(slider->getScaledValue());
		ofLogVerbose("vizManager") <<	"latitude " << latitude;
	}*/
//	if (allowEvent)
		update();
}

//--------------------------------------------------------------
void vizManager::keyPressed(int key ){
    switch(key)
    {
		case ' ':
			bMovingCursor = !bMovingCursor;
			if (bMovingCursor) cam.disableMouseInput();
			break;
		case 'h':
            guiVolume->toggleVisible();
			break;
		case 's':
			ofxSaveCamera(cam, "GUI/cameraSettings.txt");
			guiVolume->saveSettings("GUI/volume_settings.xml");
			break;
		case 'l':
			ofxLoadCamera(cam, "GUI/cameraSettings.txt");
//			guiVolume->loadSettings("GUI/volume_settings.xml");
			break;
		case 'f':
			//ofSetWindowPosition(0, 0);
			//ofSetVerticalSync(false);
			//ofSetFullscreen(false);
//			selectVoxels();
			
			break;
		case 'c':
			selection.clear();
			break;

		case 'r':
			cam.bRotate = !cam.bRotate;
			break;
		case 'F':
			ofSetVerticalSync(true);
			//ofSetFullscreen(true);
			break;
		case 't':
			// talairach tests
			int voxelValue = vol.getVoxelValue();
			//mapping from pixel value to index value on the Talairach Atlas
			int currentValue= ofMap(voxelValue, 0, 255, 0, 1105);
			
			string lobe = talairachAtlas.getLobe(currentValue);
			ofLogNotice("Talairach")<< lobe;
			break;
	}
}

//--------------------------------------------------------------
void vizManager::mousePressed(ofMouseEventArgs& e)
{
	
}

//--------------------------------------------------------------
void vizManager::select()
{
	ofRay	mouseRay;
	ofVec3f farPoint;
	float radius;
	
	ofVec3f screenMouse = ofVec3f(ofGetMouseX(), ofGetMouseY(),0);
	ofVec3f worldMouse = cam.screenToWorld(ofVec3f(screenMouse.x, screenMouse.y, 0.0f));
	ofVec3f worldMouseEnd = cam.screenToWorld(ofVec3f(screenMouse.x, screenMouse.y, 1.0f));
	ofVec3f worldMouseTransmissionVector = worldMouseEnd - worldMouse;
	mouseRay.s = worldMouse;
	mouseRay.t = worldMouseTransmissionVector;
	
	// check for intersection
	rayPlane.intersect(mouseRay, farPoint);
	farPoint /= cubeSize/2;
	radius = intersectionPosition.distance(farPoint)/2;
	radius *= cubeSize.y;
	ofVec3f position = intersectionPosition*cubeSize/2;

	selection.select(position, radius);
}

//--------------------------------------------------------------
void vizManager::addSelection()
{
	selection.add();
}

//--------------------------------------------------------------
void vizManager::moveCursor()
{
	bool	doesIntersect;
	
	cam.begin();
	doesIntersect = vol.getIntersection(&cam, cubeSize, intersectionPosition);
	if (doesIntersect) {
		updateVolume2Slices();
		updateCoordinates();
		updateSlicesImage();
	}
	cam.end();
}


//--------------------------------------------------------------
void vizManager::selectVoxels()
{
	vector<ofVec3f> positions;
	vector<float> radius;
	positions.push_back(ofVec3f(50, 50, 50));
	radius.push_back(0);
	
//	vol.selectVoxels(positions, radius);
}





