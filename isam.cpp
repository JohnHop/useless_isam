#include "record.h"
#include "params.h"
#include "statement.h"

#include <string>
#include <iostream>

int main(int argc, char const *argv[])
{
  std::string input;
  bool exit{false};

  while(!exit) {
    std::cout << "isam > ";
    std::getline(std::cin, input, '\n');

    if(input == ".exit") {
      exit = true;
    }
    else {
      Statement statement(input);
      // statement.execute();
    }
  }

  return EXIT_SUCCESS;
}
