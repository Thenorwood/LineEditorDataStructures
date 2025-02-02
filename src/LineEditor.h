#ifndef ASSIGNMENT1_LINEEDITOR_H
#define ASSIGNMENT1_LINEEDITOR_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using std::string;
using std::istream;
using std::ostream;

class LineEditor {
private:
    // Node for linked list
    struct Node {
        string data;
        Node* next;
        Node(const string &line) : data(line), next(nullptr) {}
    };

    // Data members
    Node* head;
    int lineCount;
    int currentLine;
    string filename;

    // File I/O routines
    void loadFile();
    void saveFile();
    void clearList();

    // List manipulation
    void insertLineAt(const string &text, int pos);
    void deleteLineAt(int pos);
    void deleteRange(int start, int end);
    Node* getNodeAt(int pos);

    // Listing routines
    void listAll();
    void listLine(int pos);
    void listRange(int start, int end);

    // Command processing
    void processCommand(const string &input);

public:
    // Constructor & Destructor
    LineEditor(const string &file);
    ~LineEditor();

    // Main loop
    void displayPrompt();
};

#endif // ASSIGNMENT1_LINEEDITOR_H