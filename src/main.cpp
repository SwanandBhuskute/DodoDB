#include <iostream>
#include "../utils/string_utils.h"
using namespace std;

void handleInput(string& input) {
    input = toLower(input);

    if (input.substr(0, 6) == "insert") {
        cout << "This is where INSERT would be handled.\n";
    } else if (input.substr(0, 6) == "select") {
        cout << "This is where SELECT would be handled.\n";
    } else {
        cout << "Unrecognized command: " << input << endl;
    }
}

int main() {
    string input;

    cout << "$ ./dodoDb\n";
    while (true) {
        cout << "dodoDb > ";
        getline(cin, input);

        if (toLower(input) == "exit") {
            cout << "Bye says dodo!" << endl;
            break;
        }

        handleInput(input);
    }

    return 0;
}
