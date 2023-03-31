#include "statement.h"

#include <stdexcept>
#include <fstream>

/**
 * Esegue direttamente il parsing della stringa di input.
 * TODO: la generazione delle eccezioni deve avvenire secondo la regola RAII...
*/
Statement::Statement(const std::string& input)
: lexer{input}, index{nullptr}, index_size{0}
{
  if(lexer.get().type != Token_type::SELECT) {
    throw std::logic_error("Statement::construct(): SELECT expected!");
  }

  expression(); //Attenzione: si mangia un token in più

  if(lexer.current().type != Token_type::end) {
    throw std::logic_error{"Statement::construct(): end of input expected!"};
  }
}

void Statement::expression() {
  start_id = id();

  if(lexer.get().type == Token_type::RANGE_OPERATOR) {
    statement_type = Type::SELECT_RANGE;
    end_id = id();
  }
  else {
    statement_type = Type::SELECT_SINGLE;
  }
}

/**
 * Si aspetta semplicemente un intero positivo.
*/
int Statement::id() {
  if(lexer.get().type == Token_type::POSITIVE_NUMBER) {
    return std::stoi(lexer.current().value);
  }
  else {
    throw std::logic_error{"Statement::id(): expected POSITIVE_NUMBER!"};
  }
}

/**
 * Esegue la query in base a come il parser ha modificato la rappresentazione di Statement.
*/
void Statement::execute() {
  
}