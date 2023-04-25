#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#include "record.h"
#include "params.h"

void extract(int& to, const std::string& from) {
  to = strtoul(from.c_str(), nullptr, 10);
}

void extract(char* to, const std::string& from) {
  strcpy(to, from.c_str());
}

/**
 * Preleva e salva una colonna del tipo dal file
*/
template<typename T>
void get(const std::string& row, const std::string delim, size_t& last_pos, size_t& next_pos, T& column) {
  std::string attr;

  next_pos = row.find(delim, last_pos);
  attr = row.substr(last_pos, next_pos-last_pos);
  extract(column, attr);
  last_pos = next_pos + 1;
}

int main(int argc, char const *argv[])
{
  std::ifstream input_file{DATASET_FILENAME};
  std::ofstream output_file{DATABASE_FILENAME, std::ios_base::out | std::ios_base::binary};

  if(!input_file || !output_file) {
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

  const unsigned records_per_page{ PAGE_SIZE / sizeof(Record) };  //23
  const unsigned overhead_byte_length{ PAGE_SIZE % sizeof(Record) }; //48 byte
  const char overhead_data[overhead_byte_length]{0}; //Per riempire la parte restante della pagina

  size_t count{0};  //Record reads counter

  while(!input_file.eof()) {
    std::getline(input_file, row, '\n'); //extracts a row

    if(input_file.fail()) {
      std::clog << "Error occurred while reading from " << DATASET_FILENAME << "\n";
      return EXIT_FAILURE;
    }
    
    //1. ottengo la prima colonna: id (int)
    get(row, delim, last_pos, next_pos, record.id);

    //2. ottengo la seconda colonna: name (char[64]) a dimensione variabile
    get(row, delim, last_pos, next_pos, record.name);

    //3. ottengo la terza colonna: country_code (char[3]) a dimensione fissa
    get(row, delim, last_pos, next_pos, record.country_code);

    //4. ottengo la quarta colonna: country_name (char[64]) a dimensione variabile
    get(row, delim, last_pos, next_pos, record.country_name);

    //5. ottengo la quinta colonna: population (int)
    get(row, delim, last_pos, next_pos, record.population);

    //6. ottengo la sesta colonna: DIgital Elevation Model (int)
    get(row, delim, last_pos, next_pos, record.dem);

    //7. ottengo la settima colonna: timezone (char[64]) a dimensione variabile
    get(row, delim, last_pos, next_pos, record.timezone);

    //8. ottengo l'ottava colonna: latitude (char[9]) a lunghezza variabile
    get(row, delim, last_pos, next_pos, record.latitude);

    //9. ottengo la nona colonna: longitude (char[9]) a lunghezza variabile
    get(row, delim, last_pos, next_pos, record.longitude);

    //Writing data to output_file
    try {
      output_file.write(reinterpret_cast<char*>(&record), sizeof(Record));
      count += 1;

      if( (count % records_per_page) == 0) {  //se abbiamo scritto un multiplo di records_per_page (23)
        output_file.write(overhead_data, overhead_byte_length); //aggiungo l'overhead in modo da costituire una pagina intera
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