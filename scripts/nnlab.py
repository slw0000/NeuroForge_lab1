import numpy as np
import nnlab_py
import pandas as pd
from sklearn.model_selection import train_test_split

import random

#билд проекта
# cd C:\Users\yesia\CLionProjects\NeuroForge
# Remove-Item -Recurse -Force build
# mkdir build
# cd build
# cmake .. -G "Visual Studio 17 2022"
# cmake --build . --config Release


file_id = input("\n Enter file's id from google drive: \n")
url = f'https://drive.google.com/uc?export=download&id={file_id}'

data = pd.read_csv(url)

target_column = 'target'
X = data.drop(target_column, axis=1)
y = data[target_column]

# X, y = nnlab_py.min_max_normalization(X, y)

X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42,
)
## датасет 2
model = nnlab_py.NeuralNetwork(
    structure=[4, 48, 64, 32, 1],
    activations=[nnlab_py.tanh, nnlab_py.tanh, nnlab_py.tanh, nnlab_py.sigmoid],
    activations_deriv=[nnlab_py.tanhDerivative, nnlab_py.tanhDerivative, nnlab_py.tanhDerivative, nnlab_py.sigmoidDerivative]
)


# датасет 1
# model = nnlab_py.NeuralNetwork(
#     structure=[2, 16, 16, 1],
#     activations=[nnlab_py.tanh, nnlab_py.tanh, nnlab_py.sigmoid],
#     activations_deriv=[nnlab_py.tanhDerivative, nnlab_py.tanhDerivative, nnlab_py.sigmoidDerivative]
# )
print("Model is created")


model.train(X_train, y_train,
            loss_func=nnlab_py.mseLoss,
            loss_deriv=nnlab_py.mseDerivative,
            max_epochs=600, lr=0.05, patience=20)
print("Learning is done")


preds = model.predict(X_test)
accuracy = np.mean(preds == y_test)
print(f"Accuracy on learning data: {accuracy:.3f}")

proba = model.predict_proba(X_test[:5])
print(f"Probabilities for 5 first elements: {proba}")