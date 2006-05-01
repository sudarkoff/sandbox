template <bool b, int inhibit_inst = 0> struct executor;

template <int inhibit_inst> struct executor<true, inhibit_inst>
{
   static void foo() { /* compile this */ }
};

template <int inhibit_inst> struct executor<false, inhibit_inst>
{
   static void foo() { /* compile that */ }
};


template <bool b> inline void execute() { return executor<b>::foo(); }

int main()
{
   execute<true>();
}
