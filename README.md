<div align="center">
  <img src="https://raw.githubusercontent.com/ayogun/42-project-badges/main/covers/cover-get_next_line.png" alt="get_next_line" />
</div>

# get_next_line - Reading Lines Made Easy

The `get_next_line` project consists of creating a function that reads and returns a line from a file descriptor, one line at a time. This project introduces a key concept in C programming: **static variables**.

## Status
<div align="center">

![42 Bangkok](https://img.shields.io/badge/42-Bangkok-000000?style=for-the-badge&logo=42&logoColor=white)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Norminette](https://img.shields.io/badge/Norminette-passing-success?style=for-the-badge)

<br>

### 🌍 Language / Langue

[![Français](https://img.shields.io/badge/🇫🇷_Lire_en_Français-blue?style=for-the-badge&logoColor=white)](README.fr.md)

</div>

---

## Overview

The goal is to create a function `get_next_line()` that reads a line from a file descriptor each time it is called. This exercise improves understanding of static variables, file descriptor management, and dynamic memory allocation in C.

## Useful Resources

- [man read(2)](https://man7.org/linux/man-pages/man2/read.2.html) - Read system call documentation
- [Static variables in C](https://www.geeksforgeeks.org/static-variables-in-c/) - Understanding static storage
- [GNL Tester - Tripouille](https://github.com/Tripouille/gnlTester)
- [Francinette Tester](https://github.com/xicodomingues/francinette)

---

## 📋 Mandatory Part

<table>
  <tr>
    <th>Function name</th>
    <td>get_next_line</td>
  </tr>
  <tr>
    <th>Turn in files</th>
    <td>get_next_line.c, get_next_line_utils.c, get_next_line.h</td>
  </tr>
  <tr>
    <th>Makefile</th>
    <td>No</td>
  </tr>
  <tr>
    <th>External functs.</th>
    <td>read, malloc, free</td>
  </tr>
  <tr>
    <th>Libft authorized</th>
    <td>No</td>
  </tr>
  <tr>
    <th>Description</th>
    <td>Write a function that returns a line read from a file descriptor.</td>
  </tr>
</table>

### Important Rules
- The function must compile with the flag `-D BUFFER_SIZE=n`.
- Global variables are forbidden.
- All .c files must compile with the flags `-Wall -Wextra -Werror`.
- The returned line must include the terminating `\n` (except at EOF without `\n`).

---

## 📂 Project Structure

```
get_next_line/
├── 📄 get_next_line.h              # Header (prototypes + defines)
├── 📄 get_next_line.c              # Main function + reading logic
├── 📄 get_next_line_utils.c        # Utility functions (strlen, strjoin, etc.)
│
├── 📄 get_next_line_bonus.h        # Bonus header (multi-fd support)
├── 📄 get_next_line_bonus.c        # Bonus implementation
└── 📄 get_next_line_utils_bonus.c  # Bonus utilities
```

---

## 🎯 Key Functions

<details open>
<summary><b>Click to see functions</b></summary>

| Function | Purpose | Description |
|:------:|:-----|:------------|
| **get_next_line(fd)** | Main function | Coordinates reading and returns one line |
| **read_file(fd, buffer)** | Read phase | Reads from fd until a newline is found or EOF |
| **find_newline(buffer)** | Search phase | Searches for `\n`, returns position or -1 |
| **extract_line(buffer)** | Extract phase | Creates and returns the line (including `\n`) |
| **update_buffer(buffer)** | Update phase | Removes returned line, keeps remaining data |

</details>

---

## 🔄 Algorithm & Data Flow

<details>
<summary><b>🔹 How Static Variables Work</b></summary>

The function uses a **static buffer** to persist data between calls:

```c
char *get_next_line(int fd)
{
    static char *buffer;  // Persists between function calls
    // ...
}
```

Without static variables, the buffer would be destroyed after each call, losing any partial data read.

</details>

<details>
<summary><b>🔹 Data Flow Diagram</b></summary>

```
┌─────────────┐
│    File     │  "Hello World!\nThis is line 2\n..."
└──────┬──────┘
       │ read(fd, temp, BUFFER_SIZE)
       ↓
┌─────────────┐
│   Buffer    │  "Hello World!\nThis"  (static, persists)
└──────┬──────┘
       │ find_newline() → position 12
       ↓
┌─────────────┐
│ extract_line│  Returns: "Hello World!\n"
└─────────────┘
       │
┌─────────────┐
│update_buffer│  Buffer now: "This"  (ready for next call)
└─────────────┘
```

</details>

<details>
<summary><b>🔹 Memory Management</b></summary>

- All allocated memory is freed when no longer needed
- The buffer is freed when EOF is reached or an error occurs
- Each returned line **must be freed by the caller**

</details>

---

## 🛠️ Compilation and Usage

### Compile

```bash
# Compile with default BUFFER_SIZE
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl

# Small buffer (tests edge cases)
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 get_next_line.c get_next_line_utils.c main.c -o gnl

# Large buffer (better performance)
cc -Wall -Wextra -Werror -D BUFFER_SIZE=4096 get_next_line.c get_next_line_utils.c main.c -o gnl
```

### Use in Your Code

```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int     fd;
    char    *line;

    fd = open("example.txt", O_RDONLY);
    if (fd == -1)
        return (1);

    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);  // Don't forget to free!
    }

    close(fd);
    return (0);
}
```

### Reading from stdin

```c
#include "get_next_line.h"
#include <stdio.h>

int main(void)
{
    char *line;

    while ((line = get_next_line(0)) != NULL)  // 0 = stdin
    {
        printf("You entered: %s", line);
        free(line);
    }
    return (0);
}
```

---

## 🌟 Bonus Part

<details>
<summary><b>🔹 Multiple File Descriptors</b></summary>

The bonus implementation handles multiple file descriptors simultaneously without losing track of each one's reading state:

```c
static char *buffer[MAX_FD];  // Array of buffers, one per fd
```

**Example:**

```c
int fd1 = open("file1.txt", O_RDONLY);
int fd2 = open("file2.txt", O_RDONLY);

char *line1 = get_next_line(fd1);  // Read from file1
char *line2 = get_next_line(fd2);  // Read from file2
char *line3 = get_next_line(fd1);  // Read from file1 again (next line)

// Each fd maintains its own state
free(line1);
free(line2);
free(line3);
```

Compile bonus version:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c -o gnl_bonus
```

</details>

---

## ⚡ BUFFER_SIZE Impact

<div align="center">

| BUFFER_SIZE | read() calls | Speed | Memory |
|:-----------:|:------------:|:-----:|:------:|
| **1** | Many | Slow | Low |
| **42** | Moderate | Normal | Normal |
| **4096** | Few | Fast | Higher |

</div>

---

## 📊 Statistics

<div align="center">

| Metric | Value |
|:------:|:-----:|
| **Files** | 6 .c + 2 .h |
| **Key concept** | Static variables |
| **Bonus** | Yes (multi-fd) |

</div>

---

## 🚨 Disclaimer

**🇬🇧 This project has an educational purpose and you should under no circumstances copy and paste.**
**Cheat is bad. Don't cheat.**

**🇫🇷 Ce projet a un but éducatif et vous ne devez en aucun cas faire du copier-coller.**
**Tricher c'est mal. Ne trichez pas.**

---

<div align="center">

**Made with ❤️ for 42 Bangkok**

[![Back to top](https://img.shields.io/badge/⬆-Back_to_top-red?style=for-the-badge)](#get_next_line---reading-lines-made-easy)

</div>
