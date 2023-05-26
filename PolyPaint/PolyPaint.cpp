//#define GLEW_STATIC 1
#include "PolyPaint.h"

GLShader g_triangleShader;
GLShader g_curveShader;
GLShader g_Curves;

float Screen[2]{ 1920,1920 };

double xpos, ypos;
float* mousePos;

float* Points;
int lastPoint = 0;

std::vector<float> lstPoints;
std::vector<float> lstPointsFenetre;
std::vector<float> blank;
bool Fenetrage = false;
bool PolyEdit = true;
bool WindoEdit = false;
bool Sutherland = true;
int currentSelectPoly = 0;
float* k = new float[1] {0.05f};
bool Lerp = true;
bool moving = false;
enum Input
{
    FillCCitUpdate,
    FillCCit,
    FillCCRec,
    FillCCLine,
    FIllCCPoly,
    AddPoly
}; 

Input ClickInput;

std::stack<int*> pile;
bool fill = false;
Polygone Poly;

Entity Wind;
Curve WindCurve;

std::vector<Entity> lstPolygones;

std::vector<float> lstofPixel;
float *pixelScreens;
bool move = false;
std::vector<float*> lstOfPS;

float** array2D(int n, int m, int val = 0) {
    float** tab = new float* [n];
    for (int i = 0; i < n; ++i) {
        tab[i] = new float[m];
        for (int j = 0; j < m; ++j)
            tab[i][j] = val;
    }
    return tab;
}


void loadTexture(int f, float* data,float with,float hieght) {



    unsigned int texId;
    glGenTextures(1, &texId);

    glActiveTexture(GL_TEXTURE0 + f);
    glBindTexture(GL_TEXTURE_2D, texId);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
        GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
        GL_NEAREST);




    int w, h;
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, with, hieght, 0, GL_RED, GL_FLOAT, data);
        
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
   

}


int findSelectedPoint(Entity ent) {


    for (int i = 0; i < ent.getPoints().size(); i++)
    {
        if (distance(ent.getPoints()[i], mousePos) < 20.0f) {
            return i;
        }
    }

    return -1;
}


void Initialize()
{

    lstPoints = std::vector<float>();
    lstPointsFenetre = std::vector<float>();
    lstofPixel = std::vector<float>();
    mousePos = new float[2];
   
    ClickInput = AddPoly;

    Poly = Polygone();
    Wind = Polygone();
    

    lstOfPS.push_back(pixelScreens);
    lstPolygones.push_back(Poly);

    blank.push_back(-10);
    blank.push_back(-10);

    g_triangleShader.LoadFragmentShader("../../../../PolyPaint/shader.frag");
    g_triangleShader.LoadVertexShader("../../../../PolyPaint/shader.vert");
    g_triangleShader.Create();

    g_curveShader.LoadFragmentShader("../../../../PolyPaint/shaderCurve.frag");
    g_curveShader.LoadVertexShader("../../../../PolyPaint/shaderCurve.vert");
    g_curveShader.Create();

    g_Curves.LoadFragmentShader("../../../../PolyPaint/Curves.frag");
    g_Curves.LoadVertexShader("../../../../PolyPaint/Curves.vert");
    g_Curves.Create();

}

void Terminate()
{
    
}

void MyLowLevelMouseButtonHandler(int button, bool down)
{
    // (1) ALWAYS forward mouse data to ImGui! This is automatic with default backends. With your own backend:
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(button, down);


}



bool isMousePressed;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGuiIO& io = ImGui::GetIO();
  
  
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

    if (!io.WantCaptureMouse)
    {

        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                isMousePressed = true;
            }
            else if (action == GLFW_RELEASE) {
                isMousePressed = false;
            }
        }

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS )
        {
            switch (ClickInput)
            {


                case FillCCitUpdate:
                    pile.push(new int[2] {(int)mousePos[0], (int)Screen[1] - (int)mousePos[1]});
                    fill = true;
                    ClickInput = AddPoly;
                break;

                case FillCCRec:
                    lstOfPS[currentSelectPoly] = FillFormConex((int)mousePos[0], (int)Screen[1] - (int)mousePos[1], Screen[0], Screen[1], lstOfPS[currentSelectPoly]);
                    loadTexture(currentSelectPoly, lstOfPS[currentSelectPoly], Screen[0], (int)Screen[1]);
                    ClickInput = AddPoly;
                break;

                case FillCCLine:
                    lstOfPS[currentSelectPoly] = LineFill((int)mousePos[0], (int)Screen[1] - (int)mousePos[1], Screen[0], Screen[1], lstOfPS[currentSelectPoly]);
                    loadTexture(currentSelectPoly, lstOfPS[currentSelectPoly], Screen[0], (int)Screen[1]);
                    ClickInput = AddPoly;
                break;

                case FillCCit:
                    lstOfPS[currentSelectPoly] = FillStack((int)mousePos[0], (int)Screen[1] - (int)mousePos[1], (int)Screen[0], (int)Screen[1], lstOfPS[currentSelectPoly]);
                    loadTexture(currentSelectPoly, lstOfPS[currentSelectPoly], Screen[0], (int)Screen[1]);
                    ClickInput = AddPoly;
                break;
                case AddPoly:

                    if (!Fenetrage && PolyEdit) {
                        lstPolygones[currentSelectPoly].push_back(mousePos);
                        lstPoints.push_back(mousePos[0]);
                        lstPoints.push_back(mousePos[1]);
                    }
                    else if (WindoEdit)
                    {
                        Wind.push_back(mousePos);
                        lstPointsFenetre.push_back(mousePos[0]);
                        lstPointsFenetre.push_back(mousePos[1]);
                    }
                    ClickInput = AddPoly;
                break;
               
            }

           
          
        }
    }

}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        Fenetrage = !Fenetrage;
    }


    if (key == GLFW_KEY_DELETE && action == GLFW_PRESS) {
        if (lstPolygones[currentSelectPoly].getPoints().size() > 0) {

            std::vector<float*>temp = std::vector<float*>(lstPolygones[currentSelectPoly].getPoints());
            int idx = findSelectedPoint(lstPolygones[currentSelectPoly]);


            if (idx >= 0 && idx < temp.size()) {
                temp.erase(temp.begin() + idx);
            }
            moving = false;

            lstPolygones[currentSelectPoly].setPoints(temp);
        }
    }

    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        if (lstPolygones[currentSelectPoly].getPoints().size() > 0) {

            std::vector<float*>temp = std::vector<float*>(lstPolygones[currentSelectPoly].getPoints());
            std::vector<float*>temp2;
            int idx = findSelectedPoint(lstPolygones[currentSelectPoly]);


            for (int i = 0; i < temp.size(); i++)
            {
                temp2.push_back(temp[i]);
                if (i == idx) {
                    temp2.push_back(temp[i]);
                }
            }
            lstPolygones[currentSelectPoly].setPoints(temp2);
        }
    }

 
}


void RenderPolygone(int width, int height, int index) {

    auto sh_Triangle = g_triangleShader.GetProgram();
    glUseProgram(sh_Triangle);

    const float aspectRatio = float(width) / float(height);
    const float zNear = 0.1f, zFar = 10000000.0f;
    const float fovY = (90.f * M_PI) / 180.0f;
    const float cot = 1.f / tanf(fovY / 2.f);


    float projectionMatrix[16] = {
            cot / aspectRatio, 0.f,0.f,0.f,
            0.f,1.0f,0.f,0.f,
            0.f,0.f,-1.0f,-1,
            0.f,0.f,0.0,1.0f,
    };




    float time = glfwGetTime();


    const float stride = 2 * sizeof(float);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
            //               stride, triangle);


    const auto loc_proj_mat_triangle = glGetUniformLocation(
        sh_Triangle, "u_projectionMatrix"
    );
    glUniformMatrix4fv(loc_proj_mat_triangle, 1, GL_FALSE, projectionMatrix);


    const auto loc_postiion = glGetAttribLocation(
        sh_Triangle, "a_position"
    );
    glEnableVertexAttribArray(loc_postiion);
    glVertexAttribPointer(loc_postiion, 2, GL_FLOAT, GL_FALSE,
        stride, triangle);
    glEnableVertexAttribArray(loc_postiion);


    const auto loc_uv = glGetAttribLocation(
        sh_Triangle, "a_uv"
    );
    glEnableVertexAttribArray(loc_uv);
    glVertexAttribPointer(loc_uv, 2, GL_FLOAT, GL_FALSE,
        stride, uv);

    glEnableVertexAttribArray(loc_uv);


    const auto loc_size_selectedPoint = glGetUniformLocation(
        sh_Triangle, "u_indexSelectedPoint"
    );

    glUniform1i(loc_size_selectedPoint, findSelectedPoint(lstPolygones[index]));


    const auto loc_float_texture = glGetUniformLocation(
        sh_Triangle, "float_texture"
    );

    glUniform1i(loc_float_texture, index);

    /*  mousePos[0] = (xpos / Screen[0]);
      mousePos[1] = (1.0f - (ypos / Screen[1]));*/

    GLint m_viewport[4];

    glGetIntegerv(GL_VIEWPORT, m_viewport);

    mousePos[0] = xpos;
    mousePos[1] = height - ypos;


    const auto loc_mouse = glGetUniformLocation(
        sh_Triangle, "u_mousePos"
    );

    glUniform2fv(loc_mouse, 1, &mousePos[0]);
    glEnableVertexAttribArray(loc_mouse);


    const auto loc_resolution = glGetUniformLocation(
        sh_Triangle, "u_resolution"
    );
    glUniform2fv(loc_resolution, 1, &Screen[0]);


    std::vector<float> tempVector = lstPolygones[index].getFlatVector();

    if (tempVector.size() <= 0) {
        tempVector = blank;
    }

    const auto loc_points = glGetUniformLocation(
        sh_Triangle, "u_points"
    );

    glUniform2fv(loc_points, tempVector.size(), &tempVector[0]);

    const auto loc_size_point = glGetUniformLocation(
        sh_Triangle, "u_sizeOfPoints"
    );

    glUniform1i(loc_size_point, int(tempVector.size() / 2));


    const auto loc_size_pixel = glGetUniformLocation(
        sh_Triangle, "u_sizePixel"
    );

    glUniform1i(loc_size_pixel, int(lstofPixel.size() / 2));

    tempVector = Wind.getFlatVector();

    if (tempVector.size() <= 0) {
        tempVector = blank;
    }

    const auto loc_fenetre = glGetUniformLocation(
        sh_Triangle, "u_fenetre"
    );

    glUniform2fv(loc_fenetre, tempVector.size(), &tempVector[0]);

    const auto loc_size_fenetre = glGetUniformLocation(
        sh_Triangle, "u_sizeOfFenetre"
    );

    glUniform1i(loc_size_fenetre, int(tempVector.size() / 2));

  



    glDrawArrays(GL_QUADS, 0, 4);
}

void RenderCurve(int width, int height, int index) {

    auto sh_Triangle = g_curveShader.GetProgram();
    glUseProgram(sh_Triangle);

    const float aspectRatio = float(width) / float(height);
    const float zNear = 0.1f, zFar = 10000000.0f;
    const float fovY = (90.f * M_PI) / 180.0f;
    const float cot = 1.f / tanf(fovY / 2.f);


    float projectionMatrix[16] = {
            cot / aspectRatio, 0.f,0.f,0.f,
            0.f,1.0f,0.f,0.f,
            0.f,0.f,-1.0f,-1,
            0.f,0.f,0.0,1.0f,
    };




    float time = glfwGetTime();


    const float stride = 2 * sizeof(float);
    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,
            //               stride, triangle);


    const auto loc_proj_mat_triangle = glGetUniformLocation(
        sh_Triangle, "u_projectionMatrix"
    );
    glUniformMatrix4fv(loc_proj_mat_triangle, 1, GL_FALSE, projectionMatrix);


    const auto loc_postiion = glGetAttribLocation(
        sh_Triangle, "a_position"
    );
    glEnableVertexAttribArray(loc_postiion);
    glVertexAttribPointer(loc_postiion, 2, GL_FLOAT, GL_FALSE,
        stride, triangle);
    glEnableVertexAttribArray(loc_postiion);


    const auto loc_uv = glGetAttribLocation(
        sh_Triangle, "a_uv"
    );
    glEnableVertexAttribArray(loc_uv);
    glVertexAttribPointer(loc_uv, 2, GL_FLOAT, GL_FALSE,
        stride, uv);

    glEnableVertexAttribArray(loc_uv);



    const auto loc_float_texture = glGetUniformLocation(
        sh_Triangle, "float_texture"
    );

    glUniform1i(loc_float_texture, index);

    /*  mousePos[0] = (xpos / Screen[0]);
      mousePos[1] = (1.0f - (ypos / Screen[1]));*/

    GLint m_viewport[4];

    glGetIntegerv(GL_VIEWPORT, m_viewport);

    mousePos[0] = xpos;
    mousePos[1] = height - ypos;


    const auto loc_mouse = glGetUniformLocation(
        sh_Triangle, "u_mousePos"
    );

    glUniform2fv(loc_mouse, 1, &mousePos[0]);
    glEnableVertexAttribArray(loc_mouse);


    const auto loc_resolution = glGetUniformLocation(
        sh_Triangle, "u_resolution"
    );
    glUniform2fv(loc_resolution, 1, &Screen[0]);


    std::vector<float> tempVector = lstPolygones[index].getFlatVector();

    if (tempVector.size() <= 0) {
        tempVector = blank;
    }

    const auto loc_points = glGetUniformLocation(
        sh_Triangle, "u_points"
    );

    glUniform2fv(loc_points, tempVector.size(), &tempVector[0]);

    const auto loc_size_point = glGetUniformLocation(
        sh_Triangle, "u_sizeOfPoints"
    );

    glUniform1i(loc_size_point, int(tempVector.size() / 2));



    const auto loc_size_selectedPoint = glGetUniformLocation(
        sh_Triangle, "u_indexSelectedPoint"
    );

    glUniform1i(loc_size_selectedPoint, findSelectedPoint(lstPolygones[index]) );


    const auto loc_size_pixel = glGetUniformLocation(
        sh_Triangle, "u_sizePixel"
    );

    glUniform1i(loc_size_pixel, int(lstofPixel.size() / 2));

    tempVector = Wind.getFlatVector();


    if (tempVector.size() <= 0) {
        tempVector = blank;
    }



    const auto loc_fenetre = glGetUniformLocation(
        sh_Triangle, "u_fenetre"
    );

    glUniform2fv(loc_fenetre, tempVector.size(), &tempVector[0]);

    const auto loc_size_fenetre = glGetUniformLocation(
        sh_Triangle, "u_sizeOfFenetre"
    );

    glUniform1i(loc_size_fenetre, int(tempVector.size() / 2));



    glDrawArrays(GL_QUADS, 0, 4);

    // CHJANTGE SHAFER

    auto sh_Curves = g_Curves.GetProgram();
    glUseProgram(sh_Curves);



    const auto loc_proj_mat_triangle_Curves = glGetUniformLocation(
        sh_Curves, "u_projectionMatrix"
    );
    glUniformMatrix4fv(loc_proj_mat_triangle_Curves, 1, GL_FALSE, projectionMatrix);


    const auto loc_postiion_Curves = glGetAttribLocation(
        sh_Curves, "a_position"
    );
    glEnableVertexAttribArray(loc_postiion_Curves);
    glVertexAttribPointer(loc_postiion_Curves, 2, GL_FLOAT, GL_FALSE,
        stride, triangle);
    glEnableVertexAttribArray(loc_postiion_Curves);


    const auto loc_uv_Curves = glGetAttribLocation(
        sh_Curves, "a_uv"
    );
    glEnableVertexAttribArray(loc_uv_Curves);
    glVertexAttribPointer(loc_uv_Curves, 2, GL_FLOAT, GL_FALSE,
        stride, uv);

    glEnableVertexAttribArray(loc_uv_Curves);


    glGetIntegerv(GL_VIEWPORT, m_viewport);

    mousePos[0] = xpos;
    mousePos[1] = height - ypos;


    const auto loc_mouse_Curves = glGetUniformLocation(
        sh_Curves, "u_mousePos"
    );

    glUniform2fv(loc_mouse_Curves, 1, &mousePos[0]);
    glEnableVertexAttribArray(loc_mouse_Curves);


    const auto loc_resolution_Curves = glGetUniformLocation(
        sh_Curves, "u_resolution"
    );
    glUniform2fv(loc_resolution_Curves, 1, &Screen[0]);


    std::vector<float> tempVector_Curves = lstPolygones[index].getFlatVectorControl();

    if (tempVector_Curves.size() <= 0) {
        tempVector_Curves = blank;
    }

    const auto loc_points_Curves = glGetUniformLocation(
        sh_Curves, "u_points"
    );

    glUniform2fv(loc_points_Curves, tempVector_Curves.size(), &tempVector_Curves[0]);

    const auto loc_size_point_Curves = glGetUniformLocation(
        sh_Curves, "u_sizeOfPoints"
    );

    glUniform1i(loc_size_point_Curves, int(tempVector_Curves.size() / 2));


    const auto loc_size_pixel_Curves = glGetUniformLocation(
        sh_Curves, "u_sizePixel"
    );

    glUniform1i(loc_size_pixel_Curves, int(lstofPixel.size() / 2));


    const auto color_Curves = glGetUniformLocation(
        sh_Curves, "u_color"
    );

    glUniform3fv(color_Curves, 1, lstPolygones[index].color);
    glEnableVertexAttribArray(color_Curves);



    glDrawArrays(GL_QUADS, 0, 4);
}

void Render(GLFWwindow* window)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    glViewport(0, 0, width, height);
    glClearColor(13.0 / 255.0, 15.0 / 255.0, 19.0 / 255.0, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

    glfwGetCursorPos(window, &xpos, &ypos);

   
    glfwSetKeyCallback(window, key_callback);

   

    
    for (int i = 0; i < lstPolygones.size(); i++)
    {
        if (lstPolygones[i].getType() == Pol) {
            RenderPolygone(width, height, i);
        }
        else {
            RenderCurve(width, height, i);
       
        }
        
    }
    

}


void SwitchEdit() {

    PolyEdit = !PolyEdit;
    WindoEdit = !WindoEdit;
}



int it = 0;

int idxMoving = -1;
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(Screen[0], Screen[1], "PolyPaint", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);


    GLenum error = glewInit();
    if (error != GL_NO_ERROR) {
        std::cout << "Erreur d'initialisation de GLEW" << std::endl;
    }


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsLight();
    ImVec4* colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.64f, 0.68f, 0.82f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.64f, 0.68f, 0.82f, 1.00f);
    ImGui::GetStyle().WindowRounding = 9.0f;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    pixelScreens = new float[Screen[0] * Screen[1]];

    Initialize();

   

    for (int i = 0; i < Screen[0]; i++) {
        for (int j = 0; j < Screen[1]; j++) {
            pixelScreens[( (int)Screen[0] * j) + i] = 0.0;
        }
    }

  

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
       
         
        glfwSetKeyCallback(window, key_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        /* Poll for and process events */
       

      //  std::cout << mousePos[0] << " " << mousePos[1] << std::endl;


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
   

        if (isMousePressed && lstPolygones[currentSelectPoly].getPoints().size() >0) {

            if (moving == false) {
                if (lstPolygones[currentSelectPoly].getPoints().size() > 0) {
                    idxMoving = findSelectedPoint(lstPolygones[currentSelectPoly]);
                }
                moving = true;
            }
            
            if (idxMoving >= 0) {
                lstPolygones[currentSelectPoly].getPoints()[idxMoving][0] = mousePos[0];
                lstPolygones[currentSelectPoly].getPoints()[idxMoving][1] = mousePos[1];
            }

          
        }
        else {
            moving = false;
        }

        glfwPollEvents();

   /*     ImGui::ShowDemoWindow();
       
       ;*/
        //ImGui::ShowStyleEditor();

        ImGui::SetNextWindowPos(ImVec2(30.0, 30.0));

        ImGui::Begin("PolyPaint Point Handler");
        ImGui::Text("PolyPaint Manager");
        ImGui::Spacing();
       
        ImGui::Text("PolyGone Manager");

        ImVec4* colors = ImGui::GetStyle().Colors;
        ImVec4 basicButton = colors[ImGuiCol_Button];

      
        colors[ImGuiCol_Button] = basicButton;
        if (!PolyEdit && !move) {
           
            if (ImGui::Button("Add point to the polygone") ) {
                SwitchEdit();
            }
                
        }
        else if (!move) {
            colors[ImGuiCol_Button] = ImVec4(0.00f, 1.0, 128.0 / 255.0, 0.5);
            if (ImGui::Button("Stop adding point to the polygone") )
            {
                SwitchEdit();
            }
               
        }

        
        ImGui::Spacing();
        colors[ImGuiCol_Button] = ImVec4(0.00f, 1.0, 128.0 / 255.0, 0.5);
        if (lstPolygones[currentSelectPoly].getPoints().size() > 0 && ImGui::Button("Reset Pollygone")) {
         
            lstPolygones[currentSelectPoly].setPoints(std::vector<float*>());

        }
        colors[ImGuiCol_Button] = basicButton;

     

        ImGui::Spacing();
        ImGui::Text("Window Manager");
  
        colors[ImGuiCol_Button] = basicButton;
        if (!WindoEdit && !move) {
          
            if (ImGui::Button("Add point to the Windo") ) {
                SwitchEdit();
            }
      
           


        }
        else if(!move) {
            colors[ImGuiCol_Button] = ImVec4(144.0 / 255.0, 18.0 / 255.0, 208.0 / 255.0, .5);
           
            if (ImGui::Button("Stop adding point to the Windo")) {
                SwitchEdit();
            }
               
        }
     

        ImGui::Spacing();
        colors[ImGuiCol_Button] = ImVec4(144.0 / 255.0, 18.0 / 255.0, 208.0 / 255.0, .5);
        if (Wind.getPoints().size() > 0 && ImGui::Button("Remove")) {

            Wind.setPoints(std::vector<float*>());
        }

        colors[ImGuiCol_Button] = ImVec4(255.0 / 255.0, 18.0 / 255.0, 50.0 / 255.0, .5);
        if (ImGui::Button("Moving vertex")) {
            move = !move;
            if (move) {
                PolyEdit = false;
                WindoEdit = false;
            }
            else {
                PolyEdit = true;
                WindoEdit = false;
            }

        }
        colors[ImGuiCol_Button] = basicButton;


 

        ImGui::NewLine();
        ImGui::Spacing();
        
        if (Wind.getPoints().size() >= 3 && lstPolygones[currentSelectPoly].getPoints().size() >= 3) {

       

            ImGui::Text("Clip polygone");
            if ( ImGui::Button("Clip Sutherland")) {
               // lstPointsFenetre.push_back(lstPointsFenetre[0]);
                //lstPointsFenetre.push_back(lstPointsFenetre[1]);
            
               // lstPoints = SutherLandHodman(lstPoints, lstPointsFenetre);
                Wind.push_back(Wind[0]);
                type _Type = lstPolygones[currentSelectPoly].getType();
                lstPolygones[currentSelectPoly] = SutherLandHodman(lstPolygones[currentSelectPoly], Wind);
                lstPolygones[currentSelectPoly].setType(_Type);
               
            }
            ImGui::Spacing();

            if (ImGui::Button("Clip Cyrus Beck") ) {
       
               
                std::vector<float> d = GenerateCyrusBeck(Wind.getFlatVector(), lstPolygones[currentSelectPoly].getFlatVector() );
                std::vector<float*> tmp;

                if (d.size() > 0) {
                    for (int i = 0; i <= d.size() - 2; i += 2) {
                        tmp.push_back(new float[2] {d[i], d[i + 1]});
                    }

                    lstPolygones[currentSelectPoly].setPoints(tmp);
                }

             


            }
        }
        
      

        ImGui::BeginTable("t",2);
        ImGui::TableHeader("test");
        ImGui::EndTable();
   
          
 
        ImGui::SetWindowSize(ImVec2(300, 250));
     


        ImGui::End();

       
        ImGui::SetNextWindowPos(ImVec2(30.0,(250+30)+30));
     

        ImGui::Begin("Remplissage");


  

        ImGui::Spacing();


        colors[ImGuiCol_Button] = basicButton;
     
        if (ClickInput == FillCCitUpdate || fill) {
            colors[ImGuiCol_Button] = ImVec4(214 / 255.0, 34 / 255.0, 61 / 255.0, 1);
        }
      

        
        ImGui::Text("Pixel Fill");
        ImGui::Spacing();

        if (ImGui::Button("Fill")) {
           
            if (ClickInput == FillCCitUpdate) {
                ClickInput = AddPoly;
            }
            else {
                ClickInput = FillCCitUpdate;
            }
            
           
        }


        ImGui::SameLine();
     
        colors[ImGuiCol_Button] = basicButton;
        if (ClickInput == FillCCit) {
            colors[ImGuiCol_Button] = ImVec4(219 / 255.0, 202 / 255.0, 46 / 255.0, .7);
        }


        if (ImGui::Button(" Interat Fill")) {

            if (ClickInput == FillCCit) {
                ClickInput = AddPoly;
            }
            else {
                ClickInput = FillCCit;
            }

            
            
        }

        colors[ImGuiCol_Button] = basicButton;
        if (ClickInput == FillCCRec) {
            colors[ImGuiCol_Button] = ImVec4(0.00f, 1.0, 128.0 / 255.0, 0.5);
        }

    

        if (ImGui::Button(" RecFill")) {

            if (ClickInput == FillCCRec) {
                ClickInput = AddPoly;
            }
            else {
                ClickInput = FillCCRec;
            }

            
        }

        ImGui::SameLine();

        colors[ImGuiCol_Button] = basicButton;
        if (ClickInput == FillCCLine) {
            colors[ImGuiCol_Button] = ImVec4(219 / 255.0, 202 / 255.0, 46 / 255.0, .7);
        }

    
         
        if (ImGui::Button("Fill Line")) {

            if (ClickInput == FillCCLine) {
                ClickInput = AddPoly;
            }
            else {
                ClickInput = FillCCLine;
            }



        }


        colors[ImGuiCol_Button] = basicButton;



        ImGui::NewLine();
        ImGui::Text("Poly Fill");
        ImGui::Spacing();
        
       
        colors[ImGuiCol_Button] = ImVec4(0.00f, 1.0, 128.0 / 255.0, 0.5);
        if (ImGui::Button("Fill Rect")) {

            int xmin = Screen[0];
            int ymin = Screen[1];
            
            int xmax = -Screen[0];
            int ymax = -Screen[1];

            for (int i = 0; i < lstPolygones[currentSelectPoly].getPoints().size(); i++)
            {
                if (lstPolygones[currentSelectPoly][i][0] < xmin) {
                    xmin = lstPolygones[currentSelectPoly][i][0];
                }

                if (lstPolygones[currentSelectPoly][i][0] > xmax) {
                    xmax = lstPolygones[currentSelectPoly][i][0];
                }

                if (lstPolygones[currentSelectPoly][i][1] > ymax) {
                    ymax = lstPolygones[currentSelectPoly][i][1];
                }

                if (lstPolygones[currentSelectPoly][i][1] < ymin) {
                    ymin = lstPolygones[currentSelectPoly][i][1];
                }

            }


            lstOfPS[currentSelectPoly] = fillRect(lstPolygones[currentSelectPoly], new int[2] {xmin, ymin}, new int[2] {xmax, ymax}, lstOfPS[currentSelectPoly], Screen[0], Screen[1]);
            loadTexture(currentSelectPoly, lstOfPS[currentSelectPoly], Screen[0], (int)Screen[1]);
          
        }


        colors[ImGuiCol_Button] = ImVec4(144.0 / 255.0, 18.0 / 255.0, 208.0 / 255.0, .5);
        ImGui::Spacing();
        if (ImGui::Button("Fill LCA")) {

            int xmin = Screen[0];
            int ymin = Screen[1];

            int xmax = -Screen[0];
            int ymax = -Screen[1];

            for (int i = 0; i < lstPolygones[currentSelectPoly].getPoints().size(); i++)
            {
                if (lstPolygones[currentSelectPoly][i][0] < xmin) {
                    xmin = lstPolygones[currentSelectPoly][i][0];
                }

                if (lstPolygones[currentSelectPoly][i][0] > xmax) {
                    xmax = lstPolygones[currentSelectPoly][i][0];
                }

                if (lstPolygones[currentSelectPoly][i][1] > ymax) {
                    ymax = lstPolygones[currentSelectPoly][i][1];
                }

                if (lstPolygones[currentSelectPoly][i][1] < ymin) {
                    ymin = lstPolygones[currentSelectPoly][i][1];
                }

            }

            lstOfPS[currentSelectPoly] = FillLCA(lstPolygones[currentSelectPoly], lstOfPS[currentSelectPoly], Screen[0],Screen[1], xmin, ymin, xmax, ymax);
            loadTexture(currentSelectPoly, lstOfPS[currentSelectPoly], Screen[0], (int)Screen[1]);
        }

        colors[ImGuiCol_Button] = basicButton;

        ImGui::NewLine();
        ImGui::Text("Reset Fill");
        ImGui::Spacing();
        if (ImGui::Button("Reset Fill")) {



            for (int i = 0; i < Screen[0]; i++) {
                for (int j = 0; j < Screen[1]; j++) {
                   lstOfPS[currentSelectPoly][((int)Screen[0] * j) + i] = 0.0;
                }
            }

            loadTexture(currentSelectPoly, lstOfPS[currentSelectPoly], Screen[0], Screen[1]);

        }

        if (fill && pile.size() > 0) {
           
            while (pile.size() > 0 && it < 500) {
                lstOfPS[currentSelectPoly] = FillStackUpdate((int)Screen[0], (int)Screen[1], lstOfPS[currentSelectPoly], pile);
                it++;
            }
          
         //   std::cout << getPixelColor(600, 600, Screen[0], Screen[1], pixelScreens) << std::endl;
            
            if (it >= 500) {
                loadTexture(currentSelectPoly, lstOfPS[currentSelectPoly], Screen[0], Screen[1]);
                it = 0;
            }

           
        
        }
        else {
            fill = false;
        }
       
      


        ImGui::SetWindowSize(ImVec2(300, 150));
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(30.0, ( (400) + 90 )));


        ImGui::Begin("Polygones");
        
        if (ImGui::Button("Add Polygone")) {
            Polygone pol = Polygone();
           
            lstPolygones.push_back(pol);
            currentSelectPoly++;
            float* f = new float[Screen[0] * Screen[1]];
            lstOfPS.push_back(f);

            for (int i = 0; i < Screen[0]; i++) {
                for (int j = 0; j < Screen[1]; j++) {
                    lstOfPS[currentSelectPoly][((int)Screen[0] * j) + i] = 0.0;
                }
            }

            
        
        }

        if (ImGui::Button("Add Curve")) {
            Curve pol = Curve();
            
            lstPolygones.push_back(Curve());
            currentSelectPoly++;
            float* f = new float[Screen[0] * Screen[1]];
            lstOfPS.push_back(f);

            for (int i = 0; i < Screen[0]; i++) {
                for (int j = 0; j < Screen[1]; j++) {
                    lstOfPS[currentSelectPoly][((int)Screen[0] * j) + i] = 0.0;
                }
            }


        }


        ImGui::SameLine();

        if (ImGui::Button("Remove Polygone") && lstPolygones.size() >1) {
            lstPolygones.erase(lstPolygones.begin() + currentSelectPoly);

            currentSelectPoly = (currentSelectPoly + 1) % lstPolygones.size();

        }

        ImGui::SetWindowSize(ImVec2(300, 200));

        if (ImGui::BeginTable("Table", 3,
            ImGuiTableFlags_RowBg |
            ImGuiTableFlags_ScrollY))
        {
            ImGui::TableSetupColumn("Name");
            ImGui::TableSetupColumn("Number of Point");
            ImGui::TableSetupColumn(" ");

            const int nbLines = lstPolygones.size();
            for (int i = 0; i < nbLines; ++i)
            {
                ImGui::TableNextRow();
                ImGui::PushID(i);

                ImGui::TableSetColumnIndex(0);

                if (lstPolygones[i].getType() == Pol) {
                    ImGui::Selectable("Polygone : ", nbLines,
                        ImGuiSelectableFlags_SpanAllColumns
                    );
                }
                else {
                    ImGui::Selectable("Curve : ", nbLines,
                        ImGuiSelectableFlags_SpanAllColumns
                    );
                }
              

                if (ImGui::IsItemClicked(0)) {
                    currentSelectPoly = i;
                }

                ImGui::TableNextColumn();
                
                ImGui::Text("%i",lstPolygones[i].getPoints().size());

                ImGui::TableNextColumn();


                if (currentSelectPoly == i) {
                    ImGui::Text("X");
                }

                
               
                ImGui::PopID();
            }
            ImGui::EndTable();
        }
      


        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(30.0, ((640) + 90)));
       
        if (lstPolygones[currentSelectPoly].getType() == Cur) {
            ImGui::Begin("Calcul Curve");

            
            ImGui::SliderFloat("Valeur du pas", k, 0.005f, .5f);
            
            (*lstPolygones[currentSelectPoly].getCTRL()).clear();

            ImGui::Checkbox("mycheckbox", &Lerp);

            if ( Lerp && lstPolygones[currentSelectPoly].getPoints().size() > 0) {
               
                auto start = std::chrono::system_clock::now();
                std::vector<float*> tempPoints;
                std::vector<float*> newPoints;
                Entity p = lstPolygones[currentSelectPoly];
                tempPoints = std::vector<float*>(p.getPoints());
                lstPolygones[currentSelectPoly].color = new float[3] {245, 216, 73};
                newPoints.push_back(p.getPoints()[0]);
                for (float t = 0; t <= 1; t += *k)
                {
                    std::vector<float*> control = tempPoints;

                    while (control.size() > 1)
                    {
                        std::vector<float*> tempUpdatedPoints;
                        for (int i = 0; i < control.size()-1; i++)
                        {
                        
                            float x = t * control[i + 1][0] + (1 - t) * control[i][0];
                            float y = t * control[i + 1][1] + (1 - t) * control[i][1];

                            float* newP = new float[2] {x, y};
                            tempUpdatedPoints.push_back(newP);
                        }
                        control = std::move(tempUpdatedPoints);
                    }

                    newPoints.push_back(control[0]);
                }

                lstPolygones[currentSelectPoly].setCTRls(newPoints);
                auto end = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_seconds = end - start;
                std::cout << elapsed_seconds.count()*1000 << std::endl;

            }
            else if(lstPolygones[currentSelectPoly].getPoints().size() > 0){
                auto start = std::chrono::system_clock::now();
                int size = lstPolygones[currentSelectPoly].getPoints().size()-1;
                std::vector<float*> pascal = GetPascal(size+1);
                std::vector<float*> tempPoints;
                std::vector<float*> Points = lstPolygones[currentSelectPoly].getPoints();

                lstPolygones[currentSelectPoly].color = new float[3] {88, 116, 226};

                for (float t = 0; t <= 1; t += *k)
                {
                    float x = 0.0;
                    float y = 0.0;

                    float* p = new float[2] {0.0, 0.0 };

                    for (int i = 0; i <= size; i++)
                    {
                        double blend = pascal[size][i] * std::pow(t, i) * std::pow(1 - t, size - i);
                        x += blend * Points[i][0];
                        y += blend * Points[i][1];

                        
                    }
                    p[0] = x;
                    p[1] = y;
                    tempPoints.push_back(p);
                
                }
                lstPolygones[currentSelectPoly].setCTRls(tempPoints);
                auto end = std::chrono::system_clock::now();
                std::chrono::duration<double> elapsed_seconds = end - start;
                std::cout << elapsed_seconds.count()*1000 << std::endl;
            }
         

            if (ImGui::Button("add ConvexShape")) {
      
                Polygone pol = Polygone();
                std::vector<float*> curves = convexShape(*lstPolygones[currentSelectPoly].getCTRL());

                for (int i = 0; i < lstPolygones[currentSelectPoly].getCTRL()->size(); i++)
                {
                    std::cout << (*lstPolygones[currentSelectPoly].getCTRL())[i][0] << " , " << (*lstPolygones[currentSelectPoly].getCTRL())[i][1] << std::endl;
                }

                std::cout << " convex shape" << std::endl;
                for (int i = 0; i < curves.size(); i++)
                {
                    std::cout << curves[i][0] << " , " << curves[i][1] << std::endl;
                }

                std::cout << curves.size() << std::endl;
                lstPolygones.push_back(pol);
                currentSelectPoly++;
                float* f = new float[Screen[0] * Screen[1]];
                lstOfPS.push_back(f);

                for (int i = 0; i < Screen[0]; i++) {
                    for (int j = 0; j < Screen[1]; j++) {
                        lstOfPS[currentSelectPoly][((int)Screen[0] * j) + i] = 0.0;
                    }
                }

                lstPolygones[currentSelectPoly].setPoints(curves);

            }


            ImGui::SetWindowSize(ImVec2(300, 150));
            ImGui::End();
        }
       



        /* Render here */
        ImGui::Render();





        Render(window);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

    }


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    Terminate();

    glfwTerminate();
    return 0;
}