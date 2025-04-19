#include <iostream>
#include "../tables/table.h"
#include "../tables/table_manager.h"
#include "../../utils/string_utils.h"
using namespace std;

extern TableManager manager;

void useTableCommand(string &input)
{
    string tableName = trim(input.substr(10));
    if (tableName.empty())
    {
        cout << "Please specify a table name. Usage: use table <table_name>\n";
        return;
    }

    manager.useTable(tableName);
}
