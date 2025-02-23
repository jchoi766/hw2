#include <sstream>
#include <iomanip>

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>

#include "util.h"
#include "movie.h"

using namespace std;


Movie::Movie(const string category, const string name, const double price, const int qty, const string genre, const string rating) : 
  Product(category, name, price, qty)
{
  genre_ = genre;
  rating_ = rating;
}

std::set<std::string> Movie::keywords() const
{
  set<string> keyWords;
  keyWords = parseStringToWords(name_);

  // genre is searchable (verbatim) 
  string genre = convToLower(genre_);
  keyWords.insert(genre);
  
  return keyWords;
}

void Movie::dump(std::ostream& os) const {
  Product::dump(os);
  os << genre_ << "\n" << rating_ << endl;
}

std::string Movie::displayString() const 
{
  string output = "";

  output += name_ + "\nGenre: " + genre_ + " Rating: " + rating_ + "\n";
  output += to_string(price_) + " " + to_string(qty_) + " left.";
  return output;
}

