import numpy as np
import nnlab_py
import pandas as pd
from sklearn.model_selection import train_test_split



flag = input("\nEnter 1 for 1st dataset, 2 for 2nd or smth else for another: \n")

if flag == '1':
    model = nnlab_py.NeuralNetwork(
        structure=[2, 16, 16, 1],
        activations=[nnlab_py.tanh, nnlab_py.tanh, nnlab_py.sigmoid],
        activations_deriv=[nnlab_py.tanhDerivative, nnlab_py.tanhDerivative, nnlab_py.sigmoidDerivative]
    )
    url = 'https://drive.google.com/uc?export=download&id=16VzwtrZRP9QScMdueb3BK0fvZtP8a6sH'
    data = pd.read_csv(url)
elif flag == '2':
    model = nnlab_py.NeuralNetwork(
        structure=[4, 48, 64, 32, 1],
        activations=[nnlab_py.tanh, nnlab_py.tanh, nnlab_py.tanh, nnlab_py.sigmoid],
        activations_deriv=[nnlab_py.tanhDerivative, nnlab_py.tanhDerivative, nnlab_py.tanhDerivative, nnlab_py.sigmoidDerivative]
    )
    url = 'https://drive.google.com/uc?export=download&id=13npPTtGGiKyug7fEKyvaZZbFavFkhGLm'
    data = pd.read_csv(url)
else:
    file_id = input("\n Enter file's id from google drive: \n")
    url = f'https://drive.google.com/uc?export=download&id={file_id}'
    data = pd.read_csv(url)

    struc = [data.shape[1] - 1] + list(map(int, input("Enter layers from 2 to N as: \n l2 l3 l4 ... \n ").split()))
    act = []
    act_der = []
    print('Let`s make list of activation funcs: \n')
    for i in range(len(struc) - 1):
        func = input("Enter 1 for tanh, 2 for relu, smth else for sigmoid: \n")
        if func == '1':
            act.append(nnlab_py.tanh)
            act_der.append(nnlab_py.tanhDerivative)
        elif func == '2':
            act.append(nnlab_py.relu)
            act_der.append(nnlab_py.reluDerivative)
        else:
            act.append(nnlab_py.sigmoid)
            act_der.append(nnlab_py.sigmoidDerivative)

    model = nnlab_py.NeuralNetwork(structure=struc, activations=act, activations_deriv=act_der)


print("Model is created")


target_column = 'target'
X = data.drop(target_column, axis=1)
y = data[target_column]

X_raw = np.ascontiguousarray(X, dtype=np.float64)
y_raw = np.ascontiguousarray(y, dtype=np.int32)

X, y = nnlab_py.min_max_normalization(X_raw, y_raw)

X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42,
)



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