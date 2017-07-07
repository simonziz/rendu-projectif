
/** TP 5ETI IMI- CPE Lyon - 2015-2016 */



#include "lib/common/error_handling.hpp"

#include "discrete/bresenham.hpp"
#include "discrete/line_discrete.hpp"
#include "discrete/line_interpolation_parameter.hpp"
#include "image/image.hpp"
#include "image/drawer.hpp"
#include "discrete/triangle_scanline.hpp"
#include "image/image_zbuffer.hpp"
#include "lib/mesh/mesh.hpp"
#include "lib/mesh/mesh_io.hpp"
#include "render_engine/render_engine.hpp"
#include "lib/3d/mat4.hpp"
#include "lib/3d/mat3.hpp"
#include "image/texture.hpp"

#include <iostream>
#include <fstream>
#include <cmath>



using namespace cpe;

int main(int argc,char *argv[])
{

    std::cout<<"***********************"<<std::endl;
    std::cout<<"run "<<argv[0]<<" with "<<argc-1<<" parameters ... \n"<<std::endl;


    try
    {
        //int const Nx = 40;
        //int const Ny = 25;

        int const Nx = 1000;
        int const Ny = 800;

        image im(Nx,Ny);
        im.fill({1.0f,1.0f,1.0f});

        //im(20,10) = color(1.0f,1.0f,0.0f);
        //im(21,10) = color(1.0f,0.0f,0.0f);

        /// Tracé d'une ligne verticale, dégradée du noir au blanc
        /*for(int k=0;k<20;++k)
            //im(10+k,5) = color(k/20.0f,0.0f,1.0f-k/20.0f);
            im(5,3+k) = color(k/22.0f,k/22.0f,k/22.0f);*/

        //std::cout<<im(91,10)<<std::endl;

        line_discrete line; // creation d’une ligne discrete
        /*line.push_back(ivec2(4,5)); // ajout d’une position
        line.push_back({5,6}); // autre methode d’ajout
        ivec2 const p(6,7);
        line.push_back(p); // autre methode d’ajout
        std::cout<<line<<std::endl; // affichage du contenu de la ligne*/

        /*//premiere methode de parcours de la ligne
        for(int k=0;k<line.size();++k)
            std::cout<<line[k]<<" ";
        std::cout<<std::endl;

        //deuxieme methode de parcours de la ligne
        for(ivec2 const& p : line)
            std::cout<<p<<" ";
        std::cout<<std::endl;*/

       /* ivec2 p0 = {350,10};
        ivec2 p1 = {10,400};
        ivec2 p2 = {900,750};*/
        color c(0,0,0);
        color c1(1,0,0);
        color c2(0,1,0);
        color c3(0,0,1);
        /*float z0 = 0;
        float z1 = -0.9;
        float z2 = -0.9;*/
        image_zbuffer zbuffer(Nx,Ny);

        //bresenham({5,5},{12,9},line);
        //std::cout<<line<<std::endl;
        //draw_line(im,p0,p1,c);


        //draw_line(im,p0,p1,c,c1);

        //line_interpolation_parameter interpolation(line2);
        //std::cout<<interpolation<<std::endl;

        //auto scanline = triangle_scanline_factory({15,12}, {2,17}, {6,3}, color(1,0,0), color(0,1,0), color(0,0,1));
        //std::cout<<scanline<<std::endl;

        //draw_triangle_wireframe(im,p0,p1,p2,c1);  //contours du triangle
        //draw_triangle(im,p0,p1,p2,c1);  // triangle plein d'une seule couleur
        //draw_triangle(im,p0,p1,p2,c1,c2,c3); //triangle plein avec interpolation des couleurs
        //draw_triangle(im, zbuffer, p0, p1, p2, c1, c2, c3, z0, z1, z2);

        // Construction d'un deuxieme triangle, plus loin que le premier,
        // pour verifier qu'il se place derriere à l'affichage
        /*ivec2 p3 = {800,15};
        ivec2 p4 = {100,750};
        ivec2 p5 = {950,650};*/
        //draw_triangle(im, zbuffer, p3, p4, p5, c1, c, c1, 0.0, 0.9, 0.9);

        /**  ========= Projection ========= */

        mat4 model; //identitee
        mat4 view; //identitee
        mat4 projection; //identitee
        //matrice de projection angle de vue de 60 degres,
        // image de taille carree,
        // sommets visibles entre z=0.1 et z=20.
        //projection.set_projection_perspective(60*M_PI/180.0f,1.0f,0.1f,20.0f);
        //view.set_translation({0,0,-10});

        vec3 a0 = {0.0, -0.9, 0};
        vec3 a1 = {-0.9, 0.1, -0.5};
        vec3 a2 = {0.9, 0.9, 1.0};
        vec3 n0 = normalized(cross(a0-a2,a0-a1));
        render(im, zbuffer, a0, a1, a2, c1, c2, c3, n0, n0, n0, model, view, projection);


        vec3 a3 = {0.8, -0.9, 0.2};
        vec3 a4 = {-0.6, 0.9, 0.5};
        vec3 a5 = {0.9, 0.8,-0.5};
        vec3 n1 = normalized(cross(a3-a4,a3-a5));
        render(im, zbuffer, a3, a4, a5, c1, c1, c1, n1, n1, n1, model, view, projection);


        /**  ========= Maillage ========= */

        //chargement du fichier
        mesh m = load_mesh_file("../data/dino.obj");

        //applique potentiellement une rotation, translation
        // (scaling possible egalement)
        m.transform_apply_rotation({0,1,0},-M_PI/4.0f);
        m.transform_apply_translation({0,0,-1.5f});
        m.fill_color({1,1,1});  //applique la couleur blanche a
                                // l’ensemble des sommets
        m.fill_normal();        //calcul automatique des normales

        texture text;
        text.load("dino.ppm");

        //render(im, zbuffer, m, model, view, projection, text);

        im.save("mon_image.ppm");




    }
    catch(cpe::exception_cpe const& e)
    {


        std::cout<<"**********************************"<<std::endl;
        std::cout<<"**********************************"<<std::endl;
        std::cout<<"Exception found"<<std::endl;
        std::cout<<e.info()<<std::endl;
        std::cout<<"\nSaving backtrace in file backtrace_log.txt"<<std::endl;
        std::cout<<"**********************************"<<std::endl;
        std::cout<<"**********************************"<<std::endl;


        std::ofstream back_file("backtrace_log.txt");
        back_file<<e.info()<<std::endl;
        back_file<<e.info_backtrace();
        back_file.close();

        exit(1);
    }


    std::cout<<"Exit Main"<<std::endl;

    return 0;
}
