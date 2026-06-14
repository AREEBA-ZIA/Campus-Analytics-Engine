#include "student_ops.h"

bool isValidRoll(const string& roll) {
    if (roll.length() != 12) return false;
    if (roll.substr(0, 5) != "BSAI-") return false;
    if (!isdigit(roll[5]) || !isdigit(roll[6])) return false;
    if (roll[7] != '-') return false;
    if (!isdigit(roll[8]) || !isdigit(roll[9]) || !isdigit(roll[10]) || !isdigit(roll[11])) return false;
    return true;
}

bool isValidName(const string& name) {
    for (int i = 0; i < (int)name.length(); i++)
        if (isdigit(name[i])) return false;
    return true;
}

bool addStudent(const string& roll, const string& name, const string& dept,
                int semester, double cgpa) {
    if (!isValidRoll(roll)) {
        cout << "ERROR: Roll must be BSAI-YY-XXX format.\n";
        return false;
    }
    if (!isValidName(name)) {
        cout << "ERROR: Name cannot contain digits.\n";
        return false;
    }
    if (cgpa < 0.0 || cgpa > 4.0) {
        cout << "ERROR: CGPA must be 0.0 to 4.0\n";
        return false;
    }

    vector<vector<string>> students = readTXT("students.txt");
    if (rowExists(students, 0, roll)) {
        cout << "ERROR: Student " << roll << " already exists.\n";
        return false;
    }

    // Format CGPA to 2 decimal places manually
    int cgpaInt = (int)(cgpa * 100);
    string cgpaStr = to_string(cgpaInt / 100) + "." +
                     (cgpaInt % 100 < 10 ? "0" : "") +
                     to_string(cgpaInt % 100);

    vector<string> newRow;
    newRow.push_back(roll);
    newRow.push_back(name);
    newRow.push_back(dept);
    newRow.push_back(to_string(semester));
    newRow.push_back(cgpaStr);
    newRow.push_back("active");

    appendTXT("students.txt", newRow);
    cout << "Student " << roll << " added successfully.\n";
    return true;
}

vector<string> searchByRoll(const string& roll) {
    vector<vector<string>> students = readTXT("students.txt");
    return findRow(students, 0, roll);
}

vector<vector<string>> searchByName(const string& namePart) {
    vector<vector<string>> students = readTXT("students.txt");
    vector<vector<string>> results;
    for (int i = 0; i < (int)students.size(); i++) {
        if (students[i][1].find(namePart) != string::npos)
            results.push_back(students[i]);
    }
    return results;
}

bool updateStudent(const string& roll, int colIndex, const string& newValue) {
    vector<vector<string>> students = readTXT("students.txt");
    bool found = false;
    for (int i = 0; i < (int)students.size(); i++) {
        if (students[i][0] == roll) {
            students[i][colIndex] = newValue;
            found = true;
            break;
        }
    }
    if (!found) { cout << "Student not found.\n"; return false; }

    vector<string> header = {"roll_no","name","department","semester","cgpa","status"};
    writeTXT("students.txt", header, students);
    cout << "Student updated.\n";
    return true;
}

bool softDelete(const string& roll) {
    return updateStudent(roll, 5, "inactive");
}

vector<vector<string>> listActiveStudents() {
    vector<vector<string>> students = readTXT("students.txt");
    vector<vector<string>> active;
    for (int i = 0; i < (int)students.size(); i++)
        if (students[i][5] == "active")
            active.push_back(students[i]);

    // Selection sort by roll number (column 0)
    for (int i = 0; i < (int)active.size() - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < (int)active.size(); j++)
            if (active[j][0] < active[minIdx][0])
                minIdx = j;
        if (minIdx != i) swap(active[i], active[minIdx]);
    }
    return active;
}