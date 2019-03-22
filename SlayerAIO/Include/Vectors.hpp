#pragma once

#pragma warning(push, 0)
#pragma warning(disable: 26495 4774)
#include <d3dx9.h>
#include <d3d9.h>
#include <Windows.h>
#include <vector>
#include <sstream>
#pragma warning(pop)

class Vector3;
class IntersectionResult;
class ProjectionInfo;

#define EPSILON 0.0001
#define M_PI	3.14159265358979323846

#pragma region Vector2 Declaration
typedef class Vector2 : public D3DXVECTOR2 {
public:
	#pragma region Constructors
	Vector2() {
		x = 0.0f;
		y = 0.0f;
	}
	Vector2(D3DXVECTOR2 vec) {
		x = vec.x;
		y = vec.y;
	}
	Vector2(D3DXVECTOR3 vec) {
		x = vec.x;
		y = vec.z;
	}
	Vector2(D3DXVECTOR2* vec) {
		x = vec->x;
		y = vec->y;
	}
	Vector2(D3DXVECTOR3* vec) {
		x = vec->x;
		y = vec->z;
	}
	Vector2(float X, float Y, float Z) {
		UNREFERENCED_PARAMETER(Y);

		x = X;
		y = Z;
	}
	Vector2(float X, float Z) {
		x = X;
		y = Z;
	}
	#pragma endregion
	
	#pragma region Operator Overloads
	// casting
	D3DXINLINE operator float* () {
		return (float *)&x;
	}
	D3DXINLINE operator CONST float* () const {
		return (CONST float *) &x;
	}

	// assignment operators
	D3DXINLINE Vector2& operator=(const Vector2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}
	D3DXINLINE Vector2&	 operator += (CONST Vector2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	D3DXINLINE Vector2&	 operator -= (CONST Vector2& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	D3DXINLINE Vector2&	 operator *= (float f) {
		x *= f;
		y *= f;
		return *this;
	}
	D3DXINLINE Vector2&	 operator /= (float f) {
		float fInv = 1.0f / f;
		x *= fInv;
		y *= fInv;
		return *this;
	}
	
	// unary operators
	D3DXINLINE Vector2 operator + () const {
		return *this;
	}
	D3DXINLINE Vector2	operator - () const {
		return Vector2(-x, -y);
	}

	// binary operators
	D3DXINLINE Vector2	 operator + (CONST Vector2& v) const {
		return Vector2(x + v.x, y + v.y);
	}
	D3DXINLINE Vector2	 operator - (CONST Vector2& v) const {
		return Vector2(x - v.x, y - v.y);
	}
	D3DXINLINE Vector2	 operator * (float f) const {
		return Vector2(x * f, y * f);
	}
	D3DXINLINE Vector2	 operator / (float f) const {
		float fInv = 1.0f / f;
		return Vector2(x * fInv, y * fInv);
	}
	D3DXINLINE bool operator == (CONST Vector2& v) const {
		return x == v.x && y == v.y;
	}
	D3DXINLINE bool	 operator != (CONST Vector2& v) const {
		return x != v.x || y != v.y;
	}
	#pragma endregion

	std::string ToString();
	float Length();
	Vector2 Normalize();
	Vector2 Normalized();
	float AngleBetween(Vector2& b);
	float AngleBetween(Vector3& b);
	Vector2 Closest(std::vector<Vector2>& arr);
	Vector3 Closest(std::vector<Vector3>& arr);	
	float DotProduct(Vector2& other);
	float CrossProduct(Vector2& other);
	float Distance(Vector2& b);
	float Distance(Vector3& b);
	float Distance(Vector2& segmentStart, Vector2& segmentEnd, bool onlyIfOnSegment = false);
	float DistanceSqr(Vector2& b);
	float DistanceSqr(Vector3& b);
	float DistanceSqr(Vector2& segmentStart, Vector2& segmentEnd, bool onlyIfOnSegment = false);
	Vector2 Extended(Vector2& b, float distance);
	Vector2 Extended(Vector3& b, float distance);
	IntersectionResult Intersection(Vector2& lineSegment1End, Vector2& lineSegment2Start, Vector2& lineSegment2End);
	bool IsValid();
	Vector2 Perpendicular(int offset = 0);

	//Returns Angle in Degrees
	float Polar();

	ProjectionInfo ProjectOn(Vector2& segmentStart, Vector2& segmentEnd);

	//Input Angle in Radians
	Vector2 Rotated(float angle);

	//Input Angle in Radians
	Vector2 RotatedAroundPoint(Vector2& b, float angle);

	Vector3 To3D(float h = 0.0f);
	bool IsOnScreen(float radius = 0.0f);
	int  GetCollisionFlags(float height = 0.0f);
	bool IsWall(float height = 0.0f);
	bool IsGrass(float height = 0.0f);
	float GetTerrainHeight(float height = 0.0f);
	bool IsBetween(Vector2& a, Vector2& c);
	bool IsLeftOfLineSegment(Vector2& start, Vector2& end);
	bool IsRightOfLineSegment(Vector2& start, Vector2& end);

} SDKPOINT, *PSDKPOINT;

D3DXINLINE Vector2 operator * (float f, CONST Vector2& v) {
	return Vector2(f * v.x, f * v.y);
}
#pragma endregion

#pragma region Projection And Intersection Classes
class ProjectionInfo {
public:
	bool IsOnSegment;
	Vector2 LinePoint;
	Vector2 SegmentPoint;

	ProjectionInfo() {
		IsOnSegment  = false;
		LinePoint    = Vector2();
		SegmentPoint = Vector2();
	};
	ProjectionInfo(bool isOnSegment, Vector2 linePoint, Vector2 segmentPoint) {
		IsOnSegment  = isOnSegment;
		LinePoint    = linePoint;
		SegmentPoint = segmentPoint;
	};
};

class IntersectionResult {
public:
	bool Intersects;
	Vector2 Point;
	IntersectionResult() {
		Intersects = false;
	};
	IntersectionResult(bool intersects, Vector2 point) {
		Intersects = intersects;
		Point = point;
	};
};

#pragma endregion

#pragma region Vector3 Declaration
typedef class Vector3 : public D3DXVECTOR3 {
public:
	#pragma region Constructors
	Vector3() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	Vector3(D3DXVECTOR2 vec) {
		x = vec.x;
		y = 0.0f;
		z = vec.y;
	}
	Vector3(D3DXVECTOR3 vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
	Vector3(float X, float Y, float Z) {
		x = X;
		y = Y;
		z = Z;
	}
	Vector3(float X, float Z) {
		x = X;
		y = 0.0f;
		z = Z;
	}
	#pragma endregion

	#pragma region Operator Overloads
	// assignment operators
	D3DXINLINE Vector3&  operator=(const Vector3& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
	D3DXINLINE Vector3&  operator += (CONST Vector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	D3DXINLINE Vector3&  operator -= (CONST Vector3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	D3DXINLINE Vector3&  operator *= (float f) {
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}
	D3DXINLINE Vector3&  operator /= (float f) {
		float fInv = 1.0f / f;
		x *= fInv;
		y *= fInv;
		z *= fInv;
		return *this;
	}

	// unary operators
	D3DXINLINE Vector3  operator + () const {
		return *this;
	}
	D3DXINLINE Vector3  operator - () const {
		return Vector3(-x, -y, -z);
	}
	
	// binary operators
	D3DXINLINE Vector3  operator + (CONST Vector3& v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}
	D3DXINLINE Vector3  operator - (CONST Vector3& v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}
	D3DXINLINE Vector3  operator * (float f) const {
		return Vector3(x * f, y * f, z * f);
	}
	D3DXINLINE Vector3  operator / (float f) const {
		float fInv = 1.0f / f;
		return Vector3(x * fInv, y * fInv, z * fInv);
	}
	D3DXINLINE bool	operator == (CONST Vector3& v) const {
		return x == v.x && y == v.y && z == v.z;
	}
	D3DXINLINE bool	operator != (CONST Vector3& v) const {
		return x != v.x || y != v.y || z != v.z;
	}
	#pragma endregion
	
	std::string ToString();
	float Length();
	Vector3 Normalize();
	Vector3 Normalized();
	float DotProduct(Vector3& other);
	Vector3 CrossProduct(Vector3& other);
	float AngleBetween(Vector3& b);
	float AngleBetween(Vector2& b);
	Vector3 Closest(std::vector<Vector3>& arr);
	Vector2 Closest(std::vector<Vector2>& arr);
	float Distance(Vector3& b);
	float Distance(Vector2& b);
	float DistanceSqr(Vector3& b);
	float DistanceSqr(Vector2& b);
	Vector3 Extended(Vector3& b, float distance);
	Vector3 Extended(Vector2& b, float distance);
	bool IsValid();
	Vector3 Perpendicular(int offset = 0);

	//Returns Angle in Degrees
	float Polar();

	//Input Angle in Radians
	Vector3 Rotated(float angle);

	//Input Angle in Radians
	Vector3 RotatedAroundPoint(Vector3& b, float angle);

	Vector2 To2D();

	int GetCollisionFlags();
	
	bool IsWall();
	bool IsGrass();
	bool IsOnScreen(float radius = 0.0f);
	float GetTerrainHeight();

	static Vector3 Zero;
	static Vector3 DirectionVector;
}SDKVECTOR, *PSDKVECTOR;

D3DXINLINE Vector3 operator * (float f, CONST class Vector3& v) {
	return Vector3(f * v.x, f * v.y, f * v.z);
}
#pragma endregion

inline std::pair<float, Vector2> VectorMovementCollision(Vector2& startPoint1, Vector2& endPoint1, float v1, Vector2& startPoint2, float v2, float delay = 0.0f) {
	float sP1X = startPoint1.x;
	float sP1Y = startPoint1.y;
	float eP1X = endPoint1.x;
	float eP1Y = endPoint1.y;
	float sP2X = startPoint2.x;
	float sP2Y = startPoint2.y;

	float d = eP1X - sP1X;
	float e = eP1Y - sP1Y;
	float dist = (float)std::sqrt(d * d + e * e), t1 = nanf("");

	float s = std::abs(dist) > EPSILON ? v1 * d / dist : 0.0f;
	float k = std::abs(dist) > EPSILON ? v1 * e / dist : 0.0f;

	float r = sP2X - sP1X, j = sP2Y - sP1Y;
	auto c = r * r + j * j;

	if (dist > 0.0f) {
		if (std::abs(v1 - HUGE_VALF) < EPSILON) {
			auto t = dist / v1;
			t1 = v2 * t >= 0.f ? t : nanf("");
		}
		else if (std::abs(v2 - HUGE_VALF) < EPSILON) {
			t1 = 0.f;
		}
		else {
			float a = s * s + k * k - v2 * v2, b = -r * s - j * k;

			if (std::abs(a) < EPSILON) {
				if (std::abs(b) < EPSILON) {
					t1 = std::abs(c) < EPSILON ? 0.f : nanf("");
				}
				else {
					auto t = -c / (2 * b);
					t1 = v2 * t >= 0.f ? t : nanf("");
				}
			}
			else {
				auto sqr = b * b - a * c;

				if (!(sqr >= 0)) {
					return std::make_pair(t1, !isnan(t1) ? Vector2(sP1X + s * t1, sP1Y + k * t1) : Vector2());
				}

				auto nom = (float)std::sqrt(sqr);
				auto t = (-nom - b) / a;

				t1 = v2 * t >= 0.f ? t : nanf("");
				t = (nom - b) / a;

				auto t2 = v2 * t >= 0.f ? t : nanf("");

				if (isnan(t2) || isnan(t1)) {
					return std::make_pair(t1, !isnan(t1) ? Vector2(sP1X + s * t1, sP1Y + k * t1) : Vector2());
				}

				if (t1 >= delay && t2 >= delay) {
					t1 = min(t1, t2);
				}
				else if (t2 >= delay) {
					t1 = t2;
				}
			}
		}
	}
	else if (std::abs(dist) < EPSILON) {
		t1 = 0.f;
	}

	return std::make_pair(t1, !isnan(t1) ? Vector2(sP1X + s * t1, sP1Y + k * t1) : Vector2());
}

//Delay actually works for spells
inline std::pair<float, Vector2> VectorMovementCollisionEx(Vector2& _startPoint1, Vector2& endPoint1, float v1, Vector2& startPoint2, float v2, float delay = 0.0f) {
	auto startPoint1{delay > EPSILON ? _startPoint1.Extended(endPoint1, v1 * delay) : _startPoint1};
	float sP1X = startPoint1.x;
	float sP1Y = startPoint1.y;
	float eP1X = endPoint1.x;
	float eP1Y = endPoint1.y;
	float sP2X = startPoint2.x;
	float sP2Y = startPoint2.y;

	float d = eP1X - sP1X;
	float e = eP1Y - sP1Y;
	float dist = (float)std::sqrt(d * d + e * e), t1 = nanf("");

	float s = std::abs(dist) > EPSILON ? v1 * d / dist : 0.0f;
	float k = std::abs(dist) > EPSILON ? v1 * e / dist : 0.0f;

	float r = sP2X - sP1X, j = sP2Y - sP1Y;
	auto c = r * r + j * j;

	if (dist > 0.0f) {
		if (std::abs(v1 - HUGE_VALF) < EPSILON) {
			auto t = dist / v1;
			t1 = v2 * t >= 0.f ? t : nanf("");
		}
		else if (std::abs(v2 - HUGE_VALF) < EPSILON) {
			t1 = 0.f;
		}
		else {
			float a = s * s + k * k - v2 * v2, b = -r * s - j * k;

			if (std::abs(a) < EPSILON) {
				if (std::abs(b) < EPSILON) {
					t1 = std::abs(c) < EPSILON ? 0.f : nanf("");
				}
				else {
					auto t = -c / (2 * b);
					t1 = v2 * t >= 0.f ? t : nanf("");
				}
			}
			else {
				auto sqr = b * b - a * c;

				if (!(sqr >= 0)) {
					return std::make_pair(t1, !isnan(t1) ? Vector2(sP1X + s * t1, sP1Y + k * t1) : Vector2());
				}

				auto nom = (float)std::sqrt(sqr);
				auto t = (-nom - b) / a;

				t1 = v2 * t >= 0.f ? t : nanf("");
				t = (nom - b) / a;

				auto t2 = v2 * t >= 0.f ? t : nanf("");

				if (isnan(t2) || isnan(t1)) {
					return std::make_pair(t1, !isnan(t1) ? Vector2(sP1X + s * t1, sP1Y + k * t1) : Vector2());
				}

				if (t1 >= 0.0f && t2 >= 0.0f) {
					t1 = min(t1, t2);
				}
				else if (t2 >= 0.0f) {
					t1 = t2;
				}
			}
		}
	}
	else if (std::abs(dist) < EPSILON) {
		t1 = 0.f;
	}

	return std::make_pair(t1, !isnan(t1) ? Vector2(sP1X + s * t1, sP1Y + k * t1) : Vector2());
}


inline std::vector<Vector2> CircleCircleIntersection(Vector2& center1, Vector2& center2, float radius1, float radius2) {
	float d = center1.Distance(center2);
	if (d > radius1 + radius2 || d <= std::abs(radius1 - radius2)) {
		return std::vector<Vector2> {};
	}
	float a = (((radius1 * radius1) - (radius2*radius2) + (d*d)) / (2 * d));
	auto h = std::sqrt((radius1 * radius1) - (a * a));
	auto direction = (center2 - center1).Normalized();
	auto pa = center1 + (a * direction);
	auto s1 = pa + (h * direction.Perpendicular());
	auto s2 = pa - (h * direction.Perpendicular());
	return std::vector<Vector2>{ s1, s2 };
}
inline std::vector<Vector2> CircleCircleIntersection(Vector3& center1, Vector3& center2, float radius1, float radius2) {
	Vector2 Center1{ center1}, Center2{ center2 };
	return CircleCircleIntersection(Center1, Center2, radius1, radius2);
}

inline bool CheckLineIntersection(Vector2& a, Vector2& b, Vector2& c, Vector2& d) {
	return a.Intersection(b, c, d).Intersects;
}

//Returns a pair where lines meet (between 0 and 1 or HUGE_VALF if parallel)
inline std::pair<float, float> LineToLineIntersection(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) {
	auto d = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);

	if (d == 0)	{
		return std::make_pair(HUGE_VALF, HUGE_VALF); 
	}
	else {
		return std::make_pair(((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / d,
			((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / d);
	}
}

//Returns true if lines meet
inline bool CheckLineIntersectionEx(Vector2 a, Vector2 b, Vector2 c, Vector2 d) {
	auto ret{ LineToLineIntersection(a.x, a.y, b.x, b.y, c.x, c.y, d.x, d.y)};

	if (ret.first >= 0 && ret.first <= 1 && ret.second >= 0 && ret.second <= 1) {
		return true;
	}
	return false;	
}

//Returns a Vector2 where lines meet (between 0 and 1 or Vector2(0,0) if parallel)
inline Vector2 CheckLineIntersectionEx2(Vector2 a, Vector2 b, Vector2 c, Vector2 d) {
	auto ret{ LineToLineIntersection(a.x, a.y, b.x, b.y, c.x, c.y, d.x, d.y) };

	if (ret.first >= 0 && ret.first <= 1 && ret.second >= 0 && ret.second <= 1) {
		return Vector2(ret.first, ret.second);
	}
	return Vector2();
}

//https://code.google.com/p/xna-circle-collision-detection/downloads/detail?name=Circle%20Collision%20Example.zip&can=2&q=
inline std::pair<bool, float> GetCollisionTime(Vector2& Pa, Vector2& Pb, Vector2& Va, Vector2& Vb, float Ra, float Rb) {
	if (Pa.Distance(Pb) < (Ra + Rb)) {
		return std::make_pair(true, 0.0f);
	}

	Vector2 Pab = Pa - Pb;
	Vector2 Vab = Va - Vb;
	
	bool collision{ false };
	float a{ Vab.DotProduct(Vab) };
	float b{ 2 * Pab.DotProduct(Vab) };
	float c{ Pab.DotProduct(Pab) - (Ra + Rb) * (Ra + Rb) };

	float discriminant = b * b - 4 * a * c;

	float t;
	if (discriminant < 0) {
		t = -b / (2 * a);
		collision = false;
	}
	else {
		float t0 = (-b + (float)std::sqrt(discriminant)) / (2 * a);
		float t1 = (-b - (float)std::sqrt(discriminant)) / (2 * a);

		if (t0 >= 0 && t1 >= 0)
			t = min(t0, t1);
		else
			t = max(t0, t1);

		if (t < 0)
			collision = false;
		else
			collision = true;
	}

	if (t < 0)
		t = 0;

	return std::make_pair(collision, t);
}

inline float GetCollisionDistance(Vector2 Pa, Vector2 PaEnd, Vector2 Va, float Ra,
	Vector2 Pb, Vector2 PbEnd, Vector2 Vb, float Rb)
{
	auto t{ GetCollisionTime(Pa, Pb, Va, Vb, Ra, Rb) };
	bool collision{ t.first };
	auto collisionTime{ t.second };

	if (collision) {
		Vector2 PA = Pa + (collisionTime * Va);
		Vector2 PB = Pb + (collisionTime * Vb);

		PA = PA.ProjectOn(Pa, PaEnd).SegmentPoint;
		PB = PB.ProjectOn(Pb, PbEnd).SegmentPoint;

		return PA.Distance(PB);
	}

	return HUGE_VALF;
}

inline float GetCollisionDistanceEx(Vector2 Pa, Vector2 Va, float Ra,
	Vector2 Pb, Vector2 Vb, float Rb,
	_Out_opt_ Vector2* PA, _Out_opt_ Vector2* PB)
{
	auto t{ GetCollisionTime(Pa, Pb, Va, Vb, Ra, Rb) };
	bool collision{ t.first };
	auto collisionTime{ t.second };

	if (collision)	{
		auto _Pa{ Pa + (collisionTime * Va) };
		if (PA != NULL)
			*PA = _Pa;

		auto _Pb{ Pb + (collisionTime * Vb) };
		if (PB != NULL)
			*PB = _Pb;

		return _Pa.Distance(_Pb);
	}

	if (PA != NULL)
		*PA = Vector2();

	if (PB != NULL)
		*PB = Vector2();

	return HUGE_VALF;
}

///http://geomalgorithms.com/a07-_distance.html
#pragma region Closest Point of Approach

struct Track {
	Vector2 P0;
	Vector2 v;

	Track(Vector2 P0, Vector2 v) {
		this->P0 = P0;
		this->v = v;
	}
};


// cpa_time(): compute the time of CPA for two tracks
//    Input:  two tracks Tr1 and Tr2
//    Return: the time at which the two tracks are closest
inline float cpa_time(Track& Tr1, Track& Tr2) {
	Vector2 dv = Tr1.v - Tr2.v;

	float dv2 = dv.DotProduct(dv);
	if (dv2 < EPSILON)      // the  tracks are almost parallel
		return 0.0f;        // any time is ok.  Use time 0.

	Vector2 w0 = Tr1.P0 - Tr2.P0;
	return -w0.DotProduct(dv) / dv2;
}

// cpa_time(): compute the time of CPA for two tracks
//    Input:  two pos vectors p1 and p2 / two vel vectors v1 and v2
//    Return: the time at which the two tracks are closest
inline float cpa_time(Vector2& p1, Vector2& v1, Vector2& p2, Vector2& v2) {
	Track Tr1 {p1, v1};
	Track Tr2 {p2, v2};
	return cpa_time(Tr1, Tr2);
}

// cpa_distance(): compute the distance at CPA for two tracks
//    Input:  two tracks Tr1 and Tr2
//    Return: the distance for which the two tracks are closest
inline float cpa_distance(Track& Tr1, Track& Tr2) {
	float ctime = cpa_time(Tr1, Tr2);
	Vector2 P1 = Tr1.P0 + (ctime * Tr1.v);
	Vector2 P2 = Tr2.P0 + (ctime * Tr2.v);

	return (P1-P2).Length(); 
}

// cpa_distance(): compute the distance at CPA for two tracks
//    Input:  two pos vectors p1 and p2 / two vel vectors v1 and v2
//    Return: the distance for which the two tracks are closest
inline float cpa_distance(Vector2& p1, Vector2& v1, Vector2& p2, Vector2& v2) {
	Track Tr1{ p1, v1 };
	Track Tr2{ p2, v2 };
	return cpa_distance(Tr1, Tr2);
}

inline float CPAPoints(Vector2 p1, Vector2 v1, Vector2 p2, Vector2 v2, _Out_opt_ Vector2* ret1, _Out_opt_ Vector2* ret2) {
	Track Tr1 {p1, v1};
	Track Tr2 {p2, v2};

	float ctime = cpa_time(Tr1, Tr2);

	Vector2 P1 = Tr1.P0 + (ctime * Tr1.v);
	Vector2 P2 = Tr2.P0 + (ctime * Tr2.v);

	if (ctime <= 0)	{
		P1 = Tr1.P0;
		P2 = Tr2.P0;
	}

	if (ret1 != NULL)
		*ret1 = P1;
	if (ret2!= NULL)
		*ret2 = P2;

	return (P1-P2).Length();
}

inline float CPAPointsEx(Vector2& p1, Vector2& v1, Vector2& p2, Vector2& v2, Vector2& p1end, Vector2& p2end) {
	Track Tr1 {p1, v1};
	Track Tr2 {p2, v2};

	float ctime{ max(0, cpa_time(Tr1, Tr2)) };

	Vector2 P1 {Tr1.P0 + (ctime * Tr1.v)};
	Vector2 P2 {Tr2.P0 + (ctime * Tr2.v)};

	P1 = (p1-P1).Length() > (p1-p1end).Length() ? p1end : P1;
	P2 = (p2-P2).Length() > (p2-p2end).Length() ? p2end : P2;

	return (P1-P2).Length();
}

inline float CPAPointsEx(Vector2& p1, Vector2& v1, Vector2& p2, Vector2& v2, Vector2& p1end, Vector2& p2end, _Out_opt_ Vector2* p1out, _Out_opt_ Vector2* p2out) {
	UNREFERENCED_PARAMETER(p1end);
	UNREFERENCED_PARAMETER(p2end);

	Track Tr1 {p1, v1};
	Track Tr2 {p2, v2};

	float ctime{ cpa_time(Tr1, Tr2) };

	if (ctime == 0)	{
		auto t{ GetCollisionTime(p1, p2, v1, v2, 10, 10) };
		bool  &collision{ t.first };
		float &collisionTime{ t.second };

		if (collision) {
			ctime = collisionTime;
		}
	}

	Vector2 P1 {Tr1.P0 + (ctime * Tr1.v)};
	Vector2 P2 {Tr2.P0 + (ctime * Tr2.v)};	

	if (p1out != NULL)
		*p1out = P1;

	if (p2out != NULL)
		*p2out = P2;

	return (P1-P2).Length();
}

#pragma endregion


#define _SDKVECTOR Vector3
#define SDK_USE_CUSTOM_VECTOR
#define SDK_USE_CUSTOM_POINT

#include "sdkapi.h"

__declspec(selectany) PSDK_CONTEXT SDK_CONTEXT_GLOBAL;
__declspec(selectany) void* g_LocalPlayer;

#pragma region Vector2 Implementations
inline std::string Vector2::ToString() {
	std::ostringstream out;
	out << "{ " << this->x << ", " << this->y << " }";
	return out.str();
}

inline Vector3 Vector2::Closest(std::vector<Vector3>& arr) {
	Vector3 result = Vector3{};
	float distance = HUGE_VALF;

	for (auto vector : arr) {
		float tempDist = this->Distance(vector);
		if (distance < tempDist) {
			distance = tempDist;
			result = vector;
		}
	}

	return result;
}

inline float Vector2::DotProduct(Vector2 & other) {
	return (other.x * this->x) + (other.y * this->y);
}

inline float Vector2::CrossProduct(Vector2 & other) {
	return (other.y * this->x) - (other.x * this->y);
}

inline float Vector2::Distance(Vector2 & b) {
	float m_distanceX = (this->x - b.x) * (this->x - b.x);
	float m_distanceY = (this->y - b.y) * (this->y - b.y);

	return std::sqrt(m_distanceX + m_distanceY);
}

inline float Vector2::Distance(Vector3 & b) {
	float m_distanceX = (this->x - b.x) * (this->x - b.x);
	float m_distanceY = (this->y - b.z) * (this->y - b.z);

	return std::sqrt(m_distanceX + m_distanceY);
}

inline float Vector2::Distance(Vector2 & segmentStart, Vector2 & segmentEnd, bool onlyIfOnSegment) {
	auto objects = this->ProjectOn(segmentStart, segmentEnd);

	return (objects.IsOnSegment || onlyIfOnSegment == false) ?
		(this->Distance(objects.SegmentPoint)) :
		HUGE_VALF;
}

inline float Vector2::DistanceSqr(Vector2 & b) {
	float m_distanceX = (this->x - b.x) * (this->x - b.x);
	float m_distanceY = (this->y - b.y) * (this->y - b.y);

	return m_distanceX + m_distanceY;
}

inline float Vector2::DistanceSqr(Vector3 & b) {
	float m_distanceX = (this->x - b.x) * (this->x - b.x);
	float m_distanceY = (this->y - b.z) * (this->y - b.z);

	return m_distanceX + m_distanceY;
}

inline float Vector2::DistanceSqr(Vector2 & segmentStart, Vector2 & segmentEnd, bool onlyIfOnSegment) {
	auto objects = this->ProjectOn(segmentStart, segmentEnd);

	return (objects.IsOnSegment || onlyIfOnSegment == false) ?
		(this->DistanceSqr(objects.SegmentPoint)) :
		HUGE_VALF;
}

inline Vector2 Vector2::Extended(Vector2 & b, float distance) {
	return *this + (distance * (b - *this).Normalized());
}

inline Vector2 Vector2::Extended(Vector3 & b, float distance) {
	return *this + (distance * (b.To2D() - *this).Normalized());
}

inline IntersectionResult Vector2::Intersection(Vector2 & lineSegment1End, Vector2 & lineSegment2Start, Vector2 & lineSegment2End) {
	double deltaACy = this->y - lineSegment2Start.y;
	double deltaDCx = lineSegment2End.x - lineSegment2Start.x;
	double deltaACx = this->x - lineSegment2Start.x;
	double deltaDCy = lineSegment2End.y - lineSegment2Start.y;
	double deltaBAx = lineSegment1End.x - this->x;
	double deltaBAy = lineSegment1End.y - this->y;

	auto denominator = (deltaBAx * deltaDCy) - (deltaBAy * deltaDCx);
	auto numerator = (deltaACy * deltaDCx) - (deltaACx * deltaDCy);

	if (std::abs(denominator) < EPSILON) {
		if (std::abs(numerator) < EPSILON) {
			// collinear. Potentially infinite intersection points.
			// Check and return one of them.
			if (this->x >= lineSegment2Start.x && this->x <= lineSegment2End.x) {
				return IntersectionResult{ true, *this };
			}

			if (lineSegment2Start.x >= this->x && lineSegment2Start.x <= lineSegment1End.x) {
				return IntersectionResult{ true, lineSegment2Start };
			}

			return IntersectionResult{};
		}

		// parallel
		return IntersectionResult{};
	}

	auto r = numerator / denominator;
	if (r < 0 || r > 1) {
		return IntersectionResult{};
	}

	auto s = ((deltaACy * deltaBAx) - (deltaACx * deltaBAy)) / denominator;
	if (s < 0 || s > 1) {
		return IntersectionResult{};
	}

	return IntersectionResult{
		true,
		Vector2((float)(this->x + (r * deltaBAx)),	(float)(this->y + (r * deltaBAy)))
	};
}

inline bool Vector2::IsValid() {
	return *this != Vector2();
}

inline Vector2 Vector2::Perpendicular(int offset) {
	return (offset == 0) ? Vector2(-this->y, this->x) : Vector2(this->y, -this->x);
}

inline float Vector2::Polar() {	
	if (std::abs(this->x - 0) <= (float)1e-9) {
		return (this->y > 0) ? 90.f : ((this->y < 0) ? 270.f : 0.f);
	}

	#pragma warning ( push )
	#pragma warning ( disable: 4723 )
	auto theta = (float)(std::atan(this->y / this->x) * (180 / M_PI));
	#pragma warning ( pop )	
	if (this->x < 0) {
		theta += 180;
	}
	
	return (theta < 0) ? theta + 360 : theta;
}

inline ProjectionInfo Vector2::ProjectOn(Vector2 & segmentStart, Vector2 & segmentEnd) {
	auto cx = this->x;
	auto cy = this->y;
	auto ax = segmentStart.x;
	auto ay = segmentStart.y;
	auto bx = segmentEnd.x;
	auto by = segmentEnd.y;	

	auto rL = (((cx - ax) * (bx - ax)) + ((cy - ay) * (by - ay)))
		/ ((float)std::pow(bx - ax, 2) + (float)std::pow(by - ay, 2));
	auto pointLine = Vector2(ax + (rL * (bx - ax)), ay + (rL * (by - ay)));
	float rS;
	if (rL < 0) {
		rS = 0;
	}
	else if (rL > 1) {
		rS = 1;
	}
	else {
		rS = rL;
	}

	auto isOnSegment = (std::abs(rS - rL) < EPSILON);
	auto pointSegment = isOnSegment ? pointLine : Vector2(ax + (rS * (bx - ax)), ay + (rS * (by - ay)));
	return ProjectionInfo{ isOnSegment, pointSegment, pointLine };
}

inline Vector2 Vector2::Rotated(float angle) {
	float cos = std::cos(angle);
	float sin = std::sin(angle);

	return Vector2(
		(float)(this->x * cos + this->y * sin),
		(float)(this->y * cos - this->x * sin)
	);
}

inline Vector2 Vector2::RotatedAroundPoint(Vector2 & b, float angle) {
	return (*this - b).Rotated(angle) + b;
}

inline Vector3 Vector2::To3D(float h) {
	return Vector3(x, h, y);
}

inline bool Vector2::IsOnScreen(float radius) {
	return this->To3D().IsOnScreen(radius);
}

inline int Vector2::GetCollisionFlags(float height) {
	int Flags{ 0 };
	auto position{ this->To3D(height) };
	SdkGetCollisionFlags(&position, &Flags);
	return Flags;
}

inline bool Vector2::IsWall(float height) {
	bool bIsWall = false;
	auto position { this->To3D(height) };
	SdkIsLocationWall(&position, &bIsWall);

	return bIsWall;
}

inline bool Vector2::IsGrass(float height) {
	return (GetCollisionFlags(height) & COLLISION_FLAG_GRASS);
}

inline float Vector2::GetTerrainHeight(float height) {
	auto Position { this->To3D(height) };

	float h = 0.0f;
	bool valid = false;
	SdkGetTerrainHeight(&Position, &h, &valid);

	return ((valid) ? h : 0.0f);
}

inline bool Vector2::IsBetween(Vector2 & a, Vector2 & c) {
	return a.Distance(c) + c.Distance(*this) - a.Distance(*this) < EPSILON;
}

inline bool Vector2::IsLeftOfLineSegment(Vector2 & start, Vector2 & end) {
	return ((end.x - start.x) * (this->y - start.y) - (end.y - start.y) * (this->x - start.x)) > 0;
}

inline bool Vector2::IsRightOfLineSegment(Vector2 & start, Vector2 & end) {
	return !this->IsLeftOfLineSegment(start, end);
}

inline float Vector2::Length() {
	return (float)sqrt((x * x) + (y * y));
}

inline Vector2 Vector2::Normalize() {
	*this *= 1.0f / Length();
	return *this;
}

inline Vector2 Vector2::Normalized() {
	return (*this * 1.0f / Length());
}

inline float Vector2::AngleBetween(Vector2 & b) {
	auto theta = this->Polar() - b.Polar();

	if (theta < 0) { theta += 360; }
	return (theta > 180) ? (360 - theta) : theta;
}

inline float Vector2::AngleBetween(Vector3 & b) {
	Vector2 Pos(b.To2D());
	return this->AngleBetween(Pos);
}

inline Vector2 Vector2::Closest(std::vector<Vector2>& arr) {
	Vector2 result = Vector2{};
	float distance = HUGE_VALF;

	for (auto vector : arr) {
		float tempDist = this->Distance(vector);
		if (distance < tempDist) {
			distance = tempDist;
			result = vector;
		}
	}

	return result;
}
#pragma endregion

#pragma region Vector3 Implementations

inline std::string Vector3::ToString() {
	std::ostringstream out;
	out << "{ " << this->x << ", " << this->y << ", " << this->z << " }";
	return out.str();
}

inline float Vector3::Length() {
	return (float)sqrt((x * x) + (y * y) + (z * z));
}

inline Vector3 Vector3::Normalize() {
	*this *= 1.0f / Length();
	return *this;
}

inline Vector3 Vector3::Normalized() {
	return (*this * 1.0f / Length());
}

inline float Vector3::DotProduct(Vector3 & other) {
	return (this->x * other.x + this->y * other.y + this->z * other.z);
}

inline Vector3 Vector3::CrossProduct(Vector3 & other) {
	return {
		this->y * other.z - this->z * other.y,
		this->z * other.x - this->x * other.z,
		this->x * other.y - this->y * other.x
	};
}


inline float Vector3::AngleBetween(Vector3 & b) {
	return this->To2D().AngleBetween(b);
}

inline float Vector3::AngleBetween(Vector2 & b) {
	return this->To2D().AngleBetween(b);
}

inline Vector3 Vector3::Closest(std::vector<Vector3>& arr) {
	return this->To2D().Closest(arr);
}

inline Vector2 Vector3::Closest(std::vector<Vector2>& arr) {
	return this->To2D().Closest(arr);
}

inline float Vector3::Distance(Vector3 & b) {
	return this->To2D().Distance(b);
}

inline float Vector3::Distance(Vector2 & b) {
	return this->To2D().Distance(b);
}

inline float Vector3::DistanceSqr(Vector3 & b) {
	return this->To2D().DistanceSqr(b);
}

inline float Vector3::DistanceSqr(Vector2 & b) {
	return this->To2D().DistanceSqr(b);
}

inline Vector3 Vector3::Extended(Vector3 & b, float distance) {
	return *this + (distance * (b - *this).Normalized());
}

inline Vector3 Vector3::Extended(Vector2 & b, float distance) {
	return *this + (distance * (b.To3D(this->y) - *this).Normalized());
}

inline bool Vector3::IsValid() {
	return (this->x != 0 && this->z != 0);
}

inline Vector3 Vector3::Perpendicular(int offset) {
	return (offset == 0) ? Vector3(this->z, this->y, -this->x) : Vector3(-this->z, this->y, this->x);
}

inline float Vector3::Polar() {
	return this->To2D().Polar();
}

inline Vector3 Vector3::Rotated(float angle) {
	float cos = std::cos(angle);
	float sin = std::sin(angle);

	return Vector3(
		(float)(this->x * cos - this->z * sin),
		this->y,
		(float)(this->z * cos + this->x * sin)
	);
}

inline Vector3 Vector3::RotatedAroundPoint(Vector3 & b, float angle) {
	Vector2 Pos{ b };
	return (this->To2D()).RotatedAroundPoint(Pos, angle).To3D(b.y);
}

inline Vector2 Vector3::To2D() {
	return Vector2(x, z);
}
inline int Vector3::GetCollisionFlags() {
	int Flags{ 0 };
	SdkGetCollisionFlags(this, &Flags);
	return Flags;
}
inline bool Vector3::IsWall() {
	bool tmp;  SdkIsLocationWall(this, &tmp);
	return tmp;
}
inline bool Vector3::IsGrass() {	
	return (GetCollisionFlags() & COLLISION_FLAG_GRASS);
}
inline bool Vector3::IsOnScreen(float radius) {
	RECT lpRect{};
	HWND Handle;
	SdkUiGetWindow(&Handle);
	GetClientRect(Handle, &lpRect);

	auto Width{ (lpRect.right - lpRect.left) };
	auto Height{ (lpRect.bottom - lpRect.top) };

	Vector2 pos;
	SdkWorldToScreen(this, &pos);

	return !(pos.x + radius < 0.0f || pos.x - radius > Width
		|| pos.y + radius < 0.0f || pos.y - radius > Height);
}
inline float Vector3::GetTerrainHeight() {
	float h{ 0.0f };
	bool valid{ false };
	SdkGetTerrainHeight(this, &h, &valid);
	return valid ? h : 0.0f;
}
#pragma endregion

__declspec(selectany) Vector3 Vector3::DirectionVector{ 0.0f, 0.0f, 1.0f };