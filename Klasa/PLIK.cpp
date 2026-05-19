#include "PLIK.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>
#include <iomanip>

void Pliki::Wyswietl() {
    uint16_t opcja;
    char path[50];
    char name[50];
    uint8_t opcja2;
    while (true) {
        std::cout << "What do we do today, boss?\n";
        std::cout <<"1.Wypisujemy pliki z danej ścieżki.\n";
        std::cout <<"2.Usuwamy pliki z danej ścieżki.\n";
        std::cout <<"3.Znajdujemy i usuwamy.\n";
        std::cout <<"4.Znajdujemy.\n";
        std::cout <<"5.Dodajemy folder.\n";
        std::cout <<"6.Kończymy na dziś.\n";
        std::cout<<"7.Dodajemy jakiś plik.\n";
        std::cin >> opcja;

        switch (opcja) {
            case 1:
                std::cout << "Proszę o podanie ścieżki: "; std::cin >> path;
                Wypisz(path);
                std::cout << "\n";
                break;
            case 2:
                std::cout << "Proszę o podanie ścieżki: "; std::cin >> path;
                std::cout << "Proszę o podanie nazwy pliku (folderu): "; std::cin >> name;
                std::cout << "Która opcja? (1 - sam plik, 2 - folder bez plików, 3 - folder z plikami)"; std::cin >> opcja2;
                Usun(path, opcja2 ,name);
                std::cout << "\n";
                break;
            case 3:
                std::cout << "Proszę o podanie nazwy: "; std::cin >> name;
                Find_delete(name);
                std::cout << "\n";
                break;
            case 4:
                std::cout << "Proszę o podanie nazwy: "; std::cin >> name;
                Open_file(name);
                std::cout << "\n";
                break;
            case 5:
                std::cout << "Proszę o podanie ścieżki: "; std::cin >> path;
                std::cout << "Proszę o podanie nazwy: "; std::cin >> name;
                Dodaj_folder(path, name);
                std::cout << "\n";
                break;
            case 6:
                std::cout << "Understood boss!";
                Clear_file_data();
                return;
            case 7:
                std::cout << "Proszę o podanie ścieżki: "; std::cin >> path;
                std::cout << "Proszę o podanie nazwy: "; std::cin >> name;
                Dodaj_plik(name, path);

            default:
                std::cout << "I don't undesrtand boss\n";
                break;
        }
    }
}

void Pliki::Wypisz(char p[50]) {
    std::filesystem::path pom = p;  //Rzutowanie z char na scieżkę
    if (std::filesystem::exists(pom) && std::filesystem::is_directory(pom)) { //sprawdzamy czy folder instenieje i czy jest to folder a nie plik
        for (const auto& j : std::filesystem::recursive_directory_iterator(pom)) {
            std::cout << j.path().filename().string() <<"\n"; //pętla wypisująca nazwę pliku
        }
    }
}

void Pliki::Dodaj_folder(char p[50], char n[50]) {
    std::filesystem::path pom = p;

    if (std::filesystem::exists(pom) && std::filesystem::is_directory(pom)) {
        std::filesystem::path nowaSciezka = pom; // Zaczynamy od ścieżki p
        nowaSciezka /= n; // "Doklejamy" do niej nazwę n (np. p/n)

        if (std::filesystem::exists(nowaSciezka)) {
            std::cout << "Istnieje!!!!\n";
        } else {
            std::filesystem::create_directory(nowaSciezka); // Tworzymy p/n
        }
    }

    else if (std::filesystem::is_regular_file(pom)) {
        std::filesystem::path PA = pom.parent_path();
        PA /= n;
        if (!std::filesystem::exists(PA)) {
            std::filesystem::create_directory(PA);
        }
    }
}

void Pliki::Usun(char p[50], uint8_t opcja, char n[50]) {
    std::filesystem::path pom = p;
    switch (opcja) {
        case 1: //Sam plik
            if (std::filesystem::exists(pom)) {
                std::filesystem::remove(n);
            }
        case 2: //pusty folder
            if (std::filesystem::exists(pom) && std::filesystem::is_directory(pom)) {
                std::filesystem::remove(n);
            }
        case 3: //folder z zawartością
            if (std::filesystem::exists(pom) && std::filesystem::is_directory(pom)) {
                std::uintmax_t i = std::filesystem::remove_all(n);
                std::cout << "Usunieto folder i " << i << " elementow ze srodka.\n";
            }
    }
}

void Pliki::Find_delete(char n[50]) { //Uwaga, jest to przystosowane wyłącznie pod Linuxa i pomija pliki lub foldery do których nie ma uprawnień
    std::filesystem::path root = "/";
    auto iterator = std::filesystem::recursive_directory_iterator(root, std::filesystem::directory_options::skip_permission_denied);
    for (const auto& entry : iterator) {
        if (entry.path().filename() == n) {
            std::cout << "I found it, now it's time to delete it.\n";
            std::filesystem::remove(n);
        }
    }
}

std::filesystem::path Pliki::Find_path(char n[50]) {
    std::filesystem::path root = "/";
    try {
        auto iterator = std::filesystem::recursive_directory_iterator(root, std::filesystem::directory_options::skip_permission_denied);
        for (const auto& entry : iterator) {
            std::error_code ec;
            if (entry.exists(ec) && entry.path().filename() == n) {
                //std::cout << "I found it! Here's the path: " << entry.path().string() << "\n";
                return entry.path();
            }
        }
    }
    catch (...) {
    }

    return {};
}

void Pliki::Dodaj_plik(char n[50], char p[50]) {
    std::filesystem::path pom = p;
    std::filesystem::create_directories(pom);
    pom /= n;
    std::ofstream plik(pom);
    if (plik.is_open()) std::cout << "Congratulations!!!\n";
    plik.close();
}

void Pliki::Open_file(char n[50]) {
    std::filesystem::path pom = Find_path(n);
    if (!pom.empty()) {
        // 1. Otwarcie pliku w trybie binarnym na końcu pliku (ios::ate)
        std::ifstream file(pom, std::ios::in | std::ios::binary | std::ios::ate);

        if (!file) {
            std::cerr << "Blad: Nie mozna otworzyc pliku binarnego: " << pom.string() << std::endl;
            return;
        }

        // 2. Pobranie rozmiaru i powrót na początek
        std::streamsize fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // 3. Czyszczenie starych danych i alokacja miejsca w zmiennej klasowej
        this->Plik_binarny.clear();
        this->Plik_binarny.resize(fileSize);

        // 4. Wczytanie pliku bezpośrednio do zmiennej Plik_binarny
        // Rzutujemy wskaźnik danych na char*, ponieważ ifstream::read wymaga takiego formatu
        if (file.read(reinterpret_cast<char*>(this->Plik_binarny.data()), fileSize)) {
            std::cout << "Sukces! Plik zapisany w pamięci RAM klasy.\n";
            std::cout << "Rozmiar w zmiennej Plik_binarny: " << this->Plik_binarny.size() << " bajtow.\n";
        } else {
            std::cerr << "Blad: Nie udalo sie odczytac zawartosci pliku." << std::endl;
            this->Plik_binarny.clear(); // W razie błędu czyścimy wektor
        }

        file.close();
    } else {
        std::cerr << "Blad: Sciezka pliku jest pusta." << std::endl;
    }
}

void Pliki::Clear_file_data() {
    this->Plik_binarny.clear();         // 1. Usuwa wszystkie elementy (rozmiar wynosi 0)
    this->Plik_binarny.shrink_to_fit(); // 2. Zwalnia zarezerwowaną pamięć RAM do systemu
}
