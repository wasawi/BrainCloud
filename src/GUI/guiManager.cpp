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
	
	//reserve space for contends
	gui->addWidgetLeft(new ofxUILabel("TITLE", "Tweets", OFX_UI_FONT_LARGE));	// Title
	gui->addSpacer( WidgetW, 2 );
	
	//....
	
	ofAddListener(gui->newGUIEvent,this,&guiManager::guiEvent);
	
	adjustContendstoGui(bsnap);
	
}

void guiManager::adjustContendstoGui(bool _bsnap){
	
	if(_bsnap){
		gui->autoSizeToFitWidgets();//works*: only setting full window heigth
	}
	else {
		gui->setSnapping(_bsnap); //Auto damping levels only works for full size window
		gui->updateScrollBarSize(gui->getScroll()->getWidgets(), 3000 , 500); // set new default size depending contend inside // max , min
	}
}

void guiManager::addTwitterContend(ofImage img, int dim, int WidgetW, string nameuser, string myText, bool _bsnap){
	
	//Load Contend	Tweets
	//ofImage _img;
	//_img = new ofImage();
    //_img.loadImage("images/bikers.jpg");
    //myText = "\"It's a little-acknowledged fact, yet an unanswerable one, that states exist in great part to maintain a monopoly on violence\" - Deborah Orr";
	
	gui->addWidgetDown( new ofxUIImage( 0, 10, dim, dim, img, "", false)); // ofxUIImage , 0 
	
	//gui->addWidgetRight( new ofxUITextArea("USER", nameuser, WidgetW - (WidgetW/2), 0, 0, -100, OFX_UI_FONT_MEDIUM ), OFX_UI_ALIGN_FREE, false);
	gui->addTextArea("USER", nameuser, OFX_UI_FONT_SMALL);
	cout << "Added USER text" << endl;
	
	/*
	vector<string> lines = ofSplitString(myText, "\n");
	for (int i=0; i<lines.size(); i++) {
	 float textsizeH = lines[i].size()*0.4;
	 float textsizeW = WidgetW - dim -10;
		
		cout << "textsizeH=" << textsizeH << endl;
		cout << "textsizeW=" << textsizeW << endl;
		gui->addWidgetDown( new ofxUITextArea("TEXT", lines[i], textsizeW, textsizeH , 0, 0, OFX_UI_FONT_SMALL ),OFX_UI_ALIGN_RIGHT, false);
		
		cout << "Added text line=" << i << endl;
	}
	*/
	
	float textsizeH = myText.size()*0.4;
	float textsizeW = WidgetW - dim -10;
	
	/*
	 myText = myText +"BLABLA"; 
	 cout << "TEXT to add=" << myText << endl;
	gui->addWidgetDown( new ofxUITextArea("TEXT", myText, textsizeH, textsizeW, 0, 0, OFX_UI_FONT_SMALL ),OFX_UI_ALIGN_RIGHT, false);
	 */
	
	gui->addTextArea("TEXT", myText, OFX_UI_FONT_SMALL);
	cout << "Added TEXT text" << endl;
	
	gui->addSpacer( WidgetW, 2 );
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