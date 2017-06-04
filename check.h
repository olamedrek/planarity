
#include "Graph.h"

class Point {
public:
    double x, y;

    Point(double x, double y) : x(x), y(y) {}

    Point(pair<int,int> p) : x(p.first), y(p.second) {}
};

class Segment {
private:
    Point p1, p2;
    double A, B, C;
    const double EPS = 0.0000001;

public:
    Segment(Point p1, Point p2) : p1(p1), p2(p2) {
        if(p1.x == p2.x) {
            A = 1;
            B = 0;
            C = -p1.x;
        } else {
            A = (p2.y - p1.y) / (p2.x - p1.x);
            B = -1;
            C = p1.y - A * p1.x;
        }
    }

    bool contains(Point p) {
        if(A * p.x + B * p.y + C >= EPS) return false;

        return p1.x - p2.x < EPS ?
               p.y >= min(p1.y, p2.y) && p.y <= max(p1.y, p2.y) :
               p.x >= min(p1.x, p2.x) && p.x <= max(p1.x, p2.x);
    }

    friend Point get_intersection(Segment s1, Segment s2) {
        double y = (s2.A * s1.C - s1.A * s2.C) / (s1.A * s2.B - s1.B * s2.A);
        double x = (-1 * s1.C - s1.B * y) / s1.A;
        return Point(x, y);
    }
};

bool check_drawing(const Graph &G, vector<pair<int,int>> &coordinates) {
    for(int i = 0; i < G.n; i++) {
        for(int j = i+1; j < G.n; j++) {
            if(coordinates[i] == coordinates[j]) {
                return false;
            }
        }
    }
    for(int i = 0; i < G.num_of_edges(); i++) {
        for(int j = i+1; j < G.num_of_edges(); j++) {
            int u1, v1, u2, v2;
            tie(u1, v1) = G.get_edge_ends(i);
            tie(u2, v2) = G.get_edge_ends(j);

            if(u1 == u2 || u1 == v2 || v1 == u2 || v1 == v2) continue;

            Segment edge1(coordinates[u1], coordinates[v1]);
            Segment edge2(coordinates[u2], coordinates[v2]);

            Point intersection = get_intersection(edge1, edge2);
            if(edge1.contains(intersection) && edge2.contains(intersection)) {
                return false;
            }
        }
    }
    return true;
}