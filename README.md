# Instruções para rodar aplicação

Obs: Testado no Ubuntu. O arquivo SVG da arena tem que ser passado considerando o diretório que está, no caso deste trabalho existe uma pasta chamada arenas em que há o arquivo SVG

```bash
    # Compila aplicação
    make
    
    # Roda executável indicando arquivo svg da arena
    ./trabalhocg arenas/arena_teste.svg

    # Compila e roda a aplicação indicando arquivo da arena
    make run FILE=arenas/arena_teste.svg 
```