/**
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                                   *
 *   Name: Reinier Garcia Ramos                                      *
 *                                                                   *
 *   Program Title: Final Project (Payroll Pro 2.0)                  *
 *                                                                   *
 *   Purpose:                                                        *
 *   Allows the user to enter the Employees data, their payments,    *
 *   as well as printing the addition & average Payroll Reports      *
 *   for a specific employee and for the whole company as well.      *
 *                                                                   *
 *   Notes:                                                          *
 *   This program requires at least c++20 on Microsoft Visual        *
 *   Studio, as it uses designated initializers on the structure     *
 *   variables.                                                      *
 *                                                                   *
 *   Instead, on MacOS with gcc Compiler Apple clang                 *
 *   version 15.0.0 (clang-1500.3.9.4), it runs perfectly on c++17   *
 *   from the CLion IDE 2024.1.4                                     *
 *                                                                   *
 *   It also runs perfectly fine on xCode using c++17 with the       *
 *   Apple Clang Compiler.                                           *
 *                                                                   *
 *   More Details:                                                   *
 *   https://github.com/reymillenium/20240718_1021_final_project     *
 *                                                                   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 **/

#include <iostream>
#include <string>
#include <iomanip>
#include<random>
#include <sstream>
#include <vector>
#include <algorithm>
#include <regex>

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
constexpr double MIN_HOURLY_WAGE = 10.00;
constexpr double MAX_HOURLY_WAGE = 30.00;
constexpr double FICA_RATE = .20;
constexpr double SS_MED_RATE = .0765;
constexpr double OT_MULT = 1.5;

constexpr char ADD_EMPLOYEE_OPTION = 'A';
constexpr char DELETE_EMPLOYEE_OPTION = 'B';
constexpr char SHOW_CURRENT_EMPLOYEES_OPTION = 'C';
constexpr char ADD_PAYMENT_OPTION = 'D';
constexpr char GENERATE_AND_PRINT_CURRENT_EPR_OPTION = 'E';
constexpr char SHOW_ALL_THE_PAYMENTS_OPTION = 'F';
constexpr char GENERATE_AND_PRINT_COMPANY_PR_OPTION = 'G';
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
void print(const T &item) {
    cout << item;
}

// Prints a given value, of almost any kind, N given times in the terminal
template<typename T>
void printNTimes(const T &, int);

// Prints a given value, of almost any kind, N given times in the terminal, and then breaks the line
template<typename T>
void printNTimesAndBreak(const T &, int);

// Prints a given value, of almost any kind, once in the terminal, and then breaks the line
template<typename T>
void printLine(const T &);

// Determines if a given string is a valid integer, using a regular expression
bool isInteger(const string &input);

// Determines if a given string is a valid floating point number, using a regular expression
bool isFloatingPoint(const string &input);

// Receives and validates a double number (or the equivalent of an integer) from the console
double getDouble(const string &, double, double, bool = false, const string & = "Invalid input. Please try again.", const vector<double> & = {});

// Determines if a given string is a single valid char
bool containsSingleChar(const string &input);

// Receives and validates a char from the console
char getAlphaChar(const string &, const string & = "Invalid input. Please try again.");

// Gets a string with or without spaces, from the terminal, as a response of a given question
string getStringFromMessage(const string &);

// Formats a given positive int by inserting a comma every 3 digits of its equivalent string, to make it more readable, by US standards
string humanizeUnsignedInteger(unsigned long long int);

// Formats a given positive double by inserting a comma every 3 digits of its equivalent string, to make it more readable, by US standards
string humanizeUnsignedDouble(long double, int = 2);

// Formats a given double by inserting a comma every 3 digits of its equivalent string, to make it more readable, and adds a customizable currency symbol
string monetizeDouble(long double, int = 2, bool = true, const string & = "$");

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
    string id;
    string firstName;
    string lastName;
    double regRate {0.0};

    // Employee() = default;

    [[nodiscard]] string fullName() const { return firstName + " " + lastName; }
};

// The Employee could be deleted from the system, but we still have its data (I'm not using an Employee,
// to avoid theorically a DB persistence validation over the data layer [obviously none-existent, as we are not even using a DB in the first place])
// But still in a real life scenario it would the best approach to avoid many issues, using an instance/object of a Class instead of structure variables
// ...and we just need a few fields anyway, so it will remain denormalized with these 3 elements, instead of using an Employee structure variable
struct Payment {
    string employeeId;
    string firstName;
    string lastName;

    double hoursWorked {0.0};
    double regRate {0.0};

    // Payment() = default; // Prevents from using the cleaner designated list initializer syntax in MSVS

    [[nodiscard]] string fullName() const { return firstName + " " + lastName; }

    [[nodiscard]] double regHours() const { return (hoursWorked <= MAX_REG_HOURS ? hoursWorked : MAX_REG_HOURS); }
    [[nodiscard]] double otHours() const { return (hoursWorked <= MAX_REG_HOURS ? 0 : hoursWorked - MAX_REG_HOURS); }
    [[nodiscard]] double otRate() const { return regRate * OT_MULT; }
    [[nodiscard]] double regPay() const { return regHours() * regRate; }
    [[nodiscard]] double otPay() const { return otHours() * otRate(); }
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

    // PayrollReport() = default;

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
    string employeeId;
    string firstName;
    string lastName;

    // EmployeePayrollReport() = default;

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

// Gets the length of the pargest full name from a given vector of Employee structure variables
int getLargestFullNameLength(const vector<Employee> &employees);

// Gets the length of the pargest full name from a given vector of Payment structure variables
int getLargestFullNameLength(const vector<Payment> &payments);

void showEmployeesTable(const vector<Employee> &);

// Prints an appropiate length "line" conformed by dashes, as part of a good looking Employees table
void renderLineUnderEmployeesTableRow(int);

// Adds a Payment structure variable to the reference of a given vector of Payments
void addPaymentToEmployee(vector<Payment> &, const Employee &);

// Prints on the terminal all the payments made by the company, including those to ex employees
void printAllThePayments(const vector<Payment> &);

// Prints an appropiate length "line" conformed by dashes, as part of a good looking Payments table
void renderLineUnderPaymentsTableRow(int);

// Prints on the terminal a given vector of Payment structure variables
void printPayments(const vector<Payment> &);

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
void print(const T &);

// Prints a given value, of almost any kind, N given times in the terminal
template<typename T>
void printNTimes(const T &item, const int times) {
    for (int i = 0; i < times; i++) {
        print(item);
    }
}

// Prints a given value, of almost any kind, N given times in the terminal, and then breaks the line
template<typename T>
void printNTimesAndBreak(const T &item, const int times) {
    printNTimes(item, times);
    printLine("");
}

// Prints a given value, of almost any kind, once in the terminal, and then breaks the line
template<typename T>
void printLine(const T &item) {
    cout << item << endl;
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
    return input.length() == 1;
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

// Formats a given double by inserting a comma every 3 digits of its equivalent string, to make it more readable, and adds a customizable currency symbol
string monetizeDouble(const long double doubleValue, const int precision, const bool prepend, const string &symbol) {
    return static_cast<string>(prepend ? (symbol + " ") : "") + humanizeUnsignedDouble(doubleValue, precision) + static_cast<string>(prepend ? "" : " " + symbol);
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
        cout << SHOW_CURRENT_EMPLOYEES_OPTION << " - Show all the current/hired employees in our system." << endl;
        cout << ADD_PAYMENT_OPTION << " - Input a Payment for an existing Employee." << endl;
    }

    if (hasEmployees && hasPayments) {
        cout << GENERATE_AND_PRINT_CURRENT_EPR_OPTION << " - Print the Addition & Average Payroll Report only for a current & specific employee." << endl;
    }

    if (hasPayments) {
        cout << SHOW_ALL_THE_PAYMENTS_OPTION << " - Show all the payments made by the company, even to ex employees" << endl;
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
    const vector<char> ifHasEmployeesOptions {DELETE_EMPLOYEE_OPTION, SHOW_CURRENT_EMPLOYEES_OPTION, ADD_PAYMENT_OPTION};
    const vector<char> ifHasEmployeesAndPaymentsOptions {GENERATE_AND_PRINT_CURRENT_EPR_OPTION};
    const vector<char> ifHasPaymentsOptions {SHOW_ALL_THE_PAYMENTS_OPTION, GENERATE_AND_PRINT_COMPANY_PR_OPTION};
    const vector<char> noMatterWhatAndLastOptions {QUITTING_OPTION}; // Done this way so the validation message with the available options gets shown ordered alphabetically

    if (hasEmployees) allowedMenuOptions.insert(allowedMenuOptions.end(), ifHasEmployeesOptions.begin(), ifHasEmployeesOptions.end());
    if (hasEmployees && hasPayments) allowedMenuOptions.insert(allowedMenuOptions.end(), ifHasEmployeesAndPaymentsOptions.begin(), ifHasEmployeesAndPaymentsOptions.end());
    if (hasPayments) allowedMenuOptions.insert(allowedMenuOptions.end(), ifHasPaymentsOptions.begin(), ifHasPaymentsOptions.end());
    allowedMenuOptions.insert(allowedMenuOptions.end(), noMatterWhatAndLastOptions.begin(), noMatterWhatAndLastOptions.end());

    do {
        selection = static_cast<char>(toupper(getAlphaChar("Type your selection please"))); // Typecasting to avoid a warning

        isInvalidAnswer = !isValidMenuSelection(selection, allowedMenuOptions);
        if (isInvalidAnswer) {
            cout << "The only available options are: " << endl;
            const size_t size = allowedMenuOptions.size();
            for (int i = 0; i < size; i++) {
                cout << allowedMenuOptions[i];
                cout << (i == size - 2 ? ", or " : i == size - 1 ? "" : ", ");
            }
            cout << ". Try again." << endl;
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
        case SHOW_CURRENT_EMPLOYEES_OPTION:
            showCurrentEmployeesTable(employees);
            break;
        case ADD_PAYMENT_OPTION:
            addPayment(payments, employees);
            break;
        case SHOW_ALL_THE_PAYMENTS_OPTION:
            printAllThePayments(payments);
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

// Gets the length of the pargest full name from a given vector of Employee structure variables
int getLargestFullNameLength(const vector<Employee> &employees) {
    // Finds the largest full name's length among the employees using max_element
    const auto largestEmployeeFullNameFirstIterator = max_element(employees.begin(), employees.end(),
                                                                  [](const Employee &a, const Employee &b) {
                                                                      return a.fullName().size() < b.fullName().size();
                                                                  });
    return static_cast<int>(largestEmployeeFullNameFirstIterator->fullName().size()); // Typecasting from size_t to int, just to avoid a warning
}

// Gets the length of the pargest full name from a given vector of Payment structure variables
int getLargestFullNameLength(const vector<Payment> &payments) {
    // Finds the largest full name's length among the payments done to employees, using max_element
    const auto largestPaymentFullNameFirstIterator = max_element(payments.begin(), payments.end(),
                                                                 [](const Payment &a, const Payment &b) {
                                                                     return a.fullName().size() < b.fullName().size();
                                                                 });
    return static_cast<int>(largestPaymentFullNameFirstIterator->fullName().size()); // Typecasting from size_t to int, just to avoid a warning
}

void showEmployeesTable(const vector<Employee> &employees) {
    cout << endl;
    cout << "Ok, these are the current employees:" << endl;
    cout << endl;

    // Finds the length of the employee with the largest full name
    const int largestFullNameLength = getLargestFullNameLength(employees);

    // Table Header
    renderLineUnderEmployeesTableRow(largestFullNameLength);
    cout << "|                Unique ID             | Full Name ";
    printNTimes(" ", largestFullNameLength - 10);
    cout << " |" << endl;
    renderLineUnderEmployeesTableRow(largestFullNameLength);

    // Each one of the rows
    for (const Employee &employee: employees) {
        cout << "| " << employee.id << " | " << setw(largestFullNameLength) << setfill(' ') << left << employee.fullName() << " |" << endl;
        renderLineUnderEmployeesTableRow(largestFullNameLength);
    }
}

// Prints an appropiate length "line" conformed by dashes, as part of a good looking Employees table
void renderLineUnderEmployeesTableRow(const int largestFullNameLength) {
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
    cout << endl;
    const double hoursWorked = getDouble("Please type how many hours the Employee worked in total on the week", 1, MAX_HOURS_WORKED, true);;
    payments.push_back(Payment {.employeeId = employee.id, .firstName = employee.firstName, .lastName = employee.lastName, .hoursWorked = hoursWorked, .regRate = employee.regRate});
}

// prints on the terminal all the payments made by the company, including those to ex employees
void printAllThePayments(const vector<Payment> &payments) {
    cout << endl;
    cout << "-----------------------------------------------------------------" << endl;
    cout << "                 A L L   T H E   P A Y M E N T S                 " << endl;
    cout << "-----------------------------------------------------------------" << endl;

    // We send to print all the payments done by the company, including those to ex employees
    printPayments(payments);
}

// Prints an appropiate length "line" conformed by dashes, as part of a good looking Payments table
void renderLineUnderPaymentsTableRow(const int largestFullNameLength) {
    printNTimes("-", largestFullNameLength);
    printNTimesAndBreak("-", 162);
}

// Prints on the terminal a given vector of Payment structure variables
void printPayments(const vector<Payment> &payments) {
    // We get the length of the payment done to the employee with the largest full name
    const int largestFullNameLength = getLargestFullNameLength(payments);

    cout << endl;

    // Table Header
    renderLineUnderPaymentsTableRow(largestFullNameLength);
    cout << "| Full Name ";
    printNTimes(" ", largestFullNameLength - 10);
    cout << " | Hrs Worked | Reg Hrs | Reg Rate | OT Hrs | OT Rate |    Reg Pay   |    OT Pay    |  Total Pay   |     FICA     | Soc Security | Total Deduc. |    Net Pay   |" << endl;
    renderLineUnderPaymentsTableRow(largestFullNameLength);

    // Each one of the rows
    for (const Payment &payment: payments) {
        cout << "| " << right << setw(largestFullNameLength) << setfill(' ') << left << payment.fullName() << " | " << right << setw(10) << payment.hoursWorked << " | ";
        cout << setw(7) << payment.regHours() << " | " << setw(8) << monetizeDouble(payment.regRate) << " | " << setw(6) << payment.otHours() << " | " << setw(7) << monetizeDouble(payment.otRate()) << " | ";
        cout << setw(12) << monetizeDouble(payment.regPay()) << " | " << setw(12) << monetizeDouble(payment.otPay()) << " | " << setw(12) << monetizeDouble(payment.totalPay()) << " | ";
        cout << setw(12) << monetizeDouble(payment.fica()) << " | " << setw(12) << monetizeDouble(payment.socSec()) << " | " << setw(12) << monetizeDouble(payment.totDeductions()) << " | ";
        cout << setw(12) << monetizeDouble(payment.netPay()) << " |" << endl;
        renderLineUnderPaymentsTableRow(largestFullNameLength);
    }
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
    const auto it = remove_if(employees.begin(), employees.end(), [&](const Employee &emp) {
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
            theAdditionEmployeePayrollReport.regHours += payment.regHours();
            theAdditionEmployeePayrollReport.otHours += payment.otHours();
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
        anAdditionPayrollReport.regHours += payment.regHours();
        anAdditionPayrollReport.otHours += payment.otHours();
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
    cout << "The employee " << additionEPR.fullName() << ", with ID " << additionEPR.employeeId << " has received " << additionEPR.paymentsAmount << " payment" << (additionEPR.paymentsAmount == 1 ? "" : "s") << "." << endl;
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
