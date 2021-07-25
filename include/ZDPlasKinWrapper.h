#ifndef ZDPLASKIN_SRC_ZDPLASKINWRAPPER_H_
#define ZDPLASKIN_SRC_ZDPLASKINWRAPPER_H_

#include "LibraryLoader.h"
#include "ZDPlasKinFunctionDefs.h"
#include "ZDPlasKinParams.h"
#include "helpers/vector_helpers.h"

class ZDPlasKinWrapper {

	ILibraryLoader *_lib;
	ZDPlasKinParams *_params;

	static std::string lib_f(const std::string &name);

 public:

	ZDPlasKinWrapper(ILibraryLoader *lib, ZDPlasKinParams *params);

	void init();

	// takes and returns dtime
	double &timestep(double time, double &dtime);

	// takes and returns dtime
	double &timestep_explicit(double time,
							  double &dtime,
							  double rtol_loc,
							  double atol_loc,
							  double switch_implicit);

	void bolsig_rates(bool lbolsig_force = false);

	int get_species_index(const std::string &string);

	void set_density(const std::string &string, double DENS = 0.0, bool LDENS_CONST = false);

	struct DensityResult { double DENS; bool LDENS_CONST; };
	DensityResult get_density(const std::string &string);

	struct TotalDensityResult {
		double ALL_SPECIES;
		double ALL_NEUTRAl;
		double ALL_ION_POSITIVE;
		double ALL_ION_NEGATIVE;
		double ALL_CHARGE;
	};
	TotalDensityResult get_density_total();

	struct RatesResult {
		std::vector<double> SOURCE_TERMS;
		std::vector<double> REACTION_RATES;
		utils::matrix<double> SOURCE_TERMS_MATRIX;
		std::vector<double> MEAN_DENSITY;
		std::vector<double> MEAN_SOURCE_TERMS;
		std::vector<double> MEAN_REACTION_RATES;
		utils::matrix<double> MEAN_SOURCE_TERMS_MATRIX;
	};
	RatesResult get_rates();

	void set_config(double ATOL,
					double RTOL,
					bool SILENCE_MODE,
					bool STAT_ACCUM,
					bool QTPLASKIN_SAVE,
					double BOLSIG_EE_FRAC,
					bool BOLSIG_IGNORE_GAS_TEMPERATURE);

	void set_conditions(double GAS_TEMPERATURE,
						double REDUCED_FREQUENCY,
						double REDUCED_FIELD,
						double ELEC_TEMPERATURE,
						bool GAS_HEATING,
						double SPEC_HEAT_RATIO,
						double HEAT_SOURCE,
						bool SOFT_RESET);

	struct GetConditionsResult {
		double GAS_TEMPERATURE;
		double REDUCED_FREQUENCY;
		double REDUCED_FIELD;
		double ELEC_TEMPERATURE;
		double ELEC_DRIFT_VELOCITY;
		double ELEC_DIFF_COEFF;
		double ELEC_MOBILITY_N;
		double ELEC_MU_EPS_N;
		double ELEC_DIFF_EPS_N;
		double ELEC_FREQUENCY_N;
		double ELEC_POWER_N;
		double ELEC_POWER_ELASTIC_N;
		double ELEC_POWER_INELASTIC_N;
		utils::matrix<double> ELEC_EEDF;
	};
	GetConditionsResult get_conditions();

	void reset();

	void stop(const std::string &string);

	void write_file(const std::string &FILE_SPECIES,
					const std::string &FILE_REACTIONS,
					const std::string &FILE_SOURCE_MATRIX,
					int FILE_UNIT);

	void write_qtplaskin(double time, bool LFORCE_WRITE = false);

	utils::matrix<double> reac_source_matrix(double reac_rate_local[]);

	std::vector<double> fex(int neq, double t, double y[]);

	utils::matrix<double> jex(int neq, double t, double y[], int ml, int mu, int nrpd);

	void reac_rates(double time);
};

#endif //ZDPLASKIN_SRC_ZDPLASKINWRAPPER_H_
