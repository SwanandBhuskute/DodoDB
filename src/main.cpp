#include <iostream>
#include "../utils/linenoise.h"
#include "./commands/insertCommand.cpp"
#include "./commands/selectCommand.cpp"
#include "./commands/deleteCommand.cpp"
#include "./commands/updateCommand.cpp"
#include "./commands/createTableCommand.cpp"
#include "./commands/useTableCommand.cpp"
#include "./commands/currTableCommand.cpp"
#include "./commands/describeTableCommand.cpp"
#include "./commands/rollbackCommand.cpp"
using namespace std;

TableManager manager;

void handleInput(string &input)
{
    input = toLower(input);

    if (input.substr(0, 6) == "insert")
    {
        insertCommand(input);
    }
    else if (input.substr(0, 6) == "select")
    {
        selectCommand(input);
    }
    else if (input.substr(0, 6) == "delete")
    {
        deleteCommand(input);
    }
    else if (input.substr(0, 6) == "update")
    {
        updateCommand(input);
    }
    else if (input.substr(0, 12) == "create table")
    {
        createTableCommand(input);
    }
    else if (input.rfind("use table ", 0) == 0)
    {
        useTableCommand(input);
    }
    else if (input.substr(0, 10) == "curr table")
    {
        currTableCommand(input);
    }
    else if (input.substr(0, 8) == "describe")
    {
        describeTableCommand(input);
    }
    else if (input.substr(0, 8) == "rollback")
    {
        rollbackCommand(input);
    }
    else
    {
        cout << "Unrecognized command: " << input << endl;
    }
}

int main()
{
    cout << endl
         << "$ ./dodoDb" << endl;

    linenoiseHistoryLoad("utils/meta/dodo_history.txt");

    while (true)
    {
        char *inputRaw = linenoise("dodoDb > ");
        if (!inputRaw)
        {
            continue;
        }

        string input(inputRaw);
        free(inputRaw);

        input = trim(input);
        if (input.empty())
            continue;

        linenoiseHistoryAdd(input.c_str());

        if (toLower(input) == "exit")
        {
            linenoiseHistorySave("utils/meta/dodo_history.txt");
            cout << "Bye says dodo!" << endl;
            break;
        }

        // call for handling all commandds
        handleInput(input);
    }

    return 0;
}
