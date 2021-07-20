#ifndef ZDPLASKIN_INCLUDE_ZDPLASKINFUNCTIONDEFS_H_
#define ZDPLASKIN_INCLUDE_ZDPLASKINFUNCTIONDEFS_H_

// Fortran specific types
using complex = struct { float re, im; };
using double_complex = struct { double re, im; };


// Interface idk if need to be callable, probably not
using ZDPlasKin_bolsig_Init = void (*)(char *a, int a_len);
using ZDPlasKin_bolsig_ReadCollisions = void (*)(char *a, int a_len);
using ZDPlasKin_bolsig_GetNumCollisions = void (*)(int &i, int &j);
using ZDPlasKin_bolsig_GetSpeciesName = void (*)(char *a, int &i, int a_len);
using ZDPlasKin_bolsig_GetReactionName = void (*)(char *a, int &i, int a_len);
using ZDPlasKin_bolsig_SolveBoltzmann = double *(*)(int &a_len, double a[], int &b_len, double b[]);
using ZDPlasKin_bolsig_GetEEDF = void *(double **a, int &i);

using ZDPlasKin_init = void (*)();
using ZDPlasKin_timestep = double (*)(double &time, double &dtime);
using ZDPlasKin_timestep_explicit = double (*)(double &time,
											   double &dtime,
											   double &rtol_loc,
											   double &atol_loc,
											   double &switch_implicit);
using ZDPlasKin_bolsig_rates = void (*)(bool &lbolsig_force);
using ZDPlasKin_get_species_index = int (*)(char *str, int &i, int str_len);
using ZDPlasKin_set_density = void (*)(char *string, double &DENS, bool &LDENS_CONST, int string_len);
using ZDPlasKin_get_density = void (*)(char *string, double &DENS, bool &LDENS_CONST, int string_len);
using ZDPlasKin_get_density_total = void (*)(double &ALL_SPECIES,
											 double &ALL_NEUTRAL,
											 double &ALL_ION_POSITIVE,
											 double &ALL_ION_NEGATIVE,
											 double &ALL_CHARGE);
using ZDPlasKin_get_rates = void (*)(double SOURCE_TERMS[],
									 double REACTION_RATES[],
									 double **SOURCE_TERMS_MATRIX,
									 double MEAN_DENSITY[],
									 double MEAN_SOURCE_TERMS[],
									 double MEAN_REACTION_RATES[],
									 double **MEAN_SOURCE_TERMS_MATRIX);

using ZDPlasKin_set_config = void (*)(double &ATOL,
									  double &RTOL,
									  bool &SILENCE_MODE,
									  bool &STAT_ACCUM,
									  bool &QTPLASKIN_SAVE,
									  double &BOLSIG_EE_FRAC,
									  bool &BOLSIG_IGNORE_GAS_TEMPERATURE);

using ZDPlasKin_set_conditions = void (*)(double &GAS_TEMPERATURE,
										  double &REDUCED_FREQUENCY,
										  double &REDUCED_FIELD,
										  double &ELEC_TEMPERATURE,
										  bool &GAS_HEATING,
										  double &SPEC_HEAT_RATIO,
										  double &HEAT_SOURCE,
										  bool &SOFT_RESET);
using ZDPlasKin_get_conditions = void (*)(double &GAS_TEMPERATURE,
										  double &REDUCED_FREQUENCY,
										  double &REDUCED_FIELD,
										  double &ELEC_TEMPERATURE,
										  double &ELEC_DRIFT_VELOCITY,
										  double &ELEC_DIFF_COEFF,
										  double &ELEC_MOBILITY_N,
										  double &ELEC_MU_EPS_N,
										  double &ELEC_DIFF_EPS_N,
										  double &ELEC_FREQUENCY_N,
										  double &ELEC_POWER_N,
										  double &ELEC_POWER_ELASTIC_N,
										  double &ELEC_POWER_INELASTIC_N,
										  double **ELEC_EEDF);
using ZDPlasKin_reset = void (*)();
using ZDPlasKin_stop = void (*)(char *string, int string_len);
using ZDPlasKin_write_file = void (*)(char *FILE_SPECIES,
									  char *FILE_REACTIONS,
									  char *FILE_SOURCE_MATRIX,
									  int &FILE_UNIT,
									  int FILE_SPECIFIES_len,
									  int FILE_REACTIONS_len,
									  int FILE_SOURCE_MATRIX_len);
using ZDPlasKin_write_qtplaskin = void (*)(double &time, bool &LFORCE_WRITE);
using ZDPlasKin_reac_source_matrix = void (*)(double reac_rate_local[], double **reac_source_local);
using ZDPlasKin_fex = double (*)(int &neq, double &t, double y[], double ydot[]);
using ZDPlasKin_jex = double (*)(int &neq, double &t, double y[], int &ml, int &mu, double **pd, int &nrpd);
using ZDPlasKin_reac_rates = void (*)(double &Time);

#endif //ZDPLASKIN_INCLUDE_ZDPLASKINFUNCTIONDEFS_H_
