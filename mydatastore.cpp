#include <string>
#include <set>
#include <vector>
#include "product.h"
#include "user.h"
#include <iostream>

#include "mydatastore.h"
#include <bits/stdc++.h>

using namespace std;

MyDataStore::MyDataStore()
{}

MyDataStore::~MyDataStore()
{
  for(Product* cur : products_)
  {
    delete cur;
  }
  for(User* cur : users_)
  {
    delete cur;
  }
}

void MyDataStore::addProduct(Product* p)
{
  products_.push_back(p);
}

void MyDataStore::addUser(User* u)
{
  std::vector<Product*> newCart;
  users_.push_back(u);
  userCarts_.push_back(newCart);
}

bool MyDataStore::addToCart(User* u, Product* p)
{
  bool worked = true;
  auto it = find(users_.begin(), users_.end(), u);
  if (it != users_.end()) 
  {
      int spot = it - users_.begin();
      userCarts_[spot].push_back(p);
  } else {
    worked = false;
  }
  return worked;
}

std::vector<Product*> MyDataStore::getCart(User* u)
{
  std::vector<Product*> returnMe;
  auto it = find(users_.begin(), users_.end(), u);
  if (it != users_.end()) 
  {
      int spot = it - users_.begin();
      returnMe = userCarts_[spot];
  }
  return returnMe;
}

User* MyDataStore::getUser(std::string usernameCur)
{
  for(User* cur : users_)
  {
    std::string tempName = cur->getName();
    int cmp = strcasecmp(tempName.c_str(), usernameCur.c_str());
    if (cmp == 0)
    {
      return cur;
    }
  }
  return nullptr;
}

std::vector<Product*> MyDataStore::search(std::vector<std::string>& terms, int type)
{
  /**
     * Performs a search of products whose keywords match the given "terms"
     *  type 0 = AND search (intersection of results for each term) while
     *  type 1 = OR search (union of results for each term)
     */
  std::vector<Product*> returnMe;

  int tempInt = 0;
  if (type == 0)
  {
    for(Product* cur : products_)
    {
      bool yesIn = true;
      std::set<std::string> keywords = cur->keywords();
      std::set<std::string>::iterator it;
      tempInt = terms.size();
      for(int i = 0; i < tempInt; i++)
      {
        it = keywords.find(terms[i]);
        if (it == keywords.end())
		    {
			    yesIn = false;
          break;
		    }
      }
      if (yesIn)
      {
        returnMe.push_back(cur);
      }
    }
  } else if (type == 1) {
    for(Product* cur : products_)
    {
      bool yesIn = false;
      std::set<std::string> keywords = cur->keywords();
      std::set<std::string>::iterator it;
      tempInt = terms.size();
      for(int i = 0; i < tempInt; i++)
      {
        it = keywords.find(terms[i]);
        if (it != keywords.end())
		    {
			    yesIn = true;
          break;
		    }
      }
      if (yesIn)
      {
        returnMe.push_back(cur);
      }
    }
  }

  return returnMe;
}

void MyDataStore::dump(std::ostream& ofile)
{
  ofile << "<products>" << endl;
  for(Product* cur : products_)
  {
    cur->dump(ofile);
  }
  ofile << "</products>\n<users>" << endl;
  for(User* cur : users_)
  {
    cur->dump(ofile);
  }
  ofile << "</users>" << endl;
}