#include "DiamondSquareAlg.h"

Point::Point(vec3 pos)
{
	this->p = pos;
	left = pos;
	right = pos;
	up = pos;
	down = pos;
}
void Point::GetNormal() {
	vec3 tan1 = right - left;
	vec3 tan2 = down - up;
	n = normalize(cross(tan2, tan1));
}
Edge::Edge(Point* a, Point* b)
{
	this->a = a;
	this->b = b;
	if (b->p.x - a->p.x > b->p.z - a->p.z) {
		this->a->right = b->p;
		this->b->left = a->p;
	}
	else {
		this->a->down = b->p;
		this->b->up = a->p;
	}
}
Edge::~Edge() {

}
void Edge::GetMidPoint(float spread) {
	vec3 p = a->p + b->p;
	int n = 2;
	if (squares.size() > 1) {
		vector<Square*>::iterator i = squares.begin();
		for (i; i < squares.end(); i++) {
			p += (*i)->mid->p;
		}
		n += 2;
	}
	p = p / float(n) + offset(spread);
	mid = new Point(p);
}
void Edge::GenerateNewEdges() {
	ne0 = new Edge(a, mid);
	ne1 = new Edge(mid, b);
}

Square::Square(Edge* e0, Edge* e1, Edge* e2, Edge* e3) {
	this->e0 = e0;
	this->e1 = e1;
	this->e2 = e2;
	this->e3 = e3;
	e0->squares.push_back(this);
	e1->squares.push_back(this);
	e2->squares.push_back(this);
	e3->squares.push_back(this);
}
Square::~Square() {

}
void Square::GetMidPoint(float spread) {
	vec3 p = (e0->a->p + e0->b->p + e2->a->p + e2->b->p) / 4.f
		+ offset(spread);
	mid = new Point(p);
}
void Square::GenerateNewSquares() {
	ne0 = new Edge(e0->mid, mid);
	ne1 = new Edge(mid, e1->mid);
	ne2 = new Edge(mid, e2->mid);
	ne3 = new Edge(e3->mid, mid);

	nf0 = new Square(e0->ne0, ne0, ne3, e3->ne0);
	nf1 = new Square(e0->ne1, e1->ne0, ne1, ne0);
	nf2 = new Square(ne1, e1->ne1, e2->ne1, ne2);
	nf3 = new Square(ne3, ne2, e2->ne0, e3->ne1);
}

DiamondSquareAlg::DiamondSquareAlg(float w, float spread) {
	init(w, spread);
}
void DiamondSquareAlg::init(float w, float spread) {
	float width = w / 2.f;
	this->spread = spread;
	//4 initial points
	vec3 v0(-width, random11(), -width), v1(width, random11(), -width),
		v2(-width, random11(), width), v3(width, random11(), width);
	Point* a = new Point(v0),
		* b = new Point(v1),
		* c = new Point(v2),
		* d = new Point(v3);
	points.push_back(a); points.push_back(b);
	points.push_back(c); points.push_back(d);
	//4 initial edges
	Edge* e0 = new Edge(a, b), * e1 = new Edge(b, d),
		* e2 = new Edge(c, d), * e3 = new Edge(a, c);
	edges.push_back(e0); edges.push_back(e1);
	edges.push_back(e2); edges.push_back(e3);
	//1 initial face
	Square* s = new Square(e0, e1, e2, e3);
	squares.push_back(s);
}

DiamondSquareAlg* MidpointDisplacement(DiamondSquareAlg* origin, int times) {
	DiamondSquareAlg* res, * diamondSquareAlg;
	diamondSquareAlg = new DiamondSquareAlg();
	//original points
	for (int i = 0; i < times; i++) {
		diamondSquareAlg = new DiamondSquareAlg();
		vector<Point*>::iterator itp = origin->points.begin();
		for (itp; itp < origin->points.end(); itp++) {
			diamondSquareAlg->points.push_back(*itp);
		}
		//get new mid points
		vector<Square*>::iterator its = origin->squares.begin();
		for (its; its < origin->squares.end(); its++) {
			(*its)->GetMidPoint(origin->spread);
			diamondSquareAlg->points.push_back((*its)->mid);
		}
		//edges split into two
		vector<Edge*>::iterator ite = origin->edges.begin();
		for (ite; ite < origin->edges.end(); ite++) {
			(*ite)->GetMidPoint(origin->spread);
			diamondSquareAlg->points.push_back((*ite)->mid);

			(*ite)->GenerateNewEdges();
			diamondSquareAlg->edges.push_back((*ite)->ne0);
			diamondSquareAlg->edges.push_back((*ite)->ne1);
		}
		//1 face generate 4 new faces and edges
		for (its = origin->squares.begin(); its < origin->squares.end(); its++) {

			(*its)->GenerateNewSquares();

			diamondSquareAlg->edges.push_back((*its)->ne0);
			diamondSquareAlg->edges.push_back((*its)->ne1);
			diamondSquareAlg->edges.push_back((*its)->ne2);
			diamondSquareAlg->edges.push_back((*its)->ne3);

			diamondSquareAlg->squares.push_back((*its)->nf0);
			diamondSquareAlg->squares.push_back((*its)->nf1);
			diamondSquareAlg->squares.push_back((*its)->nf2);
			diamondSquareAlg->squares.push_back((*its)->nf3);
		}

		diamondSquareAlg->spread = origin->spread * 0.5f;
		origin = diamondSquareAlg;
	}
	res = diamondSquareAlg;
	return res;
}

void DiamondSquareAlg::CalculateNormal() {
	vector<Point*>::iterator itp = points.begin();
	for (itp; itp < points.end(); itp++) {
		(*itp)->GetNormal();
	}
}
DiamondSquareAlg::DiamondSquareAlg()
{
}


DiamondSquareAlg::~DiamondSquareAlg()
{
	vector<Point*>::iterator itp = points.begin();
	for (itp; itp < points.end(); itp++) {
		delete (*itp);
	}
	points.clear();
	vector<Edge*>::iterator ite = edges.begin();
	for (ite; ite < edges.end(); ite++) {
		delete (*ite);
	}
	edges.clear();
	vector<Square*>::iterator its = squares.begin();
	for (its; its < squares.end(); its++) {
		delete (*its);
	}
	squares.clear();
	v.clear();
}

inline vec3 offset(float spread) {
	return random11() * spread * vec3(0, 1, 0);
}