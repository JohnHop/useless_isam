#include "database.h"

/**
 * Restituisce con esattezza il record con @key == id
*/
Cursor Database::search(const unsigned key) {
  unsigned int page_num = this->index.search(key);

  if(page_num == -1) {  //se Index non ha trovato la pagina
    return Cursor{};
  }

  const Page* page = this->pager.get_page(page_num);

  //Ora cerchiamo all'interno della pagina
  unsigned int i{0};
  while(i < pager.records_per_page && page->records[i].id != key) {
    i += 1;
  }

  if(i == pager.records_per_page) { //non trovato
    return Cursor{};
  }
  else {
    return Cursor{&pager, page, &(page->records[i]), i, page_num};
  }
}

/**
 * Restituisce una coppia di cursori: il primo elemento del range e l'ultimo. 
 * TODO: migliora questa funzione
*/
std::pair<Cursor,Cursor> Database::search(const unsigned int start_key, const unsigned int end_key) {
  const auto not_found = std::pair<Cursor,Cursor>{ Cursor{}, Cursor{} };  //shortcut per il valore "non trovato"

  //Controllo correttezza dei parametri
  if(start_key >= end_key) {
    return not_found;
  }

  unsigned int start_page_pos = this->index.search(start_key);  //Restituisce la pagina che dovrebbe contenere start_key

  if(start_page_pos == -1) {  //se Index non ha trovato la pagina relativa alla chiave
    return not_found;
  }

  const Page* start_page_ptr = this->pager.get_page(start_page_pos);

  unsigned int start_i = start_page_ptr->search_greater_equal(start_key); //otteniamo il primo id >= di @start_key nella pagina

  if(start_i == -1) { //Non trovato. Allora dobbiamo cercare nella pagina successiva
    start_page_pos += 1;
    start_page_ptr = this->pager.get_page(start_page_pos);
    start_i = start_page_ptr->search_greater_equal(start_key);
  }
  
  //Cerchiamo l'ultimo elemento più piccolo di @end_key
  unsigned int end_page_pos = this->index.search_reverse(end_key);

  if(end_page_pos == -1) {
    return not_found;
  }

  const Page* end_page_ptr = this->pager.get_page(end_page_pos);

  //Ora cerchiamo l'ultima chiave <= @end_key
  unsigned int end_i = end_page_ptr->search_less_equal(end_key);

  if(end_i == -1) {
    return not_found;
  }

  return std::pair{ 
    Cursor{&pager, start_page_ptr, &(start_page_ptr->records[start_i]), start_page_pos, start_i}, 
    Cursor{&pager, end_page_ptr, &(end_page_ptr->records[end_i]), end_page_pos, end_i}
  };
}