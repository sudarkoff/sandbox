#include <fstream>
#include <string>

using namespace std;

class link {
public:
	link() { id = 0; prev = next = NULL; }
	virtual ~link();

	void set(long _id) { id = _id; }
	link* prev() { return prev; }
	link* next() { return next; }
	ostream& operator<< (ostream& os) { return os << id; }
	void insert(long id);

private:
	long id;

	link* prev;
	link* next;
};

void link::insert(long _id)
{
	link* cur = new link;
	cur->id = _id;
}

void main(void)
{
    long i=0;
	link* head = new link;
	head->set(i++);
	
}
