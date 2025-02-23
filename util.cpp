#include <iostream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include "util.h"

using namespace std;
std::string convToLower(std::string src)
{
    std::transform(src.begin(), src.end(), src.begin(), ::tolower);
    return src;
}
/** Complete the code to convert a string containing a rawWord
    to a set of words based on the criteria given in the assignment **/
std::set<std::string> parseStringToWords(string rawWords)
{
  set<string> words;
  //split at punctuation, each word has at least 2 character words 
  //make every word lower case before putting in words 
  stringstream ss(rawWords);
  string word;
  while (ss >> word) {
    word = convToLower(word);
    string subWord = "";
  
    for (int i = 0; i < word.length(); i++) {
      if (isalpha(word[i])) { // not punctuation
        subWord += word[i];
      } else { // came across punctuation 
        if (subWord.length() > 1) {
          words.insert(subWord);
        }
        subWord = "";
      }
    }
    if (subWord.length() > 1) {
      words.insert(subWord);
    }
  }
  return words;
}

/**************************************************
 * COMPLETED - You may use the following functions
 **************************************************/

// Used from http://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
// trim from start
std::string &ltrim(std::string &s) {
    s.erase(s.begin(), 
	    std::find_if(s.begin(), 
			 s.end(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
}

// trim from end
std::string &rtrim(std::string &s) {
    s.erase(
	    std::find_if(s.rbegin(), 
			 s.rend(), 
			 std::not1(std::ptr_fun<int, int>(std::isspace))).base(), 
	    s.end());
    return s;
}

// trim from both ends
std::string &trim(std::string &s) {
    return ltrim(rtrim(s));
}
