# C Shell README

## Table of Contents
1. [Introduction](#introduction)
2. [Installation](#installation)
3. [Usage](#usage)
4. [Functionality](#functionality)
    - [Basic System Calls](#basic-system-calls)
    - [Processes, Files, and Misc.](#processes-files-and-misc)
    - [Networking](#networking)
5. [Limitations and Assumptions](#limitations-and-assumptions)


## Introduction

This is a basic shell implementation in C programming language with various functionalities similar to a typical Unix shell. It supports basic system calls, process management, file operations, networking, and more. The shell provides a command-line interface for users to interact with their system.

## Installation

To install the shell, simply compile the source code provided and run the executable. Ensure you have the necessary libraries installed as mentioned in the compilation instructions.

## Usage

Use the makefile to generate the terminal binary. Then simply run it.

## Functionality

### Basic System Calls

#### Display Requirement
- The shell prompt displays the username, system name, and current directory address.
- The directory address changes dynamically based on the user's current working directory.

#### Input Requirements
- The shell supports commands separated by ';' or '&' symbols.
- It accounts for random spaces and tabs in user input.

#### warp
- Changes the shell's working directory.
- Supports absolute, relative, and symbolic paths.
- Special symbols like '~', '.', '..', and '-' are supported.

    **Command Syntax:** `warp [directory]`

#### peek
- Lists files and directories in specified directories with options for displaying hidden files and extra information.
- Supports relative and absolute paths.
- Special symbols like '~', '.', '..', and '-' are supported.
- Differentiates between files, directories, and executables using color coding.

    **Command Syntax:** `peek [flags] [path/name]`

#### pastevents commands
- Records and retrieves the 15 most recent command statements.
- Supports executing past commands.
- Supports purging command history.

    **Command Syntax:** `pastevents [execute <index>] | [purge]`

#### System commands
- Executes other system commands present in Bash.
- Supports foreground and background processes.

#### proclore
- Displays information about a process, including PID, process status, process group, virtual memory, and executable path.

    **Command Syntax:** `proclore [pid]`

#### seek
- Searches for files/directories in the specified target directory.
- Supports flags for filtering by file type and for executing a single result.

    **Command Syntax:** `seek [flags] [search] [target_directory]`

### Processes, Files, and Misc.

#### I/O Redirection
- Redirects input and output for commands.
- Supports '>' (overwrite), '>>' (append), and '<' (input).

    **Command Syntax:** `<command> [<arguments>] [> output_file] [| <command> ...]`

#### Pipes
- Passes information between commands.
- Supports any number of pipes.

    **Command Syntax:** `<command> [<arguments>] [| <command> ...]`

#### Redirection along with pipes
- Supports combining I/O redirection with pipes.

    **Command Syntax:** `<command> [<arguments>] [> output_file] [| <command> ...]`

#### activities
- Lists all running processes spawned by the shell.
- Displays command name, PID, and state.

#### Signals
- Sends signals to processes.
- Supports common signals like SIGKILL and SIGTERM.

    **Command Syntax:** `ping <pid> <signal_number>`

#### fg and bg
- Brings background processes to foreground and vice versa.

    **Command Syntax:** `fg <pid> | bg <pid>`

#### Neonate
- Prints the PID of the most recently created process at regular intervals until 'x' is pressed.

    **Command Syntax:** `neonate -n [time_arg]`

### Networking

#### iMan
- Fetches man pages from http://man.he.net/.
- Prints the name, synopsis, and description of the specified command.

    **Command Syntax:** `iMan <command_name>`

## Limitations and Assumptions

- Paths/names should not contain whitespace characters.
- Certain features like background process handling for user-defined commands are not implemented.
- Assumptions are documented in the README for clarity.

