/*  myVolume - draw myVolumetric data
 Created by Jordi on 01/04/14.
 */

#include "myVolume.h"

/*
myVolume::myVolume()
{
}
//--------------------------------------------------------------
myVolume::~myVolume()
{
}
*/

//--------------------------------------------------------------
void myVolume::loadAsRGBAPow2(string path)
{
	ofxImageSequencePlayer imageSequence;
	imageSequence.init(path + "IM-0001-0", 3, ".tif", 0);
	
	int channels =4;
	
	// calculate myVolume size
	w	= imageSequence.getWidth();
    h	= imageSequence.getHeight();
    d	= imageSequence.getSequenceLength();
	
	ofLogNotice("myVolume::loadColor") << "setting up myVolume data buffer at " << w << "x" << h << "x" << d;
//	unsigned char* voxels;
//	voxels = new unsigned char[(int) (w*h*d)*channels];

	voxels.allocate(w, h, d, channels);
	
	// fill my array with voxels
    for(int z=0; z<d; z++)
    {
        imageSequence.loadFrame(z);
		for(int y=0; y<h; y++)
        {
			for(int x=0; x<w; x++)
			{
				if (x<w && y<h)
				{													// get values from image
					int i = ((x + y*w) + z*w*h)*channels;			// the pointer position at Array
					int sample = imageSequence.getPixels()[(int)(x+y*w)];		// the pixel on the image
					ofColor c;
					c.set(sample);
					
					voxels[i] = c.r;
					voxels[i+1] = c.g;
					voxels[i+2] = c.b;
					voxels[i+3] = sample;
					//					ofLogVerbose("vizManager") << sample << " ";
				}
            }
        }
    }//end for
	
	ofxVoxels origin;
	origin.setFromVoxels(voxels.getVoxels(), w, h, d, channels);
	
	// we will put the original pixels here with an added padding
	allocate(ofNextPow2(w), ofNextPow2(h),ofNextPow2(d), channels);
	origin.pasteInto(*this, 0, 0, 0);
}


void myVolume::loadAsRGBA(string path)
{
	ofxImageSequencePlayer imageSequence;
	imageSequence.init(path + "IM-0001-0", 3, ".tif", 0);
	
	int channels =4;
	
	// calculate myVolume size
	w	= imageSequence.getWidth();
    h	= imageSequence.getHeight();
    d	= imageSequence.getSequenceLength();
	
	ofLogNotice("myVolume::loadMono")
	<< "setting up myVolume data buffer at "
	<< w << "x" << h << "x" << d;
	
	voxels.allocate(w, h, d, channels);

	// fill my array with voxels
    for(int z=0; z<d; z++)
    {
        imageSequence.loadFrame(z);
		voxels.copyFrontSliceFrom(imageSequence.getPixelsRef(), z);
    }

}
void myVolume::loadAsMono(string path)
{
	ofxImageSequencePlayer imageSequence;
	imageSequence.init(path + "IM-0001-0", 3, ".tif", 0);
	
	// calculate myVolume size
	w	= imageSequence.getWidth();
    h	= imageSequence.getHeight();
    d	= imageSequence.getSequenceLength();
	
	ofLogNotice("myVolume::loadMono")
	<< "setting up myVolume data buffer at "
	<< w << "x" << h << "x" << d;
	
	voxels.allocate(w, h, d, OF_IMAGE_GRAYSCALE);
	
	// fill my array with voxels
    for(int z=0; z<d; z++)
    {
        imageSequence.loadFrame(z);
		voxels.copyFrontSliceFrom(imageSequence.getPixelsRef(), z);
    }
}

//--------------------------------------------------------------
void myVolume::setup(float bW, float bH)
{
	boxW		= bW;
	boxH		= bH;
	
	// Needed to align the myVolume at the center of the box
	// Attention! this is not correct..
	// will give problems if W & H are not d same
	halfH = (boxW - h) /2;
	halfW = (boxW - w) /2;
	halfD = (boxW - d) /2;
	
/*	//allocate my pixls type of the image slices
	coronalPixels.allocate(w, d, OF_IMAGE_GRAYSCALE);
	coronalPixels.set(255);
	sagittalPixels.allocate(d, h, OF_IMAGE_GRAYSCALE);
	sagittalPixels.set(255);
	axialPixels.allocate(w, h, OF_IMAGE_GRAYSCALE);
	axialPixels.set(255);	
 */
}

int myVolume::getVoxelValue(){
	
	int value	=0;
	for(int z=0; z<d; z++){
		if (z==axialS){
			for(int y=0; y<h; y++){
				if (y==coronalS) {
					for(int x=0; x<w; x++){
						if (x==sagittalS){
							int line = y*w;
							int page = z*w*h;
							int i = x + line + page;					// the pointer position at Array
							value= voxels[i];							// the pixel on the image
						}
					}
				}
			}
		}
	}
	
	ofLogVerbose("volume") << "sagittalS= " << sagittalS;
	ofLogVerbose("volume") << "coronalS= " << coronalS;
	ofLogVerbose("volume") << "axialS= " << axialS;
	ofLogVerbose("volume") << "voxelValue= " << value;
	return value;
}


//--------------------------------------------------------------
ofColor myVolume::getCursorColor()
{
	ofColor value;
	value = getColor(sagittalS, coronalS, axialS);
	return value;
}

//--------------------------------------------------------------
ofxIntPoint myVolume::getCursorCoords(int _index)
{
	ofxIntPoint value;
	value = voxels.getVoxelCoordinates(_index);
	return value;
}
//--------------------------------------------------------------
int myVolume::getCursorID()
{
	int value;
	value = voxels.getVoxelID(sagittalS, coronalS, axialS);
	return value;
}

//--------------------------------------------------------------
bool myVolume::getVoxelCoordAndVal(int _index, ofxIntPoint& _coord, int& _val){
	
	ofxIntPoint value(0);
	int row=0;
	int page=0;
	int index=0;
	
	for(int z=0; z<d; z++){
		value.z=z;
		
		for(int y=0; y<h; y++){
			value.y=y;
			
			for(int x=0; x<w; x++){
				value.x=x;
				
				row = y*w;
				page = z*w*h;
				index = x + row + page;
				
				if (_index==index){
					value= ofxIntPoint(x,y,z);

					_coord= value;
					_val= getVoxels()[index];
//					ofLogVerbose("myVolume") << "voxelCoord= " << _coord;
//					ofLogVerbose("myVolume") << "voxelVal= " << _val;
					return true;
				}
			}
		}
	}
	//not found
	return false;
}


//--------------------------------------------------------------
int myVolume::getVoxelNumber(){
	
	int value	=0;
	for(int z=0; z<d; z++){
		if (z==axialS){
			for(int y=0; y<h; y++){
				if (y==coronalS) {
					for(int x=0; x<w; x++){
						if (x==sagittalS){
							int line = y*w;
							int page = z*w*h;
							int i = x + line + page;			// the pointer position at Array
							value= i;							// the pixel on the image
						}
					}
				}
			}
		}
	}
	
	ofLogVerbose("myVolume") << "voxelNumber= " << value;
	return value;
}

//--------------------------------------------------------------
void myVolume::colourRandomVoxels(int count)
{
	// calculate myVolume size
	w	= getWidth();
    h	= getHeight();
    d	= getDepth();
	
	for(int i=0; i<count; i++)
    {
		ofColor newColor;
		newColor.set(ofRandom(255), ofRandom(255), ofRandom(255), ofRandom(255));
		setColor(ofRandom(w), ofRandom(h), ofRandom(d), newColor);
	}
}

//--------------------------------------------------------------
void myVolume::colourRandomBoxes(int count)
{
	// calculate myVolume size
	w	= getWidth();
    h	= getHeight();
    d	= getDepth();
	
	for(int i=0; i<count; i++)
    {
		ofColor newColor;
		newColor.set(ofRandom(255));
		
		ofxIntPoint position	= ofVec3f(ofRandom(200), ofRandom(200), ofRandom(200));
		int r =ofRandom(60);
		ofxIntPoint size		= ofVec3f(r/10, r, r/10);
		
		ofxIntBox box(position, size);
		vector<ofxPoint> c = getVoxelsinBox(box);
		
		for(int i=0; i<c.size(); i++){
			setColor(c[i].x, c[i].y, c[i].z, newColor);
		}
	}
}


//--------------------------------------------------------------
void myVolume::redraw(slice vP, int depth)
{
	// try clamp the value like this out = MIN(h, MAX(in, 0));
	if(vP==CORONAL)
	{
		coronalS = depth;	// maybe talairch will need unclamped position??
		if(depth>=0 && depth<h)
		{
			insideCoronal=true;
//			redrawCoronal();
			voxels.copyTopSliceTo(coronal.getPixelsRef(), depth);
		}else{
			insideCoronal=false;
//			coronalS = MIN(h, MAX(depth, 0));
		}
	}
	else if(vP==SAGITTAL)
	{
		sagittalS = depth;
		if(depth>=0 && depth<w)
		{
			insideSagittal=true;
//			redrawSagittal();
			voxels.copyRightSliceTo(sagittal.getPixelsRef(), depth);
		}else{
			insideSagittal=false;
//			sagittalS = MIN(w, MAX(depth, 0));
		}
	}
	else if (vP==AXIAL)
	{
		axialS = depth;
		if(depth>=0 && depth<d)
		{
			insideAxial=true;
//			redrawAxial();
			voxels.copyFrontSliceTo(axial.getPixelsRef(), depth);
		}else{
			insideAxial=false;
//			axialS = MIN(d, MAX(depth, 0));
		}
	}
}

//--------------------------------------------------------------
void myVolume::draw(slice vP)
{
	drawBox();
	if(vP==CORONAL)
	{
		if(insideCoronal) coronal.draw(halfW, halfD);	// otherwise set to black (do not draw).
	}
	else if(vP==SAGITTAL)
	{
		if(insideSagittal) sagittal.draw(halfH, halfD);	// otherwise set to black (do not draw).
	}
	else if(vP==AXIAL)
	{
		if(insideAxial) axial.draw(halfW, halfH);		// otherwise set to black (do not draw).
	}
}
//--------------------------------------------------------------
void myVolume::drawBox()
{
	// Draw Box
	ofPushStyle();
	ofSetColor(0);
	ofRect(0, 0, boxW, boxH);
	ofPopStyle();
	ofSetColor(255);
}
/*
//--------------------------------------------------------------
void myVolume::redrawCoronal()
{
	for(int z=0; z<d; z++)
	{
		for(int y=0; y<h; y++)
		{
			if (y==coronalS){
				for(int x=0; x<w; x++)
				{
					int line = y*w;
					int page = z*w*h;
					int i = x + line + page;					// the position at the pixel array
					coronalPixels[x+(z*w)] = getVoxels()[i];	// get the correct voxel and put it to the pixel array
				}
			}
		}
	}
	
	//draw image
	coronal.setFromPixels(coronalPixels.getPixels(), w, d, OF_IMAGE_GRAYSCALE);
	coronal.mirror(true, false);
}

//--------------------------------------------------------------
void myVolume::redrawSagittal()
{
	for(int z=0; z<d; z++)
    {
		for(int y=0; y<h; y++)
        {
			for(int x=0; x<w; x++)
			{
				if (x==sagittalS){
					int line = y*w;
					int page = z*w*h;
					int i = x + line + page;					// the position at the pixel array
					sagittalPixels[z+(y*d)] = getVoxels()[i];	// get the correct voxel and put it to the pixel array
				}
			}
		}
    }
	
	//draw image
	sagittal.setFromPixels(sagittalPixels.getPixels(), d, h, OF_IMAGE_GRAYSCALE);
	sagittal.rotate90(3);
}

//--------------------------------------------------------------
void myVolume::redrawAxial()
{
	for(int z=0; z<d; z++)
    {
		if (z==axialS){
			for(int y=0; y<h; y++)
			{
				for(int x=0; x<w; x++)
				{
					int line = y*w;
					int page = z*w*h;
					int i = x + line + page;			// the position at the pixel array
					axialPixels[x+line] = getVoxels()[i];	// get the correct voxel and put it to the pixel array
				}
			}
		}
    }
	
	//draw image
	axial.setFromPixels(axialPixels.getPixels(), w, h, OF_IMAGE_GRAYSCALE);
}
*/

/*
//--------------------------------------------------------------
ofxPoint myVolume::getSize(){
	return this->size;
}
//--------------------------------------------------------------
ofxPoint myVolume::getPos(){
	return this->position;
}
*/
/*
 //--------------------------------------------------------------
 template<typename PixelType>
 PixelType* myVolume::getVoxels(){
 return voxels.getVoxels();
 }

*/

//--------------------------------------------------------------
void myVolume::selectVoxels(ofxBox& box){
//	selectedVoxels.pushBack(getVoxelsinBox(box));
}
//--------------------------------------------------------------
void myVolume::selectVoxels(vector <ofxIntBox>& boxes){
	
	selectedVoxels.clear();
	
	for(int i=0; i<boxes.size(); i++){
//		cout << "size = "<< boxes[i].size<<endl;
//		getVoxelsinRadius(_coord[i], _radius[i]);
		vector <ofxPoint> outputVoxels;
		outputVoxels = getVoxelsinBox(boxes[i]);
	}
}
//--------------------------------------------------------------
vector <ofxPoint> myVolume::getVoxelsinBox(ofxIntBox& box){
	
	vector <ofxPoint> outputVoxels;
	ofxIntPoint pointer	(0);
	ofxIntBox bounds(pointer,(ofxIntPoint)getSize());

	for(int z=0; z<box.d; z++){
		for(int y=0; y<box.h; y++){
			for(int x=0; x< box.w; x++){
				pointer.set(x, y, z);
				pointer+=box.position;

				if (bounds.inside(pointer)){
					outputVoxels.push_back(pointer);
//					ofLogVerbose("myVolume") << "voxelCoord= " << pointer;
				}else{
//					ofLogVerbose("myVolume") << "OUTSIDE" << pointer;
				}
			}
		}
	}
	return outputVoxels;
}

vector <ofxPoint> myVolume::getVoxelsinRadius(ofxPoint& _coord, float& _radius){

	vector <ofxPoint> outputVoxels;
	outputVoxels.push_back(_coord);
	
	ofxPoint coord	(0);
	int row			=0;
	int page		=0;
	int index		=0;

	for(int z=0; z<d; z++){
		coord.z=z;
		
		if (z==_coord.z){
			for(int y=0; y<h; y++){
				coord.y=y;
				
				if (y==_coord.y) {
					for(int x=0; x<w; x++){
						coord.x=x;
						
						if (x==_coord.x){
							row = y*w;
							page = z*w*h;
							index = x + row + page;
							
							coord= ofxPoint(x,y,z);
//							value= voxels[index];
							ofLogVerbose("myVolume") << "voxelCoord= " << coord;
//							ofLogVerbose("myVolume") << "voxelVal= " << value;
							//return true;
						}
					}
				}
			}
		}
	}
	
}

/*
 size of box is symetrical for now
 so, size (1,2,3)=
 
 x= -1 to 1;
 y= -2 to 2;
 z= -3 to 3;
 
 */



bool myVolume::inside(ofxPoint _coord){
	
	if (_coord.x<0) return false;
	if (_coord.y<0) return false;
	if (_coord.z<0) return false;
	
	if (_coord.x>w)	return false;
	if (_coord.y>h) return false;
	if (_coord.z>d)	return false;
	
	return true;
}


void myVolume::destroy(){

	
}





void myVolume::setSlices(ofVec3f* _planes)
{
    planeCoords = _planes;
}

void myVolume::setRayPlane(ofPlane* _rayPlane)
{
    rayPlane = _rayPlane;
}

//--------------------------------------------------------------
bool myVolume::getIntersection(ofCamera* cam,ofVec3f &intersectionPosition)
{
    rayPlane->setCenter(ofVec3f(0,0,planeCoords->y)*cubeSize*-.5);
    rayPlane->setScale(cubeSize*.5);
    rayPlane->draw();
	
    //--------------------------------------------------------------
    ofVec3f screenMouse = ofVec3f(ofGetMouseX(), ofGetMouseY(),0);
    ofVec3f worldMouse = cam->screenToWorld(ofVec3f(screenMouse.x, screenMouse.y, 0.0f));
    ofVec3f worldMouseEnd = cam->screenToWorld(ofVec3f(screenMouse.x, screenMouse.y, 1.0f));
    ofVec3f worldMouseTransmissionVector = worldMouseEnd - worldMouse;
    mouseRay.s = worldMouse;
    mouseRay.t = worldMouseTransmissionVector;
	
    // check for intersection
    bool doesIntersect = rayPlane->intersect(mouseRay, intersectionPosition);
    //--------------------------------------------------------------
	
    // get object space position
    intersectionPosition /= cubeSize * .5;
	
    if (doesIntersect) {
		planeCoords->x= -intersectionPosition.x;
		//  planeCoords->y= -intersectionPosition.y;
		planeCoords->z= intersectionPosition.y;
    }
	
    string label = doesIntersect ? "hits" : "misses";
    label += + " at " + ofToString(intersectionPosition);
    ofLogVerbose("Volumetrix") << label;
	
    return doesIntersect;
}










