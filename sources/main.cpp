#include <iostream>
#include <stdio.h>
#include <vector>
#include "SQLite-Wrapper.h"
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
				prompt();
				break;
			case 10:
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
}

void DBtester::addTable()
{
	cout << "Enter table name > ";
	getline(cin, multi2);
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
	cout << "9) Prompt" << endl;
	cout << "10) (exit)" << endl;
}

int main(int argc, char** argv) {
	
	DBtester tester;
	tester.mainLoop();
	return 0;
}
