void split
    (string & text, string & separators, list<string> & words)
 {
    int n = text.length();
    int start, stop;

    start = text.find_first_not_of(separators);
    while ((start >= 0) && (start < n)) {
       stop = text.find_first_of(separators, start);
       if ((stop < 0) || (stop > n)) stop = n;
       words.push_back(text.substr(start, stop - start));
       start = text.find_first_not_of(separators, stop+1);
       }
 }
