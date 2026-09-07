# Image Steganography using LSB Encoding (C)

A command-line tool written in C that hides (encodes) and extracts (decodes) secret text data inside `.bmp` images using the **Least Significant Bit (LSB)** steganography technique. Each bit of the secret data is embedded into the least significant bit of the image's pixel byte data, making the change visually imperceptible.

## Features

- Encode a secret text file into a `.bmp` image
- Decode and extract the hidden secret data from a stego image
- Input validation for file extensions (`.bmp` for images, `.txt` for secret file)
- Capacity check to ensure the image can hold the secret data before encoding
- Modular code structure with separate headers for encoding, decoding, and common utilities

## Project Structure

```
Gaurav_Ambolikar_/
├── common.h        # Shared macros, structs, and status codes
├── decode.h        # Decoding function declarations
├── encode.h        # Encoding function declarations
├── types.h         # Common type definitions (e.g. Status enum)
├── encode.c        # Encoding logic (LSB embedding)
├── decode.c        # Decoding logic (LSB extraction)
├── main.c          # Entry point — parses CLI args, dispatches encode/decode
├── beautiful.bmp   # Sample cover image
└── secret.txt      # Sample secret message file
```

All source files, headers, and sample files sit together in one folder for simplicity, so the binary can reference them without relative paths.

## How It Works

**Encoding:**
1. Validates the source `.bmp` and secret `.txt` file extensions
2. Reads the BMP header and pixel data
3. Checks whether the image has enough capacity to hold the secret data (magic string + file size + extension + data)
4. Embeds a **magic string** (a defined macro) first, followed by the secret file extension size, extension, file size, and the actual secret data — each bit embedded into the LSB of successive image bytes
5. Writes the result to a new stego `.bmp` image

**Decoding:**
1. Reads the stego `.bmp` image
2. Extracts the LSBs of the pixel bytes to first read back the **magic string** and verify it matches the expected value — this confirms the image actually contains encoded data before proceeding
3. If the magic string check passes, continues extracting the secret file extension, file size, and secret data byte by byte
4. Writes the recovered secret data to an output file

### Magic String

A magic string constant (defined in `common.h`) is embedded at the very start of the encoded data. During decoding, this string is the first thing extracted and compared against the expected value — if it doesn't match, decoding stops immediately, since the image either isn't a stego image or wasn't encoded by this tool.

## Prerequisites

- GCC (or any C compiler)
- Linux / WSL environment (developed and tested on Ubuntu 24.04 via WSL)

## Build

From inside the project folder:

```bash
gcc -o encode_decode main.c encode.c decode.c
```

## Usage

**Encoding a secret file into an image:**

```bash
./encode_decode -e beautiful.bmp secret.txt stego.bmp
```

- `beautiful.bmp` — source cover image
- `secret.txt` — file containing the secret message
- `stego.bmp` — (optional) output image name; defaults to `stego_img.bmp` if omitted

**Decoding a secret file from a stego image:**

```bash
./encode_decode -d stego.bmp output.txt
```

- `stego.bmp` — image containing hidden data (generated in the previous step)
- `output.txt` — (optional) output file for the recovered secret; defaults to `decode.txt` if omitted

> Note: Update the exact flag names/argument order above if your `main.c` parses them differently — adjust this section to match your actual `argv` handling.

## Sample Files

The project folder includes a sample `beautiful.bmp` cover image and a `secret.txt` message so you can test encoding/decoding right away.

## Author

Gaurav Ambolikar — Final Year B.Tech, Electronics and Telecommunication Engineering, YCCE Nagpur
