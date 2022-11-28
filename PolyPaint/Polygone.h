#pragma once


class Polygone {

	std::vector<float*> Points;
	
	public :

		Polygone() :Points(std::vector<float*>()) { }

		Polygone(std::vector<float*> P) : Points(P) { }

		Polygone(const Polygone& p) : Points(p.Points){ };

		void push_back(float* b) {
			float* p = new float[2] {b[0], b[1]};
			Points.push_back(p);
		}
		
		std::vector<float*> getPoints() { return Polygone::Points; };
		

		void setPoints(std::vector<float*> p) {
			Points = std::vector<float*>(p);
		}

		std::vector<float> getFlatVector() {
			std::vector<float> t;

			for (int i = 0; i < Points.size(); i++) {
				t.push_back(Points[i][0]);
				t.push_back(Points[i][1]);
			}

			return t;
		}

		float* operator[](int a) {
			return Points[a];
		}



};

struct segment {
	float* A;
	float* B;
};

struct CriticalSegment {
	 float* Point;
	 segment Seg1;
	 segment Seg2;
};

class PolySeg : public Polygone {

	std::vector<CriticalSegment> Segments;

	public : 

	PolySeg() :Polygone(),Segments(std::vector<CriticalSegment>()){}

	PolySeg(Polygone p , std::vector<CriticalSegment> seg) : Polygone(p.getPoints()), Segments(seg) {}

	std::vector<CriticalSegment> GetSegments() { return this->Segments; }

	void InitializePolySeg( Polygone wind, Polygone Poly) {
		Segments = std::vector<CriticalSegment>();
		for (int i = 0; i < wind.getPoints().size(); i++) {
			bool b = isInside(wind[i], Poly.getFlatVector());
			
			if (b) {
				CriticalSegment seg;
				seg.Point = wind[i];
				int midx = i - 1;
				if (midx < 0) {
					midx = 0;
				}

				seg.Seg1.A = wind[i];
				seg.Seg1.B = wind[midx];
				
				int maxidx = (i + 1) % wind.getPoints().size();

				seg.Seg2.A = wind[i];
				seg.Seg2.B = wind[maxidx];
				

				Segments.push_back(seg);
				
			}
		}
	}



	CriticalSegment operator[](int a) {
		return Segments[a];
	}
	


};