#include <iostream>
#include <Kokkos_Core.hpp>

int main(int argc, char** argv){

    Kokkos::initialize(argc,argv);
    {

        int rows = 7000;
        int cols = 7000;

        Kokkos::View<int **> A("one", rows, cols);

        Kokkos::View<int *> B("two", rows);
        Kokkos::View<int *> C("three", rows);

        //fill in view
        for(int i = 0; i<rows; i++){
            for(int j = 0; j <cols; j++){
                A(i,j) = (i * 2) + j;
            }
        }

        Kokkos::Timer t1;

        for( int i=0; i<rows; i++){
            int sum = 0;
            for(int j = 0; j<cols; j++){
                sum += A(i,j);
            }
            B(i) = sum;
        }

        //time serial
        float time1 = t1.seconds();


        Kokkos::Timer t2;


        Kokkos::parallel_for(
            "name",
            Kokkos::RangePolicy<>(0,rows),
            KOKKOS_LAMBDA(const int i){
                int sum = 0;

                for(int j = 0; j < cols; j++){
                    sum+= A(i,j);
                }
                C(i) = sum;
            }
        );

        Kokkos::fence();

        //time kokkos
        float time2 = t2.seconds();


        //Print out times
        std::cout << "Regular For Loop: " << time1 << std::endl;
        std::cout << "Parallel For Loop " << time2 << std::endl;

        float improvement = time1-time2;

        std::cout << "Parallel_for was " << improvement << " faster\n";
    }

    Kokkos::finalize();

}