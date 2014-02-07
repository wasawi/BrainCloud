#include "volumeSlice.h"

volumeSlice::volumeSlice()
{
    volWidth = 0;
    volHeight = 0;
    volDepth = 0;
	viewPoint = SAGITTAL;
	
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
	
}
//--------------------------------------------------------------
void volumeSlice::draw(int zTexOffset, enum viewPoint vP){

	viewPoint	= vP;

	// needed to align the volume at the center of the box
	// Attention! this is not correct..
	// will give problems if W & H are not d same
	halfH = (boxW - volHeight) /2;
	halfW = (boxW - volWidth) /2;
	halfD = (boxW - volDepth) /2;
		
	// Draw Box
	ofPushStyle();
	ofSetColor(0);
	ofRect(0, 0, boxW, boxH);
	ofPopStyle();
	
	// Draw Volume2D
	ofSetColor(255);
	if(viewPoint==CORONAL){
		coronalS = zTexOffset;
		drawCoronal(halfW, halfD, zTexOffset);
		
	}else if(viewPoint==SAGITTAL){
		sagittalS = zTexOffset;
		drawSagittal(halfH, halfD, zTexOffset);

	}else if (viewPoint==AXIAL){
		axialS = zTexOffset;
		drawAxial(halfD, halfH, zTexOffset);
	}

	
}

//--------------------------------------------------------------
void volumeSlice::drawCoronal(float x, float y, int zTexOffset){
	
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
	myImage.setFromPixels(myPixels.getPixels(), volWidth, volDepth, OF_IMAGE_GRAYSCALE);
	myImage.mirror(true, false);
	myImage.draw(x, y);
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
void volumeSlice::drawSagittal(float x, float y, int zTexOffset){
	
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
	myImage.setFromPixels(myPixels.getPixels(), volDepth, volHeight, OF_IMAGE_GRAYSCALE);
	myImage.rotate90(3);
	myImage.draw(x, y);
	
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
void volumeSlice::drawAxial(float x, float y, int zTexOffset){
	
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
	myImage.setFromPixels(myPixels.getPixels(), volWidth, volHeight, OF_IMAGE_GRAYSCALE);
	myImage.draw(x, y);
	
	/*
	//Coronal X and Y LINES
	ofSetColor(lineColor);
	ofLine(0, axialS+ y, boxW,  axialS+ y);					//coronal hor line
	ofLine(sagittalS+ x, 0, sagittalS+ x, boxH);			//coronal vert
*/
}
