#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include <string>
#include <set>
#include <vector>
#include <queue>
#include <map>
#include <deque> 


#include "datastore.h"
#include "product.h"
#include "user.h"

class MyDataStore : public DataStore {
  public:
    ~MyDataStore();
    void addProduct(Product* p);
    void addUser(User* u);
    std::vector<Product*> search(std::vector<std::string>& terms, int type);
    void dump(std::ostream& ofile);

    std::vector<Product*>& getCart(User* u); // return user's cart 
    //std::set<User*> getUsers();
    User* userExists(std::string uName);
    void addToCart(User* user, Product* product);

  protected: 
    //data members 
    std::set<Product*> products;
    std::set<User*> users;
    //cart for each user 
    std::map<User*, std::vector<Product*>> carts;
    // keywords: string key -> <product1, product2> 
    std::map< std::string, std::set<Product*> > keyWordsMap;

};

#endif