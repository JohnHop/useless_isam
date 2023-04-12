#include "index.h"

#include <fstream>

/**
 * Carica l'indice dal file.
 * TODO: lancia secondo la regola RAII
*/
Index::Index(const std::string& filename) {
  std::ifstream file{filename, std::ios_base::binary | std::ios_base::ate};

  //TODO verifica sull'apertura del file

  index_size = file.tellg() / sizeof(index_entry_t);  //determino la dimensione dell'indice in memoria
  index = new index_entry_t[index_size];  //TODO: lancia se non può allocare

  file.seekg(0, std::ios_base::beg);  //mi riposiziono all'inizio del file

  for(int i = 0; !file.eof(); i += 1) {
    file.read( reinterpret_cast<char*>(&index[i]), sizeof(index_entry_t) );
  }
}

/**
 * Restituisce il numero della pagina che potrebbe contenere la chiave key.
 * La ricerca parte dalla fine e viene restituita la posizione della prima pagina che contiene tutte le chiavi >= @key
*/
int Index::search(const unsigned int key) {
  int i{index_size-1};

  while(i >= 0 && key < index[i].first) { //ricerca all'interno del vettore dell'indice
    i -= 1;
  }

  if(i >= 0) {  //trovata!
    return index[i].second;
  }
  else {
    return -1;  //TODO: trova una soluzione più elegante e meno old-school
  }
}

/**
 * Restituisce il numero della pagina che potrebbe contenere l'ultimo elemento del database <= @key.
 * La ricerca parte dall'inizio e prosegue finché non si arriva all'indice > key; quindi si restituisce la posizione dell'indice precedente
*/
int Index::search_reverse(const unsigned int key) {
  int i{0};

  while(i < index_size && key >= index[i].first) {
    i += 1;
  }

  if(i == 0) {
    return -1;  //nessun elemento è <= key
  }
  else {
    return index[i-1].second;
  }
}