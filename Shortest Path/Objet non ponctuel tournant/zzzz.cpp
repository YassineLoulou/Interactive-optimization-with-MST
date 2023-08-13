#include <bits/stdc++.h>
#include <fstream>
#include <cstdlib>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define inf 1e9
#define e 1.19209e-06
#define PI float(acos(-1))
#define MX 100
#define rot 1e-9

using namespace std;


class sommet {
public:
	float x, y;
	sommet() {}
	sommet(float X, float Y) {
		x = X;
		y = Y;
	}
	bool operator<(const sommet& other) const {
		if (x < other.x) {
			return true;
		}
		else if (x == other.x) {
			return y < other.y;
		}
		else {
			return false;
		}
	}
	bool operator ==(sommet S) {
		return abs(x - S.x) < e && abs(y - S.y) < e;
	}
	bool operator !=(sommet S) {
		return !((*this) == S);
	}

	sommet operator*(const float& t) {
		return sommet(x * t, y * t);
	}
	sommet operator+(sommet S) {
		return sommet(x + S.x, y + S.y);
	}
	sommet operator-(sommet S) {
		return sommet(x - S.x, y - S.y);
	}
};
class space_sommet {
public:
	float x, y, z;
	space_sommet() {}
	space_sommet(float X, float Y, float Z) :x(X), y(Y), z(Z) {}
	space_sommet(sommet s, float angle) :x(s.x), y(s.y), z(angle) {}
	bool operator < (const space_sommet& other) const {
		if (x != other.x)
			return x < other.x;
		if (y != other.y)
			return y < other.y;
		return z < other.z;
	}
	bool operator ==(space_sommet S) {
		return (abs(x - S.x) < e) && (abs(y - S.y) < e);
	}
	bool equals(space_sommet S) {
		 return (abs(x - S.x) < e) && (abs(y - S.y) < e) && (z == S.z);
	}
	bool operator !=(space_sommet S) {
		return !(*this == S);
	}
	space_sommet operator*(const float& t) {
		return space_sommet(x * t, y * t, z * t);
	}
	space_sommet operator+(space_sommet S) {
		return space_sommet(x + S.x, y + S.y, z + S.z);
	}
	space_sommet operator-(space_sommet S) {
		return space_sommet(x - S.x, y - S.y, z - S.z);
	}

};
class space_arc {
public:
	space_sommet S1;
	space_sommet S2;
	float len;
public:
	space_arc(space_sommet A, space_sommet B, float d) :S1(A), S2(B), len(d) {}

};
class space_graphe {
public:
	vector < space_arc > edges;
	set < space_sommet> nodes;
};

float dist(sommet A, sommet B) {
	float x1 = A.x, y1 = A.y, x2 = B.x, y2 = B.y;
	return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

class segment {
public:
	sommet S1;
	sommet S2;
	float norm;
public:
	segment() {}
	segment(sommet A, sommet B) :S1(A), S2(B) {}
	segment(sommet A, sommet B, float d) :S1(A), S2(B), norm(d) {}
	bool operator ==(segment S) {
		return S1 == S.S1 && S2 == S.S2;
	}
	float a() {
		if (S2.x != S1.x)
			return (S2.y - S1.y) / (S2.x - S1.x);
		return float(inf);
	}
	float b() {
		if (S2.x != S1.x)
			return S1.y - (*this).a() * S1.x;
		return S1.x;
	}
	bool in_str_seg(sommet S) {
		if ((*this).a() == float(inf))
			return e < max(S1.y, S2.y) - S.y - S.y && S.y - min(S1.y, S2.y)>e;
		return S.x - min(S1.x, S2.x) > e && e < max(S1.x, S2.x) - S.x;
	}
	bool in_seg(sommet S) {
		if ((*this).a() == float(inf))
			return S.y >= min(S1.y, S2.y) && S.y <= max(S1.y, S2.y);
		if (S.x >= min(S1.x, S2.x) && S.x <= max(S1.x, S2.x))
			return true;
		return false;
	}
};

pair<bool, sommet> inter(segment p, segment q, int f) {
	if (abs(p.a() - q.a()) < e) {
		return { false,p.S1 };
	}
	float x, y;
	if (q.a() == float(inf)) {
		x = q.S1.x;
		y = p.a() * x + p.b();
	}
	else if (p.a() == float(inf)) {
		x = p.S1.x;
		y = q.a() * x + q.b();
	}
	else {
		x = (q.b() - p.b()) / (p.a() - q.a());
		y = p.a() * x + p.b();
	}
	sommet S(x, y);
	segment ss = segment(sommet(1, -0.5), sommet(6, -0.5));

	if (f == 0) {

		if (q.in_str_seg(S) && p.in_str_seg(S)) {

			return { true,S };
		}
		else
			return { false, S };

	}
	else {

		if (q.in_seg(S) && p.in_seg(S))
			return { true,S };

		else
			return { false, S };
	}
}
bool out(sommet edge, segment S) {

	if ((S.a() == inf && S.norm * (edge.x - S.b()) < 0) || (S.a() != inf && S.norm * (edge.y - (edge.x * S.a() + S.b())) < 0))
		return false;
	return true;
}
class obstacle {
public:
	vector < segment > obs;
public:

	bool inside(sommet S) {
		for (auto seg : obs)
			if (out(S, seg))
				return false;
		return true;
	}
	bool intersect(segment p) {
		segment ss = segment(sommet(1, -0.5), sommet(6, -0.5));

		sommet p1 = p.S1, p2 = p.S2;
		if ((*this).inside(p1) || (*this).inside(p2)) {
			return true;
		}
		set<sommet>int_pts;
		for (auto q : obs) {
			if (inter(p, q, 0).first) {
				int_pts.insert(inter(p, q, 0).second);
				if (int_pts.size() == 1 && ss == p) {
					cout << q.S1.x << " " << q.S1.y << " " << q.S2.x << " " << q.S2.y << endl;
				}
			}
		}
		if (int_pts.size() > 0) {

			return true;
		}
		int_pts.clear();
		for (auto q : obs) {
			if (inter(p, q, 1).first) {
				int_pts.insert(inter(p, q, 1).second);
			}
		}
		int sz = obs.size();
		for (auto vertex : int_pts) {

			segment seg1, seg2;
			for (int i = 0; i < sz; i++) {
				if (obs[i].S1 == vertex) {
					seg1 = obs[i];
					seg2 = obs[((i - 1) + sz) % sz];
					break;
				}
			}
			sommet edge;
			float t = 0.01;
			if (p.S1 != vertex) {
				edge = p.S1 * t + vertex * (1 - t);
				if (((seg1.a() == inf && seg1.norm * (edge.x - seg1.b()) < -e) || (seg1.a() != inf && seg1.norm * (edge.y - (edge.x * seg1.a() + seg1.b())) < -e))
					&& ((seg2.a() == inf && seg2.norm * (edge.x - seg2.b()) < -e) || (seg2.a() != inf && seg2.norm * (edge.y - (edge.x * seg2.a() + seg2.b())) < -e))) {
					if (ss == p)
						cout << "hi1" << endl;
					return true;
				}
			}
			if (p.S2 != vertex) {
				edge = p.S2 * t + vertex * (1 - t);
				if (((seg1.a() == inf && seg1.norm * (edge.x - seg1.b()) < -e) || (seg1.a() != inf && seg1.norm * (edge.y - (edge.x * seg1.a() + seg1.b())) < -e))
					&& ((seg2.a() == inf && seg2.norm * (edge.x - seg2.b()) < -e) || (seg2.a() != inf && seg2.norm * (edge.y - (edge.x * seg2.a() + seg2.b())) < -e))) {
					if (ss == p) {
						cout << "hi2" << endl;
						cout << seg1.norm * (edge.y - (edge.x * seg1.a() + seg1.b())) << endl;
						cout << seg1.norm << " " << seg2.norm << endl;
						cout << edge.x << " " << edge.y << "          " << seg1.S1.x << " " << seg1.S1.y << " " << seg1.S2.x << " " << seg1.S2.y << endl;
					}
					return true;
				}
			}

		}
		return false;
	}
};

bool inter_obs(vector<obstacle>obs, segment p) {
	for (auto ob : obs) {
		if (ob.intersect(p))
			return true;
	}
	return false;
}

class arc {
public:
	sommet S1;
	sommet S2;
	float len;
public:
	arc(sommet A, sommet B, float d) :S1(A), S2(B), len(d) {}
};
class graphe {
public:
	vector < arc > edges;
	set < sommet> nodes;
};


sommet fix(obstacle shape) {
	sommet A = sommet(0, 0);
	for (auto s : shape.obs) {
		A = A + s.S1;
	}
	A = A * (1.0 / shape.obs.size());
	return A;
}
float func(sommet S) {
	if (abs(S.y) < e) {
		if (S.x > 0)
			return 1.0f;
		return -1.0f;
	}
	if (S.y > 0)
		return 1.0f;
	else if (S.y < 0)
		return -1.0f;
}
void move(obstacle& object, sommet u) {
	for (int i = 0; i < object.obs.size(); i++) {
		object.obs[i].S1 = object.obs[i].S1 + u;
		object.obs[i].S2 = object.obs[i].S2 + u;
	}
}



sommet center(obstacle ob) {
	sommet A(0, 0);
	float sz = float(ob.obs.size());
	for (int i = 0; i < ob.obs.size(); i++) {
		A = A + ob.obs[i].S1;
	}
	A = A * (1.0f / sz);
	return A;
}
float theta(segment A, segment B) {
	float a1 = A.S2.x - A.S1.x;
	float a2 = A.S2.y - A.S1.y;
	float b1 = B.S2.x - B.S1.x;
	float b2 = B.S2.y - B.S1.y;

	float dot = a1 * b1 + a2 * b2;
	float mag_a = sqrt(a1 * a1 + a2 * a2);
	float mag_b = sqrt(b1 * b1 + b2 * b2);

	//float theta = acos(dot / (mag_a * mag_b)) * 180 / PI;
	float cross_product = a1 * b2 - a2 * b1;
	float theta = atan2(cross_product, dot) * 180 / PI;
	if (theta < 0)
		theta += 360.0f;
	return theta;
}
bool obs_merge(obstacle a, obstacle b) {
	for (auto i : a.obs) {
		if (a.intersect(i))
			return true;
	}
	return false;
}
void rotate(sommet& S, sommet C, float angle) {
	float s = sin(angle);
	float c = cos(angle);

	S.x -= C.x;
	S.y -= C.y;

	float xnew = S.x * c - S.y * s;
	float ynew = S.x * s + S.y * c;

	S.x = xnew + C.x;
	S.y = ynew + C.y;
}
void rotate_obs(obstacle& ob, float angle) {
	sommet centre = center(ob);
	for (int i = 0; i < ob.obs.size(); i++) {
		rotate(ob.obs[i].S1, centre, angle);
		rotate(ob.obs[i].S2, centre, angle);
	}
}

bool belongs(sommet S, vector<obstacle>ob) {
	for (auto obst : ob) {
		bool in = true;
		for (auto seg : obst.obs) {
			in &= ((seg.a() == inf && seg.norm * (S.x - seg.b()) < -e) || (seg.a() != inf && seg.norm * (S.y - (S.x * seg.a() + seg.b())) < -e));
		}
		if (in)
			return false;
	}
	return true;
}


vector<space_sommet> dijkstra(space_graphe graph, space_sommet source, space_sommet destination) {

	map<space_sommet, set<space_sommet>>suc;
	map<pair<space_sommet, space_sommet>, float>dis;

	for (auto i : graph.nodes) {
		for (auto j : graph.edges) {
			if (j.S1.equals(i)) {
				suc[i].insert(j.S2);
				dis[{i, j.S2}] = j.len;

			}
			else if (j.S2.equals(i)) {
				suc[i].insert(j.S1);
				dis[{i, j.S1}] = j.len;
			}
		}
	}
	map<space_sommet, float> l;
	for (auto i : graph.nodes) {
		l[i] = float(inf);
	}
	l[source] = 0;
	map<space_sommet, space_sommet>p;
	for (auto i : graph.nodes) {
		p[i] = i;
	}

	for (auto i : graph.edges) {
		if (i.S1.equals(source)) {
			l[i.S2] == i.len;
			p[i.S2] = source;
		}
		else if (i.S2.equals(source)) {
			l[i.S1] = i.len;
			p[i.S1] = source;
		}
	}

	//for (auto i : l) {cout << i.first.x << " " << i.first.y << " " << i.second << endl;}
	//cout << endl << endl << endl;

	set<space_sommet> S, T;
	S.insert(source);
	T = graph.nodes;
	T.erase(source);
	while (!T.empty()) {
		space_sommet P;
		float c = float(inf);
		for (auto som : T) {
			if (l[som] <= c) {
				P = som;
				c = l[som];
			}
		}
		S.insert(P);
		T.erase(P);
		for (auto i : suc[P]) {
			if (e < l[i] - l[P] - dis[{i, P}]) {
				l[i] = dis[{i, P}] + l[P];
				p[i] = P;
			}
		}
	}
	vector<space_sommet>chemin;
	chemin.push_back(destination);
	space_sommet P = destination;
	while (!(P.equals(source) )) {
		chemin.push_back(p[P]);
		P = p[P];
	}
	cout << endl;
	return chemin;
}


int main() {
	


	// define our system

	vector < sommet > points, new_points;
	vector < obstacle > ob, actual_ob, new_ob;
	space_graphe g;
	sommet source, destination;
	obstacle src, dest;

	// build points 
	// build obstacles in ob
	// from file

	fstream f;
	f.open("shape.txt", ios::in);
	if (f.is_open()) {

		string s;
		int nbr_obs = 2;
		int c = 1;
		obstacle o;
		vector < sommet > obs_pts;
		vector<float>norms;

		for (int k = 0; k < nbr_obs + 2; k++) {

			bool b = false;

			// push in obs_pts

			while (getline(f, s)) {
				//cout << s << endl;
				int i = 0, cnt = 0;
				float x, y;
				for (int j = 0; j < s.size(); j++) {
					if (s[j] == ',') {
						if (cnt == 0) {
							x = stof(s.substr(i, j - i));
							i = j + 1;
						}
						else if (cnt == 1) {
							y = stof(s.substr(i, j - i));
						}
						else {
							norms.push_back(stof(s.substr(j - 1, 1)));
						}
						cnt++;
					}
					if (s[j] == ';') {
						b = true;
					}
				}
				sommet pt(x, y);
				obs_pts.push_back(pt);
				if (b)
					break;
			}

			// form obstacle with obs_pts

			int obs_sz = obs_pts.size();
			for (int i = 0; i < obs_sz; i++) {
				sommet A = obs_pts[i];
				sommet B = obs_pts[(i + 1) % obs_sz];
				segment seg = segment(A, B, (norms[i] > 0 ? 1 : -1));
				if (c == 1) {
					src.obs.push_back(seg);
				}
				else if (c == nbr_obs + 2) {
					dest.obs.push_back(seg);
				}
				else {
					o.obs.push_back(seg);
				}
			}

			// if obstacle , then add to set of obstacles

			if (c != 1 && c != (nbr_obs + 2)) {
				ob.push_back(o);
			}

			// clear current obstacle

			obs_pts.clear();
			o.obs.clear();
			norms.clear();
			c++;
		}
		f.close();
	}
	else cout << "no";

	// output system
	// for (auto i : p.obs) {cout << "( " << i.S1.x << " , " << i.S1.y << " ) " << "( " << i.S2.x << " , " << i.S2.y << " ) " << endl;}

	// ADAPT TO SHAPE

	source = fix(src);
	destination = fix(dest);
	points.push_back(source);
	points.push_back(destination);
	int shape_sz = src.obs.size();
	obstacle object = src;
	vector<set<sommet>>my_set;
	
	vector<vector<obstacle>>space_ob;
	for (auto i : object.obs) {
		cout << i.S1.x << " "<<i.S1.y<<" "<<i.S2.x<<" " << i.S2.y << endl;
	}
	cout << endl;
	for (float angle = 0; angle < MX; angle+=1.0) {
		object = src;
		//cout << angle << endl;
		rotate_obs(object, angle*PI/180);
		int p = 0;
		for (auto obst : ob) {
		int k = 0, ind = -1;
		obstacle updated_obst;
		segment current_seg = obst.obs[k];
		sommet mid = (current_seg.S1 + current_seg.S2) * 0.5f;

		// create contact point
		
		sommet contact;
		bool colinear = false;
		for (int i = 0; i < shape_sz; i++) {
			if (object.obs[i].a() == current_seg.a()) {
				contact = object.obs[i].S1;
				colinear = true;
				ind = i;
				break;
			}
		}
		if (!colinear) {

			for (int i = 0; i < shape_sz; i++) {
				move(object, mid - object.obs[i].S1);
				float t = 0.001;
				sommet A = object.obs[i].S2 * t + mid * (1 - t);
				sommet B = object.obs[(i - 1 + shape_sz) % shape_sz].S1 * t + mid * (1 - t);

				if (out(A, current_seg) && out(B, current_seg)) {
					contact = object.obs[i].S1;
					ind = i;
					break;
				}
			}
		}
		//cout << contact.x << " " << contact.y << endl;
		// contact created!

		//-------------------------------------------------------------------------------------------------------------------------------------------------

		// object.obs[ind] current segment of object
		// obst.obs[k] current segment of obstacle
		// draw new obstacle


		sommet u, v;
		int obst_sz = obst.obs.size();
		int object_sz = object.obs.size();

		u = obst.obs[k].S1 - contact;
		move(object, u);
		points.push_back(center(object));
		contact = obst.obs[k].S1;
		float normale;
		while (k < obst_sz) {

			u = obst.obs[k].S2 - obst.obs[k].S1;
			v = u;
			rotate(v, sommet(0.0, 0.0), -PI / 2);
			normale = func(v);
			updated_obst.obs.push_back(segment(center(object), center(object) + u, normale));
			move(object, u);
			points.push_back(center(object));
			contact = obst.obs[k].S2;


			// angle between current object segment and next obstacle segment meeting at contact, should be  0<=x<=180

			while (theta(object.obs[ind % object_sz], obst.obs[(k + 1 + obst_sz) % obst_sz]) > 180) {
				u = object.obs[ind % object_sz].S1 - object.obs[ind % object_sz].S2;
				v = u;
				rotate(v, sommet(0, 0), -PI / 2.0);
				normale = func(v);
				updated_obst.obs.push_back(segment(center(object), center(object) + u, normale));
				move(object, u);
				points.push_back(center(object));
				ind++;

			}
			k++;
		}
		actual_ob.push_back(updated_obst);
		p++;
	}


		for (auto i : points)
			g.nodes.insert(space_sommet(i,angle));

		set<sommet>pts;
		int n = points.size();
		for (auto i : points)
			pts.insert(i);
		pts.insert(center(dest));
		pts.insert(center(src));
		my_set.push_back(pts);
		for (auto i : pts) {
			for (auto j : pts) {
				segment s(i, j);
				if (!inter_obs(actual_ob, s) && i != j) {
					space_sommet u(i, angle), v(j, angle);
					g.edges.push_back(space_arc(u, v, dist(i, j)));
				}
			}
		}
		space_ob.push_back(actual_ob);
		points.clear();
		actual_ob.clear();
	}
		
	for (int i = 0; i < MX; i++) {
		for (auto point:my_set[i]) {
				if ((i<MX-1)&&belongs(point, space_ob[i+1])) {
					space_sommet p1(point, i), p2(point, i+1);
					g.nodes.insert(p1);
					g.nodes.insert(p2);
					g.edges.push_back(space_arc(p1, p2, rot));
				}
				if ((i>0)&&belongs(point, space_ob[i-1])) {
					space_sommet p1(point, i), p2(point, i-1);
					g.nodes.insert(p1);
					g.nodes.insert(p2);
					g.edges.push_back(space_arc(p1, p2, rot));
				}
		}
	}
	


	// fix obstacle intersections
	/*
	int nbr_obs = actual_ob.size();
	bool merge = false;
	do {
		for (int i = 0; i < nbr_obs; i++) {
			bool this_merges = false;
			for (int j = i + 1; j < nbr_obs; j++) {
				if (obs_merge(actual_ob[i], actual_ob[j])) {
					merge = true;
					this_merges = true;
					vector<sommet>z;
					for (auto k : actual_ob[i].obs)
						z.push_back(k.S1);
					vector <sommet>v = convex_hull(z);
					obstacle new_obs;
					for (int p = 0; p < v.size(); p++) {
						new_obs.obs.push_back(segment(v[p], v[(p + 1) % v.size()]));
					}
					new_ob.push_back(new_obs);
				}
			}
			if (this_merges == false) {
				new_ob.push_back(actual_ob[i]);
			}
		}
		actual_ob = new_ob;
	} while (merge == true);

	new_points.push_back(source);
	new_points.push_back(destination);
	*/

	
	//for (auto i : g.edges) { cout << "( " << i.S1.x << " , " << i.S1.y << " ) " << "( " << i.S2.x << " , " << i.S2.y << " ) " << endl; }

	//find shortest path

	vector <space_sommet > res;
	space_sommet space_source(source, 0), space_destination(destination, 0);
	res = dijkstra(g, space_source, space_destination);


	//output result
    cout << "SHORTEST PATH FROM ( " << source.x << " , " << source.y << " ) TO ( " << destination.x << " , " << destination.y << " ) " << endl << endl; 
	cout << res.size() << endl;
	reverse(res.begin(), res.end());
	for (auto i : res)
		cout << i.x << " " << i.y << " " << i.z << endl;
//	for (int i = 0; i < res.size(); i++) { cout << "( " << res[i].x << " , " << res[i].y << " , " << res[i].z << " ) " << (i < res.size() - 1 ? " -> " : "\n\n"); }


	return 0;
}

