### Copyright Rares-Stefan Goidescu 312CAb 2023-2024

# Tema 3 - Editor de imagini

## Ideea de abordare a problemei

Pentru imaginile de tip grayscale, lucrurile sunt destul de straight-forward.
Insa atunci cand intra in discutie si imagini color cu 3 canale de culoare,
toate cu valoarea maxima 255, lucrurile se complica putin.
Pentru a putea aborda problema trebuie aleasa o metoda in care sa stocam
valorile corespunzatoare fiecarui canal de culoare pentru fiecare pixel.
Acest lucru se putea face fie prin uniuni, fie prin mai multe variabile in
cadrul structurii `img_data`, sau folosind o tehnica foarte cunoscuta in
image processing: packing.

Aceasta metoda se bazeaza pe operatii pe biti, variabile cu numar mare de biti
si tipul sistemului Little/Big-Endian.
Abordarea din acest program este una
specifica Little-Endian, insa se poate adapta si pentru Big-Endian.

Explicatie: valorile maxime intalnite in cadrul temei sunt de 255, acesta fiind
cel mai mare numar care se poate scrie in baza 2 pe 8 biti.
O variabila de tip `unsigned int` are dimensiunea de 32 de biti, iar 32 / 8 = 4
(alpha, blue, green, red).
In aceasta abordare alpha va fi pe cea mai semnificativa pozitie (pe Little-E).

Astfel, _"valoarea"_ unui pixel este rezultatul a unor shiftari la stanga si
a unor operatii de tip _"sau"_ logic.

Acesta a fost packing-ul. Unpacking-ul este operatia inversa si vom ajunge
inapoi la valorile specifice canalelor RGB folosind shiftari la dreapta si
operatia _"si"_ logic.

