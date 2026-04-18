#include <iostream>
#include <Kokkos_Core.hpp>

int main(int argc, char** argv){

    Kokkos::initialize(argc,argv);
    {

    Kokkos::View<int*> A("problem2Label", 8);

    std::cout<<"\n ------------------------ \n" <<  A.label() << std::endl;
    }
    Kokkos::finalize();
    return 0;
}