export module DatabaseConfig;

import <iostream>;
import <memory>;
import <map>;
import <vector>;
import <sstream>;
import <string>;

using namespace std;

// Base class for Row with a virtual destructor
class BaseCol
{
public:
    virtual string identify(string type = "title") = 0;
    virtual void addToColumn() = 0;
    virtual void deleteCell(int at) = 0;
    virtual void editCell(int at) = 0;
    virtual void bulkEdit() = 0;
    virtual void filter(string query) = 0;
    virtual void printCell(int at) = 0;
    virtual ~BaseCol() = 0; // Virtual destructor
};

BaseCol::~BaseCol() {}

// Templated Row class
template <typename T>
class Column : public BaseCol
{                // Inherit from BaseCol
    string type; // Type of the Row
    vector<T> column;
    string title;
    int height; // starts from 1

public:
    Column(string type, string title, int height, T value) : type{type}, title{title}, height{height}
    {
        column.emplace_back(value);
    };
    Column(string type, string title, int height) : type{type}, title{title}, height{height} {}; // Default constructor
    ~Column() {};

    // Column addition Function
    void addToColumn() override
    {
        T input;
        // string input_s;
        if (type == "String")
        {
            cin.ignore();
            if constexpr (std::is_same_v<T, std::string>)
            {
                getline(cin, input);
            }
            // input = input_s;
        }
        else
        {
            cin >> input;
            if (!cin)
            {
                cin.clear();
                cin.ignore();
                cout << "Invaid Addition. Try again(" << type << "): ";
                addToColumn();
                return;
            }
        }
        column.emplace_back(input);
        height++;
    }
    //! remove from column Function
    void fillColumn(T value)
    {

        for (int i = 0; i < height; i++)
        {
            try
            {
                column.at(i);
                column[i] = value;
            }
            catch (...)
            {
                column.emplace_back(value);
            }
        }
    }

    string identify(string arg = "title") override
    {
        if (arg == "title")
        {
            return title;
        }
        else if (arg == "type")
        {
            return type;
        }
        return "";
    }

    void printCell(int at) override
    {
        cout << column[at];
    }
    void deleteCell(int at) override
    {
        int counter = 1;
        for (auto it = column.begin(); it < column.end(); it++)
        {
            if (counter == at)
            {
                column.erase(it);
                height--;
                return;
            }
            counter++;
        }
    }
    void editCell(int at) override
    {
        T newValue;
        cout << "Current Value: " << column[at] << endl;
        cout << "New Value: ";
        cin >> newValue;
        column[at] = newValue;
    }
    void bulkEdit() override
    {
        T newValue;
        cout << "Enter new value for column " << title << ": ";
        if (type == "String")
        {
            // cout << endl;
            cin.ignore();
            if constexpr (std::is_same_v<T, std::string>)
            {
                getline(cin, newValue);
            }
        }
        else
        {
            cin >> newValue;
            if (!cin)
            {
                cin.clear();
                cin.ignore();
                cout << "Invalid Input. Please try again!!!" << endl;
                bulkEdit();
                return;
            }
        }
        for (size_t i = 0; i < column.size(); i++)
        {
            column[i] = newValue;
        }
    }

    bool filterMet(string filterType, T input, string filterQuery)
    {
        T filterQuery_T;
        if (type != "String")
        {
            istringstream iss{filterQuery};
            iss >> filterQuery_T;
        }
        if constexpr (std::is_same_v<T, std::string>)
        {

            if (filterType == "Sw" && type == "String" && input.find(filterQuery, 0) == 0)
            { // starts with/ ends with/ contains only works with strings
                return true;
            }
            else if (filterType == "Ew" && type == "String" && input.find(filterQuery, 0) == (input.length() - filterQuery.length() - 1))
            {
                return true;
            }
            else if (filterType == "Ct" && type == "String" && input.find(filterQuery) != std::string::npos)
            {
                return true;
            }
        }

        else if (filterType == "Et" && input == filterQuery_T)
        {
            return true;
        }
        else if (filterType == "Lt" && input < filterQuery_T)
        {
            return true;
        }
        else if (filterType == "Gt" && input > filterQuery_T)
        {
            return true;
        }

        return false;
    }

    // make private
    // might need to add safeguards to ensure sw and ew only work with strings
    vector<int> filterHelper(string current, istringstream &iss, vector<int> filteredColumn)
    {
        string filterQuery;
        vector<int> filtered;
        if (current[0] == '$')
        {
            // bool met;
            current = current.substr(1);
            iss >> filterQuery;
            if (!iss)
            {
                return filtered;
            }
            if (filteredColumn.empty())
            {
                for (size_t i = 0; i < column.size(); i++)
                {
                    if (filterMet(current, column[i], filterQuery))
                    {
                        filtered.emplace_back(i);
                    }
                }
            }
            else
            {
                for (size_t i = 0; i < filteredColumn.size(); i++)
                {
                    if (filterMet(current, column[filteredColumn[i]], filterQuery))
                    {
                        filtered.emplace_back(i);
                    }
                }
            }
        }
        return filteredColumn;
    }

    void filter(string query) override
    {
        string query_iss;
        vector<int> columnFiltered;
        vector<int> blank;
        istringstream iss{query};
        while (iss >> query_iss)
        {
            if (query_iss[0] == '$')
            {
                query_iss = query_iss.substr(1);
                // note on how this 'or' works, this 'or' works on the entire column and then adds whatever is new to the current filtered coolumn
                if (query_iss == "OR")
                {
                    // vector<T> copy = columnFiltered;
                    iss >> query_iss; // This second one is to get the actual command which should follow from an 'and' or an 'or' as given by the format
                    vector<int> columnFilteredNew = filterHelper(query_iss, iss, blank);
                    continue;
                    // note on how this 'and' works this 'and' works on the already filtered column and removes whatever doesnt fit the 'and's command
                }
                else if (query_iss == "AND")
                {
                    cin >> query_iss;
                }
                columnFiltered = filterHelper(query_iss, iss, columnFiltered);
                // we might as well do the printing here
            }
        }
    }
};

// Utility function to validate allowed types
bool typeAllowed(string type)
{
    return (type == "Integer" || type == "Boolean" ||
            type == "Float" || type == "Char" ||
            type == "String");
}

export class Database
{
    string owner;                        // owner username
    string dbName;                       // database name
    string dbId;                         // database identification unique - we could use uuid too ig
    map<string, string> column;          //! Might be redundat              // This contains information about the columns being made in the database. Contains column name and type
    int height;                          // starts from 1
    vector<unique_ptr<BaseCol>> columns; // Store each Column as BaseCol pointers

    // This function is used for creating the Columns
    unique_ptr<BaseCol> createCol(string type, string title)
    {
        if (type == "Integer")
        {
            return make_unique<Column<int>>(type, title, height); // Create Column<int>
        }
        else if (type == "Boolean")
        {
            return make_unique<Column<bool>>(type, title, height); // Create Column<bool>
        }
        else if (type == "Float")
        {
            return make_unique<Column<float>>(type, title, height); // Create Column<float>
        }
        else if (type == "Char")
        {
            return make_unique<Column<char>>(type, title, height); // Create Column<char>
        }
        else if (type == "String")
        {
            return make_unique<Column<string>>(type, title, height); // Create Column<string>
        }
        return nullptr; // Safety return but should technically not be reached
    }

public:
    Database(string owner, string dbName) : owner{owner}, dbName{dbName}
    {
        dbId = owner + "_" + dbName;
        height = 0;
    }

    string identify(string type = "id")
    {
        if (type == "id")
        {
            return dbId;
        }
        else if (type == "owner")
        {
            return owner;
        }
        else if (type == "name")
        {
            return dbName;
        }
        return "";
    }

    void setDBName(string newName)
    {
        dbName = newName;
    }

    // title represents column name/title, type represents column type(lowercase) e.g integer
    // return types: false means the add was invalid maybe because of duplicate column name, or wrong column type, true is add was successful
    // #alteration: we could also include column allowances like can this column be empty for example
    bool addColumn(string title, string type)
    {
        // cout <<"Hello" << endl;
        if (!typeAllowed(type) || column.count(title) == 1)
        {
            cout << "Column Name already exists. Please try another!!!" << endl;
            return false;
        }
        // cout <<"Ho" << endl;
        column[title] = type;
        // cout <<"Helllo" << endl;
        unique_ptr<BaseCol> add = createCol(type, title);

        // Resize rows vector and add the new row
        // columns.push_back(vector<unique_ptr<BaseCol>>());
        columns.emplace_back(std::move(add));

        return true;
    }

    // we add a new row and print success and also the new row id, ////but dont modify it yet
    void addRow()
    { // for now no return is added at this point to prevent complicating things
        for (auto &i : columns)
        {
            cout << i->identify() << "(" << i->identify("type") << ")" << ": ";
            i->addToColumn();
            // cout << endl;
        }
    }

    // returns -1 if column is not found else return the location of the column;
    int colNameToInt(string colName)
    {
        int id = 0;
        for (auto &cols : columns)
        {
            if (cols->identify() == colName)
            {
                return id;
            }
            id++;
        }
        return -1;
    }
    // This function performs the modify options given in modifyDB(in main)

    // false means either user selected to go back or add failed
    bool modify(int i)
    {
        switch (i)
        {
        case 1:
        {
            string colName;
            string type;
            cout << "Enter new column name(or 'return' to go to Modify Menu): ";
            cin.ignore();
            getline(cin, colName);
            if (colName == "return")
            {
                return false;
            }
            cout << "Next choose from one of the column types below" << endl;
            cout << "1. String\n2. Integer\n3. Boolean\n4. Float\n5. Char\n"
                 << endl;
            cout << "Enter the Column type, as above(case sensitive): ";
            cin >> type;
            return addColumn(colName, type); // later we look at the logic of where we ask the user whether they want to provide a default fill or maybe a bulk fill for current data in the database . if we do this remember to increment height
        }
        case 2:
        {
            // Add the new row
            cout << "To add a new row, you must fill each required field." << endl;
            if (columns.empty())
            {
                cout << "No Columns = No Rows" << endl;
                return false;
            }
            addRow();
            height++;
            cout << "Row successfully added!!!" << endl;
            return true;
            break;
        }
        case 3:
        {
            int input_i;
            cout << "Enter the number of the Row to erase: ";
            cin >> input_i;
            if (input_i > height || input_i < 1)
            {
                cout << "Invalid Row Number." << endl;
                return false;
            }
            for (size_t i = 0; i < columns.size(); i++)
            {
                columns[i]->deleteCell(input_i);
            }
            height--;
            return true;
        }
        case 4:
        {
            string input_s;
            cout << "Enter column name to delete: ";
            cin >> input_s;
            for (auto it = columns.begin(); it != columns.end(); i++)
            {
                if ((*it)->identify() == input_s)
                {
                    columns.erase(it);
                    return true;
                }
            }
            return false;
        }
        case 5:
        {
            string input_s;
            int input_i;
            cout << "Enter the column name you would like to edit: ";
            cin >> input_s;
            cout << endl;
            cout << "Enter the Row Number you would like to edit: ";
            cin >> input_i;
            cout << endl;
            int id = colNameToInt(input_s);
            if (id == -1)
            {
                cout << "Invalid Column Name, please try again!!!" << endl;
                return false;
            }
            if (input_i > height || input_i < 1)
            {
                cout << "Invalid Row Number, please try again!!!" << endl;
                return false;
            }
            columns[id]->editCell(input_i);
            cout << "Edit Successful. Redirecting to Main Menu...";
            return true;
        }
        case 6:
        {
            string input_oldName;
            string input_newName;

            cout << "Enter column name to edit: ";
            cin >> input_oldName;
            if (column.count(input_oldName) != 1)
            {
                cout << "Column name does not exists, please try another one!!!" << endl;
                return modify(i);
            }
            cout << "Enter column's new name";
            cin >> input_newName;
            if (column.count(input_newName) == 1)
            {
                cout << "Column already exists, please try another one!!!" << endl;
                return modify(i);
            }
            column[input_newName] = column[input_oldName];
            column.erase(input_oldName);
            return true;
        }
        case 7:
        {
            string input_name;
            string input_value;
            cout << "Enter column name to bulk edit: ";
            cin.ignore();
            getline(cin, input_name);
            // cout << "Enter new value for column " << input_name << ": " << endl;
            // getline(cin, input_value);
            int id = colNameToInt(input_name);
            if (id == -1)
            {
                cout << "Invalid Column Name. Please try another one!!!" << endl;
                return false;
            }
            columns[id]->bulkEdit();
            return true;
        }
        case 8:
        {
            //! We segfault if we add a new column recently unfilled and we havent yet filled
            cout << "-.0.--.0.--.0.--.0.--.0.--Printing Database--.0.--.0.--.0.--.0.--.0.-" << endl;
            cout << "Database Name: " << dbName << endl;
            cout << "# of Cols: " << columns.size() << endl;
            cout << "# of Rows: " << height << endl; // Formatting here might suck a little bit
            cout << "Row Names: ";
            for (size_t i = 0; i < columns.size(); i++)
            {
                cout << columns[i]->identify() << "   ";
            }
            for (int i = 0; i < height; i++)
            {
                cout << "\nRow " << i + 1 << ": " << "|  ";
                for (size_t j = 0; j < columns.size(); j++)
                {

                    columns[j]->printCell(i);
                    cout << "  |";
                }
            }
            cout << endl;
            return true;
            break;
        }
        case 9:
        {
            string cName;
            string query;
            string input;
            // vector
            cout << "Enter Column Name to filter below: " << endl;
            cin.ignore();
            getline(cin, cName);
            cout << "Here are your current filter options: " << endl;
            cout << "1. Starts with(Sw)" << endl;
            cout << "2. Ends with(Ew)" << endl;
            cout << "3. Equals to(Et)" << endl;
            cout << "4. Less than(Lt)" << endl;
            cout << "5. Greater than(Gt)" << endl;
            cout << "6. Contains(Ct)" << endl;
            cout << "Filter query style: $cmd value $OR $cmd value $AND $cmd value" << endl;
            cout << "E.g.                $Sw    a  $AND  $Ew   B " << endl;
            cout << "Enter query below: " << endl;
            cin.ignore();
            getline(cin, query);
            int cNum = colNameToInt(cName);
            if (cNum == -1)
            {
                cout << "Column doestn't exist." << endl;
                return false;
            }
            columns[cNum]->filter(query);
            return true;
        }
        default:
            return false;
        }
    }

    // This is the overriden version with type included
    // fill represents a value for bulk fillings
    template <typename T>
    bool addColumn(string title, string type, T fill)
    {
        if (!typeAllowed(type) || column.count(title) == 1)
        {
            return false;
        }
        column[title] = type;
        unique_ptr<BaseCol> add = createCol(type, title);
        static_cast<Column<T> *>(add.get())->fillColumn(fill); // Cast to correct Row type
        columns.emplace_back(std::move(add));
        return true;
    }
};

//! need to fix when a new column is added into a database that is already filled and we try to print we segfault i know why I just have to find the best fix for it