#include <iostream>
#include <random>
#include <chrono>
#include <omp.h>
#include <array>
#include <memory>
#include <set>
#include <thread>

using namespace std;
using namespace chrono;

class searchArray{
public:
    static int binarySearchIterative(int array[],int size,int number){
        auto begin = high_resolution_clock::now();
        int left = 0;

        while (left <= size){
            int mid = left + (size - left)/2;

            if(array[mid] == number){
                auto endTime = high_resolution_clock::now();
                auto elapsed = duration_cast<milliseconds>(endTime-begin);
                cout <<"Tempo da busca binaria:" << elapsed.count() << " ms"<< endl;

                return array[mid];
            }
            else if(number > array[mid])
                left = mid + 1;
            else
                size = mid - 1;
        }
        auto endTime = high_resolution_clock::now();
        auto elapsed = duration_cast<milliseconds>(endTime-begin);
        cout <<"Tempo da busca binaria:" << elapsed.count() << " ms"<< endl;

        return NULL;

    }

    static int interpolationSearch(int array[], int size, int x) {

        int low = 0, high = size - 1;

        while (low <= high && x >= array[low] && x <= array[high]) {
            // Calculando a posição aproximada do valor desejado
            int pos = low + ((double)(high - low) / (array[high] - array[low])) * (x - array[low]);

            // Se o valor desejado foi encontrado
            if (array[pos] == x) {
                return pos;
            }
                // Se o valor desejado está à direita da posição atual
            else if (array[pos] < x) {
                low = pos + 1;
            }
                // Se o valor desejado está à esquerda da posição atual
            else {
                high = pos - 1;
            }
        }
        // Se o valor desejado não foi encontrado
        return NULL;
    }
};


static void printArray(int array[],int size){
    cout << endl;
    for(int i = 0; i < size;i++){
        cout << i << " " << array[i] << endl;
    }
}

class sortArray{
public:

    static void selectionSort(int arr[], int size) {
        auto begin = high_resolution_clock::now();
        for (int i = 0; i < size - 1; i++) {
            int min_index = i;
            for (int j = i + 1; j < size; j++) {
                if (arr[j] < arr[min_index]) {
                    min_index = j;
                }
            }
            std::swap(arr[i], arr[min_index]);
        }
        auto endTime = high_resolution_clock::now();
        auto elapsed = duration_cast<milliseconds>(endTime-begin);
        cout <<"Tempo de execução Selection Sort:" << elapsed.count() << " ms"<< endl;
    }

    static void quickSort(int arr[], int left, int right) {
        auto begin = high_resolution_clock::now();

        int i = left, j = right;
        int pivot = arr[(left + right) / 2];

        // Partition

        while (i <= j) {
            while (arr[i] < pivot) {
                i++;
            }
            while (arr[j] > pivot) {
                j--;
            }
            if (i <= j) {
                swap(arr[i], arr[j]);
                i++;
                j--;
            }
        }
        auto endTime = high_resolution_clock::now();
        auto elapsed = duration_cast<milliseconds>(endTime-begin);

        cout <<"Tempo de execução Quick Sort:" << elapsed.count() << " ms"<< endl;
    }

    static void heapSort(int array[], int size){
        auto begin = high_resolution_clock::now();
        for(int i = size/2 - 1;i >= 0;i--)
            heapify(array,size,i);

        for(int i = size - 1;i >= 0;i--){
            swap(array[0],array[i]);
            heapify(array,i,0);
        }
        auto endTime = high_resolution_clock::now();
        auto elapsed = duration_cast<milliseconds>(endTime-begin);
        cout <<"Execução Heap Sort:" << elapsed.count() << " ms"<< endl;
    }


private:

    static int Left(int i){
        return (2*i)+1;
    }

    static int Right(int i){
        return (2*i)+2;
    }

    static int Father(int i){
        return (i-1)/2;
    }

    static void heapify(int arr[], int size, int i) {
        int high = i;
        int l = 2*i + 1;
        int r = 2*i + 2;

        if (l < size && arr[l] > arr[high]) {
            high = l;
        }

        if (r < size && arr[r] > arr[high]) {
            high = r;
        }

        if (high != i) {
            swap(arr[i], arr[high]);
            heapify(arr, size, high);
        }
    }

};

class GenerateRandom{
public:

    static int generateRandomNumber(int start, int end){
        random_device rd;
        default_random_engine eng(rd());
        uniform_int_distribution<int> distr(start,end);

        return distr(eng);
    }

    //Executa com multithread
    static void fillPragma(int array[],int size){

        //Mude o valor de num_threads para definir quantas threads serão usadas.
        #pragma omp parallel for num_threads(8) schedule(dynamic)
        for(int i = 0; i < size;i++){
            array[i] = generateRandomNumber(20,2000000);
        }
    }

    static set<int> generateSetOnRange(int range){
        set<int>generatedSet;
        for(int i = 0;i < range;i++){
            generatedSet.insert(generateRandomNumber(20,2000000));
        }
        return generatedSet;
    }

};

static void acessVectorOnRange(int start, int end,vector<set<int>> vec){
    for(int i = start; i < end; i++){
        //int lineSize = GenerateRandom::generateRandomNumber(10000,1000000);
        cout << i << endl;
        vec[i] = GenerateRandom::generateSetOnRange(1000000);
    }
}

const int NUMBERTHREAD = 4;

int main(){
        //Por questões de segurança, o limite é 20

        int quantityLine = 40;//GenerateRandom::generateRandomNumber(5, 20);
        vector<set<int>>vectorSet(quantityLine);
        vector<set<int>>vectorSetTwo(quantityLine);
        thread threadsLine[8];

        int chunkThread = quantityLine/NUMBERTHREAD;

        int start;
        int end;
        cout << "Tamanho:" << quantityLine<<endl;

        auto begin = high_resolution_clock::now();
        cout << "Criação de threads iniciada: " << endl;
        for(int i = 0;i < NUMBERTHREAD;i++){
            start = chunkThread*i;

            if(i == NUMBERTHREAD-1)
                end = quantityLine;
            else
                end = (i+1)*chunkThread;
            threadsLine[i] = thread(acessVectorOnRange,start,end,vectorSet);
        }


        cout << "Criação das árvores "  << endl;
        for(int i = 0; i < NUMBERTHREAD;i++){
            cout << "Thread: " << threadsLine[i].get_id() << " completou execução" << endl;
            threadsLine[i].join();
        }

        auto endTime = high_resolution_clock::now();
        auto elapsed = duration_cast<seconds>(endTime-begin);

        //Execução sequencial
        auto beginSingle = high_resolution_clock::now();

        acessVectorOnRange(0,quantityLine,vectorSetTwo);
        auto endSingle = high_resolution_clock::now();

        auto elapesdSingle = duration_cast<seconds>(endSingle-beginSingle);
        cout << "Tempo de execução singleThread: " << elapesdSingle.count() << " s" << endl;
        cout << "Tempo de execução multiThread: " << elapsed.count() << " s" << endl;


        return 0;
}
