#pragma once

#pragma once

#include <vector>

#include "SDK Extensions.h"
#include "clipper.hpp"

using namespace ClipperLib;

namespace Geometry {
	class Polygon {
	public:
		std::vector<Vector2> Points;

		Polygon() {};
		Polygon(std::vector<Polygon>& poly) {
			this->Points.reserve(poly.size());
			for (auto polygon : poly) {
				for (auto point : polygon.Points) {
					this->Points.push_back(point);
				}
			}
		}
		Polygon(Path& clipperPath) {
			this->Points.reserve(clipperPath.size());
			for (auto point : clipperPath) {				
				this->Points.push_back({(float)point.X, (float)point.Y});
			}
		}

		inline void Add(Vector2 point) {
			Points.push_back(point);
		}

		inline void Add(Polygon poly) {
			this->Points.reserve(poly.Points.size());
			for (auto point : poly.Points) {
				this->Points.push_back(point);
			}
		}

		inline void Draw(int width = 1, PSDKCOLOR Color = &Color::White) {
			for (size_t i = 0; i < Points.size(); i++) {
				auto nextIndex = (Points.size() - 1 == i) ? 0 : (i + 1);
				auto start = Points[i].To3D(Points[i].GetTerrainHeight());
				auto end = Points[nextIndex].To3D(Points[nextIndex].GetTerrainHeight());
				Draw::Line(&start, &end, (float)width, Color);
			}
		};

		/// Creates Paths from polygon for Clipper
		inline Path ToClipperPath() {
			Path result;
			result.reserve(this->Points.size());

			for (auto point : this->Points)
				result.emplace_back((ClipperLib::cInt) point.x, (ClipperLib::cInt) point.y);

			return result;
		}

		virtual inline bool IsOutside(Vector2& point, float extraRadius = 0.0f) {
			UNREFERENCED_PARAMETER(extraRadius);
			IntPoint p((ClipperLib::cInt)point.x, (ClipperLib::cInt)point.y);
			return PointInPolygon(p, this->ToClipperPath()) != 1;
		}

		virtual inline bool IsOutside(Vector3& point, float extraRadius = 0.0f) {
			UNREFERENCED_PARAMETER(extraRadius);
			IntPoint p((ClipperLib::cInt)point.x, (ClipperLib::cInt)point.z);
			return PointInPolygon(p, this->ToClipperPath()) != 1;
		}

		virtual inline bool IsInside(Vector2 point, float extraRadius = 0.0f);

		virtual inline bool IsInside(Polygon& poly) {
			for (auto pos : poly.Points) {
				if (!IsOutside(pos)) {
					return true;
				}				
			}
			return Intersects(poly);
		}		

		virtual inline bool IsInside(Vector3& point, float extraRadius = 0.0f) {
			return IsInside(point.To2D(), extraRadius);
		}

		virtual inline bool IsInside(GameObject* obj, float extraRadius = 0.0f) {
			return IsInside(obj->GetPosition().To2D(), obj->GetBoundingRadius() + extraRadius);
		}

		inline bool Intersects(Polygon& other);
	};

	class Circle : public Polygon {
		static const int CircleLineSegmentN = 22;
		int Quality;
	public:
		Vector2 Center;
		float Radius;

		Circle(Vector3 center, float radius, int quality = 20)
			: Circle(center.To2D(), radius, quality) {
		}

		Circle(Vector2 center, float radius, int quality = 20) {
			Center = center;
			Radius = radius;
			Quality = quality;
			UpdatePolygon();
		}

		void UpdatePolygon(int offset = 0, float overrideWidth = -1) {
			Points.clear();
			auto outRadius = (overrideWidth > 0 ? overrideWidth	: (offset + Radius) / (float)std::cos(2 * M_PI / Quality));
			for (auto i = 1; i <= Quality; i++)	{
				auto angle = i * 2 * M_PI / Quality;				
				Points.emplace_back(Center.x + outRadius * (float)std::cos(angle), Center.y + outRadius * (float)std::sin(angle));
			}
		}

		inline bool IsOutside(Vector2& point, float extraRadius = 0.0f) final {
			return Center.Distance(point) > (Radius + extraRadius);
		}

		inline bool IsOutside(Vector3& point, float extraRadius = 0.0f) final {
			return Center.Distance(point) > (Radius + extraRadius);
		}

		inline bool IsInside(Vector2 point, float extraRadius = 0.0f) final {
			return !IsOutside(point, extraRadius);
		}
	};

	class Rectangle : public Polygon {
	public:
		Vector2 End;
		Vector2 Start;
		float Width;

		Rectangle(Vector3 start, Vector3 end, float width)
			: Rectangle(start.To2D(), end.To2D(), width) {
		}

		Rectangle(Vector2 start, Vector2 end, float width) {
			Start = start;
			End = end;
			Width = width;
			UpdatePolygon();
		}

		void UpdatePolygon(int offset = 0, float overrideWidth = -1) {
			Points.clear();
			auto _width{ (overrideWidth > 0 ? overrideWidth : Width + (float)offset) };
			auto dir{ (float)offset * Direction() };
			auto perp{ _width * Perpendicular() };

			Points.emplace_back(Start + perp - dir);
			Points.emplace_back(Start - perp - dir);
			Points.emplace_back(End   - perp + dir);
			Points.emplace_back(End   + perp + dir);
		}

		Vector2 Direction() {
			return (End - Start).Normalized();
		}

		Vector2 Perpendicular() {
			return Direction().Perpendicular();
		}

		inline bool IsOutside(Vector2& point, float extraRadius = 0.0f) final {
			auto projection{ point.ProjectOn(Start, End) };
			return (!projection.IsOnSegment || projection.SegmentPoint.Distance(point) > (Width + extraRadius));
		}

		inline bool IsOutside(Vector3& point, float extraRadius = 0.0f) final {
			auto projection{ point.To2D().ProjectOn(Start, End) };
			return (!projection.IsOnSegment || projection.SegmentPoint.Distance(point) > (Width + extraRadius));
		}	

		inline bool IsInside(Vector2 point, float extraRadius = 0.0f) final {
			return !IsOutside(point, extraRadius);
		}
	};

	class Ring : public Polygon {
		int Quality;
	public:
		Vector2 Center;
		float InnerRadius;
		float OuterRadius;

		Ring(Vector3 center, float innerRadius, float outerRadius, int quality = 20)
			: Ring(center.To2D(), innerRadius, outerRadius, quality) {
		}

		Ring(Vector2 center, float innerRadius, float outerRadius, int quality = 20) {
			Center = center;
			InnerRadius = innerRadius;
			OuterRadius = outerRadius;
			Quality = quality;
			UpdatePolygon();
		}

		void UpdatePolygon(int offset = 0) {
			Points.clear();
			auto outRadius = (OuterRadius + offset) / (float)std::cos(2 * M_PI / Quality);
			auto innerRadius = (InnerRadius - offset);
			for (int i = 0; i <= Quality; i++) {
				auto angle = i * 2.0 * M_PI / Quality;
				Points.emplace_back(Center.x - outRadius * (float)std::cos(angle), Center.y - outRadius * (float)std::sin(angle));
			}
			for (int i = 0; i <= Quality; i++) {
				auto angle = i * 2.0 * M_PI / Quality;
				Points.emplace_back(Center.x - innerRadius * (float)std::cos(angle), Center.y - innerRadius * (float)std::sin(angle));
			}
		}

		inline bool IsOutside(Vector2& point, float extraRadius = 0.0f) final {
			return Center.Distance(point) > (OuterRadius + extraRadius) || Center.Distance(point) < (InnerRadius - extraRadius);
		}

		inline bool IsOutside(Vector3& point, float extraRadius = 0.0f) final {
			return Center.Distance(point) > (OuterRadius + extraRadius) || Center.Distance(point) < (InnerRadius - extraRadius);
		}

		inline bool IsInside(Vector2 point, float extraRadius = 0.0f) final {
			return !IsOutside(point, extraRadius);
		}
	};

	class Line : public Polygon {
	public:
		Vector2 LineStart;
		Vector2 LineEnd;

		float Length() {
			return LineStart.Distance(LineEnd);
		}

		void SetLength(float value) {
			LineEnd = (LineEnd - LineStart).Normalized() * value + LineStart;
		}

		Line(Vector3 start, Vector3 end, float length = -1) : Line(start.To2D(), end.To2D(), length) {}

		Line(Vector2 start, Vector2 end, float length = -1) {
			LineStart = start;
			LineEnd = end;
			if (length > 0) {
				SetLength(length);
			}
			UpdatePolygon();
		}

		void UpdatePolygon() {
			Points.clear();
			Points.push_back(LineStart);
			Points.push_back(LineEnd);
		}

		inline bool IsOutside(Vector2& point, float extraRadius = 0.0f) final {
			auto projection{ point.ProjectOn(LineStart, LineEnd) };
			return (!projection.IsOnSegment || projection.SegmentPoint.Distance(point) > (1.0f + extraRadius));
		}

		inline bool IsOutside(Vector3& point, float extraRadius = 0.0f) final {
			auto projection{ point.To2D().ProjectOn(LineStart, LineEnd) };
			return (!projection.IsOnSegment || projection.SegmentPoint.Distance(point) > (1.0f + extraRadius));
		}

		inline bool IsInside(Vector2 point, float extraRadius = 0.0f) final {
			return !IsOutside(point, extraRadius);
		}
	};
		
	class Sector :public Polygon {
		int Quality;
	public:
		float Angle;
		Vector2 Center;
		Vector2 Direction;
		float Radius;

		Sector(Vector3 center, Vector3 direction, float angle, float radius, int quality = 20)
			: Sector(center.To2D(), direction.To2D(), angle, radius, quality) {
		}

		Sector(Vector2 center, Vector2 direction, float angle, float radius, int quality = 20) {
			Center = center;
			Direction = (direction - center).Normalized();
			Angle = angle;
			Radius = radius;
			Quality = quality;
			UpdatePolygon();
		}

		void UpdatePolygon(int offset = 0) {
			Points.clear();
			auto outRadius{ (Radius + offset) / (float)std::cos(2 * M_PI / Quality) };
			Points.push_back(Center);
			auto side1{ Direction.Rotated(-Angle * 0.5f) };
			for (int i = 0; i <= Quality; i++) {
				auto cDirection{ side1.Rotated(i * Angle / Quality).Normalized() };
				Points.emplace_back(Center.x + outRadius * cDirection.x, Center.y + outRadius * cDirection.y);
			}
		}

		inline bool IsOutside(Vector2& point, float extraRadius = 0.0f) final {
			if (Center.Distance(point) > (Radius + extraRadius)) { return true; }
			float halfAngle{ Common::RadToDeg((Angle * 0.5f)) };

			auto pointSegment{ point - Center };
			if (Direction.AngleBetween(pointSegment) <= halfAngle) {
				return false;
			}

			if (extraRadius > 0.1f) {
				auto side1{ Center + Direction.Rotated(Angle *  0.5f) * Radius };
				auto side2{ Center + Direction.Rotated(Angle * -0.5f) * Radius };
				auto &nearestSide{ side1.Distance(point) < side2.Distance(point) ? side1 : side2 };

				auto proj{ point.ProjectOn(Center, nearestSide) };
				auto pointSegmentExt{ point.Extended(proj.SegmentPoint, extraRadius) - Center };
				if (Direction.AngleBetween(pointSegmentExt) <= halfAngle) {
					return false;
				}
			}			
			return true;			
		}

		inline bool IsOutside(Vector3& point, float extraRadius = 0.0f) final {
			auto tmp{ point.To2D()};
			return IsOutside(tmp, extraRadius);
		}

		inline bool IsInside(Vector2 point, float extraRadius = 0.0f) final {
			return !IsOutside(point, extraRadius);
		}
	};
	typedef Sector Cone;

	class Arc : public Polygon {
		int Quality;
	public:
		Vector2 StartPos;
		Vector2 EndPos;
		float Radius;
		float Angle;

		Arc(Vector3 start, Vector3 direction, float angle, float radius, int quality = 20)
			: Arc(start.To2D(), direction.To2D(), angle, radius, quality) {
		}

		Arc(Vector2 start, Vector2 direction, float angle, float radius, int quality = 20) {
			StartPos = start;
			EndPos = (direction - start).Normalized();
			Angle = angle;
			Radius = radius;
			Quality = quality;
			UpdatePolygon();
		}

		void UpdatePolygon(int offset = 0) {
			Points.clear();
			auto outRadius = (Radius + offset) / (float)std::cos(2 * M_PI / Quality);
			auto side1 = EndPos.Rotated(-Angle * 0.5f);
			for (int i = 0; i <= Quality; i++) {
				auto cDirection = side1.Rotated(i * Angle / Quality).Normalized();
				Points.emplace_back(StartPos.x + outRadius * cDirection.x, StartPos.y + outRadius * cDirection.y);
			}
		}
	};
	   
	/// Gets position with time
	inline Vector2 PositionAfter(std::vector<Vector2>& Path, float t, float speed, float delay = 0.0f) {
		auto distance = std::max<float>(0.0f, t - delay) * speed;

		for (size_t i = 1; i < Path.size(); i++) {
			auto a = Path[i - 1];
			auto b = Path[i];
			auto d = a.Distance(b);

			if (d < distance) {
				distance -= d;
			}
			else {
				return a + distance * (b - a).Normalized();
			}
		}
		return Path.size() > 0 ? Path[Path.size() - 1] : Vector2();
	}

	/// Gets closest point on circle
	inline Vector2 ClosestCirclePoint(Vector2* center, float radius, Vector2* point) {
		Vector2 v = (*point - *center).Normalized();
		return *center + v * radius;
	}

	inline Vector2 Deviation(Vector2* point1, Vector2* point2, double angle) {
		angle *= M_PI / 180.0;
		Vector2 temp = *point2 - *point1;
		Vector2 result = Vector2(0.0f, 0.0f);
		result.x = (float)(temp.x * std::cos(angle) - temp.y * std::sin(angle)) / 4;
		result.y = (float)(temp.x * std::sin(angle) + temp.y * std::cos(angle)) / 4;
		result = result + *point1;
		return result;
	}

	//Fixed-Radius MEC algorithm made for league of legends by @RMAN#9401
	inline std::pair<Vector2, int> BestCoveringCircle(std::vector<Vector2>& Points, float circleRadius) {
		static constexpr size_t MAX_POINTS(100);

		auto n{ Points.size() };
		if (n == 0) 
			return { Vector2(), 0 };		
		else if (n == 1) 
			return { Points.front(), 1 };
		else if (n >= MAX_POINTS)
			return { Vector2(), 0 };

		//Memoize distance between pairs
		float dis[MAX_POINTS][MAX_POINTS];
		for (size_t i = 0; i < n - 1; i++) {
			for (size_t j = i + 1; j < n; j++) {
				dis[j][i] = Points[i].Distance(Points[j]);
				dis[i][j] = dis[j][i];
			}				
		}			

		int hitCount = 0;
		Vector2 pos{};

		Vector2 xAxis{ 0.0f, 0.0f };
		Vector2 range{ circleRadius, 0.0f };		
		
		//Rotates around each point
		for (size_t i = 0; i < n; i++) {			
			std::vector<std::pair<double, std::pair<Vector2, bool>>> angles;

			//Sorts at what angle each point enters/exits the circle [true = entry, false = exit]
			for (size_t j = 0; j < n; j++) {
				if (i != j && dis[i][j] <= (2 * circleRadius)) {
					double A = (Points[j] - Points[i]).Polar() * (M_PI / 180.0f);
					if (A > M_PI) { A -= (M_PI * 2.0); }
					double B = std::acos(dis[i][j] / (2 * circleRadius));	
					if (B > M_PI) { B -= (M_PI * 2.0); }					
					
					Vector2 c1{ Points[i] + range.Rotated((float)(B-A)) };
					Vector2 c2{ Points[i] + range.Rotated((float)(-B-A)) };			

					angles.push_back({ A-B, {c1, true } });
					angles.push_back({ A+B, {c2, false} });
				}
			}
			std::sort(angles.begin(), angles.end());

			//Gets current hitCount and Center
			Vector2 cent{}; int count = 1, res = 1;
			for (auto it = angles.begin(); it != angles.end(); ++it) {
				if ((*it).second.second)
					count++; //entry
				else
					count--; //exit

				if (count > res) {
					res = count;
					cent = (*it).second.first;
				}	
				
			}

			//Compares current data to best data
			if (res > hitCount) {
				hitCount = res;
				pos = cent;				
			}
		}
		
		//Centralize Circle to enhance hitChance
		Vector2 Center{};
		for (auto &Point : Points) {
			if (Point.Distance(pos) <= (circleRadius+1.0f)) {
				Center += Point;				
			}
		}
		Center /= (float)hitCount;		
		
		return std::make_pair(Center, hitCount);
	}	

	//Fixed-Width MER algorithm made for league of legends by @RMAN#9401
	inline std::pair<Vector2, int> BestCoveringRectangle(std::vector<Vector2>& Points, Vector2& startPos, float width) {
		auto n{ Points.size() };
		if (n == 0)
			return { Vector2(), 0 };
		else if (n == 1)
			return { Points.front(), 1 };

		#pragma region Define Sector All Points Are In
		std::pair<double, Vector2> smallestPolar{ M_PI, Vector2()};
		std::pair<double, Vector2> biggestPolar {-M_PI, Vector2()};

		float range{ 0.0f };
		for (auto &Point : Points) {
			range = std::max<float>(range, startPos.Distance(Point));

			double polar = (Point - startPos).Polar() * (M_PI / 180.0f);
			if (polar > M_PI) { polar -= (M_PI * 2.0); }

			if (polar > biggestPolar.first) {
				biggestPolar.first  = polar;
				biggestPolar.second = Point;
			}

			if (polar < smallestPolar.first) {
				smallestPolar.first  = polar;
				smallestPolar.second = Point;
			}
		}
		
		double angleBetween{ (biggestPolar.first - smallestPolar.first) };
		if (angleBetween > M_PI) { angleBetween = (2 * M_PI) - angleBetween; }		

		Vector2 possibleCenter1{ smallestPolar.second.RotatedAroundPoint(startPos, - ((float)angleBetween * 0.5f)) };
		Vector2 possibleCenter2{ smallestPolar.second.RotatedAroundPoint(startPos, ((float)angleBetween * 0.5f)) };

		float distFromPoints1{ possibleCenter1.Distance(smallestPolar.second) + possibleCenter1.Distance(biggestPolar.second) };
		float distFromPoints2{ possibleCenter2.Distance(smallestPolar.second) + possibleCenter2.Distance(biggestPolar.second) };

		Vector2 centerPoint{distFromPoints1 < distFromPoints2 ? possibleCenter1 : possibleCenter2};		
		#pragma endregion

		//All Points are inside this cone
		Geometry::Cone cone(startPos, centerPoint, (float)angleBetween, range);

		int hitCount{ 0 };
		

		//Iterate Cone Sectors to find best solution
		std::vector<size_t> hitPoints{};
		Vector2 LastPoint{};
		for (auto &sectorPoint : cone.Points) {
			if (sectorPoint != startPos && sectorPoint.Distance(LastPoint) > (width * 0.25f)) {
				LastPoint = sectorPoint;

				auto tmpCount{ 0 };
				std::vector<size_t> tmpPoints{};
				Geometry::Rectangle rect(startPos, sectorPoint, width);
				for (size_t i = 0; i < n; ++i) {
					if (rect.IsInside(Points[i])) {
						++tmpCount;
						tmpPoints.push_back(i);
					}
				}

				if (tmpCount > hitCount) {
					hitCount = tmpCount;
					hitPoints.swap(tmpPoints);
				}
			}
		}	

		//Centralize Rectangle for best HitChance
		Vector2 Center{};
		for (auto i : hitPoints) {
			Center += Points[i];
		}
		Center /= (float)hitCount;

		return { startPos.Extended(Center, range), hitCount };
	}

	//Fixed-Angle MEC algorithm made for league of legends by @RMAN#9401
	inline std::pair<Vector2, int> BestCoveringCone(std::vector<Vector2>& Points, Vector2& startPos, float angle) {
		auto n{ Points.size() };
		if (n == 0)
			return { Vector2(), 0 };
		else if (n == 1)
			return { Points.front(), 1 };

		#pragma region Define Sector All Points Are In
		std::pair<double, Vector2> smallestPolar{ M_PI, Vector2() };
		std::pair<double, Vector2> biggestPolar{ -M_PI, Vector2() };

		float range{ 0.0f };
		for (auto &Point : Points) {
			range = std::max<float>(range, startPos.Distance(Point));

			double polar = (Point - startPos).Polar() * (M_PI / 180.0f);
			if (polar > M_PI) { polar -= (M_PI * 2.0); }

			if (polar > biggestPolar.first) {
				biggestPolar.first = polar;
				biggestPolar.second = Point;
			}

			if (polar < smallestPolar.first) {
				smallestPolar.first = polar;
				smallestPolar.second = Point;
			}
		}

		double angleBetween{ (biggestPolar.first - smallestPolar.first) };
		if (angleBetween > M_PI) { angleBetween = (2 * M_PI) - angleBetween; }

		Vector2 possibleCenter1{ smallestPolar.second.RotatedAroundPoint(startPos, -((float)angleBetween * 0.5f)) };
		Vector2 possibleCenter2{ smallestPolar.second.RotatedAroundPoint(startPos, ((float)angleBetween * 0.5f)) };

		float distFromPoints1{ possibleCenter1.Distance(smallestPolar.second) + possibleCenter1.Distance(biggestPolar.second) };
		float distFromPoints2{ possibleCenter2.Distance(smallestPolar.second) + possibleCenter2.Distance(biggestPolar.second) };

		Vector2 centerPoint{ distFromPoints1 < distFromPoints2 ? possibleCenter1 : possibleCenter2 };
		#pragma endregion

		//All Points are inside this cone
		Geometry::Cone cone(startPos, centerPoint, (float)angleBetween, range);

		float arcLength{range*angle};

		
		//Iterate Cone Sectors to find best solution
		int hitCount{ 0 };
		std::vector<size_t> hitPoints{};
		Vector2 LastPoint{};
		for (auto &sectorPoint : cone.Points) {
			if (sectorPoint != startPos && sectorPoint.Distance(LastPoint) > (arcLength * 0.1f)) {
				LastPoint = sectorPoint;

				auto tmpCount{ 0 };
				std::vector<size_t> tmpPoints{};				
				Geometry::Cone tmpCone(startPos, sectorPoint, angle, range);
				for (size_t i = 0; i < n; ++i) {
					if (tmpCone.IsInside(Points[i])) {
						++tmpCount;
						tmpPoints.push_back(i);
					}
				}

				if (tmpCount > hitCount) {
					hitCount = tmpCount;					
					hitPoints.swap(tmpPoints);
				}
			}
		}

		//Centralize Cone for best HitChance
		Vector2 Center{};
		for (auto i : hitPoints) {
			Center += Points[i];
		}
		Center /= (float)hitCount;

		return { Center, hitCount };
	}
}

inline bool Geometry::Polygon::IsInside(Vector2 point, float extraRadius) {
	if (!IsOutside(point, extraRadius)) {
		return true;
	}

	Geometry::Circle poly(point, extraRadius * 0.6f, 6);
	return IsInside(poly);
}


namespace ClipperWrapper {
	/// Checks if polygons are intersecting
	inline static bool IsIntersects(Paths p1, Paths p2) {
		Clipper c{};
		Paths solution{};
		c.AddPaths(p1, PolyType::ptSubject, true);
		c.AddPaths(p2, PolyType::ptClip, true);

		c.Execute(ClipType::ctIntersection, solution, PolyFillType::pftEvenOdd, PolyFillType::pftEvenOdd);

		return solution.size() != 0;
	}

	inline static bool IsIntersects(Paths p1, std::vector<Paths> p2) {
		Clipper c{};
		Paths solution{};
		c.AddPaths(p1, PolyType::ptSubject, true);

		for (size_t i = 0; i < p2.size(); i++)
			c.AddPaths(p2[i], PolyType::ptClip, true);

		c.Execute(ClipType::ctIntersection, solution, PolyFillType::pftEvenOdd, PolyFillType::pftEvenOdd);

		return solution.size() != 0;
	}

	inline static std::vector<std::vector<IntPoint>> ClipPolygons(std::vector<Geometry::Polygon> polygons) {

		std::vector<std::vector<IntPoint>> subj = {};
		std::vector<std::vector<IntPoint>> clip = {};

		for (auto& polygon : polygons) {
			subj.push_back(polygon.ToClipperPath());
			clip.push_back(polygon.ToClipperPath());
		}

		std::vector<std::vector<IntPoint>> solution{};
		Clipper c{};
		c.AddPaths(subj, PolyType::ptSubject, true);
		c.AddPaths(clip, PolyType::ptClip, true);
		c.Execute(ClipType::ctUnion, solution, PolyFillType::pftPositive, PolyFillType::pftEvenOdd);
		return solution;
	}

	inline static std::vector<std::vector<IntPoint>> ClipPolygons(std::vector<Geometry::Polygon*> polygons) {

		std::vector<std::vector<IntPoint>> subj = {};
		std::vector<std::vector<IntPoint>> clip = {};

		for (auto& polygon : polygons) {
			subj.push_back(polygon->ToClipperPath());
			clip.push_back(polygon->ToClipperPath());
		}

		std::vector<std::vector<IntPoint>> solution{};
		Clipper c{};
		c.AddPaths(subj, PolyType::ptSubject, true);
		c.AddPaths(clip, PolyType::ptClip, true);
		c.Execute(ClipType::ctUnion, solution, PolyFillType::pftPositive, PolyFillType::pftEvenOdd);
		return solution;
	}

	/// Defines Line Polygon
	inline static Geometry::Polygon DefineLine(Vector2 start, Vector2 end, float scale) {
		return Geometry::Line(start, end, scale);
	}

	/// Defines Rectangle Polygon
	inline static Geometry::Polygon DefineRectangle(Vector2 start, Vector2 end, float scale) {
		return Geometry::Rectangle(start, end, scale);
	}

	/// Defines Circle Polygon
	inline static Geometry::Polygon DefineCircle(Vector2 c, float r) {
		return Geometry::Circle(c, r);
	}

	/// Defines Ring Polygon
	inline static Geometry::Polygon DefineRing(Vector2 c, float r1, float r2) {
		return Geometry::Ring(c, r1, r2);
	}

	/// Defines Sector Polygon
	inline static Geometry::Polygon DefineSector(Vector2 center, Vector2 direction, float angle, float radius) {
		return Geometry::Sector(center, direction, angle, radius);
	}

	/// Defines Arc Polygon
	inline static Geometry::Polygon DefineArc(Vector2 start, Vector2 direction, float angle, float radius, int quality = 20) {
		return Geometry::Arc(start, direction, angle, radius, quality);
	}

	// Defines Cone Polygon
	inline static Geometry::Polygon DefineCone(Vector2 startPosition, Vector2 endPosition, float coneAngle, float range) {
		return Geometry::Cone(startPosition, endPosition, coneAngle, range);		
	}

	//I felt like killing myself after this so if you use it give credits to RMAN thx
	inline static Geometry::Polygon DefineDianaArc(Vector2 sourcePosition, Vector2 cursorPos, float width, Vector3 misPos = {0,0,0}) {
		//Find center of gyration
		double totalDist{ cursorPos.Distance(sourcePosition) };
		double d = totalDist * 0.5 * (1 - std::pow(cursorPos.Length(), 2)) / std::pow(cursorPos.Length(), 2);
		double c1 = ((cursorPos.y - sourcePosition.y) / totalDist) * d + (sourcePosition.x + cursorPos.x) / 2;
		double c2 = ((sourcePosition.x - cursorPos.x) / totalDist) * d + (sourcePosition.y + cursorPos.y) / 2;
		Vector2 centerPoint{ (float)c1, (float)c2 };

		//Get Angles Between CoG, Start and End pos
		float angleToEnd{ std::atan2(sourcePosition.y - centerPoint.y, sourcePosition.x - centerPoint.x) };
		float angleToPlayer{ std::atan2(cursorPos.y - centerPoint.y, cursorPos.x - centerPoint.x) };
		double angleBetween{ std::fmod((double)angleToEnd - (double)angleToPlayer + 5.0 * M_PI, M_PI * 2.0) - M_PI };

		//Arc Data
		Geometry::Polygon arc{};
		float radius = centerPoint.Distance(sourcePosition) - width * 0.5f;
		auto innerRadius = (radius) / (float)std::cos(2 * M_PI / 20.0f);
		auto outerRadius = (radius + width) / (float)std::cos(2 * M_PI / 20.0f);
		auto midPos{ ((sourcePosition + cursorPos)*0.5f) };
		auto dir{ (midPos - centerPoint) };

		std::vector<Vector2> backwardTrip{};
		auto side1 = dir.Rotated(-(float)angleBetween * 0.5f);
		for (int i = 0; i <= 20.0f; i++) {
			auto cDirection = side1.Rotated((float)i * (float)angleBetween / 20.0f).Normalized();

			Vector2 tmpPos{ centerPoint.x + innerRadius * cDirection.x, centerPoint.y + innerRadius * cDirection.y };
			Vector2 extendedTmp{ centerPoint.Extended(tmpPos, radius) };

			//Cuts the polygon right behind the missile
			if (misPos.IsValid() && extendedTmp.Distance(cursorPos) > misPos.Distance(cursorPos)) { continue; }
			arc.Add(extendedTmp);

			Vector2 tmpPos2{ centerPoint.x + outerRadius * cDirection.x, centerPoint.y + outerRadius * cDirection.y };
			Vector2 extendedTmp2{ centerPoint.Extended(tmpPos2, radius + width) };
			backwardTrip.push_back(extendedTmp2);
		}

		for (auto it = backwardTrip.rbegin(); it < backwardTrip.rend(); ++it) { arc.Add(*it); }
		return arc;
	}

	/// Creates Paths from Polygon for Clipper
	inline static Paths MakePaths(Geometry::Polygon& plist) {
		return Paths{ plist.ToClipperPath() };
	}
	inline static Paths MakePaths(Geometry::Polygon* plist) {
		return Paths{ plist->ToClipperPath() };
	}

	/// Creates Paths from Polygon list for Clipper
	inline static Paths MakePaths(std::vector<Geometry::Polygon>& plist) {
		Paths ps;
		ps.reserve(plist.size());
		for (size_t i = 0; i < plist.size(); i++)
			ps.push_back(plist[i].ToClipperPath());
		return ps;
	}
	inline static Paths MakePaths(std::vector<Geometry::Polygon*>& plist) {
		Paths ps;
		ps.reserve(plist.size());
		for (size_t i = 0; i < plist.size(); i++) 		
			ps.push_back(plist[i]->ToClipperPath());
					
		return ps;
	}
}

inline bool Geometry::Polygon::Intersects(Geometry::Polygon& other) {
	auto p1{ ClipperWrapper::MakePaths(this) };
	auto p2{ ClipperWrapper::MakePaths(other) };
	return ClipperWrapper::IsIntersects(p1, p2);
}
