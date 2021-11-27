#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <string>
#include <string.h>
#include <Windows.h>

using namespace std;

//Global Varables
const char* dir = "system.db";
string temp, userTypeRes, userFName, userLName;
int choice, userNumber;
bool login = false;



//Database Functions
void createDB(const char* s);
void createTable(const char* s);
void addUser(const char* s, int number, string fname, string lname, string password, string userType);
int callbackStudentGradeS(void* NotUsed, int argc, char** argv, char** azColName);
int callbackStudentGradeT(void* NotUsed, int argc, char** argv, char** azColName);
bool gradeAccessible(const char* s, int studentNumber, int userNumber);
void gradeAccessibleOutputSpecs(const char* s, int studentNumber, int teacherNumberTmp);
void gradeAccessibleOutputSpecsCode(const char* s, int teacherNumberTmp, int gradeCode);
bool gradeCode(const char* s, int gradeCode, int studentNumber, int userNumber);
bool userExist(const char* s, int number);
bool userExist(const char* s, int number, string password, string userType);
void userInfo(const char* s, int number);
void addGrades(const char* s, int studentNumber, int userNumber, int grade, string subject, string comment);
void updateGrades(const char* s, int studentNumber, int grade, string subject, string comment);
void deleteGrades(const char* s, int studentNumber);
//Validation Functions
int intValid(string temp);
void invalidInput();
bool checkAlpha(string str);
int checkWhiteSpace(string str);
//Output Menu Functions
void oSystem();
void oStudentAuth();
void oStudentAuthLog();
void oStudentAuthReg();
void oStudentMenu();
void oStudentGrades();
void oTeacherAuth();
void oTeacherAuthLog();
void oTeacherAuthReg();
void oTeacherMenu();
void oTeacherGradeView();
void oTeacherGradeViewAll();
void oTeacherGradeViewSpec();
void oTeacherAddGrade();
void oTeacherUpdateGrade();
void oTeacherDeleteGrade();


int main() {
	//Run these to make sure there is a database
	createDB(dir);
	createTable(dir);

	do {
		//System Menu Output
		oSystem();

		//Switch case for the menu selection
		switch(choice) {
			//Student Menu Authentication
		case 1:
			do {
				//Output Menu
				oStudentAuth();

				switch (choice) {
					//Student Login
				case 1:
					oStudentAuthLog();
                    if(login) {
                        do {
                            //Output Menu For Login Student
                            oStudentMenu();

                            //Switch case for the Student Menu
                            switch(choice) {
                                case 1:
                                    //Show the Students Grades
                                    oStudentGrades();
                                break;
                                case 2: 
                                    //For the logout
                                    login = false;
                                    userNumber = 0;
                                break;
                                default:
                                    invalidInput();
                            }
                        } while (choice != 2);
                    }
				break;

					//Student Register
				case 2:
					oStudentAuthReg();
				break;

					//Student Back
				case 3:
				break;

					//Output for Invalid Choice
				default:
					invalidInput();
				}
			} while(choice != 3);
			choice = 0;
			break;

			//Teacher Menu Authentication
		case 2:
			do {
				//Output Menu
				oTeacherAuth();

				switch(choice) {
					//Teacher Login
				case 1:
					oTeacherAuthLog();
                    if(login) {
                        do {
                            //Output Menu For login Teacher
                            oTeacherMenu();

                            //Switch case for the Teacher Menu
                            switch(choice) {
                            case 1:
                                do {
                                    //Show Graded Informations
                                    oTeacherGradeView();

                                    //Switch for the Student Grade Viewer
                                    switch (choice) {
                                    case 1:
                                        //View All Graded Students
                                        oTeacherGradeViewAll();
                                        break;
                                    case 2:
                                        //View Specific Grade From a Student
                                        oTeacherGradeViewSpec();
                                        break;
                                    case 3: //For the Back
                                        break;
                                    default:
                                        invalidInput();
                                    }
                                } while(choice != 3);
                            break;
                            case 2:
                                //Add Grade
                                oTeacherAddGrade();
                            break;
                            case 3:
                                //Edit or Update Grade
                                oTeacherUpdateGrade();
                            break;
                            case 4:
                                //Delete Grade
                                oTeacherDeleteGrade();
                            break;
                            case 5:
                                //For the logout
                                login = false;
                                userNumber = 0;
                            break;
                            default:
                                invalidInput();
                            }
                        } while (choice != 5);
                    }
				break;

					//Teacher Register
				case 2:
					oTeacherAuthReg();
				break;

					//Teacher Back
				case 3:
				break;

					//Output for Invalid Choice
				default:
					invalidInput();
				}
			} while (choice != 3);
			choice = 0;
			break;

			//Exit
		case 3:
		break;

			//Output for Invalid Choice
		default:
			invalidInput();
		}
	} while (choice != 3);

	return 0;
}



/*Start of Database Functions*/

//Makes sure there is a database
void createDB(const char* s) {
	sqlite3* DB;

	int state = sqlite3_open(s, &DB);
	sqlite3_close(DB);
}

//Create a table if there is no table in the database
void createTable(const char* s) {
	sqlite3* DB;

    string tbl1 = "CREATE TABLE IF NOT EXISTS USERS("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "NUMBER    INT  NOT NULL, "
        "FNAME     TEXT NOT NULL, "
        "LNAME     TEXT NOT NULL, "
        "PASSWORD  TEXT NOT NULL, "
        "USER_TYPE TEXT NOT NULL);";
    string tbl2 = "CREATE TABLE IF NOT EXISTS GRADES("
        "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
        "STUDENT_NUMBER     INT  NOT NULL, "
        "TEACHER_NUMBER     INT  NOT NULL, "
        "GRADE              INT  NOT NULL, "
        "SUBJECT            TEXT NOT NULL, "
        "COMMENT            TEXT NOT NULL);";
	try
	{
		int state = sqlite3_open(s, &DB);
		state = sqlite3_exec(DB, tbl1.c_str(), NULL, 0, 0);
        state = sqlite3_exec(DB, tbl2.c_str(), NULL, 0, 0);
		sqlite3_close(DB);
	}
	catch (exception e)
	{
		cout << "Error Database Table Creation";
	}
}

// argc: holds the number of results, argv: holds each value in array, azColName: holds each column returned in array
int callbackStudentGradeS(void* NotUsed, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        if (i == 2)
            cout << "Teacher: " << argv[i] << endl;
        else if (i == 4)
            cout << "Subject/Course: " << argv[i] << endl;
        else if (i == 3)
            cout << "Grade: " << argv[i] << endl;
        else if (i == 5)
            cout << "Comment: " << argv[i] << "\n-------------------------------------------------------\n";
    }
    return 0;
}

// argc: holds the number of results, argv: holds each value in array, azColName: holds each column returned in array
int callbackStudentGradeT(void* NotUsed, int argc, char** argv, char** azColName) {
    for(int i = 0; i < argc; i++) {
        if(i == 0)
            cout << "Grade Code: " << argv[i] << endl;
        else if(i == 1)
            cout << "Student Number: " << argv[i] << endl;
        else if (i == 3)
            cout << "Grade: " << argv[i] << endl;
        else if (i == 4)
            cout << "Subject/Course: " << argv[i] << endl;
        else if (i == 5)
            cout << "Comment: " << argv[i] << "\n-------------------------------------------------------\n";
    }
    return 0;
}

//This add a user to the database
void addUser(const char* s, int number, string fname, string lname, string password, string userType) {
    sqlite3* DB;;
    char* messageError;

    //Convert number to string
    string numberConverted = to_string(number);

    string sql("INSERT INTO USERS (NUMBER, FNAME, LNAME, PASSWORD, USER_TYPE) VALUES('" + numberConverted + "','" + fname + "','" + lname + "','" + password + "','" + userType + "');");

    int state = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here */
    state = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);
    if (state != SQLITE_OK) { //If the Query failed to Execute
        cout << "\nCannot Add User." << endl << endl;
        sqlite3_free(messageError);
        system("pause");
    }
    else { //If the Query Execute Successfully
        cout << "\nUser Added Successfuly" << endl << endl;
        system("pause");
    }
}

//Check if the userId already exist
bool userExist(const char* s, int number) {
    sqlite3* DB;
    sqlite3_stmt* res;

    //Convert number to string
    string numberConverted = to_string(number);

    string query = "SELECT * FROM USERS WHERE NUMBER = '" + numberConverted + "';";

    int state = sqlite3_open(s, &DB);
    state = sqlite3_prepare_v2(DB, query.c_str(), query.length(), &res, 0);
    if (state != SQLITE_OK) { //Check if the query executed successfully
        cout << "Error";
    }

    state = sqlite3_step(res);

    if (state != SQLITE_ROW) { //When there is no result from the query
        sqlite3_finalize(res);
        return true;
    } else {
        sqlite3_finalize(res); //When there is a result from the query
        return false;
    }
}

//This is for the Authentication of user login
bool userExist(const char* s, int number, string password, string userType) {
    sqlite3* DB;
    sqlite3_stmt* res;

    //Convert number to string
    string numberConverted = to_string(number);

    string query = "SELECT * FROM USERS WHERE NUMBER = '" + numberConverted + "' AND PASSWORD = '" + password + "' AND USER_TYPE = '" + userType + "';";

    int state = sqlite3_open(s, &DB);
    state = sqlite3_prepare_v2(DB, query.c_str(), query.length(), &res, 0);
    if (state != SQLITE_OK) { //Check if the query executed successfully
        cout << "Error";
    }

    state = sqlite3_step(res);

    if (state == SQLITE_ROW) { //When there is a result from the query
        userFName = (char*)sqlite3_column_text(res, 2);
        userLName = (char*)sqlite3_column_text(res, 3);
        sqlite3_finalize(res);
        return true;
    }
    else {
        sqlite3_finalize(res); //When there is no result from the query
        return false;
    }
}

//Get the user information, namely the User Type
void userInfo(const char* s, int number) {
    sqlite3* DB;
    sqlite3_stmt* res;

    //Convert number to string
    string numberConverted = to_string(number);

    string query = "SELECT USER_TYPE FROM USERS WHERE NUMBER = '" + numberConverted + "';";

    int state = sqlite3_open(s, &DB);
    state = sqlite3_prepare_v2(DB, query.c_str(), query.length(), &res, 0);
    if (state != SQLITE_OK) { //Check if the query executed successfully
        cout << "Error";
    }

    state = sqlite3_step(res);

    if (state != SQLITE_ROW) { //When there is no result from the query
        sqlite3_finalize(res);
        return;
    }
    else { //When there is a result from the query
        userTypeRes = (char*)sqlite3_column_text(res, 0);
        sqlite3_finalize(res); 
        return;
    }
}

//Add grades to the student
void addGrades(const char* s, int studentNumber, int userNumber, int grade,string subject, string comment) {
    sqlite3* DB;
    sqlite3_stmt* res;

    //Convert number to string
    string sNumConverted = to_string(studentNumber);
    string tNumConverted = to_string(userNumber);
    string gradeConverted = to_string(grade);

    string insert = "INSERT INTO GRADES(STUDENT_NUMBER, TEACHER_NUMBER, GRADE, SUBJECT, COMMENT) VALUES('" + sNumConverted + "','" + tNumConverted + "','" + gradeConverted + "','" + subject + "','" + comment + "');";

    int state = sqlite3_open(s, &DB);
    state = sqlite3_prepare_v2(DB, insert.c_str(), insert.length(), &res, 0);
    if (state != SQLITE_OK) { //Check if the query executed successfully
        cout << "Error\n\n";
        system("pause");
    } else {
        cout << "Student Successfully Graded.\n\n";
        system("pause");
    }

    state = sqlite3_step(res);
}

//Check if the grade is accessible
bool gradeAccessible(const char* s, int studentNumber, int userNumber) {
    sqlite3* DB;
    sqlite3_stmt* res;

    //Convert number to string
    string studentNumConverted = to_string(studentNumber);
    string teacherNumConverted = to_string(userNumber);

    string query = "SELECT * FROM GRADES WHERE STUDENT_NUMBER = '" + studentNumConverted + "' AND TEACHER_NUMBER = '" + teacherNumConverted + "';";

    int state = sqlite3_open(s, &DB);
    state = sqlite3_prepare_v2(DB, query.c_str(), query.length(), &res, 0);
    if (state != SQLITE_OK) { //Check if the query executed successfully
        cout << "Error";
    }

    state = sqlite3_step(res);

    if (state == SQLITE_ROW) { //When there is a result from the query
        sqlite3_finalize(res);
        return true;
    } else {
        sqlite3_finalize(res); //When there is no result from the query
        return false;
    }
}

//Check if the grade already exist
bool gradeCode(const char* s, int gradeCode, int studentNumber, int userNumber) {
    sqlite3* DB;
    sqlite3_stmt* res;

    //Convert number to string
    string gradeCodeConverted = to_string(gradeCode);
    string studentNumConverted = to_string(studentNumber);
    string teacherNumConverted = to_string(userNumber);

    string query = "SELECT * FROM GRADES WHERE STUDENT_NUMBER = '" + studentNumConverted + "' AND TEACHER_NUMBER = '" + teacherNumConverted + "' AND ID = '" + gradeCodeConverted + "';";

    int state = sqlite3_open(s, &DB);
    state = sqlite3_prepare_v2(DB, query.c_str(), query.length(), &res, 0);
    if (state != SQLITE_OK) { //Check if the query executed successfully
        cout << "Error";
    }

    state = sqlite3_step(res);

    if (state == SQLITE_ROW) { //When there is a result from the query
        sqlite3_finalize(res);
        return true;
    }
    else {
        sqlite3_finalize(res); //When there is no result from the query
        return false;
    }
}

//Output data, this uses a callback function callbackStudentGradeT
void gradeAccessibleOutput(const char* s) {
    sqlite3* DB;
    char* messageError;

    string query = "SELECT * FROM GRADES;";

    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, query.c_str(), callbackStudentGradeT, NULL, &messageError);

    if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
    }
}

//Output data, this uses a callback function callbackStudentGradeS
void gradeAccessibleOutputStudent(const char* s, int studentNumber) {
    sqlite3* DB;
    char* messageError;

    //Convert int to string
    string studentNumConverted = to_string(studentNumber);

    string query = "SELECT * FROM GRADES WHERE STUDENT_NUMBER = '" + studentNumConverted + "';";

    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, query.c_str(), callbackStudentGradeS, NULL, &messageError);

    if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
    }
}

//Output specific data, this uses a callback function callbackStudentGradeT
void gradeAccessibleOutputAll(const char* s, int teacherNumberTmp) {
    sqlite3* DB;
    char* messageError;

    //Convert int to string
    string teacherNumConverted = to_string(teacherNumberTmp);
    
    string query = "SELECT * FROM GRADES WHERE TEACHER_NUMBER = '" + teacherNumConverted + "';";

    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, query.c_str(), callbackStudentGradeT, NULL, &messageError);

    if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
    }
}

//Output all the data, this uses a callback function callbackStudentGradeT
void gradeAccessibleOutputSpecs(const char* s, int studentNumber, int teacherNumberTmp) {
    sqlite3* DB;
    char* messageError;

    //Convert int to string
    string studentNumConverted = to_string(studentNumber);
    string teacherNumConverted = to_string(teacherNumberTmp);

    string query = "SELECT * FROM GRADES WHERE STUDENT_NUMBER = '" + studentNumConverted + "' AND TEACHER_NUMBER = '" + teacherNumConverted + "';";

    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, query.c_str(), callbackStudentGradeT, NULL, &messageError);

    if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
    }
}

//Output all the data, this uses a callback function callbackStudentGradeT
void gradeAccessibleOutputSpecsCode(const char* s, int gradeCode, int teacherNumberTmp) {
    sqlite3* DB;
    char* messageError;

    //Convert int to string
    string teacherNumConverted = to_string(teacherNumberTmp);
    string gCodeNumConverted = to_string(gradeCode);

    string query = "SELECT * FROM GRADES WHERE ID = '" + gCodeNumConverted + "' AND TEACHER_NUMBER = ' " + teacherNumConverted + " ';";

    int exit = sqlite3_open(s, &DB);
    /* An open database, SQL to be evaluated, Callback function, 1st argument to callback, Error msg written here*/
    exit = sqlite3_exec(DB, query.c_str(), callbackStudentGradeT, NULL, &messageError);

    if (exit != SQLITE_OK) {
        sqlite3_free(messageError);
    }
}

//Update or Edit grades of an existing and accessible grade
void updateGrades(const char* s, int gradeCodeNum, int grade, string subject, string comment) {
    sqlite3* DB;
    sqlite3_stmt* res;

    //Convert number to string
    string gCodeConverted = to_string(gradeCodeNum);
    string tNumConverted = to_string(userNumber);
    string gradeConverted = to_string(grade);

    string update = "UPDATE GRADES SET GRADE = '" + gradeConverted + "',SUBJECT = '" + subject + "',COMMENT = '" + comment + "' WHERE ID = '" + gCodeConverted + "';";

    int state = sqlite3_open(s, &DB);
    state = sqlite3_prepare_v2(DB, update.c_str(), update.length(), &res, 0);
    if (state != SQLITE_OK) { //Check if the query executed successfully
        cout << "Error\n\n";
        system("pause");
    }
    else {
        cout << "\n\nStudent Grade Updated.\n\n";
        system("pause");
    }

    state = sqlite3_step(res);
}

//Delete an existing and accessible grade
void deleteGrades(const char* s, int gradeCode) {
    sqlite3* DB;
    sqlite3_stmt* res;

    //Convert number to string
    string gradeCodeConverted = to_string(gradeCode);

    string deleteQuery = "DELETE FROM GRADES WHERE ID = '" + gradeCodeConverted + "';";
    int state = sqlite3_open(s, &DB);
    state = sqlite3_prepare_v2(DB, deleteQuery.c_str(), deleteQuery.length(), &res, 0);
    if (state != SQLITE_OK) { //Check if the query executed successfully
        cout << "Error\n\n";
        system("pause");
    }
    else {
        cout << "\n\nStudent Grade Deleted.\n\n";
        system("pause");
    }

    state = sqlite3_step(res);

}
/*End of Database Functions*/




/*Start of Data Verification Functions*/
//Verify if the choice is valid
int intValid(string temp) {
	//Check if the Input is a valid input
	try {
		return stoi(temp);
	}
	catch (...) {
		return 0;
	}
}

//Verify if the input is a alpha characters
bool checkAlpha(string str) {
    //Check if the Code Contain other characters other than alpha characters
	for (int i = 0; i < str.size(); i++) {
		if (!isalpha(str[i]) && !isspace(str[i])) {
			return true;
		}
	}
	return false;
}

//Check for white spaces in the input
int checkWhiteSpace(string str) {
    //As the function name suggest, it checks for whitespaces
	int num = 0;

	for (int i = 0; i < str.size(); i++) {
		if (isspace(str[i])) {
			num++;
		}
	}
	return num;
}

//Output for an error message for invalid inputs
void invalidInput() {
    //An output for Invalid Inputs
	cout << "\nInvalid Input\n";
	system("pause");
}
/*End of Data Verification Functions*/




/*Start of Output Menu Functions*/
//Starting System Menu
void oSystem() {
	//System Menu Output
	SetConsoleTitle(TEXT("Student Report/Record Management System"));
	system("cls");
	cout << "Student Report/Record Management System\n";
	cout << "[1] Student Menu\n";
	cout << "[2] Teacher Menu\n";
	cout << "[3] Exit\n";
	cout << "\nChoice>>> ";
	getline(cin, temp);
	choice = intValid(temp);
}

//Student Selection Menu
void oStudentAuth() {
    SetConsoleTitle(TEXT("Student Menu"));
    system("cls");
    cout << "Student Menu\n";
    cout << "[1] Login\n";
    cout << "[2] Register\n";
    cout << "[3] Back\n";
    cout << "\nChoice>>> ";
    getline(cin, temp);
    choice = intValid(temp);
}

//Menu for the Student Login
void oStudentAuthLog() {
    int number;
    string password;

    SetConsoleTitle(TEXT("Student Login"));
    system("cls");
    cout << "Student Login\n\n";
    cout << "Student Number: ";
    getline(cin, temp);
    number = intValid(temp);
    cout << "Student Password: ";
    getline(cin, password);

    if(userExist(dir, number, password, "student")) { //If the user exist
        login = true;
        userNumber = number;
        cout << "Login Successful\n";
        system("pause");
    } else { //If the user does not exist
        cout << "Student Number/Password Incorrect\n";
        system("pause");
    }
}

//Menu for the Student Registration
void oStudentAuthReg() {
    int number;
    string fname, lname, password, cpassword;
    bool pass;

    SetConsoleTitle(TEXT("Student Register"));
    //Loop Until Valid Input For Student Number
    do {
        pass = false;
        system("cls");
        cout << "Student Register\n\n";
        cout << "Student Number: ";
        getline(cin, temp);
        if (!(number = intValid(temp) > 0)) {
            cout << "Invalid Student Number\n\n";
            system("pause");
        }
        else {
            pass = true;
            number = intValid(temp);
        }
    } while (!pass);

    //Loop Until Valid Input For Student First Name
    do {
        pass = false;
        system("cls");
        cout << "Student Register\n\n";
        cout << "Student Number: " << number << endl;
        cout << "Student First Name: ";
        getline(cin, fname);
        if (fname.empty() || checkWhiteSpace(fname) == fname.size()) {
            cout << "First Name Cannot be Empty\n\n";
            system("pause");
        }
        else if (checkAlpha(fname)) {
            cout << "First Name Should Only Contain AlphaCharacters\n\n";
            system("pause");
        }
        else {
            pass = true;
        }
    } while (!pass);

    //Loop Until Valid Input For Student Last Name
    do {
        pass = false;
        system("cls");
        cout << "Student Register\n\n";
        cout << "Student Number: " << number << endl;
        cout << "Student First Name: " << fname << endl;
        cout << "Student Last Name: ";
        getline(cin, lname);
        if (lname.empty() || checkWhiteSpace(lname) == lname.size()) {
            cout << "Last Name Cannot be Empty\n\n";
            system("pause");
        }
        else if (checkAlpha(lname)) {
            cout << "Last Name Should Only Contain AlphaCharacters\n\n";
            system("pause");
        }
        else {
            pass = true;
        }
    } while (!pass);

    //Loop Until Valid Input For Student Password
    //Should be At least 8 Characters
    do {
        pass = false;
        system("cls");
        cout << "Student Register\n\n";
        cout << "Student Number: " << number << endl;
        cout << "Student First Name: " << fname << endl;
        cout << "Student Last Name: " << lname << endl;
        cout << "Student Password: ";
        getline(cin, password);
        cout << "Confirm Student Password: ";
        getline(cin, cpassword);
        if (checkWhiteSpace(password) == password.size() || password.size() < 8 || password.empty() || (password.size() - checkWhiteSpace(password) < 8)) {
            cout << "Password Cannot Have Spaces and Should be at least 8 Character long\n\n";
            system("pause");
        } else if(!(password == cpassword)) {
            cout << "Password Does Not Match\n\n";
            system("pause");
        }
        else {
            pass = true;
        }

    } while (!pass);

    //Check if the Student Number Already Exist
    if(userExist(dir, number)) { //If does not exist
        //Register User
        addUser(dir, number, fname, lname, password, "student");
    } else { //If it exist
        cout << "Student Number already exist\n\n";
        system("pause");
    }
}

//Menu for the Login Student
void oStudentMenu() {
    SetConsoleTitle(TEXT("Student Menu"));
    system("cls");
    cout << "Welcome Back Student, " << userFName << " " << userLName << endl;
    cout << "Student Menu\n";
    cout << "[1] View Grades\n";
    cout << "[2] Logout\n";
    cout << "\nChoice>>> ";
    getline(cin, temp);
    choice = intValid(temp);
}

//Menu when the student view their grades
void oStudentGrades() {
    SetConsoleTitle(TEXT("Student Grades"));
    system("cls");
    cout << "Student Grades\n";
    cout << "-------------------------------------------------------\n";
    gradeAccessibleOutputStudent(dir, userNumber);
    cout << "\n\n";
    system("pause");
}

//Teacher Selection Menu
void oTeacherAuth() {
    SetConsoleTitle(TEXT("Teacher Menu"));
    system("cls");
    cout << "Teacher Menu\n";
    cout << "[1] Login\n";
    cout << "[2] Register\n";
    cout << "[3] Back\n";
    cout << "\nChoice>>> ";
    getline(cin, temp);
    choice = intValid(temp);
}

//Menu for Teacher Login
void oTeacherAuthLog() {
    int number;
    string password;

    SetConsoleTitle(TEXT("Student Login"));
    system("cls");
    cout << "Teacher Login\n\n";
    cout << "Teacher Number: ";
    getline(cin, temp);
    number = intValid(temp);
    cout << "Teacher Password: ";
    getline(cin, password);

    if (userExist(dir, number, password, "teacher")) { //If the user exist
        login = true;
        userNumber = number;
        cout << "Login Successful\n";
        system("pause");
    }
    else { //If the user does not exist
        cout << "Teacher Number/Password Incorrect\n";
        system("pause");
    }
}

//Menu for Teacher Registration
void oTeacherAuthReg() {
    int number;
    string fname, lname, password, cpassword;
    bool pass;

    SetConsoleTitle(TEXT("Teacher Register"));
    //Loop Until Valid Input For Teacher Number
    do {
        pass = false;
        system("cls");
        cout << "Teacher Register\n\n";
        cout << "Teacher Number: ";
        getline(cin, temp);
        if (!(number = intValid(temp) > 0)) {
            cout << "Invalid Teacher Number\n\n";
            system("pause");
        }
        else {
            pass = true;
            number = intValid(temp);
        }
    } while (!pass);

    //Loop Until Valid Input For Teacher First Name
    do {
        pass = false;
        system("cls");
        cout << "Teacher Register\n\n";
        cout << "Teacher Number: " << number << endl;
        cout << "Teacher First Name: ";
        getline(cin, fname);
        if (fname.empty() || checkWhiteSpace(fname) == fname.size()) {
            cout << "First Name Cannot be Empty\n\n";
            system("pause");
        }
        else if (checkAlpha(fname)) {
            cout << "First Name Should Only Contain AlphaCharacters\n\n";
            system("pause");
        }
        else {
            pass = true;
        }
    } while (!pass);

    //Loop Until Valid Input For Teacher Last Name
    do {
        pass = false;
        system("cls");
        cout << "Teacher Register\n\n";
        cout << "Teacher Number: " << number << endl;
        cout << "Teacher First Name: " << fname << endl;
        cout << "Teacher Last Name: ";
        getline(cin, lname);
        if (lname.empty() || checkWhiteSpace(lname) == lname.size()) {
            cout << "Last Name Cannot be Empty\n\n";
            system("pause");
        }
        else if (checkAlpha(lname)) {
            cout << "Last Name Should Only Contain AlphaCharacters\n\n";
            system("pause");
        }
        else {
            pass = true;
        }
    } while (!pass);

    //Loop Until Valid Input For Teacher Password
    //Should be At least 8 Characters
    do {
        pass = false;
        system("cls");
        cout << "Teacher Register\n\n";
        cout << "Teacher Number: " << number << endl;
        cout << "Teacher First Name: " << fname << endl;
        cout << "Teacher Last Name: " << lname << endl;
        cout << "Teacher Password: ";
        getline(cin, password);
        cout << "Confirm Teacher Password: ";
        getline(cin, cpassword);
        if (checkWhiteSpace(password) == password.size() || password.size() < 8 || password.empty() || (password.size() - checkWhiteSpace(password) < 8)) {
            cout << "Password Cannot Have Spaces and Should be at least 8 Character long\n\n";
            system("pause");
        } else if(!(password == cpassword)) {
            cout << "Password Does Not Match\n\n";
            system("pause");
        }
        else {
            pass = true;
        }

    } while (!pass);

    //Check if the Teacher Number Already Exist
    if (userExist(dir, number)) { //If the user does not exist
        //Register User
        addUser(dir, number, fname, lname, password, "teacher");
    } else {
        cout << "Teacher Number already exist\n\n";
        system("pause");
    }
}

//Menu for Login Teacher
void oTeacherMenu() {
    //Teacher Menu Output
    SetConsoleTitle(TEXT("Teacher Menu"));
    system("cls");
    cout << "Welcome Back Teacher, " << userFName << " " << userLName << endl;
    cout << "Teacher Menu\n";
    cout << "[1] Graded Informations\n";
    cout << "[2] Add Grade\n";
    cout << "[3] Update Grade\n";
    cout << "[4] Delete Grade\n";
    cout << "[5] Logout\n\n";
    cout << "Choice>>> ";
    getline(cin, temp);
    choice = intValid(temp);
}

//Menu selection for Grade Viewing for Teacher
void oTeacherGradeView() {
    //Teacher Grade View Selection Output
    system("cls");
    SetConsoleTitle(TEXT("Student Grade Viewer"));
    cout << "Student Grade Viewer Selection\n";
    cout << "[1] View All Students\n";
    cout << "[2] Specific Student\n";
    cout << "[3] Back\n";
    cout << "Choice>>> ";
    getline(cin, temp);
    choice = intValid(temp);
}

//Menu for Viewing all the grade for Teacher
void oTeacherGradeViewAll() {
    //Teacher View All Graded Student Output
    system("cls");
    SetConsoleTitle(TEXT("Student Grade Viewer"));
    cout << "-------------------------------------------------------\n";
    gradeAccessibleOutputAll(dir, userNumber);
    cout << "\n\n";
    system("pause");
}

//Menu for Viewing specific grades for Teacher
void oTeacherGradeViewSpec() {
    int studentNumber;

    //Teacher Add Grade Output
    system("cls");
    SetConsoleTitle(TEXT("Teacher Edit/Update Grade"));
    cout << "View Specific Student Grade\n\n";
    cout << "Student Number: ";
    getline(cin, temp);
    studentNumber = intValid(temp);
    if(!gradeAccessible(dir, studentNumber, userNumber)) {
        cout << "Student  Not Graded/Invalid";
        system("pause");
        return;
    }

    system("cls");
    cout << "-------------------------------------------------------\n";
    gradeAccessibleOutputSpecs(dir, studentNumber, userNumber);
    cout << "\n\n";
    system("pause");
}

//Menu for adding grades to Student for Teacher
void oTeacherAddGrade() {
    bool pass = false;
    int studentNumber, studentGrade;
    string subject, comment;

    //Teacher Add Grade Output
    SetConsoleTitle(TEXT("Teacher Add Grade"));

    //Loop Until Valid Input For Student Number
    do {
        pass = false;
        system("cls");
        cout << "Entry Grade for Student\n\n";
        cout << "Student Number: ";
        getline(cin, temp);
        studentNumber = intValid(temp);
        userInfo(dir, studentNumber);
        if(!(studentNumber > 0)) {
            cout << "Invalid Student Number\n\n";
            system("pause");
        } else if(userTypeRes == "teacher") {
            cout << "This is a Teacher\n\n";
            system("pause");
        }
        else if (userTypeRes == "student") {
            pass = true;
            studentNumber = intValid(temp);
        } else if(gradeAccessible(dir, studentNumber, userNumber)) {
            cout << "Student Already Graded. Do you wish to add another Grade?\nType Y for Yes and N for No\n";
            cout << "Choice>>> ";
            getline(cin, temp);
            if(temp == "Y" || temp == "y") {
                pass = true;
            } else if(temp == "N" || temp == "n") {
                system("pause");
                return;
            } else {
                cout << "Invalid Input\n\n";
                system("pause");
            }
        } else {
            cout << "Invalid Student Number\n\n";
            system("pause");
        }
    } while (!pass);

    //Loop Until Valid Input For Student Grade
    do {
        pass = false;
        system("cls");
        cout << "Entry Grade for Student\n\n";
        cout << "Student Number: " << studentNumber << endl;
        cout << "Student Grade: ";
        getline(cin, temp);
        studentGrade = intValid(temp);
        if(!(studentGrade > 0 && studentGrade < 101)) {
            cout << "Invalid Student Grade\n\n";
            system("pause");
        }
        else {
            pass = true;
            studentGrade = intValid(temp);
        }
    } while (!pass);

    //Loop Until Valid Input For Subject
    do {
        pass = false;
        system("cls");
        cout << "Entry Grade for Student\n\n";
        cout << "Student Number: " << studentNumber << endl;
        cout << "Student Grade: " << studentGrade << endl;
        cout << "Subject/Course Name: ";
        getline(cin, subject);
        if (subject.empty() || checkWhiteSpace(subject) == subject.size()) {
            cout << "Subject/Course Name Cannot be Empty\n\n";
            system("pause");
        }
        else if (checkAlpha(subject)) {
            cout << "Subject/Course Name Should Only Contain AlphaCharacters\n\n";
            system("pause");
        }
        else {
            pass = true;
        }
    } while (!pass);

    //Loop Until Valid Input For Comments
    do {
        pass = false;
        system("cls");
        cout << "Entry Grade for Student\n\n";
        cout << "Student Number: " << studentNumber << endl;
        cout << "Student Grade: " << studentGrade << endl;
        cout << "Subject/Course Name: " << subject << endl;
        cout << "Comment: ";
        getline(cin, comment);
        if (comment.empty() || checkWhiteSpace(comment) == comment.size()) {
            cout << "Comment Cannot be Empty.\n\n";
            system("pause");
        } else {
            pass = true;
        }
    } while (!pass);

    //Add Grade
    addGrades(dir, studentNumber, userNumber, studentGrade, subject, comment);

}

//Menu for updating or editing student grades for Teacher
void oTeacherUpdateGrade() {
    bool pass = false;
    int studentNumber, studentGrade, gradeCodeNum;
    string subject, comment;

    //Teacher Add Grade Output
    system("cls");
    SetConsoleTitle(TEXT("Teacher Edit/Update Grade"));
    cout << "Edit/Update Grades for Student\n\n";
    cout << "Student Number: ";
    getline(cin, temp);
    studentNumber = intValid(temp);
    if (!gradeAccessible(dir, studentNumber, userNumber)) {
        cout << "Student  Not Graded/Invalid";
        system("pause");
        return;
    }

    //Loop Until Valid Input For Grade Code
    do {
        pass = false;
        system("cls");
        cout << "Edit/Update Grades for Student\n\n";
        cout << "-----------------------Original-----------------------\n";
        gradeAccessibleOutput(dir);
        cout << "-------------------------------------------------------\n\n";
        cout << "Grade Code: ";
        getline(cin, temp);
        gradeCodeNum = intValid(temp);
        if(!gradeCode(dir, gradeCodeNum, studentNumber, userNumber)) {
            cout << "Invalid Grade Code\n\n";
            system("pause");
        }
        else {
            pass = true;
            studentGrade = intValid(temp);
        }
    } while (!pass);

    //Loop Until Valid Input For Student Grade
    do {
        pass = false;
        system("cls");
        cout << "Edit/Update Grades for Student\n\n";
        cout << "-----------------------Original-----------------------\n";
        gradeAccessibleOutputSpecsCode(dir, gradeCodeNum, userNumber);
        cout << "\n\n";
        cout << "Grade Code: " << gradeCodeNum << endl;
        cout << "Student Number: " << studentNumber << endl;
        cout << "Student Grade: ";
        getline(cin, temp);
        studentGrade = intValid(temp);
        if (!(studentGrade > 0 && studentGrade < 101)) {
            cout << "Invalid Student Grade\n\n";
            system("pause");
        }
        else {
            pass = true;
            studentGrade = intValid(temp);
        }
    } while (!pass);

    //Loop Until Valid Input For Subject
    do {
        pass = false;
        system("cls");
        cout << "Edit/Update Grades for Student\n\n";
        cout << "-----------------------Original-----------------------\n";
        gradeAccessibleOutputSpecsCode(dir, gradeCodeNum, userNumber);
        cout << "\n\n";
        cout << "Grade Code: " << gradeCodeNum << endl;
        cout << "Student Number: " << studentNumber << endl;
        cout << "Student Grade: " << studentGrade << endl;
        cout << "Subject/Course: ";
        getline(cin, subject);
        if (subject.empty() || checkWhiteSpace(subject) == subject.size()) {
            cout << "Subject/Course Name Cannot be Empty\n\n";
            system("pause");
        }
        else if (checkAlpha(subject)) {
            cout << "Subject/Course Name Should Only Contain AlphaCharacters\n\n";
            system("pause");
        }
        else {
            pass = true;
        }
    } while (!pass);

    //Loop Until Valid Input For Comments
    do {
        pass = false;
        system("cls");
        cout << "Edit/Update Grades for Student\n\n";
        cout << "-----------------------Original-----------------------\n";
        gradeAccessibleOutputSpecsCode(dir, gradeCodeNum, userNumber);
        cout << "\n\n";
        cout << "Grade Code: " << gradeCodeNum << endl;
        cout << "Student Number: " << studentNumber << endl;
        cout << "Student Grade: " << studentGrade << endl;
        cout << "Subject/Course: " << subject << endl;
        cout << "Comment: ";
        getline(cin, comment);
        if (comment.empty() || checkWhiteSpace(comment) == comment.size()) {
            cout << "Comment Cannot be Empty.\n\n";
            system("pause");
        }
        else {
            pass = true;
        }
    } while (!pass);

    updateGrades(dir, gradeCodeNum, studentGrade, subject, comment);
}

//Menu for deleting student grades for Teacher
void oTeacherDeleteGrade() {
    bool pass = false;
    int studentNumber, gradeCodeNum;
    string choice;

    //Teacher Add Grade Output
    system("cls");
    SetConsoleTitle(TEXT("Teacher Edit/Update Grade"));
    cout << "Edit/Update Grades for Student\n\n";
    cout << "Student Number: ";
    getline(cin, temp);
    studentNumber = intValid(temp);
    if (!gradeAccessible(dir, studentNumber, userNumber)) {
        cout << "Student  Not Graded/Invalid";
        system("pause");
        return;
    }

    //Loop Until Valid Input For Grade Code
    do {
        pass = false;
        system("cls");
        cout << "Delete Grades for Student\n\n";
        cout << "-------------------------------------------------------\n";
        gradeAccessibleOutputSpecs(dir, studentNumber, userNumber);
        cout << "-------------------------------------------------------\n\n";
        cout << "Grade Code: ";
        getline(cin, temp);
        gradeCodeNum = intValid(temp);
        if (!gradeCode(dir, gradeCodeNum, studentNumber, userNumber)) {
            cout << "Invalid Grade Code\n\n";
            system("pause");
        }
        else {
            pass = true;
        }
    } while (!pass);

    //Loop Until Valid Input For Choice
    do {
        pass = false;
        system("cls");
        cout << "Delete Grades for Student\n\n";
        cout << "-------------------------------------------------------\n";
        gradeAccessibleOutputSpecsCode(dir, gradeCodeNum, userNumber);
        cout << "\nEnter Y to delete\nEnter N to cancel\n\n";
        cout << "Choice>>> ";
        getline(cin, choice);
        if (choice == "Y" || choice == "y") {
            deleteGrades(dir, gradeCodeNum);
            pass = true;
        }
        else {
            system("pause");
            return;
        }
    } while (!pass);
}
/*End of Output Menu Functions*/
