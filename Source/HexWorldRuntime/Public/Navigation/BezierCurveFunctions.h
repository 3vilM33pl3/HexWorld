#pragma once

class BezierCurveFunctions
{
public:
	BezierCurveFunctions() {};
	~BezierCurveFunctions() {};

	static FVector CubicBezierLocation(const FVector P0, const FVector P1, const FVector P2, const FVector P3, const float T)
	{
		FVector Rlocation = pow(1.0 - T, 3) * P0 + 3 * pow(1.0 - T, 2) * T * P1 + 3*(1.0 - T)*pow(T,2)*P2 + pow(T, 3)*P3;
		Rlocation.Z = P0.Z;
		return Rlocation;
	}

	static FVector CubicBezierCurveDerivative(const FVector P0, const FVector P1, const FVector P2, const FVector P3, const float T)
	{
		FVector Rlocation = 3 * pow(1.0 - T, 2) * (P1 - P0) + 6 * (1 - T) * T * (P2 - P1) + 3 * pow(T, 2) * (P3 - P2);
		Rlocation.Z = P0.Z;
		return Rlocation;
	}

	static std::vector<FVector> CubicBezierCurve(const FVector P0, const FVector P1, const FVector P2, const FVector P3, const float T)
	{
		std::vector<FVector> BezierCurvePoints;
		for (float i = 0; i <= 1; i=i+T)
		{
			FVector P = CubicBezierLocation(P0, P1, P2, P3, i);
			BezierCurvePoints.push_back(P);
		}
		return BezierCurvePoints;
	}

	static float CubicBezierCurveLength(std::vector<FVector> BezierCurvePoints)
	{
		float Length = 0;
		if (BezierCurvePoints.size() - 2 >= 0)
		{
			for (int i = 0; i < BezierCurvePoints.size() - 2; i++)
			{
				Length = +FVector::Distance(BezierCurvePoints.at(i), BezierCurvePoints.at(i + 1));
			}
		}
		return abs(Length);
	}

	static float CubicBezierLengthEstimate(FVector P0, FVector P1, FVector P2, FVector P3, float SegmentInterval)
	{
		const auto PointsApproximation = BezierCurveFunctions::CubicBezierCurve(P0, P1, P2, P3, 1.0 / 50);
		const auto LengthApproximation = BezierCurveFunctions::CubicBezierCurveLength(PointsApproximation);
		if (LengthApproximation > 0) {
			const auto SecondLengthApproximation = BezierCurveFunctions::CubicBezierCurve(P0, P1, P2, P3, (1.0 / LengthApproximation) * SegmentInterval / 10);
			return BezierCurveFunctions::CubicBezierCurveLength(SecondLengthApproximation);
		}
		else
		{
			return 0;
		}
	}
	
};
