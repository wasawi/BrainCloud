#pragma once

#include "ofMain.h"
#include "guiEvent.h"
#include "SQLiteC++.h"

class SQLiteManager {
public:
	SQLiteManager();
	~SQLiteManager();
	
	void setup(string filePath);
	void insert(guiEvent &e);
	void search(guiEvent &e);
	
private:
	
	string dbPath;
	string table_1;

};