#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef struct Point {
    int x, y;
} Point;

typedef struct Quadtree {
    int xmin, ymin, xmax, ymax;              
    Point* points;               
    int point_count;
	int cap;
    struct Quadtree* quadrants[4];
} Quadtree;

Quadtree* createQuadtree(int xmin, int ymin, int xmax, int ymax, int cap);
void divide(Quadtree* qt);
bool insert(Quadtree* qt, Point p);
bool checkBoundary(Quadtree* qt, Point p);
bool intersectsCircle(Quadtree* qt, int cx, int cy, int radius);
int checkCircle(Quadtree* qt, int cx, int cy, int radius);
double distance(double x1, double y1, double x2, double y2);
void freeQuadtree(Quadtree* qt);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <points file>\n", argv[0]);
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    Quadtree* qt = createQuadtree(-1000, -1000, 1000, 1000, 4);

    int x, y;
    while (fscanf(file, "%d %d", &x, &y) == 2) {
        Point p = {x, y};
        insert(qt, p);
    }

    fclose(file);

    int cx, cy, r;
    while (scanf("%d %d %d", &cx, &cy, &r) != EOF) {
        int count = checkCircle(qt, cx, cy, r);
        printf("%d\n", count);
    }

    freeQuadtree(qt);

    return 0;
}

Quadtree* createQuadtree(int xmin, int ymin, int xmax, int ymax, int cap) {
    Quadtree* qt = (Quadtree*)malloc(sizeof(Quadtree));
    qt->xmin = xmin;
    qt->ymin = ymin;
    qt->xmax = xmax;
    qt->ymax = ymax;
    qt->cap = cap;
    qt->points = (Point*)malloc(sizeof(Point) * cap);
    qt->point_count = 0;
    for (int i = 0; i < 4; i++) {
        qt->quadrants[i] = NULL;
    }
    return qt;
}

void divide(Quadtree* qt) {
    double midx = (qt->xmin + qt->xmax) / 2.0;
    double midy = (qt->ymin + qt->ymax) / 2.0;

    qt->quadrants[0] = createQuadtree(qt->xmin, qt->ymin, midx, midy, qt->cap); 
    qt->quadrants[1] = createQuadtree(midx, qt->ymin, qt->xmax, midy, qt->cap);
    qt->quadrants[2] = createQuadtree(qt->xmin, midy, midx, qt->ymax, qt->cap); 
    qt->quadrants[3] = createQuadtree(midx, midy, qt->xmax, qt->ymax, qt->cap);
}

bool insert(Quadtree* qt, Point p) {
    if (!checkBoundary(qt, p)) return false;

    if (qt->point_count < qt->cap) {
        qt->points[qt->point_count++] = p;
        return true;
    }

    if (qt->quadrants[0] == NULL) divide(qt);

    for (int i = 0; i < 4; i++) {
        if (insert(qt->quadrants[i], p)) return true;
    }

    return false;
}

bool checkBoundary(Quadtree* qt, Point p) {
    return (p.x >= qt->xmin && p.x <= qt->xmax && p.y >= qt->ymin && p.y <= qt->ymax);
}

bool intersectsCircle(Quadtree* qt, int cx, int cy, int radius) {
	int closestX = (cx < qt->xmin) ? qt->xmin : (cx > qt->xmax) ? qt->xmax : cx;
	int closestY = (cy < qt->ymin) ? qt->ymin : (cy > qt->ymax) ? qt->ymax : cy;
    return distance(closestX, closestY, cx, cy) <= radius * radius;
}

int checkCircle(Quadtree* qt, int cx, int cy, int radius) {
    if (!intersectsCircle(qt, cx, cy, radius)) return 0;

    int count = 0;
    for (int i = 0; i < qt->point_count; i++) {
        if (distance(qt->points[i].x, qt->points[i].y, cx, cy) <= radius * radius) {
            count++;
        }
    }

    if (qt->quadrants[0] != NULL) {
        for (int i = 0; i < 4; i++) {
            count += checkCircle(qt->quadrants[i], cx, cy, radius);
        }
    }

    return count;
}

double distance(double x1, double y1, double x2, double y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

void freeQuadtree(Quadtree* qt) {
    if (qt->quadrants[0] != NULL) {
        for (int i = 0; i < 4; i++) {
			if (qt->quadrants[i] != NULL) {
				freeQuadtree(qt->quadrants[i]);
			}
        }
    }
	if (qt->points != NULL) {
		free(qt->points);
	}
    
	free(qt);
}
