#include <iostream>
# define GREEN "\033[1;32m"
# define RESET "\033[0m"

int main (void) {
    std::cout << GREEN << "Webserv ended successfuly\n" << RESET;
}