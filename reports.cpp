#include "reports.h"

// Parse double helper (same pattern as fee_tracker — no stod allowed)
static double rParseDouble(const string& s) {
    double val = 0.0;
    int dotPos = -1;
    for (int i = 0; i < (int)s.length(); i++)
        if (s[i] == '.') { dotPos = i; break; }
    if (dotPos == -1) {
        for (int i = 0; i < (int)s.length(); i++)
            val = val * 10 + (s[i] - '0');
    } else {
        double ip = 0, fp = 0, div = 1;
        for (int i = 0; i < dotPos; i++) ip = ip*10 + (s[i]-'0');
        for (int i = dotPos+1; i < (int)s.length(); i++) {
            fp = fp*10 + (s[i]-'0'); div *= 10;
        }
        val = ip + fp / div;
    }
    return val;
}

void printMeritList() {
    vector<vector<string>> students = listActiveStudents(); // already sorted by roll

    // Selection sort by CGPA descending
    for (int i = 0; i < (int)students.size() - 1; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < (int)students.size(); j++) {
            if (rParseDouble(students[j][4]) > rParseDouble(students[maxIdx][4]))
                maxIdx = j;
        }
        if (maxIdx != i) {
            vector<string> tmp = students[i];
            students[i] = students[maxIdx];
            students[maxIdx] = tmp;
        }
    }

    cout << "\n" << string(55, '=') << "\n";
    cout << "                  MERIT LIST\n";
    cout << string(55, '=') << "\n";
    cout << left << setw(6) << "Rank"
         << setw(15) << "Roll No"
         << setw(22) << "Name"
         << setw(8)  << "CGPA" << "\n";
    cout << string(55, '-') << "\n";

    for (int i = 0; i < (int)students.size(); i++) {
        cout << left << setw(6) << i + 1
             << setw(15) << students[i][0]
             << setw(22) << students[i][1]
             << setw(8)  << students[i][4] << "\n";
    }
    cout << string(55, '=') << "\n";
}

void printAttendanceDefaulters() {
    vector<vector<string>> courses = readTXT("courses.txt");

    cout << "\n" << string(60, '=') << "\n";
    cout << "            ATTENDANCE DEFAULTERS (<75%)\n";
    cout << string(60, '=') << "\n";
    cout << left << setw(15) << "Roll"
         << setw(22) << "Name"
         << setw(10) << "Course"
         << setw(10) << "Attend%" << "\n";
    cout << string(60, '-') << "\n";

    bool anyFound = false;
    for (int c = 0; c < (int)courses.size(); c++) {
        string code = courses[c][0];
        vector<vector<string>> shortage = getShortageList(code);
        for (int i = 0; i < (int)shortage.size(); i++) {
            string roll = shortage[i][0];
            vector<string> student = searchByRoll(roll);
            string name = student.empty() ? roll : student[1];
            cout << left << setw(15) << roll
                 << setw(22) << name
                 << setw(10) << code
                 << setw(10) << shortage[i][2] << "\n";
            anyFound = true;
        }
    }
    if (!anyFound) cout << "No attendance defaulters found.\n";
    cout << string(60, '=') << "\n";
}

void printFeeDefaulters() {
    vector<vector<string>> defaulters = getDefaulters();

    cout << "\n" << string(65, '=') << "\n";
    cout << "                FEE DEFAULTERS\n";
    cout << string(65, '=') << "\n";
    cout << left << setw(15) << "Roll"
         << setw(22) << "Name"
         << setw(10) << "Total"
         << setw(10) << "Paid"
         << setw(10) << "Balance"
         << setw(8)  << "Status" << "\n";
    cout << string(65, '-') << "\n";

    if (defaulters.empty()) {
        cout << "No fee defaulters.\n";
    } else {
        for (int i = 0; i < (int)defaulters.size(); i++) {
            string roll = defaulters[i][1];
            vector<string> student = searchByRoll(roll);
            string name = student.empty() ? roll : student[1];
            double balance = rParseDouble(defaulters[i][3]) - rParseDouble(defaulters[i][4]);
            cout << left  << setw(15) << roll
                 << setw(22) << name
                 << right << setw(8)  << defaulters[i][3]
                 << setw(8)  << defaulters[i][4]
                 << setw(9)  << (int)balance
                 << "  " << defaulters[i][8] << "\n";
        }
    }
    cout << string(65, '=') << "\n";
}

void printSemesterResult(int semester) {
    vector<vector<string>> students = listActiveStudents();
    vector<vector<string>> grades   = readTXT("grades.txt");
    vector<vector<string>> courses  = readTXT("courses.txt");
    vector<vector<string>> enrollments = readTXT("enrollments.txt");

    cout << "\n" << string(70, '=') << "\n";
    cout << "            SEMESTER " << semester << " RESULT SHEET\n";
    cout << string(70, '=') << "\n";

    for (int s = 0; s < (int)students.size(); s++) {
        string roll = students[s][0];
        string name = students[s][1];

        cout << "\n" << string(70, '-') << "\n";
        cout << "Roll: " << roll << "  |  Name: " << name << "\n";
        cout << string(70, '-') << "\n";
        cout << left << setw(10) << "Course"
             << setw(10) << "Credits"
             << setw(10) << "Grade"
             << setw(10) << "GPA Pts"
             << setw(12) << "Attendance" << "\n";
        cout << string(50, '-') << "\n";

        double semGPA = computeGPA(roll, semester);

        for (int e = 0; e < (int)enrollments.size(); e++) {
            if (enrollments[e][1] != roll ||
                enrollments[e][3] != to_string(semester) ||
                enrollments[e][5] != "active") continue;

            string code = enrollments[e][2];
            string grade = "N/A";
            string gpaP  = "N/A";

            for (int g = 0; g < (int)grades.size(); g++) {
                if (grades[g][0] == roll && grades[g][1] == code) {
                    grade = grades[g][7];
                    gpaP  = grades[g][8];
                    break;
                }
            }

            vector<string> course = findRow(courses, 0, code);
            string credits = course.empty() ? "?" : course[2];

            double attPct = getAttendancePct(roll, code);
            int attInt = (int)attPct;
            string attStr = to_string(attInt) + "%" + (attPct < 75.0 ? " !" : "");

            cout << left << setw(10) << code
                 << setw(10) << credits
                 << setw(10) << grade
                 << setw(10) << gpaP
                 << setw(12) << attStr << "\n";
        }

        // Format semGPA
        int gpaI = (int)(semGPA * 100);
        string gpaStr = to_string(gpaI / 100) + "." +
                        (gpaI % 100 < 10 ? "0" : "") + to_string(gpaI % 100);
        cout << string(50, '-') << "\n";
        cout << "Semester GPA: " << gpaStr << "\n";
    }
    cout << "\n" << string(70, '=') << "\n";
}

void printDepartmentSummary() {
    vector<vector<string>> students = readTXT("students.txt");

    // Collect unique departments using parallel arrays
    string depts[50];
    int    deptCount[50];
    double deptCGPASum[50];
    int    deptPass[50];   // CGPA >= 2.0 = passing
    int    numDepts = 0;

    for (int i = 0; i < (int)students.size(); i++) {
        if (students[i][5] != "active") continue;
        string dept = students[i][2];

        // Find dept index
        int idx = -1;
        for (int d = 0; d < numDepts; d++) {
            if (depts[d] == dept) { idx = d; break; }
        }
        if (idx == -1) {
            idx = numDepts;
            depts[idx] = dept;
            deptCount[idx] = 0;
            deptCGPASum[idx] = 0.0;
            deptPass[idx] = 0;
            numDepts++;
        }
        double cgpa = rParseDouble(students[i][4]);
        deptCount[idx]++;
        deptCGPASum[idx] += cgpa;
        if (cgpa >= 2.0) deptPass[idx]++;
    }

    cout << "\n" << string(60, '=') << "\n";
    cout << "           DEPARTMENT SUMMARY\n";
    cout << string(60, '=') << "\n";
    cout << left << setw(28) << "Department"
         << setw(8)  << "Count"
         << setw(10) << "Avg CGPA"
         << setw(10) << "Pass Rate" << "\n";
    cout << string(60, '-') << "\n";

    for (int d = 0; d < numDepts; d++) {
        double avg = deptCount[d] > 0 ? deptCGPASum[d] / deptCount[d] : 0.0;
        int avgI = (int)(avg * 100);
        string avgStr = to_string(avgI/100) + "." +
                        (avgI%100 < 10 ? "0" : "") + to_string(avgI%100);

        double passRate = deptCount[d] > 0 ?
                          (deptPass[d] * 100.0 / deptCount[d]) : 0.0;
        int prI = (int)passRate;

        cout << left << setw(28) << depts[d]
             << setw(8)  << deptCount[d]
             << setw(10) << avgStr
             << prI << "%\n";
    }
    cout << string(60, '=') << "\n";
}

void exportReportToFile(int reportChoice, int semester) {
    string filename;
    if      (reportChoice == 1) filename = "report_merit.txt";
    else if (reportChoice == 2) filename = "report_attendance_defaulters.txt";
    else if (reportChoice == 3) filename = "report_fee_defaulters.txt";
    else if (reportChoice == 4) filename = "report_semester_result.txt";
    else if (reportChoice == 5) filename = "report_dept_summary.txt";
    else { cout << "Invalid report choice.\n"; return; }

    ofstream outFile(filename);
    if (!outFile.is_open()) {
        cout << "ERROR: Cannot create file " << filename << "\n";
        return;
    }

    // Redirect cout to file
    streambuf* origBuf = cout.rdbuf(outFile.rdbuf());

    if      (reportChoice == 1) printMeritList();
    else if (reportChoice == 2) printAttendanceDefaulters();
    else if (reportChoice == 3) printFeeDefaulters();
    else if (reportChoice == 4) printSemesterResult(semester);
    else if (reportChoice == 5) printDepartmentSummary();

    // Restore cout
    cout.rdbuf(origBuf);
    outFile.close();

    cout << "Report saved to: " << filename << "\n";
}