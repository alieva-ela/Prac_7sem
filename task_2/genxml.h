#ifndef GENXML_H
#define GENXML_H


using generatorParamType = std::unordered_map<std::string, std::string>;


// prototypes of functions for generator

void fillDataMap (char* fileName, generatorParamType& inputData);


void generateData (generatorParamType& dataVector, char* filename);


#endif 