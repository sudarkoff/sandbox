
#ifndef __LOGFILE__H__
#define __LOGFILE__H__


#ifndef _FSTREAM_
#include <fstream>
using namespace std;
#endif


class CLogFile  
{
public:
    void PrintCurTime();
    CLogFile();
    CLogFile(TCHAR* tszFileName);
    ~CLogFile();
    CLogFile& operator <<(long              lVal);
    CLogFile& operator <<(const     TCHAR*  str);
    CLogFile& operator <<(TCHAR             tch);
    CLogFile& operator <<(int               nVal);
    CLogFile& operator <<(unsigned  long    ulVal);
    CLogFile& operator <<(double            dVal);
    CLogFile& operator <<(unsigned  int     unVal);
    void      LOGERROR(TCHAR* formatString, ...);   


private:
    ofstream    *m_cOutFile;
    void        Output( const TCHAR* data);


};

#endif  // __LOGFILE__H__