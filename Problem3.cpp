#include <iostream>
#include <Kokkos_Core.hpp>


int main(int argc, char** argv){

    int m = 6;
    int n = 5;

    if(argc > 1){
        m = std::atoi(argv[1]);
    }

    if(argc > 2){
        n = std::atoi(argv[2]);
    }

    Kokkos::initialize(argc,argv);
    {
        // sorry i hate (n x m)
        Kokkos::View<int**> A("Problem3", m, n);

        //was going to do it with nested for loops,, but will do it with a parallel for.
        Kokkos:parallel_for(
            "PF-1000",
            Kokkos::MDRangePolicy<Kokkos::Rank<2>>({0,0}, {m,n}),
            KOKKOS_LAMBDA(const int i, const int j){
                A(i,j) = ((i+1) * 1000) + j;
            }
        );

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                std::cout << A(i, j) << " ";
            }

            std::cout << std::endl;
        }
    }

    Kokkos::finalize();
    return 0;
}