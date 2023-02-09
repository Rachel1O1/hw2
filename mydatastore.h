#ifndef MYDATASTORE_H
#define MYDATASTORE_H
#include <string>
#include <set>
#include <vector>
#include "product.h"
#include "user.h"

#include "datastore.h"

class MyDataStore : public DataStore {
public:
    MyDataStore();
		~MyDataStore();

    void addProduct(Product* p);

    void addUser(User* u);

    bool addToCart(User* u, Product* p);

    std::vector<Product*> getCart(User* u);

    User* getUser(std::string usernameCur);

    std::vector<Product*> search(std::vector<std::string>& terms, int type);

    void dump(std::ostream& ofile);

private:
		std::vector<Product*> products_;
		std::vector<User*> users_;
    std::vector<std::vector<Product*>> userCarts_;
};
#endif