#include <iostream>

////////////////////////////////////////////////////////////////////////
// Abstract base class for event handlers
class CommonEventHandler 
{
public:
   virtual void SomeEventHandler(char*) = 0;
};


////////////////////////////////////////////////////////////////////////
// Class, which will call an event handler
class Socket 
{
private:
   CommonEventHandler *phnd;

protected:
   void OnReceive(void)
   {
      std::cout << "void Socket::OnReceive(void)" << std::endl;
      char str[] = "Just some data, acquired by Socket...";
      phnd->SomeEventHandler(str);
   }

public:
   Socket() : phnd(0L) {}

   void RegisterEventHandler(CommonEventHandler *phnd_)
   {
      std::cout << "void Socket::RegisterEventHandler(CommonEventHandler)" << std::endl;
      phnd = phnd_;
   }

   void ReceivePacket(void)
   {
      OnReceive();
   }
};


////////////////////////////////////////////////////////////////////////
// Class, which will handle an even
class XML : public CommonEventHandler 
{
private:
   Socket soc;

   void SomeMethod(void)
   {
      std::cout << "void XML::SomeMethod(void)" << std::endl;
   }

public:
   XML() { };

   void SomeEventHandler(char* str)
   {
      std::cout << "void XML::SomeEventHandler(char*)" << std::endl;
      std::cout << str << std::endl;
      SomeMethod(); // Yes, you can!
   }

   void Run(void)
   {
      std::cout << "void XML::Run(void)" << std::endl;
      soc.RegisterEventHandler(this);
      soc.ReceivePacket();
   }
};


////////////////////////////////////////////////////////////////////////
// Let's test it
int cdecl main(void)
{
   XML xml;
   xml.Run();

   return 0;
}
