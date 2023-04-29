#if !defined(TOKEN_H)
#define TOKEN_H

#include <string>

enum class Token_type {
  SELECT, RANGE_OPERATOR, POSITIVE_INTEGER,
  end  //end of input
};

struct Token {
  Token_type type;
  std::string value;
};

#endif // TOKEN_H
