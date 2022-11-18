//#define GLEW_STATIC 1
#include "PolyPaint.h"

GLShader g_triangleShader;

float Screen[2]{ 1080*2,720*2 };

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
Polygone poly;

void Initialize()
{

    lstPoints = std::vector<float>();
    lstPointsFenetre = std::vector<float>();
    
    mousePos = new float[2];
   
    blank.push_back(0);
    blank.push_back(1);

    g_triangleShader.LoadFragmentShader("../../../../PolyPaint/shader.frag");
    g_triangleShader.LoadVertexShader("../../../../PolyPaint/shader.vert");
    g_triangleShader.Create();

   
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

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    ImGuiIO& io = ImGui::GetIO();
  
  
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

    if (!io.WantCaptureMouse)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS )
        {
            MyLowLevelMouseButtonHandler(button, true);
            if (!Fenetrage && PolyEdit) {
                
                lstPoints.push_back(mousePos[0]);
                lstPoints.push_back(mousePos[1]);



            }
            else if(WindoEdit)
            {
                lstPointsFenetre.push_back(mousePos[0]);
                lstPointsFenetre.push_back(mousePos[1]);
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

 
}



void Render(GLFWwindow* window)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    glViewport(0, 0, width, height);
    glClearColor(0.0f, 1.0f, 1.0f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    glfwGetCursorPos(window, &xpos, &ypos);

   
    glfwSetKeyCallback(window, key_callback);





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


   

    std::vector<float> tempVector = lstPoints;

    if (lstPoints.size() <= 0) {
        tempVector = blank;
    }

    const auto loc_points = glGetUniformLocation(
        sh_Triangle, "u_points"
    );

    glUniform2fv(loc_points, lstPoints.size(), &tempVector[0]);


    const auto loc_size_point = glGetUniformLocation(
        sh_Triangle, "u_sizeOfPoints"
    );

    glUniform1i(loc_size_point, int(lstPoints.size() / 2));
    


    tempVector = lstPointsFenetre;

    if (lstPointsFenetre.size() <= 0) {
        tempVector = blank;
    }

  

    const auto loc_fenetre = glGetUniformLocation(
        sh_Triangle, "u_fenetre"
    );


    glUniform2fv(loc_fenetre, lstPointsFenetre.size(), &tempVector[0]);



    const auto loc_size_fenetre = glGetUniformLocation(
        sh_Triangle, "u_sizeOfFenetre"
    );

    glUniform1i(loc_size_fenetre, int(lstPointsFenetre.size() / 2));
    


 

    glDrawArrays(GL_QUADS, 0, 4);
}



void SwitchEdit() {
    PolyEdit = !PolyEdit;
    WindoEdit = !WindoEdit;
}

void GenerateCyrusBeck() {
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

        for (int i = 2; i < tmpPL.size() + 1; i += 2) {

            if (criticalSeg.size() <= 0) {
                break;
            }



            int k = isInterseptCriticalSegment(new float[2] {tmpPL[i], tmpPL[i + 1]}, criticalSeg);
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


    //std::cout << "ici" <<  newPL.size() << std::endl;


    lstPoints = newPL;
}



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(Screen[0], Screen[1], "Hello World", NULL, NULL);
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

    Initialize();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        /* Poll for and process events */
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("PolyPaint Point Handler");
        ImGui::Text("PolyPaint Manager");
        ImGui::Spacing();
       
        ImGui::Text("PolyGone Manager");
        if (!PolyEdit) {
            if (ImGui::Button("Add point to the polygone")) {
                SwitchEdit();
            }
                
        }
        else {
            if (ImGui::Button("Stop adding point to the polygone"))
            {
                SwitchEdit();
            }
               
        }

        ImGui::Spacing();
        ImGui::Text("Window Manager");
        if (!WindoEdit) {
            if (ImGui::Button("Add point to the Windo")) {
                SwitchEdit();
            }
               
        }
        else {
            if (ImGui::Button("Stop adding point to the Windo")) {
                SwitchEdit();
            }
               
        }

        ImGui::NewLine();
        ImGui::Spacing();
        
        ImGui::Text("Clip polygone");
        if (ImGui::Button("Clip Sutherland")) {
            lstPointsFenetre.push_back(lstPointsFenetre[0]);
            lstPointsFenetre.push_back(lstPointsFenetre[1]);
            lstPoints = SutherLandHodman(lstPoints, lstPointsFenetre);
           
        }
        ImGui::Spacing();

        if (ImGui::Button("Clip Cyrus Beck")) {
            GenerateCyrusBeck();
        }

        
        ImGui::Spacing();
        if (ImGui::Button("Reset Point")) {
            lstPoints = std::vector<float>();
            lstPointsFenetre = std::vector<float>();
        }
   
        ImGui::NewLine();
        ImGui::Text("Window");
        ImGui::Spacing();
        if (ImGui::Button("Remove")) {
          
            lstPointsFenetre = std::vector<float>();
        }

       
        ImGui::SetWindowSize(ImVec2(300, 500));
     
        ImGui::End();


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