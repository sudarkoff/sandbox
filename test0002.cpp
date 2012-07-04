#include <iostream>
#include <string>
#include <map>
using namespace std;

void Empty (void) {
    
};

typedef map<string,void*> MAPii;
typedef MAPii::value_type value_type;

value_type array[] = {
    value_type("tic", &Empty),
    value_type("tac", &Empty),
    value_type("toe", &Empty)
};

int main (void) {
    MAPii cont (array, array+sizeof(array)/sizeof(value_type));
    MAPii::iterator pos;
    
    for (pos = cont.begin(); pos != cont.end(); ++pos)
        cout << pos->first << endl;
    return 0;
}
