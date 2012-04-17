#include <memory>

class SomeType {
public:
    SomeType();
    
private:
    class Impl;
    const std::auto_ptr<Impl> pimpl_;
};

class SomeType::Impl {
public:
    Impl() : str(0) {}
    
private:
    char* str;
};

SomeType::SomeType() : pimpl_(new SomeType::Impl) {}

int main(int argc, char* argv[])
{
    SomeType st;

    return 0;
}
