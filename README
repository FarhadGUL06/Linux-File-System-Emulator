	Gul Farhad Ali Irinel | File System Emulator

<==============================<main + apelare>==============================>
	Citirea se realizeaza rand cu rand de la tastatura. Am decis sa fac
	impartirea inputului folosind parcurgerea sirului pana la spatiu si
	am eliminat caracterul '\n' de la final. Astfel am comanda si am apelat
	functia de apelare cu sirul si comanda. In functie de cati parametri am,
	continui apelarea. Daca am un parametru, apeleaza functia care mai
	departe apeleaza functiile cu un parametru, daca am doi parametri, 
	functia care se ocupa de functiile cu 2 parametri. 
	Am decis sa impart in parametri cu parcurgerea string-ului, 
	la fel ca in cazul comenzii.
	
<==============================<touch si mkdir>==============================>
	Functiile touch si mkdir functioneaza asemanator. Verifica daca exista
	vreun director / fisier cu acelasi nume si opreste functia in cazul
	in care exista. Daca nu exista si directorul / fisierul poate fi creat,
	parcurgem lista de directoare / fisiere si adaugam la final folosind 
	functiile de initializare si adaugare in directorul parinte.
	
<====================================<ls>====================================>
	Functia de ls parcurge toate directoarele din directorul parinte
	si in cazul in care exista, sunt afisate. Dupa ce sunt afisate toate
	directoarele, sunt afisate si toate fisierele din directorul curent.
	
<================================<rm + rmdir>================================>
	In cazul in care nu exista fisiere / directoare, functiile se opresc.
	Verificam daca elementul cautat este primul din lista. Daca da,
	acesta este sters. Daca nu, se parcurge lista mai departe si se cautat
	elementul. Dupa ce este gasit, acesta este sters si memoria
	corespunzatoare este eliberata.
	Caz special: daca stergem un director, trebuie sa stergem recursiv si 
	ce contine acel director precum si sa eliberam memoria elementelor
	sterse. Astfel functia rmdir apeleaza rm_dir_files care sterge
	recursiv tot ce contine directorul de sters.
	
<====================================<cd>====================================>
	In cadrul acestei functii se verifica daca se da comanda "cd ..".
	In cazul acesta, se verifica daca parintele directorului curent exista
	si se face trecerea. Daca directorul curent este home, ramanem aici.
	In cazul in care este alta destinatie, se verifica daca numele 
	directorului indicat exista. Daca exista se face trecerea la acesta.
	Directorul la care se face trecerea trebuie sa fie subdirector
	in cadrul directorului curent.
	
<===================================<pwd>===================================>
	Aici am decis sa iau un vector de stringuri pentru a retine toate
	directoarele parcurse de la directorul curent la directorul curent.
	Dupa ce se face retinerea, concatenez in string-ul "cale" calea
	curenta parcurgand vectorul de stringuri de la final la inceput (pe 
	principiu de stiva). Eliberez memoria alocata vectorului de stringuri
	si returnez calea. Memoria alocata caii este eliberata in afara
	functiei pwd.
	
<===================================<tree>===================================>
	Functia tree este recursiva. Parcurg toate subdirectoarele din
	directorul curent si afisez cu functia "afisare_din_tree" identarea
	si numele subdirectoarelor. Apelez in cadrul parcurgerii subdirectoarelor
	si functia curenta tree dar cu level + 1 si subdirectorul curent din
	parcurgere. 
	Dupa ce am parcurs toate directoarele, afisam si fisierele din 
	directorul curent.
	
<====================================<mv>====================================>
	Functia mv cauta daca exista director sau fisier cu numele oldname. 
	Avand in vedere ca nu pot exista un fisier si un director cu acelasi 
	nume, nu exista posibilitate de coliziune.
	Functia mv_file parcurge si cauta fisierul cu numele oldname. Dupa
	ce il gaseste, ii schimba numele in newname, il scoate din lista de 
	fisiere al directorului curent si il adauga la final.
	Functia mv_dir functioneaza asemanator functiei mv_file doar ca, in plus,
	modifica referinta de parinte din directorul caruia ii schimbam numele
	dupa ce directorul caruia i-am schimbat numele a fost adaugat la
	coada listei. 
	
<===============================<init si free>===============================>
	Functiile de init_file si init_director primesc ca parametru 
	dublu pointer de fisier / director-ul ce trebuie creat, numele si 
	parintele. Aici se realizeaza legaturile initiale precum si alocarea
	memoriei pentru fisier / director si numele acestora.
	Functia free_director este asemanatoare functiei tree fiind recursiva.
	Aceasta parcurge intai fisierele din directorul curent si elibereaza
	memoria alocata pentru acestea. Apoi se parcurg directoarele din
	directorul curent si se apeleaza functia de free_director pe acestea.
	Dupa ce se apeleaza functia free_director, se elibereaza memoria
	alocata directorului indicat.