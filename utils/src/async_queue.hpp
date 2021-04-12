
#include <memory> // unique_ptr

namespace sizes {
// todo: move to a separate header file

const int Kilo = ((size_t)1024);
const int Mega = KB * KB;
const int Giga = MB * KB;

};

template<int N>
struct Range {
    // todo: move to a separate header file
    constexpr A() : arr() {
        for (auto i = 0; i != N; ++i)
            arr[i] = i; 
    }
    constexpr int val[N];
};


template<typename QueueDataType>
class AsyncQueue {

    namespace big_dataset {
        const int SINGLE_VECTOR_CAPACITY Kilo;
        const int VECTOR_GRID_CAPACITY = 32;

        template<typename VectorDataType>
        class vector {
            private:

            std::unique_ptr<VectorDataType> grid[VECTOR_GRID_CAPACITY];

            public:

            class AllocationFailedException : public std::exception {};

            explicit vector () {
                for (auto i : Range<VECTOR_GRID_CAPACITY>::val) {
                    try {
                        grid[i] = new VectorDataType[SINGLE_VECTOR_CAPACITY];
                    }
                    catch (const std::bad_alloc& e) {
                        std::cerr << "Allocation failed: " << e.why() << endl;

                        for (auto j : Range<i>) {
                            delete grid[j];
                        }

                        throw AllocationFailedException();
                    }
                }
            }

            ~vector () {
                for (auto i : Range<VECTOR_GRID_CAPACITY>) {
                    delete grid[i];
                }
            }

            vector(const vector&) = delete;
            vector& operator=(const vector&) = delete;

            vector(vector&& src);
            vector& operator=(vector&& src);

        };
    };

    class Packet {
        
    };

    class Task {
        big_dataset::vector<Packet> batch;

        public:
        Task(big_dataset::vector<Packet>&& batch);

        void write_batch () const;

        Task(Task&& src);
        Task(const Task&) = delete;
        Task& operator=(Task&& src);
        Task& operator=(const Task&) = delete;
        
        ~Task();
    };

    std::queue<Task> queue;
    thread_t logger_thread;
    
};