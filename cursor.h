#if !defined(CURSOR_H)
#define CURSOR_H

#include "pager.h"

class Cursor {
  Pager* pager; //TODO: rendilo un puntatore costante a dati variabili (oppure un riferimento)
  const Page* page; //puntatore alla pagina alla quale si riferisce il cursore

public:
  const Record* record;
  int page_pos;  //pagina dalla quale è riferito il Record
  int array_pos; //posizione del Record all'interno del vettore 'records' della pagina

  Cursor(Pager* pg = nullptr, const Page* p = nullptr, const Record* r = nullptr, int pp = 0, int ap = 0)
    : pager{pg}, page{p}, record{r}, array_pos{ap}, page_pos{pp} { };

  operator bool() const { return (record) ? true : false; } //TODO: valuta se deve essere explicit
  Cursor& operator++();
  bool operator==(const Cursor& other) const { return (this->page_pos == other.page_pos && this->array_pos == other.array_pos) ? true : false; }
  bool operator!=(const Cursor& other) const { return !this->operator==(other); }

  int diff(const Cursor&);
};

inline Cursor& Cursor::operator++() {
  if(++array_pos == page->size) { //se era l'ultimo record della pagina
    page_pos += 1;
    array_pos = 0;
  }

  if(page_pos == page->size) { //se era l'ultima pagina
    this->record = nullptr; //invalido il cursore
    return *this;
  }

  this->page = pager->get_page(page_pos);
  this->record = &(page->records[array_pos]);

  return *this;
}

/** Restituisce la distanza tra due cursori in termini di numero di record.
 * Se il cursore attuale è dopo @other allora restituisce semplicemente -1.
 */
inline int Cursor::diff(const Cursor& other) {
  if(this->page_pos > other.page_pos || (this->page_pos == other.page_pos && this->array_pos > other.array_pos) ) {  //@other viene prima del cursore attuale
    return -1;
  }

  int page_diff = other.page_pos - this->page_pos; //differenza in termini di pagine
  int record_diff = other.array_pos - this->array_pos;  //differenza in termini di record

  return (page_diff*pager->records_per_page)+record_diff;
}

#endif // CURSOR_H
