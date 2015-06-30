#include <iostream>
#include <stdio.h>
#include <vector>
#include <chrono>
#include <random>
#include "../Headers/SQLite-Wrapper.h"
using namespace std;
namespace sqlwrap = sqlite3Wrapper;

class DBtester {
	public:
		DBtester();
		void mainLoop();
	private:
		void queryD();
		void addTable();
		void dropTable();
		void selectD();
		void insertD();
		void updateD();
		void deleteD();
		void prompt();
		void cinFlush();
		void listTables();
		void generateEntries();
		int menu;
		vector <string> cols;
		vector <string> utility;
		vector <string> preds;
		vector <string> bools;
		string multi;
		string multi2;	
		sqlwrap::Database test;	
};

DBtester::DBtester()
{
	test.setReportStatus(2);
	test.loadDB("test.db");
	int menu = 0;
	cout << endl << "DB size: " << sizeof(test) << endl;
}

void DBtester::mainLoop()
{
	prompt();
	while (menu != 10)
	{
		cout << endl << "> ";
		cin >> menu;
		cinFlush();
		switch (menu)
		{
			case 1:
				queryD();
				break;
			case 2:
				addTable();
				break;
			case 3:
				dropTable();
				break;
			case 4:
				selectD();
				break;
			case 5:
				insertD();
				break;
			case 6:
				updateD();
				break;
			case 7:
				deleteD();
				break;
			case 8:
				listTables();
				break;
			case 9:
				generateEntries();
				break;
			case 10:
				prompt();
				break;
			case 11:
				return;
				break;
			default:
				cout << "Invalid entry: try again" << endl;
				break;
			
		}
		
	}
	return;
}

void DBtester::queryD()
{
	cout << "Enter query > ";
	getline(cin, multi);			
	test.queryDB(multi);
	cinFlush();
	cout << endl << "DB size: " << sizeof(test) << endl;

}

void DBtester::addTable()
{
	cout << "Enter table name > ";
	getline(cin, multi);
	cinFlush();
	while (multi2 != "EOF")
	{
		cout << "Enter 'column_name column_type'(EOF to terminate) > ";
		getline(cin, multi2);
		cinFlush();
		if (multi2 != "EOF")
			cols.push_back(multi2);
	}
	cout << "Enter 'key_name key_type' (EOF to exclude key) > ";
	getline(cin, multi2);
	cinFlush();
	if (multi2 != "EOF")
		test.createTable(multi, cols, multi2);
	else
		test.createTable(multi, cols);
	cols.clear();	
}

void DBtester::dropTable()
{
	cout << "Enter table name to be dropped > ";
	getline(cin, multi);
	test.dropTable(multi);
	cinFlush();
}

void DBtester::selectD()
{
	do
	{
		cout << "Enter 'column_name' (EOF to terminate) > ";
		getline(cin, multi);
		cinFlush();
		if (multi != "EOF")
			cols.push_back(multi);					
	}while (multi != "EOF");
	do
	{
		cout << "Enter 'table_name' (EOF to terminate) > ";
		getline(cin, multi);
		cinFlush();
		if (multi != "EOF")
			utility.push_back(multi);					
	}while (multi != "EOF");
	do
	{
		cout << "Enter 'column_name = value_name' (EOF to terminate) > ";
		getline(cin, multi);
		cinFlush();
		if (multi != "EOF")
			preds.push_back(multi);					
	}while (multi != "EOF");
	do
	{
		cout << "Enter 'bool_value' (EOF to terminate) > ";
		getline(cin, multi);
		cinFlush();
		if (multi != "EOF")
			bools.push_back(multi);					
	}while (multi != "EOF");
	if (preds.empty() && bools.empty())
		test.selectDB(cols, utility);
		else if (bools.empty())
			test.selectDB(cols, utility, preds);
			else
				test.selectDB(cols, utility, preds, bools);
	cols.clear();
	utility.clear();
	preds.clear();
	bools.clear();
	cout << endl << "DB size: " << sizeof(test) << endl;	
}

void DBtester::insertD()
{
	cout << "Enter table name > ";
	getline(cin, multi);
	cinFlush();
	do
	{
		cout << "Enter 'value_name'(EOF to terminate) > ";
		getline(cin, multi2);
		cinFlush();
		if (multi2 != "EOF")
			utility.push_back(multi2);
	} while (multi2 != "EOF");
	do
	{
		cout << "Enter 'column_name'(EOF to terminate) > ";
		getline(cin, multi2);
		cinFlush();
		if (multi2 != "EOF")
			cols.push_back(multi2);
	}while (multi2 != "EOF");
	test.insertDB(multi, utility, cols);
	cols.clear();
	utility.clear();
}

void DBtester::updateD()
{
	cout << "Enter table name > ";
	getline(cin, multi);
	cinFlush();	
	do
	{
		cout << "Enter 'column_name = new_value_name'(EOF to terminate) > ";
		getline(cin, multi2);
		cinFlush();
		if (multi2 != "EOF")
			utility.push_back(multi2);
	} while (multi2 != "EOF");
	do
	{
		cout << "Enter 'column_name = value_name' (EOF to terminate) > ";
		getline(cin, multi2);
		cinFlush();
		if (multi2 != "EOF")
			preds.push_back(multi2);					
	}while (multi2 != "EOF");
	do
	{
		cout << "Enter 'bool_value' (EOF to terminate) > ";
		getline(cin, multi2);
		cinFlush();
		if (multi2 != "EOF")
			bools.push_back(multi2);					
	}while (multi2 != "EOF");
		if (bools.empty())
			test.updateDB(multi, utility, preds);
		else
			test.updateDB(multi, utility, preds, bools);
	utility.clear();
	preds.clear();
	bools.clear();
}

void DBtester::deleteD()
{
	cout << "Enter table name > ";
	getline(cin, multi);
	cinFlush();	
	do
	{
		cout << "Enter 'column_name = value_name' (EOF to terminate) > ";
		getline(cin, multi2);
		cinFlush();
		if (multi2 != "EOF")
			preds.push_back(multi2);					
	}while (multi2 != "EOF");
	do
	{
		cout << "Enter 'bool_value' (EOF to terminate) > ";
		getline(cin, multi2);
		cinFlush();
		if (multi2 != "EOF")
			bools.push_back(multi2);					
	}while (multi2 != "EOF");
	if (bools.empty())
		test.deleteDB(multi, preds);
	else
		test.deleteDB(multi, preds, bools);
	preds.clear();
	bools.clear();
}

void DBtester::generateEntries()
{
	cout << "Enter number of entries to generated > ";
	int genCount;
	cin >> genCount;
	if (genCount < 0)
	{
		cout << "Entry count less than 0 ...\n" << endl;
		return;
	}

	multi = "TEST";
	cols.push_back("name TEXT");
	cols.push_back("age REAL");
	multi2 = "ID REAL";
	test.createTable(multi, cols, multi2);
	cols.clear();

	static string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	srand(time(NULL));
	uniform_real_distribution <float> entAge(10, 50);
	uniform_real_distribution <float> nameLen(4, 10);

	for (int j = 0; j < genCount; j++)
	{
		string name;
		name.resize(int(nameLen(generator)));
		for (size_t i = 0; i < name.size(); i++)
			name[i] = charset[rand() % charset.length()];
		int age = int(entAge(generator));
		utility.push_back(to_string(j));
		utility.push_back(to_string(age));
		utility.push_back("'"+name+"'");
		cols.push_back("ID");
		cols.push_back("age");
		cols.push_back("name");
		test.insertDB(multi, utility, cols);
		cols.clear();
		utility.clear();
	}
	return;
}

void DBtester::listTables()
{
	test.listTables();
	return;
}

void DBtester::cinFlush()
{
	cin.clear();
	cin.sync();
}

void DBtester::prompt()
{
	cout << endl << "Options --" << endl;
	cout << "1) Query" << endl;
	cout << "2) Add Table" << endl;
	cout << "3) Drop Tables" << endl;
	cout << "4) SELECT" << endl;
	cout << "5) INSERT" << endl;
	cout << "6) UPDATE" << endl;
	cout << "7) DELETE" << endl;
	cout << "8) List Tables" << endl;
	cout << "9) Generate Entries" << endl;
	cout << "10) Prompt " << endl;
	cout << "11) (exit)" << endl;
}

int main(int argc, char** argv) {
	
	DBtester tester;
	tester.mainLoop();
	return 0;
}
