#include <iostream>
#include <Kokkos_Core.hpp>


//extra credit dims check
bool check(Kokkos::View<int **> A, Kokkos::View<int *> B){
    if (A.extent(1) != B.extent(0)){
        std::cout << "Size mismatch\n";
        return false;
    }
    else{
        std::cout << "Sizes okay\n";
        return true;
    }

}

int main(int argc, char** argv){

    Kokkos::initialize(argc,argv);
    {
        Kokkos::View<int **> A("one", 3,3);
        
        //populate views

        A(0,0) =130;
        A(0,1) =147;
        A(0,2) =115;

        A(1,0) =224;
        A(1,1) =158;
        A(1,2) =187;

        A(2,0) =54;
        A(2,1) =158;
        A(2,2) =120;

        Kokkos::View<int *> B("two", 3);

        B(0) = 221;
        B(1) = 12;
        B(2) = 157;

        //call extracredit check function
        if(!check(A, B)){
            return 0;
        }

        Kokkos::View<int **> C("three",3,3);


        //perform sum
        Kokkos::parallel_for(
            "AddFunction",
            Kokkos::MDRangePolicy<Kokkos::Rank<2>>({0,0}, {3,3}),
            KOKKOS_LAMBDA(const int i, const int j){
                C(i,j) = A(i,j) + B(j);
            }
        );

        for(int i = 0; i<3; i++){
            for(int j = 0; j <3; j++){
                std::cout << C(i,j) << " ";
            }
            std::cout << std::endl;
        }


    }

    Kokkos::finalize();
    return 0;

}