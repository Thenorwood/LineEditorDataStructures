#include "LineEditor.h"
#include <cstdlib>

using std::cout;
using std::cerr;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::cin;
using std::getline;
using std::exit;

// CONSTRUCTOR -  load file & set insertion point.
LineEditor::LineEditor(const string &file)
        : head(nullptr), lineCount(0), currentLine(1), filename(file) {
    loadFile();
    currentLine = lineCount + 1;
}

// DESTRUCTOR - save file & free memory.
LineEditor::~LineEditor() {
    saveFile();
    clearList();
}

// Load file: read each line into linked list.
void LineEditor::loadFile() {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "Creating new file: " << filename << endl;
        return;
    }
    string line;
    while (getline(inFile, line))
        insertLineAt(line, lineCount + 1);
    inFile.close();
}

//Save file - write list contents file.
void LineEditor::saveFile() {
    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cerr << "Error saving file!" << endl;
        return;
    }
    Node* current = head;
    while (current) {
        outFile << current->data << "\n";
        current = current->next;
    }
    outFile.close();
}

//Clear list - free nodes.
void LineEditor::clearList() {
    Node* temp;
    while (head) {
        temp = head;
        head = head->next;
        delete temp;
    }
}

//Ins text at given position.
void LineEditor::insertLineAt(const string &text, int pos) {
    Node* newNode = new Node(text);
    if (pos <= 1 || !head) {
        newNode->next = head;
        head = newNode;
    } else {
        Node* prev = head;
        int curPos = 1;
        while (prev->next && curPos < pos - 1) {
            prev = prev->next;
            curPos++;
        }
        newNode->next = prev->next;
        prev->next = newNode;
    }
    lineCount++;
}

//Return node at given position.
LineEditor::Node* LineEditor::getNodeAt(int pos) {
    if (pos < 1 || pos > lineCount)
        return nullptr;
    Node* cur = head;
    int curPos = 1;
    while (cur && curPos < pos) {
        cur = cur->next;
        curPos++;
    }
    return cur;
}

//Delete node at given position.
void LineEditor::deleteLineAt(int pos) {
    if (pos < 1 || pos > lineCount) {
        cout << "Invalid line number to delete." << endl;
        return;
    }
    if (pos == 1) {
        Node* temp = head;
        head = head->next;
        delete temp;
    } else {
        Node* prev = getNodeAt(pos - 1);
        if (prev && prev->next) {
            Node* temp = prev->next;
            prev->next = temp->next;
            delete temp;
        }
    }
    lineCount--;
    if (currentLine > lineCount + 1)
        currentLine = lineCount + 1;
}

// Delete nodes in range.
void LineEditor::deleteRange(int start, int end) {
    if (start > end || start < 1 || end > lineCount) {
        cout << "Invalid range for deletion." << endl;
        return;
    }
    for (int i = start; i <= end; i++)
        deleteLineAt(start);
    if (currentLine > lineCount + 1)
        currentLine = lineCount + 1;
}

// List all nodes.
void LineEditor::listAll() {
    Node* cur = head;
    int pos = 1;
    while (cur) {
        cout << pos << "> " << cur->data << endl;
        cur = cur->next;
        pos++;
    }
    currentLine = lineCount + 1;
}

// List specific node.
void LineEditor::listLine(int pos) {
    Node* node = getNodeAt(pos);
    if (node) {
        cout << pos << "> " << node->data << endl;
        currentLine = pos + 1;
    } else {
        cout << "Line " << pos << " does not exist." << endl;
    }
}

// List nodes in a range.
void LineEditor::listRange(int start, int end) {
    if (start > end || start < 1 || end > lineCount) {
        cout << "Invalid range for listing." << endl;
        return;
    }
    for (int i = start; i <= end; i++) {
        Node* node = getNodeAt(i);
        if (node)
            cout << i << "> " << node->data << endl;
    }
    currentLine = end + 1;
}

// Process input - commands or text.
void LineEditor::processCommand(const string &input) {
    if (input.empty())
        return;
    istringstream iss(input);
    string token;
    iss >> token;
    if (token == "E") {
        saveFile();
        exit(0);
    } else if (token == "Q") {
        exit(0);
    } else if (token == "I") {
        int target;
        if (iss >> target) {
            if (target < 1 || target > lineCount + 1)
                cout << "Invalid line number for insertion." << endl;
            else
                currentLine = target;
        }
    } else if (token == "L") {
        int p1, p2;
        if (!(iss >> p1))
            listAll();
        else if (!(iss >> p2)) {
            if (p1 < 1 || p1 > lineCount)
                cout << "Invalid line number for listing." << endl;
            else
                listLine(p1);
        } else {
            if (p1 < 1 || p2 > lineCount || p1 > p2)
                cout << "Invalid range for listing." << endl;
            else
                listRange(p1, p2);
        }
    } else if (token == "D") {
        int p1, p2;
        if (!(iss >> p1)) {
            if (currentLine - 1 < 1)
                cout << "No previous line to delete." << endl;
            else {
                deleteLineAt(currentLine - 1);
                currentLine--;
            }
        } else if (!(iss >> p2)) {
            if (p1 < 1 || p1 > lineCount)
                cout << "Invalid line number to delete." << endl;
            else
                deleteLineAt(p1);
            if (currentLine > lineCount + 1)
                currentLine = lineCount + 1;
        } else {
            if (p1 < 1 || p2 > lineCount || p1 > p2)
                cout << "Invalid range for deletion." << endl;
            else
                deleteRange(p1, p2);
        }
    } else {
        // Insert text
        insertLineAt(input, currentLine);
        currentLine++;
    }
}

// Main loop: display prompt & get input.
void LineEditor::displayPrompt() {
    string input;
    while (true) {
        cout << currentLine << "> ";
        getline(cin, input);
        processCommand(input);
    }
}