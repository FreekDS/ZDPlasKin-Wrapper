#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <ZDPlasKinWrapper.h>
#include <ZDPlasKinCompiler.h>
#include <platform.h>
#include <iostream>
#include <filesystem>
#include "helpers/string_helpers.h"

#ifdef WINDOWS

#include <windows/WindowsLoader.h>

using PlatformLoader = WindowsLoader;

#endif

#ifdef UNIX

#include "unix/UnixLoader.h"
using PlatformLoader = UnixLoader;

#endif

static PlatformLoader libraryLoader{};
static ZDPlasKinParams params{};
static ZDPlasKinWrapper zdp{};

namespace py = pybind11;
using namespace pybind11::literals;


void preprocess(const std::string &kinetFile) {
    std::cout << std::filesystem::current_path() << '\n';
    ZDPlasKinCompiler::preprocess(kinetFile);
    ZDPlasKinCompiler::compile();

    std::string srcPath = std::filesystem::canonical(kinetFile).parent_path().string();

    std::cout << "Source Path: " << srcPath << '\n';

//    utils::removeSubstr(srcPath, kinetFile);
//    std::cout << "Source Path 2: " << srcPath << '\n';
    std::string path = srcPath + "\\zdplaskin.dll";

    libraryLoader = PlatformLoader{path};
    params = ZDPlasKinParams{srcPath + "\\zdplaskin_m.F90"};

    zdp.setLibrary(&libraryLoader);
    zdp.setParams(&params);

    libraryLoader.init();
    params.readParams();
}


PYBIND11_MODULE(ZDPlaskin, m) {

    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------
        .. currentmodule:: scikit_build_example
        .. autosummary::
           :toctree: _generate
           add
           subtract
    )pbdoc";

    m.def("test", []() { std::cout << "Test from CPP" << std::endl; }, R"pbdoc(
        Test the CPP bindings
    )pbdoc");

    m.def("preprocess", &preprocess, R"pbdoc(
        Preprocess the kinet input file in order to generate the Fortran Library
        -- kinetFile: absolute path to the input file
    )pbdoc");

    m.def("init", [&](const std::string &bolsigFile = "", bool updateDB = false) {
        zdp.init(bolsigFile, updateDB);
    });

    // TODO check by reference/by value + change result to input
    m.def("timestep", [&](double time) {
        double result;
        zdp.timestep(time, result);
        return result;
    });

    m.def("timestep_explicit", [&](double time, double rtol_loc, double atol_loc, double switch_implicit) {
        double result;
        zdp.timestep_explicit(time, result, rtol_loc, atol_loc, switch_implicit);
        return result;
    });

    m.def("timestep_explicit", [&](double time, double rtol_loc, double atol_loc) {
        double result;
        zdp.timestep_explicit(time, result, rtol_loc, atol_loc);
        return result;
    });

    m.def("bolsig_rates", [&]() {
        zdp.bolsig_rates();
    });

    m.def("bolsig_rates", [&](bool lbolsig_force) {
        zdp.bolsig_rates(lbolsig_force);
    });

    m.def("get_species_index", [&](const std::string &string) {
        return zdp.get_species_index(string);
    });

    m.def("set_density", [&](const std::string &string, double DENS = 0.0, bool LDENS_CONST = false) {
        zdp.set_density(string, DENS, LDENS_CONST);
    });

    m.def("set_density", [&](const std::string &string, bool LDENS_CONST = false) {
        zdp.set_density(string, LDENS_CONST);
    });

    m.def("get_density", [&](const std::string &string) {
        ZDPlasKinWrapper::DensityResult r = zdp.get_density(string);
        return py::dict("dens"_a = r.DENS, "ldens_const"_a = r.LDENS_CONST);
    });

    m.def("get_density_dens", [&](const std::string &string) {
        return zdp.get_density_dens(string);
    });

    m.def("get_density_total", [&]() {
        ZDPlasKinWrapper::TotalDensityResult r = zdp.get_density_total();
        return py::dict(
                "all_species"_a = r.ALL_SPECIES,
                "all_neutral"_a = r.ALL_NEUTRAL,
                "all_ion_positive"_a = r.ALL_ION_POSITIVE,
                "all_ion_negative"_a = r.ALL_ION_NEGATIVE,
                "all_charge"_a = r.ALL_CHARGE
        );
    });

    m.def("get_density_total_species", [&]() {
        return zdp.get_density_total_species();
    });

    m.def("get_density_total_neutral", [&]() {
        return zdp.get_density_total_neutral();
    });

    m.def("get_density_total_ion_positive", [&]() {
        return zdp.get_density_total_ion_positive();
    });

    m.def("get_density_total_ion_negative", [&]() {
        return zdp.get_density_total_ion_negative();
    });

    m.def("get_density_total_charge", [&]() {
        return zdp.get_density_total_charge();
    });

    m.def("get_rates", [&]() {
        ZDPlasKinWrapper::RatesResult r = zdp.get_rates();
        return py::dict(
                "source_terms"_a = r.SOURCE_TERMS,
                "reaction_rates"_a = r.REACTION_RATES,
                "source_terms_matrix"_a = r.SOURCE_TERMS_MATRIX,
                "mean_density"_a = r.MEAN_DENSITY,
                "mean_source_terms"_a = r.MEAN_SOURCE_TERMS,
                "mean_reaction_rates"_a = r.MEAN_REACTION_RATES,
                "mean_source_terms_matrix"_a = r.MEAN_SOURCE_TERMS_MATRIX
        );
    });

    m.def("get_rates_source_terms", [&]() {
        return zdp.get_rates_source_terms();
    });

    m.def("get_rates_reaction_rates", [&]() {
        return zdp.get_rates_reaction_rates();
    });

    m.def("get_rates_source_terms_matrix", [&]() {
        return zdp.get_rates_source_terms_matrix();
    });

    m.def("get_rates_mean_density", [&]() {
        return zdp.get_rates_mean_density();
    });

    m.def("get_rates_mean_source_terms", [&]() {
        return zdp.get_rates_mean_source_terms();
    });

    m.def("get_rates_mean_reaction_rates", [&]() {
        return zdp.get_rates_mean_reaction_rates();
    });

    m.def("get_rates_mean_source_terms_matrix", [&]() {
        return zdp.get_rates_mean_source_terms_matrix();
    });

    m.def("set_config", [&](
            double atol,
            double rtol,
            bool silence_mode,
            bool stat_accum,
            bool qtplaskin_save,
            double bolsig_ee_frac,
            bool bolsig_ignore_gas_temperature) {
        zdp.set_config(atol, rtol, silence_mode, stat_accum, qtplaskin_save, bolsig_ee_frac,
                       bolsig_ignore_gas_temperature);
    });

    m.def("set_config_atol", [&](double atol) {
        zdp.set_config_atol(atol);
    });

    m.def("set_config_rtol", [&](double rtol) {
        zdp.set_config_rtol(rtol);
    });

    m.def("set_config_silence_mode", [&](bool silence_mode) {
        zdp.set_config_silence_mode(silence_mode);
    });

    m.def("set_config_stat_accum", [&](bool stat_accum) {
        zdp.set_config_stat_accum(stat_accum);
    });

    m.def("set_config_qtplaskin_save", [&](bool save) {
        zdp.set_config_qtplaskin_save(save);
    });

    m.def("set_config_bolsig_ee_frac", [&](double bolsig_ee_frac) {
        zdp.set_config_bolsig_ee_frac(bolsig_ee_frac);
    });

    m.def("set_config_ignore_gas_temperature", [&](bool ignore_gas_temp) {
        zdp.set_config_bolsig_ignore_gas_temperature(ignore_gas_temp);
    });

    m.def("set_conditions",
          [&](double gas_temperature, double reduced_frequency, double reduced_field, double elec_temperature,
              bool gas_heating, double spec_heat_ratio, double heat_source, bool soft_reset) {
              zdp.set_conditions(
                      gas_temperature, reduced_frequency, reduced_field, elec_temperature, gas_heating, spec_heat_ratio,
                      heat_source, soft_reset
              );
          });

    m.def("set_conditions_gas_temperature", [&](double gas_temperature) {
        zdp.set_condition_gas_temperature(gas_temperature);
    });

    m.def("set_conditions_reduced_frequency", [&](double reduced_frequency) {
        zdp.set_condition_reduced_frequency(reduced_frequency);
    });

    m.def("set_conditions_reduced_field", [&](double reduced_field) {
        zdp.set_condition_reduced_field(reduced_field);
    });

    m.def("set_conditions_reduced_elec_temperature", [&](double elec_temp) {
        zdp.set_condition_reduced_elec_temperature(elec_temp);
    });

    m.def("set_conditions_reduced_gas_heating", [&](bool gas_heating) {
        zdp.set_condition_reduced_gas_heating(gas_heating);
    });

    m.def("set_conditions_reduced_spec_heat_ratio", [&](double spec_heat_ratio) {
        zdp.set_condition_reduced_spec_heat_ratio(spec_heat_ratio);
    });

    m.def("set_conditions_reduced_spec_heat_source", [&](double heat_source) {
        zdp.set_condition_reduced_spec_heat_source(heat_source);
    });

    m.def("set_conditions_reduced_soft_reset", [&](bool soft_reset) {
        zdp.set_condition_reduced_soft_reset(soft_reset);
    });

    m.def("get_conditions", [&](size_t ELEC_EEDF_SIZE) {
        ZDPlasKinWrapper::GetConditionsResult r = zdp.get_conditions(ELEC_EEDF_SIZE);
    });

    m.def("get_condition_gas_temperature", [&]() {
        return zdp.get_condition_gas_temperature();
    });

    m.def("get_condition_reduced_frequency", [&]() {
        return zdp.get_condition_reduced_frequency();
    });

    m.def("get_condition_reduced_field", [&]() {
        return zdp.get_condition_reduced_field();
    });

    m.def("get_condition_elec_temperature", [&]() {
        return zdp.get_condition_elec_temperature();
    });

    m.def("get_condition_elec_drift_velocity", [&]() {
        return zdp.get_condition_elec_drift_velocity();
    });

    m.def("get_condition_elec_diff_coeff", [&]() {
        return zdp.get_condition_elec_diff_coeff();
    });

    m.def("get_condition_elec_mobility_n", [&]() {
        return zdp.get_condition_elec_mobility_n();
    });

    m.def("get_condition_elec_mu_eps_n", [&]() {
        return zdp.get_condition_elec_mu_eps_n();
    });

    m.def("get_condition_elec_diff_eps_n", [&]() {
        return zdp.get_condition_elec_diff_eps_n();
    });

    m.def("get_condition_elec_frequency_n", [&]() {
        return zdp.get_condition_elec_frequency_n();
    });

    m.def("get_condition_elec_power_n", [&]() {
        return zdp.get_condition_elec_power_n();
    });

    m.def("get_condition_elec_power_elastic_n", [&]() {
        return zdp.get_condition_elec_power_elastic_n();
    });

    m.def("get_condition_elec_power_inelastic_n", [&]() {
        return zdp.get_condition_elec_power_inelastic_n();
    });

    m.def("get_condition_elec_eedf", [&](size_t ELEC_EEDF_SIZE) {
        return zdp.get_condition_elec_eedf(ELEC_EEDF_SIZE);
    });

    m.def("reset", [&]() {
        zdp.reset();
    });

    m.def("stop", [&](const std::string &string) {
        zdp.stop(string);
    });

    m.def("write_file",
          [&](const std::string &file_species, const std::string &file_reactions, const std::string &file_source_matrix,
              int file_unit) {
              zdp.write_file(file_species, file_reactions, file_source_matrix, file_unit);
          });

    m.def("write_qtplaskin", [&](double time, bool lforce_writing) {
        zdp.write_qtplaskin(time, lforce_writing);
    });

    m.def("write_qtplaskin", [&](double time) {
        zdp.write_qtplaskin(time);
    });

    m.def("reac_source_matrix", [&](double reac_rate_local[]) {
        return zdp.reac_source_matrix(reac_rate_local);
    });

    m.def("fex", [&](int neq, double t, double y[]) {
        return zdp.fex(neq, t, y);
    });

    m.def("jex", [&](int neq, double t, double y[], int ml, int mv, int nrpd) {
        return zdp.jex(neq, t, y, ml, mv, nrpd);
    });

    m.def("reac_rates", [&](double time) {
        zdp.reac_rates(time);
    });

    m.def("get_density_variable", [&]() {
        return zdp.getDensity();
    });

    m.def("get_species_charge", [&]() {
        return zdp.getSpeciesCharge();
    });

    m.def("get_species_name", [&]() {
        return zdp.getSpeciesName();
    });

    m.def("get_reaction_sign", [&]() {
        return zdp.getReactionSign();
    });

    m.def("get_l_reaction_block", [&]() {
        return zdp.getLReactionBlock();
    });

}

