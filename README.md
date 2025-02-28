# Elm Engine [![License](https://img.shields.io/github/license/JonasAlmaas/Elm.svg)](https://github.com/JonasAlmaas/Elm/blob/main/LICENSE)

Source code for the Elm Engine

## Getting started

### 1. Cloning the repository

Start by cloning the repository with `git clone --recursive https://github.com/JonasAlmaas/Elm.git`

If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

### 2. Generation build

This project uses [Premake](https://premake.github.io/) to generate build files. Here is an example of how to generate a Visual Studio solution:

```bash
premake5 vs2022
```
