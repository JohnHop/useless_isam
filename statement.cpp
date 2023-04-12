#include "statement.h"

#include <stdexcept>
#include <iostream> //FIXME

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

  expression(); //* Attenzione: si mangia un token in più. Il token successivo è gia in pancia al Lexer!

  if(lexer.current().type != Token_type::end) {
    throw std::logic_error{"Statement::construct(): end of input expected!"};
  }
}

/**
 * *Attenzione: si mangia un token in più!
 * TODO: si dovrebbe restituire, in caso di SELECT_SINGLE, il token che ha mangiato in più...
 */
void Statement::expression() {
  start_id = id();

  if(lexer.get().type == Token_type::RANGE_OPERATOR) {
    statement_type = Type::SELECT_RANGE;
    end_id = id();
    lexer.get();  //! devo comunque mangiare un token in più
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
 * Se non viene trovato alcun risultato, viene restituito un std::vector vuoto.
*/
std::vector<Record> Statement::execute(Database& database) {
  if(statement_type == Type::SELECT_SINGLE) {
    Cursor res = database.search(start_id);

    return (res) ? std::vector{*res.record} : std::vector<Record>(0);
  }
  else { //Type::SELECT_RANGE
    auto range = database.search(start_id, end_id);

    //FIXME: controllo not found di #range

    std::vector<Record> results(range.first.diff(range.second) + 1);

    int i{0}; //TODO: puoi farlo meglio
    while(range.first != range.second) {
      results[i++] = *range.first.record;
      ++range.first;
    }
    results[i] = *range.first.record; //quindi inseriamo anche l'ultimo record

    return results;
  }


}