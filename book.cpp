#include "book.h"
#include "util.h"

using namespace std;

Book::Book(const std::string name, double price, int qty, const std::string ISBN, const std::string author) :
  Product("book", name, price, qty)
{
  ISBN_ = ISBN;
	author_ = author;
}

std::set<std::string> Book::keywords() const
{
	//Returns the appropriate keywords that this product should be associated with
	std::set<std::string> returnMe = parseStringToWords(name_);
	std::set<std::string> temp1 = parseStringToWords(author_);
	returnMe.insert(temp1.begin(), temp1.end());
	returnMe.insert(ISBN_);
	return returnMe;
}

/*bool Book::isMatch(std::vector<std::string>& searchTerms) const
{
	//Allows for a more detailed search beyond simple keywords
}*/

std::string Book::displayString() const
{
	//Returns a string to display the product info for hits of the search
	string returnMe = (name_ + "\nAuthor: " + author_ + " ISBN: " + ISBN_ + "\n" + to_string(price_) + " " + to_string(qty_) + " left.");
	return returnMe;
}

void Book::dump(std::ostream& os) const
{
	//Outputs the product info in the database format
	Product::dump(os);
	os << ISBN_ << "\n" << author_ << endl;
}