struct Object { int value; };
typedef std::map< std::string, Object > main_map_t;
typedef std::set< main_map_t::iterator, lookup_cmp > lookup_set_t;

where lookup_cmp is a function which orders two main_map_iterators, i.e.

bool lookup_cmp(main_map_t::iterator lhs, main_map_t::iterator rhs)
{
    if (lhs->second.value < rhs->second.value)
        return true;
    if (lhs->second.value > rhs->second.value)
        return false;
    return lhs->first < rhs->first;
}

/*
You don't duplicate Object::value which is possibly better than my 
version, though the insert{} and find() cost will be higher. 

The killer point though is how do you find anything, since a
main_map_t::iterator is required you need to get this from
main_map before you can search the lookup set.
*/