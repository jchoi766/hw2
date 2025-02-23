
#include <string>
#include <set>
#include <vector>
#include <map>
#include <deque> 

#include "product.h"
#include "user.h"
#include "mydatastore.h"
#include "util.h"

using namespace std;


MyDataStore::~MyDataStore() {
  for (Product* p : products) {
    delete p;
  }

  for (User* u : users) {
    delete u;
  }
  
}

void MyDataStore::addProduct(Product* p) {
  cout <<"Adding product" << endl;
  products.insert(p);
  //update keywords map 
  set<string> kw = p->keywords();
  for (string k : kw) {
    if (keyWordsMap.find(k) != keyWordsMap.end()) {
      // add product to existing key's set of corresponding products 
      keyWordsMap.at(k).insert(p);
    } else {
      // add key to map and associated product 
      keyWordsMap.insert(make_pair(k, set<Product*>{p}));
    }
  }
}

void MyDataStore::addUser(User* u) {
  cout <<"Adding users" << endl;
  users.insert(u);
}
// each term in terms points to a set of products from keyWordsMap. 
// check union/intersection of these sets and return the full product set 
vector<Product*> MyDataStore::search(vector<string>& terms, int type){
  set<Product*> results;
  vector<Product*> resultVector;
  if (terms.size() < 1) return resultVector;
  if (type) {
    // type 1 - OR
    for (string term: terms) {
      results = setUnion(results, keyWordsMap[term]);
    } 
  } else {
    // type 0 - AND  
    // add in the first one 
    results = keyWordsMap[terms[0]];
    //results = setUnion(results, terms[0]);
    //set<string>::iterator itBegin = terms.begin(); // first term 
    //results = keyWordsMap[*itBegin];

    for (string term: terms) {
      
      results = setIntersection(results, keyWordsMap[term]);
    }
  }

  //convert set to vector and return vector 
  for (Product* p : results) {
    resultVector.push_back(p);
  }
  return resultVector;
}

void MyDataStore::dump(std::ostream& ofile) {
  

  ofile << "<products>" << endl;
  for (Product* p : products) {
    p->dump(ofile);
  }
  ofile << "</products>" << endl;
  ofile << "<users>" << endl;
  for (User* u : users) {
    u->dump(ofile);
  }
  ofile << "</users>";
}

vector<Product*>& MyDataStore::getCart(User* user) {
  /*User* findUser = nullptr;
  for (User* u : users) {
    if (u == user) {
      findUser = u;
      break;
    }
  }*/
  //cout << "gETTING CART for user at : " << user << endl;
  return carts[user];
}
/*
set<User*> MyDataStore::getUsers() {
  return users;
}*/

//if the user exists in the users set, return a pointer to the user
//return a null pointer if the user doesn't exist 
//case insensitive for username 
User* MyDataStore::userExists(string uName){
  for (User* u : users) {
    if (convToLower(u->getName()) == convToLower(uName)) {
      return u;
    }
  }
  return nullptr;
}

void MyDataStore::addToCart(User* user, Product* product) {
  //cout << "ADDING TO CART for user at : " << user << endl;
  carts[user].push_back(product);
}