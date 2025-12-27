#ifndef DATABASECONFIG_H
#define DATABASECONFIG_H

#include <iostream>
#include <memory>
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include "./libxlsxwriter/include/xlsxwriter.h"

using namespace std;

class Database;

// Base class for Row with a virtual destructor
class BaseCol
{
public:
    virtual string identify(string type = "title") = 0;
    virtual void addToColumn() = 0;
    virtual void deleteCell(int at) = 0;
    virtual void editCell(int at) = 0;
    virtual void bulkEdit() = 0;
    virtual void fillColumn() = 0;
    virtual void saveCell(int y, int x, lxw_worksheet *worksheet) = 0;
    virtual vector<int> filter(string query) = 0;
    virtual void printCell(int at) = 0;
    virtual ~BaseCol() = 0; // Virtual destructor
};

inline BaseCol::~BaseCol() {}

// Templated Row class
template <typename T>
class Column : public BaseCol
{                // Inherit from BaseCol
    string type; // Type of the Row
    vector<T> column;
    string title;
    int height; // starts from 1


    
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
            //cout << "Debug1: filterQuery: " << filterQuery << endl;
            //cout << "Debug1: input: " << input << endl;
            //cout << "Debug1: filterType: " << filterType << endl;
            //cout << "Debug1: filterQuery_T: " << filterQuery_T << endl;
            //cout << "Debug1: type: " << filterType << endl;
            //cout << "Debug1: input.find(filterQuery, 0): " << input.find(filterQuery, 0) << endl;

            if (filterType == "Sw" && type == "String" && input.find(filterQuery, 0) == 0)
            { // starts with, ends with, and contains only works with strings 
                //cout << "Debug12" << filterQuery << endl;
                //cout << "Debug12" << input << endl;
                //cout << "Debug12" << filterType << endl;
                //cout << "Debug12" << filterQuery_T << endl;
                return true;
            }
            else if (filterType == "Ew" && type == "String" && (input.rfind(filterQuery) + filterQuery.length() == input.length()))
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

        //cout << "Debug11" << filterQuery << endl;
        //cout << "Debug11" << input << endl;
        //cout << "Debug11" << filterType << endl;
        //cout << "Debug11" << filterQuery_T << endl;
        return false;
    }

    vector<int> filterHelper(string current, istringstream &iss, vector<int> &filteredColumn)
    {
        string filterQuery;
        vector<int> filtered;
        if (current[0] == '$')
        {
            current = current.substr(1);
        }
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

        return filtered;
    }

public:
    Column(string type, string title, int height, T value) : type{type}, title{title}, height{height}
    {
        column.emplace_back(value);
    };
    Column(string type, string title, int height) : type{type}, title{title}, height{height} {
        //column.resize(height);
    }; // Default constructor
    ~Column() {};

    // Column addition Function
    void addToColumn() override
    {
        T input;
        if (type == "String")
        {
            if constexpr (std::is_same_v<T, std::string>)
            {
                cin.ignore();
                getline(cin, input);
            }
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

    void fillColumn() override
    {
        T input;
        cin >> input;
        for (size_t i = 0; static_cast<int>(i) < height; i++)
        {
            if (i < column.size()) {
                // Index exists, just update it
                column[i] = input;
            } else {
                // Index doesn't exist, add it to the end
                column.emplace_back(input);
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
        //cout << "DEBUG 112" << endl;
        if(column.empty()){
            //cout << "DEBUG 11" << endl;
            cout << "  NULL";
            return;
        }
        cout << column[at];
    }

    
    void saveCell(int y, int x, lxw_worksheet *worksheet) override
    {
        //cout << "DEBUG 112" << endl;
        if(column.empty()){
            //cout << "DEBUG 11" << endl;
            cout << "  NULL";
            worksheet_write_string(worksheet, y, x, "NULL", NULL);
            return;
        }
        if constexpr (std::is_same_v<T, std::string>) {
            worksheet_write_string(worksheet, y, x, column[y].c_str(), NULL);
        } else if constexpr (std::is_same_v<T, bool>) {
            worksheet_write_boolean(worksheet, y, x, column[y], NULL);
        } else if constexpr (std::is_arithmetic_v<T>) {
            worksheet_write_number(worksheet, y, x, static_cast<double>(column[y]), NULL);
        } else {
            std::ostringstream oss;
            oss << column[y];
            std::string s = oss.str();
            worksheet_write_string(worksheet, y, x, s.c_str(), NULL);
        }
        cout << column[y];
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


    vector<int> filter(string query) override
    {
        string query_iss;
        //cout << "Debug11f" << query << endl;
        vector<int> columnFiltered;
        vector<int> blank;
        istringstream iss{query};
        while (iss >> query_iss)
        {
            //cout << "Debug161" << query_iss << endl;
            if (query_iss[0] == '$')
            {
                //cout << "Debug141" << query_iss[0] << endl;
                query_iss = query_iss.substr(1);
                // note on how this 'or' works, this 'or' works on the entire column and then adds whatever is new to the current filtered coolumn
                if (query_iss == "OR")
                {
                    iss >> query_iss; // This second one is to get the actual command which should follow from an 'and' or an 'or' as given by the format
                    vector<int> columnFilteredNew = filterHelper(query_iss, iss, blank);
                    // now we merge the vector we recently got from filterHelper and what we currently have
                    // Add everything from the new vector to the old one
                    columnFiltered.insert(columnFiltered.end(), columnFilteredNew.begin(), columnFilteredNew.end());
                    // Sort the vector
                    std::sort(columnFiltered.begin(), columnFiltered.end());
                    //3. Remove duplicatescolumnFiltered.erase(std::unique(columnFiltered.begin(), columnFiltered.end()), columnFiltered.end());
                    continue;
                    // note on how this 'and' works this 'and' works on the already filtered column and removes whatever doesnt fit the 'and's command
                }
                else if (query_iss == "AND")
                {
                    iss >> query_iss;
                }
                columnFiltered = filterHelper(query_iss, iss, columnFiltered);
            }
        }
        return columnFiltered;
    }
};

// Utility function to validate allowed types
bool typeAllowed(string type)
{
    return (type == "Integer" || type == "Boolean" ||
            type == "Float" || type == "Char" ||
            type == "String");
}

class Database
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

    void printDB(vector<int> n, bool emptyFine)
    {
        cout << "-.0.--.0.--.0.--.0.--.0.--Printing Database--.0.--.0.--.0.--.0.--.0.-" << endl;
        cout << "Database Name: " << dbName << endl;
        cout << "# of Cols: " << columns.size() << endl;
        cout << "# of Rows: " << height << endl; // Formatting here might suck a little bit
        cout << "Row Names: ";
        for (size_t i = 0; i < columns.size(); i++)
        {
            cout << columns[i]->identify() << "   ";
        }
        if (n.empty() && emptyFine)
        {
            for (int i = 0; i < height; i++)
            {
                cout << "\nRow " << i + 1 << ": " << "|  ";
                for (size_t j = 0; j < columns.size(); j++)
                {
                    //cout << "DEBUG 113" << endl;
                    columns[j]->printCell(i);
                    cout << "  |";
                }
            }
        }
        else
        {
            for (size_t i = 0; i < n.size(); i++)
            {
                cout << "\nRow " << i + 1 << ": " << "|  ";
                for (size_t j = 0; j < columns.size(); j++)
                {

                    columns[j]->printCell(n[i]);
                    cout << "  |  ";
                }
            }
        }
        cout << endl;
    }

    void saveDB(lxw_worksheet *worksheet)
    {
        worksheet_set_column(worksheet, 0, 0, 25, NULL);
        worksheet_write_string(worksheet, 0, 0, ("Database Name: " + dbName).c_str(), NULL);
        //cout << "Database Name: " << dbName << endl;
        //cout << "# of Cols: " << columns.size() << endl;
        worksheet_write_string(worksheet, 1, 0, ("# of Cols: " + std::to_string(columns.size())).c_str(), NULL);
        //cout << "# of Rows: " << height << endl; // Formatting here might suck a little bit
        worksheet_write_string(worksheet, 2, 0, ("# of Rows: " + std::to_string(height)).c_str(), NULL);
        //cout << "Row Names: ";
        for (size_t i = 0; i < columns.size(); i++)
        {
            //cout << columns[i]->identify() << "   ";
            worksheet_write_string(worksheet, 1, i + 1, (columns[i]->identify()).c_str(), NULL);
        }
        for (int i = 0; i < height; i++)
        {
            //cout << "\nRow " << i + 1 << ": " << "|  ";
            for (size_t j = 0; j < columns.size(); j++)
            {
                //cout << "DEBUG 113" << endl;
                columns[j]->saveCell(i + 1, j + 1, worksheet);
                //cout << "  |";
            }
        }
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
    bool addColumn(string title, string type, bool fill = false)
    {
        if (column.count(title) == 1)
        {
            cout << "Column Name already exists. Please try another!!!" << endl;
            return false;
        }
        if(!typeAllowed(type)){
            cout << "Invalid Type. Please Try again!!!" << endl;
            return false;
        }
        column[title] = type;
        unique_ptr<BaseCol> add = createCol(type, title);
        if(fill){
            add->fillColumn();
        }
        columns.emplace_back(std::move(add));
        return true;
    }

    void addRow()
    {
        for (auto &i : columns)
        {
            cout << i->identify() << "(" << i->identify("type") << ")" << ": ";
            i->addToColumn();
        }
    }

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
            if(height != 0){
                cout << "Enter fill for new column: "; // add an option for adding a fill and then a default for the field
                return addColumn(colName, type, true);
            }
            return addColumn(colName, type);// later we look at the logic of where we ask the user whether they want to provide a default fill or maybe a bulk fill for current data in the database . if we do this remember to increment height
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
            vector<int> blank;
            printDB(blank, true);
            return true;
            break;
        }
        case 9:
        {
            string cName;
            string query;
            string input;
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
            // cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            getline(cin, query);
            int cNum = colNameToInt(cName);
            if (cNum == -1)
            {
                cout << "Column doestn't exist." << endl;
                return false;
            }
            //cout << "Debug1d1" << query << endl;
            printDB(columns[cNum]->filter(query), false);
            return true;
        }
        default:
            return false;
        }
    }
    // This is the overriden version with type included
    // fill represents a value for bulk fillings
    //template <typename T>
    //bool addColumn(string title, string type, T fill)
    //{
    //    if (!typeAllowed(type) || column.count(title) == 1)
     //   {
    //        return false;
    //    }
    //    column[title] = type;
    //    if()
    //    unique_ptr<BaseCol> add = createCol(type, title);
    //    static_cast<Column<T> *>(add.get())->fillColumn();
    //    columns.emplace_back(std::move(add));
    //    return true;
    //}
};

#endif // DATABASECONFIG_H
