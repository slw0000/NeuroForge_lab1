#pragma once

#include <string>
#include <vector>


void fileToConsole(std::string const& fileName);

std::vector<std::vector<float>> fileImport(std::string const& fileName);

/* TODO: после реализации класса matrix, сделать возможность открытия файла и записи содержимого (набора координат точек) сразу в нужном для дальнейшей работы формате */
auto fileImportMatrix();


