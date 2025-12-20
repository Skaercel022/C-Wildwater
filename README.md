# C-Wildwater

## Description

Ce projet à été réalisé par 3 étudiants en deuxième année de classe préparatoire intégrée à CY Tech. Le but de celui-ci est de faire la synthèse de données d’un système de distribution d’eau. Le programme combine à la fois du script Shell pour gérer les fichiers et les entrées de l'utilisateur ainsi que de la programmation procédurale en C, étant bien plus rapide que le Shell pour le traitement des données.

## Table des matières

- [Description](#description)
- [Démarrage](#démarrage)
  - [Prérequis](#prérequis)
  - [Installation](#installation)
- [Utilisation](#utilisation)
- [Structure du projet](#structure-du-projet)

## Démarrage

Cette section vous guidera sur la façon de configurer et d'exécuter le projet C-Wildwater.

### Prérequis

Avant de commencer, assurez-vous d'avoir installé les éléments suivants :

*   **Compilateur C (GCC) :** Vous aurez besoin d'un compilateur C (comme GCC) pour compiler le code source.
*   **Gnuplot :** Nécessaire pour générer les histogrammes.

### Installation

1.  **Cloner le dépôt :**

    ```bash
    git clone https://github.com/Skaercel022/C-Wildwater.git
    cd C-Wildwater
    ```

## Utilisation

Ce projet utilise un script shell (`c-wildwater.sh`) pour compiler et exécuter le programme. Voici les différentes façons d'utiliser le script :

1.  **Histogramme :**

    ```bash
    ./c-wildwater.sh <fichier_donnees> histo <max|src|real|all>
    ```

    *   `<fichier_donnees>` : Le fichier de données à utiliser pour générer l'histogramme.
    *   `histo` : Indique que vous souhaitez générer un histogramme.
    *   `<max|src|real|all>` : Le type de données à afficher dans l'histogramme :
        *   `max` : Valeurs maximales.
        *   `src` : Valeurs sources.
        *   `real` : Valeurs réelles.
        *   `all` : L'ensemble des valeurs énnoncées.

2.  **Fuites :**

    ```bash
    ./c-wildwater.sh <fichier_donnees> leaks <ID_Usine>
    ```

    *   `<fichier_donnees>` : Le fichier de données à utiliser pour analyser les fuites.
    *   `leaks` : Indique que vous souhaitez analyser les fuites.
    *   `<ID_Usine>` : L'identifiant de l'usine à analyser.

## Structure du projet

```
C-Wildwater/
├── .gitignore                     # Spécifie les fichiers intentionnellement non suivis que Git doit ignorer
├── Projet_C-WildWater_preIng2_2025_2026_v1.1.pdf  # Documentation du projet (probablement en français)
├── README.md                      # Ce fichier !
├── c-wildwater.sh                 # Script shell pour compiler et exécuter le programme
├── codeC/                         # Répertoire contenant le code source C
└── docs/                          # Répertoire pour la documentation supplémentaire
```


