#include <iostream>
#include <iomanip>
#include "filehandler.h"
#include "student_ops.h"

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
            case 2: cout << "\n[Course module coming soon]\n"; break;
            case 3: cout << "\n[Attendance module coming soon]\n"; break;
            case 4: cout << "\n[Grade module coming soon]\n"; break;
            case 5: cout << "\n[Fee module coming soon]\n"; break;
            case 6: cout << "\n[Reports module coming soon]\n"; break;
            case 0: cout << "\nGoodbye!\n"; break;
            default: cout << "\nInvalid choice!\n";
        }
    } while (choice != 0);
    return 0;
}