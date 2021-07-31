#include <iostream>
#include <iomanip>
#include "example1.h"

void example1(ZDPlasKinWrapper &zdplaskin) {
	double gas_temperature = 301.0e0;
	double reduced_field = 51.0e0;
	double density_ini_ar = 2.5e19;
	double density_ini_elec = 1.0e0;
	double time = 0.0;
	double time_end = 3.0e-7;
	double dtime = 1.0e-8;
	int i;

	std::cout << std::scientific << std::setprecision(4);
	std::cout << "TWO REACTION TEST CASE" << std::endl;
	zdplaskin.init("bolsigdb.dat", true);

	zdplaskin.set_condition_gas_temperature(gas_temperature);
	zdplaskin.set_condition_reduced_field(reduced_field);

	zdplaskin.set_density("Ar", density_ini_ar);
	zdplaskin.set_density("e", density_ini_elec);
	zdplaskin.set_density("Ar^+", density_ini_elec);


	std::cout << "Time_s\t\t";

	for(const auto& name : zdplaskin.getSpeciesName()) {
		std::cout << name << "\t\t";
	}
	std::cout << std::endl;
	std::cout << time << "\t";
	for(const auto& dens : zdplaskin.getDensity()) {
		std::cout << dens << "\t";
	}
	std::cout << std::endl;

	while(time < time_end) {
		zdplaskin.timestep(time, dtime);
		time = time + dtime;
		std::cout << time << "\t";
		for(const auto& dens : zdplaskin.getDensity()) {
			std::cout << dens << "\t";
		}
		std::cout << std::endl;
	}
}
