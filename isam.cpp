#include "record.h"
#include "params.h"
#include "statement.h"
#include "database.h"

#include <string>
#include <iostream>

int main(int argc, char const *argv[])
{
  std::string input;  //sql statement proveniente dallo standard input
  bool exit{false};

  Database database{INDEX_FILENAME, DATABASE_FILENAME};

  //REPL
  while(!exit) {
    std::cout << "isam > ";
    std::getline(std::cin, input, '\n');

    if(input == ".exit" || std::cin.eof()) {
      exit = true;
    }
    else {
      Statement statement{input};
      std::vector<Record> res = statement.execute(database);

      //Visualizziamo i risultato
      if(res.size()) {
        for(auto& e: res) {
          std::cout << e;
        }
      }
      else {
        std::cout << "Found 0 records\n";
      }
    }
  }

  return EXIT_SUCCESS;
}
