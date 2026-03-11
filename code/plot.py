import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import sys

def plot_answer(path):
    """Визуализация данных из CSV"""
    colors = cm.tab10.colors
    df = pd.read_csv(path)
    print(f"Работаем с {path}")
    
    # Исправил цикл, чтобы он надежно перебирал все уникальные классы
    for i in df['c'].unique():
        df0 = df[df["c"] == i]
        # Берем цвет из палитры (i % len(colors) не даст выйти за пределы списка цветов)
        color_index = int(i) % len(colors)
        plt.scatter(df0["x"], df0["y"], color=colors[color_index], label=f'Class {i}')

    plt.xlabel("X")
    plt.ylabel("Y")
    plt.title("Визуализация")
    plt.grid(True)
    plt.legend()
    plt.show()

# Эта часть ОЧЕНЬ ВАЖНА. 
# Она принимает путь к файлу, который передает программа на C++
if __name__ == "__main__":
    if len(sys.argv) > 1:
        csv_path = sys.argv[1]
        plot_answer(csv_path)
    else:
        print("Ошибка: Путь к файлу не передан!")