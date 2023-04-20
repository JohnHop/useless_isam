#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#include "record.h"
#include "params.h"

/**
 * Preleva e salva una colonna del tipo int dal file
*/
void get(const std::string& row, const std::string delim, size_t& last_pos, size_t& next_pos, int& data) {
  std::string token;

  next_pos = row.find(delim, last_pos);
  token = row.substr(last_pos, next_pos-last_pos);
  data = strtoul(token.c_str(), nullptr, 10);
  last_pos = next_pos + 1;
}

/**
 * Preleva e salva una colonna del tipo stringa caratteri a dimensione variabile
*/
void get(const std::string& row, const std::string delim, size_t& last_pos, size_t& next_pos, char* data) {
  std::string token;

  next_pos = row.find(delim, last_pos);
  token = row.substr(last_pos, next_pos-last_pos);
  strcpy(data, token.c_str());
  last_pos = next_pos + 1;
}

/**
 * Preleva e salva una colonna del tipo stringa caratteri a dimensione fissa
*/
void get(const std::string& row, const std::string delim, size_t& last_pos, size_t& next_pos, char* data, const size_t nbyte) {
  std::string token;

  next_pos = row.find(delim, last_pos);
  token = row.substr(last_pos, next_pos-last_pos);
  strncpy(data, token.c_str(), nbyte);
  last_pos = next_pos + 1;
}

int main(int argc, char const *argv[])
{
  std::ifstream input_file{DATASET_FILENAME};
  std::ofstream output_file{DATABASE_FILENAME, std::ios_base::out | std::ios_base::binary};

  if(!input_file.is_open() || !output_file.is_open()) {
    std::clog << "Error opening " << DATASET_FILENAME << " or " DATABASE_FILENAME << "\n";
    return EXIT_FAILURE;
  }

  //Exception setup
  input_file.exceptions(std::ifstream::goodbit);  //Evitiamo l'uso delle eccezioni nel file di input
  output_file.exceptions(std::ofstream::failbit); //Assicuriamoci che lanci in caso di errore di scrittura

  std::string row; //singola riga di caratteri del file .csv
  Record record;
  size_t last_pos{0}; //posizione del precedente delimitatore
  size_t next_pos;  //posizione del successivo limitatore
  const std::string delim{";"}; //* Carattere delimitatore
  std::string token;  //singola elemento (colonna) di una riga

  const unsigned int records_per_page{ PAGE_SIZE / sizeof(Record) };  //23
  const unsigned int overhead_byte{ PAGE_SIZE % sizeof(Record) }; //48 byte
  const char overhead_data[overhead_byte]{0}; //Per riempire la parte restante della pagina

  size_t count{0};  //Record counter

  while(!input_file.eof()) {
    std::getline(input_file, row, '\n'); //estraggo una singola riga (record)

    if(input_file.fail()) {
      std::clog << "Error occurred while reading from " << DATASET_FILENAME << "\n";
      return EXIT_FAILURE;
    }
    
    //1. ottengo la prima colonna: id (unsigned int)
    get(row, delim, last_pos, next_pos, record.id);

    //2. ottengo la seconda colonna: name (char[64]) a dimensione variabile
    get(row, delim, last_pos, next_pos, record.name);

    //3. ottengo la terza colonna: country_code (char[2]) a dimensione fissa
    get(row, delim, last_pos, next_pos, record.country_code, 2);

    //4. ottengo la quarta colonna: country_name (char[64]) a dimensione variabile
    get(row, delim, last_pos, next_pos, record.country_name);

    //5. ottengo la quinta colonna: population (unsigned int)
    get(row, delim, last_pos, next_pos, record.population);

    //6. ottengo la sesta colonna: DIgital Elevation Model (unsigned int)
    get(row, delim, last_pos, next_pos, record.dem);

    //7. ottengo la settima colonna: timezone (char[64]) a dimensione variabile
    get(row, delim, last_pos, next_pos, record.timezone);

    //8. ottengo l'ottava colonna: latitude (char[9]) a lunghezza variabile
    get(row, delim, last_pos, next_pos, record.latitude);

    //9. ottengo la nona colonna: longitude (char[9]) a lunghezza variabile
    get(row, delim, last_pos, next_pos, record.longitude);

    //Writing data to output_file
    try {
      output_file.write(reinterpret_cast<char*>(&record), sizeof(Record));  //TODO: intercetta eventuali errori
      count += 1;

      if( (count % records_per_page) == 0) {  //se abbiamo scritto un multiplo di records_per_page (23)
        output_file.write(overhead_data, overhead_byte); //aggiungo l'overhead in modo da costituire una pagina intera  //TODO: intercetta eventuali errori
      }
    } catch(std::ofstream::failure& e) {
      std::clog << "Error occurred while writing on " << DATABASE_FILENAME << ": " << e.what() << "\n";
      return EXIT_FAILURE;
    }
  } //end of while loop

  std::cout << "Written " << count << " records of " << sizeof(Record) << " byte each.\n\
Total size: " << output_file.tellp() << " bytes\n";

  return EXIT_SUCCESS;
}
