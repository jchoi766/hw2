#include <sstream>
#include <iomanip>
#include "util.h"
#include "clothing.h"
using namespace std;


Clothing::Clothing(const string category, const string name, const double price, const int qty, const string size, const string brand) : 
  Product(category, name, price, qty)
{
  size_ = size;
  brand_ = brand;

}


std::set<std::string> Clothing::keywords() const
{
  set<string> keyWords;
  keyWords = parseStringToWords(name_);

  // words in brand are searchable 
  set<string> brandSet = parseStringToWords(brand_);
  keyWords = setUnion(keyWords, brandSet);

  return keyWords;
}

void Clothing::dump(std::ostream& os) const {
  Product::dump(os);
  os << size_ << "\n" << brand_ << endl;
}

std::string Clothing::displayString() const 
{
  string output = "";

  output += name_ + "\nSize: " + size_ + " Brand: " + brand_ + "\n";
  output += to_string(price_) + " " + to_string(qty_) + " left.";
  return output;
}

