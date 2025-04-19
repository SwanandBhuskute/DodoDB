#include <iostream>
#include "../tables/table.h"
#include "../tables/table_manager.h"
#include "../../utils/string_utils.h"
using namespace std;

extern TableManager manager;

void updateCommand(string &input)
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
