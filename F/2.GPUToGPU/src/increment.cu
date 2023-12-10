// CUDA kernel to increment each element of the array by 1
__global__ void increment(int *arr, int size)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;

    // Ensure the thread index is within the array bounds
    if (tid < size) {
        arr[tid] += 1;
    }
}

extern "C" void launch_increment(int* array_device, int SIZE)
{
    // Define block and grid dimensions
    int blockSize = 256;
    int gridSize = (SIZE + blockSize - 1) / blockSize;

    // Launch the kernel
    increment<<<gridSize, blockSize>>>(array_device, SIZE);
}