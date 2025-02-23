#include <sstream>
#include <iomanip>
#include "util.h"
#include "book.h"
using namespace std;


Book::Book(const string category, const string name, const double price, const int qty, const string isbn, const string author) : 
  Product(category, name, price, qty)
{
  isbn_ = isbn;
  author_ = author;

}


std::set<std::string> Book::keywords() const
{
  set<string> keyWords;
  keyWords = parseStringToWords(name_);

  // isbn 
  keyWords.insert(isbn_);
  // author 
  set<string> authorSet = parseStringToWords(author_);
  keyWords = setUnion(keyWords, authorSet);

  return keyWords;
}

void Book::dump(std::ostream& os) const {
  Product::dump(os);
  os << isbn_ << "\n" << author_ << endl;
}

std::string Book::displayString() const 
{
  string output = "";

  output += name_ + "\nAuthor: " + author_ + " ISBN: " + isbn_ + "\n";
  output += to_string(price_) + " " + to_string(qty_) + " left.";
  return output;
}

