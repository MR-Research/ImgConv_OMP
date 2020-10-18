# Imgconv

Parallel image convolution tool written in C

## Features

 - Wide image format compatibility.
 - Arbitrary image and filter size.
 - Low computational complexity.
 - Grayscale conversion.
 - No external dependencies.
 - Parallel execution
 - Scheduler selection

## Installation

 - Clone the repository.
```bash
git clone https://github.com/MrRobot-oss/ImgConv.git
```
 - (Optional) open the project file in KDevelop using the .kdev4 file
 - Compile using math library and openmp tags.
```bash
gcc main.c -o imgconv -lm -fopenmp
```
 - Copy the binary to your standard binary path.

## Usage

```bash
imgconv image_path filter_path resulting_image_path filter_size thead_count scheduler_type chunk_size
```
## Parameters

 1. image_path: The source image, supported formats are the same as stb image (JPG, PNG, TGA, BMP, PSD, GIF, HDR, PIC).
 2. filter_path: Filter file path, should be a plain text file, where each element is separated by a space, and each row is separated by newline (\n).
 3. resulting_image_path:  Output image file path, the image should be in jpg format.
 4. filter_size: Size of the filter, should be an odd number.
 5. thead_count: Amount of threads used for parallel execution
 6. scheduler_type: Selector for the scheduler type, supported options: static, dynamic, guided and auto
 7. chunk_size: Size for task chunks

## Limitations
The following are the current limitations of the program:
 - The program by default crops the resulting image, to account for the data loss due to the convolution process.
 - Grayscale conversion is performed by default.
 - Supported color channels is 3.
 - Output image is jpg.
 - Only odd number square filters are supported at the moment.

## Known issues
No known issues have been reported so far. If you encounter an issue, please post a new issue so it can be verified and fixed.

## TODO

 - Support more output image formats.
 - Add support for arbitrary number of channels.
 - Make grayscale conversion optional.
 - File sanitization and memory management improvements.
 - S̶u̶p̶p̶o̶r̶t̶ ̶f̶o̶r̶ ̶U̶N̶I̶X̶-̶s̶t̶y̶l̶e̶ ̶a̶r̶g̶u̶m̶e̶n̶t̶ ̶p̶a̶r̶s̶i̶n̶g̶.̶
 - S̶u̶p̶p̶o̶r̶t̶ ̶f̶o̶r̶ ̶s̶t̶a̶n̶d̶a̶r̶d̶ ̶i̶n̶p̶u̶t̶/̶o̶u̶t̶p̶u̶t̶.̶
 - Bindings to other programming languages.
 - Make binary/release version available.
 - Makefile script for easier compilation/installation.

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
This program is distributed under the MIT license, you can read more about it here: [MIT](https://choosealicense.com/licenses/mit/).
The [stb libraries](https://github.com/nothings/stb), developed by [nothings](https://github.com/nothings)  are based on public domain code (licensed under the MIT license).
