#if !defined(INDEX_H)
#define INDEX_H

#include "params.h"

#include <string>

/**
 * Si occupa di caricare e utilizzare l'indice del database.
*/

class Index {
  index_entry_t* index{nullptr};
  int index_size{0};

public:
  Index(const std::string&);
  ~Index() { if(index) delete[] index; };

  int search(const unsigned int);
  int search_reverse(const unsigned int);
};

#endif // INDEX_H
