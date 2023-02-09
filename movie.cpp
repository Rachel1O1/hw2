#include "movie.h"
#include "util.h"

using namespace std;

Movie::Movie(const std::string name, double price, int qty, const std::string genre, const std::string rating) :
  Product("movie", name, price, qty)
{
  genre_ = genre;
	rating_ = rating;
}

std::set<std::string> Movie::keywords() const
{
	//Returns the appropriate keywords that this product should be associated with
	std::set<std::string> returnMe = parseStringToWords(name_);
	returnMe.insert(convToLower(genre_));
	return returnMe;
}

/*bool Movie::isMatch(std::vector<std::string>& searchTerms) const
{
	//Allows for a more detailed search beyond simple keywords
}*/

std::string Movie::displayString() const
{
	//Returns a string to display the product info for hits of the search
	string returnMe = (name_ + "\nGenre: " + genre_ + " Rating: " + rating_ + "\n" + to_string(price_) + " " + to_string(qty_) + " left.");
	return returnMe;
}

void Movie::dump(std::ostream& os) const
{
	//Outputs the product info in the database format
	Product::dump(os);
	os << genre_ << "\n" << rating_ << endl;
}