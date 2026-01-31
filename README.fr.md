<div align="center">
  <img src="https://raw.githubusercontent.com/ayogun/42-project-badges/main/covers/cover-get_next_line.png" alt="get_next_line" />
</div>

# get_next_line - Lire Ligne par Ligne

Le projet `get_next_line` consiste à créer une fonction qui lit et retourne une ligne depuis un descripteur de fichier, une ligne à la fois. Ce projet introduit un concept clé en programmation C : les **variables statiques**.

## Statut
<div align="center">

![42 Bangkok](https://img.shields.io/badge/42-Bangkok-000000?style=for-the-badge&logo=42&logoColor=white)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Norminette](https://img.shields.io/badge/Norminette-passing-success?style=for-the-badge)

<br>

### 🌍 Language / Langue

[![English](https://img.shields.io/badge/🇬🇧_Read_in_English-red?style=for-the-badge&logoColor=white)](README.md)

</div>

---

## Vue d'ensemble

L'objectif est de créer une fonction `get_next_line()` qui lit une ligne depuis un descripteur de fichier à chaque appel. Cet exercice améliore la compréhension des variables statiques, de la gestion des descripteurs de fichiers et de l'allocation dynamique de mémoire en C.

## Ressources utiles

- [man read(2)](https://man7.org/linux/man-pages/man2/read.2.html) - Documentation de l'appel système read
- [Variables statiques en C](https://www.geeksforgeeks.org/static-variables-in-c/) - Comprendre le stockage statique
- [GNL Tester - Tripouille](https://github.com/Tripouille/gnlTester)
- [Francinette Tester](https://github.com/xicodomingues/francinette)

---

## 📋 Partie Obligatoire

<table>
  <tr>
    <th>Nom de la fonction</th>
    <td>get_next_line</td>
  </tr>
  <tr>
    <th>Fichiers à rendre</th>
    <td>get_next_line.c, get_next_line_utils.c, get_next_line.h</td>
  </tr>
  <tr>
    <th>Makefile</th>
    <td>Non</td>
  </tr>
  <tr>
    <th>Fonctions externes</th>
    <td>read, malloc, free</td>
  </tr>
  <tr>
    <th>Libft autorisée</th>
    <td>Non</td>
  </tr>
  <tr>
    <th>Description</th>
    <td>Écrire une fonction qui retourne une ligne lue depuis un descripteur de fichier.</td>
  </tr>
</table>

### Règles importantes
- La fonction doit compiler avec le flag `-D BUFFER_SIZE=n`.
- Les variables globales sont interdites.
- Tous les fichiers .c doivent compiler avec les flags `-Wall -Wextra -Werror`.
- La ligne retournée doit inclure le `\n` final (sauf en fin de fichier sans `\n`).

---

## 📂 Structure du Projet

```
get_next_line/
├── 📄 get_next_line.h              # Header (prototypes + defines)
├── 📄 get_next_line.c              # Fonction principale + logique de lecture
├── 📄 get_next_line_utils.c        # Fonctions utilitaires (strlen, strjoin, etc.)
│
├── 📄 get_next_line_bonus.h        # Header bonus (support multi-fd)
├── 📄 get_next_line_bonus.c        # Implémentation bonus
└── 📄 get_next_line_utils_bonus.c  # Utilitaires bonus
```

---

## 🎯 Fonctions Principales

<details open>
<summary><b>Cliquez pour voir les fonctions</b></summary>

| Fonction | Rôle | Description |
|:------:|:-----|:------------|
| **get_next_line(fd)** | Fonction principale | Coordonne la lecture et retourne une ligne |
| **read_file(fd, buffer)** | Phase de lecture | Lit depuis le fd jusqu'à trouver un `\n` ou EOF |
| **find_newline(buffer)** | Phase de recherche | Cherche `\n`, retourne la position ou -1 |
| **extract_line(buffer)** | Phase d'extraction | Crée et retourne la ligne (incluant `\n`) |
| **update_buffer(buffer)** | Phase de mise à jour | Supprime la ligne retournée, garde le reste |

</details>

---

## 🔄 Algorithme & Flux de Données

<details>
<summary><b>🔹 Comment fonctionnent les variables statiques</b></summary>

La fonction utilise un **buffer statique** pour conserver les données entre les appels :

```c
char *get_next_line(int fd)
{
    static char *buffer;  // Persiste entre les appels de fonction
    // ...
}
```

Sans variables statiques, le buffer serait détruit après chaque appel, perdant toutes les données partiellement lues.

</details>

<details>
<summary><b>🔹 Diagramme du flux de données</b></summary>

```
┌─────────────┐
│   Fichier   │  "Hello World!\nCeci est la ligne 2\n..."
└──────┬──────┘
       │ read(fd, temp, BUFFER_SIZE)
       ↓
┌─────────────┐
│   Buffer    │  "Hello World!\nCeci"  (statique, persiste)
└──────┬──────┘
       │ find_newline() → position 12
       ↓
┌─────────────┐
│ extract_line│  Retourne : "Hello World!\n"
└─────────────┘
       │
┌─────────────┐
│update_buffer│  Buffer maintenant : "Ceci"  (prêt pour le prochain appel)
└─────────────┘
```

</details>

<details>
<summary><b>🔹 Gestion de la Mémoire</b></summary>

- Toute la mémoire allouée est libérée quand elle n'est plus nécessaire
- Le buffer est libéré quand EOF est atteint ou qu'une erreur survient
- Chaque ligne retournée **doit être libérée par l'appelant**

</details>

---

## 🛠️ Compilation et Utilisation

### Compiler

```bash
# Compiler avec BUFFER_SIZE par défaut
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl

# Petit buffer (teste les cas limites)
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 get_next_line.c get_next_line_utils.c main.c -o gnl

# Grand buffer (meilleures performances)
cc -Wall -Wextra -Werror -D BUFFER_SIZE=4096 get_next_line.c get_next_line_utils.c main.c -o gnl
```

### Utiliser dans votre code

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
        free(line);  // N'oubliez pas de free !
    }

    close(fd);
    return (0);
}
```

### Lire depuis stdin

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

---

## 🌟 Partie Bonus

<details>
<summary><b>🔹 Descripteurs de fichiers multiples</b></summary>

L'implémentation bonus gère plusieurs descripteurs de fichiers simultanément sans perdre l'état de lecture de chacun :

```c
static char *buffer[MAX_FD];  // Tableau de buffers, un par fd
```

**Exemple :**

```c
int fd1 = open("fichier1.txt", O_RDONLY);
int fd2 = open("fichier2.txt", O_RDONLY);

char *line1 = get_next_line(fd1);  // Lire depuis fichier1
char *line2 = get_next_line(fd2);  // Lire depuis fichier2
char *line3 = get_next_line(fd1);  // Relire depuis fichier1 (ligne suivante)

// Chaque fd conserve son propre état
free(line1);
free(line2);
free(line3);
```

Compiler la version bonus :

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c -o gnl_bonus
```

</details>

---

## ⚡ Impact du BUFFER_SIZE

<div align="center">

| BUFFER_SIZE | Appels read() | Vitesse | Mémoire |
|:-----------:|:-------------:|:-------:|:-------:|
| **1** | Beaucoup | Lent | Faible |
| **42** | Modéré | Normal | Normal |
| **4096** | Peu | Rapide | Plus élevée |

</div>

---

## 📊 Statistiques

<div align="center">

| Métrique | Valeur |
|:--------:|:------:|
| **Fichiers** | 6 .c + 2 .h |
| **Concept clé** | Variables statiques |
| **Bonus** | Oui (multi-fd) |

</div>

---

## 🚨 Avertissement

**🇫🇷 Ce projet a un but éducatif et vous ne devez en aucun cas faire du copier-coller.**
**Tricher c'est mal. Ne trichez pas.**

**🇬🇧 This project has an educational purpose and you should under no circumstances copy and paste.**
**Cheat is bad. Don't cheat.**

---

<div align="center">

**Made with ❤️ for 42 Bangkok**

[![Back to top](https://img.shields.io/badge/⬆-Retour_en_haut-blue?style=for-the-badge)](#get_next_line---lire-ligne-par-ligne)

</div>
