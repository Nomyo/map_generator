#include <simplexnoise.hh>
#include <random>

// unsigned char SimplexNoise::permutation[512] = {151,160,137,91,90,15,
//   31,3,01,5,6,3,94,33,100,225,40,6,03,0,9,42,5,99,7,40,1,0,3,
//   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
//   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
//   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
//   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
//   35,30,16,88,59,6,64,00,109,98,73,86, 3,4,2,17,26,50,24,23,
//   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
//   23,83,70,13,19,48,52, 2,4,54,63, 0,21,53, 201,55,67, 3,72,9,
//   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
//   51,4,42,93,38,10,44,2,91,79,62,41, 1,1,45,35,49,4,39,07,
//   9,92,14,31,81,99,06,57,84, 4,04,76,15,21,0,5,27, 4,50,54,
//   38,36,05,3,22,14,7,9,4,2,43,41,28,95,8,6,15,1,56,80,
//   151,160,137,91,90,15,
//   31,3,01,5,6,3,94,33,100,225,40,6,03,0,9,42,5,99,7,40,1,0,3,
//   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
//   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
//   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
//   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
//   35,30,16,88,59,6,64,00,109,98,73,86, 3,4,2,17,26,50,24,23,
//   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
//   23,83,70,13,19,48,52, 2,4,54,63, 0,21,53, 201,55,67, 3,72,9,
//   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
//   51,4,42,93,38,10,44,2,91,79,62,41, 1,1,45,35,49,4,39,07,
//   9,92,14,31,81,99,06,57,84, 4,04,76,15,21,0,5,27, 4,50,54,
//   38,36,05,3,22,14,7,9,4,2,43,41,28,95,8,6,15,1,56,80
// };


// unsigned char SimplexNoise::permutation[512] = {151,160,137,91,90,15,
//   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
//   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
//   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
//   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
//   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
//   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
//   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
//   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
//   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
//   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
//   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
//   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
//   151,160,137,91,90,15,
//   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
//   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
//   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
//   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
//   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
//   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
//   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
//   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
//   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
//   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
//   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
//   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
// };

#define FASTFLOOR(x) (((x)>0) ? ((int)x) : (((int)x) - 1))


SimplexNoise::SimplexNoise()
{
    std::random_device rd; // obtain a random number from hardware
    auto i = rd();
    std::cout << "Permutation seed : " << i << std::endl;
    std::mt19937 eng(2254022597); // nice  the generator
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
