# Sobre
Código em C que insere uma mensagem de texto em uma imagem utilizando a esteganografia LSB, utilizando a biblioteca libpng.

O código para esteganografia está em **lsbStegDiffuse.c**. A compilação do código foi feita com 
```
gcc lsbStegDiffuse.c -o lsbStegDiffuse -lpng -O2
``` 
no Linux WSL Ubuntu 22.04.3 LTS

As imagens utilizadas no artigo estão em **article_images**.
Todas as imagens utilizadas para testes foram retiradas dos backgrounds do Linux Mint Una, Vanessa e Uma. Os devidos créditos estão nos arquivos **Credits1**, **Credits2** e **Credits3**.

# Uso básico da ferramenta:

Usa a imagem **cover.png** como imagem base, e insere a mensagem contida em **message.txt** com n = **stegoBitDepth** (1-8) e gera a imagem **stego.png**:
```
./lsbStegDiffuse cover.png stego.png message.txt stegoBitDepth
```

Extrai a mensagem de **stego.png** com profundidade **stegoBitDepth** (1-8) e armazena ela em **messageOut.txt**:
```
./lsbStegDiffuse stego.png messageOut.txt stegoBitDepth

```

Extrai os bit planes vermelho, verde e azul do bit **bitPlane** (1-8) da imagem **image.png**:
```
./lsbStegDiffuse image.png bitPlane
```