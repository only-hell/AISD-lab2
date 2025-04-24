#include <cstdio>
#include <cstdlib>
#include <ctime>

// Шаблонный класс хеш-таблицы с методом цепочек
template<typename K, typename T>
class HashTable {
private:
    // Узел в цепочке
    struct HashTableEntry {
        K key;
        T value;
        HashTableEntry* next;

        HashTableEntry(K k, T v) : key(k), value(v), next(nullptr) {}
    };

    static const int table_size = 10;
    HashTableEntry** table;

    // Мультипликативная хеш-функция
    int hash(K key) const {
        const double A = 0.6180339887;
        double product = key * A;
        double fractional_part = product - (int)product;
        return (int)(table_size * fractional_part);
    }

public:
    // Конструктор по умолчанию
    HashTable() {
        table = new HashTableEntry * [table_size];
        for (int i = 0; i < table_size; ++i) {
            table[i] = nullptr;
        }
    }

    // Конструктор с генерацией случайных данных
    HashTable(int size) {
        table = new HashTableEntry * [table_size];
        for (int i = 0; i < table_size; ++i) {
            table[i] = nullptr;
        }

        for (int i = 0; i < size; ++i) {
            K key = rand() % 20 + 1;
            T value = rand() % 100 + 1;
            insert(key, value);
        }
    }

    // Конструктор копирования
    HashTable(const HashTable& other) {
        table = new HashTableEntry * [table_size];
        for (int i = 0; i < table_size; ++i) {
            table[i] = nullptr;
            HashTableEntry* current = other.table[i];
            while (current) {
                insert(current->key, current->value);
                current = current->next;
            }
        }
    }

    // Деструктор
    ~HashTable() {
        for (int i = 0; i < table_size; ++i) {
            HashTableEntry* current = table[i];
            while (current) {
                HashTableEntry* to_delete = current;
                current = current->next;
                delete to_delete;
            }
        }
        delete[] table;
    }

    // Оператор присваивания
    HashTable& operator=(const HashTable& other) {
        if (this == &other) return *this;

        for (int i = 0; i < table_size; ++i) {
            HashTableEntry* current = table[i];
            while (current) {
                HashTableEntry* to_delete = current;
                current = current->next;
                delete to_delete;
            }
        }
        delete[] table;

        table = new HashTableEntry * [table_size];
        for (int i = 0; i < table_size; ++i) {
            table[i] = nullptr;
            HashTableEntry* current = other.table[i];
            while (current) {
                insert(current->key, current->value);
                current = current->next;
            }
        }
        return *this;
    }

    // Вставка нового элемента
    bool insert(K key, const T& value) {
        int index = hash(key);
        HashTableEntry* current = table[index];
        while (current) {
            if (current->key == key) return false;
            current = current->next;
        }

        HashTableEntry* new_entry = new HashTableEntry(key, value);
        new_entry->next = table[index];
        table[index] = new_entry;
        return true;
    }

    // Вставка или обновление элемента
    void insert_or_assign(K key, T& value) {
        int index = hash(key);
        HashTableEntry* current = table[index];
        while (current) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }

        HashTableEntry* new_entry = new HashTableEntry(key, value);
        new_entry->next = table[index];
        table[index] = new_entry;
    }

    // Проверка наличия значения (не ключа!)
    bool contains(T& value) {
        for (int i = 0; i < table_size; ++i) {
            HashTableEntry* current = table[i];
            while (current) {
                if (current->value == value) return true;
                current = current->next;
            }
        }
        return false;
    }

    // Поиск значения по ключу
    T* search(K key) {
        int index = hash(key);
        HashTableEntry* current = table[index];
        while (current) {
            if (current->key == key) return &current->value;
            current = current->next;
        }
        return nullptr;
    }

    // Удаление по ключу
    bool erase(K key) {
        int index = hash(key);
        HashTableEntry* current = table[index];
        HashTableEntry* prev = nullptr;

        while (current) {
            if (current->key == key) {
                if (prev) prev->next = current->next;
                else table[index] = current->next;
                delete current;
                return true;
            }
            prev = current;
            current = current->next;
        }
        return false;
    }

    // Подсчет количества элементов в бакете по ключу
    int count(K key) const {
        int index = hash(key);
        int count = 0;
        HashTableEntry* current = table[index];
        while (current) {
            count++;
            current = current->next;
        }
        return count;
    }

    // Вывод всех элементов
    void print() const {
        printf("HashTable contents:\n");
        for (int i = 0; i < table_size; ++i) {
            HashTableEntry* current = table[i];
            while (current) {
                printf("Key: %d, Value: %d\n", current->key, current->value);
                current = current->next;
            }
        }
    }
};

int main() {
    srand((unsigned int)time(0));
    HashTable<int, int> table(50);

    // Подсчет числа повторений каждого ключа
    for (int i = 1; i <= 20; ++i) {
        int cnt = table.count(i);
        printf("Bucket with key %d has %d elements.\n", i, cnt);
    }

    table.print();

    return 0;
}
