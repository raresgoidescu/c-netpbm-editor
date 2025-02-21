# C NetPBM Editor

Welcome to the C NetPBM Editor! This project is a lightweight image processing tool written in C, designed to handle NetPBM (Portable Anymap) file formats. The primary focus of this editor is to provide basic image processing functionalities such as equalization, filtering, cropping,

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Commands](#commands)
- [License](#license)

## Features

- **Equalization:** Enhance the contrast of images by spreading out the most frequent intensity values.
- **Filters:** Apply various filters to images to achieve different effects.
- **Histogram:** Analyze and manipulate the histogram of images for better image processing.
- **Rotation:** Rotate images by 90, 180, or 270 degrees left/right.
- **Cropping:** Crop images to a specified region.
- **Save:** Save images in plain text or raw format.

## Getting Started

### Prerequisites

To get started with the C NetPBM Editor, you need to have the following prerequisites installed on your system:

- GCC (GNU Compiler Collection)
- Make

### Installation

1. Clone the repository:
    ```bash
    git clone https://github.com/raresgoidescu/c-netpbm-editor.git
    ```
2. Navigate to the project directory:
    ```bash
    cd c-netpbm-editor
    ```
3. Build the project:
    ```bash
    make
    ```

## Usage

After building the project, run the editor using the following command:
```bash
./image_editor
```

Once the editor is running, you can enter commands in the application shell to perform various image processing tasks.

## Commands

### LOAD
Load an image into the editor.
```shell
LOAD <file_path>
```
- `<file_path>`: Path to the image file to be loaded.

### SELECT
Select a region of the image.
```shell
SELECT <x1> <y1> <x2> <y2>
```
- `<x1> <y1>`: Coordinates of the top-left corner.
- `<x2> <y2>`: Coordinates of the bottom-right corner.

### HISTOGRAM
Display the histogram of the selected region (only works on black & white).
```shell
HISTOGRAM <y-scale> <n-bins>
```

### EQUALIZE
Equalize the image to enhance contrast.
```shell
EQUALIZE
```

### APPLY
Apply a filter to the selected region.
```shell
APPLY <filter_name>
```
- `<filter_name>`: Name of the filter to apply EDGE|SHARPEN|BLUR|GAUSSIAN_BLUR.

### ROTATE
Rotate the image by a specified angle.
```shell
ROTATE <angle>
```
- `<angle>`: Angle to rotate the image +|- 90|180|270 degrees.

### CROP
Crop the image to the selected region.
```shell
CROP
```

### SAVE
Save the edited image to a file.
```shell
SAVE <file_path> [ascii]
```
- `<file_path>`: Path to the file where the image will be saved.
- `[ascii]`: Format to save the image (optional, default is raw).

### EXIT
Exit the editor.
```shell
EXIT
```

## Encoding RGB in a uint

For grayscale images, handling is straightforward. However, color images with three channels (RGB) complicate things slightly. To manage this, we use a technique known as packing, which involves bitwise operations and the system's endian type (Little-Endian in this case).

### Packing

The maximum encountered value is 255, which fits in 8 bits. An `unsigned int` has a size of 32 bits, allowing us to store four 8-bit values (alpha, blue, green, red) in a single `unsigned int`.

In this approach, alpha is placed in the most significant position (Little-Endian). The "value" of a pixel is the result of left shifts and bitwise OR operations.

### Unpacking

Unpacking is the reverse operation, retrieving the specific RGB channel values using right shifts and bitwise AND operations.

This approach provides several advantages:
- **Efficient Memory Use:** Especially for large images, separate storage for color channels can be inefficient.
- **Faster Processing:** Bitwise operations on a single integer tend to be quicker than operations on multiple separate variables.
- **Reduced Complexity:** Simplifies the program's structure without increasing big-O complexity.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
