1. buenosaires.bmp -> sale con LSB4 y hace una foto de un buscaminas
2. me parecio curioso asi que hice clasica tecnica de ctf de hacer de cuenta que es un zip y descomprimir
3. asi econtre el archivo sol20.txt con instrucciones
4. siguiendo las instrucciones sale: AesEcb

otros archivos

usando exiftool vimos que frozen y topgun0 son bmp v5. Estos tienen compresion y otro tamanio de header (preguntar wtf se supone que era solo v3)

buenosaires era v3 pero no funcionaba con nuestros algoritmos, asi que parece que no es un tipo de LSB insertion.

por lo tanto lo abrimos con un editor de hex y al final del archivo decia "la password es desafiar" suponemos que es la password de del archivo que esta encriptado.
