#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	int x;
	int y;
} Point;

int CheckInsideCircle(Point point, int xx, int yy, int rad) {
	int dx = point.x - xx;
	int dy = point.y - yy;
	return (rad * rad) >= (dx * dx + dy * dy);
}

int main(int c, char* v[]) {
	FILE* input = fopen(v[1], "r");
	int cap = 100;
	int count = 0;
	Point* points = (Point*)malloc(cap * sizeof(Point));

	int x_pt, y_pt;
	while (fscanf(input, "%d %d", &x_pt, &y_pt) == 2) {
		if(count == cap) {
			cap = cap * 2;
			points = (Point*)realloc(points, cap * sizeof(Point));
		}
		points[count].x = x_pt;
		points[count].y = y_pt;
		count++;
	}
	fclose(input);
	int xx, yy, rad;
	while (scanf("%d %d %d", &xx, &yy, &rad) == 3) {
		int j = 0;
		for(int i = 0; i < count; i++) {
			if(CheckInsideCircle(points[i], xx, yy, rad)) {
				j++;
			}
		}
		printf("%d\n", j);
	}
	free(points);
	return 0;
}


