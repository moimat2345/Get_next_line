# get_next_line

> Lire une ligne depuis un descripteur de fichier est bien trop fastidieux.

[![42 Project](https://img.shields.io/badge/42-Project-00babc?style=flat-square&logo=42)](https://42.fr)
[![Language](https://img.shields.io/badge/Language-C-blue?style=flat-square&logo=c)](https://fr.wikipedia.org/wiki/C_(langage))
[![Norminette](https://img.shields.io/badge/Norminette-passing-success?style=flat-square)](https://github.com/42School/norminette)

**Français** | [English](README.md)

## Description

**get_next_line** est une fonction C qui lit et retourne une ligne depuis un descripteur de fichier, une ligne à la fois. Ce projet fait partie du cursus de l'école 42 et introduit un concept important en programmation C : les **variables statiques**.

La fonction fonctionne avec n'importe quel descripteur de fichier (fichiers, stdin, sockets) et peut gérer des tailles de buffer variables, ce qui en fait un outil polyvalent pour les opérations d'entrée/sortie sur fichiers.

## Table des matières

- [Fonctionnalités](#fonctionnalités)
- [Algorithme & Approche technique](#algorithme--approche-technique)
- [Installation & Compilation](#installation--compilation)
- [Utilisation](#utilisation)
- [Structure du projet](#structure-du-projet)
- [Fonctionnalités bonus](#fonctionnalités-bonus)
- [Tests](#tests)
- [Ressources](#ressources)

## Fonctionnalités

- Lire des fichiers texte ligne par ligne depuis n'importe quel descripteur de fichier
- Gérer des fichiers de n'importe quelle taille avec un buffer configurable
- Efficace en mémoire : ne lit que ce qui est nécessaire
- Fonctionne avec stdin, les fichiers et les sockets
- **Bonus** : Support de plusieurs descripteurs de fichiers simultanément
- Pas de fuites mémoire, entièrement conforme à la norminette

## Algorithme & Approche technique

### Algorithme principal

La fonction utilise un **buffer statique** pour persister les données entre les appels. C'est le concept clé qui rend possible la lecture ligne par ligne :

1. **Phase de lecture** : Lire `BUFFER_SIZE` octets depuis le descripteur de fichier
2. **Accumulation** : Stocker les données dans un buffer statique qui persiste entre les appels
3. **Recherche** : Chercher un caractère de nouvelle ligne (`\n`) dans le buffer
4. **Extraction** : Quand un retour à la ligne est trouvé, extraire et retourner la ligne complète
5. **Mise à jour** : Garder les données restantes dans le buffer pour le prochain appel

### Pourquoi des variables statiques ?

```c
char *get_next_line(int fd)
{
    static char *buffer;  // Persiste entre les appels de fonction
    // ...
}
```

Sans variables statiques, le buffer serait détruit après chaque appel de fonction, perdant toute donnée partielle lue. Les variables statiques nous permettent de :
- Se souvenir des données restantes des lectures précédentes
- Éviter de relire les mêmes données
- Gérer des lignes plus grandes que `BUFFER_SIZE`

### Flux de données

```
┌─────────────┐
│   Fichier   │  "Hello World!\nCeci est ligne 2\n..."
└──────┬──────┘
       │ read(fd, temp, BUFFER_SIZE)
       ↓
┌─────────────┐
│   Buffer    │  "Hello World!\nCeci"  (statique, persiste)
└──────┬──────┘
       │ find_newline() → position 12
       ↓
┌─────────────┐
│extract_line │  Retourne : "Hello World!\n"
└─────────────┘
       │
┌─────────────┐
│update_buffer│  Buffer maintenant : "Ceci"  (prêt pour le prochain appel)
└─────────────┘
```

### Fonctions clés

| Fonction | Rôle |
|----------|------|
| `get_next_line(int fd)` | Fonction principale : coordonne la lecture et retourne une ligne |
| `read_file(fd, buffer)` | Lit depuis fd jusqu'à trouver un retour à la ligne ou EOF |
| `find_newline(buffer)` | Cherche le caractère `\n`, retourne la position ou -1 |
| `extract_line(buffer)` | Crée et retourne la ligne (incluant `\n`) |
| `update_buffer(buffer)` | Supprime la ligne retournée, garde les données restantes |

### Gestion de la mémoire

L'implémentation gère soigneusement la mémoire :
- Toute la mémoire allouée est libérée quand elle n'est plus nécessaire
- Le buffer est libéré quand EOF est atteint ou qu'une erreur survient
- Chaque ligne retournée doit être libérée par l'appelant

## Installation & Compilation

### Compilation de base

```bash
# Cloner le dépôt
git clone https://github.com/votreusername/get_next_line.git
cd get_next_line

# Compiler avec BUFFER_SIZE par défaut (42)
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

### BUFFER_SIZE personnalisé

La taille du buffer détermine combien d'octets sont lus par appel système `read()` :

```bash
# Petit buffer (teste les cas limites)
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 get_next_line.c get_next_line_utils.c main.c -o gnl

# Grand buffer (meilleures performances)
cc -Wall -Wextra -Werror -D BUFFER_SIZE=4096 get_next_line.c get_next_line_utils.c main.c -o gnl
```

**Impact sur les performances :**
- **Buffer plus petit** → Plus d'appels `read()` → Plus lent, mais teste les cas limites
- **Buffer plus grand** → Moins d'appels `read()` → Plus rapide, mais utilise plus de mémoire

### Version bonus (Multiple FD)

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c -o gnl_bonus
```

## Utilisation

### Exemple de base

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *line;

    fd = open("exemple.txt", O_RDONLY);
    if (fd == -1)
        return (1);

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);  // N'oubliez pas de libérer !
    }

    close(fd);
    return (0);
}
```

### Lecture depuis stdin

```c
#include "get_next_line.h"
#include <stdio.h>

int main(void)
{
    char *line;

    while ((line = get_next_line(0)) != NULL)  // 0 = stdin
    {
        printf("Vous avez saisi : %s", line);
        free(line);
    }
    return (0);
}
```

### Exemple avec fichier.txt

**Fichier d'entrée (fichier.txt) :**
```
Bonjour le monde !
Ceci est la ligne 2
Et la ligne 3
```

**Sortie :**
```
Bonjour le monde !
Ceci est la ligne 2
Et la ligne 3
```

**Notes importantes :**
- Chaque ligne inclut le `\n` de fin (sauf la dernière ligne si elle ne se termine pas par `\n`)
- Toujours faire `free()` sur la ligne retournée pour éviter les fuites mémoire
- Retourne `NULL` quand EOF est atteint ou qu'une erreur survient

## Structure du projet

```
get_next_line/
├── get_next_line.h              # Fichier d'en-tête (prototypes, defines)
├── get_next_line.c              # Implémentation de la fonction principale
├── get_next_line_utils.c        # Fonctions auxiliaires (strlen, strjoin, etc.)
│
├── get_next_line_bonus.h        # En-tête bonus (support multi-fd)
├── get_next_line_bonus.c        # Implémentation bonus
└── get_next_line_utils_bonus.c  # Utilitaires bonus
```

### Description des fichiers

**Fichiers obligatoires :**
- `get_next_line.h` - Contient les prototypes et la définition de BUFFER_SIZE
- `get_next_line.c` - Logique principale avec variable statique pour un seul fd
- `get_next_line_utils.c` - Fonctions utilitaires (`ft_strlen`, `ft_strjoin`, `ft_strdup`)

**Fichiers bonus :**
- Utilise un tableau de buffers statiques pour gérer plusieurs descripteurs de fichiers simultanément
- Supporte jusqu'à `MAX_FD` (1024) descripteurs de fichiers concurrents

## Fonctionnalités bonus

### Plusieurs descripteurs de fichiers

L'implémentation bonus peut gérer plusieurs descripteurs de fichiers en même temps sans perdre l'état de lecture de chacun.

**Comment ça fonctionne :**

```c
static char *buffer[MAX_FD];  // Tableau de buffers, un par fd
```

**Exemple :**

```c
int fd1 = open("fichier1.txt", O_RDONLY);
int fd2 = open("fichier2.txt", O_RDONLY);
int fd3 = open("fichier3.txt", O_RDONLY);

char *line1 = get_next_line(fd1);  // Lit depuis fichier1
char *line2 = get_next_line(fd2);  // Lit depuis fichier2
char *line3 = get_next_line(fd1);  // Lit depuis fichier1 encore (ligne suivante)
char *line4 = get_next_line(fd3);  // Lit depuis fichier3

// Chaque fd maintient son propre état
free(line1);
free(line2);
free(line3);
free(line4);
```

C'est utile pour :
- Lire plusieurs fichiers simultanément
- Comparer des fichiers ligne par ligne
- Opérations d'E/S multiplexées

## Tests

### Tester avec différentes valeurs de BUFFER_SIZE

```bash
# Cas limite : 1 octet à la fois
cc -D BUFFER_SIZE=1 get_next_line.c get_next_line_utils.c main.c -o gnl && ./gnl

# Taille standard
cc -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl && ./gnl

# Grand buffer
cc -D BUFFER_SIZE=9999 get_next_line.c get_next_line_utils.c main.c -o gnl && ./gnl
```

### Cas limites à tester

- Fichiers vides
- Fichiers sans retour à la ligne final
- Fichiers avec une seule ligne
- Lignes très longues (plus longues que BUFFER_SIZE)
- Fichiers binaires (comportement indéfini, mais ne devrait pas crasher)
- Descripteurs de fichiers invalides
- Lecture depuis stdin

### Testeurs recommandés

- [Tripouille/gnlTester](https://github.com/Tripouille/gnlTester)
- [xicodomingues/francinette](https://github.com/xicodomingues/francinette)
- Créez vos propres tests pour mieux comprendre la fonction !

## Ressources

### Documentation & Tutoriels

- [man read(2)](https://man7.org/linux/man-pages/man2/read.2.html) - Documentation de l'appel système read
- [man open(2)](https://man7.org/linux/man-pages/man2/open.2.html) - Gestion des descripteurs de fichiers
- [Variables statiques en C](https://www.geeksforgeeks.org/static-variables-in-c/) - Comprendre le stockage statique
- [E/S fichiers en C](https://www.gnu.org/software/libc/manual/html_node/I_002fO-Overview.html) - Documentation de la bibliothèque C GNU

### Concepts clés appris

- **Variables statiques** et leur cycle de vie
- Gestion des **descripteurs de fichiers** dans Unix/Linux
- **Gestion de la mémoire** et prévention des fuites
- Stratégies de **gestion de buffer**
- **Appels système** : `read()`, `open()`, `close()`

### Déclaration d'utilisation de l'IA

Ce projet a été complété sans assistance IA, suivant la philosophie de l'école 42 d'apprentissage par la collaboration entre pairs et la recherche personnelle. L'implémentation a été développée à travers :
- Lecture de la documentation officielle (pages man)
- Apprentissage pair-à-pair et revues de code
- Essais et erreurs avec différents cas de test
- Compréhension des concepts sous-jacents par la pratique

---

**Fait pour l'école 42**

Cette implémentation réussit tous les tests obligatoires et bonus, gère les cas limites et est entièrement conforme à la Norme 42.

Si vous trouvez cela utile, n'hésitez pas à mettre une étoile au dépôt ! Bonne chance pour votre propre implémentation.
