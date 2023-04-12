#include "pager.h"

#include <fstream>

Pager::Pager(const std::string& f): filename{f}, pages{nullptr}, pages_size{0}, last_page_is_partial{false}, file_size{0}
{
  std::ifstream file{filename, std::ios_base::in | std::ios_base::binary | std::ios_base::ate};

  //TODO: controllo corretta apertura file

  file_size = file.tellg(); //dimensione del file in byte
  pages_size = file_size / PAGE_SIZE;  //numero di pagine presenti nel file
  if(file.tellg() % PAGE_SIZE) {  //ma se è presente una pagina parziale
    last_page_is_partial = true;
    pages_size += 1;
  }

  pages = new Page*[pages_size]{nullptr};  //TODO: gestisci quando lancia
}

/**
 * Preleva una pagina dal file (se necessario), la mette nella cache e poi restituisce il suo puntatore.
 * Alternativamente restituisce un puntatore nullo se viene richiesta una pagina non esistente
*/
const Page* Pager::get_page(const int num_page) {
  //Controllo validità dell'argomento
  if(num_page >= this->pages_size) {
    return nullptr;
  }

  //Restituisce subito il puntatore alla pagina se è già stata caricata precedentemente
  if(pages[num_page]) {
    return pages[num_page];
  }
  
  //Altrimenti preleva la pagina dal file
  const int records_to_fetch{ (num_page == pages_size-1 && last_page_is_partial) ? (file_size % PAGE_SIZE) : (this->records_per_page)}; //Determino quanti record prelevare
  
  std::ifstream file{filename, std::ios_base::in | std::ios_base::binary};

  //TODO: controllo apertura file

  file.seekg(PAGE_SIZE*num_page); //Mi posiziono all'inizio della pagina da prelevare

  pages[num_page] = new Page{num_page, records_to_fetch}; //TODO: gestisci quando lancia
  
  file.read( reinterpret_cast<char*>(pages[num_page]->records), sizeof(Record)*records_to_fetch);

  return pages[num_page];
}

Pager::~Pager() {
  if(pages) {
    for(int i = 0; i < pages_size; i += 1) {
      if(pages[i]) {
        delete[] pages[i]; 
      }
    }
    delete[] pages;
  }
}