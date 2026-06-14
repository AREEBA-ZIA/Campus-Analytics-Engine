#include "course_ops.h"

EnrollResult enrollStudent(const string& roll, const string& courseCode, int semester) {
    vector<string> student = searchByRoll(roll);
    if (student.empty()) return ENROLL_STUDENT_NOT_FOUND;
    if (student[5] != "active") return ENROLL_STUDENT_INACTIVE;

    vector<vector<string>> courses = readTXT("courses.txt");
    vector<string> course = findRow(courses, 0, courseCode);
    if (course.empty()) return ENROLL_COURSE_NOT_FOUND;

    vector<vector<string>> enrollments = readTXT("enrollments.txt");
    for (int i = 0; i < (int)enrollments.size(); i++) {
        if (enrollments[i][1] == roll &&
            enrollments[i][2] == courseCode &&
            enrollments[i][5] == "active") {
            return ENROLL_ALREADY_ENROLLED;
        }
    }

    int capacity = 0;
    for (int i = 0; i < (int)course[4].length(); i++)
        capacity = capacity * 10 + (course[4][i] - '0');

    int enrolledCount = 0;
    for (int i = 0; i < (int)enrollments.size(); i++)
        if (enrollments[i][2] == courseCode && enrollments[i][5] == "active")
            enrolledCount++;

    if (enrolledCount >= capacity) return ENROLL_SEAT_FULL;

    int currentCredits = getCreditLoad(roll, semester);
    int courseCredits = 0;
    for (int i = 0; i < (int)course[2].length(); i++)
        courseCredits = courseCredits * 10 + (course[2][i] - '0');

    if (currentCredits + courseCredits > 21) return ENROLL_CREDIT_OVERLOAD;

    if (!checkPrerequisite(roll, courseCode)) return ENROLL_PREREQ_NOT_MET;

    string newId = "E";
    int nextNum = (int)enrollments.size() + 1;
    string numStr = to_string(nextNum);
    while ((int)numStr.length() < 4) numStr = "0" + numStr;
    newId += numStr;

    vector<string> newRow;
    newRow.push_back(newId);
    newRow.push_back(roll);
    newRow.push_back(courseCode);
    newRow.push_back(to_string(semester));
    newRow.push_back("15-01-2024");
    newRow.push_back("active");
    appendTXT("enrollments.txt", newRow);

    for (int i = 0; i < (int)courses.size(); i++) {
        if (courses[i][0] == courseCode) {
            int count = 0;
            for (int j = 0; j < (int)courses[i][5].length(); j++)
                count = count * 10 + (courses[i][5][j] - '0');
            count++;
            courses[i][5] = to_string(count);
            break;
        }
    }
    vector<string> courseHeader = {"course_code","course_name","credit_hours",
                                   "instructor","capacity","enrolled","prerequisite"};
    writeTXT("courses.txt", courseHeader, courses);

    return ENROLL_SUCCESS;
}

bool dropCourse(const string& roll, const string& courseCode, int semester) {
    vector<vector<string>> attendance = readTXT("attendance_log.txt");
    for (int i = 0; i < (int)attendance.size(); i++)
        if (attendance[i][0] == roll && attendance[i][1] == courseCode)
            return false;

    vector<vector<string>> enrollments = readTXT("enrollments.txt");
    bool found = false;
    for (int i = 0; i < (int)enrollments.size(); i++) {
        if (enrollments[i][1] == roll &&
            enrollments[i][2] == courseCode &&
            enrollments[i][3] == to_string(semester)) {
            enrollments[i][5] = "dropped";
            found = true;
            break;
        }
    }
    if (!found) return false;

    vector<string> header = {"enrollment_id","roll_no","course_code",
                             "semester","enrollment_date","status"};
    writeTXT("enrollments.txt", header, enrollments);
    return true;
}

int getCreditLoad(const string& roll, int semester) {
    vector<vector<string>> enrollments = readTXT("enrollments.txt");
    vector<vector<string>> courses = readTXT("courses.txt");

    int total = 0;
    for (int i = 0; i < (int)enrollments.size(); i++) {
        if (enrollments[i][1] == roll &&
            enrollments[i][3] == to_string(semester) &&
            enrollments[i][5] == "active") {
            vector<string> course = findRow(courses, 0, enrollments[i][2]);
            if (!course.empty()) {
                int credits = 0;
                for (int j = 0; j < (int)course[2].length(); j++)
                    credits = credits * 10 + (course[2][j] - '0');
                total += credits;
            }
        }
    }
    return total;
}

bool checkPrerequisite(const string& roll, const string& courseCode) {
    vector<vector<string>> courses = readTXT("courses.txt");
    vector<string> course = findRow(courses, 0, courseCode);
    if (course.empty()) return false;

    string prereq = course[6];
    if (prereq == "NONE") return true;

    vector<vector<string>> grades = readTXT("grades.txt");
    for (int i = 0; i < (int)grades.size(); i++)
        if (grades[i][0] == roll && grades[i][1] == prereq)
            if (grades[i][8] != "F") return true;

    return false;
}

vector<vector<string>> listEnrolledStudents(const string& courseCode) {
    vector<vector<string>> enrollments = readTXT("enrollments.txt");
    vector<vector<string>> result;
    for (int i = 0; i < (int)enrollments.size(); i++)
        if (enrollments[i][2] == courseCode && enrollments[i][5] == "active")
            result.push_back(enrollments[i]);
    return result;
}