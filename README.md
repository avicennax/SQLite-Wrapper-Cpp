## SQLite-Wrapper
A simple C++ Wrapper for sqlite3 API
### Introduction
The 'Database' class provides a C++ wrapper object for SQL databases, that allows developers
to load, access and query a database via 'Database' member functions.
The 'Database' class provides two major avenues for SQL database queries: queryDB() and
four specialized query member functions, selectDB(), insertDB(), updateDB() and deleteDB().
The queryDB() method takes a single std::string parameter, which is a syntactically and
semantically complete SQL query, and runs it against the target database.
The four specialized query methods take set(s) of std::vector<string> and std::string
parameters containing values relevant to the query. These methods are best used when query
values are derived from	external sources, thus removing the need to construct ones own
syntactically complete query statements. 

SELECT queries executed from either queryDB() or selectDB() will store the returned table
in a std::vector<vector<string>> (multidimensional string vector), queryResults, and print
the table via std::cout if specified.

Furthermore, two functions texQuo() and expBuild() are provided to assist the 
developer in building std::string parameters ready for passing to 'Database' methods.
