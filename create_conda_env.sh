#!/bin/bash

# Ensure the script stops on error
set -e

# Define the environment name
ENV_NAME="triton_shared_mlir_nv"
# Default CUDA version if not provided as an argument
CUDA_VERSION=${1:-12.1}

# Check if conda is installed
if ! command -v conda &> /dev/null; then
    echo "Error: conda is not installed. Please install conda first."
    exit 1
fi

# Function to install dependencies
install_dependencies() {
    echo "Installing dependencies into environment $ENV_NAME..."
    conda install -n "$ENV_NAME" scipy lit gcc libgcc libstdcxx-ng cmake pybind11 ninja pytest pandas matplotlib setuptools requests numpy wheel sympy -c conda-forge -y || {
        echo "Failed to install dependencies."
        exit 1
    }
}

# Function to install torch
install_torch() {
    echo "Installing pytorch 2.4.1 with CUDA..."
    conda install -n "$ENV_NAME" pytorch=2.4.1 torchvision torchaudio pytorch-cuda="$CUDA_VERSION" -c pytorch -c nvidia -y || {
        echo "Failed to install pytorch."
        exit 1
    }
}

# Check if the environment already exists
if conda info --envs | grep -q "^$ENV_NAME\s"; then
    echo "Environment $ENV_NAME already exists. Do you want to recreate it? (y/n)"
    read -r answer
    if [[ "$answer" != "y" ]]; then
        echo "Script terminated."
        exit 0
    else
        echo "Removing existing environment $ENV_NAME..."
        conda remove --name "$ENV_NAME" --all -y || { echo "Failed to remove existing environment."; exit 1; }
    fi
fi

# Check if torch 2.4.* exists in base environment
TORCH_VERSION=$(conda list -n base torch | grep torch | awk '{print $2}')
if [[ "$TORCH_VERSION" =~ .*2\.4.* ]]; then
    echo "Found torch $TORCH_VERSION in base environment."

    # Clone base environment
    echo "Cloning base environment to $ENV_NAME..."
    conda create --name "$ENV_NAME" --clone base -y || { echo "Failed to clone environment."; exit 1; }

    # Install additional dependencies
    install_dependencies

else
    # Create a new Conda environment
    echo "Creating a new Conda environment: $ENV_NAME..."
    conda create -n "$ENV_NAME" python=3.10 -y || { echo "Failed to create environment and install dependencies."; exit 1; }

    # Install dependencies and pytorch with CUDA
    install_dependencies
    install_torch
fi

# Activate the environment
echo "Activating environment: $ENV_NAME"
source activate "$ENV_NAME" || { echo "Failed to activate environment."; exit 1; }

# Completion message
echo "Environment $ENV_NAME has been created and all dependencies have been installed successfully!"

echo "
# To activate this environment, use
#
#     $ conda activate triton_shared_mlir_nv
#
# To deactivate an active environment, use
#
#     $ conda deactivate
"