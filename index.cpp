#include "index.h"

#include <fstream>
#include <iostream>

/**
 * Carica l'indice dal file.
*/
Index::Index(const std::string& filename)
: index{nullptr}, index_size{0}
{
  std::ifstream file{filename, std::ios_base::binary | std::ios_base::ate};

  if(!file) { //? forse si dovrebbero usare le eccezioni?
    std::clog << "Error opening file " << filename << "\n";
    exit(EXIT_FAILURE);
  }

  index_size = file.tellg() / sizeof(index_entry_t);  //determino la dimensione dell'indice in memoria
  if(index_size == -1) {
    std::clog << "Error getting file size of " << filename << "\n";
    exit(EXIT_FAILURE);
  }

  file.seekg(0, std::ios_base::beg);  //mi riposiziono all'inizio del file
  if(!file) {
    std::clog << "Error setting file pointer position of " << filename << "\n";
    exit(EXIT_FAILURE);
  }

  file.exceptions(std::ofstream::failbit);  //Ci assicuriamo che siano attive le eccezioni

  try {
    index = new index_entry_t[index_size];

    for(int i = 0; i < index_size; i += 1) {
      file.read( reinterpret_cast<char*>(&index[i]), sizeof(index_entry_t) );
    }
  } catch(std::bad_alloc& e) {
    std::clog << "Failed to allocate memory in Index(): " << e.what() << "\n";
    exit(EXIT_FAILURE);
  } 
  catch(std::ifstream::failure& e) {
    std::clog << "Error loading index records from " << filename << ": " << e.what() << "\n";
    exit(EXIT_FAILURE);
  }
}

/**
 * Restituisce il numero della pagina che potrebbe contenere la chiave key.
 * La ricerca parte dalla fine e viene restituita la posizione della prima pagina che contiene tutte le chiavi >= @key
*/
int Index::search(const unsigned int key) {
  int i{index_size-1};  //La ricerca inizia dalla fine

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
  int i{0}; //Ricerca dall'inizio

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