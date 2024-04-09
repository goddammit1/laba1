#include <iostream>
#include <cstdarg>
#include <vector>
#include <cmath>

class Matrix {
private:
    double **matrix;
    size_t size;

    void allocateMemory() {
        matrix = new double *[size];
        for (size_t i = 0; i < size; ++i) {
            matrix[i] = new double[size];
        }
    }

public:
    Matrix(size_t size, ...) : size(size) {
        matrix = new double*[size];
        va_list args;
        va_start(args, size);

        for (size_t i = 0; i < size; ++i) {
            matrix[i] = new double[size];
            for (size_t j = 0; j < size; ++j) {
                matrix[i][j] = va_arg(args, double);
            }
        }

        va_end(args);
    }

    // Конструктор копирования
    Matrix(const Matrix &other) : size(other.size) {
        matrix = new double*[size];
        for (size_t i = 0; i < size; ++i) {
            matrix[i] = new double[size];
            for (size_t j = 0; j < size; ++j) {
                matrix[i][j] = other.matrix[i][j];
            }
        }
    }

    // Оператор присваивания
    Matrix& operator=(const Matrix &other) {
        if (this == &other) {
            return *this;
        }

        for (size_t i = 0; i < size; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;

        size = other.size;
        matrix = new double*[size];
        for (size_t i = 0; i < size; ++i) {
            matrix[i] = new double[size];
            for (size_t j = 0; j < size; ++j) {
                matrix[i][j] = other.matrix[i][j];
            }
        }

        return *this;
    }

    // Деструктор
    ~Matrix() {
        for (size_t i = 0; i < size; ++i) {
            delete[] matrix[i];
        }
        delete[] matrix;
    }

    double *operator[](size_t index) {
        return matrix[index];
    }

    double &operator()(size_t i, size_t j) {
        return matrix[i][j];
    }


    Matrix add(const Matrix &other) {
        Matrix result(size);
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                result.matrix[i][j] = matrix[i][j] + other.matrix[i][j];
            }
        }
        return result;
    }


    Matrix multiply(const Matrix &other) const {

        Matrix result(size);

        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                result.matrix[i][j] = 0;
                for (size_t k = 0; k < size; ++k) {
                    result.matrix[i][j] += matrix[i][k] * other.matrix[k][j];
                }
            }
        }

        return result;
    }


    Matrix multiplyByScalar(double scalar) {
        Matrix result(size);
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                result.matrix[i][j] = matrix[i][j] * scalar;
            }
        }
        return result;
    }

    Matrix static multiplyScalar(double scalar, const Matrix &mat) {
        Matrix result(mat);
        for (size_t i = 0; i < mat.size; ++i) {
            for (size_t j = 0; j < mat.size; ++j) {
                result.matrix[i][j] *= scalar;
            }
        }
        return result;
    }

    Matrix subtract(const Matrix &other) {
        Matrix result(size);
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                result.matrix[i][j] = matrix[i][j] - other.matrix[i][j];
            }
        }
        return result;
    }


    Matrix transpose() {
        Matrix result(size);
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                result.matrix[i][j] = matrix[j][i];
            }
        }
        return result;
    }

    double determinant() {
        double det = 1;
        Matrix temp(*this); // Создаем копию матрицы

        for (size_t i = 0; i < size - 1; ++i) {
            size_t maxRow = i;
            for (size_t j = i + 1; j < size; ++j) {
                if (std::abs(temp.matrix[j][i]) > std::abs(temp.matrix[maxRow][i])) {
                    maxRow = j;
                }
            }

            if (maxRow != i) {
                // Swap the rows
                for (size_t k = 0; k < size; ++k) {
                    std::swap(temp.matrix[i][k], temp.matrix[maxRow][k]);
                }
                det *= -1; // Меняем знак определителя при перестановке строк
            }

            if (std::abs(temp.matrix[i][i]) < 1e-9) {
                return 0; // Определитель равен 0, если встречается 0 на диагональном элементе
            }

            for (size_t j = i + 1; j < size; ++j) {
                double ratio = temp.matrix[j][i] / temp.matrix[i][i];
                for (size_t k = i; k < size; ++k) {
                    temp.matrix[j][k] -= ratio * temp.matrix[i][k];
                }
            }
        }

        for (size_t i = 0; i < size; ++i) {
            det *= temp.matrix[i][i]; // Умножаем только на элементы на диагонали
        }

        return det;
    }

    Matrix getInverse() const {
        Matrix inverse(size * 2);

        // Инициализируем расширенную матрицу, вторая половина - единичная матрица
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                inverse.matrix[i][j] = matrix[i][j];
            }
            for (size_t j = size; j < size * 2; ++j) {
                inverse.matrix[i][j] = (i == j - size) ? 1 : 0;
            }
        }

        // Приводим расширенную матрицу к диагональному виду
        for (size_t i = 0; i < size; ++i) {
            double div = inverse.matrix[i][i];
            for (size_t j = 0; j < size * 2; ++j) {
                inverse.matrix[i][j] /= div;
            }
            for (size_t k = 0; k < size; ++k) {
                if (k != i) {
                    double factor = inverse.matrix[k][i];
                    for (size_t j = 0; j < size * 2; ++j) {
                        inverse.matrix[k][j] -= factor * inverse.matrix[i][j];
                    }
                }
            }
        }

        // Оставляем только правую часть (обратную матрицу)
        Matrix result(size);
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                result.matrix[i][j] = inverse.matrix[i][j + size];
            }
        }

        return result;
    }


    void printMatrix() {
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j) {
                std::cout << matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};


int main(){
    Matrix m(3, 1.0, 2.0, 5.0, 3.0, 2.0, 1.0, 1.0, 1.0, 1.0),
            m2(3, 1.0, 2.0, 4.0, 8.0, 5.0, 10.0, 6.0, 7.0, 8.0);
    m.printMatrix();
    std::cout << m.determinant() << std::endl;
    auto res = m.transpose();
    res.printMatrix();
    auto mult = m.multiplyByScalar(2.0);
    mult.printMatrix();

}












