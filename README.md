# Campus Analytics Engine

A multi-file, menu-driven C++ data analytics system for university campus management.  
Built for BS AI — Programming Fundamentals Semester Project.

## Features
- Student Management (add, search, update, soft-delete)
- Course Enrollment with prerequisite and credit hour validation
- Attendance tracking with P/A/L marking and undo support
- Grade computation with weighted totals, GPA, and attendance penalty
- Fee tracking with late fine calculation and receipt generation
- Reports: merit list, defaulters, semester results, department summary, export to file

## File Structure
| File | Responsibility |
|------|---------------|
| main.cpp | Entry point, 3-level menu |
| filehandler.cpp/h | All file read/write operations |
| student_ops.cpp/h | Student CRUD |
| course_ops.cpp/h | Enrollment, prerequisite checks |
| attendance.cpp/h | Attendance marking and reports |
| grades.cpp/h | Marks, GPA, class statistics |
| fee_tracker.cpp/h | Payments, late fines, receipts |
| reports.cpp/h | Formatted reports and export |

## Compile & Run
```bash
g++ main.cpp filehandler.cpp student_ops.cpp course_ops.cpp attendance.cpp grades.cpp fee_tracker.cpp reports.cpp -o campus.exe
./campus.exe
```

## Data Files
`students.txt` `courses.txt` `enrollments.txt` `attendance_log.txt` `fees.txt`