typedef struct CubeDirection {
    int d;
} CubeDirection;

typedef struct MaterialKind {
    int k;
} MaterialKind;

typedef struct Panel {
    CubeDirection dir;
    MaterialKind mat;
} Panel;