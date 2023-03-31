#include "record.h"

#include <cstring>

Record::Record(void):
  id{},
  name{0},
  country_code{0,0},
  country_name{0},
  population{0},
  dem{0},
  timezone{0},
  latitude{0},
  longitude{0} 
{ }


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
  id = copy.id;
  strcpy(name, copy.name);
  strncpy(country_code, copy.country_code, 2);
  strcpy(country_name, copy.country_name);
  population = copy.population;
  dem = copy.dem;
  strcpy(timezone, copy.timezone);
  strcpy(latitude, copy.latitude);
  strcpy(longitude, copy.longitude);

  return *this;
}