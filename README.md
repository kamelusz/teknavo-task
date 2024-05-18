## Limitations

- coefficients of the equation must be represented by integers;
- overflow of variables during the calculation is not verify;
- calculation of roots with an imaginary part is not implemented.

## Implementation details

To find the roots and extremum of a function, three execution contexts are created:

- [that processes input data and an instatiates entity for calculation](https://github.com/kamelusz/teknavo-task/tree/main/src/solver#L35);
- [that calculates the roots and extremum of the equation](https://github.com/kamelusz/teknavo-task/tree/main/src/solver#L36);
- [that responsible for the output of information](https://github.com/kamelusz/teknavo-task/tree/main/src/solver#L37).

Each [execution context](https://github.com/kamelusz/teknavo-task/blob/main/src/mt/execution_context.hpp#L8) is represented by a [separate thread](https://github.com/kamelusz/teknavo-task/blob/main/src/mt/execution_context.hpp#L26) and its associated [task queue](https://github.com/kamelusz/teknavo-task/blob/main/src/mt/execution_context.hpp#L25).

Information is transferred between threads using multiple producers single consumer queues. The [queue](https://github.com/kamelusz/teknavo-task/blob/main/src/mt/task_queue.hpp#L88) is implemented in such a way that it transfers not pure data, but functional entities: [functors](https://github.com/kamelusz/teknavo-task/blob/main/src/mt/test/mt.cpp#L26), [lambdas](https://github.com/kamelusz/teknavo-task/blob/main/src/mt/test/mt.cpp#L16), [functions](https://github.com/kamelusz/teknavo-task/blob/main/src/mt/test/mt.cpp#L68).

Two intrusive lists are used inside the queue to store functional entities. The [first one](https://github.com/kamelusz/teknavo-task/blob/main/src/mt/task_queue.hpp#L126) is used by producers to [insert tasks under the lock](https://github.com/kamelusz/teknavo-task/blob/main/src/mt/task_queue.cpp#L39). The [second one](https://github.com/kamelusz/teknavo-task/blob/main/src/mt/task_queue.hpp#L127) is used by the consumer to [read and perform tasks](https://github.com/kamelusz/teknavo-task/blob/main/src/mt/task_queue.cpp#L5). If the second queue does not contain data, then the [consumer swaps queues under the lock](https://github.com/kamelusz/teknavo-task/blob/main/src/mt/task_queue.cpp#L48).

Coordination of tasks between threads is performed by the [execution manager](https://github.com/kamelusz/teknavo-task/blob/main/src/solver/execution_manager.hpp#L16). it also limits the number of tasks ([1](https://github.com/kamelusz/teknavo-task/blob/main/src/solver/execution_manager.cpp#L69), [2](https://github.com/kamelusz/teknavo-task/blob/main/src/solver/execution_manager.cpp#L107)) so as not to out of memory in the case of a large data set.

## Generate Visual Studio solution

Just chek out this repo, create separate folder for generated files and execute command like that:

```
D:\dev\teknavo-task\build>cmake --version
cmake version 3.27.6

CMake suite maintained and supported by Kitware (kitware.com/cmake).

D:\dev\teknavo-task\build>cmake -G "Visual Studio 17 2022" PATH-TO-SOURCE-CODE
```
