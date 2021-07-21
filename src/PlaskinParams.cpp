#include <iostream>
#include <fstream>
#include "PlaskinParams.h"
#include "helpers/string_helpers.h"

int PlaskinParams::getSpeciesMax() const {
	return _speciesMax;
}

int PlaskinParams::getSpeciesElectrons() const {
	return _speciesElectrons;
}

int PlaskinParams::getSpeciesLength() const {
	return _speciesLength;
}

int PlaskinParams::getReactionsMax() const {
	return _reactionsMax;
}

int PlaskinParams::GetReactionsLength() const {
	return _reactionsLength;
}

PlaskinParams::PlaskinParams(const std::string &file_path)
	: _filePath(file_path),
	  _speciesElectrons(0),
	  _speciesLength(0),
	  _speciesMax(0),
	  _reactionsMax(0),
	  _reactionsLength(0) {}

void PlaskinParams::readGlobals() {
	std::ifstream file(_filePath);
	if (file.is_open()) {
		std::string line;
		std::string moduleStart = "module ZDPlasKin";
		while (line.substr(0, moduleStart.size()) != moduleStart) {
			std::getline(file, line);
		}
		for (int i = 0; i < 7; i++) {
			std::getline(file, line);
		}
		utils::removeWhitespaces(line);
		utils::removeSubstr(line, "integer,parameter::");
		std::vector<std::string> parts = utils::split(line, ",");

		for (auto &part : parts) {
			std::vector<std::string> keyValue = utils::split(part, "=");
			std::string key = keyValue[0];
			int value = std::stoi(keyValue[1]);
			if (key == "species_max")
				_speciesMax = value;
			else if (key == "species_electrons")
				_speciesElectrons = value;
			else if (key == "species_length")
				_speciesLength = value;
			else if (key == "reactions_max")
				_reactionsMax = value;
			else
				throw std::exception();
		}
	} else {
		throw std::exception();
	}
}
