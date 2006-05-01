typedef void (*FP) (char*);

class Socket {
   FP fn;

protected:
   void OnReceive(void)
   {
      // ...
      char* arr;
      fn(arr);
   }

public:
   void RegisterEventHandler(FP fn_)
   {
      fn = fn_;   
   }
   // ...
};

class XML {
   Socket soc;
   void SomeMethod(void)
   {
      // ...
      soc.RegisterEventHandler(EventHandler);
      // ...
   }

   void SomeOtherMethod(void)
   {
      // ...
   }

public:
   static void EventHandler(char*)
   {
      // SomeOtherMethod(); // Can't do it!
      // ...
   }
   // ...
};

int cdecl main(void)
{
   return 0;
}
