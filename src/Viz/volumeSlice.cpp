#include "volumeSlice.h"

volumeSlice::volumeSlice()
{
    volWidth = renderWidth = 0;
    volHeight = renderHeight = 0;
    volDepth = 0;
	viewPoint = SAGITTAL;
}

volumeSlice::~volumeSlice()
{
}

void volumeSlice::setup(unsigned char * data, int w, int h, int d, enum viewPoint vP)
{
    volWidth	= renderWidth = w;
    volHeight	= renderHeight = h;
    volDepth	= d;
    viewPoint	= vP;
	myData		= data;
}

void volumeSlice::draw(float x, float y, int zTexOffset){

	if (viewPoint==SAGITTAL){
		drawSagital(x, y, zTexOffset);
	}else if(viewPoint==AXIAL){
		drawAxial(x, y, zTexOffset);
	}else if(viewPoint==CORONAL){
		drawCoronal(x, y, zTexOffset);
	}
	
}

void volumeSlice::drawSagital(float x, float y, int zTexOffset){

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
					myPixels[z+(y*volDepth)] = myData[i];				// the pixel on the image
				}
			}
		}
    }
	
	//draw pixels
	myImage.setFromPixels(myPixels.getPixels(), volDepth, volHeight, OF_IMAGE_GRAYSCALE);
	myImage.rotate90(3);
	myImage.draw(x, y, renderHeight, renderWidth);
}

void volumeSlice::drawAxial(float x, float y, int zTexOffset){
	
//	int volHeight = volDepth;
	
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
					myPixels[x+(z*volWidth)] = myData[i];				// the pixel on the image
				}
			}
		}
    }
	
	//draw pixels
	myImage.setFromPixels(myPixels.getPixels(), volWidth, volDepth, OF_IMAGE_GRAYSCALE);
//	myImage.rotate90(2);
	myImage.mirror(true, false);
	myImage.draw(x, y);
}

void volumeSlice::drawCoronal(float x, float y, int zTexOffset){
	
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
	
	//draw pixels
	myImage.setFromPixels(myPixels.getPixels(), volWidth, volHeight, OF_IMAGE_GRAYSCALE);
	myImage.draw(x, y);
}
