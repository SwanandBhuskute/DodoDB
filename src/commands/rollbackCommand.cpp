#include <iostream>
#include <sstream>
#include <string>
#include "../tables/table.h"
#include "../tables/table_manager.h"
using namespace std;

extern TableManager manager;

void rollbackCommand(const string &input)
{
    Table *table = manager.getCurrentTable();

    if (input.substr(0, 8) == "rollback")
    {
        string versionName = input.substr(9);

        versionName.erase(0, versionName.find_first_not_of(" \t"));
        versionName.erase(versionName.find_last_not_of(" \t") + 1);

        if (!versionName.empty())
        {
            table->rollback(versionName);
        }
        else
        {
            cout << "Error: Invalid version name. Please provide a version number.\n";
        }
    }
    else
    {
        cout << "Error: Invalid command. Expected 'rollback <version>'\n";
    }
}
