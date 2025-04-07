#include "string_utils.h"
#include <cctype>
using namespace std;

string toLower(const string& str) {
    string result = str;
    for (char& c : result) {
        c = tolower(c);
    }
    return result;
}
