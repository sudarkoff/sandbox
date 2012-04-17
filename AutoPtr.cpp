struct Base {};
struct Derived : Base {};

std::auto_ptr<Derived> source();
void sink( std::auto_ptr<Base> );

int main()
{
    sink( source() );
}
