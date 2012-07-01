#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

string getTag(const string& str, long& POS)
{
    string tag="";

    while( POS < str.size() )
    {
        switch ( str[POS] )
        {
            case '>':
                POS++;
                return tag;
                break;
            case '<':
                tag = "";
                POS++;
            default:
                tag += str[POS++];
                break;
        }
    }
    return tag;
}

string getVerb(const string& tag)
{
    string verb="";
    long j=0;
    while ((tag[j] != ' ') && (j < tag.size()))
        verb += tag[j++];
    return verb;
}

string getHREF(const string& tag)
{
    string href="";
    long pos = tag.find_first_of("HREF");
    pos = tag.find_first_of('=', pos);
    pos = tag.find_first_of('\"', pos) + 1;
    while ((tag[pos] != '\"') && (pos < tag.size()))
        href += tag[pos++];
    return href;
}

string getData(const string& str, long& POS)
{
    string data="";

    while( POS < str.size() )
    {
        switch ( str[POS] )
        {
            case '<':
                return data;
                break;
            case '\n':
                POS++;
                break;
            default:
                data += str[POS++];
                break;
        }
    }
    return data;
}

string makeFileName(string href)
{
    string ILLCHARS = " ~!@#$%^&*\\|/?[]{}=+,:;'`\"";
    string filename;

    for (long i = 0; i < href.size(); i++)
        if ( ILLCHARS.find(href[i]) == -1 )
            filename += href[i];
        else
            filename += "_";

    long pos = filename.find("___");
    if (pos != -1)
        filename = filename.substr(pos + 3);

    return filename+".url";
}

int main (int argc, char* argv[])
{
    if (argc < 2) // if no parameters
    {
        cout << "Extracts URLs from Yahoo's bookmarks export file." << endl;
        cout << "Copyright (c) 2001 by George Sudarkoff. mailto:support@sudarkoff.com" << endl;
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return (-1);
    }

    // load file to string
    ifstream fstrm(argv[1]);
    istreambuf_iterator<char> beg(fstrm), end;
    string str;
    copy(beg, end, inserter(str, str.end()));

    long pos = 0;
    long level = 0;
    vector<string> path;
    string tag, verb, href;

    while( pos < str.size() )
    {
        tag = getTag(str, pos);
        verb = getVerb(tag);

        if (verb == "DL")
        {
            tag = getTag(str, pos);
            verb = getVerb(tag);
            level++;
        }
        else if (verb == "/DL")
        {
            tag = getTag(str, pos);
            verb = getVerb(tag);
            level--;
        }
        else if (verb == "DT")
        {
        }
        else if (verb == "H3")
        {
            string dirname = getData(str, pos);
            cout << endl << level << string(level-1, '\t') << dirname << endl;
            // ComposePath(...);
            // MakeSureDirectoryPathExists(...); // WIN32 API

        }
        else if (verb == "A")
        {
               href = getHREF(tag);
               cout << string(level-1, '\t') << href << endl;
               string filename = makeFileName(href);
            ofstream ofstrm(filename.c_str());
            ofstrm << "[InternetShortcut]\n"
                << "URL=" << href << endl
                << "DESC=" << getData(str, pos);
        }
    }

    return (0);
}
