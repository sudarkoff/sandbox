template<class T> struct C {
        typedef T qq;
        enum {  ww = 7 };
};

template<class T> struct D {
        enum { qq = C<T>::qq * 5};
        typedef typename C<T>::ww ww;
};

template<> struct C<int> {
        enum { qq = 5 };
        typedef int ww;
};


#include <iostream>
using namespace std;
int main() {
        D<int>::ww x = D<int>::qq;
        cout << x << endl;
        return 0;
}
