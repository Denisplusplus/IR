#include <iostream>
#include <stack>
#include <sstream>
#include <vector>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <regex>
#include <unicode/unistr.h>
#include <unicode/ustream.h>
#include <unicode/locid.h>
#include <algorithm>
#include <cctype>
#include <string>

#include "query.h"


bool is_opening_bracket(std::string value) {
  return value == "(";
}

bool is_closing_bracket(std::string value) {
  return value == ")";
}


Priority operation_priority(std::string operation) {
  if (operation == "&&") {
    return AND;
  } else if (operation == "||") {
    return OR;
  }
  return ERR;
}


bool is_operation(std::string input) {
    if (input == "&&" || input == "||" ) {
        return true;
    }
    return false;
}

bool is_value(std::string value) {
    if (!is_opening_bracket(value) && (!is_closing_bracket(value)) && (!is_operation(value)))
        return true;
    return false;
}



std::stack<Token> ToPostfix(std::string input) {
    std::vector<std::string> input_values;

    std::istringstream is (input);
    input_values.insert(input_values.end(), 
        std::istream_iterator<std::string>(is),  
        std::istream_iterator<std::string>());
    
    Token token = {
        "",
        "",
        BINARY
    };

    std::stack<Token> result_stack;
    std::stack<Token> operations_stack;

    int i = 0;
    while(i < input_values.size()) {

        if (is_value(input_values[i])) {

            token.operation = "";
            token.value = input_values[i];
            
            result_stack.push(token);
        } else if (is_operation(input_values[i])) {
            while (!operations_stack.empty() && operation_priority(input_values[i]) <= operation_priority(operations_stack.top().operation)) {
                token = operations_stack.top();
                operations_stack.pop();
                result_stack.push(token);
            }
            token.operation = input_values[i];
            token.value = "";
            operations_stack.push(token);
        } else if (is_opening_bracket(input_values[i])) {
            token.operation = input_values[i];
            token.value = "";
            operations_stack.push(token);
        } else if (is_closing_bracket(input_values[i])) {
            while (!operations_stack.empty() && operations_stack.top().operation != "(") {
                token = operations_stack.top();
                operations_stack.pop();
                result_stack.push(token);
            }

            if (!operations_stack.empty()) {
                operations_stack.pop();
            }
        }

        i++;
    }

    while (!operations_stack.empty()) {
        token = operations_stack.top();
        operations_stack.pop();
        result_stack.push(token);
    }

    std::stack<Token> s;
    while (!result_stack.empty())
    {
        s.push(result_stack.top());
        result_stack.pop();
    }
    return s;
}

bool is_word(std::string s) {
    if (s == "(" || s == ")" || s == "&&" || s == "||" || s == "" ){
        return false;
    }
    return true;
}

std::string PreprocessQuery(std::string input) {

    // highlight
    boost::replace_all(input, "(", " ( ");
    boost::replace_all(input, ")", " ) ");
    boost::replace_all(input, "&&", " && ");
    boost::replace_all(input, "||", " || ");

    // handle && 
    std::istringstream iss(input);
    std::string word, last;
    std::string new_string;
    last = "";
    while(iss >> word) {
        new_string += " ";
        if (is_word(word) && is_word(last)) {
            new_string += " && ";  
        }

        new_string += word;
        last = word;
    }
    input = new_string;

    // remove extra spaces
    std::regex re("\\s{2,}");
    input = std::regex_replace(input, re, " ");
    boost::algorithm::trim(input);

    std::cout << "Your query: " << input << std::endl;

    return input;
}
