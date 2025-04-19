#include "table.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

Table::Table(const string &tableName, const vector<string> &columns)
    : name(tableName), columnNames(columns)
{
    string filename = "data/" + name + "/" + name + ".table.txt";
    loadFromFile(filename);

    if (columnNames.empty())
    {
        columnNames = columns;
        saveToFile(filename);
    }
}

Table::Table(const string &tableName)
    : name(tableName)
{
    string filename = "data/" + name + "/" + name + ".table.txt";
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
    saveToFile("data/" + name + "/" + name + ".table.txt");
    saveTableVersion();
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

    saveToFile("data/" + name + "/" + name + ".table.txt");
    saveTableVersion();

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

    saveToFile("data/" + name + "/" + name + ".table.txt");
    saveTableVersion();

    cout << "Updated " << updatedCount << " row(s)." << endl;
}

void Table::describeTable(const string &tableName) const
{
    cout << "Table: " << tableName << endl;
    cout << "Columns: " << endl;

    for (const auto &col : columnNames)
    {
        cout << "--- " << col << endl;
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

    for (size_t i = 0; i < columnNames.size(); ++i)
    {
        outFile << columnNames[i];
        if (i != columnNames.size() - 1)
            outFile << ",";
    }
    outFile << "\n";

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

string Table::getTableData()
{
    ostringstream dataStream;

    for (size_t i = 0; i < columnNames.size(); ++i)
    {
        dataStream << columnNames[i];
        if (i != columnNames.size() - 1)
            dataStream << ",";
    }
    dataStream << "\n";

    for (const auto &row : rows)
    {
        for (size_t i = 0; i < row.size(); ++i)
        {
            dataStream << row[i];
            if (i != row.size() - 1)
                dataStream << ",";
        }
        dataStream << "\n";
    }

    return dataStream.str();
}

void Table::saveTableVersion()
{
    fs::path versionsDir = "data/" + name + "/versions/";
    if (!fs::exists(versionsDir))
    {
        fs::create_directory(versionsDir);
    }

    int versionNumber = 0;
    for (const auto &entry : fs::directory_iterator(versionsDir))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".txt")
        {
            string filename = entry.path().filename().string();
            if (filename[0] == 'v')
            {
                try
                {
                    int fileVersion = stoi(filename.substr(1, filename.find(".txt") - 1));
                    versionNumber = max(versionNumber, fileVersion + 1);
                }
                catch (const invalid_argument &e)
                {
                    continue;
                }
            }
        }
    }

    string tableData = getTableData();
    fs::path versionFilePath = versionsDir / ("v" + to_string(versionNumber) + ".txt");
    ofstream versionFile(versionFilePath);

    if (!versionFile.is_open())
    {
        cerr << "Failed to open version file for writing.\n";
        return;
    }

    versionFile << tableData;
    versionFile.close();

    cout << "New version saved as " << versionFilePath << endl;
}

void Table::rollback(const string &versionName)
{
    int version = stoi(versionName.substr(1));
    fs::path versionsDir = "data/" + name + "/versions/";
    fs::path versionFilePath = versionsDir / ("v" + to_string(version) + ".txt");

    if (!fs::exists(versionFilePath))
    {
        cerr << "Error: Version " << version << " does not exist.\n";
        return;
    }

    ifstream versionFile(versionFilePath);
    if (!versionFile.is_open())
    {
        cerr << "Error: Could not open version file for reading.\n";
        return;
    }

    string line;
    vector<string> newColumnNames;
    vector<vector<string>> newRows;

    bool isFirstLine = true;
    while (getline(versionFile, line))
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
            newColumnNames = row;
            isFirstLine = false;
        }
        else
        {
            newRows.push_back(row);
        }
    }

    columnNames = newColumnNames;
    rows = newRows;

    for (const auto &entry : fs::directory_iterator(versionsDir))
    {
        if (entry.is_regular_file() && entry.path().extension() == ".txt")
        {
            string filename = entry.path().filename().string();
            if (filename[0] == 'v')
            {
                try
                {
                    int fileVersion = stoi(filename.substr(1, filename.find(".txt") - 1));
                    if (fileVersion > version)
                    {
                        fs::remove(entry.path());
                    }
                }
                catch (const invalid_argument &e)
                {
                    continue;
                }
            }
        }
    }

    saveToFile("data/" + name + "/" + name + ".table.txt");

    cout << "Rollback to version " << version << " complete.\n";
}
