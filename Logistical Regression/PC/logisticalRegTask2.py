import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split


class LogisticalRegression():
    def __init__(self, learning_rate, iterations):
        self.learning_rate = learning_rate
        self.iterations = iterations

    def fit(self, X, Y):
        self.m, self.n = X.shape

        # Initializing weights
        self.W = np.zeros(self.n)
        self.b = 0
        self.X = X
        self.Y = Y.reshape(-1, )  # Ensure Y is a 1D array

        for i in range(self.iterations):
            self.update_weights()
        return self

    def update_weights(self):
        A = 1 / (1 + np.exp(-(self.X.dot(self.W) + self.b)))

        # Calculating gradients
        tmp = (A - self.Y)
        tmp = tmp.reshape(self.m, )
        dW = np.dot(self.X.T, tmp) / self.m  # Normalize by the number of samples
        db = np.sum(tmp) / self.m

        self.W = self.W - self.learning_rate * dW
        self.b = self.b - self.learning_rate * db

        return self

    def predict(self, X):
        Z = 1 / (1 + np.exp(-(X.dot(self.W) + self.b)))
        Y = np.where(Z > 0.5, 1, 0)
        return Y


def main():
    df = pd.read_csv("diabetes.csv")
    X = df.iloc[:, :-1].values
    Y = df.iloc[:, -1].values  # Ensure Y is extracted as a 1D array

    X_train, X_test, Y_train, Y_test = train_test_split(X, Y, test_size=1 / 3, random_state=0)

    model = LogisticalRegression(0.0001, 1000)
    model.fit(X_train, Y_train)
    Y_pred = model.predict(X_test)

    # Measuring performance
    correctly_classified = 0

    # Counter
    count = 0
    for count in range(len(Y_pred)):
        if Y_test[count] == Y_pred[count]:
            correctly_classified += 1

    print("Accuracy on test set by our model:  ", (correctly_classified / len(Y_pred)) * 100)

    print(f"Predicted: {Y_pred} \nExpected: {Y_test}")


if __name__ == "__main__":
    main()
