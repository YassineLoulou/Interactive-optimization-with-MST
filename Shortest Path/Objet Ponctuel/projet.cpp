#include <bits/stdc++.h>
#include <fstream>
#include <cstdlib>



#define eps 1.19209e-06
#define inf 1e9

using namespace std;


class sommet {
	public:
		float x, y;
		sommet() {}
		sommet(float X, float Y) {
			x = X;
			y = Y;
		}
		//comparaison
		bool operator<(const sommet &other) const {
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
			return x == S.x && y == S.y;
		}
		bool operator !=(sommet S) {
			return x != S.x || y != S.y;
		}
		//dilatation
		sommet operator*(const float& t) {
			return sommet(x * t, y * t);
		}
		//translation
		sommet operator+(sommet S) {
			return sommet(x + S.x, y + S.y);
		}
		sommet operator-(sommet S) {
			return sommet(x - S.x, y - S.y);
		}
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

		//constructeurs
		segment() {}
		segment(sommet A, sommet B) :S1(A), S2(B){}
		segment(sommet A, sommet B, float d) :S1(A), S2(B), norm(d) {}


		bool operator ==(segment A) {
			return A.S1 == S1 && A.S2 == S2;
		}
		//paramètres de la droite qui porte le segment 
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

		// point stricly inside segment


		bool in_str_seg(sommet S) {
			if ((*this).a() == float(inf))
				return S.y -min(S1.y, S2.y)>eps && eps < max(S1.y, S2.y)-S.y;
			return S.x -min(S1.x, S2.x) >eps && eps< max(S1.x, S2.x)-S.x;
		}

		// point inside segment

		bool in_seg(sommet S) {
			if ((*this).a() == float(inf))
				return S.y >= min(S1.y, S2.y) && S.y <= max(S1.y, S2.y);
			if (S.x >= min(S1.x, S2.x) && S.x <= max(S1.x, S2.x))
				return true;
			return false;
		}
};

pair<bool,sommet> inter(segment p, segment q, int f) {
	if (abs(p.a() - q.a() )< eps) {
		return { false,p.S1 };
	}
	float x, y;

	if (q.a() == float(inf)) {
		x = q.S1.x;
		y= p.a() * x + p.b();
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

	segment ss(sommet(5.5, 2), sommet(4.75, 2.5));
	if (f == 0) {
	
		if (q.in_str_seg(S) && p.in_str_seg(S)) {
			return { true,S };
		}
		else
			return { false, S };

	}
	else {
		if ( q.in_seg(S) && p.in_seg(S) )
			return { true,S };

		else
			return { false, S };
	}
}

class obstacle {
	public:
		vector < segment > obs;
	public:
		bool intersect(segment p) {
			set<sommet>int_pts;
			segment ss(sommet(5.5, 2), sommet(4.75, 2.5));
			
			for (auto q : obs) {
				if (inter(p,q,0).first) {
					int_pts.insert(inter(p, q,0).second);
					if (p == ss)
						cout << q.S1.x << " " << q.S1.y << " " << q.S2.x << " " << q.S2.y << endl;
				}
			}
			if (int_pts.size()>0) {
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
					if ( abs(obs[i].S1.x-vertex.x)<eps  && abs(obs[i].S1.y-vertex.y)<eps) {
						seg1 = obs[i];
						seg2 = obs[((i - 1) + sz) % sz];
						i = sz;
					}
				}
				
				sommet edge;
				float t = 0.01;
				if (p.S1 != vertex) {
					edge = p.S1 * t + vertex * (1 - t);
					if (((seg1.a() == inf && seg1.norm * (edge.x - seg1.b()) < -eps) || (seg1.a()!=inf&&seg1.norm * (edge.y - (edge.x * seg1.a() + seg1.b())) < -eps ))
						&& ((seg2.a() == inf && seg2.norm * (edge.x - seg2.b()) < -eps) || (seg2.a()!=inf&&seg2.norm*( edge.y - (edge.x * seg2.a() + seg2.b())) < -eps ))) {
						return true;
					}
				}
				if (p.S2 != vertex) {
					edge = p.S2 * t + vertex * (1 - t);
					if (((seg1.a() == inf && seg1.norm * (edge.x - seg1.b()) < -eps) || (seg1.a() != inf && seg1.norm * (edge.y - (edge.x * seg1.a() + seg1.b())) < -eps))
						&& ((seg2.a() == inf && seg2.norm * (edge.x - seg2.b()) < -eps) || (seg2.a() != inf && seg2.norm * (edge.y - (edge.x * seg2.a() + seg2.b())) < -eps))) {
						return true;
					}
				}

			}

			return false;
		}
};

bool inter_obs(vector<obstacle>obs,segment p) {
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
		// arc : lien entre 2 sommets
	public:
		arc(sommet A, sommet B, float d):S1(A),S2(B),len(d){}
};

class graphe {
	public:
		vector < arc > edges;
		set < sommet> nodes; 
		//vertices vertex (node)
};


vector<sommet> dijkstra(graphe graph,sommet source, sommet destination) {

	map<sommet, set<sommet>>suc;
	map<pair<sommet, sommet>, float>dis;

	for (auto i : graph.nodes) {
		for (auto j : graph.edges) {
			if (j.S1 == i) {
				suc[i].insert(j.S2);
				dis[{i, j.S2}] = j.len;
				
			}
			else if (j.S2 == i) {
				suc[i].insert(j.S1);
				dis[{i, j.S1}] = j.len;
			}
		}
	}
	map<sommet, float> l;
	for (auto i : graph.nodes) {
		l[i] = inf;
	}
	map<sommet, sommet>p;
	for (auto i : graph.nodes) {
		p[i] = i;
	}

	for (auto i : graph.edges) {
		if (i.S1 == source) {
			l[i.S2] == i.len;
			p[i.S2] = source;
		}
		else if (i.S2 == source) {
			l[i.S1] = i.len;
			p[i.S1] = source;
		}
	}
	

	set<sommet> S, T;
	S.insert(source);
	T = graph.nodes;
	T.erase(source);

	//cout << endl << endl;
	//cout << p[sommet(1, 0)].x << " " << p[sommet(1, 0)].y << endl;

	while (!T.empty()) {
		sommet P;
		int c = inf;
		for (auto som : T) {
			if (l[som] <= c) {
				P = som;
				c = l[som];
			}
		}
		S.insert(P);
		T.erase(P);
		for (auto i : suc[P]) {
			if (dis[{i, P}] + l[P] < l[i]) {
				l[i] = dis[{i, P}] + l[P];
				p[i] = P;
			}
		}
		//cout << P.x << " " << P.y << endl;
	}
	vector<sommet>chemin;
	chemin.push_back(destination);
	sommet P = destination;
	int i = 0;
	while (!(P == source)) {
		chemin.push_back(p[P]);
		P = p[P];
	}

	return chemin;
}



int main() {
	segment u, v;
	u.S1 = sommet(0, 1);
	u.S2 = sommet(4.5, 5);
	v.S1 = sommet(2, 1);
	v.S2 = sommet(2, 5);
	//cout << inter(u, v, 0).first << endl;

	//cout << endl << endl;
	//define our system

	vector < sommet > points;
	vector < obstacle > ob;
	graphe g;
	sommet source, destination;

	//build points vector
	//build obstacles in ob
	
	
	// SYSTEM : 2 POINTS DEPART ET DESTINATION + OBSTACLES
	// POINTS , LOOP OVER  POINTS, FORM SEGMENT, CHECK IF SEGMENT INTERSECTS WITH OBSTACLES, IF YES ABANDON, IF NO ADD IN GRAPH
	fstream f;
	f.open("Points.txt", ios::in);
	if (f.is_open()) {
		string s;
		int nbr_obs = 3;
		int c = 1;
		obstacle o;
		vector < sommet > obs_pts;
		vector<float>norms;
		for (int k = 0; k < nbr_obs+2; k++) {
			bool b = false;
			//push in obs_pts
			while (getline(f, s)) {
				//cout << s << endl;
				int i = 0, cnt = 0;
				float x, y;
				for (int j = 0; j < s.size(); j++) {
					if (s[j] == ',') {
						if (cnt==0) {
							x = stof(s.substr(i, j - i));
							i = j + 1;
						}
						else if(cnt==1){
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
				points.push_back(pt);
				if (c == 1) {
					source = pt;
				}
				else if (c == nbr_obs+2) {
					destination = pt;
				}
				else {
					obs_pts.push_back(pt);
				}
				if (b)
					break;
			}
			c++;
			//form obstacle with obs_pts
			int obs_sz = obs_pts.size();
			for (int i = 0; i < obs_sz; i++) {
				sommet A = obs_pts[i], B = obs_pts[(i + 1) % obs_sz];
				//cout << norms[i] << endl;
				segment seg = segment(A, B, (norms[i]>0?1:-1));
				//cout << A.x << " " << A.y << " " << B.x << " " << B.y << " " << seg.norm << endl;
				o.obs.push_back(seg);
			}
			//if obstacle then add to set of obstacles
			if (obs_pts.size()) {
				ob.push_back(o);
			}
			//clear current obstacle
			obs_pts.clear();
			o.obs.clear();
			norms.clear();
		}
		f.close();
	}
	else cout << "no";

	
	//output system
	//for (auto i : ob[1].obs) { cout << "( " << i.S1.x << " , " << i.S1.y << " ) " << "( " << i.S2.x << " , " << i.S2.y << " ) " << endl; }


	
	obstacle p = ob[0];
	

	//build graph
	
	int n = points.size();
	for (int i = 0; i < n; i++) {
		g.nodes.insert(points[i]);
		for (int j = 0; j < n; j++) {
			if (i == j)continue;
			segment s(points[i], points[j]);
			if (!inter_obs(ob, s)) {
				g.edges.push_back(arc(s.S1, s.S2, dist(s.S1, s.S2)));
			}
		}
	}
	segment ss(sommet(5.5, 2), sommet(4.75,2.5));
	//cout << inter_obs(ob, ss) << endl;

	
	//for (auto i : g.edges) { cout << "( " << i.S1.x << " , " << i.S1.y << " ) " << "( " << i.S2.x << " , " << i.S2.y << " ) " << endl; }
	
	//find shortest path
	
	vector <sommet > res;
	res=dijkstra(g,source,destination);

	

	//output result
	cout << "SHORTEST PATH FROM ( " << source.x << " , " << source.y << " ) TO ( " << destination.x << " , " << destination.y << " ) " << endl << endl;
	cout << res.size() << endl << endl;
	for (int i = 0; i < res.size();i++) { cout << "( " << res[i].x << " , " << res[i].y << " ) "<<(i < res.size() - 1 ? " -> " : "\n\n"); }
	
	return 0;
}

