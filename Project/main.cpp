#include <iostream>
#include <cstring>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
#include <set>
#include <algorithm>

using namespace std;

// Course structure
struct Course
{
    string courseNumber;
    string courseName;
    vector<string> prerequisites;

    Course () {}
};

// Node structure
struct Node {
    Course* course;
    Node* right;
    Node* left;
    Node(Course* aCourse) {
        course = aCourse;
        right = nullptr;
        left = nullptr;
    }
};

// Binary Search Tree class
class BST {
public:
    Node* root;

    // default constructor
    BST() {
        root = nullptr;
    }
    
    // inserts a course object into a binary search tree
    void Insert(Course* aCourse) {
        if (root == nullptr) {
            root = new Node(aCourse);
        }
        else {
            addCourse(root, aCourse);
        }
    }

    // adds a course object onto a binary search tree node; integrated with Insert(aCourse) function
    void addCourse(Node* node, Course* aCourse) {
        if (aCourse->courseNumber.compare(node->course->courseNumber) < 0) {
            if (node->left == nullptr) {
                node->left = new Node(aCourse);
                return;
            }
            else {
                addCourse(node->left, aCourse);
            }
        }
        else {
            if (node->right == nullptr) {
                node->right = new Node(aCourse);
                return;
            }
            else {
                addCourse(node->right, aCourse);
            }
        }
    }

};

// converts a string to all caps
string to_uppercase(string input) {
    string result = input;
    for (char& c : result) {
        c = toupper(c);
    }
    return result;
}

// parses a string line into a Course object
// also parses courseNumbers and prerequisites into sets courseNumList and prereqList respectively
bool parseLine(string oneLine, Course* aCourse, set<string>* courseNumList, set<string>* prereqList) {
    stringstream ss(oneLine);
    string token;
    vector<string> fields;

    while (getline(ss, token, ',')) {
        // gets each string in the line
        fields.push_back(token);
    }

    // inducts error if a comma is missing
    if (fields.size() < 2) {
        return false;
    }

    aCourse->courseNumber = fields[0];
    aCourse->courseName = fields[1];

    // add every courseNumber into set courseNumList
    courseNumList->insert(aCourse->courseNumber);
    
    // adds the prerequisites of a line into the vector of a corresponding course
    for (size_t i = 2; i < fields.size(); ++i) {
        aCourse->prerequisites.push_back(fields[i]);
        // add every prerequisite into set prereqList
        prereqList->insert(aCourse->prerequisites.at(i-2));
    }

    return true;

}

// reads the file, parses the lines, and returns a list of courses in binary search tree format
BST* readFile(string fileName) {

    BST* courses = new BST();
    set<string> courseNumbersList = {};
    set<string>* courseNumbers = &courseNumbersList;
    set<string> prerequisitesList = {};
    set<string>* pLists = &prerequisitesList;
    
    // opens and reads file
    ifstream courseListFromFile(fileName);

    if (!courseListFromFile.is_open()) {
        cout << "Error: Unable to open file" << endl;
        return courses;
    }

    string oneLine;

    // parses lines into courses
    // returns null if there is an error in prerequisites or courses
    while (getline(courseListFromFile, oneLine)) {
        Course* course = new Course();
        if (parseLine(oneLine, course, courseNumbers, pLists)) {
            courses->Insert(course);
        }
        else {
            cout << "Error: Invalid course format in file" << endl;
            return nullptr;
        }

    }

    //checks if prerequisitesList is a subset of courseNumbersList
    if (!includes(courseNumbersList.begin(), courseNumbersList.end(), prerequisitesList.begin(), prerequisitesList.end())) {
        cout << "\nError: Prerequisite conditions are not met" << endl;
        cout << "Prerequisite List is not a subset of CourseNumbersList" << endl;
        return nullptr;
    }

    courseListFromFile.close();
    return courses;
}

// prints the course number and name alphanumerically
void BSTPrintInorder(Node* node) {
  if (node) {
    BSTPrintInorder(node->left);
    cout << node->course->courseNumber << ", " << node->course->courseName << endl;     
    BSTPrintInorder(node->right);
  }
}

// prints a selected course and its information after traversing/searching aTree
void printACourseInformation(BST* aTree, string aCourseNumber) {
    Node* curr;
    curr = aTree->root;

    while (curr != nullptr)
    {
        if (curr->course->courseNumber.compare(aCourseNumber) == 0) {
            cout << "\n" << curr->course->courseNumber << ", ";
            cout << curr->course->courseName << endl;
            cout << "Prerequisites: ";

            if (curr->course->prerequisites.size() == 0) {
                cout << "None\n";
            }

            for (int i = 0; i < curr->course->prerequisites.size(); i++) {
                cout << curr->course->prerequisites[i];
                if (i == curr->course->prerequisites.size() - 1) {
                    cout << "\n\n";
                }
                else {cout << ", ";}
            };
            return;
        }
        else if (curr->course->courseNumber.compare(aCourseNumber) > 0) {
            curr = curr->left;
        }
        else if (curr->course->courseNumber.compare(aCourseNumber) < 0) {
            curr = curr->right;
        }
    }
    cout << "\n" << aCourseNumber << " could not be found in the data structure\n" << endl;
    return;
    
};

int main() {
    
    BST* coursesInFile = nullptr;

    int choice = 0;
    string wanted_course = "";
    cout << "Welcome to the course planner.\n";
    while (choice != 9) {
        
        cout << "\n   1. Load Data Structure.\n";
        cout << "   2. Print Course List.\n";
        cout << "   3. Print Course Information\n";
        cout << "   9. Exit\n\n";
        cout << "What would you like to do?\n";

        cin >> choice;

        switch (choice)
        {
            case 1:
            {
                cout << "Loading Data Structure...\n";
                // change the value passed in readfile()
                coursesInFile = readFile("C:\\Users\\Eyu\\Documents\\1. School Files\\DSA Analysis and Design\\Project\\Courses.txt");
                break;
            }
            case 2:
            {
                if (coursesInFile == nullptr) {
                    cout << "\nThere is no data structure loaded into the program\n";
                    break;
                }
                cout << "\nHere is a sample schedule: \n\n";
                BSTPrintInorder(coursesInFile->root);
                break;
            }
            case 3:
            {
                if (coursesInFile == nullptr) {
                    cout << "\nThere is no data structure loaded into the program\n";
                    break;
                }
                cout << "What course do you want to know about? (Enter the course Id)\n";
                cin >> wanted_course;
                printACourseInformation(coursesInFile, to_uppercase(wanted_course));
                break;
            }
            case 9:
            {
                cout << "Exiting the program." << endl;
                return 0;
            }
            default:
            {
                cout << "\nThat is not a valid option.\n\n";
                break;
            }
        }

        cin.clear ();
        cin.ignore (50, '\n');
    }

    return 0;
}