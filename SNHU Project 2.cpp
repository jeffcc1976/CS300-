//ABC University Course Selection
//Author: Jeff Carmichael

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;
#ifndef     _CSVPARSER_
# define    _CSVPARSER_

# include <stdexcept>
# include <string>
# include <vector>
# include <list>
# include <sstream>

namespace csv
{
    class Error : public std::runtime_error
    {

    public:
        Error(const std::string& msg) :
            std::runtime_error(std::string("CSVparser : ").append(msg))
        {
        }
    };

    class Row
    {
    public:
        Row(const std::vector<std::string>&);
        ~Row(void);

    public:
        unsigned int size(void) const;
        void push(const std::string&);
        bool set(const std::string&, const std::string&);

    private:
        const std::vector<std::string> _header;
        std::vector<std::string> _values;

    public:

        template<typename T>
        const T getValue(unsigned int pos) const
        {
            if (pos < _values.size())
            {
                T res;
                std::stringstream ss;
                ss << _values[pos];
                ss >> res;
                return res;
            }
            throw Error("can't return this value (doesn't exist)");
        }
        const std::string operator[](unsigned int) const;
        const std::string operator[](const std::string& valueName) const;
        friend std::ostream& operator<<(std::ostream& os, const Row& row);
        friend std::ofstream& operator<<(std::ofstream& os, const Row& row);
    };

    enum DataType {
        eFILE = 0,
        ePURE = 1
    };

    class Parser
    {

    public:
        Parser(const std::string&, const DataType& type = eFILE, char sep = ',');
        ~Parser(void);

    public:
        Row& getRow(unsigned int row) const;
        unsigned int rowCount(void) const;
        unsigned int columnCount(void) const;
        std::vector<std::string> getHeader(void) const;
        const std::string getHeaderElement(unsigned int pos) const;
        const std::string& getFileName(void) const;

    public:
        bool deleteRow(unsigned int row);
        bool addRow(unsigned int pos, const std::vector<std::string>&);
        void sync(void) const;

    protected:
        void parseHeader(void);
        void parseContent(void);

    private:
        std::string _file;
        const DataType _type;
        const char _sep;
        std::vector<std::string> _originalFile;
        std::vector<std::string> _header;
        std::vector<Row*> _content;

    public:
        Row& operator[](unsigned int row) const;
    };
}

#endif /*!_CSVPARSER_*/

// Structure for Comp Science Course
struct Course {
    string courseNumber;
    string courseTitle;
    vector< string> prereqs;
};

// Load the data
void loadSource(const string& fileName, vector<Course>& courses) {
    ifstream file(fileName);
    if (file.is_open()) {
        Course Course;
        while (file >> Course.courseNumber >> Course.courseTitle) {
            string prereqs;
            while (file >> prereqs && prereqs != "-1") {
                Course.prereqs.push_back(prereqs);
            }
            courses.push_back(Course);
            Course.prereqs.clear();
        }
        file.close();
        cout << "Courses loaded successfully!" << endl;
    }
    else {
        cout << "Unable to open file: " << fileName << endl;
    }
}

// Function to print course list
void printCourseList(const vector<Course>& courses) {
    for (const Course& course : courses) {
                if (course.courseNumber.substr(0, 4) == "CSCI" || course.courseNumber.substr(0, 4) == "MATH") {
            cout << course.courseNumber << " - " << course.courseTitle << endl;
        }
    }
}

// Function to find and print course information
void printCourseInfo(const vector<Course>& courses, const string& courseNumber) {
    bool found = false;
    for (const Course& course : courses) {
        if (course.courseNumber == courseNumber) {
            found = true;
            cout << "Course Title: " << course.courseTitle << endl;
            if (!course.prereqs.empty()) {
                cout << "Prereqs: ";
                for (const string& prereq : course.prereqs) {
                    cout << prereq << " ";
                }
                cout << endl;
            }
            else {
                cout << "No prerequisites for this course." << endl;
            }
            break;
        }
    }
    if (!found) {
        std::cout << "Course not found!" << endl;
    }
}

int main() {
    vector<Course> courses;
    string fileName;

    //Menu for course selection
    int choice;
    do {
        cout << " Welcome to the Course Planner\n\n";
        cout << "1. Load Data Structure\n2. Print Course List\n3. Print Course\n4. Exit\nEnter your choice: \n";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter the full path and file name with course data: ";
            cin >> fileName;
            loadSource(fileName, courses);
            break;
        case 2:
            if (courses.empty()) {
                cout << "Please load data first (Option 1)." << endl;
            }
            else {
                cout << "\nCourse List:" << endl;
                printCourseList(courses);
            }
            break;
        case 3:
            if (courses.empty()) {
                cout << "Please load data first (Option 1)." << endl;
            }
            else {
                string courseNumber;
                cout << "Enter the course number: ";
                cin >> courseNumber;
                printCourseInfo(courses, courseNumber);
            }
            break;
        case 4:
            cout << "Thank you for using Course Planner." << endl;
            break;
        default:
            cout << "Is not a valid option. Please enter a valid option." << endl;
        }
    } while (choice != 4);

    return 0;
}