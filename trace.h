#ifndef TRACE_H_
#define TRACE_H_

#include <stdio.h>
#include <stdlib.h>

#define TRACE_BACKTRACE_FRAMES 10

#ifdef __GNUC__
# define __FUNCTION__ __PRETTY_FUNCTION__
#endif

/* Emacs-style output */
#ifdef EMACS
# define TRACE_PREFIX   fprintf (stderr, "%s:%d:%s", __FILE__, __LINE__, __FUNCTION__)
#else
# define TRACE_PREFIX   fprintf (stderr, "%s(%d):%s", __FILE__, __LINE__, __FUNCTION__)
#endif /* EMACS */

#ifdef linux
# include <execinfo.h>

# define TRACE_DUMP()   do {     void *array[TRACE_BACKTRACE_FRAMES];     int n = backtrace (array, sizeof (array) / sizeof (void *));     char **symbols = backtrace_symbols (array, n);     int i;      for (i = 0; i < n; ++i)       fprintf (stderr, " -[%d]%s\n", i, symbols[i]);      free (symbols);   } while (0)
#else
# define TRACE_DUMP()
#endif /* linux */

#define TRACE()   TRACE_PREFIX; fprintf (stderr, "\n");   TRACE_DUMP ()

#define TRACE_MSG(MSG)   TRACE_PREFIX; fprintf (stderr, ": %s\n", MSG);   TRACE_DUMP ()

#endif /* TRACE_H_ */
