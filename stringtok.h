/*
 * stringtok.h -- Breaks a string into tokens.  This is an example for lib3.
 *
 * Template function looks like this:
 *
 *    template <typename Container>
 *    void stringtok (Container &l,
 *                    string const &s,
 *                    char const * const ws = " \t\n");
 *
 * A nondestructive version of strtok() that handles its own memory and can
 * be broken up by any character(s).  Does all the work at once rather than
 * in an invocation loop like strtok() requires.
 *
 * Container is any type that supports push_back(a_string), although using
 * list<string> and deque<string> are indicated due to their O(1) push_back.
 * (I prefer deque<> because op[]/at() is available as well.)  The first
 * parameter references an existing Container.
 *
 * s is the string to be tokenized.  From the parameter declaration, it can
 * be seen that s is not affected.  Since references-to-const may refer to
 * temporaries, you could use stringtok(some_container, readline("")) when
 * using the GNU readline library.
 *
 * The final parameter is an array of characters that serve as whitespace.
 * Whitespace characters default to one or more of tab, space, and newline,
 * in any combination.
 *
 * 'l' need not be empty on entry.  On return, 'l' will have the token
 * strings appended.
 *
 *
 * [Example:
 *       list<string>       ls;
 *       stringtok (ls, " this  \t is\t\n  a test  ");
 *       for (list<string>::const_iterator i = ls.begin();
 *            i != ls.end(); ++i)
 *       {
 *            cerr << ':' << (*i) << ":\n";
 *       }
 *
 *  would print
 *       :this:
 *       :is:
 *       :a:
 *       :test:
 * -end example]
 *
 * pedwards@jaj.com  May 1999
*/

#include <string>

template <typename Container>
void stringtok (Container &container, std::string const &in,
           const char * const delimiters = " \t\n")
{
    const std::string::size_type len = in.length();
          std::string::size_type i = 0;

    while ( i < len )
    {
        // eat leading whitespace
        i = in.find_first_not_of (delimiters, i);
        if (i == std::string::npos)
            return;   // nothing left but white space

        // find the end of the token
        std::string::size_type j = in.find_first_of (delimiters, i);

        // push token
        if (j == std::string::npos) {
            container.push_back (in.substr(i));
            return;
        } else
            container.push_back (in.substr(i, j-i));

        // set up for next loop
        i = j + 1;
    }
}
