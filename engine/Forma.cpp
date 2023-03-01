#include "headers/Forma.h"

void Forma::adicionarPonto(Ponto p)
{
	this->pontos.push_back(p);
}

std::ostream& operator<<(std::ostream& stream, const Forma& forma)
{
	for (Ponto ponto : forma.pontos)
		stream << ponto.getX() << " " << ponto.getY() << " " << ponto.getZ() << "\n";
	return stream;
}

void Forma::escreveFicheiro (char *file)
{
	std::ofstream f(file);

	for (Ponto ponto : this->pontos)
		f << ponto.getX() << ", " << ponto.getY() << ", " << ponto.getZ() << "\n";

	f.close();
}

std::vector<Ponto> Forma::getPontos()
{
	return this->pontos;
}