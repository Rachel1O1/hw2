#include "clothing.h"
#include "util.h"

using namespace std;

Clothing::Clothing(const std::string name, double price, int qty, const std::string size, const std::string brand) :
  Product("clothing", name, price, qty)
{
  size_ = size;
	brand_ = brand;
}

std::set<std::string> Clothing::keywords() const
{
	//Returns the appropriate keywords that this product should be associated with
	std::set<std::string> returnMe = parseStringToWords(name_);
	std::set<std::string> temp1 = parseStringToWords(brand_);
	returnMe.insert(temp1.begin(), temp1.end());
	return returnMe;
}

/*bool Clothing::isMatch(std::vector<std::string>& searchTerms) const
{
	//Allows for a more detailed search beyond simple keywords
}*/

std::string Clothing::displayString() const
{
	//Returns a string to display the product info for hits of the search
	string returnMe = (name_ + "\nSize: " + size_ + " Brand: " + brand_ + "\n" + to_string(price_) + " " + to_string(qty_) + " left.");
	return returnMe;
}

void Clothing::dump(std::ostream& os) const
{
	//Outputs the product info in the database format
	Product::dump(os);
	os << size_ << "\n" << brand_ << endl;
}