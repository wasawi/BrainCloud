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
	
	int lineHeight = 35;
	int totalHeight = 500;
	
	// tabCanvas
	tabCanvasX		= 570;
	tabCanvasY		= 50;
	tabCanvasW		= 600;
	tabCanvasH		= lineHeight+OFX_UI_GLOBAL_SPACING_HEIGHT*2;
	toggleW			= 100;
	toggleH			= lineHeight;
	
	// Search field
	searchCanvasX	= tabCanvasX+1;
	searchCanvasY	= tabCanvasY+tabCanvasH-1;
	searchCanvasW	= tabCanvasW;
	searchCanvasH	= lineHeight+OFX_UI_GLOBAL_WIDGET_SPACING*2;

	searchFieldX	= 0;
	searchFieldY	= lineHeight;
	searchFieldW	= searchCanvasW-OFX_UI_GLOBAL_WIDGET_SPACING*2;
	searchFieldH	= lineHeight;
	
	nResponses		= 10;
	nResponsesX	= searchFieldW+10;
	nResponsesY	= 6;
	nResponsesW	= 50;

	// Tweets Canvas
	dim				= 50;
	sliderW			= 20;
	space			= OFX_UI_GLOBAL_WIDGET_SPACING;
	WidgetW			= tabCanvasW-sliderW-OFX_UI_GLOBAL_WIDGET_SPACING*2;
	bsnap			= false;
	
	tweetsCanvasX	= searchCanvasX;
	tweetsCanvasY	= searchCanvasY+searchCanvasH;
	tweetsCanvasW	= tabCanvasW-sliderW;
	tweetsCanvasH	= totalHeight+tabCanvasH;

	// postCanvas
	postCanvasX	= searchCanvasX;
	postCanvasY	= searchCanvasY;
	postCanvasW	= searchCanvasW;
	postCanvasH	= totalHeight+tabCanvasH+searchCanvasH;
	postFieldH	= lineHeight*2;
	
	// Tab bar
	myTabselector = SEARCH;
	setupTabBar();
}
//--------------------------------------------------------------
void guiManager::update(){

}
//--------------------------------------------------------------
void guiManager::draw(){
//	ofPushStyle();
//	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
//	ofPopStyle();
}

//--------------------------------------------------------------
void guiManager::setupTabBar()
{
	tabCanvas = new ofxUICanvas(tabCanvasX, tabCanvasY,tabCanvasW, tabCanvasH);
	tabCanvas->setDrawBack(false);
	tabCanvas->setWidgetSpacing(1);
	tabCanvas->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
	
	//	(string _name, bool _value, float w, float h, float x, float y, bool _justifyLeft)
	tabCanvas->addLabelToggle("Search", searchToggle, toggleW, toggleH, 0, 0, false);
	tabCanvas->setWidgetSpacing(10);
	tabCanvas->addWidgetRight(new ofxUILabelToggle("Post",
												   postToggle,
												   toggleW,
												   toggleH,
												   toggleW,
												   0,
												   OFX_UI_FONT_MEDIUM,
												   false));
	// set properties
	ofxUILabelToggle *w = (ofxUILabelToggle *)  tabCanvas->getWidget("Search");
	ofColor back = ofColor(0,0,0,55);		// OFX_UI_COLOR_PADDED / 2
	w->setColorFill(back);
	w->setColorFillHighlight(back);
	w->setColorBack(back);
	w->setValue(true);						// we will use XML settings for this
	
	w = (ofxUILabelToggle *)  tabCanvas->getWidget("Post");
	w->setColorFill(back);
	w->setColorFillHighlight(back);
	w->setColorBack(back);
	w->setValue(false);						// we will use XML settings for this
	
	ofAddListener(tabCanvas->newGUIEvent,this,&guiManager::tabCanvasEvent);
	
	
	// setup UIs
	setupSearchCanvas();
	setupScrollCanvas();
	setupPostCanvas();
}

//--------------------------------------------------------------
void guiManager::changeTabBar()
{
	if (myTabselector == SEARCH){
		
		postCanvas->setVisible(false);
		scrollCanvas->setVisible(true);
		textInputCanvas->setVisible(true);
		
	}else if(myTabselector == POST){

		postCanvas->setVisible(true);
		scrollCanvas->setVisible(false);
		textInputCanvas->setVisible(false);

	}else if(myTabselector == FILTER){
		
	}
}


//--------------------------------------------------------------
void guiManager::setupPostCanvas(){
    
	postCanvas = new ofxUICanvas(postCanvasX, postCanvasY,postCanvasW, postCanvasH);
	postCanvas->setVisible(false);
	postCanvas->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
	postCanvas->addTextInput("PostField", "Type here ", searchFieldW, postFieldH, searchFieldX, searchFieldY)->setAutoClear(true);
	
	postCanvas->addTextInput("URL", "URL here ", searchFieldW, postFieldH, searchFieldX, 0)->setAutoClear(true);
	
	postCanvas->addWidgetDown(new ofxUILabelButton("Post",
												   postToggle,
												   toggleW,
												   toggleH,
												   toggleW,
												   0,
												   OFX_UI_FONT_MEDIUM,
												   false));

	ofAddListener(postCanvas->newGUIEvent,this,&guiManager::postCanvasEvent);
}

//--------------------------------------------------------------
void guiManager::setupSearchCanvas(){

	
	textInputCanvas = new ofxUICanvas(searchCanvasX, searchCanvasY,searchCanvasW, searchCanvasH);
	textInputCanvas->setWidgetFontSize(OFX_UI_FONT_MEDIUM);
	ofLogNotice("textInputCanvas.getGlobalSpacerHeight") << textInputCanvas->getGlobalSpacerHeight();
	ofLogNotice("textInputCanvas.getPadding") << textInputCanvas->getPadding();
	ofLogNotice("textInputCanvas.getWidgetSpacing") << textInputCanvas->getWidgetSpacing();

//	textInputCanvas->setDrawBack(true);
/*	textInputCanvas->addWidget( new ofxUITextInput( "TEXT INPUT",
													   "",
													   searchFieldW,
													   searchFieldH,
													   10,
													   0,
													   OFX_UI_FONT_LARGE));
*/

	// (string _name, string _textstring, float w, float h, float x, float y, int _size)
	textInputCanvas->addTextInput("TEXT INPUT", "Type here ", searchFieldW, searchFieldH, searchFieldX, searchFieldY)->setAutoClear(true);
//	textInputCanvas->setWidgetFontSize(OFX_UI_FONT_MEDIUM);

	// ofxUINumberDialer(float x, float y, float _min, float _max, float _value, int _precision, string _name, int _size)
/*	textInputCanvas->addWidget( new ofxUINumberDialer(nResponsesX,
														   nResponsesY,
														   0,
														   200,
														   nResponses,
														   0,
														   "responses",
														   OFX_UI_FONT_MEDIUM));
*/
	ofAddListener(textInputCanvas->newGUIEvent,this,&guiManager::textInputEvent);
}


//--------------------------------------------------------------
void guiManager::setupScrollCanvas(){

	// Canvas for Tweets
	scrollCanvas = new ofxUIScrollableSliderCanvas(tweetsCanvasX, tweetsCanvasY, tweetsCanvasW, tweetsCanvasH, sliderW);
	scrollCanvas->setScrollArea(tweetsCanvasX, tweetsCanvasY, tweetsCanvasW, tweetsCanvasH);
	scrollCanvas->setScrollableDirections(false, true);
//	scrollCanvas->addSpacer( WidgetW, 1 );
	
	ofLogNotice("scrollCanvas.getGlobalSpacerHeight") << scrollCanvas->getGlobalSpacerHeight();
	ofLogNotice("scrollCanvas.getPadding") << scrollCanvas->getPadding();
	ofLogNotice("scrollCanvas.getWidgetSpacing") << scrollCanvas->getWidgetSpacing();

	
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

	
	// clean emojis:
	name = removeEmojis(name);
	user_name = removeEmojis(user_name);
	tweetText = removeEmojis(tweetText);
	
	float lineHeight = scrollCanvas->getFont()->getLineHeight();
	//(float x, float y, float w, float h, ofImage _image, string _name, bool _showLabel);
	scrollCanvas->addWidgetDown( new ofxUIImage( 0, 0, dim, dim, img, "IMAGE", false));

	//(string _name, string _textstring, float w, float h = 0, float x = 0, float y = 0, int _size = OFX_UI_FONT_MEDIUM);
	scrollCanvas->addWidgetRight( new ofxUITextArea("NAME",
										   name,
										   (WidgetW - dim)/2,
										   0,
										   0,
										   dim*-1. - space,
										   OFX_UI_FONT_MEDIUM ),
						OFX_UI_ALIGN_FREE, false)->setDrawBack(false);

	scrollCanvas->addWidgetDown( new ofxUITextArea("USER",
										   "@"+user_name,
										   (WidgetW - dim)/2,
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
										  lineHeight*-1.8+dim,	// <- this has to be fixed
										  OFX_UI_FONT_MEDIUM ),
					   OFX_UI_ALIGN_FREE, false)->setDrawBack(false);

	scrollCanvas->addSpacer( WidgetW, 1 );
	adjustContentstoGui(bsnap);
}

//--------------------------------------------------------------
void guiManager::exit(){
	delete tabCanvas;
	delete postCanvas;
	delete scrollCanvas;
	delete textInputCanvas;
	
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
			//ofUnregisterKeyEvents((guiManager*)this);
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

//--------------------------------------------------------------
void guiManager::tabCanvasEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
	
	if(name == "Search"){
		myTabselector=SEARCH;

		searchToggle = true;
		postToggle=false;
		
		toggle = (ofxUILabelToggle *)  tabCanvas->getWidget("Search");
		toggle->setValue(searchToggle);

		toggle = (ofxUILabelToggle *)  tabCanvas->getWidget("Post");
		toggle->setValue(postToggle);
		
		changeTabBar();
		ofLogVerbose("tabCanvasEvent") << "searchToggle: " << toggle->getValue();

	}else if (name == "Post"){
		myTabselector=POST;
		
		postToggle=true;
		searchToggle = false;
		
		toggle = (ofxUILabelToggle *)  tabCanvas->getWidget("Search");
		toggle->setValue(searchToggle);
		
		toggle = (ofxUILabelToggle *)  tabCanvas->getWidget("Post");
		toggle->setValue(postToggle);
		
		changeTabBar();
		ofLogVerbose("tabCanvasEvent") << "postToggle: " << toggle->getValue();
	}
}


//--------------------------------------------------------------
void guiManager::postCanvasEvent(ofxUIEventArgs &e)
{
	string name = e.widget->getName();
	ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
	
	if(name == "Search"){
		myTabselector=SEARCH;
		
		searchToggle = true;
		postToggle=false;
		
		toggle = (ofxUILabelToggle *)  tabCanvas->getWidget("Search");
		toggle->setValue(searchToggle);
		
		toggle = (ofxUILabelToggle *)  tabCanvas->getWidget("Post");
		toggle->setValue(postToggle);
		
		changeTabBar();
		ofLogVerbose("tabCanvasEvent") << "searchToggle: " << toggle->getValue();
		
	}else if (name == "Post"){
		myTabselector=POST;
		
		postToggle=true;
		searchToggle = false;
		
		toggle = (ofxUILabelToggle *)  tabCanvas->getWidget("Search");
		toggle->setValue(searchToggle);
		
		toggle = (ofxUILabelToggle *)  tabCanvas->getWidget("Post");
		toggle->setValue(postToggle);
		
		changeTabBar();
		ofLogVerbose("tabCanvasEvent") << "postToggle: " << toggle->getValue();
	}
}

//--------------------------------------------------------------
string guiManager::removeEmojis(string s){
	
	string returnString = "";
	int l = s.length();
	int i;
	
	// for each byte
	for (i = 0; i < l; ++i) {
		char mychar = s[i];
		string binary = ofToBinary(mychar);
		binary =  binary.substr(0,4);
		if (binary == "1111"){
			i+=3;		//jump to next char
			//			cout << "Found Emoji, killing... " << endl;
		}else{
			returnString += mychar;
		}
	}
	return returnString;
}