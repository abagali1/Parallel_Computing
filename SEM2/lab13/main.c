#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define M 1920
#define N 1080
#define SPHERES 5


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

const Color BACKGROUND = (Color){.r = 51, .g = 51, .b = 51};
// the eye
const Vector eye = {
    0.50,
    0.50,
    -1.00};

// the light
const Vector g = {
    1.50,
    1.25,
    -0.50
    };

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
    a[0].c = (Vector){ // Floor
        .x = 0.50,
        .y = -20000.00,
        .z = 0.50,
    };
    a[0].r = 20000.25;
    a[0].h = (Color){
        .r = 100,
        .g = 99,
        .b = 97
    };

    a[1].c = (Vector){ // Blue Sphere
        .x = 0.50,
        .y = 0.50,
        .z = 0.50
    };
    a[1].r = 0.25;
    a[1].h = (Color){
        .r = 0,
        .g = 0,
        .b = 255
    };

    a[2].c = (Vector){ // Green Sphere
        .x = 1.00,
        .y = 0.50,
        .z = 1.00
    };
    a[2].r = 0.25;
    a[2].h = (Color){
        .r = 0,
        .g = 255,
        .b = 0
    };

    a[3].c = (Vector){ // Red Sphere
        .x = 0.00,
        .y = 0.75,
        .z = 1.25
    };
    a[3].r = 0.50;
    a[3].h = (Color){
        .r = 255,
        .g = 0,
        .b = 0
    };

    a[4].c = g; // make light a sphere cause cool
    a[4].r = 0.5;
    a[4].h = (Color){
        .r = 255,
        .g = 255,
        .b = 255
    };
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
        .y = a.y - b.y,
        .z = a.z - b.z};
}

Vector scalar_multiply(Vector a, double scalar)
{
    return (Vector){
        .x = scalar * a.x,
        .y = scalar * a.y,
        .z = scalar * a.z};
}

bool cast(Sphere sphere, Vector ray, Vector origin, double* t)
{
    Vector v = subtract_vector(origin, sphere.c);
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
            Color c = BACKGROUND;
            double t;
            int sphere_index;
            for (int s = 0; s < SPHERES; s++)
            {
                Sphere sphere = spheres[s];

                if (cast(sphere, ray, eye, &t))
                {
                    if (t < t_min)
                    {
                        t_min = t;
                        c = sphere.h;
                        sphere_index = s;
                    }
                }
            }
            if(t_min == INFINITY){
                rgb[Py][Px] = c;
                continue;
            }
            if(sphere_index == 0){
                     
            }
            Vector intersection = add_vector(eye, scalar_multiply(ray, t_min-0.001));
            Vector sphere_to_light = create_vector(intersection, g);
            for(int s = 0;s < SPHERES-1; s++){
                Sphere sphere = spheres[s];
                if(cast(sphere, sphere_to_light, intersection, &t)){
                    c.r /= 2;
                    c.g /= 2;
                    c.b /= 2;
                    break;
                }else{
                    continue;
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
