template <class T>
class link {
protected:
	link<T> *prev;
	link<T> *next;
	T value;

public:
	link() { prev = 0L;  next = 0L; }
	~link() {}

	link<T>* Prev() { return prev; }
	link<T>* Next() { return next; }
	T Value() { return value; }

	link<T>* InsertAfter(T _val)
	{
		link<T>* ptr = new T;
		ptr->prev = this;
		ptr->next = next;
		next = ptr;
		ptr->value = _val;
	}
};

void main(void)
{
	link<char> chl;
}
