#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <cctype> 

class Employee {
protected:
    std::string id; 
    std::string name;
    double salary;

public:
    Employee(std::string id, std::string name, double salary) : id(id), name(name), salary(salary) {}
    virtual ~Employee() {}
    virtual void display() const = 0;
    std::string getId() const { return id; }
};

class FullTimeEmployee : public Employee {
public:
    FullTimeEmployee(std::string id, std::string name, double salary) : Employee(id, name, salary) {}
    void display() const override {
        std::cout << "Employee: " << name << " (ID: " << id << ")\n";
        std::cout << "Fixed Monthly Salary: Php " << salary << "\n\n";
    }
};

class PartTimeEmployee : public Employee {
    double hoursWorked;

public:
    PartTimeEmployee(std::string id, std::string name, double hourlyWage, double hoursWorked)
        : Employee(id, name, hourlyWage * hoursWorked), hoursWorked(hoursWorked) {}
    void display() const override {
        std::cout << "Employee: " << name << " (ID: " << id << ")\n";
        std::cout << "Hourly Wage: Php "  << salary / hoursWorked << "\n";
        std::cout << "Hours Worked: " << hoursWorked << "\n";
        std::cout << "Total Salary: Php " << salary << "\n\n";
    }
};

class ContractualEmployee : public Employee {
    int projectsCompleted;

public:
    ContractualEmployee(std::string id, std::string name, double paymentPerProject, int projectsCompleted)
        : Employee(id, name, paymentPerProject * projectsCompleted), projectsCompleted(projectsCompleted) {}
    void display() const override {
        std::cout << "Employee: " << name << " (ID: " << id << ")\n";
        std::cout << "Contract Payment Per Project: Php " << salary / projectsCompleted << "\n";
        std::cout << "Projects Completed: " << projectsCompleted << "\n";
        std::cout << "Total Salary: Php " << salary << "\n\n";
    }
};

class PayrollSystem {
    std::vector<Employee*> employees;

public:
    ~PayrollSystem() {
        for (auto& emp : employees) {
            delete emp;
        }
    }

    void addEmployee(Employee* emp) {
        if (std::any_of(employees.begin(), employees.end(), [emp](Employee* e) { return e->getId() == emp->getId(); })) {
            std::cout << "Duplicate ID!\n";
            delete emp;
        } else {
            employees.push_back(emp);
        }
    }

    void displayPayrollReport() const {
        std::cout << "--- Employee Payroll Report ---\n";
        for (const auto& emp : employees) {
            emp->display();
        }
    }

    bool isIdUnique(const std::string& id) const {
        return std::none_of(employees.begin(), employees.end(), [id](Employee* e) { return e->getId() == id; });
    }
};

bool isNameValid(const std::string& name) {
    for (char ch : name) {
        if (!std::isalpha(ch) && !std::isspace(ch)) {
            return false; // Reject if the character is not alphabetic or a space
        }
    }
    return true;
}

std::string getValidatedStringInput(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input); // Use getline to handle spaces in names
        if (!input.empty()) {
            break;
        } else {
            std::cout << "Invalid input. Please enter a non-empty value.\n";
        }
    }
    return input;
}

std::string getValidatedNameInput(const std::string& prompt) {
    std::string name;
    while (true) {
        name = getValidatedStringInput(prompt);
        if (isNameValid(name)) {
            break;
        } else {
            std::cout << "Invalid input. Name should contain only alphabetic characters.\n";
        }
    }
    return name;
}

double getValidatedDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= 0) {
            break;
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a valid number.\n";
        }
    }
    return value;
}

int getValidatedIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value > 0) {
            break;
        } else {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a positive integer.\n";
        }
    }
    return value;
}

std::string getValidatedUniqueId(const PayrollSystem& payroll, const std::string& prompt) {
    std::string id;
    while (true) {
        id = getValidatedStringInput(prompt);
        if (payroll.isIdUnique(id)) {
            break;
        } else {
            std::cout << "Duplicate ID! Please enter another ID.\n";
        }
    }
    return id;
}

int main() {
    PayrollSystem payroll;
    int choice;
    do {
        std::cout << "Menu\n";
        std::cout << "1 - Full-time Employee\n";
        std::cout << "2 - Part-time Employee\n";
        std::cout << "3 - Contractual Employee\n";
        std::cout << "4 - Display Payroll Report\n";
        std::cout << "5 - Exit\n";
        choice = getValidatedIntInput("Enter your choice: ");
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer

        std::string id;
        std::string name;
        double salary, hourlyWage, paymentPerProject;
        double hoursWorked;
        int projectsCompleted;

        switch (choice) {
            case 1:
                id = getValidatedUniqueId(payroll, "Enter ID: ");
                name = getValidatedNameInput("Enter Name: ");
                salary = getValidatedDoubleInput("Enter Salary: ");
                payroll.addEmployee(new FullTimeEmployee(id, name, salary));
                break;
            case 2:
                id = getValidatedUniqueId(payroll, "Enter ID: ");
                name = getValidatedNameInput("Enter Name: ");
                hourlyWage = getValidatedDoubleInput("Enter Hourly Wage: ");
                hoursWorked = getValidatedDoubleInput("Enter Hours Worked: ");
                payroll.addEmployee(new PartTimeEmployee(id, name, hourlyWage, hoursWorked));
                break;
            case 3:
                id = getValidatedUniqueId(payroll, "Enter ID: ");
                name = getValidatedNameInput("Enter Name: ");
                paymentPerProject = getValidatedDoubleInput("Enter Payment Per Project: ");
                projectsCompleted = getValidatedIntInput("Enter Projects Completed: ");
                payroll.addEmployee(new ContractualEmployee(id, name, paymentPerProject, projectsCompleted));
                break;
            case 4:
                payroll.displayPayrollReport();
                break;
            case 5:
                std::cout << "Exiting...\n";
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}