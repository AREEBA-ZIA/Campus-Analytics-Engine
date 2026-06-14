#ifndef COURSE_OPS_H
#define COURSE_OPS_H

#include <iostream>
#include <vector>
#include <string>
#include "filehandler.h"
#include "student_ops.h"

using namespace std;

enum EnrollResult {
    ENROLL_SUCCESS,
    ENROLL_STUDENT_NOT_FOUND,
    ENROLL_STUDENT_INACTIVE,
    ENROLL_COURSE_NOT_FOUND,
    ENROLL_ALREADY_ENROLLED,
    ENROLL_SEAT_FULL,
    ENROLL_CREDIT_OVERLOAD,
    ENROLL_PREREQ_NOT_MET
};

EnrollResult enrollStudent(const string& roll, const string& courseCode, int semester);
bool dropCourse(const string& roll, const string& courseCode, int semester);
int getCreditLoad(const string& roll, int semester);
bool checkPrerequisite(const string& roll, const string& courseCode);
vector<vector<string>> listEnrolledStudents(const string& courseCode);

#endif