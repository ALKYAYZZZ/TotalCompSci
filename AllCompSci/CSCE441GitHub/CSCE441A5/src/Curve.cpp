#include "Curve.h"
//Constructor
Curve::Curve() {

}
//Select functions
//Load Corresponding Catmull Matrix for the B vector and draw
//Curves based off of keyframe positions.
void Curve::build_curve(std::vector<glm::vec3> cps) {
	int ncps = (int)cps.size();
	glPointSize(5.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_POINTS);
	for (int i = 0; i < ncps; ++i) {
		glVertex3f(cps[i].x, cps[i].y, cps[i].z);
	}
	glEnd();

	Bcr[0] = glm::vec4(0.0f, 2.0f, 0.0f, 0.0f);
	Bcr[1] = glm::vec4(-1.0f, 0.0f, 1.0f, 0.0f);
	Bcr[2] = glm::vec4(2.0f, -5.0f, 4.0f, -1.0f);
	Bcr[3] = glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f);
	Bcr *= 0.5f;

	Bb[0] = glm::vec4(1.0f, 4.0f, 1.0f, 0.0f);
	Bb[1] = glm::vec4(-3.0f, 0.0f, 3.0f, 0.0f);
	Bb[2] = glm::vec4(3.0f, -6.0f, 3.0f, 0.0f);
	Bb[3] = glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f);
	Bb /= 6.0f;
		// Draw spline
	glm::mat4 B = Bcr;
		glLineWidth(3.0f);
		for (int k = 0; k < 9 - 3; ++k) {
			glm::mat4 Gk;
			for (int i = 0; i < 4; ++i) {
				Gk[i] = glm::vec4(cps[k + i], 0.0f);
			}
			int n = 32; // curve discretization
			glBegin(GL_LINE_STRIP);
			if (k % 2 == 0) {
				// Even segment color
				glColor3f(0.0f, 1.0f, 0.0f);
			}
			else {
				// Odd segment color
				glColor3f(0.0f, 0.0f, 1.0f);
			}
			for (int i = 0; i < n; ++i) {
				// u goes from 0 to 1 within this segment
				float u = i / (n - 1.0f);
				// Compute spline point at u
				glm::vec4 uVec(1.0f, u, u*u, u*u*u);
				glm::vec3 P(Gk * (B * uVec));
				glVertex3fv(&P[0]);
			}
			glEnd();
		}
}


//Build table for u and s values based off of keyframe value and interpolated
//helicopter location.
void Curve::buildTable(std::vector<glm::vec3> cps, std::vector<glm::quat> qpos)
{
	usTable.clear();
	int ncps = (int)cps.size();
	glm::mat4 B = Bcr;
	float u = 0.0f;
	float s = 0.0f;
	float prevu = 0.0f;
	int count = -1;
	glm::vec3 puaprev = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 Gk, Gquat;
	for (int i = 0; i <= (10 * (ncps - 3)); i++) {
		if (i % 10 == 0 && (i != (10 * (ncps - 3)))) {
			count++;
			for (int j = 0; j < 4; ++j) {
				Gk[j] = glm::vec4(cps[count + j], 0.0f);
				Gquat[j] = glm::vec4(qpos[count + j].x, qpos[count + j].y, qpos[count + j].z, qpos[count + j].w);
			}
		}
		float tempu = u - count;
		glm::vec4 u_vec = glm::vec4(1.0f, tempu, tempu*tempu, tempu*tempu*tempu);
		glm::vec3 pub = Gk * (B * u_vec);
		if (i == 0) {
			s = 0.0f;
		}
		else {
			s += glm::length(pub - puaprev);
		}
		usTable.push_back(std::make_pair(u, s));
		puaprev = pub;
		u = u + .1f;
		if (s > smax) {
			smax = s;
		}
		if (u > umax) {
			umax = u;
		}
	}


}


//Convert s values to u vales by using simple estimation formula
float Curve::s2u(float s)
{
	// INSERT CODE HERE
	float s0, s1, u0, u1;
	if (usTable.size() != 0) {
		for (size_t i = 0; i < usTable.size(); i++) {
			if (usTable[i].second > s) {
				s1 = usTable[i].second;
				s0 = usTable[(i - 1)].second;
				u1 = usTable[i].first;
				u0 = usTable[(i - 1)].first;
				break;
			}
		}
	}
	float alpha = (s - s0) / (s1 - s0);
	float u_final = ((1 - alpha) * u0) + (alpha * u1);
	return u_final;
}

//Convert time values to s values 
float Curve::t2s(float t) {
	float tNorm = std::fmod(t, tmax) / tmax;
	float sNorm = tNorm;
	float s = smax * sNorm;
	return s;
}

//Return smax value
float Curve::get_smax() {
	return smax;
}

//Return umax value
float Curve::get_umarker() {
	return umax;
}

//Return helicopter matrix to use for first person view of helicopter
glm::mat4 Curve::get_mainmatrix(std::vector<glm::vec3> cps, std::vector<glm::quat> qpos, float u) {
	glm::mat4 Gk, Gquat;
	glm::mat4 B = Bcr;
	int count = int(u);
	for (int j = 0; j < 4; ++j) {
		Gk[j] = glm::vec4(cps[count + j], 0.0f);
		Gquat[j] = glm::vec4(qpos[count + j].x, qpos[count + j].y, qpos[count + j].z, qpos[count + j].w);
	}
	float tempu = u - count;
	glm::vec4 u_vec = glm::vec4(1.0f, tempu, tempu*tempu, tempu*tempu*tempu);
	glm::vec3 pub = Gk * (B * u_vec);
	glm::vec4 puq = Gquat * (B * u_vec);
	glm::quat qps(puq[3], puq[0], puq[1], puq[2]);
	main_matrix = glm::mat4_cast(glm::normalize(qps));
	main_matrix[3] = glm::vec4(pub.x, pub.y, pub.z, 1.0f);
	return main_matrix;
}
//Increase tmax
void Curve::inc_tmax() {
	tmax = tmax + 1.0f;
}
//Decrease tmax
void Curve::dec_tmax() {
	if (tmax - 1.0f <= 1) {
		tmax = 1.0f;
	}
	else {
		tmax = tmax - 1.0f;
	}

}
