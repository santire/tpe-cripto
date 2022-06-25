# Criptografía y Seguridad - Trabajo Práctico

<br />
<p align="center">
  <a href="https://github.com/santire/tpe-redes/">
    <img src="assets/logo.png" alt="Logo" width="234" height="115">
  </a>
</p>

<h3 align="center">stegobmp</h3>

## Tabla de Contenidos

- [Criptografía y Seguridad - Trabajo Práctico](#criptografía-y-seguridad---trabajo-práctico)
  - [Tabla de Contenidos](#tabla-de-contenidos)
  - [Introducción](#introducción)
    - [Consigna](#consigna)
  - [Requisitos](#requisitos)
  - [Instalación](#instalación)
  - [Modo de uso](#modo-de-uso)

## Introducción

La esteganografía es la ciencia que se ocupa de la manera de ocultar un
mensaje. La existencia de un mensaje u objeto es ocultada dentro de otro,
llamado portador. El objetivo es proteger información sensible, pero a
diferencia de la criptografía que hace ininteligible dicha información, la
esteganografía logra que la información pase completamente desapercibida al
ocultar su existencia misma.

### Consigna

Realizar un programa `stegobmp` en lenguaje C o Java que efectúe las siguientes operaciones:

- Oculte un archivo cualquiera en un archivo .bmp, mediante un método de
  esteganografiado elegido, con o sin password.
- Descubra un archivo oculto en un archivo .bmp que haya sido previamente
  esteganografiado con uno de los métodos provistos.
- Estegoanalice un archivo .bmp para determinar si tiene un archivo incrustado, con qué
  algoritmo y lo extraiga correctamente.

## Requisitos

- Sistema linux
- OpenSSL 1.1.1f

## Instalación

Para instalar basta con pararse en la carpeta raíz del proyecto y correr

```sh
make
```

Esto va a generar un binario `./bin/stegobmp`.

Si se quisiera evaluar el correcto funcionamiento del programa se puede ejecutar

```sh
make tests
```

Esto compila el programa, corre algunas pruebas, retorna los resultados y
limpia los binarios al terminar.

## Modo de uso

Se cuenta con dos modos distintos `extract` y `embed`. El modo `extract` se
utiliza para extraer mensajes ocultos de imágenes, el `embed` para ocultar
mensajes en imágenes.

El programa cuenta con una opción `stegobmp --help` para ver como utilizar los comandos, la salida es la siguiente:

```
Usage: stegobmp [OPTION...]
Hide and extract files from bitmap files using different stenography
techniques.

 Mode Options  (Mutually exclusive):
      --embed                Use program to embed a message in bmp file.
      --extract              Use program to extract a message from a bmp file.

 Embed Specific Options
  -i, --in=FILE              File to embed.

 General Options:
  -o, --out=FILE             Output file. Bitmap with hidden file if embed
                             mode, extracted file otherwise.
  -p, --bmp=FILE             Bitmap porter file.
  -s, --steg=<LSB1 | LSB4 | LSBI>
                             Stenography algorithm.

 Optional Cypher Options:

 Optional arguments require = (ex: -m=ECB)
  -a, --cypher[=AES128 | AES192 | AES256 | DES]
                             Cypher algorithm.
  -m, --block-mode[=ECB | CBF | OFB | CBC]
                             Block mode.
      --pass[=TEXT]          Cypher password. If not present, other cypher
                             parameters are ignored.

  -?, --help                 Give this help list
      --usage                Give a short usage message
  -V, --version              Print program version

Mandatory or optional arguments to long options are also mandatory or optional
for any corresponding short options.
```

Nota: En los parámetros opcionales se debe utilizar `=` para asignar el valor
(ej: `-x=valor o --equis=valor`), en los obligatorios se puede obviar.

### Ejemplos de ejecución

**Ejemplo de embed**

Esteganografiar el archivo de texto `mensaje1.txt` en el archivo portador
`imagen1.bmp` obteniendo un archivo `imagenmas1.bmp` mediante el algoritmo LSB
Improved, con encripción DES en modo CBC con password “oculto”

```sh
bin/stegobmp --embed -i mensaje1.txt -p imagen1.bmp -o imagenmas1.bmp \
-s LSBI -a=des -m=cbc --pass=oculto
```


**Ejemplo de extract**

Extraer el archivo de texto “mensaje1.txt” del archivo portador
“imagenmas1.bmp” ocultado mediante el algoritmo LSB Improved, con encripción
DES en modo CBC con password “oculto”

```sh
bin/stegobmp --extract -p imagenmas1.bmp -o mensaje1 -s LSBI -a=des -m=cbc \
--pass=oculto
```

o análogamente con las versiones largas de los parámetros:

```sh
bin/stegobmp --extract --bmp imagenmas1.bmp --out mensaje1 --steg LSBI \
--cypher=des --block-mode=cbc --pass=oculto
```

