#ifndef TABLE_H
#define TABLE_H

#include <vector>
#include <string>
using namespace std;

class Table
{
private:
    string name;
    vector<string> columnNames;
    vector<vector<string>> rows;
    string getTableData();

public:
    // Constructor for creating a table with name and schema
    Table(const string &tableName, const vector<string> &columns);
    Table(const string &tableName);

    // Basic Getters
    string getName() const
    {
        return name;
    }

    vector<string> getColumnNames() const
    {
        return columnNames;
    }

    // Core Operations
    void insertRow(const vector<string> &row);
    void deleteRowsWhere(const string &columnName, const string &value);
    void updateRowsWhere(const string &conditionCol, const string &conditionVal,
                         const string &targetCol, const string &newValue);
    void printAllRows() const;
    void describeTable(const string &tableName) const;
    void rollback(const string &versionName);

    // File I/O
    void saveToFile(const string &filename) const;
    void loadFromFile(const string &filename);
    void saveTableVersion();
};

#endif
