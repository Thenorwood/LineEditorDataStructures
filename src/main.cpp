#include <iostream>
#include "LineEditor.h"

//Check args; create editor and start prompt.
using std::cerr;
using std::endl;

int main(int argc, char* argv[]) {
    // Validate arg count
    if (argc != 2) {
        cerr << "Usage: edit <filename>" << endl;
        return 1;
    }
    // Create editor & start command loop
    // extra comment for final commit
    LineEditor editor(argv[1]);
    editor.displayPrompt();
    return 0;
}