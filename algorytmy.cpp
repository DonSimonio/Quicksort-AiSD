/*
    Program ma za zadanie testowanie szybkości działania algorytów Quicksort w zależności od wybieranego pivotu i danych wejściowych.
    Obrazuje nam pesymistyczną i optypistyczną złożoność obliczeniową każdego z sposóbu zaimplementowanego poniżej.
    Prezentuje wady, zalety oraz ograniczenia.
*/

#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <stack>
#include <chrono>
using namespace std::chrono;
using namespace std;
int tab[200000];


/*
    Funckja wykonująca algorytm Quicksort.
    Wybiera środkowy element tablicy jako pivot.
*/
void QS_srodek(int tab[], int low, int high)
{
    int lewy = low;
    int prawy = high;
    int pivot = tab[(low + high) / 2];
    do
    {
        while (tab[lewy] < pivot)
            lewy++;

        while (tab[prawy] > pivot)
            prawy--;

        if (lewy <= prawy)
        {
            swap(tab[lewy], tab[prawy]);

            lewy++;
            prawy--;
        }
    } while (lewy <= prawy);

    if (low < prawy) QS_srodek(tab, low, prawy);

    if (high > lewy) QS_srodek(tab, lewy, high);

}


/*
    Funkcja partycjonowania tablicy dla funkcji QS_prawy.
*/
int partition(int tab[], int low, int high)
{
    int pivot = tab[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++)
    {
        if (tab[j] <= pivot)
        {
            i++;
            swap(tab[i], tab[j]);
        }
    }
    swap(tab[i + 1], tab[high]);
    return (i + 1);
}


/*
    Funkcja wykonująca algorytm Quicksort.
    Wybiera prawy element tablicy jako pivot.
*/
void QS_prawy(int tab[], int low, int high)
{
    if (low < high)
    {
        int q = partition(tab, low, high);
        QS_prawy(tab, low, q - 1);
        QS_prawy(tab, q + 1, high);
    }
}


/*
    Funkcja partycjonowania tablicy dla funkcji QS_losowy.
*/
int partition2(int tab[], int low, int high)
{
    int pivot = tab[low];
    int lewy = low - 1, prawy = high + 1;

    while (true) {
        do {
            lewy++;
        } while (tab[lewy] < pivot);

        do {
            prawy--;
        } while (tab[prawy] > pivot);

        if (lewy >= prawy)
            return prawy;

        swap(tab[lewy], tab[prawy]);
    }
}


/*
    Funkcja wybierania losowego pivota.
*/
int partition_r(int tab[], int low, int high)
{
    srand(time(NULL));
    int random = low + rand() % (high - low);
    swap(tab[random], tab[low]);

    return partition2(tab, low, high);
}


/*
    Funkcja wykonująca algorytm Quicksort.
    Wybiera losowy element tablicy jako pivot.
*/
void QS_losowy(int tab[], int low, int high)
{
    if (low < high) {
        int q = partition_r(tab, low, high);
        QS_losowy(tab, low, q);
        QS_losowy(tab, q + 1, high);
    }
}


/*
    Funkcja wypisująca elementy tablicy
*/
void wypisz(int tab[], int dl)
{
    for (int i = 0; i < dl; i++)
    {
        cout << tab[i] << " ";
    }
    cout << endl;
}


/*
    Wczytywanie danych z pliku "QS_tablica.txt"
*/
void czytaj()
{
    fstream plik;
    plik.open("QS_tablica.txt");
    if (plik.good() == false)
    {
        cout << "Blad!!!";
        return;
    }

    int liczba, i = 0;
    while (plik >> liczba)
    {
        tab[i++] = liczba;
    }

    plik.close();
    return;
}


/*
    Funkcja partycjonowania tablicy dla iteracyjnej werjsi algorytmu Quicksort.
*/
int partition5(int arr[], int start, int end)
{
    int pivot = arr[end];
    int pIndex = start;

    for (int i = start; i < end; i++)
    {
        if (arr[i] <= pivot)
        {
            swap(arr[i], arr[pIndex]);
            pIndex++;
        }
    }

    swap(arr[pIndex], arr[end]);

    return pIndex;
}


/*
    Funkcja wykonująca iteracyjnie algorytm Quicksort z wykorzystaniem stosu.
*/
void iterativeQuicksort(int arr[], int n)
{
    stack<pair<int, int>> s;

    int start = 0;
    int end = n - 1;

    s.push(make_pair(start, end));

    while (!s.empty())
    {
        start = s.top().first, end = s.top().second;
        s.pop();

        int pivot = partition5(arr, start, end);

        if (pivot - 1 > start) {
            s.push(make_pair(start, pivot - 1));
        }

        if (pivot + 1 < end) {
            s.push(make_pair(pivot + 1, end));
        }
    }
}


int main()
{

    fstream wyj;
    wyj.open("wyjscie.txt");
    czytaj();
    //wypisz(tab, 200000);

    // Wykonywanie algorytmów i pomiar czasów
    for (int i = 0; i <= 100000; i+=5000)
    {
        auto start = high_resolution_clock::now();
        QS_srodek(tab, 100000-i, 100000+i);
        auto stop = high_resolution_clock::now();
        auto czas = duration_cast<milliseconds>(stop - start);
        wyj << i*2 << "-srodek-" << czas.count()<< endl;
    }
    czytaj();
    for (int i = 0; i <= 100000; i += 5000)
    {
        auto start = high_resolution_clock::now();
        QS_losowy(tab, 100000 - i, 100000 + i);
        auto stop = high_resolution_clock::now();
        auto czas = duration_cast<milliseconds>(stop - start);
        wyj << i * 2 << "-losowy-" << czas.count() << endl;
    }
    czytaj();
    for (int i = 0; i <= 200000; i += 10000) 
    {
        auto start = high_resolution_clock::now();
        iterativeQuicksort(tab, i);
        auto stop = high_resolution_clock::now();
        auto czas = duration_cast<milliseconds>(stop - start);
        wyj << i << "-prawy-" << czas.count() << endl;
    }

    wyj.close();
    return 0;
}
