#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

#include "tinyxml.h" // additional library (XML Parser)
#include "genxml.h"



void fillDataMap (char* fileName, generatorParamType& generatorParams)
{
    std::ifstream file;
    file.open(fileName);
    if (not file.is_open())
    {
        std::cerr << "Can't open the file with name "
                  << fileName << std::endl;
        exit(1);
    }
    file >> generatorParams["NUMBER_OF_PROC"];
    file >> generatorParams["NUMBER_OF_JOBS"];
    file >> generatorParams["LEFT_BOUND"];
    file >> generatorParams["RIGHT_BOUND"];
    file.close();
}


void generateData (generatorParamType& generatorParams, char* fileName) 
{
    TiXmlDocument doc;
    TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
    doc.LinkEndChild( decl );

    TiXmlElement * root = new TiXmlElement( "Date_test" );  
    doc.LinkEndChild( root );

    TiXmlElement * proc = new TiXmlElement( "Processor" );  
    root->LinkEndChild( proc );  
    proc->SetAttribute("processsor",std::stoi(generatorParams["NUMBER_OF_PROC"])); 
    int numberOfJobs = std::stoi(generatorParams["NUMBER_OF_JOBS"]);
    TiXmlElement * jobs = new TiXmlElement( "Jobs" );
    jobs->SetAttribute("jobs",numberOfJobs);
    root->LinkEndChild( jobs );
    for (int i = 0; i < numberOfJobs; i++)
        {
            TiXmlElement * job = new TiXmlElement( "Job" );
            job->SetAttribute("job",i);
            int leftBound = std::stoi(generatorParams["LEFT_BOUND"]);
            int rightBound = std::stoi(generatorParams["RIGHT_BOUND"]);
            int delta = arc4random_uniform(rightBound - leftBound + 1); 
            int executionTime = leftBound + delta;
            job->SetAttribute("time", executionTime);
            root->LinkEndChild( job );
        }
    std::string nameXmlFile = fileName;
    nameXmlFile.replace(nameXmlFile.find(".txt"), 4, ".xml");
    doc.SaveFile( nameXmlFile.c_str());
    TiXmlDocument d(nameXmlFile.c_str());

}

int main (int argc, char** argv) 
{
    // generator parametrs

    generatorParamType generatorParams = { {"NUMBER_OF_PROC", "0"},
                                           {"NUMBER_OF_JOBS", "0"},
                                           {"LEFT_BOUND", "0"},
                                           {"RIGHT_BOUND", "0"} };
    fillDataMap (argv[1], generatorParams);  // read file with system
    generateData (generatorParams, argv[1]); // generate 
    return 0;
}