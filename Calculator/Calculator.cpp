// Program Description: This file contains the frontend implementation of the scientific calculator
// It interacts with the backend (.asm file) to perform integer and floating-point arithmetic and scientific operations
// The frontend handles user input handling, displays results, and interfaces with the backend functions for calculations
// Authors: Syed Haider Murtaza, Mujtaba Kamran, Muhammad Rayyan

#include <iostream>  // Provides facilities for input/output operations (e.g., cin, cout)
#include <iomanip>   // Allows formatting of output, such as setting decimal precision
#include <algorithm> // Provides algorithms such as transform for modifying data (e.g., tolower for case-insensitive comparison)
#include <windows.h> // For GetConsoleScreenBufferInfo to enhance UI aesthetics

using namespace std;

// Declare the external assembly functions
extern "C" float addition();
extern "C" float subtraction();
extern "C" float multiplication();
extern "C" float division();

// Declare external assembly functions for advanced operations
extern "C" void trigSin();
extern "C" void trigCos();
extern "C" void trigTan();
extern "C" void exponentiation();
extern "C" void performLn();
extern "C" void squareRoot();
extern "C" float power();
extern "C" void factorial();

const int MAX_SIZE = 100; // Define the maximum size for storing parsed input

// Class to represent and parse a mathematical expression
class Expression
{
public:
    float numbers[MAX_SIZE];  // Stores numbers in the expression
    char operators[MAX_SIZE]; // Stores operators (+, -, *, /)
    int numCount;             // Count of numbers in the expression
    int opCount;              // Count of operators in the expression

    // Constructor that intialises counts to zero
    Expression() : numCount(0), opCount(0) {}
};

// Function to check if character is a digit
bool isDigit(char c)
{
    return (c >= '0' && c <= '9');
}
// Function to check if character is an operator
bool isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '!');
}

// Function to perform basic arithmetic and power operations
float performOperation(float a, float b, char op)
{
    float result;
    switch (op)
    {
    case '+':
        _asm {
            push dword ptr[a]
            push dword ptr[b]
            call addition
            mov dword ptr[result], eax
            add esp, 8
        }
        break;
    case '-':
        _asm {
            push dword ptr[a]
            push dword ptr[b]
            call subtraction
            mov dword ptr[result], eax
            add esp, 8
        }
        break;
    case '*':
        _asm {
            push dword ptr[a]
            push dword ptr[b]
            call multiplication
            mov dword ptr[result], eax
            add esp, 8
        }
        break;
    case '/':
        if (b == 0.0f)
        {
            cout << "\nError: Division by zero!\n\n";
            return 3.402823466e+38f; // Return a sentinel value (maximum 32-bit floating) to indicate an error
        }
        _asm {
            push dword ptr[a]
            push dword ptr[b]
            call division
            mov dword ptr[result], eax
            add esp, 8
        }
        break;
    case '^':
        if (b == 0.5f) // Special case: square root (exponent 0.5)
        {
            float value = static_cast<float>(a); // Convert a to a float
            float tempResult;                    // Temporary variable to store the result

            // Check for negative number under square root
            if (value < 0.0f)
            {
                cout << "\nError: Cannot calculate square root of a negative number\n\n";
                return 3.402823466e+38f; // Return a sentinel value (maximum 32-bit floating) to indicate an error
            }

            _asm {
                fld value // Load the floating - point value from memory(at 'value') onto the top of the FPU stack
                call squareRoot // Call the square root function, which operates on the value at the top of the FPU stack
                fstp tempResult // Store the value from the top of the FPU stack into memory(at 'tempResult') and pop it from the stack
            }

            result = tempResult; // Store the floating-point result
        }
        else // General power function
        {
            int base = static_cast<int>(a);
            int exp = static_cast<int>(b);
            int tempResult; // Temporary variable to store the result

            _asm {
                mov eax, dword ptr[base] // Load base into EAX
                mov edx, dword ptr[exp] // Load exponent into EDX
                call power // Call power function
                mov dword ptr[tempResult], eax // Store the result in tempResult
            }

            result = static_cast<float>(tempResult); // Convert integer result to float
        }
        break;

    default:
        result = 0.0f; // For invalid operators
    }

    return result;
}

int performFactorial(float num)
{
    int intNum = static_cast<int>(num); // Explicitly convert the float to an integer since floating-point values factorial does not exist
    int result;

    _asm {
        mov eax, dword ptr[intNum] // Load the converted integer into EAX
        call factorial // Call the Assembly factorial function
        mov dword ptr[result], eax // Store the result from EAX into 'result'
    }

    return result;
}

float performTrigFunction(float angle, const char* func) // Angle and the specific trigonometric function (sin, cos, tan) passed
{
    float result;
    float radAngle = angle * 3.14159f / 180.0f; // Convert the angle from degrees to radians for trigonometric calculations
    if (func[0] == 's' && func[1] == 'i' && func[2] == 'n')
    {
        _asm {
            push dword ptr[radAngle] // Push the radian angle onto the stack
            call trigSin // Call the trigSin function, which calculates the sine of the angle
            mov dword ptr[result], eax // Move the result(returned in eax) into the result variable
            add esp, 4 // Clean up the stack by removing the argument(4 bytes float)
        }
    }
    else if (func[0] == 'c' && func[1] == 'o' && func[2] == 's')
    {
        _asm {
            push dword ptr[radAngle] // Push the radian angle onto the stack
            call trigCos // Call the trigCos function, which calculates the cosine of the angle
            mov dword ptr[result], eax // Move the result(returned in eax) into the result variable
            add esp, 4 // Clean up the stack by removing the argument(4 bytes float)
        }
    }
    else if (func[0] == 't' && func[1] == 'a' && func[2] == 'n')
    {
        // Check for angles where the tangent function has vertical asymptotes (90° or 270°)
        if (fmod(angle, 180.0f) == 90.0f)
        {
            cout << "\nError: Tangent is undefined at 90/270 degrees\n\n";
            return 3.402823466e+38f; // Return a sentinel value (maximum 32-bit floating) to indicate an error
        }

        _asm {
            push dword ptr[radAngle] // Push the radian angle onto the stack
            call trigTan // Call the trigTan function, which calculates the tangent of the angle
            mov dword ptr[result], eax // Move the result(returned in eax) into the result variable
            add esp, 4 // Clean up the stack by removing the argument(4 bytes float)
        }
    }

    return result;
}

float performLnFunction(float x)
{
    // Check if the input is invalid (non-positive number)
    if (x <= 0.0f)
    {
        cout << "\nError: Logarithm is undefined for non-positive numbers\n\n";
        return 3.402823466e+38f; // Return a sentinel value (maximum 32-bit floating) to indicate an error
    }

    float result;
    _asm {
        push dword ptr[x] // Push the argument 'x' onto the stack for the 'performLn' function
        call performLn // Call 'performLn' function to compute the natural logarithm of 'x'
        fst dword ptr[result] // Store the result from the FPU stack into the 'result' variable
        add esp, 4 // Clean up the stack by adjusting the stack pointer(esp) after pushing 'x'
    }

    return result;
}

float performExpFunction(float x) // x will be input as expx
{
    float result;
    _asm {
        push dword ptr[x]
        call exponentiation
        mov dword ptr[result], eax // Store result, leaving it on the FPU stack
        add esp, 4
    }

    // No error case since exponential is valid for all real numbers

    return result;
}

// Function to parse input string into numbers and operators
float parseInput(const char* input, Expression& exp)
{
    int i = 0;              // Index for traversing input expression
    bool checkMinus = true; // Flag used to handle negative numbers

    while (input[i] != '\0') // Process the entire input string until null terminator reached
    {
        if (input[i] == ' ') // Skip whitespace
        {
            i++;
            continue;
        }
        // Handle trigonometric functions
        if ((input[i] == 's' && input[i + 1] == 'i' && input[i + 2] == 'n') ||
            (input[i] == 'c' && input[i + 1] == 'o' && input[i + 2] == 's') ||
            (input[i] == 't' && input[i + 1] == 'a' && input[i + 2] == 'n'))
        {
            // Save the trig function type
            char func[4] = { input[i], input[i + 1], input[i + 2], '\0' }; // Passed to performTrigFunction(float angle, const char* func)
            i = i + 3;                                                   // Advance index past the function name

            // Parse the angle associated with the trig function
            float num = 0.0f;
            bool decimalFound = false; // Flag to handle invalid decimal points
            float decimalPlace = 1.0f; // To correctly store digits in fractional part (e.g. 123 in 4.123)

            while (isDigit(input[i]) || input[i] == '.')
            {
                if (input[i] == '.')
                {
                    if (decimalFound) // Initially flag set to false, if decimal found again then error returned
                    {
                        cout << "\nError: Multiple decimal points\n\n";
                        return 3.402823466e+38f; // Return a sentinel value (maximum 32-bit floating) to indicate an error
                    }
                    decimalFound = true;
                    i++;
                    continue;
                }

                if (decimalFound) // Handles the fractional part of the angle
                {
                    decimalPlace *= 10.0f;                  // Digit at each decimal place is processed
                    num += (input[i] - '0') / decimalPlace; // "input[i] - '0'" converts char into numeric value; division by decimalPlace to place digit after '.' at correct place
                }
                else
                {
                    num = num * 10.0f + (input[i] - '0'); // For non-fractional angle value, existing number is multiplied by 10 then new digit's numeric value added to build-up the angle
                }
                i++;
            }

            // Compute trigonometric function result
            float trigResult = performTrigFunction(num, func);
            exp.numbers[exp.numCount++] = trigResult; // Store the computed trigonometric result in the numbers array and increment numCount
            checkMinus = false;
            continue;
        }
        // Handle factorial
        if ((input[i] == '!')) // Factorial input format requires '!' at start
        {
            i = i + 1;
            float num = 0.0;
            while (isDigit(input[i]))
            {
                num = num * 10.0f + (input[i] - '0'); // Build the number by multiplying by 10 and adding the new digit
                i++;
            }

            // Compute factorial function result
            float factResult = performFactorial(num);
            exp.numbers[exp.numCount++] = factResult; // Store the computed factorial result in the numbers array and increment numCount
            continue;
        }

        // Handle natural log
        if (input[i] == 'l' && input[i + 1] == 'n')
        {
            i = i + 2; // Advance index past the function name

            // Parse the number associated with the log function
            float num = 0.0f;
            bool decimalFound = false;
            float decimalPlace = 1.0f;

            while (isDigit(input[i]) || input[i] == '.')
            {
                if (input[i] == '.')
                {
                    if (decimalFound) // Initially set to false, set to true for invalid input like 1.1.1
                    {
                        cout << "\nError: Multiple decimal points\n\n";
                        return 3.402823466e+38f; // Return a sentinel value (maximum 32-bit floating) to indicate an error
                    }
                    decimalFound = true;
                    i++;
                    continue;
                }

                if (decimalFound) // Handle fractional part of number
                {
                    decimalPlace *= 10.0f;
                    num += (input[i] - '0') / decimalPlace; // "input[i] - '0'" converts char into numeric value; division by decimalPlace to place digit after '.' at correct place
                }
                else // Handle non-fractional part of number
                {
                    num = num * 10.0f + (input[i] - '0'); // Build the number by multiplying by 10 and adding the new digit
                }
                i++;
            }

            // Compute natural logarithm function result
            float logResult = performLnFunction(num);
            exp.numbers[exp.numCount++] = logResult; // Store the computed natural logarithm result in the numbers array and increment numCount
            checkMinus = false;
            continue;
        }

        // Handle exponential
        if (input[i] == 'e' && input[i + 1] == 'x' && input[i + 2] == 'p')
        {
            i = i + 3; // Advance index past the function name

            // Parse the number associated with the exponential function
            float num = 0.0f;
            bool decimalFound = false;
            float decimalPlace = 1.0f;

            while (isDigit(input[i]) || input[i] == '.')
            {
                if (input[i] == '.')
                {
                    if (decimalFound) // Initially set to false, set to true for invalid input like 1.1.1
                    {
                        cout << "\nError: Multiple decimal points\n\n";
                        return 3.402823466e+38f; // Return a sentinel value (maximum 32-bit floating) to indicate an error
                    }
                    decimalFound = true;
                    i++;
                    continue;
                }

                if (decimalFound)
                {
                    decimalPlace *= 10.0f;
                    num += (input[i] - '0') / decimalPlace; // "input[i] - '0'" converts char into numeric value; division by decimalPlace to place digit after '.' at correct place
                }
                else
                {
                    num = num * 10.0f + (input[i] - '0'); // Build the number by multiplying by 10 and adding the new digit
                }
                i++;
            }

            // Compute exponential function result
            float expResult = performExpFunction(num);
            exp.numbers[exp.numCount++] = expResult; // Store the computed natural exponential result in the numbers array and increment numCount
            checkMinus = false;
            continue;
        }

        // Handle numeric values
        // Handle negative numbers
        if (input[i] == '-' && checkMinus == true) // Ensures that '-' with negative numbers isn't treated as an operator
        {
            float num = 0.0f;
            int sign = -1; // Will multiply with final parsed number to make it negative
            i++;
            if (!isDigit(input[i]) && input[i] != '.') // Check if next character is not a digit or decimal
            {
                exp.operators[exp.opCount++] = '-'; // Store '-' as an operator
                checkMinus = true;                  // Set checkMinus back to true to handle any upcoming negative number
                continue;
            }
            bool decimalFound = false;
            float decimalPlace = 1.0f;
            while (isDigit(input[i]) || input[i] == '.')
            {
                if (input[i] == '.')
                {
                    if (decimalFound) // Initially set to false, set to true for invalid input like 1.1.1
                    {
                        cout << "\nError: Multiple decimal points\n\n";
                        return 3.402823466e+38f; // Return a sentinel value (maximum 32-bit floating) to indicate an error
                    }
                    decimalFound = true;
                    i++;
                    continue;
                }
                if (decimalFound)
                {
                    decimalPlace *= 10.0f;
                    num += (input[i] - '0') / decimalPlace; // "input[i] - '0'" converts char into numeric value; division by decimalPlace to place digit after '.' at correct place
                }
                else
                {
                    num = num * 10.0f + (input[i] - '0'); // Build the number by multiplying by 10 and adding the new digit
                }
                i++;
            }
            exp.numbers[exp.numCount++] = num * sign; // Num multiplied by -1 (sign) since it's negative then stored in numbers array
            checkMinus = false;
            continue;
        }
        // Handle non-negative numbers
        if (isDigit(input[i]) || input[i] == '.')
        {
            float num = 0.0f;
            bool decimalFound = false;
            float decimalPlace = 1.0f;

            while (isDigit(input[i]) || input[i] == '.')
            {
                if (input[i] == '.')
                {
                    if (decimalFound) // Initially set to false, set to true for invalid input like 1.1.1
                    {
                        cout << "\nError: Multiple decimal points\n\n";
                        return 3.402823466e+38f; // Return a sentinel value (maximum 32-bit floating) to indicate an error
                    }
                    decimalFound = true;
                    i++;
                    continue;
                }

                if (decimalFound)
                {
                    decimalPlace *= 10.0f;
                    num += (input[i] - '0') / decimalPlace; // "input[i] - '0'" converts char into numeric value; division by decimalPlace to place digit after '.' at correct place
                }
                else
                {
                    num = num * 10.0f + (input[i] - '0'); // Build the number by multiplying by 10 and adding the new digit
                }
                i++;
            }
            exp.numbers[exp.numCount++] = num;
            checkMinus = false;
            continue;
        }

        // Handle operators
        if (isOperator(input[i]))
        {
            exp.operators[exp.opCount++] = input[i];
            checkMinus = true;
            i++;
            continue;
        }

        // Handle remaining invalid character
        cout << "\nError: Invalid input\n\n";
        return 3.402823466e+38f; // Return a sentinel value (maximum 32-bit floating) to indicate an error
    }
}

// Function to evaluate expression following DMAS
float evaluateExpression(Expression& exp)
{
    // First, process factorials and powers (unary and binary operators)

    // Handle powers (^) - left to right
    for (int i = 0; i < exp.opCount; i++)
    {
        if (exp.operators[i] == '^')
        {
            // Perform exponentiation
            float result = performOperation(exp.numbers[i], exp.numbers[i + 1], exp.operators[i]);

            // Replace the two numbers with the result
            exp.numbers[i] = result;

            // Shift the remaining numbers to fill empty space
            for (int j = i + 1; j < exp.numCount - 1; j++)
            {
                exp.numbers[j] = exp.numbers[j + 1];
            }

            // Shift the remaining operators to fill empty space
            for (int j = i; j < exp.opCount - 1; j++)
            {
                exp.operators[j] = exp.operators[j + 1];
            }

            // Reduce counts
            exp.numCount--;
            exp.opCount--;

            i--; // Adjust index
        }
    }

    // Then process multiplication and division left to right
    for (int i = 0; i < exp.opCount; i++)
    {
        if (exp.operators[i] == '*' || exp.operators[i] == '/')
        {
            // Perform multiplication or division
            float result = performOperation(exp.numbers[i], exp.numbers[i + 1], exp.operators[i]);

            // Replace the two numbers with the result
            exp.numbers[i] = result;

            // Shift the remaining numbers
            for (int j = i + 1; j < exp.numCount - 1; j++)
            {
                exp.numbers[j] = exp.numbers[j + 1];
            }

            // Shift the remaining operators
            for (int j = i; j < exp.opCount - 1; j++)
            {
                exp.operators[j] = exp.operators[j + 1];
            }

            // Reduce counts
            exp.numCount--;
            exp.opCount--;

            i--; // Adjust index to reprocess this position
        }
    }

    // Then process addition and subtraction left to right on the results obtained after other operations
    float result = exp.numbers[0];
    for (int i = 0; i < exp.opCount; i++)
    {
        if (exp.operators[i] == '+')
        {
            result = performOperation(result, exp.numbers[i + 1], '+'); // Perform addition on result (exp.numbers[0]) and next number (exp.numbers[i + 1])
        }
        else if (exp.operators[i] == '-')
        {
            result = performOperation(result, exp.numbers[i + 1], '-'); // Perform subtraction on result (exp.numbers[0]) and next number (exp.numbers[i + 1])
        }
    }
    return result;
}

// UI Improvement functions
// Function to get the console width
int getConsoleWidth()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
}
// Function to center text
void centerText(const string& text)
{
    int consoleWidth = getConsoleWidth();
    int textLength = text.length();
    int padding = (consoleWidth - textLength) / 2;

    for (int i = 0; i < padding; ++i)
    {
        cout << " "; // Print spaces before the text
    }
    cout << text << endl;
}

int main()
{
    centerText("==============================================================");
    centerText("Scientific Calculator");
    centerText("==============================================================");
    centerText("Features:");
    centerText("==============================================================");
    centerText("  1. Arithmetic: (+, -, *, /)");
    centerText("  2. Complex Expressions (DMAS)");
    centerText("  3. Trig: sinx, cosx, tanx, cosecx, secx, cotx (in degrees)");
    centerText("  4. Factorial (!n)");
    centerText("  5. Natural Logarithm: lnx");
    centerText("  6. Exponentiation (e^x or a^b)");
    centerText("  7. Square Root (a^0.5)");
    centerText("==============================================================");
    centerText("Instructions:");
    centerText("==============================================================");
    centerText("  - Enter expressions (e.g., -2.5 + 3 * -4.0 / 2.5)");
    centerText("  - For trig, use sinx, cosx, tanx (no parentheses | positive values only)");
    centerText("  - For cosecx, use 1/sinx");
    centerText("  - For secx, use 1/cosx");
    centerText("  - For cotx, use 1/tanx");
    centerText("  - Factorial: !n (e.g., !3 for 3!) (positive integers only)");
    centerText("  - For natural logarithm, use lnx (no parentheses)");
    centerText("  - Exponential Function: expx (e.g., exp2 for e^2)");
    centerText("  - Power: a^b (e.g., 2^3 for 2 raised to 3) (Integers only)");
    centerText("  - Square root: a^0.5");
    centerText("  - Type 'exit' to quit");
    centerText("==============================================================");

    while (true)
    {
        cout << "\nEnter expression: ";

        char input[MAX_SIZE]; // Buffer for user input
        Expression exp;       // Expression object to hold parsed data

        cin.getline(input, MAX_SIZE); // Take expression input

        // Convert input to lowercase to make the exit/other conditions case-insensitive
        string inputStr(input);
        transform(inputStr.begin(), inputStr.end(), inputStr.begin(), ::tolower);

        if (inputStr == "exit")
        {
            cout << "\n";
            centerText("Thank you for using the Scientific Calculator");
            cout << "\n";
            break;
        }

        float parseOutputCheck = parseInput(input, exp); // Parse the input into numbers and operators

        cout << fixed << setprecision(2);
        float result = evaluateExpression(exp); // Evaluate the expression

        // Only print result if no error(s) occurred
        if (result != 3.402823466e+38f && parseOutputCheck != 3.402823466e+38f)
        {
            cout << "\nResult: " << result << "\n\n";
        }

        cout << "==============================================================\n";
    }

    return 0;
}