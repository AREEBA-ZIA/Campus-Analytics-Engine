#include "attendance.h"

// Backup vector for undo
vector<vector<string>> attendanceBackup;

void markAttendance(const string &courseCode, const string &date)
{
    vector<vector<string>> enrolled = listEnrolledStudents(courseCode);
    if (enrolled.empty())
    {
        cout << "No students enrolled in " << courseCode << "\n";
        return;
    }

    // Save backup before marking
    attendanceBackup = readTXT("attendance_log.txt");

    cout << "\nMarking attendance for " << courseCode << " on " << date << "\n";
    cout << "Enter P (Present), A (Absent), L (Late) for each student:\n\n";

    // Get next log ID
    vector<vector<string>> existingLogs = readTXT("attendance_log.txt");
    int nextId = (int)existingLogs.size() + 1;

    for (int i = 0; i < (int)enrolled.size(); i++)
    {
        string roll = enrolled[i][1];
        vector<string> student = searchByRoll(roll);
        string name = student.empty() ? roll : student[1];

        string status;
        bool valid = false;
        while (!valid)
        {
            cout << roll << " - " << name << ": ";
            cin >> status;
            if (status == "P" || status == "A" || status == "L" ||
                status == "p" || status == "a" || status == "l")
            {
                if (status == "p")
                    status = "P";
                if (status == "a")
                    status = "A";
                if (status == "l")
                    status = "L";
                valid = true;
            }
            else
            {
                cout << "Invalid! Enter P, A, or L only.\n";
            }
        }

        // Build log_id like L00001
        string logId = "L";
        string numStr = to_string(nextId++);
        while ((int)numStr.length() < 5)
            numStr = "0" + numStr;
        logId += numStr;

        vector<string> newRow;
        newRow.push_back(logId);
        newRow.push_back(roll);
        newRow.push_back(courseCode);
        newRow.push_back(date);
        newRow.push_back(status);
        appendTXT("attendance_log.txt", newRow);
    }
    cout << "Attendance saved!\n";
}

double getAttendancePct(const string &roll, const string &courseCode)
{
    vector<vector<string>> attendance = readTXT("attendance_log.txt");

    int total = 0;
    double score = 0.0;

    for (int i = 0; i < (int)attendance.size(); i++)
    {
        if ((int)attendance[i].size() < 5)
            continue;
        // cols: [0]=log_id [1]=roll_no [2]=course_code [3]=session_date [4]=status
        if (attendance[i][1] == roll && attendance[i][2] == courseCode)
        {
            total++;
            if (attendance[i][4] == "P")
                score += 1.0;
            else if (attendance[i][4] == "L")
                score += 0.5;
        }
    }

    if (total == 0)
        return 0.0;
    return (score / total) * 100.0;
}

vector<vector<string>> getShortageList(const string &courseCode)
{
    vector<vector<string>> enrolled = listEnrolledStudents(courseCode);
    vector<vector<string>> shortage;

    for (int i = 0; i < (int)enrolled.size(); i++)
    {
        string roll = enrolled[i][1];
        double pct = getAttendancePct(roll, courseCode);
        if (pct < 75.0)
        {
            vector<string> row;
            row.push_back(roll);
            row.push_back(courseCode);
            int pctInt = (int)pct;
            row.push_back(to_string(pctInt) + "%");
            shortage.push_back(row);
        }
    }
    return shortage;
}

bool undoLastSession(const string &courseCode)
{
    if (attendanceBackup.empty())
    {
        cout << "No backup available to undo.\n";
        return false;
    }

    vector<string> hdr;
    hdr.push_back("log_id");
    hdr.push_back("roll_no");
    hdr.push_back("course_code");
    hdr.push_back("session_date");
    hdr.push_back("status");
    writeTXT("attendance_log.txt", hdr, attendanceBackup);
    cout << "Last session undone successfully.\n";
    return true;
}

void printDailySheet(const string &courseCode, const string &date)
{
    vector<vector<string>> attendance = readTXT("attendance_log.txt");
    vector<vector<string>> enrolled = listEnrolledStudents(courseCode);

    cout << "\n--- Attendance Sheet: " << courseCode << " | " << date << " ---\n";
    cout << left;
    cout.width(15);
    cout << "Roll";
    cout.width(22);
    cout << "Name";
    cout.width(10);
    cout << "Status";
    cout << "\n"
         << string(47, '-') << "\n";

    for (int i = 0; i < (int)enrolled.size(); i++)
    {
        string roll = enrolled[i][1];
        string status = "---";

        for (int j = 0; j < (int)attendance.size(); j++)
        {
            if ((int)attendance[j].size() < 5)
                continue;
            // [1]=roll [2]=course [3]=date [4]=status
            if (attendance[j][1] == roll &&
                attendance[j][2] == courseCode &&
                attendance[j][3] == date)
            {
                status = attendance[j][4];
                break;
            }
        }

        vector<string> student = searchByRoll(roll);
        string name = student.empty() ? roll : student[1];

        cout.width(15);
        cout << roll;
        cout.width(22);
        cout << name;
        cout.width(10);
        cout << status;
        cout << "\n";
    }
}