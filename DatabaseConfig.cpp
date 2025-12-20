export module DatabaseConfig;

import <string>;
import <map>;
import <vector>;
import <memory>;

using namespace std;

bool typeAllowed(string type){
    switch (type)
    {
    case "Integer":
        return true;
    
    case "Boolean":
        return true;
    
    case "Float":
        return true;
    case "Char":
        return true;
    case "String":
        return true;
    case "Float":
        return true;
    default:
        return false;
    }
}

template <typename T>
export class Row{

    string type;// maybe make as an enum but choices include integer, string, char, bool, float, double thats all for now
    T value; 
    public:
    Row(string type, T value): type{type}, value{value}{};
    Row(string type): type{type}{};//# for now values can be default constructed but later might add an empty field to row
    ~Row(){};
    // Value Setter Function
    void setValue(T value){
        value = value;
    }
}

export class Database{
    map<string, string> column; // This contains information about the columns being made in the database. Contains column name and type
    vector<vector<unique_ptr<Row>>> rows; // This vector would store each row as a row class Type. #Needs MIL CTOR since Row contains no default;

    // This function is used for creating the rows
    unique_ptr<Row> createRow(string type){
        switch (type)
        {
            case "Integer":
                return unique_ptr<Row>{new Row<int>{type}};
            case "Boolean":
                return unique_ptr<Row>{new Row<bool>{type}};
            case "Float":
                return unique_ptr<Row>{new Row<float>{type}};
            case "Char":
                return unique_ptr<Row>{new Row<char>{type}};
            case "String":
                return unique_ptr<Row>{new Row<string>{type}};
            case "Float":
                return unique_ptr<Row>{new Row<float>{type}};
            default:
                return nullptr; // this shouldnt be reached due to all the safeguards in addColumn but we could use this for better safety
        }
    }
    // no need for a ctor default is fine alter if needed
    public:

    // title represents column name/title, type represents column type(lowercase) e.g integer
    // return types: false means the add was invalid maybe because of duplicate column name, or wrong column type, true is add was successful
    // #alteration: we could also include column allowances like can this column be empty for example
    bool addColumn(string title,string type){
        if(!typeAllowed(column) || column.count(title) == 1){
            return false;
        }
        column[title] = type;
        unique_ptr<Row> add = createRow(type);

        for(auto p : rows){
            p.emplace_back(std::move(add));
        }
        return true;
    }
    
    // This is the overriden version with type included
    // fill represents a value for bulk fillings
    template <typename T>
    bool addColumn(string title,string type,T fill){
        if(!typeAllowed(column) || column.count(title) == 1){
            return false;
        }
        column[title] = type;
        unique_ptr<Row> add = createRow(type);
        add->setRow(fill);
        for(auto p : rows){
            p.emplace_back(std::move(add));
        }
        return true;
    } 

};

// Templated type class that stores the rows, you would require one instantiation of this class for each row to store in the vector


// SHould we perhaps utilize the decorator pattern
// We could maybe for printing but generally it isnt needed here