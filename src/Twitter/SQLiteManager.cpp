#include "SQLiteManager.h"

//----------------------------------------------
SQLiteManager::SQLiteManager(){

	dbPath = "SQLite/NTDB.sqlite";
}

//----------------------------------------------
SQLiteManager::~SQLiteManager(){
	
}

//--------------------------------------------------------------
void SQLiteManager::setup(string db){
	dbPath= db;
}

//--------------------------------------------------------------
void SQLiteManager::insert(guiEvent &e) {
	
    /// Load Database
    std::string exampleDB = ofToDataPath(dbPath, true);

	try
    {
        // Open a database file in create/write mode
        SQLite::Database db(exampleDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE);
		
        ofLogNotice() << "SQLite database file '" << db.getFilename() << "' opened successfully";
		
        db.exec("DROP TABLE IF EXISTS test");
        db.exec("CREATE TABLE test (id INTEGER PRIMARY KEY, value TEXT)");
		
        // first row
        int nb = db.exec("INSERT INTO test VALUES (NULL, \"test\")");
		
        ofLogNotice() << "INSERT INTO test VALUES (NULL, \"test\")\", returned " << nb;
		
        // second row
        nb = db.exec("INSERT INTO test VALUES (NULL, \"second\")");
		
        ofLogNotice() << "INSERT INTO test VALUES (NULL, \"second\")\", returned " << nb;
		
        // update the second row
        nb = db.exec("UPDATE test SET value=\"second-updated\" WHERE id='2'");
		
        ofLogNotice() << "UPDATE test SET value=\"second-updated\" WHERE id='2', returned " << nb;
		
        // Check the results : expect two row of result
        SQLite::Statement query(db, "SELECT * FROM test");
		
        ofLogNotice() << "SELECT * FROM test :";
		
        while (query.executeStep())
        {
            ofLogNotice() << "row (" << query.getColumn(0) << ", \"" << query.getColumn(1) << "\")";
        }
		
		//db.exec("DROP TABLE test");
    }
    catch (std::exception& e)
    {
        ofLogError() << "SQLite exception: " << e.what();
    }
}


//--------------------------------------------------------------
void SQLiteManager::search(guiEvent &e) {
	
    /// Load Database
    std::string exampleDB = ofToDataPath(dbPath, true);
	
    try
    {
        // Open a database file in readonly mode
        SQLite::Database db(exampleDB);  // SQLITE_OPEN_READONLY
		
        // Test if the 'test' table exists
        bool testExists = db.tableExists(table_1);
		
        if (testExists)
        {
            ofLogVerbose("SQLiteManager::search") << "Table found.";
        }
        else
        {
            ofLogError("SQLiteManager::search") << "Test table did NOT exist!";
        }
		
        // Get a single value result with an easy to use shortcut
        std::string value = db.execAndGet("SELECT value FROM test WHERE id = 2");
		
        ofLogNotice("SQLiteManager::search") << "Value was: " << value;
		
        std::string queryString = "";
        queryString += "SELECT id AS test_id, value AS test_val, weight AS test_weight ";
        queryString += "FROM "+table_1+" ";
        queryString += "WHERE weight > ?"; // We will bind a variable to the ?.
		
        // Compile a SQL query, containing one parameter (index 1)
        SQLite::Statement query(db, queryString);
		
        // See how many columns are in our prepared statement:
		
        ofLogNotice("SQLiteManager::search") << "SQLite statement '" << query.getQuery() << "' compiled (" << query.getColumnCount() << " columns in the result)";
		
        // Bind the integer value 2 to the first parameter of the SQL query
        query.bind(1, 2);
		
        // Loop to execute the query step by step, to get one a row of results at a time
        while (query.executeStep())
        {
            // Demonstrate how to get some typed column value (and the equivalent explicit call)
            int         id      = query.getColumn(0); // = query.getColumn(0).getInt()
            std::string value2  = query.getColumn(1); // = query.getColumn(1).getText()
            int         bytes   = query.getColumn(1).getBytes();
            double      weight  = query.getColumn(2); // = query.getColumn(2).getInt()
			
            static bool bFirst = true;
            if (bFirst)
            {
                // Show how to get the aliased names of the result columns.
                std::string name0 = query.getColumn(0).getName();
                std::string name1 = query.getColumn(1).getName();
                std::string name2 = query.getColumn(2).getName();
				
                ofLogNotice("SQLiteManager::search") << "aliased result [\"" << name0 << "\", \"" << name1 << "\", \"" << name2 << "\"]";
				
                bFirst = false;
            }
			
            ofLogNotice() << "row (" << id << ", \"" << value2 << "\" "  << bytes << " bytes, " << weight << ")";
        }
	}
    catch (std::exception& e)
    {
        ofLogError("SQLiteManager::search") << e.what();
    }


}
