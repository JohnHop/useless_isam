#include <cstddef>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "record.h"
#include "params.h"

/**
 * Carica tutti i record del file in un vettore in modo che possa essere successivamente ordinato.
 * Viene restituito un rvalue del database caricato dal file. 
*/
std::vector<Record> load_data(const std::string& filename) {
  std::ifstream file{DATABASE_FILENAME, std::ios_base::in | std::ios_base::binary | std::ios_base::ate};

  if(!file) {
    std::clog << "Error opening file " << filename << "\n";
    exit(EXIT_FAILURE);
  }

  const std::streamoff db_size{file.tellg()}; //dimensione in byte del file

  if(db_size == -1) {
    std::clog << "Error getting file size of " << filename << "\n";
    exit(EXIT_FAILURE);
  }

  const unsigned int sizeof_record{sizeof(Record)};
  const unsigned int records_per_page{ PAGE_SIZE / sizeof_record };  //numero di record che entrano in una pagina
  const unsigned int page_count( db_size / PAGE_SIZE );  //numero di pagine presenti nel file (narrowing)
  const unsigned remaining_bytes( db_size % PAGE_SIZE );  //dimensione della pagina parziale

  unsigned int record_count{ (page_count * records_per_page) + (remaining_bytes / sizeof_record) }; //numero di record di cui è costituito il file

  file.seekg(0);  //Mi porto all'inizio del file

  std::vector<Record> database(record_count);
  const long long int overhead_bytes = PAGE_SIZE % sizeof_record; //calcolo i byte da saltare ogni 23 record

  file.exceptions(std::ofstream::failbit);  //Ci assicuriamo che siano attive le eccezioni

  try {
    for(size_t i = 0; i < record_count; i += 1) { //carico tutti i record nel vettore
      file.read(reinterpret_cast<char*>(&database[i]), sizeof_record);

      if( ( (i+1) % records_per_page) == 0 ) {
        file.seekg(overhead_bytes, std::ios_base::cur); //salto i byte aggiungi all'overhead
      }
    }
  } catch(std::ifstream::failure& e) {
    std::clog << "Error loading records from " << filename << ": " << e.what() << "\n";
    exit(EXIT_FAILURE);
  }

  return database;
}

void write_data(const std::vector<Record>& database, const std::string& filename) {
  std::ofstream file{filename, std::ios_base::out | std::ios_base::binary};

  if(!file) {
    std::cerr << "Error opening file " << filename << "\n";
    exit(EXIT_FAILURE);
  }

  file.exceptions(std::ofstream::failbit);  //Ci assicuriamo che siano attive le eccezioni

  const size_t records_per_page{PAGE_SIZE / sizeof(Record)};  //Numero di record presenti in una pagina
  const size_t overhead{ PAGE_SIZE % sizeof(Record) };  //numero di byte necessari per completare una pagina
  const char padding[overhead]{0};

  size_t count{0};  //numero di record scritti nel file
  for(const auto it: database) {
    try {
      file.write(reinterpret_cast<const char*>(&it), sizeof(Record));
      count += 1;

      if( (count % records_per_page) == 0) {  //ogni 23 pagine
        file.write(padding, overhead);
      }
    } catch(std::ostream::failure& e) {
      std::clog << "Error wrting " << filename << ": " << e.what() << "\n";
      exit(EXIT_FAILURE);
    }
  }
}

void generate_index(const std::vector<Record>& database, const std::string& filename) {
  const unsigned records_per_page{ PAGE_SIZE / sizeof(Record) };  //23
  std::size_t num_of_pages{ database.size() / records_per_page };

  if(database.size() % records_per_page) {  //se è presente una pagina parziale
    num_of_pages += 1;
  }

  std::vector<index_entry_t> index(num_of_pages);

  //Per ogni pagina prendiamo il primo record e lo salviamo nel vettore indice
  for(int page = 0; page < num_of_pages; page += 1) {
    index[page].first = database[page*records_per_page].id;
    index[page].second = page;
  }

  //Scriviamo l'index in un file
  std::ofstream file{filename, std::ios_base::out | std::ios_base::binary};

  if(!file) {
    std::cerr << "Error opening file " << filename << "\n";
    exit(EXIT_FAILURE);
  }

  for(const auto it: index) {
    try {
      file.write(reinterpret_cast<const char*>(&it), sizeof(index_entry_t));
    } catch(std::ofstream::failure& e) {
      std::clog << "Error writing " << filename << ": " << e.what() << "\n";
      exit(EXIT_FAILURE);
    }
  }

  std::cout << "Indice scritto: " << num_of_pages << " elementi\n";
}

int main(int argc, char* argv[])
{
  std::vector<Record> database{load_data(DATABASE_FILENAME)};  //viene invocato il costruttore di spostamento

  std::sort(database.begin(), database.end(), //ordino sulla base del campo 'id'
    [](const Record& a, const Record& b) {
      return a.id < b.id;
    }
  );

  write_data(database, DATABASE_FILENAME);  //salviamo il dabase ordinato per la chiave scelta (organizzato in pagine)
  std::cout << "Sorting executed on " << DATABASE_FILENAME << "\n";

  generate_index(database, INDEX_FILENAME); //Realizziamo l'indice e lo scriviamo in un file

  return EXIT_SUCCESS;
}