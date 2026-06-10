# Menu Driven 2D Graphics Editor in C

This program implements a menu-driven 2D graphics editor using a 2D character array as the canvas.

## Features

- Canvas is stored using a 2D character array.
- Empty spaces are represented using `_`.
- Graphical objects are drawn using `*`.
- Supports:
  - Circle
  - Rectangle
  - Line
  - Triangle
- User can:
  - Add objects
  - Delete objects
  - Modify objects
  - Display the picture
  - Clear all objects
  - List all objects

## Canvas Size

The canvas size is 25 rows × 60 columns.

Coordinates:
- `x` means column number
- `y` means row number

## Compile and Run

### Linux / Mac

```bash
gcc graphics_editor.c -o graphics_editor
./graphics_editor
```

### Windows MinGW

```bash
gcc graphics_editor.c -o graphics_editor.exe
graphics_editor.exe
```

## GitHub Submission Steps

```bash
git init
git add .
git commit -m "Initial commit - 2D graphics editor"
git branch -M main
git remote add origin https://github.com/YOUR_USERNAME/YOUR_REPO_NAME.git
git push -u origin main
```

