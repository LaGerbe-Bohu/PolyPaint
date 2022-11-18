#pragma once


class Polygone {

	std::vector<float*> Points;
	
	public :

		Polygone() :Points(std::vector<float*>()) { }

		Polygone(std::vector<float*> P) : Points(P) { }

		Polygone(const Polygone& p) : Points(p.Points){ };


};