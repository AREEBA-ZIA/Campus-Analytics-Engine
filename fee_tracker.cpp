#include "fee_tracker.h"

// Month lengths (index 1-12); index 0 unused
static int monthDays[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};

static bool isLeapYear(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

// Convert DD-MM-YYYY string to total days from year 0 (approx, for difference only)
static int dateToDays(const string& date) {
    // date format: DD-MM-YYYY (length 10)
    if (date.length() != 10) return 0;
    if (date == "00-00-0000") return 0;

    int dd = (date[0]-'0')*10 + (date[1]-'0');
    int mm = (date[3]-'0')*10 + (date[4]-'0');
    int yyyy = (date[6]-'0')*1000 + (date[7]-'0')*100 +
               (date[8]-'0')*10   + (date[9]-'0');

    // Total days = years*365 + leap days + month days + day
    int total = yyyy * 365;
    // Add leap years: count years divisible by 4 minus exceptions
    total += (yyyy - 1) / 4 - (yyyy - 1) / 100 + (yyyy - 1) / 400;

    for (int m = 1; m < mm; m++) {
        total += monthDays[m];
        if (m == 2 && isLeapYear(yyyy)) total += 1;
    }
    total += dd;
    return total;
}

// Returns date2 - date1 in days (positive if date2 is later)
int daysBetween(const string& date1, const string& date2) {
    return dateToDays(date2) - dateToDays(date1);
}

// Validate DD-MM-YYYY format and logical values
static bool isValidDate(const string& date) {
    if (date.length() != 10) return false;
    if (date[2] != '-' || date[5] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (date[i] < '0' || date[i] > '9') return false;
    }
    int dd   = (date[0]-'0')*10 + (date[1]-'0');
    int mm   = (date[3]-'0')*10 + (date[4]-'0');
    int yyyy = (date[6]-'0')*1000 + (date[7]-'0')*100 +
               (date[8]-'0')*10   + (date[9]-'0');
    if (mm < 1 || mm > 12) return false;
    int maxD = monthDays[mm];
    if (mm == 2 && isLeapYear(yyyy)) maxD = 29;
    if (dd < 1 || dd > maxD) return false;
    return true;
}

// Parse double from string (no stod)
static double parseDouble(const string& s) {
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

bool recordPayment(const string& roll, int semester, double amount,
                   const string& payDate, const string& method) {
    if (!isValidDate(payDate)) {
        cout << "ERROR: Invalid date format. Use DD-MM-YYYY.\n";
        return false;
    }
    if (amount <= 0) {
        cout << "ERROR: Amount must be positive.\n";
        return false;
    }

    vector<vector<string>> fees = readTXT("fees.txt");
    bool found = false;

    for (int i = 0; i < (int)fees.size(); i++) {
        if (fees[i][1] == roll && fees[i][2] == to_string(semester)) {
            double totalFee  = parseDouble(fees[i][3]);
            double alreadyPaid = parseDouble(fees[i][4]);
            double newPaid   = alreadyPaid + amount;
            if (newPaid > totalFee) newPaid = totalFee; // cap at total

            // Format newPaid as string (no decimal if whole number)
            int np = (int)(newPaid * 100);
            string paidStr = to_string(np / 100);
            if (np % 100 != 0) {
                paidStr += ".";
                if (np % 100 < 10) paidStr += "0";
                paidStr += to_string(np % 100);
            }

            fees[i][4] = paidStr;
            fees[i][6] = payDate;
            fees[i][7] = method;

            // Update status
            if (newPaid >= totalFee) {
                // Check if late
                int diff = daysBetween(fees[i][5], payDate);
                fees[i][8] = (diff > 0) ? "paid_late" : "paid";
            } else {
                fees[i][8] = "partial";
            }
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "ERROR: Fee record not found for " << roll << " semester " << semester << ".\n";
        return false;
    }

    vector<string> header = {"fee_id","roll_no","semester","total_fee",
                             "amount_paid","due_date","payment_date","payment_method","status"};
    writeTXT("fees.txt", header, fees);
    cout << "Payment of " << (int)amount << " recorded for " << roll << ".\n";
    return true;
}

double computeLateFine(const string& roll, int semester) {
    vector<vector<string>> fees = readTXT("fees.txt");
    for (int i = 0; i < (int)fees.size(); i++) {
        if (fees[i][1] == roll && fees[i][2] == to_string(semester)) {
            string dueDate  = fees[i][5];
            string payDate  = fees[i][6];
            double totalFee = parseDouble(fees[i][3]);
            double paid     = parseDouble(fees[i][4]);

            if (paid >= totalFee) return 0.0;             // fully paid
            if (payDate == "00-00-0000") return 0.0;      // not yet paid — fine pending

            int diff = daysBetween(dueDate, payDate);
            if (diff <= 0) return 0.0;                    // paid on time

            int completeWeeks = diff / 7;
            double fine = totalFee * 0.02 * completeWeeks;
            return fine;
        }
    }
    return 0.0;
}

void generateReceipt(const string& roll, int semester) {
    vector<vector<string>> fees = readTXT("fees.txt");
    vector<string> feeRow;
    for (int i = 0; i < (int)fees.size(); i++) {
        if (fees[i][1] == roll && fees[i][2] == to_string(semester)) {
            feeRow = fees[i];
            break;
        }
    }
    if (feeRow.empty()) {
        cout << "No fee record found.\n";
        return;
    }

    vector<string> student = searchByRoll(roll);
    string name = student.empty() ? roll : student[1];

    double totalFee = parseDouble(feeRow[3]);
    double paid     = parseDouble(feeRow[4]);
    double fine     = computeLateFine(roll, semester);
    double balance  = (totalFee + fine) - paid;
    if (balance < 0) balance = 0;

    cout << "\n" << string(45, '=') << "\n";
    cout << "         FEE RECEIPT\n";
    cout << string(45, '=') << "\n";
    cout << left << setw(20) << "Student:" << name << "\n";
    cout << setw(20) << "Roll No:" << roll << "\n";
    cout << setw(20) << "Semester:" << semester << "\n";
    cout << setw(20) << "Due Date:" << feeRow[5] << "\n";
    cout << setw(20) << "Payment Date:" << feeRow[6] << "\n";
    cout << setw(20) << "Method:" << feeRow[7] << "\n";
    cout << string(45, '-') << "\n";
    cout << setw(20) << "Tuition Fee:" << right << setw(10) << (int)totalFee << "\n";
    if (fine > 0.0)
        cout << left << setw(20) << "Late Fine:" << right << setw(10) << (int)fine << "\n";
    cout << left << setw(20) << "Amount Paid:" << right << setw(10) << (int)paid << "\n";
    cout << string(45, '-') << "\n";
    cout << left << setw(20) << "Balance Due:" << right << setw(10) << (int)balance << "\n";
    cout << left << setw(20) << "Status:" << feeRow[8] << "\n";
    cout << string(45, '=') << "\n";
}

vector<vector<string>> getDefaulters() {
    vector<vector<string>> fees = readTXT("fees.txt");
    vector<vector<string>> defaulters;

    for (int i = 0; i < (int)fees.size(); i++) {
        double totalFee = parseDouble(fees[i][3]);
        double paid     = parseDouble(fees[i][4]);
        if (paid < totalFee) {
            defaulters.push_back(fees[i]);
        }
    }

    // Bubble sort by outstanding amount (descending)
    for (int i = 0; i < (int)defaulters.size() - 1; i++) {
        for (int j = 0; j < (int)defaulters.size() - 1 - i; j++) {
            double bal1 = parseDouble(defaulters[j][3]) - parseDouble(defaulters[j][4]);
            double bal2 = parseDouble(defaulters[j+1][3]) - parseDouble(defaulters[j+1][4]);
            if (bal2 > bal1) {
                vector<string> tmp = defaulters[j];
                defaulters[j] = defaulters[j+1];
                defaulters[j+1] = tmp;
            }
        }
    }
    return defaulters;
}