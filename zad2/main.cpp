#include <iostream>

class logical_values_array {
private:
    unsigned int _value;

public:
    logical_values_array(unsigned int value = 0) : _value(value) {}

    logical_values_array(const logical_values_array &other) : _value(other._value) {}

    ~logical_values_array(){}

    logical_values_array& operator= (logical_values_array const &other){
        if(this != &other){
            _value = other._value;
        }
        return *this;
    }


    // Accessor для поля value
    unsigned int get_value() const {
        return _value;
    }

    // Инверсия
    logical_values_array invert() const {
        return logical_values_array(!_value);
    }

    // Конъюнкция
    logical_values_array conjunction(const logical_values_array &other) const {
        return logical_values_array(_value && other._value);
    }

    // Дизъюнкция
    logical_values_array disjunction(const logical_values_array &other) const {
        return logical_values_array(_value || other._value);
    }

    // Импликация
    logical_values_array implication(const logical_values_array &other) const {
        return logical_values_array(!_value || other._value);
    }

    // Коимпликация
    logical_values_array coimplication(const logical_values_array &other) const {
        return invert().implication(other.invert());
    }

    // Сложение по модулю 2
    logical_values_array modulo2_sum(const logical_values_array &other) const {
        return conjunction(other.invert()).disjunction(invert().conjunction(other));
    }

    // Эквивалентность
    logical_values_array equivalence(const logical_values_array &other) const {
        return modulo2_sum(other).invert();
    }

    // Стрелка Пирса
    logical_values_array pierce_arrow(const logical_values_array &other) const {
        return disjunction(other).invert();
    }

    // Штрих Шеффера
    logical_values_array sheffer_stroke(const logical_values_array &other) const {
        return conjunction(other).invert();
    }

    // Статический метод equals
    static bool equals(const logical_values_array &a, const logical_values_array &b) {
        return a._value == b._value;
    }

    // Получение значения бита по позиции
    unsigned int get_bit(int position) const {
        return (_value >> position) & 1;
    }

    // Получение двоичного представления
    void binary_representation(char *output) const {
        for (int i = 31; i >= 0; --i) {
            output[31 - i] = '0' + get_bit(i);
        }
        output[32] = '\0';
    }
};

int main() {
    logical_values_array a(12);
    logical_values_array b(0);

    logical_values_array c = a.sheffer_stroke(b);
    std::cout <<  c.get_value() << std::endl;

    logical_values_array d = a.equivalence(b);
    std::cout << d.get_value() << std::endl;

    char binary_rep[33];
    c.binary_representation(binary_rep);
    std::cout << binary_rep << std::endl;

    return 0;
}