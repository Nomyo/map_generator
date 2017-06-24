class SimplexNoise
{
public:
  SimplexNoise() = default;
  ~SimplexNoise() = default;

  double make_noise(double x, double y);
  double sum_octave(int nb_iterations, double x, double y,
		    double persistence, double scale,
		    double low, double high);

private:
  double grad( int hash, double x, double y);
  static unsigned char permutation[];
};
