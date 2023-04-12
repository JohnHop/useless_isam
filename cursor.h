#if !defined(CURSOR_H)
#define CURSOR_H

#include "pager.h"

class Cursor {
  Pager* pager; //TODO: rendilo un puntatore costante a dati variabili (oppure un riferimento)
  const Page* page;

public:
  const Record* record;
  unsigned int page_pos;  //pagina dalla quale è riferito il Record
  unsigned int array_pos; //posizione del Record all'interno del vettore 'records' della pagina

  Cursor(Pager* pg = nullptr, const Page* p = nullptr, const Record* r = nullptr, unsigned int pp = 0, unsigned int ap = 0)
    : pager{pg}, page{p}, record{r}, array_pos{ap}, page_pos{pp} { };

  operator bool() const { return (record) ? true : false; }; //TODO: valuta se deve essere explicit
  Cursor& operator++();
  bool operator==(const Cursor& o) const { return (this->page_pos == o.page_pos && this->array_pos == o.array_pos) ? true : false; };
  bool operator!=(const Cursor& o) const { return !this->operator==(o); }

  long int diff(const Cursor&);
};

inline Cursor& Cursor::operator++() {
  if(++array_pos == page->size) { //se era l'ultimo record della pagina
    page_pos += 1;
    array_pos = 0;
  }

  if(page_pos == pager->get_num_of_pages()) { //se era l'ultima pagina
    this->record = nullptr; //invalido il cursore
    return *this;
  }

  this->page = pager->get_page(page_pos);
  this->record = &(page->records[array_pos]);

  return *this;
}

/** Restituisce la distanza tra due cursori in termini di numero di record.
 * Se il cursore attuale è dopo @other allora restituisce semplicemente -1.
 * FIXME: controllo dell'ultima pagina parziale
 */
inline long int Cursor::diff(const Cursor& other) {
  if(this->page_pos > other.page_pos || this->page_pos == other.page_pos && this->array_pos > other.array_pos) {  //@other viene prima del cursore attuale
    return -1;
  }

  long int page_diff = other.page_pos - this->page_pos; //differenza in termini di pagine
  long int record_diff = other.array_pos - this->array_pos;  //differenza in termini di record

  return (page_diff*pager->records_per_page)+record_diff;
}

#endif // CURSOR_H
