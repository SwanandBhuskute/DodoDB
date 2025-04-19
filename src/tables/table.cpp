#include "table.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

Table::Table(const string &tableName, const vector<string> &columns)
    : name(tableName), columnNames(columns)
{
    string filename = "data/" + name + ".table.txt";
    loadFromFile(filename);

    // If table has no columns, this is a new table
    if (columnNames.empty())
    {
        columnNames = columns;
        saveToFile(filename);
    }
}

Table::Table(const string &tableName)
    : name(tableName)
{
    string filename = "data/" + name + ".table.txt";
    loadFromFile(filename);
}

void Table::insertRow(const vector<string> &row)
{
    if (row.size() != columnNames.size())
    {
        cout << "Error: Row size doesn't match table schema." << endl;
        return;
    }

    rows.push_back(row);
    saveToFile("data/" + name + ".table.txt");
}

void Table::printAllRows() const
{
    if (rows.empty())
    {
        cout << "Empty\n";
        return;
    }

    for (const auto &col : columnNames)
    {
        cout << col << "\t";
    }
    cout << endl;

    for (const auto &row : rows)
    {
        for (const auto &val : row)
        {
            cout << val << "\t";
        }
        cout << endl;
    }
}

void Table::deleteRowsWhere(const string &columnName, const string &value)
{
    auto it = find(columnNames.begin(), columnNames.end(), columnName);
    if (it == columnNames.end())
    {
        cout << "Error: Column '" << columnName << "' not found." << endl;
        return;
    }

    int colIndex = distance(columnNames.begin(), it);
    int beforeCount = rows.size();

    rows.erase(remove_if(rows.begin(), rows.end(),
                         [&](const vector<string> &row)
                         {
                             return row[colIndex] == value;
                         }),
               rows.end());

    int afterCount = rows.size();

    saveToFile("data/" + name + ".table.txt");

    cout << "Deleted " << (beforeCount - afterCount) << " row(s)." << endl;
}

void Table::updateRowsWhere(const string &conditionCol, const string &conditionVal,
                            const string &targetCol, const string &newValue)
{
    auto condIt = find(columnNames.begin(), columnNames.end(), conditionCol);
    auto targetIt = find(columnNames.begin(), columnNames.end(), targetCol);

    if (condIt == columnNames.end())
    {
        cout << "Error: Column '" << conditionCol << "' not found." << endl;
        return;
    }

    if (targetIt == columnNames.end())
    {
        cout << "Error: Column '" << targetCol << "' not found." << endl;
        return;
    }

    int condIndex = distance(columnNames.begin(), condIt);
    int targetIndex = distance(columnNames.begin(), targetIt);

    int updatedCount = 0;

    for (auto &row : rows)
    {
        if (row[condIndex] == conditionVal)
        {
            row[targetIndex] = newValue;
            updatedCount++;
        }
    }

    saveToFile("data/" + name + ".table.txt");

    cout << "Updated " << updatedCount << " row(s)." << endl;
}

void Table::describeTable(const string &tableName) const
{
    cout << "Table: " << tableName << endl;
    cout << "Columns: " << endl;

    for (const auto &col : columnNames)
    {
        cout << "- " << col << endl;
    }
}

void Table::saveToFile(const string &filename) const
{
    ofstream outFile(filename);
    if (!outFile.is_open())
    {
        cout << "Error: Could not open file to save table.\n";
        return;
    }

    // Save column headers
    for (size_t i = 0; i < columnNames.size(); ++i)
    {
        outFile << columnNames[i];
        if (i != columnNames.size() - 1)
            outFile << ",";
    }
    outFile << "\n";

    // Save rows
    for (const auto &row : rows)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            outFile << row[i];
            if (i != row.size() - 1)
                outFile << ",";
        }
        outFile << "\n";
    }

    outFile.close();
}

void Table::loadFromFile(const string &filename)
{
    ifstream inFile(filename);
    if (!inFile.is_open())
    {
        cout << "No saved data found for this table.\n";
        return;
    }

    rows.clear();
    columnNames.clear();

    string line;
    bool isFirstLine = true;

    while (getline(inFile, line))
    {
        stringstream ss(line);
        string value;
        vector<string> row;

        while (getline(ss, value, ','))
        {
            row.push_back(value);
        }

        if (isFirstLine)
        {
            columnNames = row;
            isFirstLine = false;
        }
        else if (row.size() == columnNames.size())
        {
            rows.push_back(row);
        }
    }

    inFile.close();
}
