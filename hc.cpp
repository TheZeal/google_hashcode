#include <iostream>
#include <fstream>
#include <assert.h>

using namespace std;

int w;
int h;


//  A car in the simulation

struct car
{

};

int main( int argc, char **argv )
{
    assert( argc==2 );

    ifstream pbfile;
    pbfile.open( argv[1] );
    int n;
    pbfile >> n;
    std::cout << n << std::endl;
    pbfile.close();
    return 0;
}
