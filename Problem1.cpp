#include <mpi.h>
#include <iostream>
#include <ctime> //randomness

using namespace std;

//ripped from previoous cockett assignment
void merge(int array[], int left, int mid, int right);
void mergeSort(int array[], int left, int right);
int* mergeSorted(int one[], int sizeOne, int two[], int sizeTwo);

int main(int argc, char** argv){
    
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    //So i'm assuming I can make a default array size (I get to set N)

    int N = 32;

    //update N if provided
    if(argc > 1){
        N = atoi(argv[1]);
    }

    int localSize = N / size;
    int* globalArray = nullptr;
    int* localArray = new int[localSize];

    //generate the array in rank zero which will then be evenly distributed to each process
    if(rank == 0){

        globalArray = new int[N];
        srand(time(0));

        //generate values for array
        for (int i = 0; i < N; i++){
            globalArray[i] = rand() % 256 + 1;

        }

        //cout original array
        cout << "Original Array: \n";
        for (int i = 0; i < N; i++){
            cout << globalArray[i] << ", ";
        }
        cout << endl;
    }
    //using mpi scatter instead of creating my own implementation
    MPI_Scatter(globalArray, localSize, MPI_INT, localArray, localSize, MPI_INT, 0 , MPI_COMM_WORLD);

    //print each rank's unsorted chunk
    for( int i = 0; i < size; i++){
        MPI_Barrier(MPI_COMM_WORLD);

        if (rank == i){

            cout << "Rank: " << rank << " | ";
            for (int x = 0; x < localSize; x++){
                cout << localArray[x] << ", ";
            }
        }

        cout << endl;
    }

    int* currentArray = localArray;
    int currSize = localSize;

    //sort lock chunks and print
    mergeSort(localArray, 0, localSize-1);

    for( int i = 0; i < size; i++){
        MPI_Barrier(MPI_COMM_WORLD);

        if (rank == i){

            cout << "Rank Updated: " << rank << " | ";
            for (int x = 0; x < currSize; x++){
                cout << localArray[x] << ", ";
            }
        }

        cout << endl;
    }

    if(rank == 0){
        cout << "----------------------------------------\n";
    }

    //merge and sort processes 
    for( int i = 1; i < size; i*=2){
        if(rank % (2*i) == 0){
            int pair = rank + i;

            if (pair < size){
                int recv;

                MPI_Recv(&recv, 1, MPI_INT, pair, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                int* received = new int[recv];

                MPI_Recv(received, recv, MPI_INT, pair, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                int* merged = mergeSorted(currentArray, currSize, received, recv);


                if(currentArray != localArray){
                    delete[] currentArray;
                }
                delete[] received;

                currentArray = merged;

                currSize += recv;

            }
        }
        else{
            int pair = rank - i;

            MPI_Send(&currSize, 1, MPI_INT, pair, 0, MPI_COMM_WORLD);
            MPI_Send(currentArray, currSize, MPI_INT, pair, 0, MPI_COMM_WORLD);

            if(currentArray != localArray){
                delete[] currentArray;
            }

            break;
        }
    }

    //Final array is now in root. Print the elements

    if(rank ==0){
        cout << "----------------------------\nFinal: ";
        for (int x = 0; x < currSize; x++){
                cout << currentArray[x] << ", ";
            }

        delete[] currentArray;
        delete[] globalArray;
    }

    MPI_Finalize();
}

//ripped from previous cockett assignment and slightly modified.
void merge(int array[], int left, int mid, int right){
    int leftSize = mid - left +1;
    int rightSize = right - mid;

    int* leftArray = new int[leftSize];
    int* rightArray = new int[rightSize];

    for(int i = 0; i < leftSize; i++){
        leftArray[i] = array[left+i];
    }

    for(int i = 0; i < rightSize; i++){
        rightArray[i] = array[mid+1+i];
    }

    int i = 0, x = 0, z = left;

    while(i < leftSize && x < rightSize){

        if(leftArray[i] <= rightArray[x]){

            array[z++] = leftArray[i++];
        }
        else{
            array[z++] = rightArray[x++];
        }
    }

    while(i < leftSize){
        array[z++] = leftArray[i++];
    }
    while(x < rightSize){
        array[z++] = rightArray[x++];
    }

    delete[] leftArray;
    delete[] rightArray;

}

//also modified from previous assignment. recursive because
void mergeSort(int array[], int left, int right){
    if(left >=right){
        return;
    }

    int mid = (left + right) /2;

    mergeSort(array, left, mid);
    mergeSort(array, mid+1, right);
    merge(array, left, mid, right);
}
//yeah also crockett
int* mergeSorted(int one[], int sizeOne, int two[], int sizeTwo){
    int newSize = sizeOne + sizeTwo;

    int* newArray = new int[newSize];

    int i = 0;
    int x =0;
    int z = 0;

    while(i < sizeOne && x < sizeTwo){
        if (one[i] <= two[x]){
            newArray[z++] = one[i++];

        }
        else newArray[z++] = two[x++];
    }

    while(i < sizeOne){
        newArray[z++] = one[i++];

    }

    while(x < sizeTwo){
        newArray[z++] = two[x++];
    }

    return newArray;
}