#include "volume.h"

volume::volume()
{
	//Volume
	volPos		= ofVec3f(0);
	volSize		= ofVec3f(0);
	
    volWidth	= 0;
    volHeight	= 0;
    volDepth	= 0;
}
//--------------------------------------------------------------
volume::~volume()
{

}

void volume::load(string path)
{
	imageSequence.init(path + "IM-0001-0", 3, ".tif", 0);
	
	// calculate volume size
	volWidth	= imageSequence.getWidth();
    volHeight	= imageSequence.getHeight();
    volDepth	= imageSequence.getSequenceLength();
	volSize		= ofVec3f(volWidth, volHeight, volDepth);
	
	ofLogNotice("vizManager") << "setting up volume data buffer at " << volWidth << "x" << volHeight << "x" << volDepth;
    voxels = new unsigned char[volWidth*volHeight*volDepth];
	
	//fill out the array pixel in white for easy debugging
	for (int i=0; i<volWidth*volHeight*volDepth; i++ )
	{
		voxels[i]= (unsigned char) 255;
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
					voxels[i] = sample;
					//					ofLogVerbose("vizManager") << sample << " ";
				}
            }
        }
    }

}

//--------------------------------------------------------------
void volume::setup(float bW, float bH)
{
	boxW		= bW;
	boxH		= bH;
	
	// Needed to align the volume at the center of the box
	// Attention! this is not correct..
	// will give problems if W & H are not d same
	halfH = (boxW - volHeight) /2;
	halfW = (boxW - volWidth) /2;
	halfD = (boxW - volDepth) /2;
	
	//allocate my pixls type of the image slices
	coronalPixels.allocate(volWidth, volDepth, OF_IMAGE_GRAYSCALE);
	coronalPixels.set(255);
	sagittalPixels.allocate(volDepth, volHeight, OF_IMAGE_GRAYSCALE);
	sagittalPixels.set(255);
	axialPixels.allocate(volWidth, volHeight, OF_IMAGE_GRAYSCALE);
	axialPixels.set(255);	
}

//--------------------------------------------------------------
int volume::getVoxelValue(){
	
	int value	=0;
	for(int z=0; z<volDepth; z++){
		if (z==axialS){
			for(int y=0; y<volHeight; y++){
				if (y==coronalS) {
					for(int x=0; x<volWidth; x++){
						if (x==sagittalS){
							int line = y*volWidth;
							int page = z*volWidth*volHeight;
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
ofVec3f volume::getVoxelCoordinates(int _index){
	
	ofVec3f value (0);
	for(int z=0; z<volDepth; z++){
		value.z=z;
		if (z==axialS){
			for(int y=0; y<volHeight; y++){
				value.y=y;
				if (y==coronalS) {
					for(int x=0; x<volWidth; x++){
						value.x=x;

						int line = y*volWidth;
						int page = z*volWidth*volHeight;
						int i = x + line + page;

						if (_index==i){
							value= ofVec3f(x,y,z);
							ofLogVerbose("volume") << "voxelCoordinates= " << value;
							return value;
						}
					}
				}
			}
		}
	}
	
}

//--------------------------------------------------------------
bool volume::getVoxelCoordAndVal(int _index, ofVec3f& _coord, int& _val){
	
	ofVec3f value(0);
	int row=0;
	int page=0;
	int index=0;
	
	for(int z=0; z<volDepth; z++){
		value.z=z;
		
		for(int y=0; y<volHeight; y++){
			value.y=y;
			
			for(int x=0; x<volWidth; x++){
				value.x=x;
				
				row = y*volWidth;
				page = z*volWidth*volHeight;
				index = x + row + page;
				
				if (_index==index){
					value= ofVec3f(x,y,z);

					_coord= value;
					_val= voxels[index];
//					ofLogVerbose("volume") << "voxelCoord= " << _coord;
//					ofLogVerbose("volume") << "voxelVal= " << _val;
					return true;
				}
			}
		}
	}
	//not found
	return false;
}


//--------------------------------------------------------------
int volume::getVoxelNumber(){
	
	int value	=0;
	for(int z=0; z<volDepth; z++){
		if (z==axialS){
			for(int y=0; y<volHeight; y++){
				if (y==coronalS) {
					for(int x=0; x<volWidth; x++){
						if (x==sagittalS){
							int line = y*volWidth;
							int page = z*volWidth*volHeight;
							int i = x + line + page;			// the pointer position at Array
							value= i;							// the pixel on the image
						}
					}
				}
			}
		}
	}
	
	ofLogVerbose("volume") << "voxelNumber= " << value;
	return value;
}


//--------------------------------------------------------------
void volume::redraw(viewPoint vP, int depth)
{

	// try clamp the value like this out = MIN(volHeight, MAX(in, 0));
	if(vP==CORONAL)
	{
		coronalS = depth;	// maybe talairch will need unclamped position??
		if(depth>=0 && depth<volHeight)
		{
			insideCoronal=true;
			redrawCoronal();
		}else{
			insideCoronal=false;
//			coronalS = MIN(volHeight, MAX(depth, 0));

		}
	}
	else if(vP==SAGITTAL)
	{
		sagittalS = depth;
		if(depth>=0 && depth<volWidth)
		{
			insideSagittal=true;
			redrawSagittal();
		}else{
			insideSagittal=false;
//			sagittalS = MIN(volWidth, MAX(depth, 0));
		}
	}
	else if (vP==AXIAL)
	{
		axialS = depth;
		if(depth>=0 && depth<volDepth)
		{
			insideAxial=true;
			redrawAxial();
		}else{
			insideAxial=false;
//			axialS = MIN(volDepth, MAX(depth, 0));
		}
	}
}

//--------------------------------------------------------------
void volume::draw(viewPoint vP)
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
void volume::drawBox()
{
	// Draw Box
	ofPushStyle();
	ofSetColor(0);
	ofRect(0, 0, boxW, boxH);
	ofPopStyle();
	ofSetColor(255);
}

//--------------------------------------------------------------
void volume::redrawCoronal()
{
	for(int z=0; z<volDepth; z++)
	{
		for(int y=0; y<volHeight; y++)
		{
			if (y==coronalS){
				for(int x=0; x<volWidth; x++)
				{
					int line = y*volWidth;
					int page = z*volWidth*volHeight;
					int i = x + line + page;					// the position at the pixel array
					coronalPixels[x+(z*volWidth)] = voxels[i];	// get the correct voxel and put it to the pixel array
				}
			}
		}
	}
	
	//draw image
	coronal.setFromPixels(coronalPixels.getPixels(), volWidth, volDepth, OF_IMAGE_GRAYSCALE);
	coronal.mirror(true, false);
}

//--------------------------------------------------------------
void volume::redrawSagittal()
{
	for(int z=0; z<volDepth; z++)
    {
		for(int y=0; y<volHeight; y++)
        {
			for(int x=0; x<volWidth; x++)
			{
				if (x==sagittalS){
					int line = y*volWidth;
					int page = z*volWidth*volHeight;
					int i = x + line + page;					// the position at the pixel array
					sagittalPixels[z+(y*volDepth)] = voxels[i];	// get the correct voxel and put it to the pixel array
				}
			}
		}
    }
	
	//draw image
	sagittal.setFromPixels(sagittalPixels.getPixels(), volDepth, volHeight, OF_IMAGE_GRAYSCALE);
	sagittal.rotate90(3);
}

//--------------------------------------------------------------
void volume::redrawAxial()
{
	for(int z=0; z<volDepth; z++)
    {
		if (z==axialS){
			for(int y=0; y<volHeight; y++)
			{
				for(int x=0; x<volWidth; x++)
				{
					int line = y*volWidth;
					int page = z*volWidth*volHeight;
					int i = x + line + page;			// the position at the pixel array
					axialPixels[x+line] = voxels[i];	// get the correct voxel and put it to the pixel array
				}
			}
		}
    }
	
	//draw image
	axial.setFromPixels(axialPixels.getPixels(), volWidth, volHeight, OF_IMAGE_GRAYSCALE);
}


//--------------------------------------------------------------
ofVec3f volume::getVolSize(){
	return volSize;
}
//--------------------------------------------------------------
ofVec3f volume::getVolPos(){
	return volPos;
}

//--------------------------------------------------------------
unsigned char* volume::getVoxels(){
	return voxels;
}

/*
 //--------------------------------------------------------------
 void volume::drawCoronal(float x, float y, float z)
 {
 // this will test if z is within the limits and then draw the image
 // otherwise set to black (do not draw).
 drawBox();
 if(z>-1&&z<volHeight){					//this has to be erased and function drawCoronal must have no params
 coronal.draw(halfW, halfD);
 }
 }
 
 //--------------------------------------------------------------
 void volume::drawSagittal(float x, float y, float z)
 {
 drawBox();
 if(z>-1&&z<volWidth){
 sagittal.draw(halfH, halfD);
 }
 }
 
 //--------------------------------------------------------------
 void volume::drawAxial(float x, float y, float z)
 {
 drawBox();
 if(z>-1&&z<volDepth){
 axial.draw(halfW, halfH);
 }
 }
 */
