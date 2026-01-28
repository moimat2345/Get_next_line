# get_next_line

> Reading a line from a file descriptor is way too tedious.

[![42 Project](https://img.shields.io/badge/42-Project-00babc?style=flat-square&logo=42)](https://42.fr)
[![Language](https://img.shields.io/badge/Language-C-blue?style=flat-square&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Norminette](https://img.shields.io/badge/Norminette-passing-success?style=flat-square)](https://github.com/42School/norminette)

[Français](README.fr.md) | **English**

## Description

**get_next_line** is a C function that reads and returns a line from a file descriptor, one line at a time. This project is part of the 42 School curriculum and introduces an important concept in C programming: **static variables**.

The function works with any file descriptor (files, stdin, sockets) and can handle variable buffer sizes, making it a versatile tool for file I/O operations.

## Table of Contents

- [Features](#features)
- [Algorithm & Technical Approach](#algorithm--technical-approach)
- [Installation & Compilation](#installation--compilation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Bonus Features](#bonus-features)
- [Testing](#testing)
- [Resources](#resources)

## Features

- Read text files line by line from any file descriptor
- Handle files of any size with configurable buffer
- Memory-efficient: reads only what's necessary
- Works with stdin, files, and sockets
- **Bonus**: Support for multiple file descriptors simultaneously
- No memory leaks, fully norminette compliant

## Algorithm & Technical Approach

### Core Algorithm

The function uses a **static buffer** to persist data between calls. This is the key concept that makes line-by-line reading possible:

1. **Read Phase**: Read `BUFFER_SIZE` bytes from the file descriptor
2. **Accumulate**: Store the data in a static buffer that persists between function calls
3. **Search**: Look for a newline character (`\n`) in the buffer
4. **Extract**: When a newline is found, extract and return the complete line
5. **Update**: Keep the remaining data in the buffer for the next call

### Why Static Variables?

```c
char *get_next_line(int fd)
{
    static char *buffer;  // Persists between function calls
    // ...
}
```

Without static variables, the buffer would be destroyed after each function call, losing any partial data read. Static variables allow us to:
- Remember leftover data from previous reads
- Avoid re-reading the same data
- Handle lines larger than `BUFFER_SIZE`

### Data Flow

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

### Key Functions

| Function | Purpose |
|----------|---------|
| `get_next_line(int fd)` | Main function: coordinates reading and returns one line |
| `read_file(fd, buffer)` | Reads from fd until a newline is found or EOF |
| `find_newline(buffer)` | Searches for `\n` character, returns position or -1 |
| `extract_line(buffer)` | Creates and returns the line (including `\n`) |
| `update_buffer(buffer)` | Removes returned line, keeps remaining data |

### Memory Management

The implementation carefully manages memory:
- All allocated memory is freed when no longer needed
- The buffer is freed when EOF is reached or an error occurs
- Each returned line must be freed by the caller

## Installation & Compilation

### Basic Compilation

```bash
# Clone the repository
git clone https://github.com/yourusername/get_next_line.git
cd get_next_line

# Compile with default BUFFER_SIZE (42)
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
```

### Custom BUFFER_SIZE

The buffer size determines how many bytes are read per `read()` system call:

```bash
# Small buffer (tests edge cases)
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 get_next_line.c get_next_line_utils.c main.c -o gnl

# Large buffer (better performance)
cc -Wall -Wextra -Werror -D BUFFER_SIZE=4096 get_next_line.c get_next_line_utils.c main.c -o gnl
```

**Performance Impact:**
- **Smaller buffer** → More `read()` calls → Slower, but tests edge cases
- **Larger buffer** → Fewer `read()` calls → Faster, but uses more memory

### Bonus Version (Multiple FD)

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main.c -o gnl_bonus
```

## Usage

### Basic Example

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

### Example with file.txt

**Input file (file.txt):**
```
Hello World!
This is line 2
And line 3
```

**Output:**
```
Hello World!
This is line 2
And line 3
```

**Important Notes:**
- Each line includes the terminating `\n` (except the last line if it doesn't end with `\n`)
- Always `free()` the returned line to avoid memory leaks
- Returns `NULL` when EOF is reached or an error occurs

## Project Structure

```
get_next_line/
├── get_next_line.h              # Header file (prototypes, defines)
├── get_next_line.c              # Main function implementation
├── get_next_line_utils.c        # Helper functions (strlen, strjoin, etc.)
│
├── get_next_line_bonus.h        # Bonus header (multi-fd support)
├── get_next_line_bonus.c        # Bonus implementation
└── get_next_line_utils_bonus.c  # Bonus utilities
```

### File Descriptions

**Mandatory files:**
- `get_next_line.h` - Contains function prototypes and BUFFER_SIZE definition
- `get_next_line.c` - Core logic with static variable for single fd
- `get_next_line_utils.c` - Utility functions (`ft_strlen`, `ft_strjoin`, `ft_strdup`)

**Bonus files:**
- Uses an array of static buffers to handle multiple file descriptors simultaneously
- Supports up to `MAX_FD` (1024) concurrent file descriptors

## Bonus Features

### Multiple File Descriptors

The bonus implementation can handle multiple file descriptors at the same time without losing track of each one's reading state.

**How it works:**

```c
static char *buffer[MAX_FD];  // Array of buffers, one per fd
```

**Example:**

```c
int fd1 = open("file1.txt", O_RDONLY);
int fd2 = open("file2.txt", O_RDONLY);
int fd3 = open("file3.txt", O_RDONLY);

char *line1 = get_next_line(fd1);  // Read from file1
char *line2 = get_next_line(fd2);  // Read from file2
char *line3 = get_next_line(fd1);  // Read from file1 again (next line)
char *line4 = get_next_line(fd3);  // Read from file3

// Each fd maintains its own state
free(line1);
free(line2);
free(line3);
free(line4);
```

This is useful for:
- Reading from multiple files simultaneously
- Comparing files line by line
- Multiplexed I/O operations

## Testing

### Test with Different BUFFER_SIZE Values

```bash
# Edge case: 1 byte at a time
cc -D BUFFER_SIZE=1 get_next_line.c get_next_line_utils.c main.c -o gnl && ./gnl

# Standard size
cc -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl && ./gnl

# Large buffer
cc -D BUFFER_SIZE=9999 get_next_line.c get_next_line_utils.c main.c -o gnl && ./gnl
```

### Edge Cases to Test

- Empty files
- Files without a final newline
- Files with only one line
- Very long lines (longer than BUFFER_SIZE)
- Binary files (undefined behavior, but shouldn't crash)
- Invalid file descriptors
- Reading from stdin

### Recommended Testers

- [Tripouille/gnlTester](https://github.com/Tripouille/gnlTester)
- [xicodomingues/francinette](https://github.com/xicodomingues/francinette)
- Create your own tests to understand the function better!

## Resources

### Documentation & Tutorials

- [man read(2)](https://man7.org/linux/man-pages/man2/read.2.html) - Read system call documentation
- [man open(2)](https://man7.org/linux/man-pages/man2/open.2.html) - File descriptor management
- [Static variables in C](https://www.geeksforgeeks.org/static-variables-in-c/) - Understanding static storage
- [File I/O in C](https://www.gnu.org/software/libc/manual/html_node/I_002fO-Overview.html) - GNU C Library documentation

### Key Concepts Learned

- **Static variables** and their lifecycle
- **File descriptor** management in Unix/Linux
- **Memory management** and leak prevention
- **Buffer management** strategies
- **System calls**: `read()`, `open()`, `close()`

### AI Usage Declaration

This project was completed without AI assistance, following the 42 School philosophy of learning through peer collaboration and personal research. The implementation was developed through:
- Reading official documentation (man pages)
- Peer-to-peer learning and code reviews
- Trial and error with various test cases
- Understanding the underlying concepts through practice

---

**Made for 42 School**

This implementation passes all mandatory and bonus requirements, handles edge cases, and is fully compliant with the 42 Norm.

If you find this helpful, feel free to star the repository! Good luck with your own implementation.
