#ifndef SQLITE3_WRAPPER
#define SQLITE3_WRAPPER
#include <vector>
#include "sqlite3.h"

using namespace std;

namespace sqlite3Wrapper
{
	class Database;
	static int callbackHandler(void*, int, char**, char**);	
	string texQuo(string);
	string expBuild(string, string, string);
	
	class Database
	{
		public:
			friend int callbackHandler(void*, int, char**, char**);
			
			Database(int verbose=1, string databaseName=string()); // Default reporting is '1', errors only.
			~Database();
			int setReportStatus(int verbose);
			int loadDB(string databaseName);
			int listTables();
			int createTable(string tableName, vector<string> columns, string key=string());
			int dropTable(string tableName);
			int queryDB(string query);
			int selectDB(vector<string> columns, vector<string> tables, vector<string> predicates=vector<string>(), vector<string> predBools=vector<string>());
			int insertDB(string tableName, vector<string> values, vector<string> paramOrder=vector<string>());
			int updateDB(string tableName, vector<string> setClauses, vector<string> predicates, vector<string> predBools=vector<string>());
			int deleteDB(string tableName, vector<string> predicates, vector<string> predBools=vector<string>());
			
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
