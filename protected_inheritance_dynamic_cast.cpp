#include <iostream>

class Base {
protected:
        Base() {}
        void foo() {}
};

class Derived : protected Base {
public:
        Derived() {}
};

void goo(Base* b)
{
        std::cout << "Goooeee...";
        return;
}


int main (int argc, char* argv[])
{
        // this doesn't work
        Base* obj = new Derived;
        
        // neither does this
        goo(obj);

        // nor this
        if (dynamic_cast<Derived>(obj)) {
                std::cout << "Whoosh...";
        }

        // the whole thing simply fails to compile
        delete obj;

        return 0;
}
