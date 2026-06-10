#include <stdio.h>
#include <stdlib.h>

#define ROWS 25
#define COLS 60
#define MAX_OBJECTS 50

#define EMPTY '_'
#define DRAW '*'

typedef enum {
    CIRCLE = 1,
    RECTANGLE,
    LINE,
    TRIANGLE
} ShapeType;

typedef struct {
    int active;
    ShapeType type;
    int x1, y1;
    int x2, y2;
    int x3, y3;
    int radius;
} Object;

void initializeCanvas(char canvas[ROWS][COLS]);
void displayPicture(char canvas[ROWS][COLS]);
void plotPoint(char canvas[ROWS][COLS], int x, int y);
void drawCircle(char canvas[ROWS][COLS], int cx, int cy, int r);
void drawRectangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2);
void drawLine(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2);
void drawTriangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2, int x3, int y3);
void renderObject(char canvas[ROWS][COLS], Object obj);
void redrawCanvas(char canvas[ROWS][COLS], Object objects[MAX_OBJECTS]);
void addObject(Object objects[MAX_OBJECTS], char canvas[ROWS][COLS]);
void deleteObject(Object objects[MAX_OBJECTS], char canvas[ROWS][COLS]);
void modifyObject(Object objects[MAX_OBJECTS], char canvas[ROWS][COLS]);
void listObjects(Object objects[MAX_OBJECTS]);
void clearAllObjects(Object objects[MAX_OBJECTS], char canvas[ROWS][COLS]);
void readObjectData(Object *obj);
int readInt(const char *message);
int absValue(int n);
void swapInt(int *a, int *b);
const char *shapeName(ShapeType type);

int main() {
    char canvas[ROWS][COLS];
    Object objects[MAX_OBJECTS] = {0};
    int choice;

    initializeCanvas(canvas);

    do {
        printf("\n========== 2D GRAPHICS EDITOR ==========");
        printf("\n1. Add object");
        printf("\n2. Delete object");
        printf("\n3. Modify object");
        printf("\n4. Display picture");
        printf("\n5. List objects");
        printf("\n6. Clear all objects");
        printf("\n7. Exit");
        printf("\n========================================");
        printf("\nCanvas size: %d rows x %d columns", ROWS, COLS);
        printf("\nUse x = column number and y = row number.");
        printf("\nEnter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:
                addObject(objects, canvas);
                break;
            case 2:
                deleteObject(objects, canvas);
                break;
            case 3:
                modifyObject(objects, canvas);
                break;
            case 4:
                displayPicture(canvas);
                break;
            case 5:
                listObjects(objects);
                break;
            case 6:
                clearAllObjects(objects, canvas);
                break;
            case 7:
                printf("Exiting 2D Graphics Editor.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 7);

    return 0;
}

void initializeCanvas(char canvas[ROWS][COLS]) {
    int i, j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            canvas[i][j] = EMPTY;
        }
    }
}

void displayPicture(char canvas[ROWS][COLS]) {
    int i, j;

    printf("\n-------------------- PICTURE --------------------\n");
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            putchar(canvas[i][j]);
        }
        putchar('\n');
    }
    printf("-------------------------------------------------\n");
}

void plotPoint(char canvas[ROWS][COLS], int x, int y) {
    if (x >= 0 && x < COLS && y >= 0 && y < ROWS) {
        canvas[y][x] = DRAW;
    }
}

void drawCircle(char canvas[ROWS][COLS], int cx, int cy, int r) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    if (r <= 0) {
        plotPoint(canvas, cx, cy);
        return;
    }

    while (y >= x) {
        plotPoint(canvas, cx + x, cy + y);
        plotPoint(canvas, cx - x, cy + y);
        plotPoint(canvas, cx + x, cy - y);
        plotPoint(canvas, cx - x, cy - y);
        plotPoint(canvas, cx + y, cy + x);
        plotPoint(canvas, cx - y, cy + x);
        plotPoint(canvas, cx + y, cy - x);
        plotPoint(canvas, cx - y, cy - x);

        x++;

        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

void drawRectangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2) {
    int x, y;

    if (x1 > x2) swapInt(&x1, &x2);
    if (y1 > y2) swapInt(&y1, &y2);

    for (x = x1; x <= x2; x++) {
        plotPoint(canvas, x, y1);
        plotPoint(canvas, x, y2);
    }

    for (y = y1; y <= y2; y++) {
        plotPoint(canvas, x1, y);
        plotPoint(canvas, x2, y);
    }
}

void drawLine(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2) {
    int dx = absValue(x2 - x1);
    int dy = absValue(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int e2;

    while (1) {
        plotPoint(canvas, x1, y1);

        if (x1 == x2 && y1 == y2) {
            break;
        }

        e2 = 2 * err;

        if (e2 > -dy) {
            err = err - dy;
            x1 = x1 + sx;
        }

        if (e2 < dx) {
            err = err + dx;
            y1 = y1 + sy;
        }
    }
}

void drawTriangle(char canvas[ROWS][COLS], int x1, int y1, int x2, int y2, int x3, int y3) {
    drawLine(canvas, x1, y1, x2, y2);
    drawLine(canvas, x2, y2, x3, y3);
    drawLine(canvas, x3, y3, x1, y1);
}

void renderObject(char canvas[ROWS][COLS], Object obj) {
    if (!obj.active) {
        return;
    }

    switch (obj.type) {
        case CIRCLE:
            drawCircle(canvas, obj.x1, obj.y1, obj.radius);
            break;
        case RECTANGLE:
            drawRectangle(canvas, obj.x1, obj.y1, obj.x2, obj.y2);
            break;
        case LINE:
            drawLine(canvas, obj.x1, obj.y1, obj.x2, obj.y2);
            break;
        case TRIANGLE:
            drawTriangle(canvas, obj.x1, obj.y1, obj.x2, obj.y2, obj.x3, obj.y3);
            break;
    }
}

void redrawCanvas(char canvas[ROWS][COLS], Object objects[MAX_OBJECTS]) {
    int i;
    initializeCanvas(canvas);

    for (i = 0; i < MAX_OBJECTS; i++) {
        if (objects[i].active) {
            renderObject(canvas, objects[i]);
        }
    }
}

void addObject(Object objects[MAX_OBJECTS], char canvas[ROWS][COLS]) {
    int i;
    Object newObject;

    for (i = 0; i < MAX_OBJECTS; i++) {
        if (!objects[i].active) {
            readObjectData(&newObject);
            objects[i] = newObject;
            redrawCanvas(canvas, objects);
            printf("Object added successfully with ID %d.\n", i + 1);
            displayPicture(canvas);
            return;
        }
    }

    printf("Object limit reached. Cannot add more objects.\n");
}

void deleteObject(Object objects[MAX_OBJECTS], char canvas[ROWS][COLS]) {
    int id;

    listObjects(objects);
    id = readInt("Enter object ID to delete: ");

    if (id < 1 || id > MAX_OBJECTS || !objects[id - 1].active) {
        printf("Invalid object ID.\n");
        return;
    }

    objects[id - 1].active = 0;
    redrawCanvas(canvas, objects);
    printf("Object deleted successfully.\n");
    displayPicture(canvas);
}

void modifyObject(Object objects[MAX_OBJECTS], char canvas[ROWS][COLS]) {
    int id;
    Object updatedObject;

    listObjects(objects);
    id = readInt("Enter object ID to modify: ");

    if (id < 1 || id > MAX_OBJECTS || !objects[id - 1].active) {
        printf("Invalid object ID.\n");
        return;
    }

    printf("Enter new details for object ID %d.\n", id);
    readObjectData(&updatedObject);
    objects[id - 1] = updatedObject;
    redrawCanvas(canvas, objects);
    printf("Object modified successfully.\n");
    displayPicture(canvas);
}

void listObjects(Object objects[MAX_OBJECTS]) {
    int i;
    int found = 0;

    printf("\n--------------- OBJECT LIST ---------------\n");
    for (i = 0; i < MAX_OBJECTS; i++) {
        if (objects[i].active) {
            found = 1;
            printf("ID %d: %s ", i + 1, shapeName(objects[i].type));

            if (objects[i].type == CIRCLE) {
                printf("Center(%d,%d), Radius=%d", objects[i].x1, objects[i].y1, objects[i].radius);
            } else if (objects[i].type == RECTANGLE || objects[i].type == LINE) {
                printf("Point1(%d,%d), Point2(%d,%d)", objects[i].x1, objects[i].y1, objects[i].x2, objects[i].y2);
            } else if (objects[i].type == TRIANGLE) {
                printf("Point1(%d,%d), Point2(%d,%d), Point3(%d,%d)",
                       objects[i].x1, objects[i].y1,
                       objects[i].x2, objects[i].y2,
                       objects[i].x3, objects[i].y3);
            }
            printf("\n");
        }
    }

    if (!found) {
        printf("No objects added yet.\n");
    }
    printf("-------------------------------------------\n");
}

void clearAllObjects(Object objects[MAX_OBJECTS], char canvas[ROWS][COLS]) {
    int i;

    for (i = 0; i < MAX_OBJECTS; i++) {
        objects[i].active = 0;
    }

    initializeCanvas(canvas);
    printf("All objects cleared successfully.\n");
    displayPicture(canvas);
}

void readObjectData(Object *obj) {
    int type;

    obj->active = 1;
    obj->x1 = obj->y1 = obj->x2 = obj->y2 = obj->x3 = obj->y3 = obj->radius = 0;

    printf("\nChoose object type:\n");
    printf("1. Circle\n");
    printf("2. Rectangle\n");
    printf("3. Line\n");
    printf("4. Triangle\n");
    type = readInt("Enter object type: ");

    while (type < 1 || type > 4) {
        printf("Invalid object type. Try again.\n");
        type = readInt("Enter object type: ");
    }

    obj->type = (ShapeType) type;

    switch (obj->type) {
        case CIRCLE:
            obj->x1 = readInt("Enter center x: ");
            obj->y1 = readInt("Enter center y: ");
            obj->radius = readInt("Enter radius: ");
            while (obj->radius < 0) {
                printf("Radius cannot be negative.\n");
                obj->radius = readInt("Enter radius: ");
            }
            break;

        case RECTANGLE:
            obj->x1 = readInt("Enter top-left x: ");
            obj->y1 = readInt("Enter top-left y: ");
            obj->x2 = readInt("Enter bottom-right x: ");
            obj->y2 = readInt("Enter bottom-right y: ");
            break;

        case LINE:
            obj->x1 = readInt("Enter start x: ");
            obj->y1 = readInt("Enter start y: ");
            obj->x2 = readInt("Enter end x: ");
            obj->y2 = readInt("Enter end y: ");
            break;

        case TRIANGLE:
            obj->x1 = readInt("Enter point 1 x: ");
            obj->y1 = readInt("Enter point 1 y: ");
            obj->x2 = readInt("Enter point 2 x: ");
            obj->y2 = readInt("Enter point 2 y: ");
            obj->x3 = readInt("Enter point 3 x: ");
            obj->y3 = readInt("Enter point 3 y: ");
            break;
    }
}

int readInt(const char *message) {
    int value;

    printf("%s", message);
    while (scanf("%d", &value) != 1) {
        printf("Invalid input. Enter a number: ");
        while (getchar() != '\n');
    }

    return value;
}

int absValue(int n) {
    return (n < 0) ? -n : n;
}

void swapInt(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

const char *shapeName(ShapeType type) {
    switch (type) {
        case CIRCLE:
            return "Circle";
        case RECTANGLE:
            return "Rectangle";
        case LINE:
            return "Line";
        case TRIANGLE:
            return "Triangle";
        default:
            return "Unknown";
    }
}
