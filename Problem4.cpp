#include <iostream>
#include <Kokkos_Core.hpp>



int main(int argc, char** argv){

    int n = 6;

    if(argc > 1){
        n = std::atoi(argv[1]);
    }

    Kokkos::initialize(argc,argv);
    {
        Kokkos::View<int****> A("problem4 View", 5, 7, 12, n);

        std::cout << A.label() << std::endl;
        std::cout << A.extent(0) << std::endl;
        std::cout << A.extent(1) << std::endl;
        std::cout << A.extent(2) << std::endl;
        std::cout << A.extent(3) << std::endl;

    }
    Kokkos::finalize();
    return 0;
}