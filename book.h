#ifndef BOOK_H
#define BOOK_H
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>

#include "product.h"

class Book : public Product{
  public:
    Book(std::string category, std::string name, double price, int qty, std::string isbn, std::string author);
    std::set<std::string> keywords() const;
    std::string displayString() const;
    void dump(std::ostream& os) const;

  protected:
    std::string category_; // book 
    std::string isbn_;
    std::string author_;

};
#endif
