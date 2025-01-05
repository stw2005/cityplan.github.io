#include "../include/sort-techniques.hpp"
#include "../include/custom-exceptions.hpp"
#include "../include/string-matching.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <set>
using namespace std;

// Data structure to store student data.
struct Stu {
    string srn;
    string name;
    char div;
    double percentage;
    
    // Constructor
    Stu(string srn, string name, char div, double percentage): srn(srn), name(name), div(div), percentage(percentage) {}

    void display_srn() const {
        cout << "SRN: " << srn << '\n';
    }

    void display_name() const {
        cout << "Name: " << name << '\n';
    }

    void display_div() const {
        cout << "Division: " << div << '\n';
    }

    void display_percentage() const {
        cout << "Percentage: " << percentage << '\n';
    }

    // Writes the output to file
    void write_to_file(ofstream& output_file) {
        output_file << "SRN: " << srn << '\n';
        output_file << "Name: " << name << '\n';
        output_file << "Division: " << div << '\n';
        output_file << "Percentage: " << percentage << '\n' << std::endl;
    }

    // Display student data.
    void display() const {
        display_srn();
        display_name();
        display_div();
        display_percentage();
        cout << endl;
    }
};

// Returns the student data read from "students.txt" file.
vector<Stu> read_student_data();

// Selects the appropriate data using given input.
vector<Stu> select_vector(const vector<Stu>&, string, string, char, string);

// This sets the order of the fetched data according to the comparator.
void order_by(vector<Stu>&, function<bool(Stu, Stu)>);

// Gets top k student data.
vector<Stu> fetch_only(vector<Stu>&, int, function<bool(Stu, Stu)>);

// Handles the console and its operations
class ConsoleOperations {
public:
    // Displays the menu of operations
    void start_console(const vector<Stu>&);

    // Performs the operations according to given data.
    vector<Stu> perform_operations(const vector<Stu>&, string, string, char, string, string, string, int);

    // Writes the output to "output.txt" file.
    void write_to_file(const vector<Stu>&, ofstream&);
};

int main() {
    vector<Stu> student_data = read_student_data();
    ConsoleOperations console = ConsoleOperations();
    console.start_console(student_data);
    return 0;
}

vector<Stu> read_student_data() {
    ifstream input_file("data/students.txt");
    if (!input_file)
        throw FileNotOpeningException();
    vector<Stu> student_data;
    string srn, name, line, attribute;
    char div;
    double percentage;
    while (getline(input_file, line)) {
        int attribute_no = 0;
        istringstream ss(line);
        while (getline(ss, attribute, ',')) {
                switch (attribute_no) {
                case 0:
                    srn = attribute;
                    break;
                case 1:
                    name = attribute;
                    break;
                case 2:
                    div = attribute[0];
                    break;
                case 3:
                    percentage = stod(attribute);
                    break;
                default:
                    throw OutOfBoundException();
                    break;
                }
                attribute_no++;
        }
        student_data.push_back(Stu(srn, name, div, percentage));
    }
    input_file.close();
    return student_data;
}

vector<Stu> select_vector(const vector<Stu>& student_data, string year, string dept, char division, string name) {
    vector<Stu> selection;
    int n = student_data.size(), i = 0;
    while (i < n) {
        if (year == "*" or str_match(student_data[i].srn, year, 4)) {
            if (dept == "*" or str_match(student_data[i].srn, dept, 6)) {
                if (division == '*' or division == student_data[i].div) {
                    if (name == "*" or name == student_data[i].name) {
                        selection.push_back(student_data[i]);
                    }
                }
            }
        }
        i++;
    }
    return selection;
}

void order_by(vector<Stu>& selection, function<bool(Stu, Stu)> comparator) {
    quick_sort(selection, comparator);
}

vector<Stu> fetch_only(vector<Stu>& selection, int k, function<bool(Stu, Stu)> comparator) {
    vector<Stu> output = selection;
    order_by(output, comparator);
    if (output.size() > k)
        output.erase(output.begin() + k, output.end());
    cout << output.empty() <<endl;
    return output;
}

void ConsoleOperations::start_console(const vector<Stu>& student_data) {
    int command = -1;
    vector<string> depts = {"BCS", "BCI", "BME", "BEC"};
    set<string> valid_inputs = {"1", "2", "3", "4", "*"};
    do {
        cout << "\n\n\t\t\t\t-----Console-----\n";
        cout << "These are the operations you can perform:\n";
        cout << "1.Fetch data\t2.Exit console\n";
        while (command != 1 and command != 2) {
            cout << "Enter the number behind the operation you want to perform: "; cin >> command;
            if (command != 1 and command != 2)
                cout << "Invalid input. Try again\n";
        }
        if (command == 1) {
            string dept = "0", name, percentage, rows, attribute = "0", year = "0";
            char div;
            int ord = -1;
            double percent;
            cout << "\nIf you want to search through all possiblities press '*'.\n\n";
            cout << "Enter the details of the required data:\n";
            while (year != "1" and year != "2" and year != "3" and year != "4" and year != "*") {
                cout << "Enter the year of the student(s): "; cin >> year;
                if (year != "1" and year != "2" and year != "3" and year != "4" and year != "*")
                    cout << "Invalid input. Try again\n";
            }
            if (year != "*") {
                int c = year[0];
                year = "2";
                if (c == '4')
                    year.push_back('1');
                else if (c == '3')
                    year.push_back('2');
                else if (c == '2')
                    year.push_back('3');
                else
                    year.push_back('4');
            }
            while (valid_inputs.find(dept) == valid_inputs.end()) {
                cout << "These are the departments\n";
                cout << "1.Computer Science Engineering\t2.Computer Science and Artificial Intelligence Engineering\n3.Mechanical Engineering\t4.Electronics Engineering\n";
                cout << "Enter the department of the student(s): "; cin >> dept;
                if (valid_inputs.find(dept) == valid_inputs.end())
                    cout << "Invalid input. Try again\n";
            }
            if (dept != "*")
                dept = depts[stoi(dept)];
            cout << "Enter the division of the student(s): "; cin >> div;
            cout << "Enter the name of the student(s): "; cin >> name;
            cout << "How many rows of data do you intend to fetch?: "; cin >> rows;
            cout << "\nThese are the attributes to choose from:\n";
            cout << "1.Name\t2.Division\t3.Percentage\n";
            cout << "Choose an attribute to set the order (Default is SRN): "; cin >> attribute;
            while (ord != 1 and ord != 2) {
                cout << "1. Ascending\t2.Descending\n";
                cout << "Choose the order: "; cin >> ord;
                if (ord != 1 and ord != 2)
                    cout << "Invalid input. Try again";
            }
            vector<Stu> output = perform_operations(student_data, year, dept, div, name, rows, attribute, ord);
            ofstream output_file("output.txt");
            if (output_file.is_open()) {
                write_to_file(output, output_file);
                cout << "The output file has been filled with the necessary data.";
            } else
                throw FileNotOpeningException();
        }
    } while (command != 2);
}

vector<Stu> ConsoleOperations::perform_operations(const vector<Stu>& student_data, string year, string dept, char div, string name, string row, string attribute, int ord) {
    vector<Stu> output = select_vector(student_data, year, dept, div, name);
    int rows = (row == "*" ? output.size() : stoi(row));
    cout << output.empty() << endl;
    if (attribute == "1") {
        output = fetch_only(output, rows, [&ord](Stu a,Stu b) { return (ord == 1)? a.name < b.name : a.name > b.name; });
    } else if (attribute == "2") {
        output = fetch_only(output, rows, [&ord](Stu a, Stu b) { return (ord == 1)? a.div < b.div : a.div > b.div; });
    } else if (attribute == "3") {
        output = fetch_only(output, rows, [&ord](Stu a,Stu b) { return (ord == 1)? a.percentage < b.percentage : a.percentage > b.percentage; });
    } else
        output = fetch_only(output, rows, [&ord](Stu a,Stu b) { return (ord == 1)? a.srn < b.srn : a.srn > b.srn; });
    return output;
}

void ConsoleOperations::write_to_file(const vector<Stu>& output_data, ofstream& output_file) {
    for (Stu x: output_data)
        x.write_to_file(output_file);
}