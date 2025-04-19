#include <iostream>
#include <string>
#include <sstream>
#include "../tables/table.h"
#include "../tables/table_manager.h"
using namespace std;

extern TableManager manager;

void insertCommand(string &input)
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
