# Conversor de Vídeo (FFmpeg Wrapper)

Ferramenta de linha de comando para conversão em lote com presets
personalizados e suporte a multi‑thread.

------------------------------------------------------------------------

## O que o programa faz

Este programa é um **wrapper do FFmpeg** que automatiza conversões de
vídeo usando **presets personalizados**, suporte a múltiplos arquivos e
execução paralela.

Ele lê comandos pré-definidos de uma pasta `presets/` e aplica variáveis
dinamicamente para gerar o comando final do FFmpeg.

------------------------------------------------------------------------

## Estrutura de Presets

Cada arquivo `.preset` pode conter várias definições:

``` bash
nomeDoPreset: -i ${input}.${extension} -c:v libx264 ${input}.mp4;
```

### Variáveis disponíveis

  Variável         Descrição
  ---------------- -------------------------------
  `${input}`       Nome do arquivo sem extensão
  `${extension}`   Extensão original
  `${outdir}`      Diretório definido com `-out`

------------------------------------------------------------------------

## Uso Básico

``` bash
./fconv video.mp4 h264
```

Converte um único arquivo usando o preset informado.

------------------------------------------------------------------------

## Flags Disponíveis

### `-out <dir>`

Define diretório de saída para os arquivos convertidos.

### `-g [lista]`

Converte múltiplos arquivos específicos.

``` bash
./fconv -g "["a.mp4","b.mov"]" h264
```

### `-a <dir>`

Converte todos os vídeos de um diretório automaticamente.

``` bash
./fconv -a ./videos h264
```

### `--`

Passa argumentos extras diretamente ao FFmpeg.

------------------------------------------------------------------------

## Multi‑thread

O programa processa vários vídeos em paralelo usando o número de threads
disponíveis no processador (`hardware_concurrency`).

------------------------------------------------------------------------

## Fluxo Interno

1.  Parser de flags interpreta os argumentos\
2.  Preset é localizado e carregado\
3.  Variáveis são substituídas\
4.  Comando FFmpeg é montado\
5.  Tarefas são distribuídas entre threads

------------------------------------------------------------------------

**Projeto de conversão em lote com FFmpeg • C++17 • DevGhostWhite**
