#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

class Student {
private:
    int rollNumber;
    string name;
    string course;
    float marks;

public:
    // Function to take user input
    void inputStudent() {
        cout << "Enter Roll Number: ";
        while (!(cin >> rollNumber)) {
            cout << "Invalid input. Enter numeric Roll Number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(); // Clear buffer
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Course: ";
        getline(cin, course);
        cout << "Enter Marks: ";
        while (!(cin >> marks) || marks < 0 || marks > 100) {
            cout << "Invalid marks. Enter between 0 and 100: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    // Function to print details in table row format
    void displayRow() const {
        cout << left << setw(12) << rollNumber 
             << setw(20) << name 
             << setw(15) << course 
             << setw(8) << fixed << setprecision(2) << marks << endl;
    }

    // Getters
    int getRollNumber() const { return rollNumber; }
    string getName() const { return name; }
    string getCourse() const { return course; }
    float getMarks() const { return marks; }

    // Setters for modification
    void setName(string n) { name = n; }
    void setCourse(string c) { course = c; }
    void setMarks(float m) { marks = m; }
};

// --- FILE OPERATIONS ---

// 1. Add a new record
void addStudent() {
    ofstream outFile("students.txt", ios::app);
    if (!outFile) {
        cout << "Error opening file!\n";
        return;
    }
    
    Student s;
    s.inputStudent();
    
    // Writing data in a pipe-separated flat-file format
    outFile << s.getRollNumber() << "|" 
            << s.getName() << "|" 
            << s.getCourse() << "|" 
            << s.getMarks() << "\n";
            
    outFile.close();
    cout << "\nRecord added successfully!\n";
}

// 2. Display all records
void displayAllStudents() {
    ifstream inFile("students.txt");
    if (!inFile) {
        cout << "\nNo records found. File is empty!\n";
        return;
    }

    string line;
    cout << "\n==================== STUDENT RECORDS ====================\n";
    cout << left << setw(12) << "Roll No" << setw(20) << "Name" << setw(15) << "Course" << setw(8) << "Marks" << endl;
    cout << "---------------------------------------------------------\n";

    while (getline(inFile, line)) {
        if (line.empty()) continue;
        
        // Parsing pipe-separated values
        size_t pos = 0;
        string token;
        string data[4];
        int i = 0;
        
        while ((pos = line.find('|')) != string::npos && i < 3) {
            data[i++] = line.substr(0, pos);
            line.erase(0, pos + 1);
        }
        data[i] = line; // Last item (marks)

        cout << left << setw(12) << data[0] 
             << setw(20) << data[1] 
             << setw(15) << data[2] 
             << setw(8) << data[3] << endl;
    }
    cout << "=========================================================\n";
    inFile.close();
}

// 3. Update or Delete records
void modifyStudent(bool isDelete) {
    ifstream inFile("students.txt");
    if (!inFile) {
        cout << "\nNo records found!\n";
        return;
    }

    int searchRoll;
    cout << "Enter Roll Number to " << (isDelete ? "delete" : "update") << ": ";
    cin >> searchRoll;

    ofstream tempFile("temp.txt");
    string line;
    bool found = false;

    while (getline(inFile, line)) {
        if (line.empty()) continue;

        // Peak at the Roll Number
        size_t pos = line.find('|');
        int roll = stoi(line.substr(0, pos));

        if (roll == searchRoll) {
            found = true;
            if (isDelete) {
                // Skip writing this line to temp file to delete it
                continue; 
            } else {
                // Get new data for update
                cout << "\n--- Enter New Details ---\n";
                Student s;
                s.inputStudent();
                tempFile << s.getRollNumber() << "|" << s.getName() << "|" << s.getCourse() << "|" << s.getMarks() << "\n";
                continue;
            }
        }
        tempFile << line << "\n";
    }

    inFile.close();
    tempFile.close();

    remove("students.txt");
    rename("temp.txt", "students.txt");

    if (found) {
        cout << "\nRecord " << (isDelete ? "deleted" : "updated") << " successfully!\n";
    } else {
        cout << "\nRecord with Roll Number " << searchRoll << " not found!\n";
    }
}

int main() {
    int choice;
    do {
        cout << "\n*** Student Management System ***\n";
        cout << "1. Add Student Record\n";
        cout << "2. Display All Records\n";
        cout << "3. Update Student Record\n";
        cout << "4. Delete Student Record\n";
        cout << "5. Exit\n";
        cout << "Enter your choice (1-5): ";
        
        while (!(cin >> choice)) {
            cout << "Invalid choice. Enter a number between 1 and 5: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAllStudents(); break;
            case 3: modifyStudent(false); break; // update
            case 4: modifyStudent(true);  break; // delete
            case 5: cout << "\nExiting program. Goodbye!\n"; break;
            default: cout << "\nInvalid choice! Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}