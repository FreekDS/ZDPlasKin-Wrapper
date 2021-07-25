#ifndef ZDPLASKIN_SRC_PLASKINPARAMS_H_
#define ZDPLASKIN_SRC_PLASKINPARAMS_H_

#include <string>

class ZDPlasKinParams {
 private:
	std::string _filePath;
	int _speciesMax;
	int _speciesElectrons;
	int _speciesLength;
	int _reactionsMax;
	int _reactionsLength;

 public:

	explicit ZDPlasKinParams(const std::string &file_path);

	void readParams();

	[[nodiscard]] int getSpeciesMax() const;
	[[nodiscard]] int getSpeciesElectrons() const;
	[[nodiscard]] int getSpeciesLength() const;
	[[nodiscard]] int getReactionsMax() const;
	[[nodiscard]] int getReactionsLength() const;
};

#endif //ZDPLASKIN_SRC_PLASKINPARAMS_H_
