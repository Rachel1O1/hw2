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
            else if ( cmd == "ADD") {
                string curUser;
                int hitIndex;
                bool worked = false;
                if(ss >> curUser) {
                    if(ss >> hitIndex) {
                        int sss = hits.size();
                        if ((hitIndex >= 0) && (hitIndex < sss))
                        {
                            User* tempUser = ds.getUser(curUser);
                            worked = ds.addToCart(tempUser, hits[hitIndex]);
                            if (!worked) {
                                cout << "Invalid username" << endl;
                            }
                        } else {
                            cout << "Invalid index" << endl;
                        }
                    }
                }
            }
            else if ( cmd == "VIEWCART") {
                string curUser;
                if(ss >> curUser) {
                    User* tempUser = ds.getUser(curUser);
                    if (tempUser != nullptr)
                    {
                        std::vector<Product*> curCart = ds.getCart(tempUser);
                        int count = 1;
                        for(Product* cur : curCart)
                        {
                            cout << count << ". " << cur->displayString() << endl;
                            count++;
                        }
                    } else {
                        cout << "Invalid username" << endl;
                    }
                }
            }
            else if ( cmd == "BUYCART") {
                string curUser;
                if(ss >> curUser) {
                    User* tempUser = ds.getUser(curUser);
                    if (tempUser != nullptr)
                    {
                        //item in stock and user has money
                        std::vector<Product*> curCart = ds.getCart(tempUser);
                        for(Product* cur : curCart)
                        {
                            int tempInt = cur->getQty();
                            int tempBal = tempUser->getBalance();
                            int tempCost = cur->getPrice();
                            if ((tempInt > 0) && (tempBal >= tempCost))
                            {
                                cur->subtractQty(1);
                                tempUser->deductAmount(cur->getPrice());
                                //remove request from cart;
                            }
                        }
                    } else {
                        cout << "Invalid username" << endl;
                    }
                }
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
