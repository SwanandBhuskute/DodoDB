#include <iostream>
#include "../tables/table.h"
#include "../tables/table_manager.h"
#include "../../utils/string_utils.h"
using namespace std;

extern TableManager manager;

void currTableCommand(string &input)
{
    Table *table = manager.getCurrentTable();
    if (!table)
    {
        cout << "No table selected. Use: use table <name>\n";
        return;
    }

    cout << "Curr table: " << manager.getCurrentTableName() << endl;
}
