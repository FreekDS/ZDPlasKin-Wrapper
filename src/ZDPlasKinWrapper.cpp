#include "ZDPlasKinWrapper.h"

std::string ZDPlasKinWrapper::lib_f(const std::string &name) {
	return "__zdplaskin_MOD_zdplaskin_" + name;
}

void ZDPlasKinWrapper::init() {
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
	auto timestepExplicitFunc = _lib->getFunction<ZDPlasKin_timestep_explicit>(lib_f("timestep_explicit"));
	timestepExplicitFunc(time, dtime, rtol_loc, atol_loc, switch_implicit);
	return dtime;
}

void ZDPlasKinWrapper::bolsig_rates(bool lbolsig_force) {
	auto bolsigRatesFunc = _lib->getFunction<ZDPlasKin_bolsig_rates>(lib_f("bolsig_rates"));
	bolsigRatesFunc(lbolsig_force);
}

int ZDPlasKinWrapper::get_species_index(const std::string &string) {
	int result = 0;
	auto speciesIndexFunc = _lib->getFunction<ZDPlasKin_get_species_index>(lib_f("get_species_index"));
	speciesIndexFunc(string.c_str(), result, string.size());
	return result;
}

void ZDPlasKinWrapper::set_density(const std::string &string, double DENS, bool LDENS_CONST) {
	auto setDensityFunc = _lib->getFunction<ZDPlasKin_set_density>(lib_f("set_density"));
	setDensityFunc(string.c_str(), DENS, LDENS_CONST, string.size());
}

ZDPlasKinWrapper::DensityResult ZDPlasKinWrapper::get_density(const std::string &string) {
	auto getDensityFunc = _lib->getFunction<ZDPlasKin_get_density>(lib_f("get_density"));
	double DENS = 0;
	bool LDENS_CONST = false;
	getDensityFunc(string.c_str(), DENS, LDENS_CONST, string.size());
	return {DENS, LDENS_CONST};
}

ZDPlasKinWrapper::TotalDensityResult ZDPlasKinWrapper::get_density_total() {
	auto getDensityTotalFunc = _lib->getFunction<ZDPlasKin_get_density_total>(lib_f("get_density_total"));
	double ALL_SPECIES = 0;
	double ALL_NEUTRAL = 0;
	double ALL_ION_NEGATIVE = 0;
	double ALL_ION_POSITIVE = 0;
	double ALL_CHARGE = 0;
	getDensityTotalFunc(ALL_SPECIES, ALL_NEUTRAL, ALL_ION_POSITIVE, ALL_ION_NEGATIVE, ALL_CHARGE);
	return {ALL_SPECIES, ALL_NEUTRAL, ALL_ION_NEGATIVE, ALL_ION_POSITIVE, ALL_CHARGE};
}

ZDPlasKinWrapper::RatesResult ZDPlasKinWrapper::get_rates() {
	auto getRatesFunc = _lib->getFunction<ZDPlasKin_get_rates>(lib_f("get_rates"));
	// TODO check if we can access parameters of modules to create an array of appropriate size here
	return ZDPlasKinWrapper::RatesResult();
}

void ZDPlasKinWrapper::set_config(double ATOL,
								  double RTOL,
								  bool SILENCE_MODE,
								  bool STAT_ACCUM,
								  bool QTPLASKIN_SAVE,
								  double BOLSIG_EE_FRAC,
								  bool BOLSIG_IGNORE_GAS_TEMPERATURE) {
	auto setConfigFunc = _lib->getFunction<ZDPlasKin_set_config>(lib_f("set_config"));
	setConfigFunc(ATOL, RTOL, SILENCE_MODE, STAT_ACCUM, QTPLASKIN_SAVE, BOLSIG_EE_FRAC, BOLSIG_IGNORE_GAS_TEMPERATURE);
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
	setConditionsFunc(GAS_TEMPERATURE,
					  REDUCED_FREQUENCY,
					  REDUCED_FIELD,
					  ELEC_TEMPERATURE,
					  GAS_HEATING,
					  SPEC_HEAT_RATIO,
					  HEAT_SOURCE,
					  SOFT_RESET);
}

ZDPlasKinWrapper::GetConditionsResult ZDPlasKinWrapper::get_conditions() {
	auto getConditionsFunc = _lib->getFunction<ZDPlasKin_get_conditions>(lib_f("get_conditions"));
	// Todo: check if we can access module parameters to assing appropriate array sizes
	return ZDPlasKinWrapper::GetConditionsResult();
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
	writeFileFunc(FILE_SPECIES.c_str(),
				  FILE_REACTIONS.c_str(),
				  FILE_SOURCE_MATRIX.c_str(),
				  FILE_UNIT,
				  FILE_SPECIES.size(),
				  FILE_REACTIONS.size(),
				  FILE_SOURCE_MATRIX.size());
}

void ZDPlasKinWrapper::write_qtplaskin(double time, bool LFORCE_WRITE) {
	auto writeQTPlaskinFunc = _lib->getFunction<ZDPlasKin_write_qtplaskin>(lib_f("write_qtplaskin"));
	writeQTPlaskinFunc(time, LFORCE_WRITE);
}

double **ZDPlasKinWrapper::reac_source_matrix(double *reac_rate_local) {
	auto reacSourceMatrixFunc = _lib->getFunction<ZDPlasKin_reac_source_matrix>(lib_f("reac_source_matrix"));
	// TODO array sizes again
	// TODO convert to vector
	return nullptr;
}

double *ZDPlasKinWrapper::fex(int neq, double t, double *y) {
	auto fexFunc = _lib->getFunction<ZDPlasKin_fex>(lib_f("fex"));
	auto *ydot = new double[neq];
	// TODO convert to vector
	fexFunc(neq, t, y, ydot);
	return ydot;
}

double **ZDPlasKinWrapper::jex(int neq, double t, double *y, int ml, int mu, int nrpd) {
	auto jexFunc = _lib->getFunction<ZDPlasKin_jex>(lib_f("jex"));
	// Todo convert to vector
	auto **pd = new double *[nrpd];
	for (int i = 0; i < nrpd; i++) {
		pd[i] = new double[neq];
	}
	jexFunc(neq, t, y, ml, mu, pd, nrpd);
	return pd;
}

void ZDPlasKinWrapper::reac_rates(double time) {
	auto reacRatesFunc = _lib->getFunction<ZDPlasKin_reac_rates>(lib_f("reac_rates"));
	reacRatesFunc(time);
}

ZDPlasKinWrapper::ZDPlasKinWrapper(ILibraryLoader *lib) : _lib(lib) {}

