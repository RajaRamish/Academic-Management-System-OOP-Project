// Acadmey Managment System comprises of 1 Main class Acadmey that runs All functions and classes 
// 1 Admin class that can add and remove students and teachers and also manges fees and salary
// 1 teacher class that can compile results of students and see his salary status
// 1 student class that can view results and their fees status (paid or not)
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// Main Academy class that runs the system
class Academy
{
public:
    void run();  // Main function to run the management system
};

// Student class definition
class Student
{
    friend class Teacher; // Teacher can access private members
    friend class Admin;   // Admin can access private members
private:
    bool feeStatus; // true if fee is paid, false otherwise
    string subjects[5] = {"Maths", "Physics", "Chemistry", "English", ""}; // Last subject is chosen by student
    int marks[5];   // Marks for each subject
public:
    string username; // Student's username
    string password; // Student's password

    // Constructor initializes feeStatus and marks
    Student() : feeStatus(false)
    {
        for (int i = 0; i < 5; ++i)
        {
            marks[i] = 0;
        }
    }

    // Returns fee status (paid/unpaid)
    bool getFeeStatus() const
    {
        return feeStatus;
    }

    // Sets fee status
    void setFeeStatus(bool status) {
        feeStatus = status;
    }

    // Sets marks for a specific subject
    void setMarks(int subjectIndex, int newMarks)
    {
        if (subjectIndex >= 0 && subjectIndex < 5)
        {
            marks[subjectIndex] = newMarks;
        }
    }

    // Gets marks for a specific subject
    int getMarks(int subjectIndex) const
    {
        if (subjectIndex >= 0 && subjectIndex < 5) {
            return marks[subjectIndex];
        }
        return 0;
    }

    // Sets the student's choice for the 5th subject
    void setSubjectChoice(const string& choice)
    {
        subjects[4] = choice;
    }

    // Displays the student's marks for all subjects
    void viewResults() const
    {
        cout << "Your marks:\n";
        for (int i = 0; i < 5; ++i) {
            cout << subjects[i] << ": " << marks[i] << endl;
        }
    }

    // Displays the student's fee status
    void viewFeeStatus() const
    {
        cout << "Fee Status: " << (feeStatus ? "Paid" : "Unpaid") << endl;
    }

    // Overloaded input operator for Student (for file I/O)
    friend istream& operator>>(istream& is, Student& student) {
        is >> student.username >> student.password >> student.feeStatus;
        for (int i = 0; i < 5; ++i) {
            is >> student.subjects[i] >> student.marks[i];
        }
        return is;
    }

    // Overloaded output operator for Student (for file I/O)
    friend ostream& operator<<(ostream& os, const Student& student) {
        os << student.username << " " << student.password << " " << student.feeStatus;
        for (int i = 0; i < 5; ++i) {
            os << " " << student.subjects[i] << " " << student.marks[i];
        }
        return os;
    }
};

// Teacher class definition
class Teacher
{
private:
    bool salaryStatus; // true if salary is ready, false otherwise
    string subjects[2] = {"", ""}; // Subjects the teacher can teach (up to 2)
public:
    string username; // Teacher's username
    string password; // Teacher's password

    // Constructor initializes salaryStatus
    Teacher() : salaryStatus(false) {}

    // Returns salary status
    bool getSalaryStatus() const
    {
        return salaryStatus;
    }

    // Sets salary status
    void setSalaryStatus(bool status) {
        salaryStatus = status;
    }

    // Assigns subjects to the teacher
    void setSubjects(const string& subject1=" ", const string& subject2 = " ") {
        subjects[0] = subject1;
        subjects[1] = subject2;
    }

    // Checks if teacher can teach a given subject
    bool canTeachSubject(const string& subject) const
    {
        return subjects[0] == subject || subjects[1] == subject;
    }

    // Allows teacher to change marks for a student in a subject they teach
    void changeMarks(Student& student, const string& subject, int newMarks)
    {
        int count = 0;
        for (int i = 0; i < 5; ++i)
        {
            if (student.subjects[i] == subject)
            {
                student.setMarks(i, newMarks);
                count++;
                break;
            }
        }
        if (count == 0)
            cout << "student Not found ";
    }

    // Displays results for all students in subjects taught by this teacher
    void compileResults(Student students[], int studentCount) const {
        for (int i = 0; i < studentCount; ++i) {
            cout << students[i].username << ": ";
            for (int j = 0; j < 5; ++j) {
                if (canTeachSubject(students[i].subjects[j])) {
                    cout << students[i].subjects[j] << ": " << students[i].marks[j] << " ";
                }
            }
            cout << endl;
        }
    }

    // Displays salary status
    void SalaryStatus() const
    {
        cout << "Your Salary is: " << (salaryStatus ? "Ready" : "Not Ready") << endl;
    }

    // Overloaded input operator for Teacher (for file I/O)
    friend istream& operator>>(istream& is, Teacher& teacher) {
        is >> teacher.username >> teacher.password >> teacher.subjects[0] >> teacher.subjects[1];
        return is;
    }

    // Overloaded output operator for Teacher (for file I/O)
    friend ostream& operator<<(ostream& os, const Teacher& teacher)
    {
        os << teacher.username << " " << teacher.password << " " << teacher.subjects[0] << " " << teacher.subjects[1];
        return os;
    }
};

// Admin class definition
class Admin
{
    const string adminUsername = "admin";     // Default admin username
    const string adminPassword = "admin123";  // Default admin password
public:
    // Authenticates admin credentials
    bool authenticate(const string &uname, const string &pass)
    {
        return uname == adminUsername && pass == adminPassword;
    }

    // Adds a new student to the system
    void addStudent(Student students[], int &studentCount, const string &uname, const string &pass)
    {
        students[studentCount].username = uname;
        students[studentCount].password = pass;
        cout << "Choose an additional subject (Computer/Biology): ";
        cin.ignore(); // <-- Add this line
        string subjectChoice;
        getline(cin, subjectChoice);

        // Only allow Computer or Biology as the 5th subject
        if (subjectChoice == "Computer" || subjectChoice == "Biology")
        {
            students[studentCount].setSubjectChoice(subjectChoice);
        }
        else
        {
            cout << "Invalid choice, defaulting to Computer.\n";
            students[studentCount].setSubjectChoice("Computer");
        }
        studentCount++;
        cout << "Student added successfully!" << endl;
    }

    // Removes a student by username
    void removeStudent(Student students[], int &studentCount, const string &uname) {
        bool found = false;
        for (int i = 0; i < studentCount; ++i) {
            if (students[i].username == uname) {
                for (int j = i; j < studentCount - 1; ++j) {
                    students[j] = students[j + 1];
                }
                studentCount--;
                found = true;
                cout << "Student " << uname << " removed successfully!" << endl;
                break;
            }
        }
        if (!found) {
            cout << "Student " << uname << " not found!" << endl;
        }
    }

    // Adds a new teacher to the system
    void addTeacher(Teacher teachers[], int &teacherCount, const string &uname, const string &pass, const string &subject1, const string &subject2 = "") {
        teachers[teacherCount].username = uname;
        teachers[teacherCount].password = pass;
        teachers[teacherCount].setSubjects(subject1, subject2);
        teacherCount++;
        cout << "Teacher added successfully!" << endl;
    }

    // Removes a teacher by username
    void removeTeacher(Teacher teachers[], int &teacherCount, const string &uname) {
        bool found = false;
        for (int i = 0; i < teacherCount; ++i) {
            if (teachers[i].username == uname) {
                for (int j = i; j < teacherCount - 1; ++j) {
                    teachers[j] = teachers[j + 1];
                }
                teacherCount--;
                found = true;
                cout << "Teacher " << uname << " removed successfully!" << endl;
                break;
            }
        }
        if (!found) {
            cout << "Teacher " << uname << " not found!" << endl;
        }
    }

    // Changes a student's fee status (paid/unpaid)
    void manageFees(Student &student, bool status)
    {
        student.setFeeStatus(status);
    }

    // Changes a teacher's salary status (ready/unready)
    void calculateSalary(Teacher &teacher, bool status) {
        teacher.setSalaryStatus(status);
    }
};

// Main function for running the Academy Management System
void Academy::run()
{
    const int MAX_STUDENTS = 100;  // Maximum number of students
    const int MAX_TEACHERS = 10;   // Maximum number of teachers

    Student students[MAX_STUDENTS]; // Array to store students
    Teacher teachers[MAX_TEACHERS]; // Array to store teachers
    Admin admin;                    // Admin object

    int studentCount = 0;           // Current number of students
    int teacherCount = 0;           // Current number of teachers

    // Load students from file
    ifstream studentsFile("Students.txt");
    if (studentsFile.is_open())
    {
        while (studentsFile >> students[studentCount]) {
            studentCount++;
        }
        studentsFile.close();
    }

    // Load teachers from file
    ifstream teachersFile("Teachers.txt");
    if (teachersFile.is_open()) {
        while (teachersFile >> teachers[teacherCount]) {
            teacherCount++;
        }
        teachersFile.close();
    }

    char choice;
    while (true)
    {
        // Main menu
        cout << "                                   Welcome to Academy Management System\n\n";
        cout << "1. Admin login\n2. Teacher login\n3. Student login\n4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // To consume the newline character after choice input

        if (choice == '4')
        {
            system("cls");
            cout << "\nGood Bye" << endl;
            break;
        }

        switch (choice) {
        case '1':  // Admin login and menu
        {
            string uname, pass;
            cout << "Enter admin username: ";
            getline(cin, uname);
            cout << "Enter admin password: ";
            getline(cin, pass);
            if (admin.authenticate(uname, pass))
            {
                system("cls");
                int adminChoice;
                while (true)
                {
                    // Admin menu
                    cout << "1. Add Student\n2. Remove Student\n3. Add Teacher\n4. Remove Teacher\n5. Manage Fees\n6. Manage Salary\n7. Logout\n";
                    cout << "Enter your choice: ";
                    cin >> adminChoice;
                    cin.ignore();

                    if (adminChoice == 1)
                    {
                        // Add student
                        string sname, spass;
                        cout << "Enter student username: ";
                        getline(cin, sname);
                        cout << "Enter student password: ";
                        getline(cin, spass);
                        system("cls");
                        admin.addStudent(students, studentCount, sname, spass);

                    }
                    else if (adminChoice == 2)
                    {
                        // Remove student
                        string sname;
                        cout << "Enter student username: ";
                        getline(cin, sname);
                        system("cls");
                        admin.removeStudent(students, studentCount, sname);
                    }
                    else if (adminChoice == 3)
                    {
                        // Add teacher
                        string tname, tpass, subject1, subject2;
                        cout << "Enter teacher username: ";
                        getline(cin, tname);
                        cout << "Enter teacher password: ";
                        getline(cin, tpass);
                        cout << "Enter subject1: ";
                        getline(cin, subject1);
                        cout << "Enter subject2 (optional): ";
                        getline(cin, subject2);
                        system("cls");
                        admin.addTeacher(teachers, teacherCount, tname, tpass, subject1, subject2);
                    }
                    else if (adminChoice == 4)
                    {
                        // Remove teacher
                        string tname;
                        cout << "Enter teacher username: ";
                        getline(cin, tname);
                        system("cls");
                        admin.removeTeacher(teachers, teacherCount, tname);
                    }
                    else if (adminChoice == 5)
                    {
                        // Manage student fees
                        system("cls");
                        string sname;
                        cout << "Enter student username: ";
                        getline(cin, sname);
                        cout << "Want to Mark Paid(0), Unpaid(1): ";
                        int paid;
                        cin >> paid;
                        if (paid == 0)
                        {
                            for (int i = 0; i < studentCount; ++i)
                            {
                                if (students[i].username == sname)
                                {
                                    admin.manageFees(students[i], true);
                                    cout << "Fee is marked as paid for " << sname << endl;
                                    break;
                                }
                            }
                        }
                        if (paid == 1)
                        {
                            for (int i = 0; i < studentCount; ++i)
                            {
                                if (students[i].username == sname)
                                {
                                    admin.manageFees(students[i], false);
                                    cout << "Fee is marked as unpaid for " << sname << endl;
                                    break;
                                }
                            }
                        }

                    }
                    else if (adminChoice == 6)
                    {
                        // Manage teacher salary
                        system("cls");
                        string tname;
                        cout << "Enter teacher username: ";
                        getline(cin, tname);
                        cout << "Want to marked ready(0), Unready(1): ";
                        int choice;
                        cin >> choice;
                        if (choice == 0)
                        {
                            for (int i = 0; i < teacherCount; ++i) {
                                if (teachers[i].username == tname) {
                                    admin.calculateSalary(teachers[i], true);
                                    cout << "Salary marked as ready for " << tname << endl;
                                    break;
                                }
                            }
                        }
                        if (choice == 1)
                        {
                            for (int i = 0; i < teacherCount; ++i)
                            {
                                if (teachers[i].username == tname)
                                {
                                    admin.calculateSalary(teachers[i], false);
                                    cout << "Salary marked as Unready for " << tname << endl;
                                    break;
                                }
                            }
                        }
                    }
                    else if (adminChoice == 7)
                    {
                        // Logout admin
                        system("cls");
                        break;
                    }
                }
            }
            else
            {
                system("cls");
                cout << "Invalid admin credentials!" << endl;
            }
            break;
        }
        case '2':  // Teacher login and menu
        {
            string uname, pass;
            cout << "Enter teacher username: ";
            getline(cin, uname);
            cout << "Enter teacher password: ";
            getline(cin, pass);
            system("cls");
            bool found = false;
            for (int i = 0; i < teacherCount; ++i) {
                if (teachers[i].username == uname && teachers[i].password == pass) {
                    found = true;
                    int teacherChoice;
                    while (true) {
                        // Teacher menu
                        cout << "1. Change Student Marks\n2. Compile Results\n3. View Salary Status\n4. Logout\n";
                        cout << "Enter your choice: ";
                        cin >> teacherChoice;
                        cin.ignore();
                        system("cls");

                        if (teacherChoice == 1)
                        {
                            // Change student marks
                            string sname, subject;
                            int marks;
                            cout << "Enter student username: ";
                            getline(cin, sname);
                            cout << "Enter subject: ";
                            getline(cin, subject);
                            cout << "Enter new marks: ";
                            cin >> marks;
                            cin.ignore();
                            system("cls");
                            if (teachers[i].canTeachSubject(subject))
                            {
                                int count = 0;
                                for (int j = 0; j < studentCount; ++j)
                                {
                                    if (students[j].username == sname)
                                    {
                                        teachers[i].changeMarks(students[j], subject, marks);
                                        cout << "Marks updated for " << sname << " in subject " << subject << endl;
                                        count++;
                                        break;
                                    }
                                }

                                if (count == 0)
                                    cout << "Student not found" << endl;
                            }
                            else {
                                system("cls");
                                cout << "You are not authorized to change marks for this subject.\n";
                            }
                        }
                        else if (teacherChoice == 2)
                        {
                            // Compile results for all students
                            system("cls");
                            teachers[i].compileResults(students, studentCount);
                        }
                        else if (teacherChoice == 3)
                        {
                            // View salary status
                            system("cls");
                            teachers[i].SalaryStatus();
                        }
                        else if (teacherChoice == 4)
                        {
                            // Logout teacher
                            system("cls");
                            break;
                        }
                    }
                    break;
                }
            }
            if (!found) {
                system("cls");
                cout << "Invalid teacher credentials!" << endl;
            }
            break;
        }
        case '3':  // Student login and menu
        {
            string uname, pass;
            cout << "Enter student username: ";
            getline(cin, uname);
            cout << "Enter student password: ";
            getline(cin, pass);
            system("cls");
            bool found = false;
            for (int i = 0; i < studentCount; ++i)
            {
                if (students[i].username == uname && students[i].password == pass) {
                    found = true;
                    int studentChoice;
                    while (true) {
                        // Student menu
                        cout << "1. View Results\n2. View Fee Status\n3. Logout\n";
                        cout << "Enter your choice: ";
                        cin >> studentChoice;
                        cin.ignore();
                        system("cls");
                        if (studentChoice == 1)
                        {
                            // View results
                            system("cls");
                            students[i].viewResults();
                        }
                        else if (studentChoice == 2)
                        {
                            // View fee status
                            system("cls");
                            students[i].viewFeeStatus();
                        }
                        else if (studentChoice == 3)
                        {
                            // Logout student
                            system("cls");
                            break;
                        }
                    }
                    break;
                }
            }
            if (!found)
            {
                system("cls");
                cout << "Invalid student credentials!" << endl;
            }
            break;
        }
        default:
        {
            system("cls");
            cout << "Invalid Input" << endl;
            break;
        }
        }
    }

    // Save students to file before exiting
    ofstream studentsFileOut("Students.txt");
    for (int i = 0; i < studentCount; ++i) {
        studentsFileOut << students[i] << endl;
    }
    studentsFileOut.close();

    // Save teachers to file before exiting
    ofstream teachersFileOut("Teachers.txt");
    for (int i = 0; i < teacherCount; ++i)
    {
        teachersFileOut << teachers[i] << endl;
    }
    teachersFileOut.close();
}

// Main function: Entry point of the program
int main()
{
    Academy academy;
    academy.run();
    return 0;
}
