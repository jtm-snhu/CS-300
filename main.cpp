// Project: CS-300 Module 7 Project 2
// Author : Jeffrey McGinty
// Email  : jeffrey.mcginty@snhu.edu
// Date   : October 13, 2022
/////////////////////////////////////

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <list>

using namespace std;

//
// Define Course Object Structure
//
struct Course{
    string courseCode = "0";
    string courseTitle = "0";
    vector<string> preReqs;
};

//
// Vector To Hold Courses
//
vector<Course> courses;

//
// Clear Terminal Screen
// Credit: https://stackoverflow.com/questions/17335816/clear-screen-using-c
//
void ClearScreen(){
    cout << "\033[2J\033[1;1H";
}


//
// Check that data is loaded
//
bool DataLoaded() {
    if (courses.size() > 0) {
        return true;
    }
    cout << "No Data Has Been Loaded." << endl;
    return false;
}

//
// Display Main Menu
//
void DisplayMenu(){
    cout << endl;
    cout << "1 - Load Data\n";
    cout << "2 - Print Course List\n";
    cout << "3 - Print Single Course Details\n";
    cout << "9 - Exit\n\n";

    cout << "Your choice: ";
}


//
// Add a course to the main vector list
//
void AddCourse(vector<string> tempVec){
    // Takes temp vector with individual course information

    Course tempCourse;              // Temporary object to format course data
    vector<string> tempPre;         // Temporary vector to hold preReqs

    if (tempVec.size() == 2) {                          // If only 2 items there are no preReqs
        tempCourse = {tempVec.at(0), tempVec.at(1)};    // create formatted course object
        courses.push_back(tempCourse);                  // push object onto the main courses vector
    }
    else {
        for (int i = 2; i < tempVec.size(); i++) {              // Start at tempVec index 2 to get preReqs
            tempPre.push_back(tempVec.at(i));                   // And loop to end of vector to get all
        }                                                       // Push each item onto temp preReqs vector
        tempCourse = {tempVec.at(0), tempVec.at(1), tempPre};   // Create formatted object
        courses.push_back(tempCourse);                          // Push object onto the main courses vector
    }
}


//
// Selection Sort List On courseCode
//
void SortCourses() {
    // Comparing entire courseCode string to sort
    // does not yield desired results, so codes
    // must be compared in two parts to achieve
    // correct alphanumeric sort order
    Course tempCourse;          // Temp object for swapping
    int indexSmallest = 0;

    for (int i = 0; i < courses.size(); ++i){
        //Get index of smallest remaining item
        for (int j = i+1; j < courses.size(); ++j){
            // compare courseCode letters first and swap if needed
            if (courses.at(j).courseCode.substr(0,3) < courses.at(i).courseCode.substr(0,3)) {
                tempCourse = courses.at(i);
                courses.at(i) = courses.at(j);
                courses.at(j) = tempCourse;
            }
            // compare courseCode numbers and swap if needed
            else if (courses.at(j).courseCode.substr(4,7) < courses.at(i).courseCode.substr(4,7)) {
                tempCourse = courses.at(i);
                courses.at(i) = courses.at(j);
                courses.at(j) = tempCourse;
            }
        }
    }
}


//
// Load data file
//
void LoadData(string filename) {
    // Open file for reading
    ifstream dataFile;
    dataFile.open(filename);

    string line;                // To loop through file lines
    string token;               // Temporary variable to split lines
    vector<string> tempVec;     // Temporary variable to split lines

    if (dataFile.is_open()){
        while (getline(dataFile, line)) {               // Read each line until end of file
            stringstream tempStream(line);              // Convert line to stream for parsing
            while (tempStream.good()) {                 // While there is data left on the line
                    getline(tempStream, token, ',');    // Get tokens delimited by comma
                    tempVec.push_back(token);           // Push token onto temporary vector
            }
            AddCourse(tempVec);                         // Add information to courses vector
            tempVec.clear();                            // Clear temp vector for next loop
        }
        dataFile.close();                               // Close file when loop exits
        SortCourses();                         // Sort list into ascending order
    }
    else {
        cout << "Error opening data file " << filename << " - No data loaded\n";
    }

}


//
//Print entire list of courses
//
void PrintCourseList(){
    Course tempCourse; // temporary object to print from

    cout << endl;
    for (int i = 0; i < courses.size(); i++){       //Loop through each course in the vector
        tempCourse = courses.at(i);
        cout << tempCourse.courseCode << ", " << tempCourse.courseTitle << endl;

    }
}


//
// Search for course by course code
//
Course SearchCourses(string searchCode){
    Course notFound;    //Empty course if not found

    for (int i = 0; i < courses.size(); i++){
            if (courses.at(i).courseCode.compare(searchCode) == 0){
                return courses.at(i);
            }
    }
    return notFound;
}


//
// Print course details
//
void PrintCourseDetails(){
    string courseCode;
    Course details;     //Empty course object to receive data


    cout << "Enter Course Code For Details: ";
    cin >> courseCode;

    //Convert input to uppercase for comparison
    for (int i = 0; i < courseCode.length(); i++) {
        courseCode[i] = toupper(courseCode[i]);
    }


    details = SearchCourses(courseCode);
    if (details.courseCode == "0") {
        cout << "That course code does not exist." << endl;
    }
    else {
        cout << endl;
        cout << details.courseCode << ", " << details.courseTitle << endl;  // Output code and title
        if (details.preReqs.size() > 0) {                                   // Check for preReqs
            cout << "Prerequisites: ";
            bool needComma = false;                                         // Decide if comma needed
            for (int i = 0; i < details.preReqs.size(); i++){               // Loop through preReqs
                if (needComma) {
                    cout << ", ";
                }
                cout << details.preReqs.at(i);                              // Print preReq code
                needComma = true;                                           // Turn on comma for next loop
            }
            cout << "\n\n";
        }
        else {
            cout << "No Prerequisites" << endl;
        }
    }

}

//
// Main
//
int main(int argc, char* argv[])
{
        // process command line arguments
    string dataFile;
    switch (argc) {
        case 2:
            dataFile = argv[1];
            break;

        default:
        dataFile = "courselist.csv";
    }

    // Show menu and get choice
    int menuChoice = 0;
    ClearScreen();
    while (menuChoice != 9){
        DisplayMenu();
        cin >> menuChoice;

        switch (menuChoice){
            case 1:
                LoadData(dataFile);
                break;

            case 2:
                if (DataLoaded()){
                    PrintCourseList();
                }
                break;

            case 3:
                if (DataLoaded()){
                    PrintCourseDetails();
                }
                break;
            default:
                cout << "\nInvalid Choice - Try Again\n";
        }
    }

    return 0;
}
