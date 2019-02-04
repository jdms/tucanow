#include <string>
#include <vector>

#include "main_window.hpp"
#include "sphere.hpp"


int main()
{
    // Set mesh to be visualized
    std::vector<float> vertices1, vertices2, normals1, normals2;
    std::vector<unsigned int> triangles1, triangles2;

    Sphere s1, s2; 
    s1.setCenter(.5f, .5f, .5f);
    s1.setRadius(1./4.f);
    s1.getMesh(vertices1, triangles1, normals1);
    size_t num_elements = vertices1.size()/3;

    s2.setCenter(-1./2., -1./2., -1./2.);
    s2.setRadius(1./4.);
    s2.getMesh(vertices2, triangles2, normals2);

    std::copy(vertices2.begin(), vertices2.end(), std::back_inserter(vertices1));
    std::copy(normals2.begin(), normals2.end(), std::back_inserter(normals1));

    /* std::cout << "\nNew indices (= old + " << num_elements << "):\n"; */
    for ( size_t i = 0; i < triangles2.size(); ++i )
    {
        triangles1.push_back( triangles2[i] + num_elements );
        /* std::cout << triangles2[i] << " --> " << triangles1.back() << "\n"; */
    }

    // Create main window
    auto& main_window = MainWindow::Get();

    main_window.setMesh(vertices1, triangles1, normals1);
    main_window.setAssetsDir("./samples/assets/");

    const int window_width = 800;
    const int window_height = 800;
    std::string window_title = "A Simple Mesh Viewer";

    return main_window.run(window_width, window_height, window_title);
}
