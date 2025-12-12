#pragma once // Evita inclusiones múltiples del archivo

// Constantes de tamaño y dimensiones
constexpr unsigned char CELL_SIZE = 16; // Tamaño de cada celda del mapa en píxeles (16x16)
//estipula los frames por segundo
constexpr unsigned char CROCODILE_ANIMATION_SPEED = 32; // Velocidad de animación del cocodrilo (frames entre cambios)
constexpr unsigned char FAST_SPEED = 3; // Velocidad rápida de movimiento (3 píxeles por frame)
constexpr unsigned char FRONT_HEIGHT = 16; // Altura del frente/encabezado en píxeles
constexpr unsigned char FROG_ANIMATION_SPEED = 8; // Velocidad de animación de la rana (frames entre cambios)
constexpr unsigned char MAP_WIDTH = 16; // Ancho del mapa en celdas
constexpr unsigned char MAP_HEIGHT = 15; // Alto del mapa en celdas
constexpr unsigned char NORMAL_SPEED = 2; // Velocidad normal de movimiento (2 píxeles por frame)
constexpr unsigned char SCREEN_RESIZE = 2; // Factor de escalado de la pantalla (2x)
constexpr unsigned char SLOW_SPEED = 1; // Velocidad lenta de movimiento (1 píxel por frame)
constexpr unsigned char TURTLE_LEVELS = 8; // Número total de niveles con tortugas
constexpr unsigned char TURTLE_ANIMATION_SPEED = 16; // Velocidad de animación de tortugas (frames entre cambios)
constexpr unsigned char TURTLE_DIVING_SPEED = 32; // Velocidad de inmersión de tortugas (frames para sumergirse)

// Constantes de temporización del juego
constexpr unsigned short TIMER_INITIAL_DURATION = 4096; // Duración inicial del temporizador por nivel
constexpr unsigned short TIMER_REDUCTION = 512; // Reducción del tiempo en cada nivel progresivo

// Duración de cada frame en microsegundos (aproximadamente 60 FPS)
constexpr std::chrono::microseconds FRAME_DURATION(16667); // 16667 microsegundos = ~60 FPS (1000000/60)

