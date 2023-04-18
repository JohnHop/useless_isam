#if !defined(RECORD_H)
#define RECORD_H

#include <iostream>

struct Record {
  unsigned int id;
  char name[70];
  char country_code[2];
  char country_name[39];
  unsigned int population;
  unsigned int dem;
  char timezone[31];
  char latitude[9];
  char longitude[9];

  Record(void);
  Record(const Record&);
  Record& operator=(const Record&);
};

inline std::ostream& operator<<(std::ostream& os, const Record& r) {
  return os << r.id << ", " << r.name << ", " << r.country_name << "\n"; //TODO lista campi incompleto
}

#endif // RECORD_H
