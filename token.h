#if !defined(TOKEN_H)
#define TOKEN_H

#include <string>

enum class Token_type {
  SELECT, RANGE_OPERATOR, POSITIVE_NUMBER,
  end  //fine dell'input
};

struct Token {
  Token_type type;
  std::string value;
};

#endif // TOKEN_H
