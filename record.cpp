#include "record.h"

#include <cstring>

Record::Record(const Record& copy):
  id{copy.id},
  population{copy.population},
  dem{copy.dem}
{
  strcpy(name, copy.name);
  strncpy(country_code, copy.country_code, 2);
  strcpy(country_name, copy.country_name);
  strcpy(timezone, copy.timezone);
  strcpy(latitude, copy.latitude);
  strcpy(longitude, copy.longitude);
}

Record& Record::operator=(const Record& copy) {
  if(this != &copy) { //Protection againts self-assignment
    id = copy.id;
    strcpy(name, copy.name);
    strncpy(country_code, copy.country_code, 2);
    strcpy(country_name, copy.country_name);
    population = copy.population;
    dem = copy.dem;
    strcpy(timezone, copy.timezone);
    strcpy(latitude, copy.latitude);
    strcpy(longitude, copy.longitude);
  }

  return *this;
}