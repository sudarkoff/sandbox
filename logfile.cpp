#include "stdafx.h"
#include <strstream>
#include "logfile.h"

CLogFile::CLogFile()
{
    m_cOutFile = new ofstream(_T("c:\\default.log"), ios_base::out);  
    if (!m_cOutFile->is_open())
    {
#ifdef _MFC_VER
        AfxMessageBox(_T("Unable to create log file"));        
#endif
        delete m_cOutFile;
        m_cOutFile = NULL;
    }
}

CLogFile::CLogFile(TCHAR* tszFileName)
{
    m_cOutFile = new ofstream(tszFileName, ios_base::out);  
    if (!m_cOutFile->is_open())
    {
#ifdef __MFC_VER
        AfxMessageBox(_T("Unable to create log file"));        
#endif
        delete m_cOutFile;
        m_cOutFile = NULL;
    }
}

CLogFile::~CLogFile()
{
    m_cOutFile->close();
    delete m_cOutFile;
    m_cOutFile = NULL;
}

void CLogFile::Output(const TCHAR *data)
{
    m_cOutFile->write(data,_tcslen(data));
}

void CLogFile::PrintCurTime()
{
    TCHAR dateString[52];
    SYSTEMTIME cur;
    GetSystemTime(&cur);

    sprintf(dateString,"%02d/%02d/%02d %02d:%02d:%02d ", cur.wMonth, cur.wDay,
        cur.wYear, cur.wHour, cur.wMinute, cur.wSecond);

    Output(dateString);
}

CLogFile& CLogFile::operator <<(unsigned int unVal)
{
    strstream tmp;
    tmp << unVal;
    tmp << '\0';

    TCHAR* output = tmp.str();

    Output(output);

    tmp.freeze(false);
    return *this;
}

CLogFile& CLogFile::operator <<(long lVal)
{
    strstream tmp;
    tmp << lVal;
    tmp << '\0';

    TCHAR* output = tmp.str();

    Output(output);

    tmp.freeze(false);

    return *this;
}

CLogFile& CLogFile::operator <<(const TCHAR* str)
{
    Output(str);
    return *this;
}

CLogFile& CLogFile::operator <<(TCHAR   tch)
{
    TCHAR szCh[2];
    szCh[0] = tch;
    szCh[1] = '\0';
    Output(szCh);
    return *this;
}

CLogFile& CLogFile::operator <<(int nVal)
{
    strstream tmp;
    tmp << nVal;
    tmp << '\0';

    TCHAR* output = tmp.str();

    Output(output);

    tmp.freeze(false);
    return *this;
}

CLogFile& CLogFile::operator <<(unsigned long ulVal)
{
    strstream tmp;
    tmp << ulVal;
    tmp << '\0';

    TCHAR* output = tmp.str();
    Output(output);
    tmp.freeze(false);
    return *this;
}

CLogFile& CLogFile::operator <<(double dVal)
{
    strstream tmp;
    tmp << dVal;
    tmp << '\0';

    TCHAR* output = tmp.str();
    Output(output);
    tmp.freeze(false);
    return *this;
}

void CLogFile::LOGERROR(TCHAR* formatString, ...)
{

    PrintCurTime();
    if ( formatString == NULL)
    {
        return;
    }
        
    va_list argList;
    va_start(argList, formatString);
    TCHAR* ptr = formatString;

    while(*ptr != '\0')
    {
        TCHAR* str          = NULL;
        int nInteger        = 0;
        unsigned int unInt  = 0;
        long lLong          = 0;
        unsigned long ulLong= 0;
        double  dDoub       = 0;

        if(*ptr == '%')
        {
            switch(*(ptr+1))
            {
            case 's':
                str = va_arg(argList, TCHAR*);

                if( NULL == str)
                    break;
                *this << str;
                ptr++;
                break;

            case 'd':
                nInteger = va_arg(argList, int);
                *this << nInteger;
                ptr++;
                break;

            case 'u':
                unInt = va_arg(argList, unsigned int);
                *this << unInt;
                ptr++;
                break;

            case 'l':
                ptr++;
                if(*(ptr+1) == 'd')
                {
                    lLong = va_arg(argList, long);
                    *this << lLong;
                    ptr++;
                }
                else if(*(ptr+1) == 'u')
                {
                    ulLong = va_arg(argList, unsigned long);
                    *this << ulLong;
                    ptr++;
                }
                break;
            case 'f':
                dDoub = va_arg(argList, double);
                *this << dDoub;
                ptr++;
                break;
            default:
                *this << *ptr;
            }
        } //  if(*ptr == '%')
        else
        {
            *this << *ptr;
        }
        ptr++;
    }

    *this << '\n';

}
