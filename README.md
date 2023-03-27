# BC6X4X_parhuzamos_eszkozok
## FIGYELEM!!!! Használat csak saját felelősségre!!! A program vírusként üzemel, rossz paraméterek megadásával rengeteg adat veszhet el.

##Feladat leírása:
> - CPU és OpenCL számítások gyorsaságának összehasonlítása.
> - A program futtatásával egy inputként megadott mappának mindent tartalmát encryptálni, illetve decryptálni.
> - A rengetek kernel hívás miatt NEM minden esetben gyorsabb az OpenCL verzió, csak nagyobb file méreteknél látható változás, de az is elhanyagolható.
> - A programok maximum 600 MB adatot képesek kezelni, felette a program kifagyhat C nyelvi limitációk miatt.

##Kódok lefordítása:
> - gcc BC6X4X.c src/kernel_loader.c -o BC6X4X.exe -lOpenCL -Iinclude
> - gcc BC6X4X_normal.c -o BC6X4X_normal.exe

##Futtatás:
> - BC6X4X.exe <mappa elérési útvonal> <kódolás kiválasztása [1->ROT13, 2->dinamikus eltolás]> <encrypt[1]/decrypt[2]>
> - BC6X4X_norm.exe <mappa elérési útvonal> <kódolás kiválasztása [1->ROT13, 2->dinamikus eltolás]> <encrypt[1]/decrypt[2]>
> - Példa:
> - BC6X4X.exe ./test/ 2 1
> - BC6X4X_norm.exe ./test/ 2 2

##CPU verzió
> - A program elején a FILEFORMAT konstantal módosíthatjuk a generált file-ok kiterjesztését.
> - A program elején a SZOVEG konstantal módosíthatjuk az encryptált file-ok elején található extra szöveget.
> - Az encrypt/decrypt függvények módosítják az inputként megadott karakter tömböket a megadott key alapján.
> - A main függvényben megnyitjuk a paraméterként megadott mappát, majd végigmegyünk a mappában található minden file-on.
> - Ha a file egy almappa, akkor tovább megyünk a következő file-ra.
> - Ha a program encryptálásra van állítva és a megnyitott file már encryptálva van (alap beállításokon .bc6x4x file kiterjesztésű), akkor tovább megyünk a következő file-ra.
> - Ha a program decryptálásra van állítva és a megnyitott file nincs encryptálva (alap beállításokon .bc6x4x file kiterjesztésű), akkor tovább megyünk a következő file-ra.
> - Ha megfelelő a file, akkor létrehozunk egy új file-t az inputhoz hasonlóan, de már az új kiterjesztéssel.
> - Encrpytálás esetén az új file elé írunk egy extra szöveget, amelyet a SZOVEG konstant tartalmaz.
> - Decrpytálás esetén az új file-nál figyelmen kívül hadjuk az első X sort, amely a SZOVEG konstant méretével megegyezik.
> - Beolvassuk az eredeti file teljes tartalmát, majd eltároljuk egy karakter tömbben.
> - A karakter tömböt átadjuk a megfelelő függvénynek.
> - Kiíratjuk az encryptált/decryptált szöveget az új file-ba, majd töröljük az eredeti filet.
> - Folytatjuk ezeket a lépéseket, amíg van az adott mappában file.

##OpenCL verzió
> - A program elején a FILEFORMAT konstantal módosíthatjuk a generált file-ok kiterjesztését.
> - A program elején a SZOVEG konstantal módosíthatjuk az encryptált file-ok elején található extra szöveget.
> - A main függvény elején létrehozzuk a szükséges OpenCL változókat. Bármi probléma esetén a program azonnal leáll.
> - Egy .cl file-ból kiolvassuk a kernel hívás kódját. Ebben találhatóak a titkosítási algoritmusok.
> - A main függvényben megnyitjuk a paraméterként megadott mappát, majd végigmegyünk a mappában található minden file-on.
> - Ha a file egy almappa, akkor tovább megyünk a következő file-ra.
> - Ha a program encryptálásra van állítva és a megnyitott file már encryptálva van (alap beállításokon .bc6x4x file kiterjesztésű), akkor tovább megyünk a következő file-ra.
> - Ha a program decryptálásra van állítva és a megnyitott file nincs encryptálva (alap beállításokon .bc6x4x file kiterjesztésű), akkor tovább megyünk a következő file-ra.
> - Ha megfelelő a file, akkor létrehozunk egy új file-t az inputhoz hasonlóan, de már az új kiterjesztéssel.
> - Encrpytálás esetén az új file elé írunk egy extra szöveget, amelyet a SZOVEG konstant tartalmaz.
> - Decrpytálás esetén az új file-nál figyelmen kívül hadjuk az első X sort, amely a SZOVEG konstant méretével megegyezik.
> - Beolvassuk az eredeti file teljes tartalmát, majd eltároljuk egy karakter tömbben.
> - A karakter tömböt átadjuk a megfelelő kernel hívásnak.
> - Minden egyes file 1-1 kernel hívás, amely titkosítja az adatokat.
> - Kiíratjuk az encryptált/decryptált szöveget az új file-ba, majd töröljük az eredeti filet.
> - Folytatjuk ezeket a lépéseket, amíg van az adott mappában file.
> - A program végén töröljük az allokált adatokat. (pl OpenCL változók)
