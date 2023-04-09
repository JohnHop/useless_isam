#if !defined(LEXER_H)
#define LEXER_H

#include <sstream>

#include "token.h"

/**
 * Il Lexer non è altro che un Token Stream, detto anche Scanner.
 * Data una sorgente di input, la oscura in modo da poter restituire a richiesta token proveniente da tale sorgente.
*/

class Lexer {
  std::stringstream input;
  Token current_token;

public:
  Lexer(const std::string& in): input(in), current_token{Token_type::end} { };

  Token get();  //preleva e restituisce il token successivo dall'input
  const Token& current() const { return current_token; };
};

#endif // LEXER_H
