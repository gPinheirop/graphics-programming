#include <stdio.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint WIDTH = 800, HEIGHT = 600;

GLuint VAO, VBO, shader;

// 2. passo da linha de montagem =>  sombreamento de vértices
// vertex shader
// baseado no atributo 0 que passamos no Create Triangle determinamos que o shader deve pegar os valores dos vértices e os altera
static const char *vShader = "#version 330 \n layout (location = 0) in vec3 pos; \n void main () { \n gl_Position = vec4(0.4 * pos.x, 0.4 *  pos.y, pos.z, 1.0);}";

// 8. passo da linha de montagem =>  sombreamento de fragmentos
// fragment shader
// Como o sombreador de fragmento usa a saída do sombreador de vértices como base, so precisamos declarar o que vamos modificar nele
// Nesse caso apenas a cor do fragmento
static const char *fShader = "#version 330 \n out vec4 colour; \n void main () {\n colour = vec4(1.0, 0.0, 0.0, 1.0);}";

// 1. passo da linha de montagem => Especialização de vértices
void CreateTriangle()
{
    // Definimos que vamos ter 3 vértices
    GLfloat vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f};

    // Vamos ter um array de vértices e seu id vai ser armazenado no VAO
    // esse ID corresponde a um endereço de memória da GPU que foi reservado para o VAO
    glGenVertexArrays(1, &VAO);
    // Todas as funções que vão tabalhar com um array de vértices de agora em diante vão usar o VAO como base
    glBindVertexArray(VAO);

    // Vamos ter um buffer e seu id Vai ser armazenao no VBO
    // Esse id esta sendo criado dentro do VAO
    glGenBuffers(1, &VBO);
    // Como o VBO pode se juntar a diversos objetos, Precisamos especificar que ele vai ser linkado no array de buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Estabelecemos que vamos alocar todos os nossos vértices no array buffer e que esses valores não vão mudar
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Localização do atributo que estamos criando para qual estamos criando o programa (Layout)
    // de 3 em 3
    // Esses vértices tem valor de float
    // Não vamos normalizar esses valores
    // Como estamos definindo apenas os vértices no nosso array, precisamos pular 0 linhas
    // Vamos ler os atributos começando da posição 0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // Queremos habilitar o buffer que está no layout 0
    glEnableVertexAttribArray(0);

    // Como acabamos de usar o recurso temos que descartá-los
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char *shaderCode, GLenum shaderType)
{
    // Criamos um sombreador do tipo definido
    GLuint theShader = glCreateShader(shaderType);

    // Definimos que vamos um array de ponteiros para Glchar com o tamanho de um objeto
    // E declaramos esse objeto como sendo o código do shader
    const GLchar *theCode[1];
    theCode[0] = shaderCode;

    // Definimos que a largura da string do código do shaders é de um array de um objeto
    // E declaramos que esse objeto equivale ao tamanho da string do código fonte
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);

    // carregamos e compilamos o shader
    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    // tratativa de erros
    GLint result = 0;
    GLchar errorLog[1024] = {0};

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        glGetShaderInfoLog(theShader, 1024, NULL, errorLog);
        printf("Error compiling the %d shader: '%s'\n", shaderType, errorLog);
        return;
    }
    // acoplamos o código compilado à variável global de sombreamento
    glAttachShader(theProgram, theShader);
}

void CompileShaders()
{
    shader = glCreateProgram();

    if (!shader)
    {
        printf("Erro na criação do shader chefe \n");
        return;
    }

    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar errorLog[1024] = {0};

    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(errorLog), NULL, errorLog);
        printf("Error linking program: '%s' \n", errorLog);
        return;
    }

    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

    if (!result)
    {
        glGetProgramInfoLog(shader, sizeof(errorLog), NULL, errorLog);
        printf("Error validating program: '%s' \n", errorLog);
        return;
    }
}

int main()
{
    if (!glfwInit())
    {
        printf("GLFW deu ruim fio");
        glfwTerminate();
        return 1;
    }
    // setup versions 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // core profile = no backwards compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // window
    GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test window", NULL, NULL);

    if (!mainWindow)
    {
        printf("GLFW window creation deu ruim cria");
        glfwTerminate();
        return 1;
    }

    // Get the buffer size information
    int bufferWidht, bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidht, &bufferHeight);

    // Set context for glew to use
    glfwMakeContextCurrent(mainWindow);

    // allow modern extensions features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW deu ruim");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }

    // setup viewport size
    glViewport(0, 0, bufferWidht, bufferHeight);

    CreateTriangle();
    CompileShaders();

    // Loop until window close

    while (!glfwWindowShouldClose(mainWindow))
    {
        //  get + handle user input
        glfwPollEvents();

        // clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0);

        glUseProgram(0);

        glfwSwapBuffers(mainWindow);
    }

    return 0;
}