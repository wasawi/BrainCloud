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

	// Tweets Canvas
	tweetsCanvasX	= 570;
	tweetsCanvasY	= 110;
	tweetsCanvasW	= 600;
	tweetsCanvasH	= 400;
	bsnap			= false;
	
	dim				= 50;
	sliderW			= 20;
	WidgetW			= tweetsCanvasW-sliderW;
	space = OFX_UI_GLOBAL_WIDGET_SPACING;
	
	
	// Search field
	searchCanvasX	= tweetsCanvasX;
	searchCanvasY	= 60;
	searchCanvasW	= tweetsCanvasW+sliderW;
	searchCanvasH	= 50;

	searchFieldW	= 470;
	nResponses		= 15;
	nResponsesX		= searchFieldW+10;
	nResponsesY		= 18;
	nResponsesW		= 50;

	setupSearchInput();
	setupScrollCanvas();
}
//--------------------------------------------------------------
void guiManager::update(){

}
//--------------------------------------------------------------
void guiManager::draw(){
	ofPushStyle();
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
	
	ofPopStyle();
}

//--------------------------------------------------------------
void guiManager::setupSearchInput(){
	
	textInputCanvas = new ofxUICanvas(searchCanvasX, searchCanvasY,searchCanvasW, searchCanvasH);
	textInputCanvas->setWidgetFontSize(OFX_UI_FONT_LARGE);
//	textInputCanvas->setDrawBack(true);
	//	ofxUITextInput(string _name, string _textstring, float w, float h, float x, float y, int _size) :
/*	textInputCanvas->addWidget( new ofxUITextInput( "TEXT INPUT",
													   "",
													   searchFieldW,
													   searchFieldH,
													   10,
													   0,
													   OFX_UI_FONT_LARGE));
*/
	
	textInputCanvas->addTextInput("TEXT INPUT", "Search ", searchFieldW)->setAutoClear(true);
	textInputCanvas->setWidgetFontSize(OFX_UI_FONT_MEDIUM);

	
//	textInputCanvas->addNumberDialer("responses", 0, 100, nResponsesX,0, nResponses, 0)->getLabelWidget();
	// ofxUINumberDialer(float x, float y, float _min, float _max, float _value, int _precision, string _name, int _size)

	textInputCanvas->addWidget( new ofxUINumberDialer(nResponsesX,
														   nResponsesY,
														   0,
														   200,
														   nResponses,
														   0,
														   "responses",
														   OFX_UI_FONT_MEDIUM));

	ofAddListener(textInputCanvas->newGUIEvent,this,&guiManager::textInputEvent);
}


//--------------------------------------------------------------
void guiManager::setupScrollCanvas(){

	// Canvas for Tweets
	scrollCanvas = new ofxUIScrollableSliderCanvas(tweetsCanvasX, tweetsCanvasY, tweetsCanvasW, tweetsCanvasH, sliderW);
	scrollCanvas->setScrollArea(tweetsCanvasX, tweetsCanvasY, tweetsCanvasW, tweetsCanvasH);
	//gui->setScrollAreaHeight(tweetsCanvasH);
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
			ofLogVerbose("searchField") << "ON ENTER: ";
			//			ofUnregisterKeyEvents((guiManager*)this);
			static guiEvent newEvent;
			newEvent.message =  textinput->getTextString();
			newEvent.value	= nResponses;
			ofNotifyEvent(guiEvent::newSearch, newEvent);
		
		}else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_FOCUS){
			ofLogVerbose("searchField") << "ON FOCUS: ";
//			textinput->
//			textinput->recalculateDisplayString();
			//			ofRegisterKeyEvents((guiManager*)this);
			textinput->setTextString("");
		
		}else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_UNFOCUS){
			ofLogVerbose("searchField") << "ON BLUR: ";
		
		}else if(textinput->getTriggerType() == OFX_UI_TEXTINPUT_ON_LOAD){
			ofLogVerbose("searchField") << "ON LOAD: ";
		}
		string output = textinput->getTextString();
		cout << output << endl;
	 
	 }else if (name == "responses"){
		ofxUINumberDialer *numDialer = (ofxUINumberDialer *) e.widget;
		nResponses = numDialer->getValue();
		ofLogVerbose("searchField") << "nResponses: " << nResponses;
	 }
}




