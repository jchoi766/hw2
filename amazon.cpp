#include <iostream>
#include <fstream>
#include <set>
#include <sstream>
#include <vector>
#include <iomanip>
#include <algorithm>

#include "product.h"
#include "db_parser.h"
#include "product_parser.h"
#include "util.h"

#include "mydatastore.h"

using namespace std;
struct ProdNameSorter {
    bool operator()(Product* p1, Product* p2) {
        return (p1->getName() < p2->getName());
    }
};
void displayProducts(vector<Product*>& hits);

int main(int argc, char* argv[])
{
    if(argc < 2) {
        cerr << "Please specify a database file" << endl;
        return 1;
    }

    /****************
     * Declare your derived DataStore object here replacing
     *  DataStore type to your derived type
     ****************/
    MyDataStore ds;


    // Instantiate the individual section and product parsers we want
    ProductSectionParser* productSectionParser = new ProductSectionParser;
    productSectionParser->addProductParser(new ProductBookParser);
    productSectionParser->addProductParser(new ProductClothingParser);
    productSectionParser->addProductParser(new ProductMovieParser);
    UserSectionParser* userSectionParser = new UserSectionParser;

    // Instantiate the parser
    DBParser parser;
    parser.addSectionParser("products", productSectionParser);
    parser.addSectionParser("users", userSectionParser);

    // Now parse the database to populate the DataStore
    if( parser.parse(argv[1], ds) ) {
        cerr << "Error parsing!" << endl;
        return 1;
    }

    cout << "=====================================" << endl;
    cout << "Menu: " << endl;
    cout << "  AND term term ...                  " << endl;
    cout << "  OR term term ...                   " << endl;
    cout << "  ADD username search_hit_number     " << endl;
    cout << "  VIEWCART username                  " << endl;
    cout << "  BUYCART username                   " << endl;
    cout << "  QUIT new_db_filename               " << endl;
    cout << "====================================" << endl;

    vector<Product*> hits;
    bool done = false;
    while(!done) {
        cout << "\nEnter command: " << endl;
        string line;
        getline(cin,line);
        stringstream ss(line);
        string cmd;
        if((ss >> cmd)) {
            if( cmd == "AND") {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 0);
                displayProducts(hits);
            }
            else if ( cmd == "OR" ) {
                string term;
                vector<string> terms;
                while(ss >> term) {
                    term = convToLower(term);
                    terms.push_back(term);
                }
                hits = ds.search(terms, 1);
                displayProducts(hits);
            }
            else if ( cmd == "QUIT") {
                string filename;
                if(ss >> filename) {
                    ofstream ofile(filename.c_str());
                    ds.dump(ofile);
                    ofile.close();
                }
                done = true;
            }
	    /* Add support for other commands here */
            // ADD username search_hit_number 
            else if (cmd == "ADD") {
              string uName;
              int hitIndex; 
              ss >> uName;
              if (!ss.fail()) {
                User* user = ds.userExists(uName);
                if (user != nullptr) {
                  ss >> hitIndex;
                  if (!ss.fail() && hitIndex >= 0) {
                    //add product at hitIndex to user's cart 
                    hitIndex -= 1;
                    Product* product = hits[hitIndex];
                    ds.addToCart(user, product);

                  } else {
                    // index is invalid / not provided 
                    cout << "Invalid request" << endl;
                  }
                } else {
                  // this user doesn't exist
                  cout << "Invalid request" << endl;
                }
              } else {
                //no username entered 
                cout << "Invalid request" << endl;
              }
              
            }
            // VIEWCART username 
            else if (cmd == "VIEWCART") {
              string uName;
              ss >> uName;
              User* user = ds.userExists(uName);
              
              if (user != nullptr) {
                vector<Product*>& cart = ds.getCart(user);
                // print out the cart 
                int indexNum = 1;
                for (Product* p : cart) {
                  cout << "Item " << indexNum << endl;
                  cout << p->displayString() << endl;
                  indexNum++;
                }
              } else cout << "Invalid username" << endl;
            }
            // BUYCART username
            else if (cmd == "BUYCART") {
              string uName;
              ss >> uName;
              User* user = ds.userExists(uName);
              if (user != nullptr) {
                //iterate through user's cart 
                vector<Product*>::iterator it;
                vector<Product*>& cart = ds.getCart(user);
                for (it = cart.begin(); it != cart.end();) {
                  Product* p = *it;
                  double productPrice = p->getPrice();
                  if (user->getBalance() >= productPrice && p->getQty() >= 1) {
                    // if user has enough money and item is in stock -> 
                    // reduce product's quantity by one 
                    p->subtractQty(1);
                    // product price debited from user's credit
                    user->deductAmount(productPrice); 
                    //remove item from user's cart 
                    it = cart.erase(it); // returns next iterator after erasing current 
                  } else {
                    //nothing deleted - increment iterator to check next item in cart 
                    it++;
                  }
                }
              } else cout << "Invalid username" << endl;
            }
            else {
                cout << "Unknown command" << endl;
            }
        }

    }
    return 0;
}

void displayProducts(vector<Product*>& hits)
{
    int resultNo = 1;
    if (hits.begin() == hits.end()) {
    	cout << "No results found!" << endl;
    	return;
    }
    std::sort(hits.begin(), hits.end(), ProdNameSorter());
    for(vector<Product*>::iterator it = hits.begin(); it != hits.end(); ++it) {
        cout << "Hit " << setw(3) << resultNo << endl;
        cout << (*it)->displayString() << endl;
        cout << endl;
        resultNo++;
    }
}
