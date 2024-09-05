# In[1]:
import numpy as np
import matplotlib.pyplot as plt
import tensorflow as tf

# In[2]:
# Load the MNIST dataset.


mnist = tf.keras.datasets.mnist
train_data, test_data = mnist.load_data(path = 'mnist.npz')

# In[3]:
# Divide the data into features and labels.
train_images, train_labels = train_data
test_images, test_labels = test_data

# In[4]:
# Reshape and normalize the images.
X_train = train_images.reshape((60000, 784))
X_train = X_train.astype('float32') / 255
X_test = test_images.reshape((10000, 784))
X_test = X_test.astype('float32') / 255

# In[5]:
# Reshape the labels and encode them categorically.
y_train = tf.keras.utils.to_categorical(train_labels)
y_test = tf.keras.utils.to_categorical(test_labels)

# In[6]:
# Show the shapes of the data.
print("Training Images:", X_train.shape)
print("Testing Images:", X_test.shape)
print("Training Labels:", y_train.shape)
print("Test Labels:", y_test.shape)

# In[7]:
# Show a sample MNIST digit.
plt.imshow(train_images[10])
plt.show()

# In[8]:
# Define the sequential model.
model = tf.keras.models.Sequential()

# In[9]:
# Add two fully-connected layers to the network.
model.add(tf.keras.layers.Dense(20, activation='relu', input_shape=(28 * 28,)))
model.add(tf.keras.layers.Dense(10, activation='softmax'))

# In[10]:
# Show the model.
model.summary()

# In[11]:
# Compile the model.
model.compile(optimizer='adam', loss='categorical_crossentropy', metrics=['accuracy'])

# In[12]:
# Define the parameters.
num_epochs = 10
batch_size = 256

# Train the model.
history = model.fit(X_train,
                      y_train,
                      epochs=num_epochs,
                      batch_size=batch_size,
                      validation_data=(X_test, y_test))

# In[13]:
test_loss, test_accuracy = model.evaluate(X_test, y_test)
print('Test Accuracy:', test_accuracy)
print('Test Loss:', test_loss)

# In[14]:
# Save the metrics.
metrics = history.history

# In[15]:
# Save the loss values.
training_loss_list = metrics['loss']
test_loss_list = metrics['val_loss']

# In[16]:
# Plot the training and test loss.
x = np.arange(0, num_epochs, 1)
plt.title('Training and Test Loss')
plt.xlabel('Epoch')
plt.ylabel('Loss')
plt.plot(x, training_loss_list, label='Training Loss')
plt.plot(x, test_loss_list, label='Test Loss')
plt.legend()
plt.show()

# In[17]:
train_accuracy_list = metrics['accuracy']
test_accuracy_list = metrics['val_accuracy']

# In[18]:
plt.title('Training and Test Accuracy')
plt.xlabel('Epoch')
plt.ylabel('Accuracy')
plt.plot(x, train_accuracy_list, label='Training Accuracy')
plt.plot(x, test_accuracy_list, label='Test Accuracy')
plt.legend()
plt.show()

# In[19]:
# Make predictions with the trained model.
predictions = model.predict(X_test)

# In[20]:
# Choose an index.
index = 45

# Show an image from the test set.
plt.imshow(test_images[index])
plt.show()

# In[21]:
print("Prediction:", np.argmax(predictions[index]))

model.save_weights('mnistNN.weights.h5')


