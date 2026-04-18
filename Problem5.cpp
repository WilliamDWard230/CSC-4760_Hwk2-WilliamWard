#include <iostream>
#include <Kokkos_Core.hpp>



int main(int argc, char** argv){

    int n = 6;

    if(argc > 1){
        n = std::atoi(argv[1]);
    }

    Kokkos::initialize(argc,argv);
    {
        Kokkos::View<int*> A("Problem5", n);

        //practice with parallel for
        Kokkos::parallel_for(
            "Problem5_PF", n,
            KOKKOS_LAMBDA(const int i){
                A(i) = (i+1) * 2;
            }
        );

        //final max ... local max is declared inline in lambda
        int max;

        Kokkos::parallel_reduce(
            "Problem5_PR", n,
            KOKKOS_LAMBDA(const int i, int& localMax){
                if(A(i) > localMax){
                    localMax = A(i);
                }
            }, 
            Kokkos::Max<int>(max)
        );
        std::cout << "View is populated by {A(i) = (i+1) * 2} \n\n";
        std::cout << "Max = " << max << std::endl;


    }

    Kokkos::finalize();
    return 0;

}