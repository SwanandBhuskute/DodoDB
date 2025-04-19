#include <iostream>
#include "../tables/table.h"
#include "../tables/table_manager.h"
#include "../../utils/string_utils.h"
using namespace std;

extern TableManager manager;

void deleteCommand(string &input)
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
