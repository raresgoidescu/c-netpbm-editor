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
Abordarea din acest program este una specifica Little-Endian, insa se poate
adapta si pentru Big-Endian.

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

Aceasta abordare aduce cateva avantaje printre care:
	- Folosirea eficienta a memoriei: in cazul pozelor de dimensiuni mari,
	stocarea separata a canalelor de culoare poate fi destul de ineficient.
	- Procesarea mai eficienta: operatiile pe biti pe un singur numar intreg
	tind sa fie mai rapide decat operatiile pe mai multe variabile separate.
	- Reduce din complexitatea (nu big O) programului.

## Interpretarea comenzilor de la tastatura

Am folosit o functie de parsare (nu sunt sigur ca o pot numi asa), in care
citeam cuvant/cifra cu cuvant/cifra pana la finalul liniei si pastram diferite
campuri cheie, precum numele comenzii (primul cuvant), si diversii parametri.
Am incercat sa fac si cateva validari in cadrul acestei functii pentru a
trimite mai departe un rezultat cat mai corect (macar din punct de vedere
al sintaxei).

## LOAD

Load-ul a fost implementat pentru fiecare tip de fisier folosind magicword-ul,
dimensiunile imaginii si valoarea maxima a unui canal, informatii care se aflau
la inceputul fiecarui fisier in plain text, indiferent de tipul imaginii si
metoda in care aceasta a fost salvata.

Dupa ce am facut rost de aceste informatii am putut aplica operatiile
corespunzatoare fiecarui tip de imagine si am putut incarca in memorie
un _pixel map_ sub forma unei matrice de intregi fara semn cu dimensiunea
`height x width`.

De asemenea, au mai fost facute cateva verificari pentru a nu prelucra un
fisier de input invalid.

## SELECT

Aceasta comanda a fost implementata folosind un tablou unidimensional in
care retineam selectia curenta si a unui alt vector in care retineam ultima
selectie valida, pentru a putea inlocui coordonatele curente in cazul unui
set invalid introdus la tastatura.

## HISTOGRAM

Aceasta comanda a fost implementata folosind un vector de frecventa pentru
imaginile de tip grayscale.
Pentru a afisa _graficul_ a trebuit sa folosesc
formula din cerinta, care intorcea un numar trunchiat la numarul maxim de
asterisk-uri.
Pentru a afisa cu un anumit numar de bin-uri, am implementat un salt in
structurile repetitive invers proportional cu numarul de bin-uri (ex. in cazul
in care se vor 2 bin-uri, pasul va fi de 128 in parcurgerea unui vector
de frecventa cu 256 de elemente).

## EQUALIZE

Aceasta comanda a fost implementata folosind formula specifica, care folosea,
la randul ei, CDF-ul (acea suma din formula).
Pentru ca aceasta cantitate sa fie usor de calculat, am retinut fiecare
rezultat al unei sume intr-un tablou unidimensional.

## ROTATE

In implementarea acestei comenzi am incercat sa am o abordare eficiente
din punct de vedere al timpului de executie.
Avand in vedere ca fiecare unghi pe care il poate accepta comanda este
divizibil cu 90, putem simplifica acest unghi, iar de la 10 valori
(+/-0, +/-90, +/-180 etc), ajungem la 3 valori: -1, 1, 2, unde:
	- -1 => rotim imaginea cu 90 de grade la stanga (aux matrix)
	-  1 => rotim imaginea cu 90 de grade la dreapta (aux matrix)
	-  2 => rotim imaginea cu 180 de grade (o rasturnam) (in-place)
Astfel, in loc sa facem 3 rotiri in cazul unui unghi de 270, vom face o singura
rotire, la stanga, de 90 de grade, aducand un avantaj destul de mare cand
lucram cu o matrice de aceste dimensiuni.

## CROP

Aceasta comanda a fost implementata folosind o matrice auxiliara care
retine elementele din selectie, cele care vor ramane dupa aplicarea comenzii.

## APPLY

Aceasta comanda a fost implementata conform formulei si metodei de aplicare
a kernel-elor, folosind o matrice auxiliara pentru a nu altera valorile
pixelilor vecini ai pixelului de la pasul curent.
Pentru a putea face aceste operatii, a trebuit sa folosesc matrice auxiliare de
dimensiuni 3x3 pentru a extrage valorile canalelor fiecari culori dupa
unpacking.
In final, rezultatele au fost _impachetate_ si au suprascris valorile vechi.

## SAVE

Aceasta comanda a fost implementata tinand cont de felul in care vrem sa
salvam o imagine, plain text/raw.

Metoda de unpacking este aceeasi in ambele cazuri, ceea ce difera este modul
in care deschidem fisierul pentru scriere si cum scriem in el.

In cazul in care dorim sa scriem plain text, va trebui sa scriem numar cu
numar, insa in cazul in care dorim sa scriem in format raw va trebui sa scriem
caracter cu caracter.

## EXIT

Comanda `EXIT` elibereaza toata memoria alocata dinamic folosita si opreste
programul, devenind inoperabil si pierzand statusul actual.
