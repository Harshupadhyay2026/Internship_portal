#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <limits>

using namespace std;

// ─────────────────────────────────────────────
//  Data Model
// ─────────────────────────────────────────────
struct Student {
    int    id;
    string name;
    int    age;
    string course;
    float  gpa;
};

// ─────────────────────────────────────────────
//  Constants
// ─────────────────────────────────────────────
const string FILE_NAME = "students.dat";

// ─────────────────────────────────────────────
//  File I/O Helpers
// ─────────────────────────────────────────────
vector<Student> loadAll() {
    vector<Student> students;
    ifstream fin(FILE_NAME);
    if (!fin.is_open()) return students;

    Student s;
    while (fin >> s.id >> s.age >> s.gpa) {
        fin.ignore();
        getline(fin, s.name);
        getline(fin, s.course);
        students.push_back(s);
    }
    fin.close();
    return students;
}

void saveAll(const vector<Student>& students) {
    ofstream fout(FILE_NAME, ios::trunc);
    for (const auto& s : students) {
        fout << s.id   << "\n"
             << s.age  << "\n"
             << s.gpa  << "\n"
             << s.name << "\n"
             << s.course << "\n";
    }
    fout.close();
}

// ─────────────────────────────────────────────
//  Display Helpers
// ─────────────────────────────────────────────
void printHeader() {
    cout << "\n"
         << string(62, '=') << "\n"
         << left
         << setw(6)  << "ID"
         << setw(20) << "Name"
         << setw(6)  << "Age"
         << setw(20) << "Course"
         << setw(6)  << "GPA"
         << "\n"
         << string(62, '-') << "\n";
}

void printStudent(const Student& s) {
    cout << left
         << setw(6)  << s.id
         << setw(20) << s.name
         << setw(6)  << s.age
         << setw(20) << s.course
         << fixed << setprecision(2) << setw(6) << s.gpa
         << "\n";
}

// ─────────────────────────────────────────────
//  Input Helper – clears bad state
// ─────────────────────────────────────────────
int readInt(const string& prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) { cin.ignore(); return val; }
        cout << "  [!] Invalid input. Please enter a whole number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

float readFloat(const string& prompt) {
    float val;
    while (true) {
        cout << prompt;
        if (cin >> val) { cin.ignore(); return val; }
        cout << "  [!] Invalid input. Please enter a decimal number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// ─────────────────────────────────────────────
//  Core Operations
// ─────────────────────────────────────────────

// Returns the next available ID (max existing + 1, or 1 if empty).
int nextId(const vector<Student>& students) {
    if (students.empty()) return 1;
    int maxId = 0;
    for (const auto& s : students) if (s.id > maxId) maxId = s.id;
    return maxId + 1;
}

void addStudent() {
    vector<Student> students = loadAll();
    Student s;
    s.id = nextId(students);

    cout << "\n--- Add New Student ---\n";
    cout << "  Auto-assigned ID: " << s.id << "\n";

    cout << "  Full Name  : "; getline(cin, s.name);
    s.age    = readInt  ("  Age        : ");
    cout << "  Course     : "; getline(cin, s.course);
    s.gpa    = readFloat("  GPA (0-4)  : ");

    if (s.gpa < 0 || s.gpa > 4) {
        cout << "  [!] GPA must be between 0.00 and 4.00. Record not saved.\n";
        return;
    }

    students.push_back(s);
    saveAll(students);
    cout << "  [✓] Student added successfully (ID = " << s.id << ").\n";
}

void displayAll() {
    vector<Student> students = loadAll();
    if (students.empty()) {
        cout << "\n  No student records found.\n";
        return;
    }
    printHeader();
    for (const auto& s : students) printStudent(s);
    cout << string(62, '=') << "\n";
    cout << "  Total records: " << students.size() << "\n";
}

void searchStudent() {
    int id = readInt("\n  Enter Student ID to search: ");
    vector<Student> students = loadAll();

    for (const auto& s : students) {
        if (s.id == id) {
            printHeader();
            printStudent(s);
            cout << string(62, '=') << "\n";
            return;
        }
    }
    cout << "  [!] No student found with ID " << id << ".\n";
}

void updateStudent() {
    int id = readInt("\n  Enter Student ID to update: ");
    vector<Student> students = loadAll();

    for (auto& s : students) {
        if (s.id == id) {
            cout << "  Current record:\n";
            printHeader();
            printStudent(s);
            cout << string(62, '=') << "\n";

            cout << "\n  Enter new details (press Enter to keep current):\n";

            cout << "  Full Name  [" << s.name   << "]: ";
            string tmp; getline(cin, tmp);
            if (!tmp.empty()) s.name = tmp;

            int newAge = readInt("  Age        [" + to_string(s.age) + "]: ");
            if (newAge > 0) s.age = newAge;

            cout << "  Course     [" << s.course << "]: ";
            getline(cin, tmp);
            if (!tmp.empty()) s.course = tmp;

            float newGpa = readFloat("  GPA        [" + to_string(s.gpa) + "]: ");
            if (newGpa >= 0 && newGpa <= 4) s.gpa = newGpa;

            saveAll(students);
            cout << "  [✓] Student ID " << id << " updated successfully.\n";
            return;
        }
    }
    cout << "  [!] No student found with ID " << id << ".\n";
}

void deleteStudent() {
    int id = readInt("\n  Enter Student ID to delete: ");
    vector<Student> students = loadAll();

    auto it = find_if(students.begin(), students.end(),
                      [id](const Student& s){ return s.id == id; });

    if (it == students.end()) {
        cout << "  [!] No student found with ID " << id << ".\n";
        return;
    }

    cout << "  About to delete:\n";
    printHeader();
    printStudent(*it);
    cout << string(62, '=') << "\n";

    cout << "  Confirm delete? (y/n): ";
    char confirm;
    cin >> confirm; cin.ignore();
    if (tolower(confirm) == 'y') {
        students.erase(it);
        saveAll(students);
        cout << "  [✓] Student ID " << id << " deleted.\n";
    } else {
        cout << "  Delete cancelled.\n";
    }
}

// ─────────────────────────────────────────────
//  Menu
// ─────────────────────────────────────────────
void showMenu() {
    cout << "\n"
         << "╔══════════════════════════════════════╗\n"
         << "║     Student Management System        ║\n"
         << "╠══════════════════════════════════════╣\n"
         << "║  1. Add Student                      ║\n"
         << "║  2. Display All Students             ║\n"
         << "║  3. Search Student by ID             ║\n"
         << "║  4. Update Student Record            ║\n"
         << "║  5. Delete Student Record            ║\n"
         << "║  0. Exit                             ║\n"
         << "╚══════════════════════════════════════╝\n";
}

// ─────────────────────────────────────────────
//  Main
// ─────────────────────────────────────────────
int main() {
    int choice;
    do {
        showMenu();
        choice = readInt("  Select option: ");
        switch (choice) {
            case 1: addStudent();    break;
            case 2: displayAll();    break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 0: cout << "\n  Goodbye!\n\n"; break;
            default: cout << "  [!] Invalid option. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
