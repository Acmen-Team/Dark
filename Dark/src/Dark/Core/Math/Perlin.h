#include <Math/Perlin.h>

#include <Math/Math.h>

using namespace H3D::Math;
using namespace glm;
using namespace std;

float Perlin::Noise(const vec3 & p){
	// pf ΪС������
	vec3 pf = p - floor(p);
	// pi Ϊ��������
	ivec3 pi = floor(p);
	// 8��λ�õ�����
	// ��λ������8������, ÿ������һ���������
	vec3 c[2][2][2];
	for (int dx = 0; dx < 2; dx++) {
		for (int dy = 0; dy < 2; dy++) {
			for (int dz = 0; dz < 2; dz++) {
				size_t idx = permuteX[(pi.x + dx) & 255]
					^ permuteY[(pi.y + dy) & 255]
					^ permuteZ[(pi.z + dz) & 255];
				c[dx][dy][dz] = randVec[idx];
			}
		}
	}
	return PerlinInterp(c, pf.x, pf.y, pf.z);
}

float Perlin::PerlinInterp(const vec3 c[2][2][2], float u, float v, float w) {
	float uu = u * u*(3 - 2 * u);
	float vv = v * v*(3 - 2 * v);
	float ww = w * w*(3 - 2 * w);
	float sum = 0;
	for (size_t i = 0; i < 2; i++) {
		for (size_t j = 0; j < 2; j++) {
			for (size_t k = 0; k < 2; k++) {
				// ���� �� ��ֵ�� ������
				vec3 weightVec(u - i, v - j, w - k);
				// Ȩֵ Ϊ ��ֵ�� �� [����Խ�] �� xyz���� �� ����(�Ǹ�) �ĳ˻�
				//vec3 absWeightVec = abs(vec3(u-(1-i),v-(1-j),w-(1-k)));
				//float weight = absWeightVec.x * absWeightVec.y * absWeightVec.z;
				float weight = (i*uu + (1 - i)*(1 - uu))
					* (j*vv + (1 - j)*(1 - vv))
					* (k*ww + (1 - k)*(1 - ww));
				// ��Ȩ���, ��Ϊ���, �Ӷ��ﵽ����ֵת�Ƶ�����(������������)
				sum += weight * dot(c[i][j][k], weightVec);
			}
		}
	}
	return sum;
}

float Perlin::Turb(const vec3 & p, size_t depth){
	float sum = 0;
	vec3 curP = p;
	float weight = 1.0;
	for (size_t i = 0; i < depth; i++) {
		sum += weight * Noise(curP);
		// weight = pow(0.5, i);
		weight *= 0.5;
		// curP = p * pow(2, i);
		curP *= 2;
	}
	//float abs --> fabs
	return fabs(sum);
}

vector<size_t> Perlin::GenPermute(size_t n) {
	vector<size_t> rst(n);
	for (size_t i = 0; i < n; i++)
		rst[i] = i;

	Math::Permute(rst);
	return rst;
}

vector<vec3> Perlin::GenRandVec(size_t n) {
	vector<vec3> rst(n);
	for (size_t i = 0; i < n; ++i)
		rst[i] = RandInSphere();

	return rst;
}


vector<vec3> Perlin::randVec = Perlin::GenRandVec(256);
vector<size_t> Perlin::permuteX = Perlin::GenPermute(256);
vector<size_t> Perlin::permuteY = Perlin::GenPermute(256);
vector<size_t> Perlin::permuteZ = Perlin::GenPermute(256);