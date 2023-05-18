#pragma once

enum type { Pol,Cur,Ent };

class Entity {

	std::vector<float*> Points;
	type _Type;
	public :
		

		Entity() :Points(std::vector<float*>()) { }

		Entity(std::vector<float*> P) : Points(P) { }

		Entity(const Entity& p) : Points(p.Points),_Type(p._Type) { };

		void push_back(float* b) {
			float* p = new float[2] {b[0], b[1]};
			Points.push_back(p);
		}
		
		std::vector<float*> getPoints() { return Entity::Points; };
		

		void setType(type t) {
			_Type = t;
		}

		type getType() {
			return _Type;
		}

		void setPoints(std::vector<float*> p) {
		
			Points.clear();
			for (int i = 0; i < p.size(); i++)
			{
				float* b = new float[2] {p[i][0], p[i][1]};
				Points.push_back(b);
			}

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

		~Entity() {
			Points.clear();
		}


};


class Polygone : public Entity {
	
	std::vector<float*> Points;
	public :
	Polygone():Points(std::vector<float*>()) {
		setType(Pol);
	}

	Polygone(std::vector<float*> P) : Points(P) {
		setType(Pol);
	};


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


class LCA {
	public:
	int ymax;
	float x;
	float dir[2];
	bool validNext = false;
	LCA* next;

	LCA() {
		ymax = NULL;
		x = NULL;
		dir[0] = 0;
		dir[1] = 0;
		validNext = false;
		next = nullptr;
	}

	LCA(const LCA& lca) {
		ymax = lca.ymax;
		x = lca.x;
		dir[0] = lca.dir[0];
		dir[1] = lca.dir[1];
		validNext = lca.validNext;
		next = lca.next;
	}


};


class Curve: public Polygone {

	std::vector<float*> Points;
	public:

	Curve() :Points(std::vector<float*>()) {
		setType(Cur);
	}

	Curve(std::vector<float*> P) : Points(P) {
		setType(Cur);
	};
};

