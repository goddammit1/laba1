#include <iostream>
#include <cstring>

struct Node {
    size_t priority;
    char* data;
};

class BinaryHeap {
private:
    Node* heap;
    size_t virtualSize;
    size_t physicalSize;

    void heapifyUp(size_t index) {
        size_t parent = (index - 1) / 2;
        while (index > 0 && heap[index].priority < heap[parent].priority) {
            std::swap(heap[index], heap[parent]);
            index = parent;
            parent = (index - 1) / 2;
        }
    }

    void heapifyDown(size_t index) {
        size_t left = 2 * index + 1;
        size_t right = 2 * index + 2;
        size_t smallest = index;

        if (left < virtualSize && heap[left].priority < heap[smallest].priority) {
            smallest = left;
        }
        if (right < virtualSize && heap[right].priority < heap[smallest].priority) {
            smallest = right;
        }

        if (smallest != index) {
            std::swap(heap[index], heap[smallest]);
            heapifyDown(smallest);
        }
    }

public:
    BinaryHeap() : heap(new Node[16]), virtualSize(0), physicalSize(16) {}

    BinaryHeap(const BinaryHeap& other) : heap(new Node[other.physicalSize]), virtualSize(other.virtualSize), physicalSize(other.physicalSize) {
        std::copy(other.heap, other.heap + other.virtualSize, heap);
    }

    BinaryHeap& operator=(const BinaryHeap& other) {
        if (&other != this) {
            Node* newHeap = new Node[other.physicalSize];
            std::copy(other.heap, other.heap + other.virtualSize, newHeap);
            delete[] heap;
            heap = newHeap;
            virtualSize = other.virtualSize;
            physicalSize = other.physicalSize;
        }
        return *this;
    }

    ~BinaryHeap() {
        delete[] heap;
    }

    void addValue(size_t priority, char* data) {
        if (virtualSize == physicalSize) {
            physicalSize *= 2;
            Node* newHeap = new Node[physicalSize];
            std::copy(heap, heap + virtualSize, newHeap);
            delete[] heap;
            heap = newHeap;
        }
        heap[virtualSize].priority = priority;
        heap[virtualSize].data = data;
        heapifyUp(virtualSize);
        ++virtualSize;
    }

    void removeMin() {
        if (virtualSize == 0) {
            return;
        }
        heap[0] = heap[virtualSize - 1];
        --virtualSize;
        heapifyDown(0);
    }

    Node findMin() {
        if (virtualSize == 0) {
            return Node{0, nullptr};
        }
        return heap[0];
    }

    void merge(const BinaryHeap& other) {
        for (size_t i = 0; i < other.virtualSize; ++i) {
            addValue(other.heap[i].priority, other.heap[i].data);
        }
    }

    void print() {
        BinaryHeap tempHeap(*this);

        std::cout << "Элементы в порядке приоритета:" << std::endl;
        while (tempHeap.virtualSize > 0) {
            Node minNode = tempHeap.findMin();
            std::cout << "Приоритет: " << minNode.priority << ", Данные: " << minNode.data << std::endl;
            tempHeap.removeMin();
    }
    }
};

int main() {
    BinaryHeap heap1;
    heap1.addValue(3, "Hello");
    heap1.addValue(1, "World");

    BinaryHeap heap2;
    heap2.addValue(2, "Binary");
    heap2.addValue(4, "Heap");

    heap1.merge(heap2);

    heap1.print();
    heap1.removeMin();
    heap1.print();


    return 0;
}
