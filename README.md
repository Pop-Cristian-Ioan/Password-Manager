# Tema - Password Manager

## Timpul petrecut pe fiecare problema (aproximativ)
- Problema 1 (Cifrul Vigenère): 3 ore
- Problema 2 (Form Autofill): 5 ore
- Problema 3 (Generare Parole): 4 ore
- Optimizari (Comentate in cod): 3 ore
- Debuging (Comentate in cod, bazate pe checker): 5 ore
- README.md (timpul dedicat pentru readme): 1 ora
- Total: 21 ore

## Descrierea Soluțiilor

### Problema1 - Criptarea parolelor: Cifrul Vigenère
- **Input:** foloseste `getline` pentru a lua inputul din stdin si pentru a gasii si
  spatiile, in cazul in care sunt.
- **Validare:** (`is_valid`) parcurge şirul citit pentru a se asigura că toate
  caracterele sunt alfanumerice și că sirul nu depaseste numarul de caractere.
- **Criptarea:** are un algoritm care extinde alfabetul standard la 52 de caractere
  ('a'-'z' urmat de 'A'-'Z'). Folosind modulo 52, am calculat noile poziții ale
  caracterelor prin adunarea indexului parolei cu cel al cheii.
- **Memoria:** este alocata dinamic spațiul necesar pentru şirul criptat și am
  eliberat memoria la final.

### Problema2 - Completarea automată a datelor (AutoFill)
- **Structura datelor:** este pastrata de `struct pair` pentru a reține perechile
  (Cheie, Valoare).
- **Inputul:** este luat de la stdin de catre functia `get_parameters` care separa
  pointerul primit, inputul, in 2 subpointere care sunt introduse in `struct pair`.
- **Calcul preventiv:** Funcţia `autofill` parcurge textul iniţial şi calculează 
  lungimea finală a şirului rezultat (pe fiecare iteratie). Acest pas este foarte
  avantajos pentru a face o singură alocare (`malloc`) a textului.
- **Inlocuirea:** Folosind `strstr`, am identificat poziţiile fieldurilor. Am 
  verificat limitele cuvintelor (folosind `isalnum` pe caracterele adiacente,
  am adaugat o functie ajutatoare totusi pentru ca dadea checkerul niste erori)
  pentru a nu înlocui substring-uri din alte cuvinte.
- **Manipulare memorie:** Am folosit `memcpy` pentru a contrui noul text in `dest`,
  pointer al lui `new_text`, cu ajutorul lui `text` si inlocuirea necesara.

### Problema3 Generarea parolelor sigure
- **Generare:** Şirul iniţial este creat folosind caractere ASCII din
  intervalul [32, 125].
- **Codificări:** Am implementat cele 3 metode cerute:
  1. Concatenarea numărului de apariții.
  2. Operații pe biți: XOR pentru negarea biților 3 şi 6, numărarea biților de
  1, aplicarea măştilor logice OR şi AND pentru setarea biților specifici.
  3. Conversia literelor mici în mari (`toupper`).
- **Vector de funcţii:** Am folosit un vector de pointeri la funcţii
  (`CodificareFunctii`) pentru a selecta aleator metoda de codificare pentru
  fiecare caracter.
- **Construcție:** Parola finală este construită la inceput cu o memorie mare,
  iar ulterior folosesc un `realloc` pentru a o aduce la lungimea lui adevarata.
  Acest lucru a eficientizat foarte mult timpul pentru procele lungi (Testul 17
  de la problema 3).

## Mentiuni
- Toate resursele alocate dinamic (`malloc`, `realloc`, `getline`) sunt eliberate
  folosind `free` la sfârşitul execuției pentru a preveni memory leaks.
- Am folosit multe spatii pentru a aerisi codul, asa este lizibil si mai usor 
  de inteles.
- Commenturile sunt puse deasupra fiecarei functii si comezi mai greu de inteles.
