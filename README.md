# NeuroForge

Библиотека на C++ для построения и обучения нейронных сетей для задачи бинарной классификации точек в двухмерном пространстве.

Состав команды:
- Голубев Артём
- Жохов Даниил
- Потёмкин Платон

---

## Возможности

- Класс матриц с поддержкой основных математических операций
- Нейронная сеть с настраиваемой архитектурой (произвольное количество слоёв и нейронов)
- Поддержка нескольких функций активации: Sigmoid, ReLU, Tanh
- Функции потерь: MSE, Binary Cross-Entropy
- Обучение методом обратного распространения ошибки с early stopping
- Метрики качества: Accuracy, ROC-AUC
- Генерация датасетов для бинарной классификации
- Min-Max нормализация данных
- Импорт/экспорт данных в формате CSV
- Визуализация данных через Python (matplotlib)
- Тесты для всех основных компонентов

---

## Структура проекта

```
NeuroForge/
├── include/
│   ├── matrix_realization.h   — класс матриц
│   ├── utils.h                — утилиты, функции активации, метрики
│   ├── layer.h                — слой нейронной сети
│   ├── neural_network.h       — класс нейронной сети
│   ├── file_import.h          — работа с CSV файлами
│   └── visualization.h        — визуализация данных
├── src/
│   ├── main.cpp               — точка входа, демонстрация работы
│   ├── matrix_realization.cpp
│   ├── utils.cpp
│   ├── layer.cpp
│   ├── neural_network.cpp
│   ├── file_import.cpp
│   └── visualization.cpp
├── tests/
│   ├── matrix_tests.cpp
│   ├── file_import_tests.cpp
│   └── visualization_tests.cpp
├── scripts/
│   └── plot.py                — скрипт визуализации
├── data/                      — директория для CSV файлов
├── CMakeLists.txt
└── requirements.txt
```

---

## Установка и запуск

### Зависимости

Python-зависимости для визуализации:
```bash
pip install -r requirements.txt
```

### Сборка

```bash
mkdir build && cd build
cmake ..
make
```

### Запуск демонстрации

```bash
./main
```

### Запуск тестов

```bash
./main --test
```

---

## Использование

### Создание и обучение модели

```cpp
#include "neural_network.h"
#include "utils.h"

using namespace nnlab;

// Создание сети с архитектурой 2 -> 4 -> 4 -> 1
NeuralNetwork net({2, 4, 4, 1});

// Генерация и нормализация датасета
auto trainData = minMaxNormalization(genBinClassifyDataset(1000, 0.4));
auto testData  = minMaxNormalization(genBinClassifyDataset(200, 0.4));

// Обучение с BCE Loss, learning rate 0.1, early stopping
net.train(trainData, bceLoss, bceDerivative, 0.1);

// Предсказания
auto predictions     = net.predict(testData.first);
auto probabilities   = net.predictProba(testData.first);

// Метрики
std::cout << "Accuracy: " << accuracy(predictions, testData.second) << std::endl;
std::cout << "ROC-AUC:  " << rocAuc(probabilities, testData.second)  << std::endl;
```

### Настройка архитектуры

```cpp
// Сеть с пользовательскими функциями активации
NeuralNetwork net(
    {2, 8, 4, 1},
    {nnlab::relu, nnlab::relu, nnlab::sigmoid},
    {nnlab::reluDerivative, nnlab::reluDerivative, nnlab::sigmoidDerivative}
);
```

### Параметры обучения

```cpp
net.train(
    trainData,
    nnlab::bceLoss,         // функция потерь
    nnlab::bceDerivative,   // производная функции потерь
    0.1,                    // learning rate
    10000,                  // максимальное число эпох
    0.00001,                // minDelta для early stopping
    10                      // patience для early stopping
);
```

### Работа с CSV файлами

```cpp
// Сохранение датасета
fileSaveToCSV("data/dataset.csv", coordinates, labels);

// Загрузка датасета с метками
auto data = fileImportMatrixLabel("data/dataset.csv");

// Визуализация
plot("data/dataset.csv");
```

---

## Метод обучения

Модель обучается методом **градиентного спуска** с алгоритмом **обратного распространения ошибки** (backpropagation).

### Прямой проход (forward pass)

На каждом слое вычисляется взвешенная сумма входов с последующим применением функции активации:

```
layer[i] = activation(weights[i] * layer[i-1])
```

Промежуточные активации каждого слоя сохраняются — они нужны для backprop.

### Обратный проход (backpropagation)

После вычисления ошибки на выходе градиент распространяется обратно по сети. Для каждого слоя вычисляется:

```
δ[выход]  = -loss'(output, y) * σ'(output)
δ[слой l] = σ'(activation[l]) * Σ(δ[l+1] * weights[l+1])
```

Затем веса обновляются пропорционально вкладу каждого нейрона в ошибку:

```
weights[l] += δ[l] * activation[l-1] * learning_rate
```

### Early Stopping

Чтобы избежать лишних итераций, реализован механизм ранней остановки. Если изменение функции потерь между эпохами меньше порога `minDelta` на протяжении `patience` эпох подряд — обучение останавливается автоматически. Это позволяет сэкономить время и не допустить переобучения.

---

## Что добавлено по сравнению с лабораторной работой №1

В первой лабораторной работе были реализованы базовые инструменты: класс матриц, работа с CSV файлами и визуализация данных. Во второй лабораторной работе на их основе была построена полноценная модель нейронной сети:

**Новые компоненты:**
- Класс `Layer` — слой нейронной сети с настраиваемой функцией активации
- Класс `NeuralNetwork` — модель с произвольной архитектурой, обучением и предсказанием
- Алгоритм backpropagation с поддержкой произвольного числа слоёв

**Новые утилиты:**
- `minMaxNormalization` — нормализация входных данных к диапазону [0, 1]
- `accuracy` и `rocAuc` — метрики точности модели
- функции активации, функции потерь и их производные 

---

## Доступные функции активации

| Функция | Прямой проход | Производная |
|---|---|---|
| Sigmoid | `nnlab::sigmoid` | `nnlab::sigmoidDerivative` |
| ReLU | `nnlab::relu` | `nnlab::reluDerivative` |
| Tanh | `nnlab::tanh` | `nnlab::tanhDerivative` |

## Доступные функции потерь

| Функция | Прямой проход | Производная |
|---|---|---|
| MSE | `nnlab::mseLoss` | `nnlab::mseDerivative` |
| Binary Cross-Entropy | `nnlab::bceLoss` | `nnlab::bceDerivative` |