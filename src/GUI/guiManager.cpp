#include "guiManager.h"
//--------------------------------------------------------------

void guiManager::setup(){
	float dim		= 50;
	float xInit		= OFX_UI_GLOBAL_WIDGET_SPACING;
	float yInit		= 0;
    float CanvasW   = 550;
    float CanvasH   = 400;
	float ScrollW	= 40;
	float WidgetW	= CanvasW -ScrollW - (xInit * 4);
	string scrollBarname = "S";
	
	setGuiScrollingBar(scrollBarname, dim, xInit, yInit, CanvasW, CanvasH, ScrollW, WidgetW, false);
	
}
//--------------------------------------------------------------
void guiManager::update(){

}
//--------------------------------------------------------------
void guiManager::draw(){
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	//gui->autoSizeToFitWidgets();
	//	gui->getRect()->setWidth(ofGetWidth());
	gui->setDrawBack(true);
	gui->setDrawPadding(true);
	gui->drawPaddedOutline();
	gui->drawOutlineHighlight();
	
	//gui_slider->setDrawBack(true);
	//gui_slider->setDrawPadding(true);
	//gui_slider->drawOutlineHighlight();
	
	ofPopStyle();
	ofSetRectMode(OF_RECTMODE_CENTER);
	
	

}
//--------------------------------------------------------------

void guiManager::setGuiScrollingBar(string scrollBarname, float dim, float xInit, float yInit, float CanvasW, float CanvasH, float ScrollW, float WidgetW, bool bsnap){
	
	// Canvas for Tweets
	gui = new ofxUIScrollableSliderCanvas(0, 0, CanvasW, CanvasH);
	gui->setupScrollBar(scrollBarname, 0, CanvasH, CanvasH-20, CanvasH, 25, CanvasH, xInit+CanvasW, 0, OFX_UI_FONT_SMALL);
	
	gui->setScrollAreaHeight(CanvasH);
	gui->setScrollableDirections(false, true);
	
	gui->addWidgetLeft(new ofxUILabel("TITLE", "Tweets", OFX_UI_FONT_LARGE));	// Title
	gui->addSpacer( WidgetW, 2 );
	
	//Load Contend	Tweets
	img = new ofImage();
    img->loadImage("images/bikers.jpg");
    myText = "\"It's a little-acknowledged fact, yet an unanswerable one, that states exist in great part to maintain a monopoly on violence\" - Deborah Orr";
	
	
	for (int i=0; i<30; i++) {
		gui->addWidgetDown( new ofxUIImage( 0, 10, dim, dim, img, "", 0 ));
		gui->addWidgetRight( new ofxUITextArea("USER", "Barbara", WidgetW - (WidgetW/2), 0, 0, -100, OFX_UI_FONT_MEDIUM ), OFX_UI_ALIGN_FREE, false);
		gui->addWidgetDown( new ofxUITextArea("TEXT", myText, WidgetW - dim -10, myText.size()*.4, 0, 0, OFX_UI_FONT_SMALL ),OFX_UI_ALIGN_RIGHT, false);
    }
	
	gui->addSpacer( WidgetW, 2 );
	
	if(bsnap){
		gui->autoSizeToFitWidgets();//works*: only setting full window heigth
	}
	else {
		gui->setSnapping(bsnap); //Auto damping levels only works for full size window
		gui->updateScrollBarSize(gui->getScroll()->getWidgets(), 3000 , 500); // set new default size depending contend inside // max , min
	}
	
	ofAddListener(gui->newGUIEvent,this,&guiManager::guiEvent);
	
}

//--------------------------------------------------------------

void guiManager::exit(){
	delete gui;
    delete img;
}


//--------------------------------------------------------------
void guiManager::guiEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	int kind = e.widget->getKind();
	
	
	/*
	 if(name == "TEXT INPUT")
	 {
	 auto textinput = (ofxUITextInput *) e.widget;
	 if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_ENTER)
	 {
	 cout << "ON ENTER: ";
	 //            ofUnregisterKeyEvents((ofApp*)this);
	 }
	 else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_FOCUS)
	 {
	 cout << "ON FOCUS: ";
	 }
	 else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS)
	 {
	 cout << "ON BLUR: ";
	 //            ofRegisterKeyEvents(this);
	 }
	 string output = textinput->getTextString();
	 cout << output << endl;
	 }
	 */
}