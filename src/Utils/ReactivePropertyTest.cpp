#include "ReactiveProperty.h"

#include <iostream>

using namespace std;

int main()
{
    ReactiveInt rInt(0);
    rInt.Subscribe([](int f){
        cout << "value: " << f << endl;
    });

    rInt.Set(5);
    rInt.Set(50);
    return 0;
}
