#include <iostream>
#include "../tables/table.h"
#include "../tables/table_manager.h"
#include "../../utils/string_utils.h"
#include <string>
#include <sstream>
using namespace std;

extern TableManager manager;

void createTableCommand(string &input)
{
    size_t nameStart = 13;
    size_t parenStart = input.find('(', nameStart);
    size_t parenEnd = input.find(')', parenStart);

    if (parenStart == string::npos || parenEnd == string::npos)
    {
        cout << "Invalid create table syntax. Missing parentheses.\n";
        return;
    }

    string tableName = trim(input.substr(nameStart, parenStart - nameStart));
    if (tableName.empty())
    {
        cout << "Invalid table name.\n";
        return;
    }

    string columnsStr = input.substr(parenStart + 1, parenEnd - parenStart - 1);
    istringstream iss(columnsStr);
    string colDef;
    vector<string> columns;

    while (getline(iss, colDef, ','))
    {
        colDef = trim(colDef);
        size_t spacePos = colDef.find(' ');
        if (spacePos == string::npos)
        {
            cout << "Invalid column definition: " << colDef << ". Use: name type\n";
            return;
        }
        string colName = trim(colDef.substr(0, spacePos));

        if (colName.empty())
        {
            cout << "Empty column name found.\n";
            return;
        }
        columns.push_back(colName);
    }

    if (columns.empty())
    {
        cout << "No columns defined.\n";
        return;
    }

    manager.createTable(tableName, columns);
}
