#if !defined(DATABASE_H)
#define DATABASE_H

#include "pager.h"
#include "index.h"
#include "record.h"
#include "cursor.h"

#include <string>

class Database {
  Index index;
  Pager pager;

public:
  Database(const std::string& index_filename, const std::string& database_filename):
    index{index_filename}, pager{database_filename} { };

  Cursor search(const unsigned);
  std::pair<Cursor,Cursor> search(const unsigned int, const unsigned int);
};

#endif // DATABASE_H
