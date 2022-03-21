# Instruções para rodar aplicação

Obs: Testado no Ubuntu. O arquivo SVG da arena tem que ser passado considerando o diretório que está, no caso deste trabalho existe uma pasta chamada arenas em que há o arquivo SVG

## Trabalho 2D

```bash
    # Compila aplicação
    make
    
    # Roda executável indicando arquivo svg da arena
    ./trabalhocg arenas/arena_teste.svg

    # Compila e roda a aplicação indicando arquivo da arena
    make run FILE=arenas/arena_teste.svg 
```

## Trabalho 3D

```bash
    # Compila aplicação
    make
    
    # Roda executável indicando arquivo svg da arena
    ./trabalhocg arenas/arena_teste.svg

    # Compila e roda a aplicação indicando arquivo da arena
    make run FILE=arenas/arena_teste.svg 
```

## Comandos da aplicação
* `1` -> Ativa a camera 1
* `2` -> Ativa a camera 2
* `3` -> Ativa a camera 3
* `7` -> Desativa/ativa texturas
* `8` -> Desativa/ativa luz
* `9` -> Desativa/ativa smoothness
* `+` -> Aumenta o zoom da camera
* `-` -> Diminui o zoom da camera
* `l` -> Ativa/desativa o modo dark (com lanterna)
* `p` -> Faz todos os inimigos entrarem em modo STOP
* `4` -> Alterna entre os modos dos inimigos
* `0` -> Reinicia o game

### Modos dos inimigos
* `STOP` -> Completamente imóveis
* `MOVING` -> Somente anda
* `SHOOTING` -> Somente atira
* `SHOOTING_AND_MOVING` -> Ativa e anda (modo default)