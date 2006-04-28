class A {
public:
    virtual void foo() {}
};

class B {
public:
    virtual void foo() {}
};

class C :
    public A, public B
{
public:
    // Have to overload this in order for this call to be unambigious.
    virtual void foo() {}
};

int main()
{
    C c;
    c.foo();
    return 0;
}
