import <iostream>;
import <string>;
import <memory>;
import <fstream>;
import <vector>;
import DatabaseConfig;
using namespace std;
// They get tge chance to create a new db, choose from an existing list of db's to maybe add stuff to create new fields, they can delete
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
    cout << "You have chosen 'Delete Database'. Enter 'Y' to continue and 'N' to return to Main Menu" << endl;
    cin >> input_c;
    if (input_c == 'N' || input_c == 'n')
    {
        cout << "Redirecting..." << endl;
        handleMainMenu(databases);
        return;
    }
    // Maybe we can look at delay in cpp if there is any
    if (input_c != 'Y' && input_c != 'y')
    {
        cout << "Invalid Input, you've been redirected to the main menu" << endl;
        handleMainMenu(databases);
        return;
    }

    cout << "Enter the name of the database you'd like to delete(or return to go to Main Menu): ";
    cin >> dbName;
    if(dbName == "return"){
        handleMainMenu(databases);
    }
    int pos = findDB(databases, dbName);
    if(pos == NOT_FOUND){
        cout << "Invalid Database Name!!! Redirecting to Main Menu..." << endl;
        handleMainMenu(databases);
        return;
    }

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
        cout << "Database name not found. Please Try again" << endl;
        return renameDB(databases);
    }
    else
    {
        cout << "Enter your new Database Name: ";
        cin >> inputNewDB;

        if (findDB(databases, inputNewDB) != NOT_FOUND)
        {
            cout << "Database name already exists. Please try a different one" << endl;
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
    
    if(!(cin >> input_i) || input_i < 1 || input_i > 8){
        cout << "Invalid Option, Please try again: ";
        return modifyDB(databases);
    }
    bool returnValue = databases[dbFound]->modify(input_i);// var no longer used might use in future
    // false means either user selected to go back or add failed
    ///cout << returnValue << "Is" << endl;
    if(returnValue == true){
        cout << "Action Successful!!!" << endl;
    }
    cout << "Redirecting to Main Menu...." << endl;
    modifyDB(databases);
    
    
}

void viewDB(vector<unique_ptr<Database>> &databases)
{
    cout << "--------------------------VIEW MENU--------------------------" << endl;
    if (databases.empty())
    {
        cout << "You currently have no created Databases. Create one in Main Menu. \n Redirecting....\n" << endl;
    }
    cout << "Here are your current existing databases:" << endl; // when user logic is added we could do sth like databases for user and the user is
    for (size_t i = 0; i < databases.size(); i++)
    {
        cout << (i + 1) << ". Owner: " << databases[i]->identify("owner") << " Database Name: " << databases[i]->identify("name") << endl;
    }
    cout << "Redirecting to Main Menu....." << endl;
    cout << endl;
}

void handleMainMenu(vector<unique_ptr<Database>> &databases)
{    
    cout << "--------------------------MAIN MENU--------------------------" << endl;
    int input_i;
    cout << "Welcome to DB Create, your C++ Database Creation System." << endl;
    cout << "NOTE: NEVER hit Ctrl D use Ctrl C instead" << endl;
    cout << "Here are your available options: (Enter 1-5)" << endl;
    cout << "1. Create a New Database" << endl;
    cout << "2. Modify an existing database" << endl;
    cout << "3. View existing databases" << endl;
    cout << "4. Delete an existing database" << endl;
    cout << "5. Rename an existing database" << endl;
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
            cout << "\nInvalid input. Please enter an integer.";
            cin.clear();
            cin.ignore();
            continue;
        }

        if (input_i == 0)
        {
            cout << "Thank you, your changes have been saved!" << endl;
            return;
        }

        if (input_i < 0 || input_i > 5)
        {
            cout << "\nInvalid Option, Please try again: ";
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