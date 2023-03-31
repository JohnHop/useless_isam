#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

#include "record.h"
#include "params.h"

#define INPUT_FILENAME "../cities.csv"  //TODO il file deve essere passato come argomento
#define OUTPUT_FILENAME "../database.bin" //TODO il file deve essere passato come argomento

int main(int argc, char const *argv[])
{
  std::ifstream input_file{INPUT_FILENAME};
  std::ofstream output_file{OUTPUT_FILENAME, std::ios_base::out | std::ios_base::binary};

  if(!input_file.is_open() || !output_file.is_open()) {
    std::cerr << "Errore nell'apertura di un file\n";
    return EXIT_FAILURE;
  }

  std::string row; //singola riga di caratteri del file .csv
  Record record;
  size_t last_pos{0}; //posizione del precedente delimitatore
  size_t next_pos;  //posizione del successivo limitatore
  const std::string delim{";"};
  std::string token;  //singola colonna di una linea

  size_t count{0};  //numero di record scritti nel file di output

  while(input_file) {
    std::getline(input_file, row, '\n'); //estraggo una singola riga (record)
    
    //1. ottengo la prima colonna: id (unsigned int)
    next_pos = row.find(delim, 0);
    token = row.substr(0, next_pos);
    record.id = strtoul(token.c_str(), nullptr, 10);
    last_pos = next_pos + 1;

    //2. ottengo la seconda colonna: name (char[64]) a dimensione variabile
    next_pos = row.find(delim, last_pos);
    token = row.substr(last_pos, next_pos-last_pos);
    strcpy(record.name, token.c_str());
    last_pos = next_pos + 1;

    //3. ottengo la terza colonna: country_code (char[2]) a dimensione fissa
    next_pos = row.find(delim, last_pos);
    token = row.substr(last_pos, next_pos-last_pos);
    strncpy(record.country_code, token.c_str(), 2);
    last_pos = next_pos + 1;

    //4. ottengo la quarta colonna: country_name (char[64]) a dimensione variabile
    next_pos = row.find(delim, last_pos);
    token = row.substr(last_pos, next_pos-last_pos);
    strcpy(record.country_name, token.c_str());
    last_pos = next_pos + 1;

    //5. ottengo la quinta colonna: population (unsigned int)
    next_pos = row.find(delim, last_pos);
    token = row.substr(last_pos, next_pos-last_pos);
    record.population = strtoul(token.c_str(), nullptr, 10);
    last_pos = next_pos + 1;

    //6. ottengo la sesta colonna: DIgital Elevation Model (unsigned int)
    next_pos = row.find(delim, last_pos);
    token = row.substr(last_pos, next_pos-last_pos);
    record.dem = strtoul(token.c_str(), nullptr, 10);
    last_pos = next_pos + 1;

    //7. ottengo la settima colonna: timezone (char[64]) a dimensione variabile
    next_pos = row.find(delim, last_pos);
    token = row.substr(last_pos, next_pos-last_pos);
    strcpy(record.timezone, token.c_str());
    last_pos = next_pos + 1;

    //8. ottengo l'ottava colonna: latitude (char[9]) a lunghezza variabile
    next_pos = row.find(delim, last_pos);
    token = row.substr(last_pos, next_pos-last_pos);
    strcpy(record.latitude, token.c_str());
    last_pos = next_pos + 1;

    //9. ottengo la nona colonna: longitude (char[9]) a lunghezza variabile
    next_pos = row.find(delim, last_pos);
    token = row.substr(last_pos, next_pos-last_pos);
    strcpy(record.longitude, token.c_str());
    last_pos = next_pos + 1;

    // std::cout << record.id << "|"\
    // << record.name << "|"\
    // << record.country_code[0] << record.country_code[1] << "|"\
    // << record.country_name << "|"\
    // << record.population << "|"\
    // << record.dem << "|"\
    // << record.timezone << "|"\
    // << record.latitude << "|"\
    // << record.longitude << "\n";
    // std::cin.get();

    const unsigned records_per_page{ PAGE_SIZE / sizeof(Record) };  //23
    const unsigned overhead_byte{ PAGE_SIZE % sizeof(Record) }; //48 byte
    const char overhead[overhead_byte]{0};

    output_file.write(reinterpret_cast<char*>(&record), sizeof(Record));  //! really bad
    count += 1;

    if( (count % records_per_page) == 0) {  //se abbiamo scritto un multiplo di 23
      output_file.write(overhead, overhead_byte);
    }
  }

  std::cout << "Scritte " << count << " righe di " << sizeof(Record) << " byte ciascuna.\n\
Totale scritto: " << output_file.tellp() << " bytes\n";

  return EXIT_SUCCESS;
}
