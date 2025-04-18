#include "table_manager.h"
#include <iostream>
#include <filesystem>
#include <fstream>
// #include<bits/stdc++.h>

using namespace std;

namespace fs = std::filesystem;

TableManager::TableManager()
{
    string defaultTable = "users";
    string filename = "data/" + defaultTable + ".table.txt";

    if (fs::exists(filename))
    {
        currentTableName = defaultTable;
        currentTable = new Table({});
        currentTable->loadFromFile(filename);
    }
}

TableManager::~TableManager()
{
    if (currentTable)
    {
        delete currentTable;
    }
}

void TableManager::createTable(const string &tableName, const vector<string> &columns)
{
    string filename = "data/" + tableName + ".table.txt";

    if (fs::exists(filename))
    {
        cout << "Table '" << tableName << "' already exists.\n";
        return;
    }

    ofstream outFile(filename);
    if (!outFile.is_open())
    {
        cout << "Error: Could not create table file.\n";
        return;
    }

    // Save schema to file
    for (size_t i = 0; i < columns.size(); ++i)
    {
        outFile << columns[i];
        if (i != columns.size() - 1)
            outFile << ",";
    }
    outFile << "\n";
    outFile.close();

    // ✅ Create and store Table object in memory
    Table *newTable = new Table(tableName, columns); // ✅ Fixed constructor
    tables[tableName] = newTable;
    currentTableName = tableName;
    currentTable = newTable;

    cout << "Table '" << tableName << "' created and loaded.\n";
}

bool TableManager::useTable(const string &tableName)
{
    string filename = "data/" + tableName + ".table.txt";
    if (!fs::exists(filename))
    {
        cout << "Table '" << tableName << "' does not exist.\n";
        return false;
    }

    // Clean up old currentTable
    if (currentTable)
        delete currentTable;

    // Create and load new one
    currentTable = new Table(tableName); // ✅ Pass table name
    currentTable->loadFromFile(filename);
    currentTableName = tableName;

    // Save in table map
    tables[tableName] = currentTable;

    cout << "Switched to table: " << tableName << endl;
    return true;
}

Table *TableManager::getCurrentTable()
{
    return currentTable;
}

string TableManager::getCurrentTableName() const
{
    return currentTableName;
}

vector<string> TableManager::listTables() const
{
    vector<string> tableNames;

    for (const auto &entry : fs::directory_iterator("data"))
    {
        string filename = entry.path().filename().string();
        if (filename.find(".table.txt") != string::npos)
        {
            string name = filename.substr(0, filename.find(".table.txt"));
            tableNames.push_back(name);
        }
    }

    return tableNames;
}
