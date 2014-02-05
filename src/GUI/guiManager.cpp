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

	float xInit		= 400;
	float yInit		= 100;
    float CanvasW   = 300; // 550
    float CanvasH   = 200; // 400
	
	setGuiScrollingBar(xInit, yInit, CanvasW, CanvasH, false);
	
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
//	ofSetRectMode(OF_RECTMODE_CENTER);
	
	

}
//--------------------------------------------------------------

void guiManager::setGuiScrollingBar(float xInit, float yInit, float CanvasW, float CanvasH, bool bsnap){
	
	// Canvas for Tweets
	gui = new ofxUIScrollableSliderCanvas(xInit, yInit, CanvasW, CanvasH);

	gui->setScrollArea(xInit, yInit, CanvasW, CanvasH);
	//gui->setScrollAreaHeight(CanvasH);
	
	gui->setScrollableDirections(false, true);
	
	//reserve space for contends
	gui->addWidgetLeft(new ofxUILabel("TITLE", "Tweets", OFX_UI_FONT_LARGE));	// Title
	gui->addSpacer( CanvasW*0.5, 2 );
	
	//....
	
	ofAddListener(gui->newGUIEvent,this,&guiManager::guiEvent);
	
	adjustContendstoGui(bsnap);
	
}

void guiManager::adjustContendstoGui(bool _bsnap){
	
	if(_bsnap){
		gui->autoSizeToFitWidgets(); 
	}
	else {
		gui->setSnapping(_bsnap); //Auto damping levels only works for full size window
		gui->updateScrollBarSize(gui->getScroll()->getWidgets(), 3000 , 500); // set new default size depending contend inside // max , min
	}
}

void guiManager::addTwitterContend(ofImage img, int dim, int WidgetW, string nameuser, std::string myText, bool _bsnap){
	
	gui->addWidgetDown( new ofxUIImage( 0, 10, dim, dim, img, "", false)); // ofxUIImage , 0 
	
	//gui->addWidgetRight( new ofxUITextArea("USER", nameuser, WidgetW - (WidgetW/2), 0, 0, -100, OFX_UI_FONT_MEDIUM ), OFX_UI_ALIGN_FREE, false);
	
	cout << "Added USER text" << endl;
	gui->addTextArea("USER", nameuser, OFX_UI_FONT_MEDIUM);
	//gui->addWidgetRight( new ofxUITextArea("USER", nameuser, OFX_UI_FONT_MEDIUM), OFX_UI_ALIGN_FREE, false);
	cout << "Added USER text" << endl;
	

	float textsizeW = WidgetW - dim -10;

	float textsizeH = myText.size()*0.4;
	cout << "textsizeW " << textsizeW << endl;
	cout << "textsizeH " << textsizeH << endl;
	
	cout << "Go to Add TEXT text" << endl;
	
	//TODO this give error
	//myText = "\"It's a little-acknowledged fact, yet an unanswerable one, that states exist in great part to maintain a monopoly on violence\" - Deborah Orr";
	//gui->addWidgetRight( new ofxUITextArea("TEXT", myText, OFX_UI_FONT_SMALL), OFX_UI_ALIGN_RIGHT, false);
	
	gui->addTextArea("TEXT", myText, OFX_UI_FONT_SMALL);
	
	cout << "Added TEXT text" << endl;
	
	gui->addSpacer( WidgetW, 2 );
	
	adjustContendstoGui(false);
}

//--------------------------------------------------------------

void guiManager::exit(){
	delete gui;
    
	//Check number of Images createrd and delete them here
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