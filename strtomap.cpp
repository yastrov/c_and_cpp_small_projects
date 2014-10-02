/*
* Convert string "key1:val1:key2:val2" to map.
*
* Программа для преобразования строки вида:
* "key1:val1:key2:val2" в map.
* @author Yuri Astrov <yuriastrov@gmail.com>
*         Юрий Астров
*/
#include <iostream>
#include <map>
#include <string>
#include <list>
#include <algorithm>
#include <iterator>
using namespace std;

list<string> split(const string& s,
                   const string& delim = " ",
                   const bool keep_empty = true) {
    list<string> result;
    if (delim.empty()) {
        result.push_back(s);
        return result;
    }
    string::const_iterator sstart = s.begin(), send = s.begin();
    while (send != s.end()) {
        send = search(sstart, s.end(), delim.begin(), delim.end());
        string temp(sstart, send);
        if (keep_empty || !temp.empty()) {
          result.push_back(temp);
        }
        if (send != s.end()) {
          sstart = send + delim.size();
        }
    }
    return result;
}

map<string, string> str_to_dict(const list<string>& s){
    map<string, string> result;
    short i = 0;
    string key, val;
    //for ( auto val : result )
    //for ( auto itr = s.begin(), end = s.end(); itr != end; ++itr )
    for_each(s.begin(), s.end(), [&key, &val, &result, &i] (string val)
    {  
        if (i%2 == 0) {
	          key = val;
	          i++;
	      }
	      else{
	         result[key] = val;
	         i=0;
	      }
    });
    return result;
}
map<string, list<string>> str_to_dict2(const list<string>& s){
    map<string, list<string>> result;
    short i = 0;
    string key, val;
    for_each(s.begin(), s.end(), [&key, &val, &result, &i] (string val)
    {  
        if (i%2 == 0) {
	        key = val;
	        i++;
	    }
	    else{
			result[key].push_back(val);
	       i=0;
	    }
    });
    return result;
}

int main() {
    string str = "key1:val1:key2:val2";
    const list<string> words = split(str, ":");
    cout << "Input string:" << endl;
    cout << str << endl;
    cout << "Tokenize:" << endl;
    cout << words << endl;
    copy(words.begin(), words.end(), ostream_iterator<string>(cout, "\n"));
    map<string, string> first = str_to_dict(words);
    /*for (auto& kv : first) {
       cout << kv.first << " has value " << kv.second << endl;
    }*/
    cout << "Output map:" << endl;
    for_each( first.begin(), first.end(), [] (pair<string, string> pr)
    {  
        cout << pr.first << " - " << pr.second << endl;
    });
   return 0;
}
