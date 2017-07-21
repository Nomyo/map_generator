#include <simplexnoise.hh>
#include <random>

#define FASTFLOOR(x) (((x)>0) ? ((int)x) : (((int)x) - 1))


SimplexNoise::SimplexNoise()
{
    std::random_device rd; // obtain a random number from hardware
    auto i = rd();
    std::cout << "Permutation seed : " << i << std::endl;
    std::mt19937 eng(i); // nice  the generator 2254022597
    std::uniform_int_distribution<> distr(0, 255); // define the range

    auto rand = std::bind(distr, eng);

    std::generate_n(permutation.begin(), 256, rand);

    for (int i = 256; i < 512; ++i)
	   permutation[i] = permutation[i - 256];
}

SimplexNoise::SimplexNoise(int i)
{
    std::mt19937 eng(i); // nice  the generator 2254022597
    std::uniform_int_distribution<> distr(0, 255); // define the range

    auto rand = std::bind(distr, eng);

    std::generate_n(permutation.begin(), 256, rand);

    for (int i = 256; i < 512; ++i)
	   permutation[i] = permutation[i - 256];
}

double SimplexNoise::grad(int hash, double x, double y ) {
    int h = hash & 7;      // Convert low 3 bits of hash code
    double u = h < 4 ? x : y;  // into 8 simple gradient directions,
    double v = h < 4 ? y : x;  // and compute the dot product with (x,y).

    return ((h & 1) ? -u : u) + ((h & 2)? -2.0f * v : 2.0f * v);
}

double SimplexNoise::sum_octave(int nb_iterations, double x, double y,
				double persistence, double scale,
				double low, double high)
{
    double max_amp = 0;
    double amp = 1;
    double freq = scale;
    double noise = 0;

    for (auto i = 0; i < nb_iterations; ++i)
    {
	noise += make_noise(x * freq, y * freq) * amp;
	max_amp += amp;
	amp *= persistence;
	freq *= 2;
    }

    noise /= max_amp;
    noise = noise * (high - low) / 2 + (high + low) / 2;

    return noise;
}

// 2D simplex noise
double SimplexNoise::make_noise(double x, double y) {

#define F2 0.366025403f // F2 = 0.5*(sqrt(3.0)-1.0)
#define G2 0.211324865f // G2 = (3.0-Math.sqrt(3.0))/6.0

    double n0, n1, n2; // Noise contributions from the three corners

    // Skew the input space to determine which simplex cell we're in
    double s = (x + y) * F2; // Hairy factor for 2D
    double xs = x + s;
    double ys = y + s;
    int i = FASTFLOOR(xs);
    int j = FASTFLOOR(ys);

    double t = (double)(i + j) * G2;
    double X0 = i - t; // Unskew the cell origin back to (x,y) space
    double Y0 = j - t;
    double x0 = x - X0; // The x,y distances from the cell origin
    double y0 = y - Y0;

    // For the 2D case, the simplex shape is an equilateral triangle.
    // Determine which simplex we are in.
    // Offsets for second (middle) corner of simplex in (i,j) coords

    int i1 = 0;
    int j1 = 0;

    if(x0 > y0)
    {
	// lower triangle, XY order: (0,0)->(1,0)->(1,1)
	i1 = 1;
	j1 = 0;
    }
    else
    {
	// upper triangle, YX order: (0,0)->(0,1)->(1,1)
	i1 = 0;
	j1 = 1;
    }

    // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
    // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
    // c = (3-sqrt(3))/6

    double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
    double y1 = y0 - j1 + G2;
    double x2 = x0 - 1.0f + 2.0f * G2; // Offsets for last corner in (x,y) unskewed coords
    double y2 = y0 - 1.0f + 2.0f * G2;

    // Wrap the integer indices at 256, to avoid indexing permutation[] out of bounds
    int ii = i & 0xff;
    int jj = j & 0xff;

    // Calculate the contribution from the three corners
    double t0 = 0.5f - x0 * x0 - y0 * y0;
    if(t0 < 0.0f) n0 = 0.0f;
    else
    {
	t0 *= t0;
	n0 = t0 * t0 * grad(permutation[ii + permutation[jj]], x0, y0);
    }

    double t1 = 0.5f - x1 * x1 - y1 * y1;
    if(t1 < 0.0f)
	n1 = 0.0f;
    else
    {
	t1 *= t1;
	n1 = t1 * t1 * grad(permutation[ii + i1 + permutation[jj + j1]], x1, y1);
    }

    double t2 = 0.5f - x2 * x2 - y2 * y2;
    if(t2 < 0.0f) n2 = 0.0f;
    else
    {
	t2 *= t2;
	n2 = t2 * t2 * grad(permutation[ii + 1 + permutation[jj + 1]], x2, y2);
    }

    // Add contributions from each corner to get the final noise value.
    // The result is scaled to return values in the interval [-1,1].
    return 40.0f * (n0 + n1 + n2); // TODO: The scale factor is preliminary!
}
