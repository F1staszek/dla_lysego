#pragma once
#include <filesystem>
#include <vector>

class Pliki {
    std::vector<std::uint8_t> Plik_binarny;
    void Wypisz(char p[50]);
    void Dodaj_folder(char p[50], char n[50]);
    void Usun(char p[50], uint8_t opcja, char n[50]);
    void Find_delete(char n[50]);
    std::filesystem::path Find_path(char n[50]);
    void Dodaj_plik(char n[50], char p[50]);
    void Open_file(char n[50]);
    void Clear_file_data();

public:
    Pliki() {};
    void Wyswietl();
};
