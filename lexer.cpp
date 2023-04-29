#include "lexer.h"

Token Lexer::get() {
  char ch{0};

  input >> ch;  //leggiamo un carattere. Se l'operazine fallisce #ch rimane immutato. Quindi ch == 0 è l'eof

  switch(ch) {
    case 0: return current_token = {Token_type::end};
    case '-': return current_token = {Token_type::RANGE_OPERATOR};
    default:
      if(isalpha(ch)) { //se è un carattere allora è una parola chiave
        input.putback(ch);  //rimettiamo ch nello stream

        std::string keyword;
        input >> keyword; //otteniamo la parola chiave

        if(keyword == "SELECT") {
          return current_token = {Token_type::SELECT};
        } 
        else {
          throw std::logic_error{"Lexer::get(): unrecognized keyword!"};
        }
      }
      else if(isdigit(ch)) {  //se è un numero
        input.putback(ch);

        std::string number;
        input >> number;  //otteniamo il numero comunque sotto la forma di stringa

        return current_token = {Token_type::POSITIVE_INTEGER, number};
      }
      else {  //niente di ciò che è stato previsto
        throw std::logic_error{"Lexer::get(): Unrecognized token!"};
      }
  }
}