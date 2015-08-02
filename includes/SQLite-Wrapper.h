#ifndef SQLITE3_WRAPPER_H
#define SQLITE3_WRAPPER_H

/* ***
===========================================================================
SQLite-Wrapper: A simple C++ Wrapper for sqlite3 API
===========================================================================

-- Introduction --

The 'Database' class provides a C++ wrapper object for SQL databases,
that allows developers to load, access and query a database via 'Database'
member functions. The 'Database' class provides two major avenues for SQL
database queries: queryDB() and	four specialized query member functions,
selectDB(), insertDB(), updateDB() and deleteDB(). The queryDB() method
takes a single std::string parameter, which is a syntactically and semantically
complete SQL query, and runs it against the target database. The four
specialized query methods take set(s) of std::vector<string> and std::string
parameters containing values relevant to the query. These methods are best used
when query values are derived from external sources, thus removing the need to
construct ones own syntactically complete query statements.

SELECT queries executed from either queryDB() or selectDB() will store the
returned table in a std::vector<vector<string>> (multidimensional string vector),
queryResults, and print	the table via std::cout if specified.

Furthermore, two functions texQuo() and expBuild() are provided to assist
the developer in building std::string parameters ready for passing to
'Database' methods.

*** */

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

			ERROR_CODE selectDB(vector<string> columns, vector<string> tables, 
				vector<string> predicates=vector<string>(), vector<string> predBools=vector<string>());

			ERROR_CODE insertDB(string tableName, vector<string> values, vector<string> paramOrder=vector<string>());

			ERROR_CODE updateDB(string tableName, vector<string> setClauses, vector<string> predicates,
				vector<string> predBools=vector<string>());

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
