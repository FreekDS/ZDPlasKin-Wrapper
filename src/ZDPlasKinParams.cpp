#include <iostream>
#include <fstream>
#include "ZDPlasKinParams.h"
#include "ZDPlasKinExceptions.h"
#include "helpers/string_helpers.h"

int ZDPlasKinParams::getSpeciesMax() const {
	return _speciesMax;
}

int ZDPlasKinParams::getSpeciesElectrons() const {
	return _speciesElectrons;
}

int ZDPlasKinParams::getSpeciesLength() const {
	return _speciesLength;
}

int ZDPlasKinParams::getReactionsMax() const {
	return _reactionsMax;
}

int ZDPlasKinParams::getReactionsLength() const {
	return _reactionsLength;
}

ZDPlasKinParams::ZDPlasKinParams(std::string file_path)
	: _filePath(std::move(file_path)),
	  _speciesElectrons(0),
	  _speciesLength(0),
	  _speciesMax(0),
	  _reactionsMax(0),
	  _reactionsLength(0) {}

void ZDPlasKinParams::readParams() {
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
			else if (key == "reactions_length")
				_reactionsLength = value;
			else {
				std::cout << key << std::endl;
				throw ParseException("Unknown module option '" + key + "'");
			}
		}
	} else {
		throw ParseException("Cannot open '" + _filePath + "'");
	}
}

void ZDPlasKinParams::setFilePath(std::string file_path) {
	_filePath = std::move(file_path);
}
