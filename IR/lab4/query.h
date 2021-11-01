
#ifndef PARSER_H
#define PARSER_H




typedef enum Operation_category {
  UNARY,
  BINARY,
  BRACKET
} Operation_category;

typedef struct Token {
  std::string operation;
  std::string value;
  Operation_category category;
} Token;

typedef enum Priority {
  ERR,
  AND,
  OR
} Priority;


bool is_opening_bracket(std::string value);
std::stack<Token> ToPostfix(std::string input);
std::string PreprocessQuery(std::string input);
bool is_closing_bracket(std::string value);
Priority operation_priority(std::string operation);
bool is_operation(std::string input);
bool is_value(std::string value);




#endif