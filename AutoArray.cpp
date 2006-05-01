#include <memory.h>

const unsigned YES_AUTOARRAY_SIZE = 25;

class autoArray 
{
private:
	unsigned m_size;
	unsigned m_realSize;
	unsigned *m_pArray;

protected:
	unsigned reallocate();

public:
	autoArray() : m_size(YES_AUTOARRAY_SIZE), m_realSize(0) { m_pArray = new unsigned[m_size]; }
	~autoArray() { if (m_pArray) delete m_pArray; }

	unsigned append(unsigned);
	unsigned deleteAll();
	unsigned getSize() { return m_realSize; }

	unsigned operator [] (unsigned _i) { return (_i <= m_realSize) ? m_pArray[_i]: 0; }
	unsigned* operator & () const { return m_pArray; }
};

unsigned autoArray::reallocate()
{
	m_size += YES_AUTOARRAY_SIZE;	// increase the size of array by SIZE
	unsigned *pNewArray = new unsigned[m_size]; // allocate new chunk of memory
	memcpy(pNewArray, m_pArray, m_realSize); // copy old data to new location
	delete m_pArray; // deallocate old memory
	m_pArray = pNewArray; // store new memory address in member variable
	return m_size; // return the amount of allocated memory
}

unsigned autoArray::append(unsigned _val)
{
	if (m_size == m_realSize) reallocate();
	m_pArray[++m_realSize] = _val;
	return m_realSize;
}

unsigned autoArray::deleteAll()
{
	m_realSize = 0;
	return m_realSize;
}

#include <iostream>
using std::cout;
using std::endl;

void main (void)
{
	autoArray arr;
	
	for (int i=0; i<38; i++)
		arr.append(i*10);

	unsigned *myArr = &arr;
	for (i=0; i<arr.getSize(); i++)
		cout << myArr[i] << endl;
}
