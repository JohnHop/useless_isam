#if !defined(PAGER_H)
#define PAGER_H

#include "params.h"
#include "record.h"
#include "page.h"

#include <string>

/**
 * Esegue gli accessi al file organizzato in pagine.
 * Serve solo per prelevare pagine dal file.
*/
class Pager {
  std::string filename;

  Page** pages; //vettore di puntatori a pagine
  size_t pages_size;  //lunghezza del vettore di pagine e numero di pagine presenti nel file  //TODO: valuta se renderlo public e const

  long long int file_size;
  bool last_page_is_partial;

public:
  Pager(const std::string&);
  ~Pager();

  const Page* get_page(const int); //restituisce una pagina //? è meglio usare un unsigned int?
  size_t get_num_of_pages() const { return pages_size; };

  const int records_per_page{PAGE_SIZE/sizeof(Record)};
};

#endif // PAGER_H
