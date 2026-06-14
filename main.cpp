#include <iostream>
#include <iomanip>
#include "filehandler.h"
#include "student_ops.h"
#include "course_ops.h"
#include "attendance.h"

using namespace std;

void studentMenu() {
    int choice;
    do {
        cout << "\n--- Student Management ---\n";
        cout << "1. Add Student\n";
        cout << "2. Search by Roll\n";
        cout << "3. Search by Name\n";
        cout << "4. List All Active Students\n";
        cout << "5. Delete Student (soft)\n";
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

        switch(choice) {
            case 1: studentMenu(); break;
            case 2: courseMenu(); break;
            case 3: attendanceMenu(); break;
            case 4: cout << "\n[Grade module coming soon]\n"; break;
            case 5: cout << "\n[Fee module coming soon]\n"; break;
            case 6: cout << "\n[Reports module coming soon]\n"; break;
            case 0: cout << "\nGoodbye!\n"; break;
            default: cout << "\nInvalid choice!\n";
        }
    } while (choice != 0);
    return 0;
}