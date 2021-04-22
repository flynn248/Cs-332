#include <memory>
#include <string>
#include <iostream>
#ifndef ORGANISM_H
#define ORGANISM_H

class Organism 				{
private:
	std::unique_ptr<char[]> genotype;
	int fitness;
public:
	Organism();
	Organism(std::string, const int&);
	char getAlleleAt(const int&) const;
	void setFitness(const int&);
	int getFitness() const;
};
Organism::Organism(){
	genotype = nullptr;
	fitness = 0;
}
Organism::Organism(std::string genotype, const int& geno_size) 	{
	this->genotype = std::make_unique<char[]>(geno_size);
	fitness = 0;
	for (int i = 0; i < geno_size; i++)
		this->genotype[i] = genotype[i];

}

char Organism::getAlleleAt(const int& index) const {
	return genotype[index];
}

void Organism::setFitness(const int& fitness) {
	this->fitness = fitness;
}

int Organism::getFitness() const { return fitness; }
#endif // !ORGANISM_H

