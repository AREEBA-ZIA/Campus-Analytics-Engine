#include <iostream>
#include <iomanip>
#include "filehandler.h"
#include "student_ops.h"
#include "course_ops.h"
#include "attendance.h"
#include "grades.h"
#include "fee_tracker.h"
#include "reports.h"

using namespace std;

void studentMenu() {
    int choice;
    do {
        cout << "\n--- Student Management ---\n";
        cout << "1. Add Student\n";
        cout << "2. Search by Roll\n";
        cout << "3. Search by Name\n";
        cout << "4. List All Active Students\n";
        cout << "5. Update Student\n";
        cout << "6. Delete Student (soft)\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            string roll, name, dept;
            int sem;
            double cgpa;
            cout << "Roll (BSAI-YY-XXX): "; cin >> roll;
            cin.ignore();
            cout << "Name: "; getline(cin, name);
            cout << "Department: "; getline(cin, dept);
            cout << "Semester: "; cin >> sem;
            cout << "CGPA: "; cin >> cgpa;
            addStudent(roll, name, dept, sem, cgpa);
        }
        else if (choice == 2) {
            string roll;
            cout << "Enter Roll: "; cin >> roll;
            vector<string> s = searchByRoll(roll);
            if (s.empty()) cout << "Not found.\n";
            else cout << s[0] <<" | "<< s[1] <<" | CGPA: "<< s[4] <<" | "<< s[5] <<"\n";
        }
        else if (choice == 3) {
            string name;
            cin.ignore();
            cout << "Enter name (or part): "; getline(cin, name);
            vector<vector<string>> results = searchByName(name);
            if (results.empty()) cout << "No matches.\n";
            for (int i = 0; i < (int)results.size(); i++)
                cout << results[i][0] <<" | "<< results[i][1] <<" | CGPA: "<< results[i][4] <<"\n";
        }
        else if (choice == 4) {
            vector<vector<string>> list = listActiveStudents();
            cout << "\n" << left << setw(15) << "Roll"
                 << setw(20) << "Name"
                 << setw(10) << "CGPA" << "\n";
            cout << string(45, '-') << "\n";
            for (int i = 0; i < (int)list.size(); i++)
                cout << setw(15) << list[i][0]
                     << setw(20) << list[i][1]
                     << setw(10) << list[i][4] << "\n";
        }
        else if (choice == 5) {
            string roll, val;
            int col;
            cout << "Roll: "; cin >> roll;
            cout << "Field to update (1=name, 2=dept, 3=sem, 4=cgpa): "; cin >> col;
            cout << "New value: "; cin >> val;
            updateStudent(roll, col, val);
        }
        else if (choice == 6) {
            string roll;
            cout << "Enter Roll to delete: "; cin >> roll;
            softDelete(roll);
        }
    } while (choice != 0);
}

void courseMenu() {
    int choice;
    do {
        cout << "\n--- Course Management ---\n";
        cout << "1. Enroll Student\n";
        cout << "2. Drop Course\n";
        cout << "3. View Credit Load\n";
        cout << "4. List Students in Course\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            string roll, code;
            int sem;
            cout << "Roll: "; cin >> roll;
            cout << "Course Code: "; cin >> code;
            cout << "Semester: "; cin >> sem;
            EnrollResult r = enrollStudent(roll, code, sem);
            if (r == ENROLL_SUCCESS)                cout << "Enrolled successfully!\n";
            else if (r == ENROLL_STUDENT_NOT_FOUND) cout << "ERROR: Student not found.\n";
            else if (r == ENROLL_STUDENT_INACTIVE)  cout << "ERROR: Student is inactive.\n";
            else if (r == ENROLL_COURSE_NOT_FOUND)  cout << "ERROR: Course not found.\n";
            else if (r == ENROLL_ALREADY_ENROLLED)  cout << "ERROR: Already enrolled.\n";
            else if (r == ENROLL_SEAT_FULL)         cout << "ERROR: Course is full.\n";
            else if (r == ENROLL_CREDIT_OVERLOAD)   cout << "ERROR: Exceeds 21 credit hours.\n";
            else if (r == ENROLL_PREREQ_NOT_MET)    cout << "ERROR: Prerequisite not met.\n";
        }
        else if (choice == 2) {
            string roll, code; int sem;
            cout << "Roll: "; cin >> roll;
            cout << "Course Code: "; cin >> code;
            cout << "Semester: "; cin >> sem;
            if (dropCourse(roll, code, sem)) cout << "Course dropped.\n";
            else cout << "Cannot drop (attendance exists or not found).\n";
        }
        else if (choice == 3) {
            string roll; int sem;
            cout << "Roll: "; cin >> roll;
            cout << "Semester: "; cin >> sem;
            cout << "Credit load: " << getCreditLoad(roll, sem) << " hours\n";
        }
        else if (choice == 4) {
            string code;
            cout << "Course Code: "; cin >> code;
            vector<vector<string>> list = listEnrolledStudents(code);
            cout << "\nEnrolled in " << code << ":\n";
            for (int i = 0; i < (int)list.size(); i++)
                cout << list[i][1] << "\n";
        }
    } while (choice != 0);
}

void attendanceMenu() {
    int choice;
    do {
        cout << "\n--- Attendance Management ---\n";
        cout << "1. Mark Attendance\n";
        cout << "2. View Attendance %\n";
        cout << "3. View Shortage List (<75%)\n";
        cout << "4. Print Daily Sheet\n";
        cout << "5. Undo Last Session\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            string code, date;
            cout << "Course Code: "; cin >> code;
            cout << "Date (DD-MM-YYYY): "; cin >> date;
            markAttendance(code, date);
        }
        else if (choice == 2) {
            string roll, code;
            cout << "Roll: "; cin >> roll;
            cout << "Course Code: "; cin >> code;
            double pct = getAttendancePct(roll, code);
            cout << "Attendance: " << (int)pct << "%\n";
            if (pct < 75.0) cout << "WARNING: Below 75%!\n";
        }
        else if (choice == 3) {
            string code;
            cout << "Course Code: "; cin >> code;
            vector<vector<string>> list = getShortageList(code);
            if (list.empty()) cout << "No shortage cases.\n";
            for (int i = 0; i < (int)list.size(); i++)
                cout << list[i][0] << " - " << list[i][2] << "\n";
        }
        else if (choice == 4) {
            string code, date;
            cout << "Course Code: "; cin >> code;
            cout << "Date (DD-MM-YYYY): "; cin >> date;
            printDailySheet(code, date);
        }
        else if (choice == 5) {
            string code;
            cout << "Course Code: "; cin >> code;
            undoLastSession(code);
        }
    } while (choice != 0);
}

void gradesMenu() {
    int choice;
    do {
        cout << "\n--- Grade Management ---\n";
        cout << "1. Enter Marks\n";
        cout << "2. View Student GPA\n";
        cout << "3. View Class Statistics\n";
        cout << "4. Apply Attendance Penalty\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            string roll, code;
            cout << "Roll: "; cin >> roll;
            cout << "Course Code: "; cin >> code;
            enterMarks(roll, code);
        }
        else if (choice == 2) {
            string roll; int sem;
            cout << "Roll: "; cin >> roll;
            cout << "Semester: "; cin >> sem;
            double gpa = computeGPA(roll, sem);
            int g = (int)(gpa * 100);
            cout << "Semester GPA: " << g/100 << "."
                 << (g%100 < 10 ? "0" : "") << g%100 << "\n";
        }
        else if (choice == 3) {
            string code;
            cout << "Course Code: "; cin >> code;
            Stats s = computeClassStats(code);
            cout << "Highest: " << s.highest << "\n";
            cout << "Lowest:  " << s.lowest  << "\n";
            cout << "Mean:    " << s.mean    << "\n";
            cout << "Median:  " << s.median  << "\n";
        }
        else if (choice == 4) {
            string roll, code;
            cout << "Roll: "; cin >> roll;
            cout << "Course Code: "; cin >> code;
            applyAttendancePenalty(roll, code);
        }
    } while (choice != 0);
}

void feeMenu() {
    int choice;
    do {
        cout << "\n--- Fee Management ---\n";
        cout << "1. Record Payment\n";
        cout << "2. Compute Late Fine\n";
        cout << "3. Generate Receipt\n";
        cout << "4. View Fee Defaulters\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) {
            string roll, payDate, method;
            int sem; double amount;
            cout << "Roll: "; cin >> roll;
            cout << "Semester: "; cin >> sem;
            cout << "Amount: "; cin >> amount;
            cout << "Payment Date (DD-MM-YYYY): "; cin >> payDate;
            cout << "Method (Cash/Online/Bank): "; cin >> method;
            recordPayment(roll, sem, amount, payDate, method);
        }
        else if (choice == 2) {
            string roll; int sem;
            cout << "Roll: "; cin >> roll;
            cout << "Semester: "; cin >> sem;
            double fine = computeLateFine(roll, sem);
            cout << "Late Fine: " << (int)fine << "\n";
        }
        else if (choice == 3) {
            string roll; int sem;
            cout << "Roll: "; cin >> roll;
            cout << "Semester: "; cin >> sem;
            generateReceipt(roll, sem);
        }
        else if (choice == 4) {
            vector<vector<string>> def = getDefaulters();
            if (def.empty()) { cout << "No fee defaulters.\n"; }
            else {
                cout << "\n" << left << setw(15) << "Roll"
                     << setw(10) << "Total"
                     << setw(10) << "Paid"
                     << setw(10) << "Balance"
                     << setw(10) << "Status" << "\n";
                cout << string(55, '-') << "\n";
                for (int i = 0; i < (int)def.size(); i++) {
                    double bal = 0;
                    // parse manually
                    double total = 0, paid = 0;
                    string ts = def[i][3], ps = def[i][4];
                    for (int j=0;j<(int)ts.length();j++) if(ts[j]!='.') total=total*10+(ts[j]-'0'); else { double f=0,d=1; for(int k=j+1;k<(int)ts.length();k++){f=f*10+(ts[k]-'0');d*=10;} total+=f/d; break; }
                    for (int j=0;j<(int)ps.length();j++) if(ps[j]!='.') paid=paid*10+(ps[j]-'0'); else { double f=0,d=1; for(int k=j+1;k<(int)ps.length();k++){f=f*10+(ps[k]-'0');d*=10;} paid+=f/d; break; }
                    bal = total - paid;
                    cout << left << setw(15) << def[i][1]
                         << setw(10) << def[i][3]
                         << setw(10) << def[i][4]
                         << setw(10) << (int)bal
                         << setw(10) << def[i][8] << "\n";
                }
            }
        }
    } while (choice != 0);
}

void reportsMenu() {
    int choice;
    do {
        cout << "\n--- Reports ---\n";
        cout << "1. Merit List\n";
        cout << "2. Attendance Defaulters\n";
        cout << "3. Fee Defaulters\n";
        cout << "4. Semester Result Sheet\n";
        cout << "5. Department Summary\n";
        cout << "6. Export Report to File\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) printMeritList();
        else if (choice == 2) printAttendanceDefaulters();
        else if (choice == 3) printFeeDefaulters();
        else if (choice == 4) {
            int sem;
            cout << "Semester: "; cin >> sem;
            printSemesterResult(sem);
        }
        else if (choice == 5) printDepartmentSummary();
        else if (choice == 6) {
            int rpt, sem = 0;
            cout << "Which report to export?\n";
            cout << "1=Merit  2=Att.Defaulters  3=Fee Defaulters  4=Sem Result  5=Dept Summary\n";
            cout << "Choice: "; cin >> rpt;
            if (rpt == 4) { cout << "Semester: "; cin >> sem; }
            exportReportToFile(rpt, sem);
        }
    } while (choice != 0);
}

int main() {
    int choice;
    do {
        cout << "\n==================================";
        cout << "\n   CAMPUS ANALYTICS ENGINE";
        cout << "\n==================================\n";
        cout << "1. Student Management\n";
        cout << "2. Course Management\n";
        cout << "3. Attendance Management\n";
        cout << "4. Grade Management\n";
        cout << "5. Fee Management\n";
        cout << "6. Reports\n";
        cout << "0. Exit\n";
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: studentMenu();    break;
            case 2: courseMenu();     break;
            case 3: attendanceMenu(); break;
            case 4: gradesMenu();     break;
            case 5: feeMenu();        break;
            case 6: reportsMenu();    break;
            case 0: cout << "\nGoodbye!\n"; break;
            default: cout << "\nInvalid choice!\n";
        }
    } while (choice != 0);
    return 0;
}