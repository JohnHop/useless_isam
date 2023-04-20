#if !defined(RECORD_H)
#define RECORD_H

#include <iostream>

struct Record {
  int id;
  char name[70];
  char country_code[3];
  char country_name[39];
  int population;
  int dem;
  char timezone[31];
  char latitude[9];
  char longitude[9];

  Record() = default;
  Record(const Record&);
  Record& operator=(const Record&);
};

inline std::ostream& operator<<(std::ostream& os, const Record& r) {
  return os << r.id << ", " << r.name << ", " << r.country_name << ", " << r.population << "\n"; //TODO: mancano alcuni campi
}

#endif // RECORD_H
