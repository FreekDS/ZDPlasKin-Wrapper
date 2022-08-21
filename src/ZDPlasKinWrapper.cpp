#include <vector>
#include "helpers/vector_helpers.h"
#include "helpers/string_helpers.h"
#include "ZDPlasKinWrapper.h"
#include <filesystem>
#include <cstring>
#include "platform.h"

std::string ZDPlasKinWrapper::lib_f(const std::string &name, bool is_variable) {
	if (is_variable)
		return "__zdplaskin_MOD_" + name;
	else
		return "__zdplaskin_MOD_zdplaskin_" + name;
}

void ZDPlasKinWrapper::init(const std::string &bolsigDB, bool updateDB) {
	// TODO capture console output
	if (updateDB) {
		if (!std::filesystem::exists(bolsigDB)) {
			throw ZDPlaskinException("Could not find bolsig database file at '" + bolsigDB + "'");
		}
		std::string dbName = utils::split(bolsigDB, "/").back();
		std::string destination = std::filesystem::canonical(".").string() + SEP + dbName;

		std::string sourcePath = std::filesystem::canonical(bolsigDB).string();
		if (destination != sourcePath) {
			if (std::filesystem::exists(destination)) {
				std::filesystem::remove(destination);
			}
			std::filesystem::copy(bolsigDB, destination);
		}
	}

	auto initFunc = _lib->getFunction<ZDPlasKin_init>(lib_f("init"));
	initFunc();
}

double &ZDPlasKinWrapper::timestep(double time, double &dtime) {
	auto timestepFunc = _lib->getFunction<ZDPlasKin_timestep>(lib_f("timestep"));
	timestepFunc(time, dtime);
	return dtime;
}

double &ZDPlasKinWrapper::timestep_explicit(double time,
											double &dtime,
											double rtol_loc,
											double atol_loc,
											double switch_implicit) {
	auto timestepExplicitFunc =
		_lib->getFunction<ZDPlasKin_timestep_explicit>(lib_f("timestep_explicit"));
	timestepExplicitFunc(time, dtime, rtol_loc, atol_loc, &switch_implicit);
	return dtime;
}

void ZDPlasKinWrapper::bolsig_rates(bool lbolsig_force) {
	auto bolsigRatesFunc = _lib->getFunction<ZDPlasKin_bolsig_rates>(lib_f("bolsig_rates"));
	bolsigRatesFunc(&lbolsig_force);
}

int ZDPlasKinWrapper::get_species_index(const std::string &string) {
	int result = 0;
	auto speciesIndexFunc = _lib->getFunction<ZDPlasKin_get_species_index>(lib_f("get_species_index"));
	speciesIndexFunc(string.c_str(), result, string.size());
	return result;
}

void ZDPlasKinWrapper::set_density(const std::string &string, double DENS, bool LDENS_CONST) {
	auto setDensityFunc = _lib->getFunction<ZDPlasKin_set_density>(lib_f("set_density"));
	setDensityFunc(string.c_str(), &DENS, &LDENS_CONST, string.size());
}

ZDPlasKinWrapper::DensityResult ZDPlasKinWrapper::get_density(const std::string &string) {
	auto getDensityFunc = _lib->getFunction<ZDPlasKin_get_density>(lib_f("get_density"));
	double DENS = 0;
	bool LDENS_CONST = false;
	getDensityFunc(string.c_str(), &DENS, &LDENS_CONST, string.size());
	return {DENS, LDENS_CONST};
}

ZDPlasKinWrapper::TotalDensityResult ZDPlasKinWrapper::get_density_total() {
	auto getDensityTotalFunc = _lib->getFunction<ZDPlasKin_get_density_total>(lib_f("get_density_total"));
	double ALL_SPECIES = 0;
	double ALL_NEUTRAL = 0;
	double ALL_ION_NEGATIVE = 0;
	double ALL_ION_POSITIVE = 0;
	double ALL_CHARGE = 0;
	getDensityTotalFunc(&ALL_SPECIES, &ALL_NEUTRAL, &ALL_ION_POSITIVE, &ALL_ION_NEGATIVE, &ALL_CHARGE);
	return {
		ALL_SPECIES,
		ALL_NEUTRAL,
		ALL_ION_NEGATIVE,
		ALL_ION_POSITIVE,
		ALL_CHARGE};
}

ZDPlasKinWrapper::RatesResult ZDPlasKinWrapper::get_rates() {
	auto getRatesFunc = _lib->getFunction<ZDPlasKin_get_rates>(lib_f("get_rates"));

	std::vector<double> SOURCE_TERMS(_params->getSpeciesMax(), 0);
	std::vector<double> REACTION_RATES(_params->getReactionsMax(), 0);
	std::vector<double> MEAN_DENSITY(_params->getSpeciesMax(), 0);
	std::vector<double> MEAN_SOURCE_TERMS(_params->getSpeciesMax(), 0);
	std::vector<double> MEAN_REACTION_RATES(_params->getReactionsMax(), 0);

	auto *SOURCE_TERMS_MATRIX = new double[_params->getSpeciesMax()*_params->getReactionsMax()];
	auto *MEAN_SOURCE_TERMS_MATRIX = new double[_params->getSpeciesMax()*_params->getReactionsMax()];

	getRatesFunc(
		SOURCE_TERMS.data(),
		REACTION_RATES.data(),
		SOURCE_TERMS_MATRIX,
		MEAN_DENSITY.data(),
		MEAN_SOURCE_TERMS.data(),
		MEAN_REACTION_RATES.data(),
		MEAN_SOURCE_TERMS_MATRIX
	);

	utils::matrix<double> mean_src_terms_matrix = utils::arrayToMatrix(
		MEAN_SOURCE_TERMS_MATRIX,
		_params->getSpeciesMax(),
		_params->getReactionsMax()
	);
	utils::matrix<double> src_terms_matrix = utils::arrayToMatrix(
		SOURCE_TERMS_MATRIX,
		_params->getSpeciesMax(),
		_params->getReactionsMax()
	);

	delete[] SOURCE_TERMS_MATRIX;    // Free memory!
	delete[] MEAN_SOURCE_TERMS_MATRIX;    // Free memory!

	return {
		SOURCE_TERMS,
		REACTION_RATES,
		src_terms_matrix,
		MEAN_DENSITY,
		MEAN_SOURCE_TERMS,
		MEAN_REACTION_RATES,
		mean_src_terms_matrix
	};
}

void ZDPlasKinWrapper::set_config(double ATOL,
								  double RTOL,
								  bool SILENCE_MODE,
								  bool STAT_ACCUM,
								  bool QTPLASKIN_SAVE,
								  double BOLSIG_EE_FRAC,
								  bool BOLSIG_IGNORE_GAS_TEMPERATURE) {
	auto setConfigFunc = _lib->getFunction<ZDPlasKin_set_config>(lib_f("set_config"));
	setConfigFunc(&ATOL,
				  &RTOL,
				  &SILENCE_MODE,
				  &STAT_ACCUM,
				  &QTPLASKIN_SAVE,
				  &BOLSIG_EE_FRAC,
				  &BOLSIG_IGNORE_GAS_TEMPERATURE);
}

void ZDPlasKinWrapper::set_conditions(double GAS_TEMPERATURE,
									  double REDUCED_FREQUENCY,
									  double REDUCED_FIELD,
									  double ELEC_TEMPERATURE,
									  bool GAS_HEATING,
									  double SPEC_HEAT_RATIO,
									  double HEAT_SOURCE,
									  bool SOFT_RESET) {
	auto setConditionsFunc = _lib->getFunction<ZDPlasKin_set_conditions>(lib_f("set_conditions"));
	setConditionsFunc(&GAS_TEMPERATURE,
					  &REDUCED_FREQUENCY,
					  &REDUCED_FIELD,
					  &ELEC_TEMPERATURE,
					  &GAS_HEATING,
					  &SPEC_HEAT_RATIO,
					  &HEAT_SOURCE,
					  &SOFT_RESET);
}

ZDPlasKinWrapper::GetConditionsResult ZDPlasKinWrapper::get_conditions(size_t ELEC_EEDF_SIZE) {
	auto getConditionsFunc = _lib->getFunction<ZDPlasKin_get_conditions>(lib_f("get_conditions"));
	double GAS_TEMPERATURE = 0;
	double REDUCED_FREQUENCY = 0;
	double REDUCED_FIELD = 0;
	double ELEC_TEMPERATURE = 0;
	double ELEC_DRIFT_VELOCITY = 0;
	double ELEC_DIFF_COEFF = 0;
	double ELEC_MOBILITY_N = 0;
	double ELEC_MU_EPS_N = 0;
	double ELEC_DIFF_EPS_N = 0;
	double ELEC_FREQUENCY_N = 0;
	double ELEC_POWER_N = 0;
	double ELEC_POWER_ELASTIC_N = 0;
	double ELEC_POWER_INELASTIC_N = 0;

	// ELEC_EEDF_MATRIX always has 2 rows (see zdplaskin_m impl)
	auto *ELEC_EEDF = new double[2*ELEC_EEDF_SIZE];

	getConditionsFunc(&GAS_TEMPERATURE,
					  &REDUCED_FREQUENCY,
					  &REDUCED_FIELD,
					  &ELEC_TEMPERATURE,
					  &ELEC_DRIFT_VELOCITY,
					  &ELEC_DIFF_COEFF,
					  &ELEC_MOBILITY_N,
					  &ELEC_MU_EPS_N,
					  &ELEC_DIFF_EPS_N,
					  &ELEC_FREQUENCY_N,
					  &ELEC_POWER_N,
					  &ELEC_POWER_ELASTIC_N,
					  &ELEC_POWER_INELASTIC_N,
					  ELEC_EEDF);

	utils::matrix<double> elec_eedf = utils::arrayToMatrix(ELEC_EEDF, 2, ELEC_EEDF_SIZE);
	delete[] ELEC_EEDF;

	return {
		GAS_TEMPERATURE,
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
		elec_eedf
	};
}

void ZDPlasKinWrapper::reset() {
	auto resetFunc = _lib->getFunction<ZDPlasKin_reset>(lib_f("reset"));
	resetFunc();
}

void ZDPlasKinWrapper::stop(const std::string &string) {
	auto stopFunc = _lib->getFunction<ZDPlasKin_stop>(lib_f("stop"));
	stopFunc(string.c_str(), string.size());
}

void ZDPlasKinWrapper::write_file(const std::string &FILE_SPECIES,
								  const std::string &FILE_REACTIONS,
								  const std::string &FILE_SOURCE_MATRIX,
								  int FILE_UNIT) {
	auto writeFileFunc = _lib->getFunction<ZDPlasKin_write_file>(lib_f("write_file"));
	size_t fileSpeciesSize = FILE_SPECIES.size();
	size_t fileReactionsSize = FILE_REACTIONS.size();
	size_t fileSourceMatrixSize = FILE_SOURCE_MATRIX.size();
	writeFileFunc(FILE_SPECIES.c_str(),
				  FILE_REACTIONS.c_str(),
				  FILE_SOURCE_MATRIX.c_str(),
				  &FILE_UNIT,
				  &fileSpeciesSize,
				  &fileReactionsSize,
				  &fileSourceMatrixSize);
}

void ZDPlasKinWrapper::write_qtplaskin(double time, bool LFORCE_WRITE) {
	auto writeQTPlaskinFunc = _lib->getFunction<ZDPlasKin_write_qtplaskin>(lib_f("write_qtplaskin"));
	writeQTPlaskinFunc(time, &LFORCE_WRITE);
}

utils::matrix<double> ZDPlasKinWrapper::reac_source_matrix(double *reac_rate_local) {
	auto reacSourceMatrixFunc = _lib->getFunction<ZDPlasKin_reac_source_matrix>(lib_f("reac_source_matrix"));
	auto *REAC_SOURCE_LOCAL = new double[_params->getSpeciesMax()*_params->getReactionsMax()];
	reacSourceMatrixFunc(reac_rate_local, REAC_SOURCE_LOCAL);

	utils::matrix<double> reac_source_local = utils::arrayToMatrix(
		REAC_SOURCE_LOCAL,
		_params->getSpeciesMax(),
		_params->getReactionsMax()
	);
	delete[] REAC_SOURCE_LOCAL;    // Free memory!

	return reac_source_local;
}

std::vector<double> ZDPlasKinWrapper::fex(int neq, double t, double *y) {
	auto fexFunc = _lib->getFunction<ZDPlasKin_fex>(lib_f("fex"));
	std::vector<double> ydot(neq, 0);
	fexFunc(neq, t, y, ydot.data());
	return ydot;
}

utils::matrix<double> ZDPlasKinWrapper::jex(int neq, double t, double *y, int ml, int mu, int nrpd) {
	auto jexFunc = _lib->getFunction<ZDPlasKin_jex>(lib_f("jex"));
	auto *PD = new double[nrpd*neq];
	jexFunc(neq, t, y, ml, mu, PD, nrpd);

	utils::matrix<double> pd = utils::arrayToMatrix(PD, nrpd, neq);
	delete[] PD; // Free memory!

	return pd;
}

void ZDPlasKinWrapper::reac_rates(double time) {
	auto reacRatesFunc = _lib->getFunction<ZDPlasKin_reac_rates>(lib_f("reac_rates"));
	reacRatesFunc(time);
}

ZDPlasKinWrapper::ZDPlasKinWrapper(ILibraryLoader *lib, ZDPlasKinParams *params) : _lib(lib), _params(params) {}

std::vector<double> ZDPlasKinWrapper::getDensity() const {
	auto dens = _lib->getFunction<double *>(lib_f("density", true));
	return utils::ptrArrayToVec(dens, _params->getSpeciesMax());
}

std::vector<int> ZDPlasKinWrapper::getSpeciesCharge() const {
	auto speciesCharge = _lib->getFunction<int *>(lib_f("species_max", true));
	return std::move(utils::ptrArrayToVec(speciesCharge, _params->getSpeciesMax()));
}

std::vector<std::string> ZDPlasKinWrapper::getSpeciesName() const {
	auto speciesNames = _lib->getFunction<char *>(lib_f("species_name", true));
	std::vector<std::string> species = utils::splitEqualSize(std::string(speciesNames), _params->getSpeciesLength());
	return std::move(species);
}

std::vector<std::string> ZDPlasKinWrapper::getReactionSign() const {
	auto reactionSign = _lib->getFunction<char *>(lib_f("reactions_sign", true));
	std::vector<std::string> reactions = utils::splitEqualSize(
		std::string(reactionSign), _params->getReactionsLength()
	);
	return std::move(reactions);
}

std::vector<bool> ZDPlasKinWrapper::getLReactionBlock() const {
	auto LReactionBlock = _lib->getFunction<bool *>(lib_f("lreaction_block", true));
	return std::move(utils::ptrArrayToVec(LReactionBlock, _params->getReactionsMax()));
}

double &ZDPlasKinWrapper::timestep_explicit(double time, double &dtime, double rtol_loc, double atol_loc) {
	auto timestepFunc = _lib->getFunction<ZDPlasKin_timestep_explicit>(lib_f("timestep_implict"));
	timestepFunc(time, dtime, rtol_loc, atol_loc, nullptr);
	return dtime;
}

void ZDPlasKinWrapper::bolsig_rates() {
	auto bolsigRatesFunc = _lib->getFunction<ZDPlasKin_bolsig_rates>(lib_f("bolsig_rates"));
	bolsigRatesFunc(nullptr);
}

void ZDPlasKinWrapper::set_density(const std::string &string) {
	// TODO check if this is useful to have or not
	auto setDensityFunc = _lib->getFunction<ZDPlasKin_set_density>(lib_f("set_density"));
	setDensityFunc(string.c_str(), nullptr, nullptr, string.size());
}

void ZDPlasKinWrapper::set_density(const std::string &string, bool LDENS_CONS) {
	auto setDensityFunc = _lib->getFunction<ZDPlasKin_set_density>(lib_f("set_density"));
	setDensityFunc(string.c_str(), nullptr, &LDENS_CONS, string.size());
}

double ZDPlasKinWrapper::get_density_dens(const std::string &string) {
	auto getDensityFunc = _lib->getFunction<ZDPlasKin_get_density>(lib_f("get_density"));
	double dens = 0;
	getDensityFunc(string.c_str(), &dens, nullptr, string.size());
	return dens;
}

bool ZDPlasKinWrapper::get_density_ldens_cons(const std::string &string) {
	auto getDensityFunc = _lib->getFunction<ZDPlasKin_get_density>(lib_f("get_density"));
	bool ldens_cons = false;
	getDensityFunc(string.c_str(), nullptr, &ldens_cons, string.size());
	return ldens_cons;
}

double ZDPlasKinWrapper::get_density_total_species() {
	auto getDensityTotalFunc = _lib->getFunction<ZDPlasKin_get_density_total>(lib_f("get_density_total"));
	double species = 0;
	getDensityTotalFunc(&species, nullptr, nullptr, nullptr, nullptr);
	return species;
}

double ZDPlasKinWrapper::get_density_total_neutral() {
	auto getDensityTotalFunc = _lib->getFunction<ZDPlasKin_get_density_total>(lib_f("get_density_total"));
	double neutral = 0;
	getDensityTotalFunc(nullptr, &neutral, nullptr, nullptr, nullptr);
	return neutral;
}

double ZDPlasKinWrapper::get_density_total_ion_positive() {
	auto getDensityTotalFunc = _lib->getFunction<ZDPlasKin_get_density_total>(lib_f("get_density_total"));
	double ion_positive = 0;
	getDensityTotalFunc(nullptr, nullptr, &ion_positive, nullptr, nullptr);
	return ion_positive;
}

double ZDPlasKinWrapper::get_density_total_ion_negative() {
	auto getDensityTotalFunc = _lib->getFunction<ZDPlasKin_get_density_total>(lib_f("get_density_total"));
	double ion_negative = 0;
	getDensityTotalFunc(nullptr, nullptr, nullptr, &ion_negative, nullptr);
	return ion_negative;
}

double ZDPlasKinWrapper::get_density_total_charge() {
	auto getDensityTotalFunc = _lib->getFunction<ZDPlasKin_get_density_total>(lib_f("get_density_total"));
	double charge = 0;
	getDensityTotalFunc(nullptr, nullptr, nullptr, nullptr, &charge);
	return charge;
}

std::vector<double> ZDPlasKinWrapper::get_rates_source_terms() {
	auto getRatesFunc = _lib->getFunction<ZDPlasKin_get_rates>(lib_f("get_rates"));
	std::vector<double> source_terms(_params->getSpeciesMax(), 0);
	getRatesFunc(source_terms.data(), nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
	return source_terms;
}

std::vector<double> ZDPlasKinWrapper::get_rates_reaction_rates() {
	auto getRatesFunc = _lib->getFunction<ZDPlasKin_get_rates>(lib_f("get_rates"));
	std::vector<double> reaction_rates(_params->getReactionsMax(), 0);
	getRatesFunc(nullptr, reaction_rates.data(), nullptr, nullptr, nullptr, nullptr, nullptr);
	return reaction_rates;
}

utils::matrix<double> ZDPlasKinWrapper::get_rates_source_terms_matrix() {
	auto getRatesFunc = _lib->getFunction<ZDPlasKin_get_rates>(lib_f("get_rates"));
	auto *source_terms_matrix = new double[_params->getSpeciesMax()*_params->getReactionsMax()];

	getRatesFunc(nullptr, nullptr, source_terms_matrix, nullptr, nullptr, nullptr, nullptr);

	auto src_terms_matrix = utils::arrayToMatrix(
		source_terms_matrix,
		_params->getSpeciesMax(),
		_params->getReactionsMax()
	);

	delete[] source_terms_matrix; // Free memory!

	return src_terms_matrix;
}

std::vector<double> ZDPlasKinWrapper::get_rates_mean_density() {
	auto getRatesFunc = _lib->getFunction<ZDPlasKin_get_rates>(lib_f("get_rates"));
	std::vector<double> mean_density(_params->getSpeciesMax(), 0);
	getRatesFunc(nullptr, nullptr, nullptr, mean_density.data(), nullptr, nullptr, nullptr);
	return mean_density;
}

std::vector<double> ZDPlasKinWrapper::get_rates_mean_source_terms() {
	auto getRatesFunc = _lib->getFunction<ZDPlasKin_get_rates>(lib_f("get_rates"));
	std::vector<double> mean_source_terms(_params->getSpeciesMax(), 0);
	getRatesFunc(nullptr, nullptr, nullptr, nullptr, mean_source_terms.data(), nullptr, nullptr);
	return mean_source_terms;
}

std::vector<double> ZDPlasKinWrapper::get_rates_mean_reaction_rates() {
	auto getRatesFunc = _lib->getFunction<ZDPlasKin_get_rates>(lib_f("get_rates"));
	std::vector<double> mean_reaction_rate(_params->getSpeciesMax(), 0);
	getRatesFunc(nullptr, nullptr, nullptr, nullptr, nullptr, mean_reaction_rate.data(), nullptr);
	return mean_reaction_rate;
}

utils::matrix<double> ZDPlasKinWrapper::get_rates_mean_source_terms_matrix() {
	auto getRatesFunc = _lib->getFunction<ZDPlasKin_get_rates>(lib_f("get_rates"));
	auto *MEAN_SOURCE_TERMS_MATRIX = new double[_params->getSpeciesMax()*_params->getReactionsMax()];

	getRatesFunc(nullptr,
				 nullptr,
				 nullptr,
				 nullptr,
				 nullptr,
				 nullptr,
				 MEAN_SOURCE_TERMS_MATRIX
	);
	utils::matrix<double> mean_src_terms_matrix = utils::arrayToMatrix(
		MEAN_SOURCE_TERMS_MATRIX,
		_params->getSpeciesMax(),
		_params->getReactionsMax()
	);
	delete[] MEAN_SOURCE_TERMS_MATRIX; // free memory!
	return mean_src_terms_matrix;
}

void ZDPlasKinWrapper::set_config_atol(double atol) {
	auto setConfigFunc = _lib->getFunction<ZDPlasKin_set_config>(lib_f("set_config"));
	setConfigFunc(&atol, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

void ZDPlasKinWrapper::set_config_rtol(double rtol) {
	auto setConfigFunc = _lib->getFunction<ZDPlasKin_set_config>(lib_f("set_config"));
	setConfigFunc(nullptr, &rtol, nullptr, nullptr, nullptr, nullptr, nullptr);
}

void ZDPlasKinWrapper::set_config_silence_mode(bool silence_mode) {
	auto setConfigFunc = _lib->getFunction<ZDPlasKin_set_config>(lib_f("set_config"));
	setConfigFunc(nullptr, nullptr, &silence_mode, nullptr, nullptr, nullptr, nullptr);
}

void ZDPlasKinWrapper::set_config_stat_accum(bool stat_accum) {
	auto setConfigFunc = _lib->getFunction<ZDPlasKin_set_config>(lib_f("set_config"));
	setConfigFunc(nullptr, nullptr, nullptr, &stat_accum, nullptr, nullptr, nullptr);
}

void ZDPlasKinWrapper::set_config_qtplaskin_save(bool save) {
	auto setConfigFunc = _lib->getFunction<ZDPlasKin_set_config>(lib_f("set_config"));
	setConfigFunc(nullptr, nullptr, nullptr, nullptr, &save, nullptr, nullptr);
}

void ZDPlasKinWrapper::set_config_bolsig_ee_frac(double bolsig_ee_frac) {
	auto setConfigFunc = _lib->getFunction<ZDPlasKin_set_config>(lib_f("set_config"));
	setConfigFunc(nullptr, nullptr, nullptr, nullptr, nullptr, &bolsig_ee_frac, nullptr);
}

void ZDPlasKinWrapper::set_config_bolsig_ignore_gas_temperature(bool bolsig_ignore_gas_temperature) {
	auto setConfigFunc = _lib->getFunction<ZDPlasKin_set_config>(lib_f("set_config"));
	setConfigFunc(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &bolsig_ignore_gas_temperature);
}

void ZDPlasKinWrapper::set_condition_gas_temperature(double gas_temperature) {
	auto setConditionFunc = _lib->getFunction<ZDPlasKin_set_conditions>(lib_f("set_conditions"));
	setConditionFunc(&gas_temperature, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

void ZDPlasKinWrapper::set_condition_reduced_frequency(double reduced_frequency) {
	auto setConditionFunc = _lib->getFunction<ZDPlasKin_set_conditions>(lib_f("set_conditions"));
	setConditionFunc(nullptr, &reduced_frequency, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

void ZDPlasKinWrapper::set_condition_reduced_field(double reduced_field) {
	auto setConditionFunc = _lib->getFunction<ZDPlasKin_set_conditions>(lib_f("set_conditions"));
	setConditionFunc(nullptr, nullptr, &reduced_field, nullptr, nullptr, nullptr, nullptr, nullptr);
}

void ZDPlasKinWrapper::set_condition_reduced_elec_temperature(double elec_temperature) {
	auto setConditionFunc = _lib->getFunction<ZDPlasKin_set_conditions>(lib_f("set_conditions"));
	setConditionFunc(nullptr, nullptr, nullptr, &elec_temperature, nullptr, nullptr, nullptr, nullptr);
}

void ZDPlasKinWrapper::set_condition_reduced_gas_heating(bool gas_heating) {
	auto setConditionFunc = _lib->getFunction<ZDPlasKin_set_conditions>(lib_f("set_conditions"));
	setConditionFunc(nullptr, nullptr, nullptr, nullptr, &gas_heating, nullptr, nullptr, nullptr);
}

void ZDPlasKinWrapper::set_condition_reduced_spec_heat_ratio(double spec_heat_ratio) {
	auto setConditionFunc = _lib->getFunction<ZDPlasKin_set_conditions>(lib_f("set_conditions"));
	setConditionFunc(nullptr, nullptr, nullptr, nullptr, nullptr, &spec_heat_ratio, nullptr, nullptr);
}

void ZDPlasKinWrapper::set_condition_reduced_spec_heat_source(double heat_source) {
	auto setConditionFunc = _lib->getFunction<ZDPlasKin_set_conditions>(lib_f("set_conditions"));
	setConditionFunc(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &heat_source, nullptr);
}

void ZDPlasKinWrapper::set_condition_reduced_soft_reset(bool soft_reset) {
	auto setConditionFunc = _lib->getFunction<ZDPlasKin_set_conditions>(lib_f("set_conditions"));
	setConditionFunc(nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, &soft_reset);
}

double ZDPlasKinWrapper::get_condition_gas_temperature() {
	auto getConditionFunc = _lib->getFunction<ZDPlasKin_get_conditions>(lib_f("get_conditions"));
	double gas_temperature = 0;
	getConditionFunc(&gas_temperature,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr);
	return gas_temperature;
}

double ZDPlasKinWrapper::get_condition_reduced_frequency() {
	auto getConditionFunc = _lib->getFunction<ZDPlasKin_get_conditions>(lib_f("get_conditions"));
	double reduced_frequency = 0;
	getConditionFunc(nullptr,
					 &reduced_frequency,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr);
	return reduced_frequency;
}

double ZDPlasKinWrapper::get_condition_reduced_field() {
	auto getConditionFunc = _lib->getFunction<ZDPlasKin_get_conditions>(lib_f("get_conditions"));
	double reduced_field = 0;
	getConditionFunc(nullptr,
					 nullptr,
					 &reduced_field,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr);
	return reduced_field;
}

double ZDPlasKinWrapper::get_condition_elec_temperature() {
	auto getConditionFunc = _lib->getFunction<ZDPlasKin_get_conditions>(lib_f("get_conditions"));
	double elec_temperature = 0;
	getConditionFunc(nullptr,
					 nullptr,
					 nullptr,
					 &elec_temperature,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr);
	return elec_temperature;
}

double ZDPlasKinWrapper::get_condition_elec_drift_velocity() {
	auto getConditionFunc = _lib->getFunction<ZDPlasKin_get_conditions>(lib_f("get_conditions"));
	double drift_velocity = 0;
	getConditionFunc(nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 &drift_velocity,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr);
	return drift_velocity;
}

double ZDPlasKinWrapper::get_condition_elec_diff_coeff() {
	auto getConditionFunc = _lib->getFunction<ZDPlasKin_get_conditions>(lib_f("get_conditions"));
	double elec_diff_coeff = 0;
	getConditionFunc(nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 &elec_diff_coeff,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr);
	return elec_diff_coeff;
}

double ZDPlasKinWrapper::get_condition_elec_mobility_n() {
	auto getConditionFunc = _lib->getFunction<ZDPlasKin_get_conditions>(lib_f("get_conditions"));
	double elec_mobility_n = 0;
	getConditionFunc(nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 &elec_mobility_n,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr);
	return elec_mobility_n;
}

double ZDPlasKinWrapper::get_condition_elec_mu_eps_n() {
	auto getConditionFunc = _lib->getFunction<ZDPlasKin_get_conditions>(lib_f("get_conditions"));
	double elec_mu_eps_n = 0;
	getConditionFunc(nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 &elec_mu_eps_n,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr);
	return elec_mu_eps_n;
}

double ZDPlasKinWrapper::get_condition_elec_diff_eps_n() {
	auto getConditionFunc = _lib->getFunction<ZDPlasKin_get_conditions>(lib_f("get_conditions"));
	double elec_diff_eps_n = 0;
	getConditionFunc(nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 &elec_diff_eps_n,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr);
	return elec_diff_eps_n;
}

double ZDPlasKinWrapper::get_condition_elec_frequency_n() {
	auto getConditionFunc = _lib->getFunction<ZDPlasKin_get_conditions>(lib_f("get_conditions"));
	double elec_frequency_n = 0;
	getConditionFunc(nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 &elec_frequency_n,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr);
	return elec_frequency_n;
}

double ZDPlasKinWrapper::get_condition_elec_power_n() {
	auto getConditionFunc = _lib->getFunction<ZDPlasKin_get_conditions>(lib_f("get_conditions"));
	double elec_power_n = 0;
	getConditionFunc(nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 &elec_power_n,
					 nullptr,
					 nullptr,
					 nullptr);
	return elec_power_n;
}

double ZDPlasKinWrapper::get_condition_elec_power_elastic_n() {
	auto getConditionFunc = _lib->getFunction<ZDPlasKin_get_conditions>(lib_f("get_conditions"));
	double elec_power_elastic_n = 0;
	getConditionFunc(nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 &elec_power_elastic_n,
					 nullptr,
					 nullptr);
	return elec_power_elastic_n;
}

double ZDPlasKinWrapper::get_condition_elec_power_inelastic_n() {
	auto getConditionFunc = _lib->getFunction<ZDPlasKin_get_conditions>(lib_f("get_conditions"));
	double elec_power_inelastic_n = 0;
	getConditionFunc(nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 &elec_power_inelastic_n,
					 nullptr);
	return elec_power_inelastic_n;
}

utils::matrix<double> ZDPlasKinWrapper::get_condition_elec_eedf(size_t ELEC_EEDF_SIZE) {
	auto getConditionFunc = _lib->getFunction<ZDPlasKin_get_conditions>(lib_f("get_conditions"));
	auto ELEC_EEDF = new double[2*ELEC_EEDF_SIZE];
	getConditionFunc(nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 nullptr,
					 ELEC_EEDF);

	utils::matrix<double> elec_eedf = utils::arrayToMatrix(ELEC_EEDF, 2, ELEC_EEDF_SIZE);
	delete[] ELEC_EEDF;    // Free memory!
	return elec_eedf;
}
void ZDPlasKinWrapper::write_qtplaskin(double time) {
	auto writeQTPlaskinFunc = _lib->getFunction<ZDPlasKin_write_qtplaskin>(lib_f("write_qtplaskin"));
	writeQTPlaskinFunc(time, nullptr);
}

void ZDPlasKinWrapper::setLibrary(ILibraryLoader *lib) {
    _lib = lib;
}

void ZDPlasKinWrapper::setParams(ZDPlasKinParams *params) {
    _params = params;
}

