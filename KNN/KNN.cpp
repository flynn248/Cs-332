#include <iostream>
#include <string>
#include <cmath>

const int NUM_ENTRIES = 15;
const int k = 5;
const int eWords = 375;
const int eSpell = 20;

void hardCodeValues(float* wc, float* sp, bool* spam) {
	wc[0] = 49;
	wc[1] = 63;
	wc[2] = 137;
	wc[3] = 254;
	wc[4] = 363;
	wc[5] = 218;
	wc[6] = 54;
	wc[7] = 118;
	wc[8] = 189;
	wc[9] = 132;
	wc[10] = 249;
	wc[11] = 260;
	wc[12] = 363;
	wc[13] = 438;
	wc[14] = 468;

	sp[0] = 10;
	sp[1] = 40;
	sp[2] = 21;
	sp[3] = 34;
	sp[4] = 9;
	sp[5] = 11;
	sp[6] = 77;
	sp[7] = 64;
	sp[8] = 69;
	sp[9] = 36;
	sp[10] = 58;
	sp[11] = 17;
	sp[12] = 32;
	sp[13] = 54;
	sp[14] = 13;

	spam[0] = false;
	spam[1] = false;
	spam[2] = false;
	spam[3] = false;
	spam[4] = false;
	spam[5] = false;
	spam[6] = true;
	spam[7] = true;
	spam[8] = true;
	spam[9] = true;
	spam[10] = true;
	spam[11] = true;
	spam[12] = true;
	spam[13] = true;
	spam[14] = true;
}

int* findKNNIndex(float* dist) {
	int* kNNIndex = new int[k];

	for (int i = 0; i < k; i++) 		{
		int minIndex = 0;
		for (int j = 0; j < NUM_ENTRIES; j++) 		{
			if (dist[minIndex] > dist[j])
				minIndex = j;
		}
		
		dist[minIndex] = 9999999.f; //"cheating" my way through avoiding an error in the design of the code
									//Need a better method of finding the index of the first KNN, but this will work here
		kNNIndex[i] = minIndex;
	}

	return kNNIndex;
}

std::string labelType(const int i, const bool* label) {
	if (label[i] == true) { return "SPAM"; }
	else { return "NOT SPAM"; }
}

int main() {
	float wordCnt[NUM_ENTRIES],
		spellGr[NUM_ENTRIES];
	bool label[NUM_ENTRIES];
	
	float distance[NUM_ENTRIES];

	hardCodeValues(wordCnt, spellGr, label);

	for (int i = 0; i < NUM_ENTRIES; i++) 		{
		distance[i] = sqrt(pow(wordCnt[i] - eWords, 2) + pow(spellGr[i] - eSpell, 2));
	}

	int* kIndexes = findKNNIndex(distance);

	std::cout << "\tK Nearest Neighboors" << std::endl;

	for (int i = 0; i < k; i++) 		{
		std::cout << "(" << wordCnt[kIndexes[i]] << "," << spellGr[kIndexes[i]] << ")  "
			<< labelType(kIndexes[i], label) << std::endl;
	}

	delete[] kIndexes;
	std::cin.get();
	return 0;
}