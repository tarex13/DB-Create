#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <vector>
#include "DatabaseConfig.h"
#include "./libxlsxwriter/include/xlsxwriter.h"

#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define RESET   "\033[0m"      /* RESET */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */

using namespace std;
// They get the chance to create a new db, choose from an existing list of db's to maybe add stuff to create new fields, they can delete
// fields, entrys, edit an entry fields maybe thgouh column number or column name
// They cn perform group actions: e.g name start with, then we show them how many found and then we can do maybe
// a bulk delete, bulk edit, etc
// database names should  be unique and no space
// maybe add an option that if the user presses(maybe we can implement a response for a ctrl cmd) a key or enters sth we take them to the beginning options
// We could also add a back/option for the user to go back a previous menu page
// Maybe we could include database roles e.g admin which would branch into the admin adding people and this people can only have certain rights like e.g only admin can
// delete a database and then in view database we could show the fields, the users and also their roles and maybe view database s different based on roles
// If we allow people to just add a column we could give the option of filling up the fields
// 'return' is the keyword for moving back to previous menu
const int NOT_FOUND = -1;

void handleMainMenu(vector<unique_ptr<Database>> &databases);

// findDB locates the database called <name> and returns it's index or NOT_FOUND
int findDB(vector<unique_ptr<Database>> &databases, string name)
{
    for (size_t i = 0; i < databases.size(); i++)
    {
        if (databases[i]->identify("name") == name)
        {
            return i;
        }
    }
    return NOT_FOUND;
}

void deleteDB(vector<unique_ptr<Database>> &databases)
{

    cout << "--------------------------DELETE MENU--------------------------" << endl;
    char input_c;
    string dbName;
    cout << "You have chosen 'Delete Database'. Enter 'Y' to continue and 'N' to return to Main Menu: " ;
    cin >> input_c;
    if (input_c == 'N' || input_c == 'n')
    {
        cout << YELLOW << "Redirecting..." << YELLOW << endl;
        
        return;
    }
    // Maybe we can look at delay in cpp if there is any
    if (input_c != 'Y' && input_c != 'y')
    {
        cout << RED <<  "Invalid Input, you've been redirected to the main menu" << RESET << endl;
        
        return;
    }

    cout << "Enter the name of the database you'd like to delete(or return to go to Main Menu): ";
    cin >> dbName;
    if (dbName == "return")
    {
        return;
    }
    // int pos = findDB(databases, dbName);
    // if (pos == NOT_FOUND)
    // {
    //     cout << "Invalid Database Name!!!\n Redirecting to Main Menu...\n" << endl;
    //     handleMainMenu(databases);
    //     return;
    // }

    for(auto it = databases.begin(); it != databases.end(); it++){
        if((*it)->identify("name") == dbName){
            databases.erase(it);
            cout << GREEN << "Database Successfully Deleted. Redirecting to Main Menu..." << RESET << endl;
            return;
        }
    }
    cout << RED << "Invalid Database Name!!!\n Redirecting to Main Menu...\n" << RESET << endl;
    return;
    // code for deleting the database maybe we first startoff with showing the database records, maybe view existing database and this can share something
}

unique_ptr<Database> createDB(vector<unique_ptr<Database>> &databases)
{
    cout << "--------------------------CREATE MENU--------------------------" << endl;
    string input_s;
    cout << "To go to main menu, enter 'return'" << endl;
    cout << "Enter the New DataBase (Unique) Name(or return): ";
    cin >> input_s;
    if (input_s == "return")
    {
        handleMainMenu(databases);
        return unique_ptr<Database>{nullptr};
    }
    if (findDB(databases, input_s) != NOT_FOUND)
    {
        cout << "Database name already exists. Please try a different one" << endl;
        return createDB(databases);
    }

    cout << "Database successfully created.\nGo To Main Menu to modify new database. \nRedirecting to Main Menu...." << endl;
    return make_unique<Database>("admin", input_s);
}

void renameDB(vector<unique_ptr<Database>> &databases)
{
    cout << "--------------------------RENAME DATABASE MENU--------------------------" << endl;
    string inputOldDB;
    string inputNewDB;
    int dbFound;
    cout << "To go to main menu, enter 'return'" << endl;
    cout << "Enter the name of the DataBase to be rename(or return): ";
    cin >> inputOldDB;
    if (inputOldDB == "return")
    {
        handleMainMenu(databases);
        return;
    }
    dbFound = findDB(databases, inputOldDB);
    if (dbFound == NOT_FOUND)
    {
        cout << "Database name not found. Please Try again\n" << endl;
        return renameDB(databases);
    }
    else
    {
        cout << "Enter your new Database Name: ";
        cin >> inputNewDB;

        if (findDB(databases, inputNewDB) != NOT_FOUND)
        {
            cout << "Database name already exists. Please try a different one\n" << endl;
            return renameDB(databases);
        }
        databases[dbFound]->setDBName(inputNewDB);
    }
}

void modifyDB(vector<unique_ptr<Database>> &databases)
{
    string input_s;
    int dbFound;
    int input_i;
    cout << "--------------------------MODIFY MENU--------------------------" << endl;
    cout << "Note: To view available databases enter 'return' to go to main menu" << endl;
    cout << "Enter the name of the Database you want to modify(or return):";
    cin >> input_s;
    if (input_s == "return")
    {
        cout << "Redirecting to Main Menu...\n" << endl;
        handleMainMenu(databases);
        return;
    }
    dbFound = findDB(databases, input_s);
    if (dbFound == NOT_FOUND)
    {
        cout << "Database name not found. Please Try again" << endl;
        return modifyDB(databases);
    }
    cout << "Editing Database '" << databases[dbFound]->identify("name") << "'" << endl;
    cout << "Here are your available options: (Enter 1-9)" << endl;
    cout << "1. Add a New Column" << endl;
    cout << "2. Add a New Row" << endl;
    cout << "3. Delete a Row(Use Filter to find ID beforehand if unknown)" << endl;
    cout << "4. Delete a Column" << endl;
    cout << "5. Edit a Row(Use Filter to find ID beforehand if unknown)" << endl;
    cout << "6. Edit a Column Name" << endl;
    cout << "7. Bulk Edit A column" << endl; // for bulk edit we can give options for edit all or edit by filter through things like equal, greater than, less than, starts with, ends with, etc
    cout << "8. Print DataBase" << endl;
    cout << "9. DataBase Filter" << endl;
    cout << "Enter Option(1-9) Here: ";

    if (!(cin >> input_i) || input_i < 1 || input_i > 9)
    {
        cout << RED << "Invalid Option, Please try again: " << RESET;
        return modifyDB(databases);
    }
    bool returnValue = databases[dbFound]->modify(input_i); // var no longer used might use in future
    // false means either user selected to go back or add failed
    /// cout << returnValue << "Is" << endl;
    if (returnValue == true)
    {
        cout << GREEN << "Action Successful!!!\n" << RESET << endl;
    }
    cout << "Redirecting to MODIFY Menu....\n" << endl;
    modifyDB(databases);
}

void viewDB(vector<unique_ptr<Database>> &databases)
{
    cout << "--------------------------VIEW MENU--------------------------" << endl;
    if (databases.empty())
    {
        cout << YELLOW <<  "You currently have no created Databases. Create one in Main Menu. \nRedirecting to Main Menu....\n" << RESET << endl;
        return;
    }
    cout << "Here are your current existing databases:" << endl; // when user logic is added we could do sth like databases for user and the user is
    for (size_t i = 0; i < databases.size(); i++)
    {
        cout << (i + 1) << ". Owner: " << databases[i]->identify("owner") << " Database Name: " << databases[i]->identify("name") << endl;
    }
    cout << "Redirecting to Main Menu....." << endl;
    cout << endl;
}

void saveDB(vector<unique_ptr<Database>> &databases){
    
    lxw_workbook  *workbook  = workbook_new("Database.xlsx");
    for(auto &db : databases){   
        const string fileName = (db->identify("name")) + ".xlsx";

        lxw_format *header = workbook_add_format(workbook);
        lxw_format *bold = workbook_add_format(workbook);
        
        // formatting for the table
        format_set_font_color(header, LXW_COLOR_WHITE);
        format_set_bold(bold);
        format_set_bold(header);
        format_set_align(header, LXW_ALIGN_CENTER);
        format_set_align(header, LXW_ALIGN_VERTICAL_CENTER);
        format_set_fg_color(header, 0x808080);

        lxw_worksheet *worksheet = workbook_add_worksheet(workbook, fileName.c_str());
        db->saveDB(worksheet, header, bold);
    }
    workbook_close(workbook);
    cout << "Action Complete" << endl;
    return;
}

void handleMainMenu(vector<unique_ptr<Database>> &databases)
{
    cout << "--------------------------MAIN MENU--------------------------" << endl;
    int input_i;
    cout << "Welcome to DB Create, your C++ Database Creation System." << endl;
    cout << BOLDMAGENTA << "NOTE: NEVER hit Ctrl D use Ctrl C instead" << RESET << endl;
    cout << "Here are your available options: (Enter 1-5)" << endl;
    cout << "1. Create a New Database" << endl;
    cout << "2. Modify an existing database" << endl;
    cout << "3. View existing databases" << endl;
    cout << "4. Delete an existing database" << endl;
    cout << "5. Rename an existing database" << endl;
    cout << "6. Save Current Work to Excel" << endl;
    // maybe we could add a reorder column option? might be more difficult since we are currently using maps for storing info
    while (true)
    {
        cout << "\nEnter Option(1-5) Here (0 to exit): ";

        // Try to read the input and check if it failed
        if (!(cin >> input_i))
        {
            // If it failed because of EOF (Ctrl+D), break immediately
            if (cin.eof())
            {
                break;
            }

            // If it failed because of bad formatting
            cout << RED << "\nInvalid input. Please enter an integer." << RESET;
            cin.clear();
            cin.ignore();
            continue;
        }

        if (input_i == 0)
        {
            cout << "Enter 1 to save your changes or 0 to exit without saving: ";
            cin >> input_i;
            if(input_i == 1){
                cout << "Thanks for using DB Create. Saving changes now..." << endl;
                saveDB(databases);
                return;
            }
            cout << BOLDMAGENTA << "Thanks for using DB Create your changes have been discarded" << RESET << endl;
            return;
            return;
        }

        if (input_i < 0 || input_i > 6)
        {
            cout << RED <<  "\nInvalid Option, Please try again: " << RESET;
            continue;
        }
        break;
    }
    switch (input_i)
    {
    case 1:
    {
        unique_ptr<Database> dbNew = createDB(databases); // create a new database
        if (dbNew)
        {
            databases.emplace_back(std::move(dbNew));
        }
        handleMainMenu(databases);
        break;
    }
    case 2:
    {
        modifyDB(databases); // modify existing databases
        break;
    }
    case 3:
    {
        viewDB(databases); // view existing databases
        handleMainMenu(databases);
        break;
    }
    case 4:
    {
        deleteDB(databases); // delete existing databases
        break;
    }
    case 5:
    {
        renameDB(databases); // rename an existing database
        handleMainMenu(databases);
        break;
    }
    case 6:
    {
        saveDB(databases); // rename an existing database
        handleMainMenu(databases);
        break;
    }
    default:
        return;
    }
}

int main()
{
    // maybe add some command line options like disableSignIn/superuser or something which this option will grant a user access to add databases amd a;; sorts of
    // rights and priviledges
    vector<unique_ptr<Database>> databases;
    handleMainMenu(databases);
}

// maybe have an option for setworkingDB so they dont have to choose a database with each action they take