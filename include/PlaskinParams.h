#ifndef ZDPLASKIN_SRC_PLASKINPARAMS_H_
#define ZDPLASKIN_SRC_PLASKINPARAMS_H_

#include <string>

class PlaskinParams {
 private:
	const std::string& _filePath;
	int _speciesMax;
	int _speciesElectrons;
	int _speciesLength;
	int _reactionsMax;
	int _reactionsLength;

 public:

	explicit PlaskinParams(const std::string &file_path);

	void readParams();

	int getSpeciesMax() const;
	int getSpeciesElectrons() const;
	int getSpeciesLength() const;
	int getReactionsMax() const;
	int getReactionsLength() const;
};

#endif //ZDPLASKIN_SRC_PLASKINPARAMS_H_
