#pragma once

#include "table.h"
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class TableManager
{
private:
    string currentTableName;
    Table *currentTable;
    unordered_map<string, Table *> tables;

public:
    TableManager();
    ~TableManager();

    void createTable(const string &tableName, const vector<string> &columns);
    bool useTable(const string &tableName);
    Table *getCurrentTable();
    string getCurrentTableName() const;
    vector<string> listTables() const;
};
