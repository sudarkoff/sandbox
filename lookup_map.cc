#include <map>
#include <string>

struct Object { int value; };
typedef std::map< std::string, Object > main_map_t;
typedef std::map< int, main_map_t::iterator > lookup_map_t

main_map_t     main_map;
lookup_map_t    lookup_map;

//...

void index()
{
  main_map_t::iterator ptr, term;

  ptr = main_map.begin();
  term = main_map.end();

  for ( ; ptr != term; ++ptr )
  {
    lookup_map[ ptr->second.value ] = ptr;
  }
} 

void lookup( int key )
{
  lookup_map_t::iterator ptr = lookup_map.find( key );
  if ( ptr == lookup_map.end() ) return; // or throw ...

  std::string const &name = ptr->second->first;
  Object &object = ptr->second->second;

  // do something
}
