
#include "lexers/lexers.h"

#include <exception>
#include <iostream>

using namespace std;

int main(int argn, char **args){
    cout << " ---- Scalc V0.0.1 ----" << endl;
    cout << "$ ";

    string s_in = "";
    string buffer = "";
    bool console_input = true;

    if(argn > 1){
        s_in = string(args[1]);
        console_input = false;
    }

    shared_ptr<Scope> global_scope = make_shared<Scope>();

    while(true){
        if(console_input)
            getline(cin, s_in);
        buffer += s_in;

        if(buffer.size() > 0 && (*(buffer.end()-1) == '\\')){
            buffer +="\n"; 
            continue;
        }
        if(buffer.size() > 1 && (*(buffer.end()-2) == '\\' && *(buffer.end()-1) == '\r')){
            buffer +="\n"; 
            continue;
        }

        try{
            if(buffer.size() > 1){
                Statement_machine sm = Statement_machine::string_to_machine(buffer, global_scope);
                Eval_value v = sm.run_machine();

                if(v.number.number_type != Number::Number_type::INVALID)
                    cout << v.to_string() << endl;

                cout << flush;
            }
            cout << "$ ";
        } catch(exception e){
            cout << " ERROR!" << endl;
            if(e.what())
                cout << e.what() << endl;
            cout << "$ ";
        }
        s_in = "";
        buffer = "";

        cout << flush;
        if(!console_input)
            break;
    }

}