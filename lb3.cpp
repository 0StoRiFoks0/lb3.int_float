#include <iostream>
#include <array>
#include <tuple>
#include <type_traits>
#include <initializer_list>
#include <limits>
#include <stdexcept>

// Клас MathematicalVector
template<typename T, std::size_t N>
class MathematicalVector {
private:
    std::array<T, N> data;
    inline static size_t instanceCount = 0;
    inline static size_t totalCreated = 0;

public:
    // Конструктори
    MathematicalVector() {
        data.fill(T{});
        ++instanceCount;
        ++totalCreated;
    }

    explicit MathematicalVector(const T& value) {
        data.fill(value);
        ++instanceCount;
        ++totalCreated;
    }

    MathematicalVector(std::initializer_list<T> values) {
        std::size_t i = 0;
        for (auto it = values.begin(); it != values.end() && i < N; ++it, ++i) {
            data[i] = *it;
        }
        for (; i < N; ++i) {
            data[i] = T{};
        }
        ++instanceCount;
        ++totalCreated;
    }

    // Деструктор
    ~MathematicalVector() {
        --instanceCount;
    }

    // Методи доступу
    T& operator[](std::size_t index) {
        return data[index];
    }

    const T& operator[](std::size_t index) const {
        return data[index];
    }

    std::size_t size() const {
        return N;
    }

    // Арифметичні операції зі скалярами
    MathematicalVector operator+(const T& scalar) const {
        MathematicalVector result;
        for (std::size_t i = 0; i < N; ++i) {
            result[i] = data[i] + scalar;
        }
        return result;
    }

    MathematicalVector operator-(const T& scalar) const {
        MathematicalVector result;
        for (std::size_t i = 0; i < N; ++i) {
            result[i] = data[i] - scalar;
        }
        return result;
    }

    MathematicalVector operator*(const T& scalar) const {
        MathematicalVector result;
        for (std::size_t i = 0; i < N; ++i) {
            result[i] = data[i] * scalar;
        }
        return result;
    }

    MathematicalVector operator/(const T& scalar) const {
        MathematicalVector result;
        for (std::size_t i = 0; i < N; ++i) {
            result[i] = data[i] / scalar;
        }
        return result;
    }

    // Арифметичні операції з іншими векторами
    template<typename U>
    auto operator+(const MathematicalVector<U, N>& other) const {
        using ResultType = decltype(T{} + U{});
        MathematicalVector<ResultType, N> result;
        for (std::size_t i = 0; i < N; ++i) {
            result[i] = data[i] + other[i];
        }
        return result;
    }

    template<typename U>
    auto operator-(const MathematicalVector<U, N>& other) const {
        using ResultType = decltype(T{} - U{});
        MathematicalVector<ResultType, N> result;
        for (std::size_t i = 0; i < N; ++i) {
            result[i] = data[i] - other[i];
        }
        return result;
    }

    template<typename U>
    auto operator*(const MathematicalVector<U, N>& other) const {
        using ResultType = decltype(T{} * U{});
        MathematicalVector<ResultType, N> result;
        for (std::size_t i = 0; i < N; ++i) {
            result[i] = data[i] * other[i];
        }
        return result;
    }

    template<typename U>
    auto operator/(const MathematicalVector<U, N>& other) const {
        using ResultType = decltype(T{} / U{});
        MathematicalVector<ResultType, N> result;
        for (std::size_t i = 0; i < N; ++i) {
            result[i] = data[i] / other[i];
        }
        return result;
    }

    // Зважена сума
    template<typename Scalar, typename OtherT>
    auto weighted_sum(const Scalar& w1, const MathematicalVector<OtherT, N>& other, const Scalar& w2) const {
        using ResultType = decltype(w1 * T{} + w2 * OtherT{});
        MathematicalVector<ResultType, N> result;
        for (std::size_t i = 0; i < N; ++i) {
            result[i] = w1 * data[i] + w2 * other[i];
        }
        return result;
    }

    // resize
    template<std::size_t M>
    MathematicalVector<T, M> resize() const {
        MathematicalVector<T, M> result;
        for (std::size_t i = 0; i < std::min(N, M); ++i) {
            result[i] = data[i];
        }
        return result;
    }

    // convert
    template<typename NewType>
    MathematicalVector<NewType, N> convert() const {
        MathematicalVector<NewType, N> result;
        for (std::size_t i = 0; i < N; ++i) {
            result[i] = static_cast<NewType>(data[i]);
        }
        return result;
    }

    // slice
    template<std::size_t M>
    MathematicalVector<T, M> slice(std::size_t start) const {
        static_assert(M > 0, "Slice size must be positive");
        if (start + M > N) {
            throw std::out_of_range("Invalid slice indices");
        }
        MathematicalVector<T, M> result;
        for (std::size_t i = 0; i < M; ++i) {
            result[i] = data[start + i];
        }
        return result;
    }

    // Вивід вектора
    friend std::ostream& operator<<(std::ostream& os, const MathematicalVector& vec) {
        os << "[ ";
        for (const auto& el : vec.data) {
            os << el << " ";
        }
        os << "]";
        return os;
    }

    // Гетери для об'єктів
    static size_t getInstanceCount() { return instanceCount; }
    static size_t getTotalCreated() { return totalCreated; }

    // Метод для введення вектора з консолі
    void input() {
        std::cout << "Введіть " << N << " елементів вектора:\n";
        for (size_t i = 0; i < N; ++i) {
            while (!(std::cin >> data[i])) {
                std::cout << "Неправильне введення. Спробуйте ще раз: ";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }
};

// Функції concat і make_vector залишаємо без змін
template<typename T, typename... Args>
auto make_vector(T first, Args... args) {
    constexpr std::size_t N = 1 + sizeof...(Args);
    MathematicalVector<T, N> result{ first, args... };
    return result;
}

template<typename T, std::size_t N1, std::size_t N2>
auto concat(const MathematicalVector<T, N1>& v1, const MathematicalVector<T, N2>& v2) {
    MathematicalVector<T, N1 + N2> result;
    for (std::size_t i = 0; i < N1; ++i) result[i] = v1[i];
    for (std::size_t i = 0; i < N2; ++i) result[N1 + i] = v2[i];
    return result;
}

template<typename T1, std::size_t N1, typename T2, std::size_t N2>
auto concat(const MathematicalVector<T1, N1>& v1, const MathematicalVector<T2, N2>& v2) {
    using ResultType = decltype(T1{} + T2{});
    MathematicalVector<ResultType, N1 + N2> result;
    for (std::size_t i = 0; i < N1; ++i) result[i] = v1[i];
    for (std::size_t i = 0; i < N2; ++i) result[N1 + i] = v2[i];
    return result;
}

template<typename T1, std::size_t N1, typename T2, std::size_t N2, typename... Rest>
auto concat(const MathematicalVector<T1, N1>& v1, const MathematicalVector<T2, N2>& v2, Rest... rest) {
    auto temp = concat(v1, v2);
    return concat(temp, rest...);
}

template<typename T, std::size_t N>
MathematicalVector<T, N> concat(const MathematicalVector<T, N>& v) {
    return v;
}

// Інтерактивний інтерфейс для операцій з векторами
void menu() {
    std::cout << "=== Дружній інтерфейс для MathematicalVector ===\n";
    std::cout << "Обирайте тип вектора (int або float):\n1. int\n2. float\nВведіть 1 або 2: ";
    int type_choice;
    while (!(std::cin >> type_choice) || (type_choice != 1 && type_choice != 2)) {
        std::cout << "Некоректний вибір. Спробуйте ще раз: ";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    if (type_choice == 1) {
        constexpr size_t N = 5;
        MathematicalVector<int, N> v1;
        std::cout << "Введіть вектор з " << N << " цілих чисел:\n";
        v1.input();

        while (true) {
            std::cout << "\nОперації з вектором:\n";
            std::cout << "1. Додати скаляр\n2. Відняти скаляр\n3. Помножити на скаляр\n4. Поділити на скаляр\n";
            std::cout << "5. Вивести вектор\n6. Вийти\nВаш вибір: ";

            int op;
            if (!(std::cin >> op)) {
                std::cout << "Некоректне введення.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (op == 6) {
                std::cout << "Вихід з програми.\n";
                break;
            }

            int scalar;
            MathematicalVector<int, N> result;
            switch (op) {
                case 1:
                    std::cout << "Введіть ціле число для додавання: ";
                    std::cin >> scalar;
                    result = v1 + scalar;
                    std::cout << "Результат: " << result << "\n";
                    break;
                case 2:
                    std::cout << "Введіть ціле число для віднімання: ";
                    std::cin >> scalar;
                    result = v1 - scalar;
                    std::cout << "Результат: " << result << "\n";
                    break;
                case 3:
                    std::cout << "Введіть ціле число для множення: ";
                    std::cin >> scalar;
                    result = v1 * scalar;
                    std::cout << "Результат: " << result << "\n";
                    break;
                case 4:
                    std::cout << "Введіть ціле число для ділення: ";
                    std::cin >> scalar;
                    if (scalar == 0) {
                        std::cout << "Ділення на нуль неможливе.\n";
                        break;
                    }
                    result = v1 / scalar;
                    std::cout << "Результат: " << result << "\n";
                    break;
                case 5:
                    std::cout << "Ваш вектор: " << v1 << "\n";
                    break;
                default:
                    std::cout << "Невідома операція.\n";
            }
        }
    }
    else if (type_choice == 2) {
        constexpr size_t N = 5;
        MathematicalVector<float, N> v1;
        std::cout << "Введіть вектор з " << N << " чисел з плаваючою точкою:\n";
        v1.input();

        while (true) {
            std::cout << "\nОперації з вектором:\n";
            std::cout << "1. Додати скаляр\n2. Відняти скаляр\n3. Помножити на скаляр\n4. Поділити на скаляр\n";
            std::cout << "5. Вивести вектор\n6. Вийти\nВаш вибір: ";

            int op;
            if (!(std::cin >> op)) {
                std::cout << "Некоректне введення.\n";
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (op == 6) {
                std::cout << "Вихід з програми.\n";
                break;
            }

            float scalar;
            MathematicalVector<float, N> result;
            switch (op) {
                case 1:
                    std::cout << "Введіть число для додавання: ";
                    std::cin >> scalar;
                    result = v1 + scalar;
                    std::cout << "Результат: " << result << "\n";
                    break;
                case 2:
                    std::cout << "Введіть число для віднімання: ";
                    std::cin >> scalar;
                    result = v1 - scalar;
                    std::cout << "Результат: " << result << "\n";
                    break;
                case 3:
                    std::cout << "Введіть число для множення: ";
                    std::cin >> scalar;
                    result = v1 * scalar;
                    std::cout << "Результат: " << result << "\n";
                    break;
                case 4:
                    std::cout << "Введіть число для ділення: ";
                    std::cin >> scalar;
                    if (scalar == 0.0f) {
                        std::cout << "Ділення на нуль неможливе.\n";
                        break;
                    }
                    result = v1 / scalar;
                    std::cout << "Результат: " << result << "\n";
                    break;
                case 5:
                    std::cout << "Ваш вектор: " << v1 << "\n";
                    break;
                default:
                    std::cout << "Невідома операція.\n";
            }
        }
    }
}

int main() {
    menu();
    return 0;
}
