#include <memory>
#include <string>

#include "mpool_fixed.h"

int main(int, char*[])
{
    auto str = std::make_unique<std::string>("hehe");

    return 0;
}
