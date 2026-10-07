// 
// 234218 Data Structures 1.
// Semester: 2025A (Winter).
// Wet Exercise #1.
// 
// The following main file is necessary to link and run your code.
// This file is READ ONLY: even if you submit something else, the compiler will use our file.
// 

#include "TechSystem26a1.h"
#include <string>
#include <iostream>

using namespace std;

void print(string cmd, StatusType res);
void print(string cmd, output_t<int> res);
void print(string cmd, output_t<bool> res);

int main()
{
    
    int d1, d2;

    // Init
    TechSystem *obj = new TechSystem();
    
    // Execute all commands in file
    string op;
    while (cin >> op)
    {
        if (!op.compare("addStudent")) {
            cin >> d1;
            print(op, obj->addStudent(d1));
        } else if (!op.compare("removeStudent")) {
            cin >> d1;
            print(op, obj->removeStudent(d1));
        } else if (!op.compare("addCourse")) {
            cin >> d1 >> d2;
            print(op, obj->addCourse(d1, d2));
        } else if (!op.compare("removeCourse")) {
            cin >> d1;
            print(op, obj->removeCourse(d1));
        } else if (!op.compare("enrollStudent")) {
            cin >> d1 >> d2;
            print(op, obj->enrollStudent(d1, d2));
        } else if (!op.compare("completeCourse")) {
            cin >> d1 >> d2;
            print(op, obj->completeCourse(d1, d2));
        } else if (!op.compare("awardAcademicPoints")) {
            cin >> d1;
            print(op, obj->awardAcademicPoints(d1));
        } else if (!op.compare("getStudentPoints")) {
            cin >> d1;
            print(op, obj->getStudentPoints(d1));
        } else {
            cout << "Unknown command: " << op << endl;
            return -1;
        }
        // Verify no faults
        if (cin.fail()){
            cout << "Invalid input format" << endl;
            return -1;
        }
    }

    // Quit 
    delete obj;
    return 0;
}

// Helpers
static const char *StatusTypeStr[] =
{
    "SUCCESS",
    "ALLOCATION_ERROR",
    "INVALID_INPUT",
    "FAILURE"
};

void print(string cmd, StatusType res) 
{
    cout << cmd << ": " << StatusTypeStr[(int) res] << endl;
}

void print(string cmd, output_t<int> res)
{
    if (res.status() == StatusType::SUCCESS) {
        cout << cmd << ": " << StatusTypeStr[(int) res.status()] << ", " << res.ans() << endl;
    } else {
        cout << cmd << ": " << StatusTypeStr[(int) res.status()] << endl;
    }
}

void print(string cmd, output_t<bool> res)
{
    if (res.status() == StatusType::SUCCESS) {
        cout << cmd << ": " << StatusTypeStr[(int) res.status()] << ", " << (res.ans() ? "True" : "False") << endl;
    } else {
        cout << cmd << ": " << StatusTypeStr[(int) res.status()] << endl;
    }
}