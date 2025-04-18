#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "../utils/string_utils.h"
#include "./tables/table.h"
#include "./tables/table_manager.h"
using namespace std;

TableManager manager;

void handleInput(string &input)
{
    input = toLower(input);

    if (input.substr(0, 6) == "insert")
    {
        Table *table = manager.getCurrentTable();
        if (!table)
        {
            cout << "No table selected. Use: use table <name>\n";
            return;
        }

        istringstream iss(input);
        string command, val;
        vector<string> row;
        iss >> command;
        while (iss >> val)
            row.push_back(val);

        table->insertRow(row);
        cout << "Row inserted." << endl;
    }
    else if (input.substr(0, 6) == "select")
    {
        Table *table = manager.getCurrentTable();
        if (!table)
        {
            cout << "No table selected. Use: use table <name>\n";
            return;
        }

        table->printAllRows();
    }
    else if (input.substr(0, 6) == "delete")
    {
        Table *table = manager.getCurrentTable();
        if (!table)
        {
            cout << "No table selected. Use: use table <name>\n";
            return;
        }

        size_t wherePos = input.find("where");
        if (wherePos == string::npos)
        {
            cout << "Invalid delete syntax. Use: delete where column=value" << endl;
            return;
        }

        string condition = input.substr(wherePos + 6);
        size_t eqPos = condition.find('=');
        if (eqPos == string::npos)
        {
            cout << "Invalid condition. Use: delete where column=value" << endl;
            return;
        }

        string col = trim(condition.substr(0, eqPos));
        string val = trim(condition.substr(eqPos + 1));

        table->deleteRowsWhere(col, val);
    }
    else if (input.substr(0, 6) == "update")
    {
        Table *table = manager.getCurrentTable();
        if (!table)
        {
            cout << "No table selected. Use: use table <name>\n";
            return;
        }

        size_t setPos = input.find("set");
        size_t wherePos = input.find("where");

        if (setPos == string::npos || wherePos == string::npos || wherePos < setPos)
        {
            cout << "Invalid update syntax. Use: update set column=value where column=value" << endl;
            return;
        }

        string setPart = input.substr(setPos + 4, wherePos - (setPos + 4));
        string wherePart = input.substr(wherePos + 6);

        size_t eqSet = setPart.find('=');
        size_t eqWhere = wherePart.find('=');

        if (eqSet == string::npos || eqWhere == string::npos)
        {
            cout << "Invalid update format. Use: update set column=value where column=value" << endl;
            return;
        }

        string setCol = trim(setPart.substr(0, eqSet));
        string setVal = trim(setPart.substr(eqSet + 1));
        string condCol = trim(wherePart.substr(0, eqWhere));
        string condVal = trim(wherePart.substr(eqWhere + 1));

        table->updateRowsWhere(condCol, condVal, setCol, setVal);
    }
    else if (input.substr(0, 12) == "create table")
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
            // string colType = trim(colDef.substr(spacePos + 1)); // type parsing in future
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

    else if (input.rfind("use table ", 0) == 0)
    {                                              // starts with "use table "
        string tableName = trim(input.substr(10)); // 10 = length of "use table "
        if (tableName.empty())
        {
            cout << "Please specify a table name. Usage: use table <table_name>\n";
            return;
        }

        manager.useTable(tableName);
    }
    else if (input.substr(0, 10) == "curr table")
    {
        Table *table = manager.getCurrentTable();
        if (!table)
        {
            cout << "No table selected. Use: use table <name>\n";
            return;
        }

        cout << "Curr table: " << manager.getCurrentTableName() << endl; // ✅ Neat!
    }

    else
    {
        cout << "Unrecognized command: " << input << endl;
    }
}

int main()
{
    string input;

    cout << endl
         << "$ ./dodoDb" << endl;
    while (true)
    {
        cout << "dodoDb > ";
        getline(cin, input);

        if (toLower(input) == "exit")
        {
            cout << "Bye says dodo!" << endl;
            break;
        }

        handleInput(input);
    }

    return 0;
}
