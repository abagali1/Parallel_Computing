#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define M 1920
#define N 1080
#define SPHERES 5
#define PI 3.14159265358979323846

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

const Color BACKGROUND = (Color){.r = 100, .g = 99, .b = 97};
const Color WHITE = (Color){.r = 255, .g = 255, .b = 255};
const Color BLACK = (Color){.r = 0, .g = 0, .b = 0};
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
    a[4].h = BACKGROUND;
}

int mercator_w;
int mercator_h;
Color** read_mercator()
{
    FILE* ppmf;

    Color** mercator_ppm;
    char buff[255];
    ppmf = fopen("mercator82.ppm", "r");
    fscanf(ppmf, "%s", buff); //read P3 -> 0
    fscanf(ppmf, "%s", buff); //read width
    mercator_w = atoi(buff);
    fscanf(ppmf, "%s", buff); //read height
    mercator_h = atoi(buff);
    fscanf(ppmf, "%s", buff); //read 255 -> 0
    //init ppm
    mercator_ppm = (Color **)malloc(mercator_h * sizeof(Color *));
    for(int i = 0; i < mercator_h; i++)
        mercator_ppm[i] = (Color *)malloc(mercator_w * sizeof(Color));
    //fill ppm
    for(int i = 0; i < mercator_h * mercator_w; i++)
    {
        Color h;
        fscanf(ppmf, "%s", buff);
        h.r = atoi(buff);
        fscanf(ppmf, "%s", buff);
        h.g = atoi(buff);
        fscanf(ppmf, "%s", buff);
        h.b = atoi(buff);
        mercator_ppm[i / mercator_w][i % mercator_w] = h;
    }
    return mercator_ppm;
   
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


Color get_color(Vector ray, Vector origin, Sphere* spheres, Color** mercator){
    double t_min = INFINITY;
    Color c = BACKGROUND;
    double t;
    int sphere_index;
    for (int s = 0; s < SPHERES; s++) // calculate minimum T value
    {
        Sphere sphere = spheres[s];

        if (cast(sphere, ray, origin, &t))
        {
            if (t < t_min)
            {
                t_min = t;
                c = sphere.h;
                sphere_index = s;
            }
        }
    }
    if(t_min == INFINITY || sphere_index == SPHERES-1){ // base case, we have missed a sphere
        return c;
    }

    Vector X = add_vector(origin, scalar_multiply(ray, t_min-0.001)); // Position Vector
    Vector L = create_vector(X, g); // Intersection Vector
    if(sphere_index==0 && ((int)round(X.x/0.1) + (int)round(X.z/0.1)) % 2 == 0){ // Add checkerboards
        c = BLACK;
    }

    bool shadow = false; 

    for(int s = 0;s < SPHERES-1; s++){ // Calculate shadows
        Sphere sphere = spheres[s];
        if(cast(sphere, L, X, &t)){
            c.r /= 2;
            c.g /= 2;
            c.b /= 2;
            shadow = true;
            break;
        }else{
            continue;
        }   
    }

    Vector n = subtract_vector(X, spheres[sphere_index].c); // Vector Normal to Sphere
    normalize(&n); // n/R (Radius of Sphere)

    if(sphere_index == 8){ // Add mercator texture
        double lat_location = acos(n.y);
        double lon_location = atan2(n.z, n.x) + PI;

        int lat = lat_location * mercator_h/PI;
        int lon = lon_location * mercator_w/PI*2.0;
        
        c = mercator[lat][(lon+100)%mercator_w];
    }
    
    if (!shadow && sphere_index != 4) // Calculate gradient using lambert's law, except for light sphere
    {
        double intensity = .5*dotp(n,L)+.5; // cos(theta)

        if(intensity < 0){
            intensity = 0;
        }
        c.r *= intensity;
        c.g *= intensity;
        c.b *= intensity;
    }
    Vector W = subtract_vector(ray, scalar_multiply(n, 2*dotp(ray,n)));
    Color reflect = get_color(W, L, spheres, mercator);

    c.r *= 0.5;
    c.g *= 0.5;
    c.b *= 0.5;

    c.r += reflect.r*0.5;
    c.g += reflect.g*0.5;
    c.b += reflect.b*0.5;

    return c;

}


int main(void)
{
    FILE *fout;
    Color **rgb = malloc(sizeof(Color* ) * N);

    for (int i = 0; i < N; i++)
    {
        rgb[i] = malloc(sizeof(Color) * M);
    }
    Color** mercator_color = read_mercator();
    Sphere spheres[SPHERES];
    init(spheres);

    double aspect_ratio = (1.0 * M) / (1.0 * N);
    #pragma omp parallel for
    for (int Py = 0; Py < N; Py++)
    {
        for (int Px = 0; Px < M; Px++)
        {
            Vector ray = create_vector(eye, (Vector){
                .x = ((Px + 0.5) / (1.0 * M)) * aspect_ratio,
                .y = (((N - Py) + 0.5) / (1.0 * N)),
                .z = 0});
            
            rgb[Py][Px] = get_color(ray, eye, spheres, mercator_color);
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
