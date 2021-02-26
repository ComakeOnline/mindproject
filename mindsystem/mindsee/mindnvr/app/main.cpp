#include <iostream>
#include <cstring>

#include "manager.hpp"

using namespace std;

int main(int argc, char const *argv[])
{
    auto pM = Manager::GetInstance();
    pM->Run();
    return 0;
}
