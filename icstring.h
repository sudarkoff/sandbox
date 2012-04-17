/* It is based on Nicolai Josuttis' sample in 'The C++ Standard Library'
   ISBN 0201379260
*/

///////////////////////////////////////////////////////////////////////
// ic_char_traits
// replace functions of standard char_traits<> so that strings
// behave in a case-insensitive way
template<class T> struct ic_char_traits : public std::char_traits<T>
{
 static bool eq(const T& c1, const T& c2)
 {
  return toupper(c1) == toupper(c2);
 }

 static bool lt(const T& c1, const T& c2)
 {
  return toupper(c1) < toupper(c2);
 }

 static int compare(const T* s1, const T* s2, size_t n)
 {
  for (size_t i = 0; i < n; i++)
  {
   if (!eq(s1[i], s2[i]))
   {
    return lt(s1[i], s2[i]) ? -1 : 1;
   }
  }
  return 0;
 }

 static const T* find(const T* s, size_t n, const T& c)
 {
  for (size_t i = 0; i < n; i++)
  {
   if (eq(s[i], c))
   {
    return &(s[i]);
   }
  }
  return 0;
 }
};

// define a special type for case insensitive strings (that also are the correct size based on the UNICODE define)
typedef std::basic_string<TCHAR, ic_char_traits<TCHAR> > icstring;

/*
The above is flawed in a couple of ways. First, T will typically be
char, which is typically signed, so you must cast to unsigned char
before calling any <ctype.h> functions, which are defined only for int
values in the range of unsigned char and -1. Second, the standard
doesn't require that the general template char_traits be defined. I
believe it only has to be declared so that you can specialize it, so
trying to inherit things from it is IMO invalid. It's been a while
since I looked at this, but from what I recall, it's not possible to
write a generic char_traits<T> that will work for any T legal for
char_traits. In fact, I don't even know of any implementation of
char_traits<char> that adheres to the C++ Standard when plain char is
signed, but IMO, that's due to flaws in the Standard's definition of
char_traits.

>// define a special type for case insensitive strings (that also are the
>correct size based on the UNICODE define)
>typedef std::basic_string<TCHAR, ic_char_traits<TCHAR> > icstring;

Your ic_char_traits<TCHAR> has a problem if UNICODE is defined. You
can't use toupper for wide chars! In general, never try to write a
generic char_traits. Always specialize it for a specific type.
*/

/*
> Your ic_char_traits<TCHAR> has a problem if UNICODE is defined. You
> can't use toupper for wide chars! In general, never try to write a
> generic char_traits. Always specialize it for a specific type.
>
Good point.  Too bad they didn't provide a TCHAR-compatible version of that
function (and that it compiles without complaint on VC6).  This modification
works:

#ifdef _UNICODE
#define totupper(c)  towupper(c)
#else
#define totupper(c)  toupper((unsigned char)(c))        // ok, here's your cast
#endif

///////////////////////////////////////////////////////////////////////
// ic_char_traits
// replace functions of standard char_traits<> so that strings
// behave in a case-insensitive way
template<class T> struct ic_char_traits : public std::char_traits<T>
{
 static bool eq(const T& c1, const T& c2)
 {
  return totupper(c1) == totupper(c2);
 }

 static bool lt(const T& c1, const T& c2)
 {
  return totupper(c1) < totupper(c2);
 }
...
*/