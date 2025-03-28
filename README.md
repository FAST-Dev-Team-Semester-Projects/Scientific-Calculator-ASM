# Scientific Calculator in x86 Assembly Language

Welcome to the Scientific Calculator! This project implements a modular and efficient scientific calculator in x86 Assembly language, capable of performing both basic arithmetic and advanced scientific calculations.

## Project Background

This Scientific Calculator was developed as part of a third-semester project for the course "Computer Organization and Assembly Language" at FAST NUCES Karachi. The project demonstrates essential assembly language programming concepts using **MASM** and **Irvine32.lib** and covers both standard and advanced mathematical operations.

## Features

- **Basic Arithmetic**: Addition, Subtraction, Multiplication, and Division
- **Advanced Calculations**: Includes Trigonometric (sine, cosine, tangent), Logarithmic, and Exponential functions
- **Modular Design**: Each calculation type is handled by a dedicated procedure for clarity and reusability
- **Frontend Implementation**: A C++ frontend file interacts with the backend assembly file to handle user input and display results.

## Installation

To compile and run the Scientific Calculator application, follow these steps:

### Prerequisites

Ensure you have the following installed on your system:

- **MASM (Microsoft Macro Assembler)**
- **Irvine32 library** (usually provided with the textbook or source you are using for Assembly language)
- **C++ compiler** (such as MinGW or Visual Studio)

### Setup

1. **Clone the repository**:

    ```bash
    git clone https://github.com/SHaiderM16/Scientific-Calculator.git
    ```

2. **Navigate to the project directory**:

    ```bash
    cd Scientific-Calculator
    ```

3. **Set up Irvine32 library**:

    Make sure you have the `irvine32.lib` file available. You can download it from [Irvine's website](https://www.asmirvine.com/) or use the version provided with your textbook. Place `irvine32.lib` in a directory on your system.

    If the library is located in a different directory, you need to link it when compiling your `.asm` file. For example, if the library is in the directory `C:\Irvine32\lib\`, specify the path during the assembly process like this:

    ```bash
    ml /c /coff Backend.asm
    link /subsystem:console Backend.obj C:\Irvine32\lib\irvine32.lib
    ```

4. **Assemble and link the Assembly code**:

    The project uses a combination of C++ (frontend) and Assembly (backend). The C++ frontend interacts with the assembly backend to handle user input, display results, and perform calculations.

    - **Assemble the backend (`Backend.asm` file)**:

      ```bash
      ml /c /coff Backend.asm
      link /subsystem:console Backend.obj C:\Irvine32\lib\irvine32.lib
      ```

    - **Compile and link the frontend C++ code (`Calculator.cpp` file)**:

      If you're using a C++ compiler like MinGW, run:

      ```bash
      g++ Calculator.cpp -o calculator.exe
      ```

    This will create the executable for the Scientific Calculator.

5. **Run the program**:

    After successful compilation, you can run the program by executing:

    ```bash
    calculator.exe
    ```

### Key Notes

- **Irvine32 library**: The library contains essential functions for handling input/output, memory, and more, commonly used in MASM-based projects.
- The project consists of two parts:
  - **Frontend (`Calculator.cpp`)**: Handles user input and output formatting, and interfaces with the assembly backend.
  - **Backend (`Backend.asm`)**: Implements the core mathematical operations and logic.
- Ensure that the path to the `irvine32.lib` file is correctly specified based on where it is located on your system. You can update the path in the `link` command accordingly.

## Project Structure

- **Calculator.asm**: Main assembly file with modular procedures for each calculation type.
- **Frontend.cpp**: The C++ frontend file that interacts with the backend assembly functions. It handles user input and output formatting.
- **Procedures**:
  - **Arithmetic Procedures**: Handles Addition, Subtraction, Multiplication, Division.
  - **Trigonometric Procedures**: Handles Sine, Cosine, Tangent functions (degree/radian mode).
  - **Logarithmic and Exponential Procedures**: Handles advanced scientific functions.

## License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/SHaiderM16/Scientific-Calculator-ASM/blob/main/LICENSE) file for details.

## Contact

For any questions or feedback, please contact:

- **Syed Haider Murtaza** at [haidermurtaza16@gmail.com](mailto:haidermurtaza16@gmail.com)
- **Muhammad Rayyan** at [imuhammadrayyan@gmail.com](mailto:imuhammadrayyan@gmail.com)
- **Mujtaba Kamran** at [mujtaba.kamran2004@gmail.com](mailto:mujtaba.kamran2004@gmail.com)
