#include "guiManager.h"

// SINGLETON initalizations
bool guiManager::instanceFlag = false;
guiManager* guiManager::single = NULL;

//----------------------------------------------

guiManager* guiManager::getInstance()
{
    if(! instanceFlag)
    {
        single = new guiManager();
        instanceFlag = true;
        return single;
    }else{
        return single;
    }
}

//----------------------------------------------

guiManager::guiManager()
{
}

//----------------------------------------------
guiManager::~guiManager()
{
}

//--------------------------------------------------------------

void guiManager::setup(){

	float canvasX	= 570;
	float canvasY	= 110;
    float CanvasW   = 600; // 550
    float CanvasH   = 400; // 400
	
	dim					= 50;
	sliderW				= 20;
	WidgetW				= CanvasW-sliderW;
	float textInputY	= dim;
	
	setupTextInput(canvasX, textInputY, CanvasW, 40);
	setupScrollCanvas(canvasX, canvasY, CanvasW, CanvasH, false);
}
//--------------------------------------------------------------
void guiManager::update(){

}
//--------------------------------------------------------------
void guiManager::draw(){
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
//	gui->autoSizeToFitWidgets();
//	gui->getRect()->setWidth(ofGetWidth());
//	gui->setDrawPadding(true);
//	gui->drawPaddedOutline();
//	gui->drawOutlineHighlight();
	
//	gui_slider->setDrawBack(true);
//	gui_slider->setDrawPadding(true);
//	gui_slider->drawOutlineHighlight();
	
	ofPopStyle();
//	ofSetRectMode(OF_RECTMODE_CENTER);
}

//--------------------------------------------------------------
void guiManager::setupTextInput(float canvasX, float textInputY, float CanvasW, float CanvasH){
	
	textInputCanvas = new ofxUICanvas(canvasX, textInputY, CanvasW, CanvasH);
	//	string _name, string _textstring, float w, float h, float x, float y, int _size) :
	textInputCanvas->addWidget( new ofxUITextInput( "TEXT INPUT","", CanvasW, CanvasH, 0, 0, OFX_UI_FONT_LARGE));
	
	ofAddListener(textInputCanvas->newGUIEvent,this,&guiManager::textInputEvent);
}


//--------------------------------------------------------------
void guiManager::setupScrollCanvas(float canvasX, float canvasY, float CanvasW, float CanvasH, bool bsnap){

	// Canvas for Tweets
	scrollCanvas = new ofxUIScrollableSliderCanvas(canvasX, canvasY, CanvasW, CanvasH, sliderW);
	scrollCanvas->setPadding(0);
	scrollCanvas->setScrollArea(canvasX, canvasY, CanvasW, CanvasH);
	//gui->setScrollAreaHeight(CanvasH);
	scrollCanvas->setScrollableDirections(false, true);
	
	ofAddListener(scrollCanvas->newGUIEvent,this,&guiManager::scrollCanvasEvent);
	adjustContentstoGui(bsnap);
}

void guiManager::adjustContentstoGui(bool _bsnap){
	
	if(_bsnap){
		scrollCanvas->autoSizeToFitWidgets();
	}
	else {
		scrollCanvas->setSnapping(_bsnap); //Auto damping levels only works for full size window
		scrollCanvas->updateScrollBarSize(scrollCanvas->getScroll()->getWidgets(), 3000 , 500); // set new default size depending content inside // max , min
	}
}

void guiManager::addTwitterContent(ofImage img, string name, string user_name, std::string tweetText){
	
	float space = OFX_UI_GLOBAL_WIDGET_SPACING;
	float lineHeight = scrollCanvas->getFont()->getLineHeight();
	scrollCanvas->addWidgetDown( new ofxUIImage( 0, space, dim, dim, img, "IMAGE", false));
//    ofxUITextArea(string _name, string _textstring, float w, float h = 0, float x = 0, float y = 0, int _size = OFX_UI_FONT_MEDIUM);
	
	scrollCanvas->addWidgetRight( new ofxUITextArea("NAME",
										   name,
										   (WidgetW - dim)/3,
										   0,
										   0,
										   dim*-1. - space,
										   OFX_UI_FONT_MEDIUM ),
						OFX_UI_ALIGN_FREE, false)->setDrawBack(false);

	scrollCanvas->addWidgetDown( new ofxUITextArea("USER",
										   "@"+user_name,
										   (WidgetW - dim)/3,
										   0,
										   dim + space*2,
										   0,
										   OFX_UI_FONT_SMALL ),
						OFX_UI_ALIGN_FREE, false)->setDrawBack(false);
/*
	gui->addWidget( new ofxUITextArea("TEXT",
										  tweetText,
										  WidgetW,
										  0,
										  space,
										  40,
									  OFX_UI_FONT_MEDIUM ));
*/

	scrollCanvas->addWidgetDown( new ofxUITextArea("TEXT",
										  tweetText,
										  WidgetW,
										  0,
										  space,
										  (lineHeight*-2)+dim+space*2,
										  OFX_UI_FONT_MEDIUM ),
					   OFX_UI_ALIGN_FREE, false)->setDrawBack(false);

	scrollCanvas->addSpacer( WidgetW, 1 );
	adjustContentstoGui(false);
}

//--------------------------------------------------------------
void guiManager::exit(){
	delete scrollCanvas;
    
	//Check number of Images createrd and delete them here
}
//--------------------------------------------------------------
void guiManager::scrollCanvasEvent(ofxUIEventArgs &e)
{
}

//--------------------------------------------------------------
void guiManager::textInputEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	
	if(name == "TEXT INPUT")
	 {
		ofxUITextInput *textinput = (ofxUITextInput *) e.widget;
		if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER){
			cout << "ON ENTER: ";
//			ofUnregisterKeyEvents((ofApp*)this);
			tweetManager::getInstance()->searchQuery(textinput->getTextString());
		}
		else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_FOCUS){
			cout << "ON FOCUS: ";
			textinput->setTextString("");
		}
		else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS){
			cout << "ON BLUR: ";
//			ofRegisterKeyEvents(this);
		}
		 if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_LOAD){
			cout << "ON LOAD: ";
		}
		string output = textinput->getTextString();
		cout << output << endl;
	 }
}




