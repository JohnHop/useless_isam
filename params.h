#if !defined(PARAMS_H)
#define PARAMS_H

#include <utility>

#define PAGE_SIZE 4096  //TODO dipendenza da costante simbolica

#define DATABASE_FILENAME "../database.bin"  //TODO dipendenza da costante simbolica
#define INDEX_FILENAME "../index.bin" //TODO dipendenza da costante simbolica

using index_entry_t = std::pair<unsigned int, int>; //first: id, second: numero di pagina

#endif // PARAMS_H
