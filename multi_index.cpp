#include <algorithm>

#include "multi_index.h"

DieList::DieList()
{
}

DieList::~DieList()
{
}

void DieList::insert(Target& const target)
{
	Target targetCopy(target);

	DieList_t::iterator i = dieList_.insert(targetCopy);
	DieListBySpiral_.insert(std::make_pair(i.index(), i);
}

void DieList::erase(Index_t index)
{
	// ...
}

const Target& DieList::at(Index_t index)
{
	// ...
}
