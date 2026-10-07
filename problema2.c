#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Structura luata din cerinta
struct pair {
    char *field;
    char *value;
};

// Debug: Functie ajutatoare, am analizat checkerul si am decis sa o implementez pentru a functiona testele
int is_word_char(char c)
{
    return isalnum(c) || c == '_';
}

// Optimizare: Dintr-un pointer input facem 2 subpointeri (field si value) care sunt separati printr-un spatiu
void get_parameters(struct pair *input)
{
    char *buf = NULL;
    unsigned long buf_size = 0;
    long chars_read = getline(&buf, &buf_size, stdin);

    if (chars_read > 0) 
    {
        if (buf[chars_read - 1] == '\n') buf[--chars_read] = '\0';

        char *sep = strchr(buf, ' ');
        if (sep) 
        {
            *sep = '\0';
            input->field = buf;
            input->value = sep + 1;
        }
    }
}

// Optimizare: Functia aplica cate o pereche pe rand si returneaza noul text
// Optimizare: Initial am scris o functie pentru da autofill pentru fiecare aparitie de o data, dar era foarte lenta
// Debug: Gestioneaza alocarea exacta si elibereaza textul vechi
char* autofill(char *text, const char *key, const char *val) 
{
    int key_len = strlen(key);
    int val_len = strlen(val);
    int count = 0;

    // Numaram aparitiile valide
    char *p = text;
    while ((p = strstr(p, key)) != NULL) 
    {
        // Verificam limitele
        char prev = (p == text) ? 0 : *(p - 1);
        char next = *(p + key_len);

        if (!is_word_char(prev) && !is_word_char(next)) 
        {
            count++;
            p += key_len;
        } 
        else 
        {
            p++;
        }
    }
    
    // Iese daca nu gaseste nicio schimbare
    if (count == 0) return text;

    // Alocam memoria necesara pentru pointerul nou pentru a adauga text
    long new_len = strlen(text) + (long)count * (val_len - key_len);
    char *new_text = (char*)malloc((new_len + 1) * sizeof(char));

    // Optimizare: Am incercat sa o fac initial doar in text si crea erori chekerului 
    // Optimizare: Asa ca am adaugat inca un pointer ca sa nu dea erori si sa fie mai stabil codul
    char *dest = new_text;
    char *src = text;
    p = text;

    while ((p = strstr(p, key)) != NULL) 
    {
        char prev = (p == text) ? 0 : *(p - 1);
        char next = *(p + key_len);

        if (!is_word_char(prev) && !is_word_char(next)) 
        {
            // Copiem segmentul dintre ultimul match si acesta
            unsigned long segment_len = p - src;
            memcpy(dest, src, segment_len);
            dest += segment_len;

            // Copiem valoare care e de inlocuit
            memcpy(dest, val, val_len);
            dest += val_len;

            // Adaugam lungimea cheii pentru a nu o lua din nou si a face o bucla infinita
            src = p + key_len;
            p += key_len;
        }
        else
        {
            // Nu e valid, asa ca incepem cautarea de la urmatorul caracter pentru a nu forma o bucla infinita
            p++;
        }
    }

    // Copiem restul sirului dupa ultima iterare
    strcpy(dest, src);

    // Eliberam textul vechi
    free(text);

    return new_text;
}

int main()
{

    int N;
    // Debug: Eroare cand dadeam la tastatura input doar a
    // Debug: Eroare cand imbin scanf cu getline (ia new lineul din scanf si il adauga in getline)
    if (scanf("%d\n", &N) != 1) return 1;

    struct pair *pairs = malloc(N * sizeof(struct pair));
    for(int i = 0; i < N; i++)
    {
        get_parameters(&pairs[i]);
    }

    char *form_text = NULL;
    unsigned long buf_size = 0;
    long len = getline(&form_text, &buf_size, stdin);
    if (len > 0 && form_text[len - 1] == '\n') 
    {
        form_text[--len] = '\0';
    }

    // Optimizare: Aplicam inlocuirile in mod secvential
    for(int i = 0; i < N; i++)
    {
        form_text = autofill(form_text, pairs[i].field, pairs[i].value);
    }

    printf("%s\n", form_text);

    // Optimizare: Free them all (am dat foarte multe double free-uri la inceput, asa ca le-am adaugat dupa ce am terminat codul)
    for(int i = 0; i < N; i++) { free(pairs[i].field); }
    free(pairs);
    free(form_text);

    return 0;
}
