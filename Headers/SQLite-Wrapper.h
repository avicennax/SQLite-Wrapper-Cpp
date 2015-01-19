#ifndef SQLITE3_WRAPPER
#define SQLITE3_WRAPPER

#include <vector>
#include <string>
#include <mutex>
#include "sqlite3.h"

using namespace std;

namespace sqlite3Wrapper
{
	class Database;
	typedef int ERROR_CODE;
	static int callbackHandler(void*, int, char**, char**);	
	
	string texQuo(string);
	string expBuild(string, string, string);

	class Database
	{
		public:
			friend int callbackHandler(void*, int, char**, char**);
			
			Database(int verbose=1, string databaseName=string()); // Default reporting is '1', errors only.
			Database(const Database&);
			~Database();
			Database& operator=(const Database&);
			
			ERROR_CODE setReportStatus(int verbose);
			
			ERROR_CODE loadDB(string databaseName);
			
			ERROR_CODE listTables();
			ERROR_CODE createTable(string tableName, vector<string> columns, string key=string());
			ERROR_CODE dropTable(string tableName);
			
			ERROR_CODE queryDB(string query);
			ERROR_CODE selectDB(vector<string> columns, vector<string> tables, vector<string> predicates=vector<string>(), vector<string> predBools=vector<string>());
			ERROR_CODE insertDB(string tableName, vector<string> values, vector<string> paramOrder=vector<string>());
			ERROR_CODE updateDB(string tableName, vector<string> setClauses, vector<string> predicates, vector<string> predBools=vector<string>());
			ERROR_CODE deleteDB(string tableName, vector<string> predicates, vector<string> predBools=vector<string>());
			
			vector < vector <string> > queryResults;
			string errorMs; // Error message returned by sqlite3 function describing nature/location of error.
			
		private:			
			sqlite3 *db;
			string dbPrompt;
			string dbName;
			bool dbLoaded;
			char *errorMsg;
			int verbose; // 0=Silent | 1=Errors | 2=Verbose
			int queryStorer(int, char**, char**);
			int queryResReset();
			int recordNum;
			int qrSize;
				
	};

}

#endif
