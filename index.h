#if !defined(INDEX_H)
#define INDEX_H

#include "params.h"

#include <string>

/**
 * Si occupa di caricare e utilizzare l'indice del database.
*/

class Index {
  index_entry_t* index;
  int index_size;

public:
  Index(const std::string&);
  ~Index() { if(index) delete[] index; };

  int search(const unsigned int);
  int search_reverse(const unsigned int);

  //Operazioni soppresse
  Index(const Index&) = delete; //Costruttore copia
  Index(const Index&&) = delete;  //Costruttore di spostamento
  Index& operator=(const Index&) = delete; //Operatore di copia
  Index& operator=(Index&&) = delete; //Operatore di spostamento

};

#endif // INDEX_H
