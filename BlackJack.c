/*
 * Autor: Szymon Stosik
 * Data Utworzenia: 2024/01/09
 * Ostatnia modyfikacja: 2024/01/24
 * Ten plik zawiera implementację gry w Blackjacka.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

#define ROZMIAR_TALI 52

#define SPADE 0 //♠ 
#define HEART 1 // ♥
#define DIAMOND 2 //♦
#define CLUB 3 //♣

struct card {
    int kolor;
    int wartosc;
};

struct card talia[ROZMIAR_TALI];


void print_title() {
    printf("BlackJack\n");
}

//Pokazuje wszystkie karty w talii
//Tylko do debugu, nie jest używane w grze.
void list_cards() {
    for (int i = 0; i < ROZMIAR_TALI; i++) {
        switch (talia[i].kolor) {
            case SPADE:
                printf("♠%d, ", talia[i].wartosc);
                break;
            case HEART:
                printf("♥%d, ", talia[i].wartosc);
                break;
            case DIAMOND:
                printf("♦%d, ", talia[i].wartosc);
                break;
            case CLUB:
                printf("♣%d, ", talia[i].wartosc);
                break;
        }
    }
    printf("\n\n\n");
}

//Tasowanie Talii
void shuffle() {
    int how_many = rand() % 30; // Losowo decyduje, ile razy tasować
    for (int i = 0; i < how_many * 100; i++) { // Tasuje tyle razy
        int a = rand() % ROZMIAR_TALI; // Wybiera dwie losowe karty
        int b = rand() % ROZMIAR_TALI;
        struct card temp = talia[a]; // Zamienia miejscami dwie karty
        talia[a] = talia[b];
        talia[b] = temp;
    }
}

//=======================================================================
void zasady() {
    printf("\nZasady gry:\n Naciśnij enter aby pokazać następny punkt\n\n");
    getchar();
    printf("1. Gracz i krupier dostają po dwie karty.\n");
    printf("   Jedna karta krupiera jest zakryta.\n");
    getchar();
    printf("2. Gracz ma różne możliwości w czasie swojej tury:\n");
    printf("   - Dobranie karty\n");
    getchar();
    printf("   - Pas\n");
    getchar();
    printf("   - Podwojenie stawki\n");
    printf("        Gracz może podwoić swoją stawkę tylko na początku rundy\n");
    printf("        - Gracz dostaje tylko jedną nową kartę\n");
    printf("          Podwaja swoją stawkę\n");
    getchar();
    printf("  - Ubezpieczenie\n");
    printf("        - Jeśli odkryta karta krupiera jest asem\n");
    printf("          gracz może się ubezpieczyć\n");
    printf("        - Gracz stawia połowę swojej stawki\n");
    printf("        - Jeśli krupier ma blackjacka\n");
    printf("          gracz dostaje 2x ubezpieczenia\n");
    printf("        - Jeśli krupier nie ma blackjacka\n");
    printf("          gracz traci ubezpieczenie\n");
    getchar();
    printf("3. Gracz przegrywa jeśli:\n");
    printf("   - Przekroczy 21 punktów\n");
    printf("   - Krupier ma więcej punktów\n");
    getchar();
    printf("4. Gracz wygrywa jeśli:\n");
    printf("   - Ma 21 punktów\n");
    printf("        Jeśli startowa ręka gracza ma 21 punktów\n");
    printf("        gracz wygrywa 3x stawki niezależnie od ręki krupiera\n");
    printf("   - Krupier przekroczy 21 punktów\n");
    printf("   - Gracz ma więcej punktów niż krupier\n");
    getchar();
    printf("5. Remis następuje jeśli:\n");
    printf("   - Gracz i krupier mają tyle samo punktów\n");
    getchar();
    printf("6. Każda karta ma swoją wartość:\n");
    printf("   - As - 1 lub 11 w zależności co jest lepsze w danej sytuacji\n");
    printf("   - Karty od 2 do 10 - ich wartość\n");
    printf("   - Walet, Dama, Król - 10 punktów\n");
    getchar();
    printf("7. Krupier dobiera karty do 17 punktów\n");
}

// Funkcja do wyświetlania karty
void show_card(struct card karta) {
    char wartosc[3]; // Tablica znaków do przechowywania wartości karty
    switch (karta.wartosc) { // Przypisanie wartości karty
        case 1:
            strcpy(wartosc, "A"); // As
            break;
        case 11:
            strcpy(wartosc, "J"); // Walet
            break;
        case 12:
            strcpy(wartosc, "Q"); // Dama
            break;
        case 13:
            strcpy(wartosc, "K"); // Król
            break;
        default:
            sprintf(wartosc, "%d", karta.wartosc); // Dla kart od 2 do 10
            break;
    }

    switch (karta.kolor) { // Wyświetlanie koloru karty
        case SPADE:
            printf("♠%s, ", wartosc); // Pik
            break;
        case HEART:
            printf("♥%s, ", wartosc); // Kier
            break;
        case DIAMOND:
            printf("♦%s, ", wartosc); // Karo
            break;
        case CLUB:
            printf("♣%s, ", wartosc); // Trefl
            break;
    }
}

// Funkcja do obliczania sumy ręki
int suma_reki(struct card reka[]) {
    int suma = 0; // Inicjalizuje sumę
    int as = 0; // Licznik asów
    for (int i = 0; i < 6; i++) { // Przechodzi przez każdą kartę w ręce
        if (reka[i].wartosc == 0) { // Jeśli wartość karty wynosi 0, przerywa pętlę
            break;
        } else if (reka[i].wartosc > 10) { // Jeśli wartość karty jest większa niż 10, dodaje 10 do sumy
            suma += 10;
        } else if (reka[i].wartosc == 1) { // Jeśli karta to as, dodaje 11 do sumy i zwiększa licznik asów
            suma += 11;
            as++;
        } else { // W przeciwnym razie dodaje wartość karty do sumy
            suma += reka[i].wartosc;
        }

        while (suma > 21 &&
               as > 0) { // Jeśli suma jest większa niż 21 i jest as, odejmuje 10 od sumy i zmniejsza licznik asów
            suma -= 10;
            as--;
        }
    }
    return suma; // Zwraca sumę
}

//=======================================================================
int gra(int stawka) {
    int obecnaKartaWTali = 0; // Aktualna karta w talii
    shuffle(); // Tasowanie talii
    struct card rekaGracza[6] = {{0, 0}}; // Inicjalizacja ręki gracza
    struct card rekaKrupiera[6] = {{0, 0}}; // Inicjalizacja ręki krupiera
    int wybor; // Zmienna do przechowywania wyboru gracza
    int misc_zeotny = 0; // Zmienna pomocnicza

// Rozdanie kart
    obecnaKartaWTali++; // Pomijanie pierwszej karty w talii
    for (int i = 0; i < 2; i++) { // Rozdanie dwóch kart dla gracza i krupiera
        rekaGracza[i] = talia[obecnaKartaWTali]; // Przypisanie karty do ręki gracza
        obecnaKartaWTali++; // Przejście do następnej karty w talii
        rekaKrupiera[i] = talia[obecnaKartaWTali]; // Przypisanie karty do ręki krupiera
        obecnaKartaWTali++; // Przejście do następnej karty w talii
    }

    int obecnaKartaGracza = 2;
    int obecnaKartaKrupiera = 2;

    printf("Karty krupiera:\n");
    show_card(rekaKrupiera[0]); // Wyświetlanie pierwszej karty krupiera
    printf("?\n"); // Druga karta krupiera jest ukryta
    printf("Suma: %d + ?\n", rekaKrupiera[0].wartosc); // Wyświetlanie sumy punktów krupiera
    printf("--------------------\n");

    printf("Twoje karty:\n");
    show_card(rekaGracza[0]); // Wyświetlanie pierwszej karty gracza
    show_card(rekaGracza[1]); // Wyświetlanie drugiej karty gracza
    printf("\n");
    printf("Suma: %d\n", suma_reki(rekaGracza)); // Wyświetlanie sumy punktów gracza
    sleep(1); // Krótkie opóźnienie dla lepszej czytelności
    if (suma_reki(rekaGracza) == 21) { // Jeśli gracz ma blackjacka
        printf("BlackJack!\n");
        sleep(1);
        return stawka * 3; // Gracz wygrywa 3x stawkę
    }

// Ubezpieczenie
    if (rekaKrupiera[0].wartosc == 1) { // Jeśli odkryta karta krupiera to as
        wybor = 0;
        do {
            printf("Krupier ma asa.\n");
            printf("Czy chcesz się ubezpieczyć?\n");
            printf("1 - Tak | 2 - Nie\n");
            if (scanf("%d", &wybor) != 1) {
                wybor = 0;
                while (getchar() != '\n');
            }
        } while (wybor != 1 && wybor != 2);

        if (wybor == 1) { // Jeśli gracz chce się ubezpieczyć
            printf("Ubezpieczenie\n");
            printf("Ubezpieczono: %d\n", stawka / 2); // Wyświetlanie kwoty ubezpieczenia
            sleep(1);
            printf("Karty krupiera:\n");
            show_card(rekaKrupiera[0]); // Wyświetlanie pierwszej karty krupiera
            show_card(rekaKrupiera[1]); // Wyświetlanie drugiej karty krupiera
            printf("\n");
            printf("Suma: %d\n", suma_reki(rekaKrupiera)); // Wyświetlanie sumy punktów krupiera
            sleep(1);
            if (suma_reki(rekaKrupiera) == 21) { // Jeśli krupier ma blackjacka
                printf("Krupier ma blackjacka.\n");
                sleep(1);
                return stawka; // Gracz odzyskuje swoją stawkę
            } else {
                printf("Krupier nie ma blackjacka.\n");
                misc_zeotny = -stawka / 2; // Gracz traci połowę stawki
                sleep(1);
            }
        }
    }

    printf("=======================================\n");

// Tura gracza
    printf("Twoja tura:\n");

// Pętla wykonuje się dopóki suma punktów gracza jest mniejsza od 21
    while (suma_reki(rekaGracza) < 21) {
        // Wyświetlanie kart gracza
        for (int i = 0; i < obecnaKartaGracza; i++) {
            show_card(rekaGracza[i]);
        }
        printf("\n");
        printf("Suma: %d\n", suma_reki(rekaGracza)); // Wyświetlanie sumy punktów gracza

        wybor = 0;
        // Jeśli gracz ma dwie karty, ma dodatkową opcję podwojenia stawki
        if (obecnaKartaGracza == 2) {
            do {
                printf("1 - Dobierz kartę | 2 - Pas | 3 - Podwojenie stawki\n");
                if (scanf("%d", &wybor) != 1) {
                    wybor = 0;
                    while (getchar() != '\n');
                }
            } while (wybor != 1 && wybor != 2 && wybor != 3);
        } else {
            do {
                printf("1 - Dobierz kartę | 2 - Pas\n");
                if (scanf("%d", &wybor) != 1) {
                    wybor = 0;
                    while (getchar() != '\n');
                }
            } while (wybor != 1 && wybor != 2);
        }
        bool dbdown = false;

        // Obsługa wyboru gracza
        if (wybor == 1) {
            printf("Dobieranie karty.\n");
            rekaGracza[obecnaKartaGracza] = talia[obecnaKartaWTali]; // Dobranie karty z talii
            obecnaKartaWTali++; // Przejście do następnej karty w talii
            obecnaKartaGracza++; // Zwiększenie liczby kart gracza
        } else if (wybor == 2) {
            printf("Pas.\n");
            break; // Jeśli gracz pasuje, kończy turę
        } else if (wybor == 3) {
            printf("Podwojenie stawki.\n");
            rekaGracza[obecnaKartaGracza] = talia[obecnaKartaWTali]; // Dobranie karty z talii
            obecnaKartaWTali++; // Przejście do następnej karty w talii
            obecnaKartaGracza++; // Zwiększenie liczby kart gracza
            stawka *= 2; // Podwojenie stawki
            dbdown = true; // Zaznaczenie, że gracz podwoił stawkę
        }

        // Sprawdzanie sumy punktów gracza po dobieraniu karty
        if (suma_reki(rekaGracza) == 21) {
            // Wyświetlanie kart gracza
            for (int i = 0; i < obecnaKartaGracza; i++) {
                show_card(rekaGracza[i]);
            }
            printf("\n");
            printf("Suma: %d\n", suma_reki(rekaGracza));
            printf("BlackJack!\n");
            sleep(2);
            break; // Jeśli gracz ma 21 punktów, kończy turę
        } else if (suma_reki(rekaGracza) > 21) {
            // Wyświetlanie kart gracza
            for (int i = 0; i < obecnaKartaGracza; i++) {
                show_card(rekaGracza[i]);
            }
            printf("\n");
            printf("Suma: %d\n", suma_reki(rekaGracza));
            printf("Przekroczyłeś 21 punktów.\n");
            sleep(2);
            return 0 + misc_zeotny; // Jeśli gracz przekroczył 21 punktów, przegrywa
        } else if (dbdown) {
            // Wyświetlanie kart gracza
            for (int i = 0; i < obecnaKartaGracza; i++) {
                show_card(rekaGracza[i]);
            }
            printf("Suma: %d\n", suma_reki(rekaGracza));
            sleep(2);
            break; // Jeśli gracz podwoił stawkę, kończy turę
        }
    }
// Tura krupiera
    printf("Tura krupiera:\n");
    printf("Karty krupiera:\n");

// Wyświetlanie kart krupiera
    for (int i = 0; i < obecnaKartaKrupiera; i++) {
        show_card(rekaKrupiera[i]);
    }

    printf("\n");
    printf("Suma: %d\n", suma_reki(rekaKrupiera)); // Wyświetlanie sumy punktów krupiera

// Pętla wykonuje się dopóki suma punktów krupiera jest mniejsza od 21
    while (suma_reki(rekaKrupiera) < 21) {
        sleep(2); // Krótkie opóźnienie dla lepszej czytelności

        // Jeśli suma punktów krupiera jest mniejsza od 17, krupier dobiera kartę
        if (suma_reki(rekaKrupiera) < 17) {
            printf("Krupier dobiera kartę.\n");
            rekaKrupiera[obecnaKartaKrupiera] = talia[obecnaKartaWTali]; // Dobranie karty z talii
            obecnaKartaWTali++; // Przejście do następnej karty w talii
            obecnaKartaKrupiera++; // Zwiększenie liczby kart krupiera

            printf("Karty krupiera:\n");
            // Wyświetlanie kart krupiera
            for (int i = 0; i < obecnaKartaKrupiera; i++) {
                show_card(rekaKrupiera[i]);
            }
            printf("Suma: %d\n", suma_reki(rekaKrupiera)); // Wyświetlanie sumy punktów krupiera
        } else {
            printf("Krupier pasuje.\n\n"); // Jeśli suma punktów krupiera jest 17 lub więcej, krupier pasuje
            sleep(2); // Krótkie opóźnienie dla lepszej czytelności
            break; // Przerwanie pętli
        }
    }

// Wyświetlanie wyników
    printf("\e[1;1H\e[2J"); // Czyści ekran

    printf("Karty krupiera:\n");
// Wyświetlanie kart krupiera
    for (int i = 0; i < obecnaKartaKrupiera; i++) {
        show_card(rekaKrupiera[i]);
    }
    printf("\n");
    printf("Suma: %d\n", suma_reki(rekaKrupiera)); // Wyświetlanie sumy punktów krupiera
    printf("---------------------------------------\n");
    printf("Twoje karty:\n");
// Wyświetlanie kart gracza
    for (int i = 0; i < obecnaKartaGracza; i++) {
        show_card(rekaGracza[i]);
    }
    printf("\n");
    printf("Suma: %d\n", suma_reki(rekaGracza)); // Wyświetlanie sumy punktów gracza
    printf("---------------------------------------\n\n");
    sleep(1); // Krótkie opóźnienie dla lepszej czytelności

// Sprawdzanie wyniku gry
    if (suma_reki(rekaGracza) > suma_reki(rekaKrupiera) || suma_reki(rekaKrupiera) > 21) {
        return stawka * 2 + misc_zeotny; // Jeśli gracz ma więcej punktów lub krupier przekroczył 21, gracz wygrywa
    } else if (suma_reki(rekaGracza) == suma_reki(rekaKrupiera)) {
        return stawka + misc_zeotny; // Jeśli gracz i krupier mają tyle samo punktów, jest remis
    } else {
        return 0 + misc_zeotny; // W przeciwnym razie gracz przegrywa
    }

}

//=======================================================================
int main() {
    system("chcp 65001  > nul"); // Ustawia kodowanie na UTF-8
    printf("\e[1;1H\e[2J"); // Czyści ekran
    srand(time(NULL)); // Inicjalizuje generator liczb losowych

    int aktywneKarty = ROZMIAR_TALI; // Liczba aktywnych kart

    int wybor = 0; // Wybór gracza
    int zetony = 0; // Liczba żetonów gracza

    // Inicjalizacja talii kart
    for (int i = 0; i < ROZMIAR_TALI; i++) {
        struct card temp = {i / 13, i % 13 + 1};
        talia[i] = temp;
    }

    print_title(); // Wyświetla tytuł gry

    // Wybór gracza: rozpoczęcie gry, wyświetlenie zasad lub wyjście
    do {
        printf("\n1 - Start | 2 - Zasady | 3 - Wyjście\n");
        if (scanf("%d", &wybor) != 1) {
            wybor = 0;
            while (getchar() != '\n');
        }
    } while (wybor != 1 && wybor != 2 && wybor != 3);

    // Obsługa wyboru gracza
    switch (wybor) {
        case 1:
            break;

        case 2:
            zasady(); // Wyświetla zasady gry
            break;

        case 3:
            return 0; // Kończy program
    }

    // Wybór liczby startowych żetonów
    do {
        puts("\nStart gry");
        printf("\nWybierz ile dostaniesz startowych żetonów.\n1 - 1k | 2 - 5k | 3 - 10k\n");
        if (scanf("%d", &wybor) != 1) {
            wybor = 0;
            while (getchar() != '\n');
        }
    } while (wybor != 1 && wybor != 2 && wybor != 3);

    // Przypisanie liczby startowych żetonów
    switch (wybor) {
        case 1:
            zetony = 1000;
            break;

        case 2:
            zetony = 5000;
            break;

        case 3:
            zetony = 10000;
            break;

        default:
            printf("Coś poszło nie tak z żetonami.\n");
            break;
    }

    int zetony_start = zetony; // Zapamiętanie liczby startowych żetonów

    int gry = 0; // Liczba rozegranych gier
    int stawka = 0; // Stawka gracza
    printf("Dostajesz %d żetonów.\n", zetony);
    shuffle(); // Tasowanie talii

    // Główna pętla gry
    while (1 == 1) {
        // Wybór stawki
        do {
            printf("Ile chcesz postawić?\n");
            if (scanf("%d", &stawka) != 1) {
                stawka = 0;
                while (getchar() != '\n');
            }

            if (stawka > zetony) {
                printf("Nie masz tyle żetonów.\n");
            } else if (stawka <= 0) {
                printf("Musisz coś postawić\n");
            }
        } while (stawka > zetony || stawka <= 0);

        gry++;
        printf("Gra numer %d\n", gry);

        zetony -= stawka; // Odejmowanie stawki od liczby żetonów
        int wygrana = gra(stawka); // Rozpoczęcie gry

        // Obsługa wyniku gry
        if (wygrana <= 0) {
            printf("Przegrałeś.\n");
            printf("Tracisz %d żetonów.\n", stawka);
        } else if (wygrana == stawka) {
            printf("Remis twoje żetony wracają.\n");
            zetony += stawka;
        } else {
            printf("Wygrałeś.\n");
            printf("Dostajesz %d żetonów.\n", wygrana);
            zetony += wygrana;
        }

        printf("Masz %d żetonów.\n", zetony);

        wybor = 0;

        if (zetony <= 0) {
            break; // Jeśli gracz nie ma żetonów, kończy grę
        }

        // Wybór gracza: kontynuacja gry lub zakończenie
        do {
            printf("1 - Graj dalej | 2 - Zakończ\n");
            if (scanf("%d", &wybor) != 1) {
                wybor = 0;
                while (getchar() != '\n');
            }
        } while (wybor != 1 && wybor != 2);

        if (wybor == 2) {
            break; // Jeśli gracz wybrał zakończenie, kończy grę
        }
    }

    // Wyświetlanie wyniku końcowego
    if (zetony == 0) {
        printf("Przegrałeś wszystkie żetony.\n");
    } else if (zetony_start < zetony) {
        printf("Zdobyłeś %d żetonów.\n", zetony - zetony_start);
    } else if (zetony_start > zetony) {
        printf("Straciłeś %d żetonów.\n", zetony_start - zetony);
    } else {
        printf("Nie wygrałeś ani nie przegrałeś żetonów.\n");
    }

    return 0; // Kończy program
}
