#include <memory>
#include <string>
#include <iostream>
#ifndef ORGANISM_H
#define ORGANISM_H

class Organism 				{
private:
	std::unique_ptr<char[]> genotype;
	int fitness;
	int genoTypeSize;
public:
	Organism();
	Organism(std::string, const int&);
	Organism(const Organism&);
	void operator= (const Organism&);
	char getAlleleAt(const int&) const;
	void mutateAlleleAt(const int&, const char);
	void setFitness(const int&);
	int getFitness() const;
	void printGenotype();
};
Organism::Organism(){
	genotype = nullptr;
	genoTypeSize = 0;
	fitness = 0;
}
Organism::Organism(std::string genotype, const int& geno_size) 	{
	this->genotype = std::make_unique<char[]>(geno_size);
	fitness = 0;
	genoTypeSize = geno_size;
	for (int i = 0; i < geno_size; i++)
		this->genotype[i] = genotype[i];

}
Organism::Organism(const Organism& organ) {
	fitness = organ.fitness;
	genoTypeSize = organ.genoTypeSize;
	genotype.reset();
	genotype = std::make_unique<char[]>(genoTypeSize);
	for (int i = 0; i < genoTypeSize; i++)
		genotype[i] = organ.genotype[i];
}
void Organism::operator= (const Organism& organ) {
	fitness = organ.fitness;
	genoTypeSize = organ.genoTypeSize;
	genotype.reset();
	genotype = std::make_unique<char []>(genoTypeSize);
	for (int i = 0; i < genoTypeSize; i++)
		genotype[i] = organ.genotype[i];
	
}
char Organism::getAlleleAt(const int& index) const {
	return genotype[index];
}

void Organism::setFitness(const int& fitness) {
	this->fitness = fitness;
}
void Organism::mutateAlleleAt(const int& index, const char mutation) {
	genotype[index] = mutation;
}
int Organism::getFitness() const { return fitness; }

void Organism::printGenotype() {
	for (int i = 0; i < genoTypeSize; i++) 		{
		std::cout << genotype[i];
	}
	std::cout << std::endl;
}
#endif // !ORGANISM_H

