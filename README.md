# Yadro Тестовое Задание :: Компьютерный клуб

![C++](https://img.shields.io/badge/C++-20-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.10+-brightgreen.svg)
![Testing](https://img.shields.io/badge/Tested-GCC%20%7C%20MinGW-success.svg)

### Сборка и запуск проекта

```shell
mkdir build && cd build
cmake ..
make
./task <имя файла>
```

### Тестирование

Тесты расположены в директории `test-data/`. Для управления списком тестов используйте файл `tests.txt`. Далее запускаем скрипт

```shell
./run-test-data.sh build/task
```

### Форматирование

```shell
find src include -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i
```
