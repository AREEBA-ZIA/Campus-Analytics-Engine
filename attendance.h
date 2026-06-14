#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include <iostream>
#include <vector>
#include <string>
#include "filehandler.h"
#include "student_ops.h"
#include "course_ops.h"

using namespace std;

void markAttendance(const string& courseCode, const string& date);
double getAttendancePct(const string& roll, const string& courseCode);
vector<vector<string>> getShortageList(const string& courseCode);
bool undoLastSession(const string& courseCode);
void printDailySheet(const string& courseCode, const string& date);

#endif