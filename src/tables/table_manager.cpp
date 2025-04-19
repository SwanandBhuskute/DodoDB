#include "table_manager.h"
#include <iostream>
#include <filesystem>
#include <fstream>
// #include<bits/stdc++.h>

using namespace std;

namespace fs = std::filesystem;

TableManager::TableManager()
{
    string lastUsed = loadLastUsedTable();
    string fallback = "users";

    string tableToUse = lastUsed.empty() ? fallback : lastUsed;
    string filename = "data/" + tableToUse + ".table.txt";

    if (fs::exists(filename))
    {
        currentTableName = tableToUse;
        currentTable = new Table(tableToUse);
        currentTable->loadFromFile(filename);
        tables[tableToUse] = currentTable;
        cout << "Loaded last used table: " << tableToUse << endl;
    }
    else
    {
        cout << "No table found to load. Start by creating a new one.\n";
        currentTable = nullptr;
        currentTableName = "";
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

    saveLastUsedTable(tableName);

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

    if (currentTable)
        delete currentTable;

    currentTable = new Table(tableName);
    currentTable->loadFromFile(filename);
    currentTableName = tableName;
    tables[tableName] = currentTable;

    saveLastUsedTable(tableName);

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

void TableManager::saveLastUsedTable(const string &tableName)
{
    ofstream out("utils/meta/last_used_table.txt");
    if (out.is_open())
    {
        out << tableName;
        out.close();
    }
}

string TableManager::loadLastUsedTable()
{
    ifstream inFile("utils/meta/last_used_table.txt");
    if (!inFile.is_open())
    {
        return "";
    }

    string tableName;
    getline(inFile, tableName);
    inFile.close();

    tableName.erase(tableName.find_last_not_of(" \n\r\t") + 1);
    return tableName;
}

void TableManager::setCurrentTable(Table *table, const string &tableName)
{
    if (currentTable)
        delete currentTable;

    currentTable = table;
    currentTableName = tableName;
    tables[tableName] = table;

    saveLastUsedTable(tableName);
}
