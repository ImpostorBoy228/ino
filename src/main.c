#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void printData(unsigned char* data, long dataSize) {
    for (long i = 0; i < (dataSize); i++) {
        printf("%02X ", data[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

int main(void)
{
    GLFWwindow* window;


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "♂♂♂BOSS OF THIS GYM♂♂♂", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // texture create
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // file <stdio> open
    FILE *image;
    image = fopen("output.bmp", "rb");
    if (!image) {
        printf("Error opening file!\n");
        return 1;
    }


    fseek(image, 0, SEEK_END);

    long fileSize = ftell(image);
    long dataSize = fileSize - 54;
    if (dataSize < 0) dataSize = 0;

    rewind(image);
    // header stuff
    unsigned char header[54];
    fread(header, 1, 54, image);

    // info vars
    int width;
    int height;
    short bits;

    memcpy(&width, &header[18], 4);
    memcpy(&height, &header[22], 4);
    memcpy(&bits, &header[28], 2);

    // body stuff
    unsigned char* body = malloc(dataSize);

    if (body) {
        fread(body, 1, dataSize, image);
    }

    unsigned char* pixels = malloc(width * height * 3);
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            int index = (y * width + x) * 3;
            // BMP: BGR
            unsigned char b = body[index];
            unsigned char g = body[index + 1];
            unsigned char r = body[index + 2];
            // Наш буфер: RGB
            pixels[index] = r;
            pixels[index + 1] = g;
            pixels[index + 2] = b;
        }
    }



    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, body);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        /* Render here */
        //

//        glEnable(GL_TEXTURE_2D);
//        glBindTexture(GL_TEXTURE_2D, texture);
//        glBegin(GL_QUADS);
//            glTexCoord2f(0, 0); glVertex2f(-1, -1);
//            glTexCoord2f(1, 0); glVertex2f(1, -1);
//            glTexCoord2f(1, 1); glVertex2f(1, 1);
//            glTexCoord2f(0, 1); glVertex2f(-1, 1);
//        glEnd();
//        glDisable(GL_TEXTURE_2D);

        glRasterPos2f(-1, -1);  // позиция левого нижнего угла
        glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

        glfwSwapBuffers(window);
    }

    printData(body, dataSize);

    glfwTerminate();
    free(body);
    fclose(image);
    return 0;
}
