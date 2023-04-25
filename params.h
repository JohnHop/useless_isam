#if !defined(PARAMS_H)
#define PARAMS_H

#include <utility>

#define PAGE_SIZE 4096  //TODO dipendenza da costante simbolica

#define DATASET_FILENAME "../cities.csv"  //TODO il file deve essere passato come argomento
#define DATABASE_FILENAME "../database.bin"  //TODO dipendenza da costante simbolica
#define INDEX_FILENAME "../index.bin" //TODO dipendenza da costante simbolica

using index_entry_t = std::pair<int, int>; //first: id, second: numero di pagina

#endif // PARAMS_H
