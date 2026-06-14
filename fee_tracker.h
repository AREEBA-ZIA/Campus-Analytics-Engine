#ifndef FEE_TRACKER_H
#define FEE_TRACKER_H

#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <string>
#include "filehandler.h"
#include "student_ops.h"

using namespace std;

bool recordPayment(const string& roll, int semester, double amount,
                   const string& payDate, const string& method);
int  daysBetween(const string& date1, const string& date2);
double computeLateFine(const string& roll, int semester);
void generateReceipt(const string& roll, int semester);
vector<vector<string>> getDefaulters();

#endif