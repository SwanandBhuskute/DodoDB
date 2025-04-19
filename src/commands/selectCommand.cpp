#include <iostream>
#include "../tables/table.h"
#include "../tables/table_manager.h"
using namespace std;

extern TableManager manager;

void selectCommand(string &input)
{
    Table *table = manager.getCurrentTable();
    if (!table)
    {
        cout << "No table selected. Use: use table <name>\n";
        return;
    }

    table->printAllRows();
}
