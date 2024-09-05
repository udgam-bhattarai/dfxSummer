import h5py
import numpy as np
import struct


def save_weights_biases_to_bin(h5_file_path, dense_weights_file, dense_biases_file, dense_1_weights_file,
                               dense_1_biases_file):
    with h5py.File(h5_file_path, 'r') as h5_file:
        # Extract weights and biases for the first dense layer
        dense_weights = h5_file['layers']['dense']['vars']['0'][()]
        dense_biases = h5_file['layers']['dense']['vars']['1'][()]

        # Save the weights to a binary file
        with open(dense_weights_file, 'wb') as bin_file:
            dense_weights_flattened = dense_weights.flatten()
            dense_weights_binary = struct.pack('f' * len(dense_weights_flattened), *dense_weights_flattened)
            bin_file.write(dense_weights_binary)

        # Save the biases to a binary file
        with open(dense_biases_file, 'wb') as bin_file:
            dense_biases_flattened = dense_biases.flatten()
            dense_biases_binary = struct.pack('f' * len(dense_biases_flattened), *dense_biases_flattened)
            bin_file.write(dense_biases_binary)

        # Extract weights and biases for the second dense layer
        dense_1_weights = h5_file['layers']['dense_1']['vars']['0'][()]
        dense_1_biases = h5_file['layers']['dense_1']['vars']['1'][()]

        # Save the weights to a binary file
        with open(dense_1_weights_file, 'wb') as bin_file:
            dense_1_weights_flattened = dense_1_weights.flatten()
            dense_1_weights_binary = struct.pack('f' * len(dense_1_weights_flattened), *dense_1_weights_flattened)
            bin_file.write(dense_1_weights_binary)

        # Save the biases to a binary file
        with open(dense_1_biases_file, 'wb') as bin_file:
            dense_1_biases_flattened = dense_1_biases.flatten()
            dense_1_biases_binary = struct.pack('f' * len(dense_1_biases_flattened), *dense_1_biases_flattened)
            bin_file.write(dense_1_biases_binary)


# Example usage
save_weights_biases_to_bin('mnistNN.weights.h5', 'dense_weights.bin', 'dense_biases.bin', 'dense_1_weights.bin',
                           'dense_1_biases.bin')
