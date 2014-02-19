#include "volumeSlice.h"

volumeSlice::volumeSlice()
{
    volWidth = 0;
    volHeight = 0;
    volDepth = 0;
	myViewPoint = SAGITTAL;
	
	lineColor.set(150,150,150);
}
//--------------------------------------------------------------
volumeSlice::~volumeSlice()
{
}

//--------------------------------------------------------------
void volumeSlice::setup(unsigned char * data, int w, int h, int d, float bW, float bH)
{
    volWidth	= w;
    volHeight	= h;
    volDepth	= d;
	myData		= data;
	boxW = bW;
	boxH = bH;
	
	// needed to align the volume at the center of the box
	// Attention! this is not correct..
	// will give problems if W & H are not d same
	halfH = (boxW - volHeight) /2;
	halfW = (boxW - volWidth) /2;
	halfD = (boxW - volDepth) /2;
}

//--------------------------------------------------------------
int volumeSlice::getVoxelValue(){
	
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
							value= myData[i];							// the pixel on the image
						}
					}
				}
			}
		}
	}

	ofLogVerbose("volumeSlice") << "sagittalS= " << sagittalS;
	ofLogVerbose("volumeSlice") << "coronalS= " << coronalS;
	ofLogVerbose("volumeSlice") << "axialS= " << axialS;
	ofLogVerbose("volumeSlice") << "voxelValue= " << value;
	return value;
}

//--------------------------------------------------------------
void volumeSlice::redraw(int zTexOffset, viewPoint vP){

	myViewPoint	= vP;
	
	if(myViewPoint==CORONAL){
		coronalS = zTexOffset;
		redrawCoronal(zTexOffset);
		
	}else if(myViewPoint==SAGITTAL){
		sagittalS = zTexOffset;
		redrawSagittal(zTexOffset);

	}else if (myViewPoint==AXIAL){
		axialS = zTexOffset;
		redrawAxial(zTexOffset);
	}
}
//--------------------------------------------------------------
void volumeSlice::drawBox(){
	// Draw Box
	ofPushStyle();
	ofSetColor(0);
	ofRect(0, 0, boxW, boxH);
	ofPopStyle();
	ofSetColor(255);
}
//--------------------------------------------------------------
void volumeSlice::drawCoronal(float x, float y, float z){
	drawBox();
	if(z>-1&&z<volHeight){
		coronal.draw(halfW, halfD);
	}
}


//--------------------------------------------------------------
void volumeSlice::drawSagittal(float x, float y, float z){
	drawBox();
	if(z>-1&&z<volWidth){
		sagittal.draw(halfH, halfD);
	}
}

//--------------------------------------------------------------
void volumeSlice::drawAxial(float x, float y, float z){
	drawBox();
	if(z>-1&&z<volDepth){
		axial.draw(halfW, halfH);
	}
}

//--------------------------------------------------------------
void volumeSlice::redrawCoronal(int zTexOffset){
	
	//allocate my pixls size of the resulting image
	myPixels.allocate(volWidth, volDepth, OF_IMAGE_GRAYSCALE);
	
	//fill out the added pixel in white.. just to see any errors
	for (int i=0; i<volWidth * volDepth; i++ ){
		myPixels[i]= (unsigned char) 255;
	}
	
	for(int z=0; z<volDepth; z++)
	{
		for(int y=0; y<volHeight; y++)
		{
			if (y==zTexOffset){
				for(int x=0; x<volWidth; x++)
				{
					int line = y*volWidth;
					int page = z*volWidth*volHeight;
					int i = x + line + page;					// the pointer position at Array
					myPixels[x+(z*volWidth)] = myData[i];		// the pixel on the image
				}
			}
		}
	}
	
	//draw 2dVolume
	coronal.setFromPixels(myPixels.getPixels(), volWidth, volDepth, OF_IMAGE_GRAYSCALE);
	coronal.mirror(true, false);
	/*
	 //Axial X and Y LINES
	 int	invCoronalS;
	 invCoronalS = ofMap(coronalS, 0, volWidth-1, volWidth-1, 0);
	 ofSetColor(lineColor);
	 ofLine(0, invCoronalS+ y, boxW, invCoronalS+ y);			//axial hor line
	 ofLine(sagittalS+ x, 0, sagittalS+ x, boxH);				//axial vert line
	 */
}


//--------------------------------------------------------------
void volumeSlice::redrawSagittal(int zTexOffset){
	
	//allocate my pixls size of the resulting image
	myPixels.allocate(volDepth, volHeight, OF_IMAGE_GRAYSCALE);
	
	//fill out the added pixel in white.. just to see any errors
	for (int i=0; i<volDepth * volHeight; i++ ){
		myPixels[i]= (unsigned char) 255;
	}
	
	for(int z=0; z<volDepth; z++)
    {
		for(int y=0; y<volHeight; y++)
        {
			for(int x=0; x<volWidth; x++)
			{
				if (x==zTexOffset){
					int line = y*volWidth;
					int page = z*volWidth*volHeight;
					int i = x + line + page;					// the pointer position at Array
					myPixels[z+(y*volDepth)] = myData[i];		// the pixel on the image
				}
			}
		}
    }
	
	//draw 2dVolume
	sagittal.setFromPixels(myPixels.getPixels(), volDepth, volHeight, OF_IMAGE_GRAYSCALE);
	sagittal.rotate90(3);
	
	/*
	 // Sagittal X and Y LINES
	 int	invCoronalS;
	 invCoronalS = ofMap(coronalS, 0, volWidth-1, volWidth-1, 0);
	 ofSetColor(lineColor);
	 ofLine(0, invCoronalS+ y, boxW, invCoronalS+ y);		//sagital hor line
	 ofLine(axialS+ x, 0, axialS+ x, boxH);					//sagital vert line
	 */
}

//--------------------------------------------------------------
void volumeSlice::redrawAxial(int zTexOffset){
	
	//allocate my pixls size of the resulting image
	myPixels.allocate(volWidth, volHeight, OF_IMAGE_GRAYSCALE);
	
	//fill out the added pixel in white.. just to see any errors
	for (int i=0; i<volWidth * volHeight; i++ ){
		myPixels[i]= (unsigned char) 255;
	}
	
	for(int z=0; z<volDepth; z++)
    {
		if (z==zTexOffset){
			for(int y=0; y<volHeight; y++)
			{
				for(int x=0; x<volWidth; x++)
				{
					int line = y*volWidth;
					int page = z*volWidth*volHeight;
					int i = x + line + page;					// the pointer position at Array
					myPixels[x+line] = myData[i];				// the pixel on the image
				}
			}
		}
    }
	
	//draw 2dVolume
	axial.setFromPixels(myPixels.getPixels(), volWidth, volHeight, OF_IMAGE_GRAYSCALE);

	/*
	 //Coronal X and Y LINES
	 ofSetColor(lineColor);
	 ofLine(0, axialS+ y, boxW,  axialS+ y);					//coronal hor line
	 ofLine(sagittalS+ x, 0, sagittalS+ x, boxH);			//coronal vert
	 */
}


