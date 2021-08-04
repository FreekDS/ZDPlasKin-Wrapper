#ifndef ZDPLASKIN_SRC_ZDPLASKINWRAPPER_H_
#define ZDPLASKIN_SRC_ZDPLASKINWRAPPER_H_

#include "LibraryLoader.h"
#include "ZDPlasKinFunctionDefs.h"
#include "ZDPlasKinParams.h"
#include "helpers/vector_helpers.h"

class ZDPlasKinWrapper {

	ILibraryLoader *_lib;
	ZDPlasKinParams *_params;

	static std::string lib_f(const std::string &name, bool is_variable = false);

 public:

	ZDPlasKinWrapper(ILibraryLoader *lib, ZDPlasKinParams *params);

	void init(const std::string &bolsigDB = "", bool updateDB = false);

	// takes and returns dtime
	double &timestep(double time, double &dtime);

	// takes and returns dtime
	double &timestep_explicit(double time,
							  double &dtime,
							  double rtol_loc,
							  double atol_loc,
							  double switch_implicit);

	double &timestep_explicit(double time, double &dtime, double rtol_loc, double atol_loc);

	void bolsig_rates(bool lbolsig_force);

	void bolsig_rates();

	int get_species_index(const std::string &string);

	void set_density(const std::string &string, double DENS = 0.0, bool LDENS_CONST = false);

	void set_density(const std::string &string);

	void set_density(const std::string &string, bool LDENS_CONS);

	struct DensityResult { double DENS; bool LDENS_CONST; };
	DensityResult get_density(const std::string &string);

	double get_density_dens(const std::string &string);

	bool get_density_ldens_cons(const std::string &string);

	struct TotalDensityResult {
		double ALL_SPECIES;
		double ALL_NEUTRAl;
		double ALL_ION_POSITIVE;
		double ALL_ION_NEGATIVE;
		double ALL_CHARGE;
	};
	TotalDensityResult get_density_total();

	double get_density_total_species();
	double get_density_total_neutral();
	double get_density_total_ion_positive();
	double get_density_total_ion_negative();
	double get_density_total_charge();

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

	std::vector<double> get_rates_source_terms();
	std::vector<double> get_rates_reaction_rates();
	utils::matrix<double> get_rates_source_terms_matrix();
	std::vector<double> get_rates_mean_density();
	std::vector<double> get_rates_mean_source_terms();
	std::vector<double> get_rates_mean_reaction_rates();
	utils::matrix<double> get_rates_mean_source_terms_matrix();

	void set_config(double ATOL,
					double RTOL,
					bool SILENCE_MODE,
					bool STAT_ACCUM,
					bool QTPLASKIN_SAVE,
					double BOLSIG_EE_FRAC,
					bool BOLSIG_IGNORE_GAS_TEMPERATURE);

	void set_config_atol(double atol);
	void set_config_rtol(double rtol);
	void set_config_silence_mode(bool silence_mode);
	void set_config_stat_accum(bool stat_accum);
	void set_config_qtplaskin_save(bool save);
	void set_config_bolsig_ee_frac(double bolsig_ee_frac);
	void set_config_bolsig_ignore_gas_temperature(bool bolsig_ignore_gas_temperature);

	void set_conditions(double GAS_TEMPERATURE,
						double REDUCED_FREQUENCY,
						double REDUCED_FIELD,
						double ELEC_TEMPERATURE,
						bool GAS_HEATING,
						double SPEC_HEAT_RATIO,
						double HEAT_SOURCE,
						bool SOFT_RESET);

	void set_condition_gas_temperature(double gas_temperature);
	void set_condition_reduced_frequency(double reduced_frequency);
	void set_condition_reduced_field(double reduced_field);
	void set_condition_reduced_elec_temperature(double elec_temperature);
	void set_condition_reduced_gas_heating(bool gas_heating);
	void set_condition_reduced_spec_heat_ratio(double spec_heat_ratio);
	void set_condition_reduced_spec_heat_source(double heat_source);
	void set_condition_reduced_soft_reset(bool soft_reset);

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
	ZDPlasKinWrapper::GetConditionsResult get_conditions(size_t ELEC_EEDF_SIZE);

	double get_condition_gas_temperature();
	double get_condition_reduced_frequency();
	double get_condition_reduced_field();
	double get_condition_elec_temperature();
	double get_condition_elec_drift_velocity();
	double get_condition_elec_diff_coeff();
	double get_condition_elec_mobility_n();
	double get_condition_elec_mu_eps_n();
	double get_condition_elec_diff_eps_n();
	double get_condition_elec_frequency_n();
	double get_condition_elec_power_n();
	double get_condition_elec_power_elastic_n();
	double get_condition_elec_power_inelastic_n();
	utils::matrix<double> get_condition_elec_eedf(size_t ELEC_EEDF_SIZE);

	void reset();

	void stop(const std::string &string);

	// TODO match optional cases for this file
	void write_file(const std::string &FILE_SPECIES,
					const std::string &FILE_REACTIONS,
					const std::string &FILE_SOURCE_MATRIX,
					int FILE_UNIT);

	void write_qtplaskin(double time, bool LFORCE_WRITE);

	void write_qtplaskin(double time);

	utils::matrix<double> reac_source_matrix(double reac_rate_local[]);

	std::vector<double> fex(int neq, double t, double y[]);

	utils::matrix<double> jex(int neq, double t, double y[], int ml, int mu, int nrpd);

	void reac_rates(double time);

	[[nodiscard]] std::vector<double> getDensity() const;

	[[nodiscard]] std::vector<int> getSpeciesCharge() const;

	[[nodiscard]] std::vector<std::string> getSpeciesName() const;

	[[nodiscard]] std::vector<std::string> getReactionSign() const;

	[[nodiscard]] std::vector<bool> getLReactionBlock() const;

};

#endif //ZDPLASKIN_SRC_ZDPLASKINWRAPPER_H_
