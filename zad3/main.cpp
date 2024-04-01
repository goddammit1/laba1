#include <iostream>
#include <cmath>

class ComplexNumber {
private:
    double real;
    double imaginary;

public:
    ComplexNumber(double real = 0, double imaginary = 0) : real(real), imaginary(imaginary) {}

    // Сложение комплексных чисел
    ComplexNumber operator+(const ComplexNumber& other) {
        return ComplexNumber(real + other.real, imaginary + other.imaginary);
    }

    // Вычитание комплексных чисел
    ComplexNumber operator-(const ComplexNumber& other) {
        return ComplexNumber(real - other.real, imaginary - other.imaginary);
    }

    // Умножение комплексных чисел
    ComplexNumber operator*(const ComplexNumber& other) {
        return ComplexNumber(real * other.real - imaginary * other.imaginary, real * other.imaginary + imaginary * other.real);
    }

    // Деление комплексных чисел
    ComplexNumber operator/(const ComplexNumber& other) {
        double denominator = other.real * other.real + other.imaginary * other.imaginary;
        return ComplexNumber((real * other.real + imaginary * other.imaginary) / denominator,
                             (imaginary * other.real - real * other.imaginary) / denominator);
    }

    // Модуль комплексного числа
    double getMagnitude() {
        return std::sqrt(real * real + imaginary * imaginary);
    }

    // Аргумент комплексного числа
    double getArgument() {
        return std::atan2(imaginary, real);
    }

    // Вывод комплексного числа
    void print() {
        std::cout << "(" << real << " + " << imaginary << "i)" << std::endl;
    }
};

int main() {
    ComplexNumber num1(2, 3);
    ComplexNumber num2(4, -1);

    ComplexNumber sum = num1 + num2;
    ComplexNumber diff = num1 - num2;
    ComplexNumber product = num1 * num2;
    ComplexNumber quotient = num1 / num2;

    std::cout << "num1: ";
    num1.print();
    std::cout << "num2: ";
    num2.print();

    std::cout << "Сумма: ";
    sum.print();

    std::cout << "Разность: ";
    diff.print();

    std::cout << "Произведение: ";
    product.print();

    std::cout << "Частное: ";
    quotient.print();

    std::cout << "Модуль num1: " << num1.getMagnitude() << std::endl;
    std::cout << "Аргумент num1: " << num1.getArgument() << std::endl;

    return 0;
}