#include <iostream>
#include <iomanip>
#include <vector>
#include "SQLite-Wrapper.h"
#include "sqlite3.h"

using namespace std;
using namespace sqlite3Wrapper;

Database::Database(int verb, string dbNam)
{
	dbLoaded = false;
	errorMsg = 0;
	recordNum = 0;
	qrSize = 0;
	errorMs = "NULL";
	dbPrompt = "Sqlite3Wrapper::Database  ";
	if (verb >= 0 && verb <= 2)
		verbose = verb;
	else
	{
		cout << endl << dbPrompt << "invalid verbose value (must be [0-2])";
		cout << "	>> verbose set to default value: 1" << endl;
		verbose = 1;
	}
	if (dbNam != "")
		loadDB(dbNam);
}

Database::~Database()
{
	sqlite3_close(db);
}

int Database::setReportStatus(int verb)
{
	if (verb >= 0 && verb <= 2)
		verbose = verb;
	else
	{
		if (verbose >= 1)
		{
		cout << endl << dbPrompt << "invalid verbose value (must be [0-2])" << endl;
		cout << "	>> verbose value remains: " << verbose << endl;			
		}
		return 5;
	}
	return 0;
}

int Database::loadDB(string dbName_string)
{
	const char *dbName_const = dbName_string.c_str();
	int err = sqlite3_open(dbName_const, &db);
	if(err && verbose > 0)
	{
		cerr << endl << dbPrompt << sqlite3_errmsg(db) << endl;
		return 3;
	}
	else
	{
		dbLoaded = true;
		dbName = dbName_string;
		dbPrompt.erase(dbPrompt.end()-2, dbPrompt.end());
		dbPrompt+=" "+dbName_string+": ";
		if (verbose == 2)
			cout << endl << dbPrompt << "successfully loaded/created" << endl;
		return 0;
	}
}

int Database::listTables()
{
	if (dbLoaded == true)
	{
		const char* tableListQuery = "SELECT name FROM sqlite_master WHERE type='table';";
		if (verbose == 2)
		{
			cout << endl << "    Tables" << endl;
			cout << "---------------" << endl;
		}
		queryResReset();
		int err = sqlite3_exec(db, tableListQuery, callbackHandler, this, &errorMsg);
		if (err != SQLITE_OK)
		{
			if(err && verbose > 0)
				cerr << endl << dbPrompt << errorMsg << endl;
			errorMs = errorMsg;
			sqlite3_free(errorMsg);
			return 1;  
		}
		else
		{
			if (verbose == 2)
				cout << endl << dbPrompt << "list of database tables stored in 'queryResults'" << endl;
			return 0;
		}
	}
	else
	{
		if (verbose > 0)
			cout << endl << dbPrompt << "no database loaded" << endl;
		return 3;
	}
}

int Database::createTable(string tableName, vector <string> columns, string key)
{
	if (dbLoaded == true)
	{
		string createBase = "CREATE TABLE "+tableName+" (\n";
		if (!(key.empty()))
			createBase+="	"+key+" PRIMARY KEY,\n";
		for (int i=0; i < columns.size(); i++)
			createBase+="	"+columns[i]+",\n";
		createBase.erase(createBase.end()-2, createBase.end());
		createBase+="\n);";
		const char *createQuery = createBase.c_str();
		int err = sqlite3_exec(db, createQuery, callbackHandler, this, &errorMsg);
		if (err != SQLITE_OK)
		{	
			if(err && verbose > 0)
				cerr << endl << dbPrompt << errorMsg << endl;
			errorMs = errorMsg;
			sqlite3_free(errorMsg);
			return 1;  
		}
		else
		{
			if (verbose == 2)
				cout << endl << dbPrompt << "CREATE operation completed successfully" << endl;
			return 0;
		}
	}
	else
	{
		if (verbose > 0)
			cout << endl << dbPrompt << "no database loaded" << endl;
		return 3;
	}
	
}

int Database::dropTable(string table)
{
	if(dbLoaded == true)
	{
		string dropBase = "DROP TABLE "+table+";";
		const char *dropQuery = dropBase.c_str();
		int err = sqlite3_exec(db, dropQuery, callbackHandler, this, &errorMsg);
		if (err != SQLITE_OK)
		{	
			if(err && verbose > 0)
				cerr << endl << dbPrompt << errorMsg << endl;
			errorMs = errorMsg;
			sqlite3_free(errorMsg);
			return 1;  
		}
		else
		{
			if (verbose == 2)
				cout << endl << dbPrompt << "DROP operation completed successfully" << endl;
			return 0;
		}
	}
	else
	{
		if (verbose > 0)
			cout << endl << dbPrompt << "no database loaded" << endl;
		return 3;
	}
}

int Database::queryDB(string queryString)
{
	if (dbLoaded  == true)
	{
		if (queryString.empty())
		{
			if (verbose > 0)
				cout << endl << dbPrompt << "query string parameter empty" << endl;
			return 2;
		}
		const char *queryParam = queryString.c_str();
		cout << endl;
		queryResReset();
		int err = sqlite3_exec(db, queryParam, callbackHandler, this, &errorMsg);
		if (err != SQLITE_OK)
		{
			if(err && verbose > 0)
				cerr << endl << dbPrompt << errorMsg << endl;
			errorMs = errorMsg;
			sqlite3_free(errorMsg); 
			return 1; 
		}
		else
		{
			if (verbose == 2)
			{
				cout << endl << dbPrompt << "operation succesfully completed" << endl;
				cout << "	>> Operation query results stored in 'queryResults'" << endl;
			}
			return 0;
		}
	}
	else
	{
		if (verbose > 0)
			cout << endl << dbPrompt << "no database loaded" << endl;
		return 3;
	}
}

int Database::selectDB(vector<string> columns, vector<string> tables, vector<string> predicates, vector<string> predBools)
{
	if (dbLoaded == true)
	{
		string selectBase = "SELECT";
		for (int i=0; i < columns.size(); i++)
			selectBase+=" "+columns[i]+",";
		selectBase.erase(selectBase.end()-1, selectBase.end());
		selectBase+=" FROM";
		for (int i=0; i < tables.size(); i++)
			selectBase+=" "+tables[i]+",";
		selectBase.erase(selectBase.end()-1, selectBase.end());
		if (predicates.empty())
				selectBase+=";";
		else if (predicates.size()-1 == predBools.size())
		{
			selectBase+=" WHERE";
			if (predBools.empty())
				selectBase+=" "+predicates[0]+";";
			else
			{
				for (int i=0; i < predBools.size(); i++)
					selectBase+=" "+predicates[i]+" "+predBools[i];
				selectBase+=" "+predicates.back()+";";
			}
		}
		else
		{
			if (verbose > 0)
				{
					cout << endl << dbPrompt << "vector parameter sizes incompatible" << endl;
					cout << "	>> predicate vector should be 1 cell larger than bools vector" << endl;
				}
			return 4;
		}
		queryResReset();
		//cout << selectBase << endl;
		const char *selectQuery = selectBase.c_str();
		int err = sqlite3_exec(db, selectQuery, callbackHandler, this, &errorMsg);
		if (err != SQLITE_OK)
		{	
			if (err && verbose > 0)
				cerr << endl << dbPrompt << errorMsg << endl;
			errorMs = errorMsg;
			sqlite3_free(errorMsg);
			return 1;  
		}
		else
		{
			if (verbose == 2)
				cout << endl << dbPrompt << "SELECT operation completed successfully" << endl;
			return 0;
		}
	}
	else
	{
		if (verbose > 0)
			cout << endl << dbPrompt << "no database loaded" << endl;
		return 3;
	}
}

int Database::insertDB(string table, vector <string> values, vector <string> paramsOrder)
{
	if (dbLoaded == true)
	{
		string insertBase = "INSERT INTO "+table;
		if (!(paramsOrder.empty()))
		{
			if (values.size() == paramsOrder.size())
			{	
				insertBase+=" (";
				for (int i=0; i<paramsOrder.size(); i++)
					insertBase+=paramsOrder[i]+", ";
				insertBase.erase(insertBase.end()-2, insertBase.end());
				insertBase+=")";
			}
			else
			{
				if (verbose > 0)
				{
					cout << endl << dbPrompt << "vector parameter sizes incompatible" << endl;
					cout << "	>> vector parameters' sizes should be '=='" << endl;
				}
				return 4;
			}
		}
		insertBase+=" VALUES (";
		for (int i=0; i<values.size(); i++)
			insertBase+=values[i]+", ";
		insertBase.erase(insertBase.end()-2, insertBase.end());
		insertBase+=");";
		const char *insertQuery = insertBase.c_str();
		int err = sqlite3_exec(db, insertQuery, callbackHandler, this, &errorMsg);
		if (err != SQLITE_OK)
		{	
			if (err && verbose > 0)
				cerr << endl << dbPrompt << errorMsg << endl;
			errorMs = errorMsg;
			sqlite3_free(errorMsg);
			return 1;  
		}
		else
		{
			if (verbose == 2)
				cout << endl << dbPrompt << "INSERT operation completed successfully" << endl;
			return 0;
		}
	}
	else
	{
		if (verbose > 0)
			cout << endl << dbPrompt << "no database loaded" << endl;
		return 3;
	}
}

int Database::updateDB(string table, vector<string> setClauses, vector<string> predicates, vector<string> predicBools)
{
	if (dbLoaded == true)
	{
		string updateBase = "UPDATE "+table+" SET";
		for (int i=0; i < setClauses.size(); i++)
			updateBase+=" "+setClauses[i]+",";
		updateBase.erase(updateBase.end()-1, updateBase.end());
		if( predicates.size()-1 == predicBools.size() )
		{
			updateBase+=" WHERE";
			if ( predicBools.empty())
				updateBase+=" "+predicates[0]+";";
			else
			{
				for (int i=0; i < predicBools.size(); i++)
					updateBase+=" "+predicates[i]+" "+predicBools[i];
				updateBase+=" "+predicates.back()+";";
			}
		}
		else
		{
			if (verbose > 0)
			{
				cout << endl << dbPrompt << "vector parameter sizes incompatible" << endl;
				cout << "	>> predicate vector should be 1 cell larger than bools vector" << endl;
			}
			return 4;
		}
		const char *updateQuery = updateBase.c_str();
		int err = sqlite3_exec(db, updateQuery, callbackHandler, this, &errorMsg);
		if (err != SQLITE_OK)
		{	
			if (err && verbose > 0)
				cerr << endl << dbPrompt << errorMsg << endl;
			errorMs = errorMsg;
			sqlite3_free(errorMsg);
			return 1;  
		}
		else
		{
			if (verbose == 2)
				cout << endl << dbPrompt << "UPDATE operation completed successfully" << endl;
			return 0;
		}
	}
	else
	{
		if (verbose > 0)
			cout << endl << dbPrompt << "no database loaded" << endl;
		return 3;
	}

}

int Database::deleteDB(string table, vector<string> predicates, vector<string> predBools)
{
	if (dbLoaded == true)
	{
		string deleteBase = "DELETE FROM "+table+" WHERE";
		if (predicates.size()-1 == predBools.size())
		{
			if (predBools.empty())
				deleteBase+=" "+predicates[0]+";";
			else
			{
				for (int i=0; i < predBools.size(); i++)
					deleteBase+=" "+predicates[i]+" "+predBools[i];
				deleteBase+=" "+predicates.back()+";";	
			}
		}
		else
		{
			if (verbose > 0)
				{
					cout << endl << dbPrompt << "vector parameter sizes incompatible" << endl;
					cout << "	>> predicate vector should be 1 cell larger than bools vector" << endl;
				}
				return 4;
		}
		const char *deleteQuery = deleteBase.c_str();
		int err = sqlite3_exec(db, deleteQuery, callbackHandler, this, &errorMsg);
		if (err != SQLITE_OK)
		{	
			if(err && verbose > 0)
				cerr << endl << dbPrompt << errorMsg << endl;
			errorMs = errorMsg;
			sqlite3_free(errorMsg);
			return 1;  
		}
		else
		{
			if (verbose == 2)
				cout << endl << dbPrompt << "DELETE operation completed successfully" << endl;
			return 0;
		}
	}
	else
	{
		if (verbose > 0)
			cout << endl << dbPrompt << "no database loaded" << endl;
		return 3;	
	}
}

int Database::queryResReset()
{
	recordNum = 0;
	qrSize = 0;
	queryResults.clear();
}

int Database::queryStorer(int argc, char **values, char **field)
{
	qrSize++;
	queryResults.resize(qrSize);
	queryResults[recordNum].resize(argc);
	
	if (recordNum == 0)
	{
		for(int i=0; i < argc; i++)
		{
			queryResults[0][i] = field[i];
			if (verbose == 2)
				cout << left << setw(15) << field[i];
		}
		if (verbose == 2)
		{
			cout << endl;
			for(int i=0; i < argc; i++)
				cout << left << setw(15) << setfill('-') << "-";
			cout << setfill(' ')<< endl;
		}
		recordNum++;
		qrSize++;
		queryResults.resize(qrSize);
		queryResults[recordNum].resize(argc);
	}
	for(int i=0; i < argc; i++)
	{
		if (values[i])
		{
			if (verbose == 2)
				cout << left << setw(15) << values[i];
			queryResults[recordNum][i] = values[i];
		}
		else
		{
			if (verbose == 2)
				cout << left << setw(15) << "";
			queryResults[recordNum][i] = "";
		}
	}
	recordNum++;
	if (verbose == 2)
   		cout << endl;
    return 0;
  }  


static int sqlite3Wrapper::callbackHandler(void *databaseObj, int argc, char **values, char **field)
{
	Database *transDB = reinterpret_cast<Database*>(databaseObj);
	return transDB->queryStorer(argc, values, field);
}

string sqlite3Wrapper::texQuo(string target)
{
	string newString="'"+target+"'";
	return newString;
}

string sqlite3Wrapper::expBuild(string column, string value, string expBool)
{
	string newString = column+" "+expBool+" "+value;
	return newString;
}

