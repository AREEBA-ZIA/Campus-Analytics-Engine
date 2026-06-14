#ifndef GRADES_H
#define GRADES_H

#include <iostream>
#include <vector>
#include <string>
#include "filehandler.h"
#include "student_ops.h"
#include "attendance.h"

using namespace std;

struct Stats {
    double highest;
    double lowest;
    double mean;
    double median;
};

void enterMarks(const string& roll, const string& courseCode);
double bestThreeOfFive(double marks[], int n);
double computeWeightedTotal(double quiz, double asgn, double mid, double final_);
string getLetterGrade(double total);
double computeGPA(const string& roll, int semester);
Stats computeClassStats(const string& courseCode);
void applyAttendancePenalty(const string& roll, const string& courseCode);

#endif