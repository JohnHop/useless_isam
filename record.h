#if !defined(RECORD_H)
#define RECORD_H

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

public:
  Record(void);
  Record(const Record&);
  Record& operator=(const Record&);
};

#endif // RECORD_H
