import matplotlib.pyplot as plt
import matplotlib.cm as cm
import pandas as pd
import os


def plot_answer(path):
    """
    path - ссылка на .csv

    формат .csv :

    x,y,c\n
    0,0,0\n
    ...
    """
    colors = cm.tab10.colors

    df = pd.read_csv(path)
    print(f"Работаем с {path}")
    print(df['c'].unique())

    for i in range(max(df['c'])):
        df0 = df[df["c"] == i]
        plt.scatter(df0["x"], df0["y"], c=colors[i % len(colors)])

    plt.xlabel("X")
    plt.ylabel("Y")
    plt.title("Визуализация")
    plt.grid(True)   # включаем сетку
    plt.show()

def main():
    script_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))  # папка скрипта
    file_path = os.path.join(script_dir, "data", "answerAI.csv")
    plot_answer(file_path)

if __name__ == "__main__":
    main()
