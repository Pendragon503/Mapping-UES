# Parcial 3 - Algoritmos Gráficos
![Badge en Desarollo](https://img.shields.io/badge/Estado-En%20Desarollo-green)
![Language](https://img.shields.io/badge/language-C-blue)
![OpenGL](https://img.shields.io/badge/OpenGL-3DGraphics-blue)
[![License](https://img.shields.io/badge/license-Apache%202.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)


<p align="center">
  <img src="https://upload.wikimedia.org/wikipedia/commons/f/fa/Escudo_de_la_Universidad_de_El_Salvador.svg" width="300" alt="Logo UES FMOCC">
</p>


## Descripción

Este proyecto es una evaluación final para la materia de Algoritmos Gráficos y consiste en la creación de un mapa interactivo de nuestra universidad. El proyecto combina transformaciones 2D y 3D, y presenta figuras 3D dentro de un entorno de juego 2D. El usuario podrá moverse por el mapa con un personaje representado por un píxel, navegando a través de un viewport que renderiza una vista de 20x20 píxeles, similar a los juegos antiguos.

## Contenidos

1. [Requisitos del sistema](#requisitos-del-sistema)
2. [Instalación](#instalación)
3. [Uso](#uso)
4. [Estructura del proyecto](#estructura-del-proyecto)
5. [Autores](#autores)
6. [Licencia](#licencia)


## Requisitos del sistema

- Compilador GCC
- Librerías:
  - OpenGL
  - GLUT
  - FreeGLUT

## Instalación

1. Clonar el repositorio:

```bash
  git clone https://github.com/Pendragon503/Mapping-UES.git
  cd Mapping-UES/
```
2. Compilar el proyecto:

```bash
  gcc -o ejecutable main.c mapa.c -lm -lGL -lglut -lGLU
```

## Uso

1. Ejecutar el juego:

```bash
  ./ejecutable
```

2. Controles básicos:

## Estructura del proyecto

```plaintext
├── Mapping-UES/
│   ├── Clase Grabada/
│       └── 2024-06-03 15-33-45.mp4
│   ├── parcial/
│       ├── nbproject/
│       ├── Makefile
│       ├── ejecutable
│       ├── init.h
│       ├── main.c
│       └── mapa.c
│   ├── .gitignore
│   ├── LICENSE
└── └── README.md
```

## Autores

- [@Pendragon503](https://github.com/Pendragon503)
- [@EdMendez00](https://github.com/EdMendez00)


## Licencia

[Apache-2.0 license](https://www.apache.org/licenses/LICENSE-2.0.txt)
