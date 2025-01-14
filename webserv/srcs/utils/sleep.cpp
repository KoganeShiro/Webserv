#include <iostream>
#include <unistd.h>

int main ()
{
    std::cout << "Sleeping 20 seconds" << std::endl;
    for (int i = 0; i < 20; i++) {
        std::cout << i << std::endl;
        sleep(1);
        std::cout << "Slept for " << i << " seconds" << std::endl;
    }
    return (0);
}