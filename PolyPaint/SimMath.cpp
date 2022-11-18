
// quick maths :

double Det(double a, double b, double c, double d)
{
    return a * d - b * c;
}


double* VecProduct(double* vec1, double* vec2) {
    return new double[2] {
        0, 0
    };
}



double dot(float* vec1, double* vec2) {
    return vec1[0] * vec2[0] + vec1[1] * vec2[1];
}



float norme(float* vec1) {
    return sqrtf((vec1[0] * vec1[0]) + (vec1[1] * vec1[1]));
}

bool isContainOnline(float* a, float* c, float* b) {
    float* ab = new float[2] { b[0] - a[0], b[1] - a[1]};
    float* ac = new float[2] { c[0] - a[0], c[1] - a[1]};
    float* cb = new float[2] { b[0] - c[0], b[1] - c[1]};

    float acn = (norme(ac));
    float abn = (norme(ab));
    float cbn = (norme(cb));
    float v = (acn + cbn) - abn;
    return (v < 0.05);
}




// For matrix

// https://membres-ljk.imag.fr/Bernard.Ycart/mel/cm/node7.html#:~:text=Pour%20inverser%20une%20matrice%20%C3%A0,les%20coefficients%20par%20le%20d%C3%A9terminant

float* inverse(float a, float b, float c, float d) {
    float v = 1.0f / ((a * d) - (b * c));

    return  new float[4] {
        v* d, v * -b,
            v * -c, v* a
    };
}

double* multiply(float* a, int row1, int col1, float* b, int row2, int col2)
{

    int size = row1 * col2;
    double* d = new double[size];
    for (int i = 0; i < row1; i++) {
        for (int j = 0; j < col2; j++) {
            double sum = 0;
            for (int k = 0; k < col1; k++)
                sum = sum + a[i * col1 + k] * b[k * col2 + j];
            d[i * col2 + j] = sum;
        }
    }

    return d;
}

// some Function for need for clipping

float* intersection(float* currentPoint, float* nextPoint, float* f1, float* f2, bool strict = false) {

    float* A = new float[4] {
        (float)(nextPoint[0] - currentPoint[0]), (float)(f1[0] - f2[0]),
            (float)(nextPoint[1] - currentPoint[1]), (float)(f1[1] - f2[1]),
    };


    float* b = new float[2] {
        (float)f1[0] - currentPoint[0],
            (float)f1[1] - currentPoint[1]
    };


    if (Det(A[0], A[1], A[2], A[3]) == 0.0) {
        return new float[2] {0, 0};
    }

    A = inverse(A[0], A[1], A[2], A[3]);

    double* d = multiply(A, 2, 2, b, 2, 1);

    if (strict && ((d[0] > 1 || d[0] < 0) || (d[1] > 1 || d[1] < 0))) {
        return new float[2] {0, 0};
    }


    if ((d[0] > 1 || d[0] < 0)) {
        return new float[2] {0, 0};
    }


    float xm = (float)(((1.0 - d[0]) * currentPoint[0]) + (d[0] * nextPoint[0]));
    float ym = (float)(((1.0 - d[0]) * currentPoint[1]) + (d[0] * nextPoint[1]));

    float* v = new float[2] { xm, ym };

    return v;
}

bool coupe(float* currentPoint, float* nextPoint, float* f1, float* f2, bool strict = false) {


    float* d = intersection(currentPoint, nextPoint, f1, f2, strict);

    if (d[0] == 0 && d[1] == 0) {
        return false;
    }

    return true;
}



int isInterseptCriticalSegment(float* point, std::vector<float*>  criticalPoints) {

    for (int i = 0; i <= criticalPoints.size() - 2; i += 2) {
        std::cout << std::endl;
        std::cout << " c " << i << std::endl;
        float* A = new float[2] {criticalPoints[i][0], criticalPoints[i][1]};
        float* B = new float[2] {criticalPoints[i][2], criticalPoints[i][3]};

        float* C = new float[2] {criticalPoints[i + 1][0], criticalPoints[i + 1][1]};
        float* D = new float[2] {criticalPoints[i + 1][2], criticalPoints[i + 1][3]};

        std::cout << " value " << isContainOnline(A, point, B) << std::endl;

        if (isContainOnline(A, point, B) || isContainOnline(C, point, D)) {
            return i;
        }

    }

    return -1;
}

bool containPoint(float* p, std::vector<float> lst) {
    bool trouver = false;
    int i = 0;
    while (!trouver && i < lst.size() - 1) {
        if (lst[i] == p[0] && lst[i + 1] == p[1]) {
            trouver = true;

        }
        else {
            i++;
        }
    }

    return trouver;
}




bool isVisible(float* Point, float* c, float* d, bool orientation)
{
    float* v1 = new float[2] { Point[0] - c[0], Point[1] - c[1] };
    float* v2 = new float[2] {d[0] - c[0], d[1] - c[1]};

    float value = v1[0] * v2[1] - v1[1] * v2[0];


    if ((value <= 0 && orientation) || (value >= 0 && !orientation)) {
        return true;
    }

    return false;
}


bool isClockWise(std::vector<float>lst) {
    double sum = 0.0;
    float* S;
    float* F;



    S = new float[2] {lst[0], lst[1]};

    for (int i = 2; i <= lst.size() - 1; i += 2) {

        int idx = (i + 1) % lst.size();

        sum += (lst[i] - S[0]) * (lst[idx] + S[1]);

        // std::cout <<  "(" << lst[i]<<"-" << S[0]<<")("<<lst[i+1]<<"+" <<S[1]<<") ="<< sum << std::endl;

        S = new float[2] {lst[i], lst[idx]};
    }

    //  std::cout << (sum <0) << std::endl;

    return sum < 0;
}



// me dis dis le triangle est trigo ou non
bool CW3PTS(float* A, float* B, float* C) {
  
    float x1 = A[0];
    float y1 = A[1];

    float x2 = B[0];
    float y2 = B[1];

    float x3 = C[0];
    float y3 = C[1];

    float v = (y2 - y1) * (x3 - x2) - (y3 - y2) * (x2 - x1);
    return (v < 0);
}

bool isInside(float* point, std::vector<float> poly) {

    double sum = 0;

    bool clockWise = isClockWise(poly);


    for (int i = 0; i <= poly.size() - 2; i += 2) {
        float* A = new float[2] { poly[i] - point[0], poly[i + 1] - point[1] };
        int idx = (i + 2) % poly.size();
        float* B = new float[2] {poly[idx] - point[0], poly[idx + 1] - point[1]};


        bool b = CW3PTS(new float[2] {poly[i], poly[i + 1] }, point, new float[2] {poly[idx], poly[idx + 1] });

        double v = (A[0] * B[0] + A[1] * B[1]);
        double s = (acosf(v / ((norme(A) * norme(B)))) * (180 / M_PI));

        if (b != clockWise) {

            s = s * -1;
        }

        sum += round(s);
    }


    return (abs(sum) >= 340);


}










// Clipping

std::vector<float> SutherLandHodman(std::vector<float> PL, std::vector<float> PW) {
    int i, j;
    int N1 = PL.size();
    int N2 = 0;
    int N3 = PW.size();
    float* S;
    float* F;
    float* I;
    std::vector<float> PS;

    bool Orientation = isClockWise(PW);

    for (i = 0; i < N3 - 2; i += 2) {
        N2 = 0;
        PS = std::vector<float>();
        float* Fi = new float[2] {PW[i], PW[i + 1]};
        float* Fi1 = new float[2] {PW[i + 2], PW[i + 3]};

        for (j = 0; j < N1; j += 2) {

            float* Pj = new float[2] {PL[j], PL[j + 1]};

            if (j == 0) {
                F = Pj;
            }
            else {

                if (i + 3 > N3) {
                    std::cout << "ERREUR i+3 > N3 191 >" << i + 3 << ">" << N3 << std::endl;
                }

                if (coupe(S, Pj, Fi, Fi1)) {
                    I = intersection(S, Pj, Fi, Fi1);
                    PS.push_back(I[0]);
                    PS.push_back(I[1]);
                    N2 += 2;
                }
            }
            S = Pj;
            if (isVisible(S, Fi, Fi1, Orientation)) {
                // std::cout << j << std::endl;
                PS.push_back(S[0]);
                PS.push_back(S[1]);
                N2 += 2;
            }
        }
        if (N2 > 0) {

            if (coupe(S, F, Fi, Fi1)) {
                // std::cout << "je passe je crois" << std::endl;
                I = intersection(S, F, Fi, Fi1);



                if (I[0] - F[0] < 0.01 && I[1] - F[1] < .01) {
                    // std::cout << "same point" << std::endl;
                }

                PS.push_back(I[0]);
                PS.push_back(I[1]);
                N2 += 2;
            }
            PL = PS;
            N1 = N2;

        }

    }
    return PS;
}

bool CyrusBeck(float* A, float* B, std::vector<float> PW) {

    std::vector<float*> normal;
    std::vector<float>n;
    std::vector<float> _PW = PW;
    // std::cout << "===" << std::endl;
     // d�terminer les normals

    float* S = new float[2] {PW[0], PW[1]};

    bool b = isClockWise(PW);

    _PW.push_back(S[0]);
    _PW.push_back(S[1]);
    for (int i = 2; i <= _PW.size() - 2; i += 2) {

        int idx = (i + 1) % _PW.size();


        if (true) {



            float xm = -(_PW[i + 1] - S[1]);
            float ym = (_PW[i] - S[0]);

            // std::cout << xm <<" "<< ym << std::endl;
            normal.push_back(new float [2] {xm, ym});
        }
        else {

            float xm = _PW[i + 1] - S[1];
            float ym = -(_PW[i] - S[0]);
            // std::cout << xm <<" "<< ym << std::endl;
            normal.push_back(new float [2] {xm, ym});
        }


        S = new float[2] {_PW[i], _PW[idx]};
    }


    // le vrai code


    float t, tinf, tsup;
    float DX, DY, WN, DN;
    float* C;
    int i, Nbseg;

    tinf = std::numeric_limits<float>::min();
    tsup = std::numeric_limits<float>::max();

    DX = B[0] - A[0];
    DY = B[1] - A[1];

    Nbseg = PW.size() - 1;
    int currentNormal = 0;
    for (int i = 0; i < _PW.size() - 2; i += 2) {


        DN = DX * normal[currentNormal][0] + DY * normal[currentNormal][1];
        WN = (A[0] - _PW[i]) * normal[currentNormal][0] + (A[1] - _PW[i + 1]) * normal[currentNormal][1];
        if (DN == 0) {
            return WN >= 0;
        }
        else {
            t = -(WN) / (DN);
            if (DN > 0) {
                if (t > tinf) {
                    tinf = t;
                }
            }
            else {
                if (t < tsup) {
                    tsup = t;
                }
            }
        }

        currentNormal++;
    }
    if (tinf < tsup) {
        if ((tinf < 0) && (tsup > 1)) {
            return true;
        }
        else {
            if ((tinf > 1) || (tsup) < 0) {
                return false;
            }
            else if ((tinf < 0)) {
                tinf = 0;
            }
            else if (tsup > 1) {
                tsup = 1;
            }

            B[0] = A[0] + DX * tsup;
            B[1] = A[1] + DY * tsup;
            A[0] = A[0] + DX * tinf;
            A[1] = A[1] + DY * tinf;

            return true;
        }
    }

    return false;
}