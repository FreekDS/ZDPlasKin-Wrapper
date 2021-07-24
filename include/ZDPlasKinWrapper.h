#ifndef ZDPLASKIN_SRC_ZDPLASKINWRAPPER_H_
#define ZDPLASKIN_SRC_ZDPLASKINWRAPPER_H_

#include "LibraryLoader.h"
#include "ZDPlasKinFunctionDefs.h"

class ZDPlasKinWrapper {

	ILibraryLoader *_lib;

	static std::string lib_f(const std::string& name);

 public:

	explicit ZDPlasKinWrapper(ILibraryLoader *lib);

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

	struct TotalDensityResult { double ALL_SPECIES, ALL_NEUTRAl, ALL_ION_POSITIVE, ALL_ION_NEGATIVE, ALL_CHARGE; };
	TotalDensityResult get_density_total();

	struct RatesResult {
		double *SOURCE_TERMS, *REACTION_RATES, **SOURCE_TERMS_MATRIX, *MEAN_DENSITY, *MEAN_SOURCE_TERMS,
			*MEAN_REACTION_RATES, **MEAN_SOURCE_TERMS_MATRIX;
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
		double GAS_TEMPERATURE,
			REDUCED_FREQUENCY,
			REDUCED_FIELD,
			ELEC_TEMPERATURE,
			ELEC_DRIFT_VELOCITY,
			ELEC_DIFF_COEFF,
			ELEC_MOBILITY_N,
			ELEC_MU_EPS_N,
			ELEC_DIFF_EPS_N,
			ELEC_FREQUENCY_N,
			ELEC_POWER_N,
			ELEC_POWER_ELASTIC_N,
			ELEC_POWER_INELASTIC_N,
			**ELEC_EEDF;
	};
	GetConditionsResult get_conditions();

	void reset();

	void stop(const std::string &string);

	void write_file(const std::string &FILE_SPECIES,
					const std::string &FILE_REACTIONS,
					const std::string &FILE_SOURCE_MATRIX,
					int FILE_UNIT);

	void write_qtplaskin(double time, bool LFORCE_WRITE = false);

	double **reac_source_matrix(double reac_rate_local[]);

	double *fex(int neq, double t, double y[]);

	double **jex(int neq, double t, double y[], int ml, int mu, int nrpd);

	void reac_rates(double time);
};

#endif //ZDPLASKIN_SRC_ZDPLASKINWRAPPER_H_
