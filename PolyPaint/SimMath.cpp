
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
    float S[2];
    float* F;



    S[0] = lst[0];
    S[1] = lst[1];

    for (int i = 2; i <= lst.size() - 1; i += 2) {

        int idx = (i + 1) % lst.size();

        sum += (lst[i] - S[0]) * (lst[idx] + S[1]);

        // std::cout <<  "(" << lst[i]<<"-" << S[0]<<")("<<lst[i+1]<<"+" <<S[1]<<") ="<< sum << std::endl;

        S[0] = lst[i]; 
        S[1] = lst[idx];
    }

    //  std::cout << (sum <0) << std::endl;

    return sum < 0;
}

bool isClockWisePoly(std::vector<float*>lst) {
    double sum = 0.0;
    float* S;
    float* F;

    S = lst[0];

    for (int i = 2; i < lst.size(); i++) {

        float* Point = lst[i];

        sum += (Point[0] - S[0]) * (Point[1] + S[1]);

        S = Point;
    }
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

bool isInside(float point[2], std::vector<float> poly) {

    double sum = 0;

    bool clockWise = isClockWise(poly);

    float A[2];
    float B[2];

    float tmp[2];
    float tmp2[2];


    for (int i = 0; i <= poly.size() - 2; i += 2) {
        A[0] = poly[i] - point[0];
        A[1] = poly[i + 1] - point[1];
        int idx = (i + 2) % poly.size();
        B[0] = poly[idx] - point[0];
        B[1] = poly[idx + 1] - point[1];

        tmp[0] = poly[i];
        tmp[1] = poly[i + 1];

        tmp2[0] = poly[idx];
        tmp2[1] = poly[idx + 1];

        bool b = CW3PTS(tmp, point, tmp2);

        double v = (A[0] * B[0] + A[1] * B[1]);
        double s = (acosf(v / ((norme(A) * norme(B)))) * (180 / M_PI));

        if (b != clockWise) {

            s = s * -1;
        }

        sum += round(s);
    }


    return (abs(sum) >= 340);


}

#include "Polygone.h"



int isInterseptCriticalSegment(float* point, PolySeg poly) {
    for (int i = 0; i < poly.GetSegments().size(); i++) {


        if (isContainOnline(poly[i].Seg1.A, point, poly[i].Seg1.B) || isContainOnline(poly[i].Seg2.A, point, poly[i].Seg2.B)) {
            return i;
        }
    }

    return -1;
}


// Clipping

std::vector<float*> SutherLandHodman(Entity Cpolly, Entity Window) {
    
    Entity PS;
    Entity PW = Window;
    Entity PL = Cpolly;

    int i, j;
    int N1 = PL.getPoints().size();
    int N2 = 0;
    int N3 = PW.getPoints().size();
    float* S;
    float* F;
    float* I;


    bool Orientation = isClockWise(PW.getFlatVector());
  
    for (i = 0; i < N3-1; i ++) {
        N2 = 0;
        PS = std::vector<float*>();
        float* Fi = PW[i];
        float* Fi1 = PW[i + 1];

        for (j = 0; j < N1; j++) {

            float* Pj = PL[j];

            if (j == 0) {
                F = Pj;
            }
            else {


                if (coupe(S, Pj, Fi, Fi1)) {
                    I = intersection(S, Pj, Fi, Fi1);
                    PS.push_back(I);
 
                    N2++;
                }
            }
            S = Pj;
            if (isVisible(S, Fi, Fi1, Orientation)) {
                // std::cout << j << std::endl;
                PS.push_back(S);
                N2 ++;
            }
        }
        if (N2 > 0) {

            if (coupe(S, F, Fi, Fi1)) {
                // std::cout << "je passe je crois" << std::endl;
                I = intersection(S, F, Fi, Fi1);



                if (I[0] - F[0] < 0.01 && I[1] - F[1] < .01) {
                    // std::cout << "same point" << std::endl;
                }

                PS.push_back(I);
                N2 ++;
            }
            PL.setPoints(PS.getPoints());
            N1 = N2;

        }

    }

    return PS.getPoints();
}

bool CyrusBeck(float* A, float* B, std::vector<float> PW) {

    std::vector<float*> normal;
    std::vector<float>n;
    std::vector<float> _PW = PW;
    // std::cout << "===" << std::endl;
     // déterminer les normals

    float* S = new float[2] {PW[0], PW[1]};


    _PW.push_back(S[0]);
    _PW.push_back(S[1]);

    bool b = isClockWise(_PW);


    for (int i = 2; i <= _PW.size() - 2; i += 2) {

        int idx = (i + 1) % _PW.size();
        float xm = (_PW[i + 1] - S[1]);
        float ym = (_PW[i] - S[0]);

        if (b) {


         
            // std::cout << xm <<" "<< ym << std::endl;
            normal.push_back(new float [2] {-xm, ym});
        }
        else {

            
            // std::cout << xm <<" "<< ym << std::endl;
            normal.push_back(new float [2] {xm, -ym});
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

std::vector<float>  GenerateCyrusBeck(std::vector<float>lstPointsFenetre, std::vector<float>lstPoints) {
    std::vector<float> newPL;
    std::vector<float> tmpPL = std::vector<float>(lstPoints);
    float* S = new float[2] {tmpPL[0], tmpPL[1]};
    std::vector<float*> criticalSeg;

    float* der = new float[2] {lstPointsFenetre[lstPointsFenetre.size() - 2], lstPointsFenetre[lstPointsFenetre.size() - 1]};

    for (int k = 0; k <= lstPointsFenetre.size() - 2; k += 2) {

        bool b = isInside(new float[2] {lstPointsFenetre[k], lstPointsFenetre[k + 1]}, lstPoints);

        if (b) {

            int c = k - 2;
            if (c < 0) c = lstPointsFenetre.size() - 2;
            int idx = (k + 2) % lstPointsFenetre.size();
            criticalSeg.push_back(new float[4] {lstPointsFenetre[k], lstPointsFenetre[k + 1], lstPointsFenetre[idx], lstPointsFenetre[idx + 1]});
            criticalSeg.push_back(new float[4] {lstPointsFenetre[k], lstPointsFenetre[k + 1], lstPointsFenetre[c], lstPointsFenetre[c + 1]});


        }

    }
    //  std::cout << "number of size : "<< criticalSeg.size() << std::endl;
    tmpPL.push_back(S[0]);
    tmpPL.push_back(S[1]);

    float* B;
    float* A;
    for (int i = 2; i <= tmpPL.size() - 2; i += 2) {


        B = new float[2] {tmpPL[i], tmpPL[i + 1]};
        if (CyrusBeck(S, B, lstPointsFenetre)) {
            newPL.push_back(S[0]);
            newPL.push_back(S[1]);
            newPL.push_back(B[0]);
            newPL.push_back(B[1]);
        }


        S = new float[2] {tmpPL[i], tmpPL[i + 1]};
    }

    if (criticalSeg.size() > 0) {



        tmpPL = newPL;
        newPL = std::vector<float>();

        int c = tmpPL.size();
        if (tmpPL.size() == 0 && (criticalSeg.size()) == lstPointsFenetre.size())
        {
            tmpPL = lstPointsFenetre;
        }

        for (int i = 2; i < tmpPL.size() ; i += 2) {

            if (criticalSeg.size() <= 0) {
                break;
            }


            float tmp[2];
            tmp[0] = tmpPL[i];
            tmp[1] = tmpPL[i+1];

            int k = isInterseptCriticalSegment(tmp, criticalSeg);
            int j = (i) % tmpPL.size();
            if (k < 0) {

            }
            else {

                int idx = (j + 2) % tmpPL.size();



                int m = isInterseptCriticalSegment(new float[2] {tmpPL[idx], tmpPL[(idx + 1)]}, criticalSeg);

                if ((k != m) || (k == m && idx == 0))
                {

                    if (!containPoint(new float[2] {criticalSeg[k][0], criticalSeg[k][1]}, tmpPL)) {
                        tmpPL.push_back(criticalSeg[k][0]);
                        tmpPL.push_back(criticalSeg[k][1]);
                        criticalSeg.erase(criticalSeg.begin() + k);
                        criticalSeg.erase(criticalSeg.begin() + k);

                    }

                }
            }

        }

        newPL = tmpPL;
    }


    return newPL;
}


//  REMPLISSAGE ==================================

int gf = 0;

unsigned char data[3];

float  getPixelColor(int x, int y,int width,int height,float* lstColor) {
    
    float ypos = height - y;


    glReadPixels(x, ypos, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &data);

    float c= 0.0;
    if (y < height && x < width && x >= 0 && y >=0) {
       c= lstColor[(y * width) + x];
    }
    else {
        c = 1.0;
    }
  
    float r, g, b;
    r = (int)data[0] / 255.0;
    g = (int)data[1] / 255.0;
    b = (int)data[2] / 255.0;
    float v = r;
    if (g > r) {
        v = g;
    }
    
    if (b > v) {
        v = b;
    }


    
   // std::cout << v << " x " << x << std::endl;

    if (c > 0.5 || v > 0.5) {
        return 1.0;
    }

 

    return 0.0;
}

float* FillFormConex(float x, float y, int width, int height, float* lstOfColor, float CC = 1.0, float CR = 1.0) {

    if (x >= width  || x < 0 || y >= height  || y < 0 || gf > 500000) {
        return lstOfColor;
    }

    gf++;

    float CP = getPixelColor(x , y, width, height, lstOfColor);


    if ((CP != CC) && (CP != CR)) {

        lstOfColor[(width * (int)y) + (int)x] = 1.0;

        FillFormConex(x, y - 1, width, height, lstOfColor, CC, CR);
        FillFormConex(x, y + 1, width, height, lstOfColor, CC, CR);
        FillFormConex(x - 1, y, width, height, lstOfColor, CC, CR);
        FillFormConex(x + 1, y, width, height, lstOfColor, CC, CR);

        
    }

  

    return lstOfColor;
}



float* FillStackUpdate( int width, int height, float* lstColor, std::stack<int*> &pile, float CC = 1.0, float CR = 1.0) {

  
    float CP = 0.0;
    if (pile.size() > 0) {
        int* v = pile.top();
        int x = v[0];
        int y = v[1];
        pile.pop();

        CP = getPixelColor(x, y, width, height, lstColor);
        //  std::cout << "x = " <<x << " y "<< y << std::endl;


        if (CP != CC && CP != CR && y < height && x < width && y >= 0 && x >= 0) {
            lstColor[(y * width) + x] = 1.0;
        }

        CP = getPixelColor(x, y - 1, width, height, lstColor);

        if (CP != CC && CP != CR && y >= 0) {
            pile.push(new int[2] {x, y - 1});
        }

        //std::cout << " x"

        CP = getPixelColor(x - 1, y, width, height, lstColor);

        if (CP != CC && CP != CR && x >= 0) {
            pile.push(new int[2] {x - 1, y});
        }


        CP = getPixelColor(x, y + 1, width, height, lstColor);

        if (CP != CC && CP != CR && y < height) {
            pile.push(new int[2] {x, y + 1});
        }

        CP = getPixelColor(x + 1, y, width, height, lstColor);

        if (CP != CC && CP != CR && x < width) {
            pile.push(new int[2] {x + 1, y});
        }




    }
    return lstColor;
}


float* FillStack(int _x,int _y,int width,int height, float* lstColor,float CC = 1.0, float CR = 1.0) {
    std::stack<int*> pile;
    pile.push(new int[2]{_x,_y});
    float CP = 0.0;
    int x = 0;
    int y = 0;
    while (pile.size() > 0 && pile.size() < 200000 ) {
         int* v = pile.top();
         x = v[0];
         y = v[1];
        pile.pop();
     
         CP = getPixelColor(x, y,width,height,lstColor);
       //  std::cout << "x = " <<x << " y "<< y << std::endl;

      
        if (CP != CC && CP != CR && y < height && x < width && y >= 0 && x >= 0) {
            lstColor[(y * width) + x] = 1.0;
        }

        CP = getPixelColor(x, y-1, width, height, lstColor);

        if (CP != CC && CP != CR && y >= 0) {
            pile.push(new int[2] {x, y-1});
        }

        //std::cout << " x"

        CP = getPixelColor(x - 1, y, width, height, lstColor);

        if (CP != CC && CP != CR && x >= 0) {
            pile.push(new int[2] {x - 1, y});
        }


         CP = getPixelColor(x, y + 1, width, height, lstColor);

        if (CP != CC && CP != CR && y < height) {
            pile.push(new int[2] {x, y+1});
        }

         CP = getPixelColor(x+1, y , width, height, lstColor);

        if (CP != CC && CP != CR && x < width) {
            pile.push(new int[2] {x + 1, y});
        }




    }
    return lstColor;
}


float* fillRect(Entity poly,int* min,int* max,float* lstofPixel,int width,int height) {
    
    int xmin = min[0];
    int ymin = min[1];

    int xmax = max[0];
    int ymax = max[1];
    std::vector<float> vec = poly.getFlatVector();
    for (int x = xmin; x < xmax; x++) {

        for (int y = ymin; y < ymax; y++) {
            if (isInside(new float[2] {(float)x,(float)y},vec)) {
                int ypos = height - y;
                lstofPixel[(ypos * width) + x] = 1.0;
            }
        }
    }

    return lstofPixel;
}


float* DrawLine(int xA, int yA, int  xB, int yB, float* lstColor,int width) {
    int dx = xB - xA;
    int dy = yB - yA;

    int adx = abs(dx);
    int ady = abs(dy);
    
    if (adx <= 0) {
        adx = 1;
    }

    if (ady <= 0) {
        ady = 1;
    }


    if (adx > ady) {
        for (int i = 0; i < adx; i++) {
            int  x = xA + (int)((dx * i) / adx);
            int  y = yA + (int)((dy * i) / ady);
            lstColor[(width * y) + x] = 1.0;
        }
    }
    else {
        for (int i = 0; i < ady; i++) {
            int  x = xA + (int)((dy * i) / adx);
            int  y = yA + (int)((dx * i) / ady);
            lstColor[(width * y) + x] = 1.0;
        }
    }


    return lstColor;
}

float* LineFill(int _x,int _y,int width,int height,float* lstColor,float CC = 1.0,float CR = 1.0) {
    float CP;
    float CPd;
    float CPg;
    std::stack<int*> pile;
    int xd;
    int xg;
    int x;
    int y;
    pile.push(new int[2]{_x,_y});
   
    
    while (pile.size() > 0) {
        int* v = pile.top();
        x = v[0];
        y = v[1];
        pile.pop();

        CP = getPixelColor(x, y, width, height, lstColor);

        xd = x+1;
        CPd = CP;

        while (CPd != 1.0) {
            xd+=1;
            CPd = getPixelColor(xd, y, width, height, lstColor);
        }

        xd = xd - 1;
        xg = x-1;
        CPg = CP;

        while (CPg != 1.0) {
            xg -= 1;
            CPg = getPixelColor(xg, y, width, height, lstColor);
        }
        xg = xg + 1;

        lstColor = DrawLine(xg, y, xd, y, lstColor, width);

        while (x >= xg) {
            while ((CP == CC) || (CP == CR) && (x >= xg)) {
                x = x - 1;
                CP = getPixelColor(x, y+1, width, height, lstColor);
            }

            if (x >= xg && (CP != CC) && (CP != CR)) {
                pile.push(new int[2] {x, y + 1});
              
            }

            while ((CP != CC) && (x >= xg)) {
                x = x - 1;
                CP = getPixelColor(x, y + 1, width, height, lstColor);
                
            }

        }

        x = xd;
        CP = getPixelColor(x, y-1, width, height, lstColor);

        while (x >= xg) {
            while ((CP == CC) || (CP == CR) && (x >= xg)) {
                x = x - 1;
                CP = getPixelColor(x, y - 1, width, height, lstColor);
            }

            if (x >= xg && (CP != CC) && (CP != CR)) {
                pile.push(new int[2] {x, y - 1});

            }

            while ((CP != CC) && (x >= xg)) {
                x = x - 1;
                CP = getPixelColor(x, y - 1, width, height, lstColor);

            }

        }


    }

    return lstColor;
}


bool isIntersect(Polygone polly,float* point, int xdir) {

    float B[2];
    B[0] = (point[0] + xdir);
    B[1] = point[1];

    int sum = 0;


    std::vector<float*> pl = polly.getPoints();

    for (int i = 0; i < pl.size()-1; i++) {
        float* b = intersection(pl[i], pl[i + 1],point, B);
        if (b[0] != 0 && b[1] != 0) {
            sum++;
        }
    }

    return sum > 0;
}


LCA* insertLCA(LCA* insert, LCA* newLCA) {
   

    if (insert == newLCA || newLCA == NULL) return insert;

    if (insert == NULL) return newLCA;
   

    LCA* idx = insert;

    if (idx == NULL) {
        std::cout << "idx null" << std::endl;
    }

    while ( idx != NULL && idx->validNext) {
        idx = idx->next;
    }

    idx->next = newLCA;
    idx->validNext = true;

    return insert;
}

LCA sort(LCA* inserted) {
   
    LCA* idx = inserted;
    std::vector<LCA*> sorted;
    while (idx->validNext) {
        sorted.push_back(idx);
        idx = idx->next;
    }
    sorted.push_back(idx);

    bool end = false;

    while (!end) {
        end = true;
        for (int i = 0; i < sorted.size()-1; i++) {
            sorted[i]->next = NULL;
            sorted[i]->validNext = false;
            if (sorted[i + 1]->x < sorted[i]->x || (sorted[i + 1]->x == sorted[i]->x && sorted[i+1]->dir[0] < sorted[i]->dir[0])) {
                std::swap(sorted[i], sorted[i + 1]);
                end = false;
            }
        }
    }

    sorted[sorted.size() - 1]->next = NULL;
    sorted[sorted.size() - 1]->validNext = false;

    
    for (int i = 0; i < sorted.size(); i++) {

        inserted = insertLCA(inserted, sorted[i]);
    }

    return *inserted;
}



std::vector<LCA> InitSI(Entity poly,int size,int ymin) {

    std::vector<LCA> SI(size);
    for (int i = 0; i < size; i++)
    {
        int j = 0;
        bool find = false;
        int height = ymin + i;
        while (j < poly.getPoints().size() && !find) {
            int nxt = (j + 1) % (poly.getPoints().size());
            LCA* tmp;
           
            if ( abs((poly[j][1] - height)) < 1  && (poly[nxt][1] > height || height == ymin + size -1 )) {
                
               
                tmp = new LCA;

                if (poly[j][1] > poly[nxt][1]) {
                    tmp->ymax = poly[j][1];
                }
                else {
                    tmp->ymax = poly[nxt][1];
                }

                float invDir[2];
                
                float y1 = (poly[j][1]);
                float y2 = (poly[nxt][1]);
                
                float x1 = (poly[j][0]);
                float x2 = (poly[nxt][0]);

                std::cout << x1 << " " << 720 - y1 << std::endl;

                float distance = sqrt(powf((x2 - x1), 2) + powf((y2 - y1), 2));
                invDir[0] = ((x2 - x1))/((y2 - y1));
                invDir[1] = ((y2 - y1)) / distance;

                tmp->x = poly[j][0];
                tmp->dir[0] = invDir[0];
                tmp->dir[1] = invDir[1];


                if (SI[i].x == 0) {
                    SI[i] = *tmp;
                }
                else {
                 
                    SI[i] = *insertLCA(&SI[i], tmp);
               
                }
               
            }
            else if ( abs((poly[nxt][1]) - height) < 1 &&  (poly[j][1] > height || height == ymin + size -1)) {
                
              
                tmp = new LCA;

                if (poly[j][1] > poly[nxt][1]) {
                    tmp->ymax = poly[j][1];
                }
                else {
                    tmp->ymax = poly[nxt][1];
                }

                
                float invDir[2];

                float y1 = (poly[j][1]);
                float y2 = (poly[nxt][1]);

                float x1 = (poly[j][0]);
                float x2 = (poly[nxt][0]);

                std::cout << x1 << " " << 720 - y1 << std::endl;

                float distance = sqrt( powf((x2 - x1),2) + powf((y2 - y1), 2));
                invDir[0] = ( (x1 - x2)/ (y1 - y2));
                invDir[1] = ((y1 - y2) / distance);
              
                tmp->x = poly[nxt][0];
                tmp->dir[0] = invDir[0];
                tmp->dir[1] = invDir[1];

                if (SI[i].x == 0) {
                    SI[i] = *tmp;
                }
                else {
                    SI[i] = *insertLCA(&SI[i], tmp);
                }

            }
       
            j++;
          
        }

     

    }



    return SI;
}


LCA* deleteFromY(LCA* lca,int y,int miny) {

    LCA* idx = lca;

    if (lca == NULL) return NULL;

  
    if (idx->ymax <= y+ miny && !idx->validNext) {
        return NULL;
    }
    else if (idx->ymax <= y + miny && idx->validNext) {
        lca = idx->next;
    }
    

    while (idx != NULL && idx->validNext != false) {
        if (idx->next->ymax <= y+ miny) {
            idx->next = idx->next->next;
            if (idx->next == NULL) {
                idx->validNext = false;
            }
        }
        idx = idx->next;
    }


    return lca;
}


float* FillLCA(Entity Poly,float* lstColor,int width,int height,int minx,int miny,int maxx,int maxy) {
    

    // Init LCA
    int size = Poly.getPoints().size();
    int Value = int(maxy - miny)+1;
    std::vector<LCA> SI = InitSI(Poly,Value,miny);
    LCA initLCA;


    // FILL
    LCA t;
    LCA nxt;
    for (int y = 0; y < Value; y++) {
        
        if (SI[y].x != 0) {
            
            initLCA = *insertLCA(&initLCA,&SI[y]);
            initLCA = sort(&initLCA);

            initLCA = *deleteFromY(&initLCA, y, miny);
        }


        LCA* p = &initLCA;
        bool parite = true;
        while (p != NULL && p->next != nullptr) {
           
            if (parite) {
                float x1 = p->x + p->dir[0];
                float x2 = p->next->x + p->next->dir[0];
            
                for (int x = x1; x <= x2; x++) {
                    int ypos = height - (y+miny);
                    lstColor[(width * (ypos)) + (int)x] = 1.0;
                }

                p->x = x1;
                p->next->x = x2;
            }

            p = p->next;
            parite = !parite;

        }

                      
    }

 

    return lstColor;
}
