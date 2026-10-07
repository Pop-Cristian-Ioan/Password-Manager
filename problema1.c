#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_INPUT 14000

// Verifica lungimea maxima si daca numarul e alfa numeric
// Returnează 1 dacă e valid, 0 altfel
int is_valid_str(char *input)
{
    // Verificam lungimea
    unsigned long input_len = strlen(input);
    if (input_len > MAX_INPUT) return 0;

    for(unsigned long i = 0; i < input_len; i++)
    {
        if(!isalpha(input[i]))
        {
            return 0;
        }   
    }
    return 1;
}

// Cripteaza cheia si parola folosind cifrul lui Vigenère
// Cauta pozitia parolei si a chei fata de 'a'
// Le aduna si foloseste %52 (literele din alfabet *2 pentru case sensitivity[aA])
// %52 se foloseste pentru cazul in care literele au o distana mare si trebuie sa reia de la 'a'
// Apoi cauta pozitia exacta a literei si o pune in parola criptata
// La final adaugam '/0' pentru a marca sfarsitul stringului
void criptare(char *password, char *key, char **encrypted) 
{
    int pass_lenght = strlen(password);
    int key_lenght = strlen(key);
    for (int i = 0; i < pass_lenght; i++) 
    {
        int pass_index = isupper(password[i]) ? password[i] - 'A' + 26 : password[i] - 'a';
        int key_index = isupper(key[i % key_lenght]) ? key[i % key_lenght] - 'A' + 26 : key[i % key_lenght] - 'a';
        int encrypted_index = (pass_index + key_index) % 52;
        (*encrypted)[i] = (encrypted_index < 26) ? (char)('a' + encrypted_index) : (char)('A' + (encrypted_index - 26));
    }
    (*encrypted)[pass_lenght] = '\0';
}

// Imbina toate functiile la un loc, aloca pointeri si le da free la final
// Debug: Initial citeam cheia si parola cu un singur getline si faceam 2 subpointere intr-un pointer
// Debug: Acum sunt 2 getlineuri, asa poate verifica si daca sunt spatii 
int main()
{
    char *key = NULL, *password = NULL, *encrypted = NULL;
    unsigned long len_key = 0, len_pass = 0;
    
    // Debug: Citim prima linie (cheia)
    long read_key = getline(&key, &len_key, stdin);
    // Debug: Citim a doua linie (textul)
    long read_pass = getline(&password, &len_pass, stdin);

    // Eliminam newline de la final, daca exista
    if (read_key > 0 && key[read_key - 1] == '\n') key[--read_key] = '\0';
    if (read_pass > 0 && password[read_pass - 1] == '\n') password[--read_pass] = '\0';

    // Verificam validitatea si lungimea
    if (read_key > 0 && read_pass > 0 && is_valid_str(key) && is_valid_str(password))
    {
        encrypted = (char*)malloc((strlen(password) + 1) * sizeof(char));
        criptare(password, key, &encrypted);
        printf("%s\n", encrypted);
        free(encrypted);
    }
    else
    {
        printf("INVALID\n");
    }

    free(key);
    free(password);

    return 0;
}

