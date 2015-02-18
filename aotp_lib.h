#include <iostream>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <fstream>
#include "string.h"
#include "vector"
using namespace std;

int index_of_request = 0; //iterator of the initial message sent by the client

string get_message(string& b)
{
    cout<<"the received string in aotp_lib.get_message is:"<<b<<endl;
    int i = 0;
    string msg; msg.clear();
    while(b[i]!=' ')   //loop for collecting the message from request
    {
        msg += b[i];
        i++;
    }
    cout<<msg<<endl;
    index_of_request = i;
    index_of_request = index_of_request+1;
    return msg;
}

vector<double> get_operands(string& a)
{
    cout<<"the received request in the get_operands is:"<<a<<"and its size is:"<<a.size()<<endl;
    int j = 0;
    vector<double> operands;
    string operand;
    operands.clear();
    operand.clear();
    while (index_of_request < a.size()) //loop for collecting the operands
    {
        //cout<<"i am in while for checking end of the message\n";
        
        if (a[index_of_request] != ' ' && a[index_of_request] != '\r' && a[index_of_request] != '\n')
        {
            cout<<"i'm collecting an operand\n";
            operand = operand + a[index_of_request];
            cout<<operand<<endl;
        }
        
        else if(a[index_of_request] == ' ' || a[index_of_request] == '\r' || a[index_of_request] == '\n')
        {
            cout<<"i am in else-if\n";
            double number;
            
            if (operand == "0")
            {
                //cout<<"i am taking the operand 0\n";
                operands.push_back(atof(operand.c_str()));
                cout<<operands[j]<<endl;
                operand.clear();
            }
            
            else
            {
                number = atof(operand.c_str());
                if (number == 0)
                {
                    cout<<"i am taking in-valid operand\n";
                    operands.push_back(10.0103);
                    cout<<operands[j]<<endl;
                    operand.clear();
                }
                else
                {
                    cout<<"i am taking valid operand\n";
                    operands.push_back(number);
                    cout<<operands[j]<<endl;
                    operand.clear();
                }
            }
            j++;
        }
        index_of_request++;
    }
    return operands;
}


char get_operation(string& c)
{
    char operation;
    operation = c[index_of_request];  //statement for collecting the operation
    index_of_request = index_of_request+2;
    cout<<operation<<endl;
    return operation;
}

bool check_message(string& msg)
{
    bool tf;
    if(msg != "AOSR")
        tf = false;
    
    else if(msg == "AOSR")
        tf = true;
    return tf;
}

bool check_operation(char& operation)
{
    if(operation == '+' || operation == '-' || operation == '/' || operation == '*')
    {
        return true;
    }
    else
        return false;
}

bool check_operands(vector<double>& operands)
{
    int operands_check = 0;
    int k;
    for (k = 0; k<operands.size(); k++)
    {
        if (operands[k] == 10.0103)
        {
            operands_check++;
            break;
        }
    }
    if (operands_check == 0)
        return true;
    else
    {
        cout<<"operand number "<<k+1<<" is not an integer\n";
        return false; }
}

string do_operation(char& c, vector<double>& operands)
{
    double result;
    string result_str;
    ostringstream strs;
    result_str.clear();
    char operation;
    operation = c;
    //cout<<operation<<endl;
    switch (operation)
    {
        case '+':
            for (int k = 0; k<operands.size(); k++)
            {
                result = result + operands[k];
            }
            strs << result;
            result_str = strs.str();
            break;
            
        case '-':
            result = operands[0];
            for (int k = 1; k<operands.size(); k++)
            {
                result = result - operands[k];
            }
            strs << result;
            result_str = strs.str();
            break;
            
        case '/':
            result = operands[0];
            for (int k = 1; k<=operands.size(); k++)
            {
                cout<<operands[k]<<endl;
                if (k < operands.size() && operands[k]==0)
                {
                    result_str = "division is not possible";
                    break;
                }
                else
                {
                    if (k == operands.size())
                    {
                        strs << result;
                        result_str = strs.str();
                    }
                    else if(k<operands.size())
                    {
                        result = result / operands[k]; 
                    }
                }
            }
            break;
        case '*':
            result = operands[0];
            for (int k = 1; k<operands.size(); k++)
            {
                result = result * operands[k];
            }
            strs << result;
            result_str = strs.str();
            break;
            
        default:
            cout<<"not a valid operation requested\n";
            result_str = "not a valid operation!";
            break;
    }
    return result_str;
}