#ifndef _MULTI_INDEX_H__INCLUDED_
#define _MULTI_INDEX_H__INCLUDED_

#include <set>
#include <map>

class Target {
public:
	Target() : nextId_(0) {}
	~Target() {}

	index

private:
    int nextId_;
};

class DieList {
	// Data structure abbreviations
	typedef std::multiset<Target> DieList_t;
	typedef std::map<int, DieList_t::iterator> DieListBySpiral_t;
	typedef std::pair<long, long> Index_t;

public:
	DieList();
	~DieList();

	int insert(Target& const, Index_t);
	void erase(Index_t);
	const Target& at(Index_t);

	typedef DieList_t::const_iterator const_iterator;

	const_iterator begin() const { return dieList_.begin(); }
	const_iterator end() const { return dieList_.end(); }
	const_iterator find(Index_t) const;


private:
	// No copying allowed
	DieList(const DieList&); 
	DieList& operator=(const DieList&);

	DieList_t dieList_;
	DieListBySpiral_t dieListBySpiral_;
};

#endif // _MULTI_INDEX_H__INCLUDED_
