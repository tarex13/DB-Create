import <iostream>;
import <string>;
import <fstream>;
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

void handleMainMenu();

void deleteDB(){
    char input_c;
    cout << "You have chosen 'Delete Database'. Enter 'Y' to continue and 'N' to return to Main Menu" << endl;
    cin >> input_c;
    if(input_c == 'N' || input_c == 'n'){
        handleMainMenu();
    }
    // Maybe we can look at delay in cpp if there is any
    if(input_c != 'Y' || input_c != 'y'){
        cout << "Invalid Input, you've been redirected to the main menu" << endl;
        handleMainMenu();
    }

    // code for deleting the database maybe we first startoff with showing the database records, maybe view existing database and this can share something
}

void createDB(){
    
}

void renameDB(){

}

void modifyDB(){

}

void viewDB(){

}

void handleMainMenu(){
    int input_i;

    cout << "Welcome to DB Create, your C++ Database Creation System." << endl;
    cout << "Here are your available options: (Enter 1-5)" << endl;
    cout << "1. Create a New Database" << endl;
    cout << "2. Modify an existing database" << endl;
    cout << "3. View existing databases" << endl;
    cout << "4. Delete an existing database" << endl;
    cout << "5. Rename an existing database" << endl;
    cout << "Enter Option(1-5) Here: ";
    // maybe we could add a reorder column option? might be more difficult since we are currently using maps for storing info
    while(!(cin >> input_i)){
        cout << "\nInvalid Option, Please try again(or enter 0 to break): ";
    }
    if(input_i == 0){
        cout << "Thank you, your changes(if any) have been saved!!!!" << endl;
    }
    switch (input_i)
    {
    case 1:
        createDB(); // create a new database
        break;
    case 2:
        modifyDB(); // modify existing databases
        break;
    case 3:
        viewDB(); // view existing databases
        break;
    case 4:
        deleteDB(); // delete existing databases
        break;
    case 5:
        renameDB(); // rename an existing database
        break;
    default:
        return;
    }
}


int main(){
    // maybe add some command line options like disableSignIn/superuser or something which this option will grant a user access to add databases amd a;; sorts of
    // rights and priviledges
    handleMainMenu();
}