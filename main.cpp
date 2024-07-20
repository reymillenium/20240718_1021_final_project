/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                 *
 *   Name: Reinier Garcia Ramos                                    *
 *                                                                 *
 *   Program Title: Final Project (Payroll Pro 2.0)                *
 *                                                                 *
 *   Purpose:                                                      *
 *   Allows the user to enter the Employees data, their payments,  *
 *   as well as printing the addition & average Payroll Reports    *
 *   for a specific employee and for the whole company as well.    *
 *                                                                 *
 *   More Details:                                                 *
 *   https://github.com/reymillenium/20240718_1021_final_project   *
 *                                                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 **/

#include <iostream>
#include <string>  // for string, to_string, etc
#include <cmath> // for sqrt, sin, pow
#include <climits> // For SHRT_MAX, SHRT_MIN, etc
#include <limits> // For SHRT_MAX, SHRT_MIN, etc
#include <iomanip> // for setprecision, setw, fixed
#include<array>  // for array
#include<random> // for random_device
#include <cstdlib> // For rand and srand
#include <ctime>   // For the time function
#include <sstream> // for stringstream (used in humanizeInteger, humanizeDouble, etc)
#include <vector> // to use vectors
#include <fstream> // For ifstream, ofstrea, fstream
#include <numeric> // For accumulate, transform_reduce, inner_product (in the vectors)
#include <algorithm> // For max_element, min_element, find, transform (to use in vectors), or for max(), reverse, count_if
#include <regex> // For regex, regex_match
#include <cstring> // For strrev
#include <filesystem>

using namespace std;

/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                         *
 *                   GLOBAL CONSTANTS                      *
 *                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 **/

constexpr int MAX_REG_HOURS = 40;
constexpr int MAX_HOURS_WORKED = 50;
// constexpr int MAX_OT_HOURS = MAX_HOURS_WORKED - MAX_REG_HOURS;
constexpr double MIN_HOURLY_WAGE = 10.00;
constexpr double MAX_HOURLY_WAGE = 30.00;
constexpr double FICA_RATE = .20;
constexpr double SS_MED_RATE = .0765;
constexpr double OT_MULT = 1.5;

constexpr char ADD_EMPLOYEE_OPTION = 'A';
constexpr char DELETE_EMPLOYEE_OPTION = 'B';
constexpr char SHOW_EMPLOYEES_OPTION = 'C';
constexpr char ADD_PAYMENT_OPTION = 'D';
constexpr char GENERATE_AND_PRINT_CURRENT_EPR_OPTION = 'E';
constexpr char GENERATE_AND_PRINT_COMPANY_PR_OPTION = 'F';
constexpr char QUITTING_OPTION = 'X';


/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                         *
 *             UTILITY FUNCTIONS PROTOTYPES                *
 *                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 **/


// Prints a given value, of almost any kind, once in the terminal
template<typename T>
void print(const T &);

// Prints a given value, of almost any kind, N given times in the terminal
template<typename T>
void printNTimes(const T &, int);

// Prints a given value, of almost any kind, N given times in the terminal, and then breaks the line
template<typename T>
void printNTimesAndBreak(const T &, int);

// Prints a given value, of almost any kind, once in the terminal, and then breaks the line
template<typename T>
void printLine(const T &);

// Prints a given value, of almost any kind, N given times in the terminal, and then breaks the line
template<typename T>
void printLineNTimes(const T &, int);

// Gets several types of values from the console (except strings with spaces, objects, etc, among others), as a response of a given question
template<typename T>
auto getValue(const string &) -> T;

// Determines if a given string is a valid integer, using a regular expression
bool isInteger(const string &input);

// Determines if a given string is a valid floating point number, using a regular expression
bool isFloatingPoint(const string &input);

// Receives and validates an integer number from the console
int getInteger(const string &, int, int, bool = false, const string & = "Invalid input. Please try again.", const vector<int> & = {});

// Receives and validates a double number (or the equivalent of an integer) from the console
double getDouble(const string &, double, double, bool = false, const string & = "Invalid input. Please try again.", const vector<double> & = {});

// Determines if a given string is a single valid char
bool containsSingleChar(const string &input);

// Receives and validates a char from the console
char getAlphaChar(const string &, const string & = "Invalid input. Please try again.");

// Gets a string with or without spaces, from the terminal, as a response of a given question
string getStringFromMessage(const string &);

// Gets a string with or without spaces, from the terminal
string getString();

// Formats a given negative or positive int by inserting a comma every 3 digits of its equivalent string, to make it more readable, by US standards
string humanizeInteger(long long int);

// Formats a given negative or positive double by inserting a comma every 3 digits of its equivalent string, to make it more readable, by US standards
string humanizeDouble(long double, int = 2);

// Formats a given positive int by inserting a comma every 3 digits of its equivalent string, to make it more readable, by US standards
string humanizeUnsignedInteger(unsigned long long int);

// Formats a given positive double by inserting a comma every 3 digits of its equivalent string, to make it more readable, by US standards
string humanizeUnsignedDouble(long double, int = 2);

// Formats a given int by inserting a comma every 3 digits of its equivalent string, to make it more readable, and adds a customizable currency symbol
string monetizeInteger(unsigned long int, bool = true, const string & = "$");

// Formats a given double by inserting a comma every 3 digits of its equivalent string, to make it more readable, and adds a customizable currency symbol
string monetizeDouble(long double, int = 2, bool = true, const string & = "$");

// Rounds up a given double number, with a given amount of decimal places
double roundUp(double, int);

// Calculates the Recursive Sum of a given int number. Example: recursiveSum(5) == 5 + 4 + 3 + 2 + 1 = 15
int recursiveSum(unsigned long long int);

// Returns the string gramatic representation of an ordinal number, from a given int number
string ordinalFromNumber(long long int);

// Determines is a given int number is prime or not
bool isPrime(unsigned long long int);

// Calculates the sum of all the elements inside an array of integers
int simpleSumOfIntegersInArray(const int [], int);

// Calculates the sum of all the elements inside an array of integers
int neatSumOfIntegersInArray(const int [], int);

// Calculates the sum of all the elements inside an array of integers
template<int N>
int sumOfIntegersWithRangeOverReference(int (&)[N]);

// Calculates the sum of all the elements inside an array of doubles
double simpleSumOfDoublesInArray(const double [], int);

// Calculates the sum of all the elements inside an array of doubles
double neatSumOfDoublesInArray(const double [], int);

// Calculates the sum of all the elements inside an array of doubles
template<int N>
double templateSumOfDoublesInArray(double (&)[N]);

template<int N, int M>
double templateSumOfDoublesInTwoDimensionsArray(double (&)[N][M]);

// Calculates the average among all the numbers inside an array of integers
double simpleAverageAmongIntegersInArray(const int [], int);

// Calculates the average among all the numbers inside an array of integers
double neatAverageAmongIntegersInArray(const int [], int);

// Calculates the average among all the numbers inside an array of integers
template<int N>
int templateAverageAmongIntegersInArray(int (&)[N]);

// Calculates the average among all the numbers inside an array of doubles
double simpleAverageAmongDoublesInArray(const double [], int);

// Calculates the average among all the numbers inside an array of doubles
double neatAverageAmongDoublesInArray(const double [], int);

template<int N>
double templateAverageAmongDoublesInArray(double (&)[N]);

template<int N, int M>
double templateAverageAmongDoublesInTwoDimensionsArray(double (&)[N][M]);

// Determines the highest number among an array of integers
int simpleHighestIntegerInArray(const int [], int);

// Determines the lower number among an array of integers
int simpleLowerIntegerInArray(const int [], int);

// Determines the lower number among an array of doubles
double simpleLowerDoubleInArray(const double [], int);

// Determines the higher number among an array of doubles
double simpleHigherDoubleInArray(const double [], int);

// Determines the lower number among a two-dimensional array of doubles
template<int N, int M>
double templateLowerDoubleInTwoDimensionsArray(double (&)[N][M]);

// Determines the higher number among a two-dimensional array of doubles
template<int N, int M>
double templateHigherDoubleInTwoDimensionsArray(double (&)[N][M]);

// Determines if the given value is lower than the also given minimum value
bool isInvalid(long double value, double);

// Prints on the terminal a text if the given value is lower than the also given minimum value
void scoldUserIfInvalidInput(long double, double);

// Function to validate the DateTime(YYYY-MM-DD HH:MM:SS)
bool validDateTime(const string &);

// Function to extract all the components of a DateTime(YYYY-MM-DD HH:MM:SS)
vector<string> getComponentsFromDateTime(const string &);

// Function to extract the year from a DateTime(YYYY-MM-DD HH:MM:SS)
string getYearFromDateTime(const string &);

// Function to extract the month from a DateTime(YYYY-MM-DD HH:MM:SS)
string getMonthFromDateTime(const string &);

// Function to extract the day from a DateTime(YYYY-MM-DD HH:MM:SS)
string getDayFromDateTime(const string &);

// Function to extract the hours from a DateTime(YYYY-MM-DD HH:MM:SS)
string getHoursFromDateTime(const string &);

// Function to extract the minutes from a DateTime(YYYY-MM-DD HH:MM:SS)
string getMinutesFromDateTime(const string &);

// Function to extract the seconds from a DateTime(YYYY-MM-DD HH:MM:SS)
string getSecondsFromDateTime(const string &);

// Function to convert from a military Time (24 hours format) to civilian Time (12 hours format with meridian)
string convertTimeToCivilian(int, int);

// Detects if a given filename exist or not on the root of the executable file
bool fileExist(const string &);

// Gets all the non-empty lines of text inside a given file name
vector<string> getLinesFromFile(const string &);

// Either creates a .txt file and adds text to it, or adds to an existent one
void addTextToFile(const string &);

// Converts a giving string to lowercase
void strToLowerCase(string &input);

// Converts a giving string to uppercase
void strToUpperCase(string &input);

// Converts a string to lowercase and returns it
string getLowerCase(string input);

// Converts a string to uppercase and returns it
string getUpperCase(string input);

// Generates a Universally Unique IDentifier (the usual 36-character alphanumeric string. UUID style) as a string. Format: bdc0a2fb-d39e-0242-9a0a-4e760153f18d
string getUUID();


/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                         *
 *                STRUCTURE DEFINITIONS                    *
 *                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 **/


struct Employee {
    string firstName;
    string lastName;
    string id;
    double regRate {0.0};

    Employee() = default;

    [[nodiscard]] string fullName() const { return firstName + " " + lastName; }
};

struct Payment {
    string employeeId;
    double regHours {0.0};
    double otHours {0.0};
    double regRate {0.0};

    Payment() = default;

    [[nodiscard]] double hoursWorked() const { return regHours + otHours; }
    [[nodiscard]] double otRate() const { return regRate * OT_MULT; }
    [[nodiscard]] double regPay() const { return regHours * regRate; }
    [[nodiscard]] double otPay() const { return otHours * otRate(); }
    [[nodiscard]] double totalPay() const { return regPay() + otPay(); }
    [[nodiscard]] double fica() const { return totalPay() * FICA_RATE; }
    [[nodiscard]] double socSec() const { return totalPay() * SS_MED_RATE; }
    [[nodiscard]] double totDeductions() const { return fica() + socSec(); }
    [[nodiscard]] double netPay() const { return totalPay() - totDeductions(); }
};

struct PayrollReport {
    int paymentsAmount {0};
    double regHours {0.0};
    double otHours {0.0};
    double regPay {0.0};
    double otPay {0.0};
    double fica {0.0};
    double socSec {0.0};

    PayrollReport() = default;

    [[nodiscard]] double totalPay() const { return regPay + otPay; }
    [[nodiscard]] double totDeductions() const { return fica + socSec; }
    [[nodiscard]] double netPay() const { return totalPay() - totDeductions(); }
};

// Logical Interpretation: An EmployeePayrollReport is a PayrollReport, plus adding an employee's id
struct EmployeePayrollReport : PayrollReport {
    // The Employee could be deleted from the system, but we still have its data (I'm not using an Employee,
    // to avoid theorically a DB persistence validation over the data layer [obviously none-existent, as we are not even using a DB in the first place])
    // But still in a real life scenario it would the best approach to avoid many issues, using an instance/object of a Class instead of structure variables
    // ...and we just need a few fields anyway, so it will remain denormalized with these 3 elements, instead of using an Employee structure variable
    string firstName;
    string lastName;
    string employeeId;

    EmployeePayrollReport() = default;

    [[nodiscard]] string fullName() const { return firstName + " " + lastName; }
};


/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                         *
 *           CUSTOM MADE FUNCTIONS PROTOTYPES              *
 *                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 **/


// Shows the program's welcoming message
void showProgramWelcome();

// Displays the menu to the user
void displayMenu(bool, bool);

// Processes the selection made by the user from the menu
void processMenuSelection(char, vector<Employee> &, vector<Payment> &);

// Validates and returns if the given selection is among the allowed selections from the Menu
bool isValidMenuSelection(char input, const vector<char> &);

// Adds an Employee structure variable to the reference of a given vector of Employees
void addEmployee(vector<Employee> &);

// Removes an Employee structure variable from the reference of a given vector of Employees, by its given id
void deleteCurrentEmployee(vector<Employee> &);

// Shows the table with all the current employees
void showCurrentEmployeesTable(const vector<Employee> &);

// Adds a Payment structure variable, associated to a specific Employee, to the reference of a given vector of Payments
void addPayment(vector<Payment> &, const vector<Employee> &);

void showEmployeesTable(const vector<Employee> &);

// Prints an appropiate length "line" conformed by dashes, as part of a good looking table
void renderLineUnderTableRow(int);

// Adds a Payment structure variable to the reference of a given vector of Payments
void addPaymentToEmployee(vector<Payment> &, const Employee &);

// Gets the option selected by the user, from the menu's options
char getMenuSelection(bool, bool);

// Prints on the terminal a PayrollReport for a specific Employee
void generateAndPrintCurrentEmployeePayrollReports(vector<Payment> &, const vector<Employee> &);

// Prints on the terminal both PayrollReports, addition & average, for the whole company
void generateAndPrintCompanyPayrollReports(const vector<Payment> &);

// Determines if a given emloyeeID belongs to the current ones
bool existEmployee(const vector<Employee> &, const string &);

// Determines if a given emloyee's id has associated at least one payment
bool employeeHasPayments(const vector<Payment> &, const string &);

// Retrieves an Employee structure variable by a given employee's id
Employee getEmployeById(const vector<Employee> &, const string &);

// Deletes an Employee structure variable by a given employee's id
void deleteEmployeById(vector<Employee> &, const string &);

// Generates a EmployeePayrollReport with the addition of all the Payment structure variables related to a given employee
EmployeePayrollReport createAdditionEmployeePayrollReport(const vector<Payment> &, const Employee &);

// Generates a PayrollReport with the addition of all the Payment structure variables's data of the whole company across the time
PayrollReport createAdditionPayrollReport(const vector<Payment> &);

// Generates a EmployeePayrollReport with the average of all the Payment structure variables related to a given employee
EmployeePayrollReport createAverageEmployeePayrollReport(const vector<Payment> &, const Employee &);

// Generates a PayrollReport with the average of all the Payment structure variables's data of the whole company across the time
PayrollReport createAveragePayrollReport(const vector<Payment> &);

// Prints on the console both, the addition & average PayrollReports of the company
void printCompanyPayrollReports(const PayrollReport &, const PayrollReport &);

// Prints on the console both, the addition & average given EmployeePayrollReports
void printEmployeePayrollReports(const EmployeePayrollReport &, const EmployeePayrollReport &);

// Prints either a EmployeePayrollReport or a PayrollReport structure variable, with addition and average data,
// as we pass as argument a father struct PayrollReport variable, and from the received parameter we won't use the employee's id anyway at this point (either done before or not needed)
void printPayrollReportsTable(const PayrollReport &, const PayrollReport &);

// Prints on the console goodbyes to the user
void sayGoodbyeToTheUser();

/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                         *
 *                    MAIN FUNCTION                        *
 *                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 **/


int main() {
    vector<Employee> employees; // Our current employees
    vector<Payment> payments; // All the payments performed by the company to the employees. That's all we need.
    char menuSelection = ADD_EMPLOYEE_OPTION;

    // Shows once the program's welcoming message
    showProgramWelcome();

    do {
        // Adjusts accordingly the boolean variables
        const bool hasEmployees = !employees.empty();
        const bool hasPayments = !payments.empty();

        // Displays the available options to the user
        displayMenu(hasEmployees, hasPayments);

        // Gets the selected menu option from the user
        menuSelection = getMenuSelection(hasEmployees, hasPayments);

        // Processes accordingly the selection made by the user
        processMenuSelection(menuSelection, employees, payments);
    } while (menuSelection != QUITTING_OPTION);

    return 0;
}


/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                         *
 *            UTILITY FUNCTIONS DEFINITIONS                *
 *                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 **/


// Prints a given value, of almost any kind, once in the terminal
template<typename T>
void print(const T &item) {
    cout << item;
}

// Prints a given value, of almost any kind, N given times in the terminal
template<typename T>
void printNTimes(const T &item, const int times) {
    for (int i = 0; i < times; i += 1) {
        print(item);
    }
}

// Prints a given value, of almost any kind, N given times in the terminal, and then breaks the line
template<typename T>
void printNTimesAndBreak(const T &item, const int times) {
    for (int i = 0; i < times; i += 1) {
        print(item);
    }
    printLine("");
}

// Prints a given value, of almost any kind, once in the terminal, and then breaks the line
template<typename T>
void printLine(const T &item) {
    cout << item << endl;
}

// Prints a given value, of almost any kind, N given times in the terminal, and then breaks the line
template<typename T>
void printLineNTimes(const T &item, const int times) {
    for (int i = 0; i < times; i += 1) {
        printLine(item);
    }
}

// Gets several types of values from the console (except strings with spaces, objects, etc, among others), as a response of a given question
template<typename T>
auto getValue(const string &message) -> T {
    cout << message;
    T value;
    cin >> value;
    cin.ignore();
    return value;
}

// Determines if a given string is a valid integer, using a regular expression
bool isInteger(const string &input) {
    const regex pattern("^[+-]?[0-9]+$");
    return regex_match(input, pattern);
}

// Determines if a given string is a valid floating point number, using a regular expression
bool isFloatingPoint(const string &input) {
    // const regex pattern("^[+-]?[0-9]+(\.[0-9]+)?([eE][+-]?[0-9]+)?$");
    const regex pattern(R"(^[+-]?[0-9]+(.[0-9]+)?([eE][+-]?[0-9]+)?$)");
    return regex_match(input, pattern);
}

// Receives and validates an integer number from the console
int getInteger(const string &message, const int minValue, const int maxValue, const bool showRange, const string &errorMessage, const vector<int> &sentinelValues) {
    string numberAsString; // Value typed by the user, that can be an integer or not
    int number = 0; // Integer convertion (if possible) of the value typed by the user
    bool keepAsking = true; // If we must keep asking for a value to the user, until receiving an integer

    do {
        cout << message << (showRange ? (" (" + to_string(minValue) + " - " + to_string(maxValue) + ")") : "") << ": ";
        getline(cin, numberAsString);

        if (!isInteger(numberAsString)) {
            cout << "That's not an integer number. Try again." << endl;
            continue; // There is no point in keep validating any further, as it's not even an integer
        }

        number = stoi(numberAsString); // When we reach this point, that means we have a proper integer
        const bool invalidInput = number < minValue || maxValue < number; // If the input is valid, based only in minimum & maximum possible values
        // If the typed number is not among the given sentinel values (breaking values)
        const bool numberIsNotSentinel = count(sentinelValues.begin(), sentinelValues.end(), number) == 0;
        keepAsking = invalidInput && numberIsNotSentinel;
        if (keepAsking) cout << errorMessage << endl;
    } while (keepAsking);

    return number;
}

// Receives and validates a double number (or the equivalent of an integer) from the console
double getDouble(const string &message, const double minValue, const double maxValue, const bool showRange, const string &errorMessage, const vector<double> &sentinelValues) {
    string numberAsString; // Value typed by the user, that can be a valid (integer or floating point) number or not
    double number = 0; // Double convertion (if possible) of the value typed by the user
    bool keepAsking = true; // If we must keep asking for a value to the user, until receiving either an integer or a flating point number

    do {
        cout << fixed << setprecision(2) << message;
        if (showRange)
            cout << " (" << minValue << " - " << maxValue << ")";
        cout << ": ";
        getline(cin, numberAsString);

        if (const bool isIntegerOrFloatingPointNumber = isInteger(numberAsString) || isFloatingPoint(numberAsString); !isIntegerOrFloatingPointNumber) {
            cout << "That's not an integer number nor a valid floating point number. Try again." << endl;
            continue; // There is no point in keep validating any further, as it's not even a valid integer nor a floating point number
        }

        number = stod(numberAsString); // When we reach this point, that means we have either a proper integer or a floating point number
        const bool invalidInput = number < minValue || maxValue < number; // If the input is valid, based only in minimum & maximum possible values
        // If the typed number is not among the given sentinel values (breaking values)
        const bool numberIsNotSentinel = count(sentinelValues.begin(), sentinelValues.end(), number) == 0;
        keepAsking = invalidInput && numberIsNotSentinel;
        if (keepAsking) cout << errorMessage << endl;
    } while (keepAsking);

    return number;
}

bool containsSingleChar(const string &input) {
    if (input.empty() || input.length() > 1) return false;
    return true;
}

char getAlphaChar(const string &message, const string &errorMessage) {
    string characterAsString; // Value typed by the user, that can be a char or not
    char character = 'A'; // Char convertion (if possible) of the value typed by the user
    bool isNotAlphaCharacter = true; // If the character is alphabetic

    do {
        cout << message << ": ";
        getline(cin, characterAsString);

        if (!containsSingleChar(characterAsString)) {
            cout << "That's not a single character. Try again." << endl;
            continue; // There is no point in keep validating any further, as it's not even a valid char
        }

        // const char *characterPointer = characterAsString.data(); // Only valid on C++ 17 or newer
        const char *characterPointer = characterAsString.c_str();
        character = characterPointer[0];

        isNotAlphaCharacter = !isalpha(character);
        if (isNotAlphaCharacter) cout << errorMessage << endl;
    } while (isNotAlphaCharacter);

    return character;
}

// Gets a string with or without spaces, from the terminal, as a response of a given question
string getStringFromMessage(const string &message) {
    cout << message;
    string value;
    getline(cin, value);
    return value;
}

// Gets a string with or without spaces, from the terminal
string getString() {
    string value;
    getline(cin, value);
    return value;
}

// Formats a given negative or positive int by inserting a comma every 3 digits of its equivalent string, to make it more readable, by US standards
string humanizeInteger(const long long int integerValue) {
    const bool isNegative = integerValue < 0;
    string integerAsString = to_string(integerValue);
    const int initialIndex = static_cast<int>(integerAsString.length() - 3);
    // We insert commas into the string every three digits, fromm right to left.
    for (int j = initialIndex; j > (isNegative ? 1 : 0); j -= 3) {
        integerAsString.insert(j, ",");
    }
    return integerAsString;
}

// Formats a given negative or positive double by inserting a comma every 3 digits of its equivalent string, to make it more readable, by US standards
string humanizeDouble(const long double doubleValue, const int precision) {
    const auto integerValue = static_cast<long long int>(doubleValue);
    const long double decimals = doubleValue - integerValue;
    const bool hasNegativeDecimals = decimals < 0;

    // Extracts into a string the decimal part, rounded to two significant digits
    stringstream stream;
    // Places the decimals into the stream, rounded to two significant digits (by default)
    stream << fixed << setprecision(precision) << decimals;
    // Extracts the decimals from the stream, as a string, still rounded to two significant digits (by default)
    const string decimalsAsString = stream.str(); // It still includeds the zero & the dot. Ex: 0.34, -0.34 (the zero, and possibly also a negative sign, must be removed next)
    return humanizeInteger(integerValue) + decimalsAsString.substr(hasNegativeDecimals ? 2 : 1, precision + 1);
}

// Formats a given positive int by inserting a comma every 3 digits of its equivalent string, to make it more readable, by US standards
string humanizeUnsignedInteger(const unsigned long long int integerValue) {
    string integerAsString = to_string(integerValue);
    const int initialIndex = static_cast<int>(integerAsString.length() - 3);
    // We insert commas into the string every three digits, fromm right to left.
    for (int j = initialIndex; j > 0; j -= 3) {
        integerAsString.insert(j, ",");
    }
    return integerAsString;
}

// Formats a given positive double by inserting a comma every 3 digits of its equivalent string, to make it more readable, by US standards
string humanizeUnsignedDouble(const long double doubleValue, const int precision) {
    const auto integerValue = static_cast<unsigned long long int>(doubleValue);
    const long double decimals = doubleValue - integerValue;
    // Extracts into a string the decimal part, rounded to two significant digits
    stringstream stream;
    // Places the decimals into the stream, rounded to two significant digits (by default)
    stream << fixed << setprecision(precision) << decimals;
    // Extracts the decimals from the stream, as a string, still rounded to two significant digits (by default)
    const string decimalsAsString = stream.str(); // It still includeds the "0" & the dot ("."). Ex: 0.34 (so the "0" must be removed next)
    return humanizeUnsignedInteger(integerValue) + decimalsAsString.substr(1, precision + 1);
}

// Formats a given int by inserting a comma every 3 digits of its equivalent string, to make it more readable, and adds a customizable currency symbol
string monetizeInteger(const unsigned long int integerValue, const bool prepend, const string &symbol) {
    return static_cast<string>(prepend ? (symbol + " ") : "") + humanizeUnsignedInteger(integerValue) + static_cast<string>(prepend ? "" : " " + symbol);
}

// Formats a given double by inserting a comma every 3 digits of its equivalent string, to make it more readable, and adds a customizable currency symbol
string monetizeDouble(const long double doubleValue, const int precision, const bool prepend, const string &symbol) {
    return static_cast<string>(prepend ? (symbol + " ") : "") + humanizeUnsignedDouble(doubleValue, precision) + static_cast<string>(prepend ? "" : " " + symbol);
}

// Rounds up a given double number, with a given amount of decimal places
double roundUp(const double value, const int decimalPlaces) {
    const double multiplier = pow(10.0, decimalPlaces);
    return ceil(value * multiplier) / multiplier;
}

// Calculates the Recursive Sum of a given int number. Example: recursiveSum(5) == 5 + 4 + 3 + 2 + 1 = 15
int recursiveSum(const unsigned long long int number) {
    if (number != 0)
        return number + recursiveSum(number - 1);
    return 0;
}

// Returns the string gramatic representation of an ordinal number, from a given int number
string ordinalFromNumber(const long long int number) {
    const long long int lastDigit = number % 10;
    string additive = "th";

    switch (number) {
        case 11:
        case 12:
        case 13:
            break;
        default:
            switch (lastDigit) {
                case 1:
                    additive = "st";
                    break;
                case 2:
                    additive = "nd";
                    break;
                case 3:
                    additive = "rd";
                    break;
                default: ;
            }
    }

    return to_string(number) + additive;
}

// Determines is a given int number is prime or not
bool isPrime(const unsigned long long int value) {
    if (value <= 1)
        return false;
    for (unsigned long long int i = 2; i <= value / 2; i++) {
        if (value % i == 0)
            return false;
    }
    return true;
}

// Calculates the sum of all the elements inside an array of integers
int simpleSumOfIntegersInArray(const int integersArray[], const int length) {
    int sum = 0;
    for (int i = 0; i < length; i++) {
        sum += integersArray[i];
    }
    return sum;
}

// Calculates the sum of all the elements inside an array of integers
int neatSumOfIntegersInArray(const int integersArray[], const int length) {
    return accumulate(integersArray, integersArray + length, 0);
}

// Calculates the sum of all the elements inside an array of integers
template<int N>
int sumOfIntegersWithRangeOverReference(int (&integersArray)[N]) {
    int sum = 0;
    for (auto number: integersArray) {
        sum += number;
    }
    return sum;
}

// Calculates the sum of all the elements inside an array of doubles
double simpleSumOfDoublesInArray(const double doublesArray[], const int length) {
    double sum = 0.0;
    for (int i = 0; i < length; i++) {
        sum += doublesArray[i];
    }
    return sum;
}

// Calculates the sum of all the elements inside an array of doubles
double neatSumOfDoublesInArray(const double doublesArray[], const int length) {
    return accumulate(doublesArray, doublesArray + length, 0.0);
}

// Calculates the sum of all the elements inside an array of doubles
template<int N>
double templateSumOfDoublesInArray(double (&doublesArray)[N]) {
    double sum = 0.0;
    for (auto number: doublesArray) {
        sum += number;
    }
    return sum;
}

// Calculates the sum of all the elements inside a two-dimensional array of doubles
template<int N, int M>
double templateSumOfDoublesInTwoDimensionsArray(double (&twoDimensionsDoublesArray)[N][M]) {
    double sum = 0.0;

    for (auto &row: twoDimensionsDoublesArray) {
        for (auto &number: row) {
            sum += number;
        }
    }

    return sum;
}

// Calculates the average among all the numbers inside an array of integers
double simpleAverageAmongIntegersInArray(const int array[], const int length) {
    return simpleSumOfIntegersInArray(array, length) / static_cast<double>(length);
}

// Calculates the average among all the numbers inside an array of integers
double neatAverageAmongIntegersInArray(const int array[], const int length) {
    return neatSumOfIntegersInArray(array, length) / static_cast<double>(length);
}

// Calculates the average among all the numbers inside an array of integers
template<int N>
int templateAverageAmongIntegersInArray(int (&integersArray)[N]) {
    const int length = sizeof(integersArray) / sizeof(integersArray[0]);
    return sumOfIntegersWithRangeOverReference(integersArray) / length;
}

// Calculates the average among all the numbers inside an array of doubles
double simpleAverageAmongDoublesInArray(const double array[], const int length) {
    return simpleSumOfDoublesInArray(array, length) / length;
}

// Calculates the average among all the numbers inside an array of doubles
double neatAverageAmongDoublesInArray(const double array[], const int length) {
    return neatSumOfDoublesInArray(array, length) / length;
}

// Calculates the average among all the numbers inside an array of doubles
template<int N>
double templateAverageAmongDoublesInArray(double (&doublesArray)[N]) {
    const int length = sizeof(doublesArray) / sizeof(doublesArray[0]);
    return sumOfDoublesWithRangeOverReference(doublesArray) / length;
}

// Calculates the average among all the numbers inside an array of doubles
template<int N, int M>
double templateAverageAmongDoublesInTwoDimensionsArray(double (&doublesTwoDimensionalArray)[N][M]) {
    const int rows = sizeof(doublesTwoDimensionalArray) / sizeof(doublesTwoDimensionalArray[0]);
    const int columns = sizeof(doublesTwoDimensionalArray[0]) / sizeof(doublesTwoDimensionalArray[0][0]);

    return templateSumOfDoublesInTwoDimensionsArray(doublesTwoDimensionalArray) / (rows * columns);
}

// Determines the highest number among an array of integers
int simpleHighestIntegerInArray(const int integersArray[], const int length) {
    int highestInteger = integersArray[0];
    if (length == 1) return highestInteger;

    for (int i = 1; i < length; i++) {
        if (integersArray[i] > highestInteger)
            highestInteger = integersArray[i];
    }

    return highestInteger;
}

// Determines the lower number among an array of integers
int simpleLowerIntegerInArray(const int integersArray[], const int length) {
    int lower = integersArray[0];
    if (length == 1) return lower;

    for (int i = 1; i < length; i++) {
        if (integersArray[i] < lower)
            lower = integersArray[i];
    }

    return lower;
}

// Determines the lower number among an array of doubles
double simpleLowerDoubleInArray(const double doublesArray[], const int length) {
    double lower = doublesArray[0];
    if (length == 1) return lower;

    for (int i = 1; i < length; i++) {
        if (doublesArray[i] < lower)
            lower = doublesArray[i];
    }

    return lower;
}

// Determines the higher number among an array of doubles
double simpleHigherDoubleInArray(const double doublesArray[], const int length) {
    double higher = doublesArray[0];
    if (length == 1) return higher;

    for (int i = 1; i < length; i++) {
        if (doublesArray[i] > higher)
            higher = doublesArray[i];
    }

    return higher;
}

// Determines the lower number among a two-dimensional array of doubles
template<int N, int M>
double templateLowerDoubleInTwoDimensionsArray(double (&twoDimensionsDoublesArray)[N][M]) {
    const int rows = sizeof(twoDimensionsDoublesArray) / sizeof(twoDimensionsDoublesArray[0]);
    const int columns = sizeof(twoDimensionsDoublesArray[0]) / sizeof(twoDimensionsDoublesArray[0][0]);
    double lower = twoDimensionsDoublesArray[0][0];

    for (int row = 0; row < rows; row++) {
        for (int col = (row == 0 ? 1 : 0); col < columns; col++) {
            if (const double number = twoDimensionsDoublesArray[row][col]; number < lower)
                lower = number;
        }
    }

    return lower;
}

// Determines the higher number among a two-dimensional array of doubles
template<int N, int M>
double templateHigherDoubleInTwoDimensionsArray(double (&twoDimensionsDoublesArray)[N][M]) {
    const int rows = sizeof(twoDimensionsDoublesArray) / sizeof(twoDimensionsDoublesArray[0]);
    const int columns = sizeof(twoDimensionsDoublesArray[0]) / sizeof(twoDimensionsDoublesArray[0][0]);
    double higher = twoDimensionsDoublesArray[0][0];

    for (int row = 0; row < rows; row++) {
        for (int col = (row == 0 ? 1 : 0); col < columns; col++) {
            if (const double number = twoDimensionsDoublesArray[row][col]; number > higher)
                higher = number;
        }
    }

    return higher;
}

// Determines if the given value is lower than the also given minimum value
bool isInvalid(const long double value, const double minValue) {
    return value < minValue;
}

// Prints on the terminal a text if the given value is lower than the also given minimum value
void scoldUserIfInvalidInput(const long double value, const double minValue) {
    if (isInvalid(value, minValue))
        cout << "You must type a number greater or equal than " << minValue << ". Try again!" << endl;
}

// Function to validate the DateTime(YYYY-MM-DD HH:MM:SS)
bool validDateTime(const string &datetimeAsString) {
    if (datetimeAsString.empty()) return false;

    // RegExp to check a valid DateTime(YYYY-MM-DD HH:MM:SS).
    const regex pattern("^([0-9]{4})-(01|02|03|04|05|06|07|08|09|10|11|12)-([0-2][0-9]|30|31)"
        "\\s([0-1][0-9]|[2][0-3]):([0-5][0-9]):([0-5][0-9])"
        "$");

    return regex_match(datetimeAsString, pattern);
}

// Function to extract all the components of a DateTime(YYYY-MM-DD HH:MM:SS)
vector<string> getComponentsFromDateTime(const string &datetimeAsString) {
    vector<string> dataComponents;

    // RegExp to check a valid DateTime(YYYY-MM-DD HH:MM:SS).
    const regex pattern("^([0-9]{4})-(01|02|03|04|05|06|07|08|09|10|11|12)-([0-2][0-9]|30|31)"
        "\\s([0-1][0-9]|[2][0-3]):([0-5][0-9]):([0-5][0-9])"
        "$");
    smatch matches;

    if (regex_search(datetimeAsString.begin(), datetimeAsString.end(), matches, pattern)) {
        for (string match: matches) {
            dataComponents.push_back(match);
        }
    }

    return dataComponents;
}

// Function to extract the year from a DateTime(YYYY-MM-DD HH:MM:SS)
string getYearFromDateTime(const string &datetimeAsString) {
    vector<string> dateTimeComponents = getComponentsFromDateTime(datetimeAsString);
    return dateTimeComponents.size() > 1 ? dateTimeComponents[1] : ""; // YYYY
}

// Function to extract the month from a DateTime(YYYY-MM-DD HH:MM:SS)
string getMonthFromDateTime(const string &datetimeAsString) {
    vector<string> dateTimeComponents = getComponentsFromDateTime(datetimeAsString);
    return dateTimeComponents.size() > 1 ? dateTimeComponents[2] : ""; // MM
}

// Function to extract the day from a DateTime(YYYY-MM-DD HH:MM:SS)
string getDayFromDateTime(const string &datetimeAsString) {
    vector<string> dateTimeComponents = getComponentsFromDateTime(datetimeAsString);
    return dateTimeComponents.size() > 1 ? dateTimeComponents[3] : ""; // DD
}

// Function to extract the hours from a DateTime(YYYY-MM-DD HH:MM:SS)
string getHoursFromDateTime(const string &datetimeAsString) {
    vector<string> dateTimeComponents = getComponentsFromDateTime(datetimeAsString);
    return dateTimeComponents.size() > 1 ? dateTimeComponents[4] : ""; // HH
}

// Function to extract the minutes from a DateTime(YYYY-MM-DD HH:MM:SS)
string getMinutesFromDateTime(const string &datetimeAsString) {
    vector<string> dateTimeComponents = getComponentsFromDateTime(datetimeAsString);
    return dateTimeComponents.size() > 1 ? dateTimeComponents[5] : ""; // MM
}

// Function to extract the seconds from a DateTime(YYYY-MM-DD HH:MM:SS)
string getSecondsFromDateTime(const string &datetimeAsString) {
    vector<string> dateTimeComponents = getComponentsFromDateTime(datetimeAsString);
    return dateTimeComponents.size() > 1 ? dateTimeComponents[6] : ""; // SS
}

// Function to convert from a military Time (24 hours format) to civilian Time (12 hours format with meridian)
string convertTimeToCivilian(const int militaryHours, const int minutes) {
    string meridian;
    int civilianHours;

    // Handle midnight and noon
    if (militaryHours == 0) {
        civilianHours = 12;
        meridian = "AM";
    } else if (militaryHours == 12) {
        civilianHours = 12;
        meridian = "PM";
    } else if (militaryHours < 12) {
        civilianHours = militaryHours;
        meridian = "AM";
    } else {
        civilianHours = militaryHours - 12;
        meridian = "PM";
    }

    // cout << civilianHours << ":" << (minutes < 10 ? "0" : "") << minutes << meridian << endl;
    return to_string(civilianHours) + ":" + (minutes < 10 ? "0" : "") + to_string(minutes) + " " + meridian;
}

// Detects if a given filename exist or not on the root of the executable file
bool fileExist(const string &fileName) {
    bool theFileExist = false; // If the file with text was already saved in the root of the executable

    if (filesystem::exists(fileName))
        theFileExist = true;

    return theFileExist;
}

// Gets all the non-empty lines of text inside a given file name
vector<string> getLinesFromFile(const string &fileName) {
    ifstream inputFile(fileName);
    vector<string> lines;

    if (inputFile.is_open()) {
        string line; // one single line at the time, to be read from the file

        while (getline(inputFile, line)) {
            if (!line.empty()) // If the line is empty then it's not interesting for us
                lines.push_back(line);
        }
    } else {
        cerr << "Error opening file\n";
    }

    // Closing the input file
    inputFile.close();

    return lines;
}

// Either creates a .txt file and adds text to it, or adds to an existent one
void addTextToFile(const string &fileName) {
    // Opens the input file & keeps the existing data (opens in append mode)
    ofstream outputFile(fileName, ios_base::app);
    // string textLine;

    if (outputFile.is_open()) {
        // We temporally store a single line of text, to be saved/added later to the .txt file
        const string textLine = getStringFromMessage("Write a single line of text please: ");
        if (!textLine.empty()) // There is no point on adding an empty string
            outputFile << textLine << endl;
    } else {
        cerr << "Error opening file\n";
    }

    // Closing the output file
    outputFile.close();
}

// Converts a giving string to lowercase
void strToLowerCase(string &input) {
    std::transform(input.begin(), input.end(), input.begin(),
                   [](const unsigned char c) { return tolower(c); });
}

// Converts a giving string to uppercase
void strToUpperCase(string &input) {
    std::transform(input.begin(), input.end(), input.begin(),
                   [](const unsigned char c) { return toupper(c); });
}

// Converts a string to lowercase and returns it
string getLowerCase(string input) {
    string destinationString = input;
    std::transform(input.begin(), input.end(), destinationString.begin(),
                   [](const unsigned char c) { return tolower(c); });
    return destinationString;
}

// Converts a string to uppercase and returns it
string getUpperCase(string input) {
    string destinationString = input;
    std::transform(input.begin(), input.end(), destinationString.begin(),
                   [](const unsigned char c) { return toupper(c); });
    return destinationString;
}

// Generates a Universally Unique IDentifier (the usual 36-character alphanumeric string. UUID style) as a string. Format: bdc0a2fb-d39e-0242-9a0a-4e760153f18d
string getUUID() {
    static random_device dev;
    static mt19937 rng(dev());
    uniform_int_distribution<int> dist(0, 15); // basic random int distribution of the indexes for 16 possible characters to choose from
    constexpr bool mustAddDashes[] = {false, false, false, false, true, false, true, false, true, false, true, false, false, false, false, false};
    string generatedID; // The conformed string to return

    const string allowedCharacters = "0123456789abcdef";
    for (const bool mustAddDashNow: mustAddDashes) {
        if (mustAddDashNow) generatedID += "-"; // We insert the dash according to the planification inside the boolean array
        generatedID += allowedCharacters[dist(rng)]; // Fully random index
        generatedID += allowedCharacters[dist(rng)]; // Fully random index again, so we have at least a pair of characters (as string) added per iteration
    }

    return generatedID;
}


/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                         *
 *          CUSTOM MADE FUNCTIONS DEFINITIONS              *
 *                                                         *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 **/


// Shows the program's welcoming message
void showProgramWelcome() {
    cout << endl;
    cout << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl;
    cout << "*                                                               *" << endl;
    cout << "*                Welcome to Payroll Pro 2.0                     *" << endl;
    cout << "*                                                               *" << endl;
    cout << "* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *" << endl;
}

// Displays the menu to the user
void displayMenu(const bool hasEmployees, const bool hasPayments) {
    cout << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "                     P R O G R A M   M E N U                     " << endl;
    cout << "-----------------------------------------------------------------" << endl;

    cout << ADD_EMPLOYEE_OPTION << " - Input an Employee into the system." << endl;

    if (hasEmployees) {
        cout << DELETE_EMPLOYEE_OPTION << " - Delete an Employee from our system." << endl;
        cout << SHOW_EMPLOYEES_OPTION << " - Show all the employees in our system." << endl;
        cout << ADD_PAYMENT_OPTION << " - Input a Payment for an existing Employee." << endl;
    }

    if (hasPayments) {
        cout << GENERATE_AND_PRINT_CURRENT_EPR_OPTION << " - Print the Addition & Average Payroll Report only for a current & specific employee." << endl;
        cout << GENERATE_AND_PRINT_COMPANY_PR_OPTION << " - Print the Addition & Average Payroll Report for all the company's employees." << endl;
    }

    cout << QUITTING_OPTION << " - Exit the Program." << endl;

    cout << endl;
}

// Validates and returns if the given selection is among the allowed selections from the Menu
bool isValidMenuSelection(const char input, const vector<char> &allowedAnswers) {
    return count(allowedAnswers.begin(), allowedAnswers.end(), input) > 0;
}

// Gets the option selected by the user, from the menu's options
char getMenuSelection(const bool hasEmployees, const bool hasPayments) {
    char selection = ADD_EMPLOYEE_OPTION;
    bool isInvalidAnswer;

    // We dinamically fill/conform the allowed menu options to choose from, depending of the existence or not of at least one employee & if we have at least one payment
    vector<char> allowedMenuOptions {ADD_EMPLOYEE_OPTION};
    const vector<char> ifHasEmployeesOptions {DELETE_EMPLOYEE_OPTION, SHOW_EMPLOYEES_OPTION, ADD_PAYMENT_OPTION};
    const vector<char> ifHasPaymentsOptions {GENERATE_AND_PRINT_CURRENT_EPR_OPTION, GENERATE_AND_PRINT_COMPANY_PR_OPTION};
    const vector<char> noMatterWhatAndLastOptions {QUITTING_OPTION}; // Done this way so the validation message with the available options gets shown ordered alphabetically

    if (hasEmployees) allowedMenuOptions.insert(allowedMenuOptions.end(), ifHasEmployeesOptions.begin(), ifHasEmployeesOptions.end());
    if (hasPayments) allowedMenuOptions.insert(allowedMenuOptions.end(), ifHasPaymentsOptions.begin(), ifHasPaymentsOptions.end());
    allowedMenuOptions.insert(allowedMenuOptions.end(), noMatterWhatAndLastOptions.begin(), noMatterWhatAndLastOptions.end());

    do {
        selection = static_cast<char>(toupper(getAlphaChar("Type your selection please"))); // Typecasting to avoid a warning

        isInvalidAnswer = !isValidMenuSelection(selection, allowedMenuOptions);
        if (isInvalidAnswer) {
            cout << "The only available options are: " << endl;
            const size_t size = allowedMenuOptions.size();
            for (int i = 0; i < size; i++) {
                std::cout << allowedMenuOptions[i];
                std::cout << (i == size - 2 ? ", or " : i == size - 1 ? "" : ", ");
            }
            std::cout << ". Try again." << endl;
        }
    } while (isInvalidAnswer);

    return selection;
}

// Processes the selection made by the user from the menu
void processMenuSelection(const char menuSelection, vector<Employee> &employees, vector<Payment> &payments) {
    switch (menuSelection) {
        case ADD_EMPLOYEE_OPTION:
            addEmployee(employees);
            break;
        case DELETE_EMPLOYEE_OPTION:
            deleteCurrentEmployee(employees);
            break;
        case SHOW_EMPLOYEES_OPTION:
            showCurrentEmployeesTable(employees);
            break;
        case ADD_PAYMENT_OPTION:
            addPayment(payments, employees);
            break;
        case GENERATE_AND_PRINT_CURRENT_EPR_OPTION:
            generateAndPrintCurrentEmployeePayrollReports(payments, employees);
            break;
        case GENERATE_AND_PRINT_COMPANY_PR_OPTION:
            generateAndPrintCompanyPayrollReports(payments);
            break;
        case QUITTING_OPTION:
            sayGoodbyeToTheUser();
            break;
        default: ;
    }
}

void showEmployeesTable(const vector<Employee> &employees) {
    cout << endl;
    cout << "Ok, these are the current employees:" << endl;
    cout << endl;

    // Finds the largest full name's length among the employees using max_element
    const auto largestEmployeeFullNameFirstIterator = max_element(employees.begin(), employees.end(),
                                                                  [](const Employee &a, const Employee &b) {
                                                                      return a.fullName().size() < b.fullName().size();
                                                                  });
    const int largestFullNameLength = static_cast<int>(largestEmployeeFullNameFirstIterator->fullName().size()); // Typecasting from size_t to int, just to avoid a warning

    // Table Header
    renderLineUnderTableRow(largestFullNameLength);
    cout << "|                Unique ID             | Full Name ";
    printNTimes(" ", largestFullNameLength - 10);
    cout << " |" << endl;
    renderLineUnderTableRow(largestFullNameLength);

    // Each one of the rows
    for (const Employee &employee: employees) {
        cout << "| " << employee.id << " | " << setw(largestFullNameLength) << setfill(' ') << left << employee.fullName() << " |" << endl;
        renderLineUnderTableRow(largestFullNameLength);
    }
}

// Prints an appropiate length "line" conformed by dashes, as part of a good looking table
void renderLineUnderTableRow(const int largestFullNameLength) {
    cout << "-----------------------------------------";
    printNTimes("-", largestFullNameLength);
    cout << "--" << endl;
}

// Adds an Employee structure variable to the reference of a given vector of Employees
void addEmployee(vector<Employee> &employees) {
    cout << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "                  A D D I N G   E M P L O Y E E                  " << endl;
    cout << "-----------------------------------------------------------------" << endl;
    const string firstName = getStringFromMessage("Please type the first name of the new Employee: ");
    const string lastName = getStringFromMessage("Please type the last name of the new Employee: ");
    const double regRate = getDouble("Please type the regular payment rate of the new Employee", MIN_HOURLY_WAGE, MAX_HOURLY_WAGE, true);
    employees.push_back(Employee {.id = getUUID(), .firstName = firstName, .lastName = lastName, .regRate = regRate});
}

// Removes an Employee structure variable from the reference of a given vector of Employees, by its given id
void deleteCurrentEmployee(vector<Employee> &employees) {
    string employeeId; // For the employee's id, to be typed or pasted by the user later
    bool theEmployeeDoNotExist; // If the user do not exist based on the entered id

    cout << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "                D E L E T I N G   E M P L O Y E E                " << endl;
    cout << "-----------------------------------------------------------------" << endl;

    // First we show the current employee's table to the user, so that the user can decide which employee to delete
    showEmployeesTable(employees);

    do {
        employeeId = getStringFromMessage("And now I need you to either type or copy/paste the id of the employee that you want to delete: ");
        theEmployeeDoNotExist = !existEmployee(employees, employeeId);
        if (theEmployeeDoNotExist)
            cout << "We don't have an Employee with such ID. Try again please." << endl;
    } while (theEmployeeDoNotExist);

    // Once we know that an Employee exist with such id, then we can safely delete it
    deleteEmployeById(employees, employeeId);
}

// Shows the table with all the current employees
void showCurrentEmployeesTable(const vector<Employee> &employees) {
    cout << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "                C U R R E N T   E M P L O Y E E S                " << endl;
    cout << "-----------------------------------------------------------------" << endl;

    // First we show the employee's table to the user, so that the user can decide which employee to associate the new payment with
    showEmployeesTable(employees);
}

// Adds a Payment structure variable, associated to a specific Employee, to the reference of a given vector of Payments
void addPayment(vector<Payment> &payments, const vector<Employee> &employees) {
    string employeeId; // For the employee's id, to be typed or pasted by the user later
    bool theEmployeeDoNotExist; // If the user do not exist based on the entered id

    cout << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "                   A D D I N G   P A Y M E N T                   " << endl;
    cout << "-----------------------------------------------------------------" << endl;

    // First we show the employee's table to the user, so that the user can decide which employee to associate the new payment with
    showEmployeesTable(employees);

    do {
        employeeId = getStringFromMessage("And now I need you to either type or copy/paste the id of the employee to whom you are going to associate the payment: ");
        theEmployeeDoNotExist = !existEmployee(employees, employeeId);
        if (theEmployeeDoNotExist)
            cout << "We don't have an Employee with such ID. Try again please." << endl;
    } while (theEmployeeDoNotExist);

    // Once we know that an Employee exist with such id, then we can safely retrieve it
    const Employee theEmployee = getEmployeById(employees, employeeId);

    // And then we can also safely associate the payment to the retrieved employee
    addPaymentToEmployee(payments, theEmployee);
}

// Adds an Employe's Payment structure variable to the reference of a given vector of Payments
void addPaymentToEmployee(vector<Payment> &payments, const Employee &employee) {
    double otHours {0};

    cout << endl;
    const double regHours = getDouble("Please type how many regular hours the Employee worked", 1, MAX_REG_HOURS, true);;
    if (MAX_REG_HOURS <= regHours && regHours < MAX_HOURS_WORKED) // There is no point in asking for overtime if the employee dont even have the 40 weekly hours or if he already worked the legal maximum of 50
        otHours = getDouble("Please type how many overtime hours the Employee worked", 1, MAX_HOURS_WORKED - regHours, true);
    payments.push_back(Payment {.employeeId = employee.id, .regHours = regHours, .otHours = otHours, .regRate = employee.regRate});
}

// Prints on the terminal a PayrollReport for a specific Employee
void generateAndPrintCurrentEmployeePayrollReports(vector<Payment> &payments, const vector<Employee> &employees) {
    string employeeId; // For the employee's id, to be typed or pasted by the user later
    bool theEmployeeDoNotExist; // If the user do not exist based on the entered id
    bool theEmployeeHasPayments; // If the employee has received at least one payment

    cout << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "  C U R R E N T   E M P L O Y E E   P A Y R O L L   R E P O R T  " << endl;
    cout << "-----------------------------------------------------------------" << endl;

    // First we show the employee's table to the user, so that the user can decide for which employee he wants to print the Payment Report
    showEmployeesTable(employees);

    do {
        employeeId = getStringFromMessage("And now I need you to either type or copy/paste the id of the employee for whom you want to print the Payroll Report: ");
        theEmployeeDoNotExist = !existEmployee(employees, employeeId);
        if (theEmployeeDoNotExist)
            cout << "We don't have an Employee with such ID. Try again please." << endl;
    } while (theEmployeeDoNotExist); // We are not leaving until we get an existing employee's id

    // Ok, but now we also need to know if besides existing, the employee has associated payments too
    theEmployeeHasPayments = employeeHasPayments(payments, employeeId);

    if (theEmployeeHasPayments) {
        // Next we retrieve the Employee, for future printing purposes, as the future table will look way better with that useful extra data
        const Employee employee = getEmployeById(employees, employeeId);

        // Once we know that the Employee has at least an associated Payment, we can safely generate its pertinent addition & average EmployeePayrollReport
        const EmployeePayrollReport additionEmployeePayrollReport = createAdditionEmployeePayrollReport(payments, employee);
        const EmployeePayrollReport averageEmployeePayrollReport = createAverageEmployeePayrollReport(payments, employee);

        // And now we can finally send both to print
        printEmployeePayrollReports(additionEmployeePayrollReport, averageEmployeePayrollReport);
    } else {
        cout << "The selected employee has not received any payment yet. Good bye." << endl;
    }
}

// Prints on the terminal both PayrollReports, addition & average, for the whole company
void generateAndPrintCompanyPayrollReports(const vector<Payment> &payments) {
    // First we must generate the company's addition & average PayrollReports
    const PayrollReport additionPayrollReport = createAdditionPayrollReport(payments);
    const PayrollReport averagePayrollReport = createAveragePayrollReport(payments);

    cout << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "           C O M P A N Y   P A Y R O L L   R E P O R T           " << endl;
    cout << "-----------------------------------------------------------------" << endl;

    // And now we can finally send both to print
    printCompanyPayrollReports(additionPayrollReport, averagePayrollReport);
}

// Determines if a given emloyeeID belongs to the current ones
bool existEmployee(const vector<Employee> &employees, const string &employeeId) {
    return any_of(employees.begin(), employees.end(), [&](const Employee &emp) { return emp.id == employeeId; });
}

// Determines if a given emloyee's id has associated at least one payment
bool employeeHasPayments(const vector<Payment> &payments, const string &employeeId) {
    return any_of(payments.begin(), payments.end(), [&](const Payment &payment) { return payment.employeeId == employeeId; });
}

// Retrieves an Employee structure variable by a given employee's id
Employee getEmployeById(const vector<Employee> &employees, const string &employeeId) {
    // Let's just return it directly
    return *find_if(employees.begin(), employees.end(), [&](const Employee &emp) { return emp.id == employeeId; });
}

// Deletes an Employee structure variable by a given employee's id
void deleteEmployeById(vector<Employee> &employees, const string &employeeId) {
    auto it = std::remove_if(employees.begin(), employees.end(), [&](const Employee &emp) {
        return emp.id == employeeId;
    });
    employees.erase(it, employees.end());
}

// Generates a EmployeePayrollReport with the addition of all the Payment structure variables related to a given employee
EmployeePayrollReport createAdditionEmployeePayrollReport(const vector<Payment> &payments, const Employee &employee) {
    EmployeePayrollReport theAdditionEmployeePayrollReport {.employeeId = employee.id, .firstName = employee.firstName, .lastName = employee.lastName}; // Gets associated to the employee

    // And then we increase each respective field on each iteration, to leave it as an Addition EmployeePayrollReport
    for (const Payment &payment: payments) {
        if (payment.employeeId == employee.id) {
            theAdditionEmployeePayrollReport.paymentsAmount++;
            theAdditionEmployeePayrollReport.regHours += payment.regHours;
            theAdditionEmployeePayrollReport.otHours += payment.otHours;
            theAdditionEmployeePayrollReport.regPay += payment.regPay();
            theAdditionEmployeePayrollReport.otPay += payment.otPay();
            theAdditionEmployeePayrollReport.fica += payment.fica();
            theAdditionEmployeePayrollReport.socSec += payment.socSec();
        }
    }

    return theAdditionEmployeePayrollReport;
}

// Generates a PayrollReport with the addition of all the Payment structure variables's data of the whole company across the time
PayrollReport createAdditionPayrollReport(const vector<Payment> &payments) {
    PayrollReport anAdditionPayrollReport; // Defined to be filled with data next

    // And then we increase each respective field, to leave it as an Addition PayrollReport
    for (const Payment &payment: payments) {
        anAdditionPayrollReport.paymentsAmount++;
        anAdditionPayrollReport.regHours += payment.regHours;
        anAdditionPayrollReport.otHours += payment.otHours;
        anAdditionPayrollReport.regPay += payment.regPay();
        anAdditionPayrollReport.otPay += payment.otPay();
        anAdditionPayrollReport.fica += payment.fica();
        anAdditionPayrollReport.socSec += payment.socSec();
    }

    return anAdditionPayrollReport;
}

// Generates a EmployeePayrollReport with the average of all the Payment structure variables related to a given employee
EmployeePayrollReport createAverageEmployeePayrollReport(const vector<Payment> &payments, const Employee &employee) {
    // First we get a good old fashion & regular EmployeePayrollReport based on the given employee
    EmployeePayrollReport anAdditionEmployeePayrollReport = createAdditionEmployeePayrollReport(payments, employee);

    // Then we count how many payments has associated the employee to whom belongs the given id, so we can average his payment stats right after that
    // const int empoloyeePaymentsAmount = count_if(payments.begin(), payments.end(), [&](const Payment &payment) { return payment.employeeId == employeeId; });
    const int employeePaymentsAmount = anAdditionEmployeePayrollReport.paymentsAmount; // Forget it. It's way simpler & faster this way

    // And now we must average/update each field, to leave it as an average EmployeePayrollReport
    anAdditionEmployeePayrollReport.regHours /= employeePaymentsAmount;
    anAdditionEmployeePayrollReport.otHours /= employeePaymentsAmount;
    anAdditionEmployeePayrollReport.regPay /= employeePaymentsAmount;
    anAdditionEmployeePayrollReport.otPay /= employeePaymentsAmount;
    anAdditionEmployeePayrollReport.fica /= employeePaymentsAmount;
    anAdditionEmployeePayrollReport.socSec /= employeePaymentsAmount;

    return anAdditionEmployeePayrollReport; // Turned into an Average EmployeePayrollReport at this point
}

// Generates a PayrollReport with the average of all the Payment structure variables's data of the whole company across the time
PayrollReport createAveragePayrollReport(const vector<Payment> &payments) {
    // First we get a good old fashion & regular PayrollReport with addition data, of all the Payment structure variables's data of the whole company across the time
    PayrollReport anAdditionPayrollReport = createAdditionPayrollReport(payments);

    // Then we count how many payments exist, so we can average the PayrollReport's stats right after that
    const int companyPaymentsAmount = static_cast<int>(payments.size()); // It's way simpler & faster this way. Also casting from size_t to int to avoid an annoying warning

    // And now we must average/update each field, to leave it as an average PayrollReport
    anAdditionPayrollReport.regHours /= companyPaymentsAmount;
    anAdditionPayrollReport.otHours /= companyPaymentsAmount;
    anAdditionPayrollReport.regPay /= companyPaymentsAmount;
    anAdditionPayrollReport.otPay /= companyPaymentsAmount;
    anAdditionPayrollReport.fica /= companyPaymentsAmount;
    anAdditionPayrollReport.socSec /= companyPaymentsAmount;

    return anAdditionPayrollReport; // Turned into an Average PayrollReport at this point
}

// Prints on the console both, the addition & average PayrollReports of the company
void printCompanyPayrollReports(const PayrollReport &additionPR, const PayrollReport &averagePR) {
    cout << endl;
    cout << "The company has made " << additionPR.paymentsAmount << " payment" << (additionPR.paymentsAmount == 1 ? "" : "s") << "." << endl;
    printPayrollReportsTable(additionPR, averagePR);
}

// Prints on the console both, the addition & average given EmployeePayrollReports
void printEmployeePayrollReports(const EmployeePayrollReport &additionEPR, const EmployeePayrollReport &averageEPR) {
    cout << endl;
    cout << "The employee " << additionEPR.fullName() << ", with id " << additionEPR.employeeId << " has received " << additionEPR.paymentsAmount << " payment" << (additionEPR.paymentsAmount == 1 ? "" : "s") << "." << endl;
    printPayrollReportsTable(additionEPR, averageEPR);
}

// Prints either a EmployeePayrollReport or a PayrollReport structure variable, with addition and average data,
// as we pass as argument a father struct PayrollReport variable, and from the received parameter we won't use the employee's id anyway at this point (either done before or not needed)
void printPayrollReportsTable(const PayrollReport &additionPR, const PayrollReport &averagePR) {
    constexpr int MAX_ROW_WIDTH = 50; // We set the overall width of the table based on this precisely. The columns must fit inside
    constexpr int ADDITION_COL_INNER_WIDTH = 12; // To customize the width of the Addition Column, so it looks good later
    constexpr int AVERAGE_COL_INNER_WIDTH = 11; // To customize the width of the Average Column, so it looks good later

    printNTimesAndBreak("-", MAX_ROW_WIDTH);
    cout << "|       Field       |   Addition   |   Average   |" << endl;
    printNTimesAndBreak("-", MAX_ROW_WIDTH);
    cout << "|  Regular Hours    | " << setw(ADDITION_COL_INNER_WIDTH) << left << additionPR.regHours << " | " << setw(AVERAGE_COL_INNER_WIDTH) << left << averagePR.regHours << " |" << endl;
    printNTimesAndBreak("-", MAX_ROW_WIDTH);
    cout << "|  Overtime Hours   | " << setw(ADDITION_COL_INNER_WIDTH) << left << additionPR.otHours << " | " << setw(AVERAGE_COL_INNER_WIDTH) << left << averagePR.otHours << " |" << endl;
    printNTimesAndBreak("-", MAX_ROW_WIDTH);
    cout << "|  Regular Pay      | " << setw(ADDITION_COL_INNER_WIDTH) << left << monetizeDouble(additionPR.regPay) << " | " << setw(AVERAGE_COL_INNER_WIDTH) << left << monetizeDouble(averagePR.regPay) << " |" << endl;
    printNTimesAndBreak("-", MAX_ROW_WIDTH);
    cout << "|  Overtime Pay     | " << setw(ADDITION_COL_INNER_WIDTH) << left << monetizeDouble(additionPR.otPay) << " | " << setw(AVERAGE_COL_INNER_WIDTH) << left << monetizeDouble(averagePR.otPay) << " |" << endl;
    printNTimesAndBreak("-", MAX_ROW_WIDTH);
    cout << "|       FICA        | " << setw(ADDITION_COL_INNER_WIDTH) << left << monetizeDouble(additionPR.fica) << " | " << setw(AVERAGE_COL_INNER_WIDTH) << left << monetizeDouble(averagePR.fica) << " |" << endl;
    printNTimesAndBreak("-", MAX_ROW_WIDTH);
    cout << "|  Social Security  | " << setw(ADDITION_COL_INNER_WIDTH) << left << monetizeDouble(additionPR.socSec) << " | " << setw(AVERAGE_COL_INNER_WIDTH) << left << monetizeDouble(averagePR.socSec) << " |" << endl;
    printNTimesAndBreak("-", MAX_ROW_WIDTH);
    cout << "|     Total Pay     | " << setw(ADDITION_COL_INNER_WIDTH) << left << monetizeDouble(additionPR.totalPay()) << " | " << setw(AVERAGE_COL_INNER_WIDTH) << left << monetizeDouble(averagePR.totalPay()) << " |" << endl;
    printNTimesAndBreak("-", MAX_ROW_WIDTH);
    cout << "|  Total Deductions | " << setw(ADDITION_COL_INNER_WIDTH) << left << monetizeDouble(additionPR.totDeductions()) << " | " << setw(AVERAGE_COL_INNER_WIDTH) << left << monetizeDouble(averagePR.totDeductions()) << " |" << endl;
    printNTimesAndBreak("-", MAX_ROW_WIDTH);
    cout << "|      Net Pay      | " << setw(ADDITION_COL_INNER_WIDTH) << left << monetizeDouble(additionPR.netPay()) << " | " << setw(AVERAGE_COL_INNER_WIDTH) << left << monetizeDouble(averagePR.netPay()) << " |" << endl;
    printNTimesAndBreak("-", MAX_ROW_WIDTH);
}

// Prints on the console goodbyes to the user
void sayGoodbyeToTheUser() {
    cout << endl;
    cout << "Than you for using our services. " << endl;
    cout << "Payroll Pro 20.0, Copyright © 2024 https://www.reiniergarcia.dev/" << endl;
    cout << "Goodbye!" << endl;
}
