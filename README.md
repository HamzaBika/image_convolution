# C Image Processing Program

A comprehensive C program for basic image processing operations including color channel extraction, edge detection, and Gaussian blur. The program supports both RGB and grayscale image manipulation with various output options.

## Features

- **Color Channel Manipulation**
  - Extract individual RGB channels (Red, Green, Blue)
  - Save individual channels as grayscale images
  - Create random RGB images

- **Edge Detection**
  - Sobel operator implementation
  - Edge detection on individual color channels
  - Support for all RGB components

- **Gaussian Blur**
  - Customizable sigma (standard deviation) value
  - Dynamic kernel generation
  - Full RGB support for blur operations

- **File Operations**
  - Read/Write custom format text files
  - Export to PNG format
  - Save intermediate processing results

## Requirements

- C compiler (GCC recommended)
- `stb_image_write.h` library (included)
- Math library (`-lm` flag required for compilation)

## Compilation

```bash
gcc -o image_processor main.c -lm
```

## Usage

1. Prepare your input image in the required format (see Input Format section)
2. Run the program:
   ```bash
   ./image_processor
   ```
3. Use the interactive menu to select operations:
   - 1: Display and save image
   - 2: Extract color components
   - 3: Edge detection
   - 4: Apply Gaussian blur
   - 6: Create random image
   - 0: Exit

## Input Format

The program expects input files in the following format:
```
resolution width height
(R.G.B) (R.G.B) (R.G.B) ...
(R.G.B) (R.G.B) (R.G.B) ...
...
```
Where:
- First line contains three integers: resolution, width, and height
- Following lines contain RGB triplets in (R.G.B) format
- Each RGB value should be between 0 and 255

## Output Files

The program generates several types of output files:
- `image_originale.png`: Original image in PNG format
- `composante_*.png`: Individual color channel extractions
- `contours_*.png`: Edge detection results
- `flou_gaussien_*.png`: Gaussian blur results
- `image_aleatoire.png`: Randomly generated images

## Data Structures

The program uses two main structures:
```c
// RGB Image structure
typedef struct {
    int r;        // Resolution
    int l;        // Width
    int h;        // Height
    int **img[3]; // RGB matrices
} image;

// Grayscale Image structure
typedef struct {
    int r;        // Resolution
    int l;        // Width
    int h;        // Height
    int **img;    // Grayscale matrix
} grey_image;
```

## Implementation Details

### Edge Detection
- Uses Sobel operators for gradient calculation
- Implements both horizontal and vertical edge detection
- Combines gradients for final edge magnitude

### Gaussian Blur
- Dynamic kernel generation based on sigma value
- 2D convolution implementation
- Border handling for complete image processing

## Memory Management

The program includes proper memory management:
- Dynamic allocation for image matrices
- Proper cleanup of allocated resources
- Memory leak prevention

## Limitations

- Input image format must strictly follow the specified format
- Maximum image size depends on available system memory
- Edge handling in Gaussian blur uses simple border copying

## Contributing

Feel free to contribute to this project by:
1. Fork the repository
2. Create your feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## License

This project is open source and available under the MIT License.
 
