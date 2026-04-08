import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.cm as cm
import sys

def plot_answer(path, lega = 0):
    """
    Визуализация данных из CSV
    
    path:: передаем ссылку на .csv (пример data/test_data.csv)
    lega:: если требуетя отобразить легенду то передаем число не равное 0
    """
    try:
        df = pd.read_csv(path)
    except Exception as e:
        print(f"Ошибка чтения CSV: {e}")
        return 1

    print(f"Работаем с {path}")

    try:
        if len(df.columns) == 2:
            plt.scatter(df["x"], df["y"])

        else:
            colors = cm.tab10.colors
            for i in df['label'].unique():
                df0 = df[df["label"] == i]
                # Берем цвет из палитры (i % len(colors) не даст выйти за пределы списка цветов)
                plt.scatter(df0["x"], df0["y"], color=colors[i % len(colors)], label=f'Class {i}')

    except Exception as e:
        print(f"Ошибка построения графика: {e}")
        return 1

    plt.xlabel("X")
    plt.ylabel("Y")
    plt.title("Визуализация")
    plt.grid(True)
    if lega != 0 and len(df.columns) == 3:
        plt.legend()
    plt.show()


# Эта часть принимает путь к файлу, который передает программа на C++
if __name__ == "__main__":
    if len(sys.argv) > 1:
        csv_path = sys.argv[1]

        lega = 0
        if len(sys.argv) > 2:
            try:
                lega = int(sys.argv[2])
            except:
                print("Аргумент legend должен быть числом")

        exit(plot_answer(csv_path, lega))
    else:
        print("Ошибка: Путь к файлу не передан!")