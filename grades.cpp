#include "grades.h"

double bestThreeOfFive(double marks[], int n) {
    if (n <= 0) return 0.0;
    if (n <= 3) {
        double sum = 0;
        for (int i = 0; i < n; i++) sum += marks[i];
        return sum / n;
    }
    // Find and exclude two lowest
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (marks[j] < marks[i]) {
                double temp = marks[i];
                marks[i] = marks[j];
                marks[j] = temp;
            }
        }
    }
    // marks[0] and marks[1] are lowest, take marks[2..4]
    double sum = 0;
    for (int i = 2; i < n; i++) sum += marks[i];
    return sum / 3.0;
}

double computeWeightedTotal(double quiz, double asgn, double mid, double final_) {
    return quiz * 0.10 + asgn * 0.10 + mid * 0.30 + final_ * 0.50;
}

string getLetterGrade(double total) {
    if (total >= 85) return "A";
    if (total >= 80) return "B+";
    if (total >= 70) return "B";
    if (total >= 65) return "C+";
    if (total >= 60) return "C";
    if (total >= 50) return "D";
    return "F";
}

void enterMarks(const string& roll, const string& courseCode) {
    // Check student exists
    vector<string> student = searchByRoll(roll);
    if (student.empty()) { cout << "Student not found.\n"; return; }

    // Quiz marks (up to 5)
    int numQuizzes;
    cout << "How many quizzes? (1-5): ";
    cin >> numQuizzes;
    if (numQuizzes < 1 || numQuizzes > 5) { cout << "Invalid.\n"; return; }

    double quizMarks[5] = {0};
    for (int i = 0; i < numQuizzes; i++) {
        cout << "Quiz " << i+1 << " marks (0-10): ";
        cin >> quizMarks[i];
        if (quizMarks[i] < 0 || quizMarks[i] > 10) {
            cout << "Invalid marks.\n"; return;
        }
    }
    double quizAvg = bestThreeOfFive(quizMarks, numQuizzes);
    double quizOut100 = quizAvg * 10; // scale to 100

    // Assignment marks (up to 5)
    int numAsgn;
    cout << "How many assignments? (1-5): ";
    cin >> numAsgn;
    if (numAsgn < 1 || numAsgn > 5) { cout << "Invalid.\n"; return; }

    double asgnTotal = 0;
    for (int i = 0; i < numAsgn; i++) {
        double a;
        cout << "Assignment " << i+1 << " marks (0-10): ";
        cin >> a;
        if (a < 0 || a > 10) { cout << "Invalid marks.\n"; return; }
        asgnTotal += a;
    }
    double asgnAvg = (asgnTotal / numAsgn) * 10; // scale to 100

    // Mid and Final
    double mid, final_;
    cout << "Mid exam marks (0-40): "; cin >> mid;
    if (mid < 0 || mid > 40) { cout << "Invalid.\n"; return; }

    cout << "Final exam marks (0-60): "; cin >> final_;
    if (final_ < 0 || final_ > 60) { cout << "Invalid.\n"; return; }

    double midOut100  = (mid / 40.0) * 100;
    double finalOut100 = (final_ / 60.0) * 100;

    double total = computeWeightedTotal(quizOut100, asgnAvg, midOut100, finalOut100);
    string grade = getLetterGrade(total);

    // Apply attendance penalty
    double attPct = getAttendancePct(roll, courseCode);
    if (attPct < 75.0) {
        cout << "WARNING: Attendance below 75% — grade overridden to F.\n";
        grade = "F";
    }

    // GPA points
    double gpaPoints = 0.0;
    if (grade == "A")       gpaPoints = 4.0;
    else if (grade == "B+") gpaPoints = 3.5;
    else if (grade == "B")  gpaPoints = 3.0;
    else if (grade == "C+") gpaPoints = 2.5;
    else if (grade == "C")  gpaPoints = 2.0;
    else if (grade == "D")  gpaPoints = 1.0;
    else                    gpaPoints = 0.0;

    // Format total to 2 decimal places
    int totalInt = (int)(total * 100);
    string totalStr = to_string(totalInt / 100) + "." +
                      (totalInt % 100 < 10 ? "0" : "") +
                      to_string(totalInt % 100);

    int gpaInt = (int)(gpaPoints * 100);
    string gpaStr = to_string(gpaInt / 100) + "." +
                    (gpaInt % 100 < 10 ? "0" : "") +
                    to_string(gpaInt % 100);

    // Save to grades.txt
    // Format: roll,course,quiz,asgn,mid,final,total,grade,gpa
    vector<string> newRow;
    newRow.push_back(roll);
    newRow.push_back(courseCode);
    newRow.push_back(totalStr);  // quiz avg
    newRow.push_back(totalStr);  // asgn avg
    newRow.push_back(to_string((int)mid));
    newRow.push_back(to_string((int)final_));
    newRow.push_back(totalStr);
    newRow.push_back(grade);
    newRow.push_back(gpaStr);

    // Check if grades.txt exists, if not create with header
    vector<vector<string>> existing = readTXT("grades.txt");
    if (existing.empty()) {
        // Create file with header first
        ofstream f("grades.txt");
        f << "roll_no,course_code,quiz_avg,asgn_avg,mid,final,total,grade,gpa_points\n";
        f.close();
    }
    appendTXT("grades.txt", newRow);

    cout << "\n--- Result ---\n";
    cout << "Total: " << totalStr << "%\n";
    cout << "Grade: " << grade << "\n";
    cout << "GPA Points: " << gpaStr << "\n";
}

double computeGPA(const string& roll, int semester) {
    vector<vector<string>> grades = readTXT("grades.txt");
    vector<vector<string>> courses = readTXT("courses.txt");
    vector<vector<string>> enrollments = readTXT("enrollments.txt");

    double totalPoints = 0.0;
    int totalCredits = 0;

    for (int i = 0; i < (int)grades.size(); i++) {
        if (grades[i][0] != roll) continue;

        string courseCode = grades[i][1];

        // Check enrollment in this semester
        bool inSemester = false;
        for (int j = 0; j < (int)enrollments.size(); j++) {
            if (enrollments[j][1] == roll &&
                enrollments[j][2] == courseCode &&
                enrollments[j][3] == to_string(semester)) {
                inSemester = true;
                break;
            }
        }
        if (!inSemester) continue;

        // Get credit hours
        vector<string> course = findRow(courses, 0, courseCode);
        if (course.empty()) continue;

        int credits = 0;
        for (int j = 0; j < (int)course[2].length(); j++)
            credits = credits * 10 + (course[2][j] - '0');

        // Parse GPA points
        double gpa = 0.0;
        string gpaStr = grades[i][8];
        int dotPos = -1;
        for (int j = 0; j < (int)gpaStr.length(); j++)
            if (gpaStr[j] == '.') { dotPos = j; break; }

        if (dotPos == -1) {
            for (int j = 0; j < (int)gpaStr.length(); j++)
                gpa = gpa * 10 + (gpaStr[j] - '0');
        } else {
            double intPart = 0, fracPart = 0, divisor = 1;
            for (int j = 0; j < dotPos; j++)
                intPart = intPart * 10 + (gpaStr[j] - '0');
            for (int j = dotPos + 1; j < (int)gpaStr.length(); j++) {
                fracPart = fracPart * 10 + (gpaStr[j] - '0');
                divisor *= 10;
            }
            gpa = intPart + fracPart / divisor;
        }

        totalPoints += gpa * credits;
        totalCredits += credits;
    }

    if (totalCredits == 0) return 0.0;
    return totalPoints / totalCredits;
}

Stats computeClassStats(const string& courseCode) {
    vector<vector<string>> grades = readTXT("grades.txt");
    vector<double> totals;

    for (int i = 0; i < (int)grades.size(); i++) {
        if (grades[i][1] != courseCode) continue;
        // Parse total
        double val = 0.0;
        string s = grades[i][6];
        int dotPos = -1;
        for (int j = 0; j < (int)s.length(); j++)
            if (s[j] == '.') { dotPos = j; break; }
        if (dotPos == -1) {
            for (int j = 0; j < (int)s.length(); j++)
                val = val * 10 + (s[j] - '0');
        } else {
            double ip = 0, fp = 0, div = 1;
            for (int j = 0; j < dotPos; j++) ip = ip * 10 + (s[j] - '0');
            for (int j = dotPos+1; j < (int)s.length(); j++) { fp = fp*10+(s[j]-'0'); div*=10; }
            val = ip + fp / div;
        }
        totals.push_back(val);
    }

    Stats st = {0, 0, 0, 0};
    if (totals.empty()) return st;

    st.highest = totals[0];
    st.lowest  = totals[0];
    double sum = 0;

    for (int i = 0; i < (int)totals.size(); i++) {
        if (totals[i] > st.highest) st.highest = totals[i];
        if (totals[i] < st.lowest)  st.lowest  = totals[i];
        sum += totals[i];
    }
    st.mean = sum / totals.size();

    // Sort for median (bubble sort)
    for (int i = 0; i < (int)totals.size()-1; i++)
        for (int j = 0; j < (int)totals.size()-1-i; j++)
            if (totals[j] > totals[j+1]) {
                double tmp = totals[j]; totals[j] = totals[j+1]; totals[j+1] = tmp;
            }

    int n = totals.size();
    if (n % 2 == 0) st.median = (totals[n/2-1] + totals[n/2]) / 2.0;
    else            st.median = totals[n/2];

    return st;
}

void applyAttendancePenalty(const string& roll, const string& courseCode) {
    double pct = getAttendancePct(roll, courseCode);
    if (pct >= 75.0) { cout << "Attendance OK — no penalty.\n"; return; }

    vector<vector<string>> grades = readTXT("grades.txt");
    bool found = false;
    for (int i = 0; i < (int)grades.size(); i++) {
        if (grades[i][0] == roll && grades[i][1] == courseCode) {
            grades[i][7] = "F";
            grades[i][8] = "0.00";
            found = true;
            break;
        }
    }
    if (!found) { cout << "No grade record found.\n"; return; }

    vector<string> header = {"roll_no","course_code","quiz_avg","asgn_avg",
                             "mid","final","total","grade","gpa_points"};
    writeTXT("grades.txt", header, grades);
    cout << "Attendance penalty applied — grade set to F.\n";
}