#if !defined(PAGE_H)
#define PAGE_H

#include "params.h"
#include "record.h"

/**
 * Di norma una pagina contiene un numero fisso di Record, eccetto per quanto riguarda l'ultima pagina.
*/
struct Page {
  Record* records;  //Vettore di Record presenti nella pagina
  const int size; //Numero di record presenti nella pagina
  const int page_pos; //Posizione della pagina all'interno del file

  Page(const int pp, const int s): records{nullptr}, page_pos{pp}, size{s} { records = new Record[size]; };
  ~Page() { if(records) delete[] records; };

  int search_greater_equal(const unsigned int) const; //TODO: puoi generalizzare la ricerca utilizzando template di funzioni
  int search_less_equal(const unsigned int) const;
};

/**
 * Cerca il primo record di cui id >= @key per ciascun record della pagina.
 * Restituisce la posizione del record trovato al'interno dell'array. Altrimenti -1
*/
inline int Page::search_greater_equal(const unsigned int key) const {
  int i{0};

  while(i < this->size && records[i].id < key) {
    i += 1;
  }

  if(i == this->size) {
    return -1;
  }
  else {
    return i;
  }
}

/**
 * Cerca il primo record di cui key <= id e resituisce la sua posizione all'interno della pagina.
 * I record all'interno della pagina sono ordinati per id. Quindi la ricerca parte dall'ultimo record
 * Restituisce -1 se tutti i record della pagina hanno id >
*/
inline int Page::search_less_equal(const unsigned int key) const {
  int i{this->size-1};

  while(i >= 0 && key < records[i].id) {
    i -= 1;
  }

  return i; //Restituisce -1 se non trovato, altrimenti la posizione del record
}

#endif // PAGE_H
