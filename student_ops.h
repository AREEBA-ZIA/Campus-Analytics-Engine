#ifndef STUDENT_OPS_H
#define STUDENT_OPS_H

#include <iostream>
#include <vector>
#include <string>
#include "filehandler.h"

using namespace std;

bool addStudent(const string& roll, const string& name, const string& dept,
                int semester, double cgpa);

vector<string> searchByRoll(const string& roll);
vector<vector<string>> searchByName(const string& namePart);
bool updateStudent(const string& roll, int colIndex, const string& newValue);
bool softDelete(const string& roll);
vector<vector<string>> listActiveStudents();

#endif