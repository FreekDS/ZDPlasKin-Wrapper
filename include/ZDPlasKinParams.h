#ifndef ZDPLASKIN_SRC_PLASKINPARAMS_H_
#define ZDPLASKIN_SRC_PLASKINPARAMS_H_

#include <string>

class ZDPlasKinParams {
 private:
	std::string _filePath;
	int _speciesMax = 0;
	int _speciesElectrons = 0;
	int _speciesLength = 0;
	int _reactionsMax = 0;
	int _reactionsLength = 0;

 public:

	ZDPlasKinParams() noexcept = default;

	explicit ZDPlasKinParams(std::string file_path);

	void setFilePath(std::string file_path);

	void readParams();

	[[nodiscard]] int getSpeciesMax() const;
	[[nodiscard]] int getSpeciesElectrons() const;
	[[nodiscard]] int getSpeciesLength() const;
	[[nodiscard]] int getReactionsMax() const;
	[[nodiscard]] int getReactionsLength() const;
};

#endif //ZDPLASKIN_SRC_PLASKINPARAMS_H_
