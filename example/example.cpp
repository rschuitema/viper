#include <iostream>
#include "viperboard.h"

using namespace std;
using namespace Viper;

int main (void)
{
    cout << "Example program for the viperboard of Nano River Techonologies" << endl;
    cout << "--------------------------------------------------------------" << endl;
    
    Viperboard* pViper = new Viperboard();
    
    pViper->Open();
    
    uint16_t revision = pViper->Revision();
    cout << "Viperboard revision: " << revision << endl;
    
    pViper->Close();
    
    
    delete pViper;
    return 0;
}

