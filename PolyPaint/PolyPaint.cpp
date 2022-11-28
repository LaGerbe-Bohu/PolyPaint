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
bool selectFill = false;
bool selectInstanFill = false;
bool RecFill = false;

std::stack<int*> pile;
bool fill = false;
Polygone Poly;
Polygone Wind;


std::vector<float> lstofPixel;
float *pixelScreens;

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


void Initialize()
{

    lstPoints = std::vector<float>();
    lstPointsFenetre = std::vector<float>();
    lstofPixel = std::vector<float>();
    mousePos = new float[2];
   
    Poly = Polygone();
    Wind = Polygone();

    blank.push_back(-10);
    blank.push_back(-10);

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

            if (RecFill) {
                pixelScreens = FillFormConex((int)mousePos[0], (int)Screen[1] - (int)mousePos[1], (int)Screen[0], (int)Screen[1], pixelScreens);
                loadTexture(0, pixelScreens, Screen[0], (int)Screen[1]);
                RecFill = false;
            }
            else if (selectFill) {
                pile.push(new int[2] {(int)mousePos[0], (int)Screen[1] - (int)mousePos[1]});
                fill = true;
                selectFill = false;
            }
            else if (selectInstanFill) {
                pixelScreens = FillStack((int)mousePos[0], (int)Screen[1] - (int)mousePos[1], (int)Screen[0], (int)Screen[1], pixelScreens);
                loadTexture(0, pixelScreens, Screen[0], (int)Screen[1]);
                selectInstanFill = false;
            }
            else if (!Fenetrage && PolyEdit) {
                
                Poly.push_back(mousePos);

                lstPoints.push_back(mousePos[0]);
                lstPoints.push_back(mousePos[1]);

               
            }
            else if(WindoEdit)
            {
               
                Wind.push_back(mousePos);

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


    std::vector<float> tempVector = Poly.getFlatVector();
   
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

    const auto loc_float_texture = glGetUniformLocation(
        sh_Triangle, "float_texture"
    );

    glUniform1i(loc_float_texture,0);
   


    glDrawArrays(GL_QUADS, 0, 4);
}


void SwitchEdit() {

    PolyEdit = !PolyEdit;
    WindoEdit = !WindoEdit;
}





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
       
    

        glfwSetMouseButtonCallback(window, mouse_button_callback);
        /* Poll for and process events */
        glfwPollEvents();

      //  std::cout << mousePos[0] << " " << mousePos[1] << std::endl;


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
   


   /*     ImGui::ShowDemoWindow();
       
       ;*/
       // ImGui::ShowStyleEditor();

        ImGui::SetNextWindowPos(ImVec2(30.0, 30.0));

        ImGui::Begin("PolyPaint Point Handler");
        ImGui::Text("PolyPaint Manager");
        ImGui::Spacing();
       
        ImGui::Text("PolyGone Manager");

        ImVec4* colors = ImGui::GetStyle().Colors;
        ImVec4 basicButton = colors[ImGuiCol_Button];

      
        colors[ImGuiCol_Button] = basicButton;
        if (!PolyEdit) {
           
            if (ImGui::Button("Add point to the polygone") && !selectFill) {
                SwitchEdit();
            }
                
        }
        else {
            colors[ImGuiCol_Button] = ImVec4(0.00f, 1.0, 128.0 / 255.0, 0.5);
            if (ImGui::Button("Stop adding point to the polygone") )
            {
                SwitchEdit();
            }
               
        }

        
        ImGui::Spacing();
        colors[ImGuiCol_Button] = ImVec4(0.00f, 1.0, 128.0 / 255.0, 0.5);
        if (Poly.getPoints().size() > 0 && ImGui::Button("Reset Pollygone")) {
         
            Poly.setPoints(std::vector<float*>());

        }
        colors[ImGuiCol_Button] = basicButton;

     

        ImGui::Spacing();
        ImGui::Text("Window Manager");
  
        colors[ImGuiCol_Button] = basicButton;
        if (!WindoEdit) {
          
            if (ImGui::Button("Add point to the Windo")) {
                SwitchEdit();
            }
      
           


        }
        else {
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




        colors[ImGuiCol_Button] = basicButton;

        ImGui::NewLine();
        ImGui::Spacing();
        
        if (Wind.getPoints().size() >= 3 && Poly.getPoints().size() >= 3) {

       

            ImGui::Text("Clip polygone");
            if ( ImGui::Button("Clip Sutherland")) {
               // lstPointsFenetre.push_back(lstPointsFenetre[0]);
                //lstPointsFenetre.push_back(lstPointsFenetre[1]);
            
               // lstPoints = SutherLandHodman(lstPoints, lstPointsFenetre);
                Wind.push_back(Wind[0]);
                Poly = SutherLandHodman(Poly,Wind);
            }
            ImGui::Spacing();

            if (ImGui::Button("Clip Cyrus Beck") ) {
       
           
                std::vector<float> d = GenerateCyrusBeck(Wind.getFlatVector(),Poly.getFlatVector() );
                std::vector<float*> tmp;

                for (int i = 0; i <= d.size()-2; i+=2) {
                    tmp.push_back(new float[2] {d[i],d[i+1]});
                }

                Poly.setPoints(tmp);


            }
        }
        
      
   
          
 
        ImGui::SetWindowSize(ImVec2(300, 250));
     


        ImGui::End();

       
        ImGui::SetNextWindowPos(ImVec2(30.0,(250+30)+30));
     

        ImGui::Begin("Remplissage");


  

        ImGui::Spacing();


        colors[ImGuiCol_Button] = basicButton;
        if (selectFill) {
            colors[ImGuiCol_Button] = ImVec4(214 / 255.0, 34 / 255.0, 61 / 255.0, 1);
        }
      

        
        ImGui::Text("Pixel Fill");
        ImGui::Spacing();

        if (ImGui::Button("Fill")) {
           
            selectFill = !selectFill;
            selectInstanFill = false;
            RecFill = false;
        }


        
     
        colors[ImGuiCol_Button] = basicButton;
        if (selectInstanFill) {
            colors[ImGuiCol_Button] = ImVec4(219 / 255.0, 202 / 255.0, 46 / 255.0, .7);
        }


        if (ImGui::Button(" InstantFill")) {

            selectInstanFill = !selectInstanFill;

            selectFill = false;
            RecFill = false;
            
        }

        colors[ImGuiCol_Button] = basicButton;
        if (RecFill) {
            colors[ImGuiCol_Button] = ImVec4(0.00f, 1.0, 128.0 / 255.0, 0.5);
        }


        if (ImGui::Button(" RecFill")) {

            RecFill = !RecFill;
            selectFill = false;
            selectInstanFill = false;
        }


        colors[ImGuiCol_Button] = basicButton;


        ImGui::NewLine();
        ImGui::Text("Reset Fill");
        ImGui::Spacing();
        if (ImGui::Button("Reset Fill")) {



            for (int i = 0; i < Screen[0]; i++) {
                for (int j = 0; j < Screen[1]; j++) {


                    pixelScreens[((int)Screen[0] * j) + i] = 0.0;
                   

                }
            }

            loadTexture(0, pixelScreens, Screen[0], Screen[1]);

        }


        if (fill && pile.size() > 0) {
           
            pixelScreens = FillStackUpdate((int)Screen[0], (int)Screen[1], pixelScreens, pile);
         //   std::cout << getPixelColor(600, 600, Screen[0], Screen[1], pixelScreens) << std::endl;
            
            if (pile.size() % 10 == 0) {
                loadTexture(0, pixelScreens, Screen[0], Screen[1]);
            }
           
        
        }
       
      


        ImGui::SetWindowSize(ImVec2(300, 150));
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(30.0, ( (400) + 90 )));


        ImGui::ShowDebugLogWindow(new bool[1]{true});

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