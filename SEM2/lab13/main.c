#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define M 1920
#define N 1080
#define SPHERES 4

typedef struct
{
    double x;
    double y;
    double z;
} Vector;

typedef struct
{
    int r;
    int g;
    int b;
} Color;

typedef struct
{
    double r;
    Color h;
    Vector c;
} Sphere;

const Color BLACK = (Color){.r = 0, .g = 0, .b = 0};
// the eye
const Vector eye = {
    0.50,
    0.50,
    -1.00};

// the light
const Vector g = {
    0.00,
    1.25,
    -0.50};

double max(double, double);
double min(double, double);

inline double max(double a, double b) { return a > b ? a : b; }
inline double min(double a, double b) { return a < b ? a : b; }

double dotp(Vector t, Vector u)
{
    return t.x * u.x + t.y * u.y + t.z * u.z;
}

void init(Sphere *a)
{
    a[0].c.x = 0.50;
    a[0].c.y = -20000.00; // the floor
    a[0].c.z = 0.50;
    a[0].r = 20000.25;
    a[0].h.r = 205; // color is Peru
    a[0].h.g = 133;
    a[0].h.b = 63;

    a[1].c.x = 0.50;
    a[1].c.y = 0.50;
    a[1].c.z = 0.50;
    a[1].r = 0.25;
    a[1].h.r = 0; // color is Blue
    a[1].h.g = 0;
    a[1].h.b = 255;

    a[2].c.x = 1.00;
    a[2].c.y = 0.50;
    a[2].c.z = 1.00;
    a[2].r = 0.25;
    a[2].h.r = 0; // color is Green
    a[2].h.g = 255;
    a[2].h.b = 0;

    a[3].c.x = 0.00;
    a[3].c.y = 0.75;
    a[3].c.z = 1.25;
    a[3].r = 0.50;
    a[3].h.r = 255; // color is Red
    a[3].h.g = 0;
    a[3].h.b = 0;
}

void normalize(Vector* v)
{
    double mag = sqrt(pow(v->x, 2) + pow(v->y, 2) + pow(v->z, 2));
    v->x /= mag;
    v->y /= mag;
    v->z /= mag;
}

Vector create_vector(Vector origin, Vector end)
{
    Vector v = (Vector){
        .x = end.x - origin.x,
        .y = end.y - origin.y,
        .z = end.z - origin.z};
    normalize(&v);
    return v;
}

Vector add_vector(Vector a, Vector b)
{
    return (Vector){
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z};
}

Vector subtract_vector(Vector a, Vector b)
{
    return (Vector){
        .x = a.x - b.x,
        .y = a.x - b.y,
        .z = a.z - b.z};
}

Vector scalar_multiply(Vector a, double scalar)
{
    return (Vector){
        .x = scalar * a.x,
        .y = scalar * a.y,
        .z = scalar * a.z};
}

bool cast(Sphere sphere, Vector ray, double* t)
{
    Vector v = subtract_vector(eye, sphere.c);
    Vector d = ray;
    double a = 1.0;
    double b = 2.0 * dotp(v, d);
    double c = dotp(v, v) - sphere.r * sphere.r;

    double det = pow(b, 2) - 4.0 * a * c;

    if (det < 0)
    {
        return false;
    }
    double det_sqrt = sqrt(det);
    double t_plus = (-b + det_sqrt) / 2.0;
    double t_minus = (-b - det_sqrt) / 2.0;

    *t = (t_minus > 0 && t_plus > 0) ? min(t_minus, t_plus) : max(t_minus, t_plus);

    return *t > 0;
}

int main(void)
{
    FILE *fout;
    Color **rgb = malloc(sizeof(Color* ) * N);

    for (int i = 0; i < N; i++)
    {
        rgb[i] = malloc(sizeof(Color) * M);
    }
    Sphere spheres[SPHERES];
    init(spheres);

    double aspect_ratio = (1.0 * M) / (1.0 * N);
    for (int Py = 0; Py < N; Py++)
    {
        for (int Px = 0; Px < M; Px++)
        {
            Vector ray = create_vector(eye, (Vector){
                .x = ((Px + 0.5) / (1.0 * M)) * aspect_ratio,
                .y = (((N - Py) + 0.5) / (1.0 * N)),
                .z = 0});
            double t_min = INFINITY;
            Color c = BLACK;
            for (int s = 0; s < 4; s++)
            {
                Sphere sphere = spheres[s];
                double t;

                if (cast(sphere, ray, &t))
                {
                    if (t > t_min)
                    {
                        continue;
                    }
                    t_min = t;
                    c = sphere.h;
                }
            }
            rgb[Py][Px] = c;
        }
    }

    fout = fopen("out.ppm", "w");
    fprintf(fout, "P3\n");
    fprintf(fout, "%d %d\n", M, N);
    fprintf(fout, "255\n");
    for (int Py = 0; Py < N; Py++)
    {
        for (int Px = 0; Px < M; Px++)
        {
            fprintf(fout, "%d %d %d\n",
                    rgb[Py][Px].r, rgb[Py][Px].g, rgb[Py][Px].b);
        }
    }
    fclose(fout);
    return 0;
}