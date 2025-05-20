Goku is a 2D (SFML) game engine whose target is showcase of C++ design patterns used usually in video games.
Goku aka Dragon ball character.

![Alt text](Image%2025-02-2025%20at%2000.18.jpeg)

Patterns such as Singleton, State, and Flyweight are implemented for modularity and scalability.
Inspired by Entity-Component-System (ECS) architecture, Entities ar eused in the game loop in order to enhance performance and flexibility.
A state machine pattern is implemented to handle character animations efficiently.

Leveraging use of multithreaded code in the game loop. 

- Tools such as ImGui are used for real-time visualization and adjustment of game parameters and entity states.  
- Google Test for unit tests.  
- Google Benchmark for testing performance on container classes.  

Clang 19.7  
SFML 3  
CMake 3.28  
