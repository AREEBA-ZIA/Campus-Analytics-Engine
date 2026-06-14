#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Reads entire file into a 2D vector (each row = vector of fields)
vector<vector<string>> readTXT(const string& filename);

// Writes all rows back to file (overwrites)
void writeTXT(const string& filename, const vector<string>& header, const vector<vector<string>>& rows);

// Appends one row to end of file
void appendTXT(const string& filename, const vector<string>& row);

// Finds first row where column[colIndex] == value
vector<string> findRow(const vector<vector<string>>& data, int colIndex, const string& value);

// Returns true if any row has value at colIndex
bool rowExists(const vector<vector<string>>& data, int colIndex, const string& value);

#endif