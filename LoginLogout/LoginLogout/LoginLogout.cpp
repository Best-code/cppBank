#include <iostream>
#include <string>
#include <sqlite3.h>
#include <ctime>

bool loggedIn = false;

#define cls system("CLS");

bool passCorrect = false;
std::string fromPass;
std::string toPass;

std::string xname;
std::string xlastName;
std::string xusername;
std::string xpass;
std::string xlastSignIn;

float xbalance;

static int callback(void* unused, int count, char** data, char** columns)
{
    int idx;


    fromPass = data[0];
    if (toPass == fromPass)
        passCorrect = true;
    printf("\n");

    return 0;
}

static int balancc(void* unused, int count, char** data, char** columns)
{
    int idx;
    
    xbalance = atof(data[0]);

    printf("\n");

    return 0;
}


static int callUser(void* unused, int count, char** data, char** columns)
{
    int idx;

    xusername = data[1];
    xpass = data[2];
    xname = data[3];
    xlastName = data[4];
    xlastSignIn = data[5];
    xbalance = atof(data[6]);
 
    printf("\n");

    return 0;
}


void runSQL(std::string& sql, int call)
{
    sqlite3* DB;
    char* errorMsg;

    int exit = sqlite3_open("uandp.db", &DB);

    if (call == 1) {
        sqlite3_exec(DB, sql.c_str(), callback, 0, &errorMsg);
    }
    else if (call == 2)
    {
        sqlite3_exec(DB, sql.c_str(), balancc, 0, &errorMsg);
    }
    else if (call == 3)
    {
        sqlite3_exec(DB, sql.c_str(), callUser, 0, &errorMsg);

    }

    sqlite3_close(DB);
}


class User
{
public:
    std::string name = xname;
    std::string lastName = xlastName;
    std::string username = xusername;
    std::string pass = xpass;
    std::string lastSignIn = xlastSignIn;

    float balance = xbalance;

    void baseFunction()
    {
        int a;
        std::cin >> a;
        cls
            std::cout << "                Your last sign in was : " << lastSignIn << std::endl << std::endl;
        std::string mr = "What would you like to do today ";
        const char* e = R"(

withdraw > 1

Deposit > 2

Check Balance > 3

Log-Out > 4

)";
        std::cout << mr << name << e;
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            withdraw();
        }
        if (choice == 2) {
            deposit();
        }
        if (choice == 3) {
            checkBalance(true);
        }if (choice == 4) {
            Logout();
        }

    }

    void withdraw()
    {

        

        cls
        std::cout << "How much would you like to withdraw from your current balance of : " << balance << std::endl;
        std::cout << "Type 0 to cancel!" << std::endl;
        bool x = false;
        int amt;
        while (!x) {
            std::cin >> amt;
            if (amt <= balance) {
                balance -= amt;
                x = true;
            }
            else {
                cls
                std::cout << "You must not withdraw more than your current balance of : " << balance << " \nTry Again > ";
            }
        }

        std::string sql("UPDATE user SET balance = " + std::to_string(balance) + " WHERE username = \"" + username + "\"");
        runSQL(sql, 2);

        checkBalance(false);
        

        balance = xbalance;

        std::cout << std::endl << std::endl << "Type any key to go back to the main menu.";

        baseFunction();
    }

    void deposit()
    {
        

        cls
            std::cout << "How much would you like to deposit toward your current balance of : " << balance << std::endl;
        std::cout << "Type 0 to cancel!" << std::endl;

        int amt;
        std::cin >> amt;
        balance += amt;

        std::string sql("UPDATE user SET balance = "+std::to_string(balance)+" WHERE username = \""+username+"\"");
        runSQL(sql, 2);

        checkBalance(false);


        balance = xbalance;

        std::cout << std::endl << std::endl << "Type any key to go back to the main menu.";

        baseFunction();

    }

    void checkBalance(bool n)
    {
        
        std::string sql("SELECT balance FROM user WHERE username = '" + username+"';");
        runSQL(sql, 2);

        balance = xbalance;

        if (!n)
            std::cout << "Your new balance is : " << balance;

        

        if (n) {
            cls
                std::cout << name << ", your current balance is : " << balance;
            std::cout << std::endl << std::endl << "Type any key to go back to the main menu.";

            baseFunction();
        }
    }

    void Logout() {
        abort();
    }


};

class LogIn
{
public:
    void reg()
    {
        std::string firstName;
        std::string lastName;
        std::string username;
        std::string password;
        std::string conPass;
        bool bPass = false;
        cls
            std::cout << "Register Page - Maloney Bank" << std::endl;
        std::cout << std::endl << "Enter your first name > ";
        std::cin >> firstName;
        std::cout << std::endl << "Enter your Last name > ";
        std::cin >> lastName;
        std::cout << std::endl << "Enter a Username > ";
        std::cin >> username;
        while (!bPass) {
            std::cout << std::endl << "Enter a Password > ";
            std::cin >> password;
            std::cout << std::endl << "Retype to Confirm Password > ";
            std::cin >> conPass;
            if (conPass != password) {
                std::cout << "Your confirmation was incorrect" << std::endl;
            }
            else
                bPass = true;
            std::cout << std::endl;
        }
        std::cout << std::endl << "Type any key to submit";
        std::cin.get();
        addUser(username, password, firstName, lastName);
    }

    void Login()
    {
        std::string username;
        std::string password;
        while (!passCorrect) {
            cls
                std::cout << "Login Page - Maloney Bank" << std::endl << std::endl;
            std::cout << "Enter your Username > ";
            std::cin >> username;
            std::cout << std::endl << "Enter your Password > ";
            std::cin >> password;
            toPass = password;
            std::cin.get();
            loginUser(username, password);
            if (!passCorrect) {
                std::cout << std::endl << "Your usename or Password was incorrect. Try Again" << std::endl;
                std::cin.get();
            }
        }
        User newUser;
        newUser.baseFunction();

    }


    int addUser(std::string& user, std::string& pass, std::string& name, std::string& lastName)
    {

        std::string sql("INSERT INTO user(username, password, name, lastName) VALUES('"+user+"','"+pass+"','"+name+"','"+lastName+"');");
        runSQL(sql, 1);

        return 0;
    }

    int loginUser(std::string& user, std::string& pass)
    {
        //Disables so theres no error
#pragma warning(disable : 4996)
        time_t tt;
        time(&tt);
        tm TM = *localtime(&tt);

        //Must add 1 to month and 1900 to the year
        int month = TM.tm_mon + 1;
        int day = TM.tm_mday;
        int year = TM.tm_year + 1900;
        int hour = TM.tm_hour;
        int min = TM.tm_min;
        //No military time
        hour = ((hour > 12) ? hour - 12 : hour);
        //                                Adding a 0 if the min is less than 10 so its X:09 not X:9
        std::string lastOn = std::to_string(hour) + ":"+ ((min<10) ? "0":"") + std::to_string(min) + " on " + std::to_string(month) + "-" + std::to_string(day) + '-' + std::to_string(year);

        std::string sql("SELECT password FROM user WHERE username = '" + user + "';"+ "UPDATE user SET lastSignIn = '" + lastOn + "' WHERE username = '" + user + "';");
        runSQL(sql, 1);
        
        sql = "SELECT * FROM user WHERE username = '" + user+"'";

        runSQL(sql, 3);

        return 0;
    }


};

class Welcome
{
    LogIn log;
public:

    void welcomeMessage()
    {
        std::cout << R"(Welcome to Maloney Bank

Would you like to Log-in or Register?
------------------------
Log-in > Type 1
Register > Type 2
)";
        int which;
        std::cin >> which;
        ((which == 1) ? log.Login() : log.reg());
    }
};



int main()
{
    Welcome welc;
    User pu;
    welc.welcomeMessage();
    std::cin.get();
}

