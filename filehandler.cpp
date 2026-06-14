#include "filehandler.h"

// Splits a CSV line into fields (handles quoted fields with commas inside)
vector<string> splitLine(const string& line) {
    vector<string> fields;
    string field = "";
    bool inQuotes = false;

    for (int i = 0; i < line.length(); i++) {
        char c = line[i];

        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.push_back(field);
            field = "";
        } else {
            field += c;
        }
    }
    fields.push_back(field); // last field
    return fields;
}

vector<vector<string>> readTXT(const string& filename) {
    vector<vector<string>> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "ERROR: Could not open file: " << filename << endl;
        return data;
    }

    string line;
    bool firstLine = true;

    while (getline(file, line)) {
        if (firstLine) {
            firstLine = false; // skip header row
            continue;
        }
        if (line.empty()) continue;

        vector<string> row = splitLine(line);
        data.push_back(row);
    }

    file.close();
    return data;
}

void writeTXT(const string& filename, const vector<string>& header, const vector<vector<string>>& rows) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "ERROR: Could not write to file: " << filename << endl;
        return;
    }

    // Write header
    for (int i = 0; i < header.size(); i++) {
        if (i > 0) file << ",";
        file << header[i];
    }
    file << "\n";

    // Write each row
    for (int i = 0; i < rows.size(); i++) {
        for (int j = 0; j < rows[i].size(); j++) {
            if (j > 0) file << ",";

            // Wrap in quotes if field contains a comma
            if (rows[i][j].find(',') != string::npos) {
                file << '"' << rows[i][j] << '"';
            } else {
                file << rows[i][j];
            }
        }
        file << "\n";
    }

    file.close();
}

void appendTXT(const string& filename, const vector<string>& row) {
    ofstream file(filename, ios::app); // append mode

    if (!file.is_open()) {
        cout << "ERROR: Could not append to file: " << filename << endl;
        return;
    }

    for (int i = 0; i < row.size(); i++) {
        if (i > 0) file << ",";
        if (row[i].find(',') != string::npos) {
            file << '"' << row[i] << '"';
        } else {
            file << row[i];
        }
    }
    file << "\n";

    file.close();
}

vector<string> findRow(const vector<vector<string>>& data, int colIndex, const string& value) {
    for (int i = 0; i < data.size(); i++) {
        if (colIndex < data[i].size() && data[i][colIndex] == value) {
            return data[i];
        }
    }
    return vector<string>(); // empty = not found
}

bool rowExists(const vector<vector<string>>& data, int colIndex, const string& value) {
    for (int i = 0; i < data.size(); i++) {
        if (colIndex < data[i].size() && data[i][colIndex] == value) {
            return true;
        }
    }
    return false;
}