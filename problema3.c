#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MIN_ASCII 32
#define MAX_ASCII 125
#define RANGE (MAX_ASCII - MIN_ASCII + 1)

typedef char* (*CodificareFunctii)(char);

// Codificarea 1 - bazata pe numarul de aparitii
char* codificare1(char c) 
{
    // Vector de frecventa pentru a tine minte aparitiile (cu ajutorul codului ASCII)
    static int aparitii[MAX_ASCII] = {0}; 
    int index = (int)c;
    int count = aparitii[index];
    aparitii[index]++;
    
    // Cautam cate cifre trebuie sa adaugam
    int temp = count;
    int nr_cifre = 0;
    if (temp == 0) 
    {
        nr_cifre = 1;
    } 
    else 
    {
        while (temp != 0) 
        {
            temp /= 10;
            nr_cifre++;
        }
    }

    // Alocam memoria ( nr_cifre + 1 (c) + 1 (\0))
    char* rezultat = (char*) malloc((nr_cifre + 2) * sizeof(char));
    
    // Formam rezultatul
    rezultat[0] = c;
    rezultat[nr_cifre + 1] = '\0';
    temp = count;

    if (temp == 0) 
    {
        rezultat[1] = '0';
    } 
    else 
    {
        while (temp != 0) 
        {
            rezultat[nr_cifre] = (temp % 10) + '0';
            temp /= 10;
            nr_cifre--;
        }
    }
    
    return rezultat;
}

// Codificarea 2 - transformari pe biti
char* codificare2(char c) 
{
    unsigned char val = (unsigned char)c;
    
    // Shiftam 1 (primul bit) de cate 3, respectiv 6 ori si folosim XOR pentru a nega bitul respectiv
    val ^= (1 << 3);
    val ^= (1 << 6);
    
    // Calculam numarul de biti egali cu 1
    // Shiftam pe rand spre dreapta si vedem de cate ori ultimul bit e 1
    int nr_biti_1 = 0;
    unsigned char temp = val;
    for (int i = 0; i < 8; i++) 
    {
        if ((temp >> i) & 1) 
        {
            nr_biti_1++;
        }
    }
    
    // Aplicam OR cu 00100000 (0x20)
    val |= 0x20;
    
    // Folosim ~1 (1111 1110) si folosim & pentru a schimba doar ultimul bit cu 0
    val = val & (~1);
    
    // Construim stringul rezultat
    // Adaugam fix 3 variabile in rezultat (val, nr_biti si \0)
    char* rezultat = (char*)malloc(3* sizeof(char));
    rezultat[0] = val;
    rezultat[1] = nr_biti_1 + '0';
    rezultat[2] = '\0';

    return rezultat;
}

// Codificarea 3 - conversie litere
char* codificare3(char c) 
{
    // Modificam doar literele mici sa fie mari
    char val = c;
    if (islower(val)) 
    {
        val = toupper(val);
    }
    
    // Adaugam fix 2 variabile in rezultat (val si \0)
    char* rezultat = (char*)malloc(2 * sizeof(char));
    rezultat[0] = val;
    rezultat[1] = '\0';

    return rezultat;
}

// Genereaza sirul 
// Foloseste functia rand() si RANGE si MIN_ASCII (pentru a lua intervalul 32, 125 inclusiv)
char* genereaza_sir_aleator(int lungime) 
{
    char* sir = (char*)malloc((lungime + 1) * sizeof(char));
    
    for (int i = 0; i < lungime; i++) { sir[i] = (char)(rand() % RANGE + MIN_ASCII); }
    sir[lungime] = '\0';

    return sir;
}

// Transforma sirul folosind cele 3 functii la intamplare
char* transforma_sir(char* sir_input, int lungime, CodificareFunctii functii[]) 
{
    // Optimizare: Alocam o singura data si dupa realocam exact lungimea necesara
    long max_size = (long)lungime * 8 + 1;
    char* parola_finala = (char*)malloc(max_size * sizeof(char));
    
    long current_pos = 0;
    
    for (int i = 0; i < lungime; i++) 
    {
        int index_functie = rand() % 3;
        char* segment = functii[index_functie](sir_input[i]);

        int segment_len = strlen(segment);

        // Optimizare: Folosim memcpy pentru ca e mai rapid decat strcap
        memcpy(parola_finala + current_pos, segment, segment_len);
        current_pos += segment_len;

        // Dupa ce am adaugat segmentul prin copierea memoriei, d-am free la memorie si repetam pana cand am facut pentru fiecare litera
        free(segment);
    }

    // Punem finalul sirului
    parola_finala[current_pos] = '\0';

    // Optimizare: Realocam doar lungimea pe care o foloseste pointerul 
    parola_finala = (char*)realloc(parola_finala, (current_pos + 1) * sizeof(char));

    return parola_finala;
}

// Main-ul care imbina toate functiile si afiseaza rezultatul
// La final folosim free pentru a elibera memoria folosita
int main() 
{
    CodificareFunctii vector_functii[3];
    vector_functii[0] = codificare1;
    vector_functii[1] = codificare2;
    vector_functii[2] = codificare3;

    int seed, lungime;
    scanf("%d %d", &seed, &lungime);
    
    srand(seed);

    char* sir_initial = genereaza_sir_aleator(lungime);
    char* parola = transforma_sir(sir_initial, lungime, vector_functii);
    
    printf("%s\n", parola);
    
    free(sir_initial);
    free(parola);
    
    return 0;
}
