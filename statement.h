#if !defined(STATEMENT_H)
#define STATEMENT_H

#include <string>
#include <vector>

#include "lexer.h"
#include "record.h"
#include "database.h"

/**
 * Costituisce il parser. 
 * A partire dall'input, costruisce la sua rappresentazione in modo tale da poter eseguire successivamente la query.
 * Per ora abbiamo due tipi di statement:
 *  - SELECT <id>
 *  - SELECT <range>
*/
class Statement {
  enum Type {
    SELECT_SINGLE, SELECT_RANGE
  };

  Type statement_type;
  int start_id;
  int end_id;

  Lexer lexer;

public:
  Statement(const std::string&);

  void expression();
  int id();

  std::vector<Record> execute(Database& database);
};

#endif // STATEMENT_H
